//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "PKUnity_Scene.h"
#include "RuntimeManager.h"
#include "FxEffect.h"
#include "FxRendererSubView.h"
#include "NativeToManaged.h"

#include "ImplemGraphicsAPI/RenderAPI_Data.h"

#include <pk_particles_toolbox/include/pt_transforms.h>
#include <pk_particles/include/ps_effect.h>
#include <pk_particles/include/ps_system.h>
#include <pk_particles/include/ps_stats.h>
#include <pk_particles/include/ps_lod.h>
#include <pk_geometrics/include/ge_mesh_kdtree.h>
#include <pk_maths/include/pk_maths_primitives_plane.h>	// used for basic collisions against a ground plane
#include <pk_imaging/include/im_image.h>
#include <pk_kernel/include/kr_streams_memory.h>
#include <pk_kernel/include/kr_refcounted_buffer.h>
#include <pk_kernel/include/kr_caps_cpu.h>

#include <pk_particles/include/ps_descriptor.h>
#include <pk_kernel/include/kr_thread_pool_details.h>

extern volatile u32	g_UpdateId;

__PK_API_BEGIN

//----------------------------------------------------------------------------

CPKFXScene::CPKFXScene()
:	m_IsMediumCollectionInitialized(false)
,	m_ParticleMediumCollection(null)
,	m_ParticleFrameCollector(null)
,	m_GameThreadMediumCollection(null)
,	m_GameThreadFrameCollector(null)
,	m_UpdateMode(UpdateMode_Standard)
,	m_EnableRaycastCollisions(false)
,	m_IsSingleThreaded(false)
,	m_WaitForUpdateOnRenderThread(false)
,	m_GameThreadCalled(false)
,	m_RenderThreadRegistered(false)
{
	PK_ASSERT(CRuntimeManager::IsInstanceInitialized());
}

//----------------------------------------------------------------------------

CPKFXScene::~CPKFXScene()
{
	_ClearEvents();
	PK_SAFE_DELETE(m_ParticleMediumCollection);
	PK_SAFE_DELETE(m_GameThreadMediumCollection);
	PK_SAFE_DELETE(m_GameThreadFrameCollector);
	PK_SAFE_DELETE(m_ParticleFrameCollector);
}

//----------------------------------------------------------------------------

bool	CPKFXScene::InitializeInstanceIFN(const SPopcornFxSettings *settings)
{
	if (settings != null)
	{
		return PopcornFXChangeSettings(*settings);
	}
	else
	{
		return _ResetParticleMediumCollections();
	}
}

//----------------------------------------------------------------------------

bool	CPKFXScene::UnityAPIDeviceIsInitialized(UnityGfxRenderer deviceType)
{
	if (!PK_VERIFY(m_ParticleFrameCollector != null))
	{
		CLog::Log(PK_ERROR, "CPKFXScene::UnityAPIDeviceIsInitialized: ParticleFrameCollector is NULL");
		return false;
	}
	m_RenderContext.m_RenderApiData = IRenderAPIData::GetRenderAPISpecificData(deviceType);
	if (m_RenderContext.m_RenderApiData != null)
		m_RenderContext.m_RenderApiData->LoadContextFunctions();
	return m_RenderContext.m_RenderApiData != null;
}

//----------------------------------------------------------------------------

bool	CPKFXScene::PopcornFXChangeSettings(const SPopcornFxSettings &settings)
{
	// Single threaded:
	m_IsSingleThreaded = (settings.m_SingleThreadedExecution == ManagedBool_True);
	m_EnableRaycastCollisions = (settings.m_EnableRaycastForCollisions == ManagedBool_True);
	m_UnityVersion = settings.m_UnityVersion;
	if (m_EnableRaycastCollisions)
	{
		if (settings.m_UnityVersion <= 2021)
		{
			if (settings.m_RaycastCommandSize != sizeof(RaycastCommand))
			{
				CLog::Log(PK_ERROR, "CPKFXScene::PopcornFXChangeSettings: The size of the RaycastCommand struct doesn't match. Did the layout changed? Disabling EnableRaycastCollisions.");
				m_EnableRaycastCollisions = false;
			}

			if (settings.m_RaycastHitSize != sizeof(RaycastHit))
			{
				CLog::Log(PK_ERROR, "CPKFXScene::PopcornFXChangeSettings: The size of the RaycastHit struct doesn't match. Did the layout changed? Disabling EnableRaycastCollisions.");
				m_EnableRaycastCollisions = false;
			}
		}
		else
		{
			if (settings.m_RaycastCommandSize != sizeof(RaycastCommand2022orNewer))
			{
				CLog::Log(PK_ERROR, "CPKFXScene::PopcornFXChangeSettings: The size of the RaycastCommand struct doesn't match. Did the layout changed? Disabling EnableRaycastCollisions.");
				m_EnableRaycastCollisions = false;
			}

			if (settings.m_RaycastHitSize != sizeof(RaycastHit2022orNewer))
			{
				CLog::Log(PK_ERROR, "CPKFXScene::PopcornFXChangeSettings: The size of the RaycastHit struct doesn't match. Did the layout changed? Disabling EnableRaycastCollisions.");
				m_EnableRaycastCollisions = false;
			}
		}
		if (settings.m_UnityVersion <= 2021)
		{
			if (settings.m_SpherecastCommandSize != sizeof(SphereCastCommand))
			{
				CLog::Log(PK_ERROR, "CPKFXScene::PopcornFXChangeSettings: The size of the SpherecastCommand struct doesn't match. Did the layout changed? Disabling EnableRaycastCollisions.");
				m_EnableRaycastCollisions = false;
			}

			if (settings.m_SpherecastHitSize != sizeof(SphereCastHit))
			{
				CLog::Log(PK_ERROR, "CPKFXScene::PopcornFXChangeSettings: The size of the SpherecastHit struct doesn't match. Did the layout changed? Disabling EnableRaycastCollisions.");
				m_EnableRaycastCollisions = false;
			}
		}
		else
		{
				if (settings.m_SpherecastCommandSize != sizeof(SphereCastCommand2022orNewer))
				{
					CLog::Log(PK_ERROR, "CPKFXScene::PopcornFXChangeSettings: The size of the SpherecastCommand struct doesn't match. Did the layout changed? Disabling EnableRaycastCollisions.");
					m_EnableRaycastCollisions = false;
				}

				if (settings.m_SpherecastHitSize != sizeof(SphereCastHit2022orNewer))
				{
					CLog::Log(PK_ERROR, "CPKFXScene::PopcornFXChangeSettings: The size of the SpherecastHit struct doesn't match. Did the layout changed? Disabling EnableRaycastCollisions.");
					m_EnableRaycastCollisions = false;
				}
		}
	}
	m_WaitForUpdateOnRenderThread = (!m_IsSingleThreaded && !m_EnableRaycastCollisions); //settings.m_SingleThreadedExecution == 0 && settings.m_EnableRaycastForCollisions == 0;

	m_MediumCollectionSettings.m_EnableDynamicEffectBounds = !(settings.m_DisableDynamicEffectBounds == ManagedBool_True);
	m_MediumCollectionSettings.m_EnableLocalizedPages = (settings.m_EnableLocalizedPages == ManagedBool_True);
	m_MediumCollectionSettings.m_EnableLocalizedByDefault = (settings.m_EnableLocalizedByDefault == ManagedBool_True);
	m_MediumCollectionSettings.m_LODMinDist = settings.m_LODMinDistance;
	m_MediumCollectionSettings.m_LODMaxDist = settings.m_LODMaxDistance;
	m_MediumCollectionSettings.m_LODMinMinDist = settings.m_LODMinMinDistance;
	m_MediumCollectionSettings.m_Initialized = true;

	// Reset the medium collections:
	if (!_ResetParticleMediumCollections())
	{
		CLog::Log(PK_ERROR, "CPKFXScene::PopcornFXChangeSettings: ResetParticleMediumCollections failed");
		return false;
	}

	Threads::PAbstractPool	poolAbstract = Scheduler::ThreadPool();
	CWorkerThreadPool		*pool = checked_cast<CWorkerThreadPool*>(poolAbstract.Get());

	if (pool != null)
	{
		if (m_EnableRaycastCollisions && !m_IsSingleThreaded)
		{
			const u32	kExecutionFilter = SimDataState_Scene_Physics; // bitwise combination of ready-states 'ESimulationState'
			// Make sure the main thread processes tasks when waiting, therefore pick up the physics tasks.
			// otherwise, will deadlock because UpdateFence() will wait forever for tasks that will never get picked by anyone.
			pool->SetWaitsProcessesTasks(true);
			pool->BroadcastExecutionFilter(kExecutionFilter); // set all threads to EXCLUDE the tasks calling physics
			pool->WriteExecutionFilter(CRuntimeManager::ExecFilter_BackgroundTask, CCurrentThread::ThreadID()); // set the current thread to not exclude anything but the set sampler
		}
		else if (!m_IsSingleThreaded)
		{
			pool->WriteExecutionFilter(CRuntimeManager::ExecFilter_BackgroundTask, CCurrentThread::ThreadID());
		}
		m_RenderThreadRegistered = false;
	}

	// Split double sided materials:
	m_RenderDataFactory.SetGPUBillboarding(settings.m_EnableGPUBillboarding == ManagedBool_True);

	m_RenderContext.m_FreeUnusedBatches = settings.m_FreeUnusedBatches == ManagedBool_True;
	m_RenderContext.m_FrameCountBeforeFreeingUnusedBatches = settings.m_FrameCountBeforeFreeingUnusedBatches;

	return true;
}

void	CPKFXScene::TransformAllParticles(const CFloat3 &worldOffset)
{
	const ParticleToolbox::SSceneTransformDescriptor	desc(worldOffset);

	if (m_ParticleMediumCollection != null)
		ParticleToolbox::TransformDeferred(m_ParticleMediumCollection, desc);
	
	if (m_GameThreadMediumCollection != null)
		ParticleToolbox::TransformDeferred(m_GameThreadMediumCollection, desc);
}

//----------------------------------------------------------------------------

void	CPKFXScene::SyncPreviousUpdateAndRunDeferredCallsIFN()
{
	if (m_WaitForUpdateOnRenderThread)
			m_ParticleMediumCollection->UpdateFence();
	// Now that we are sure that the update of the previous frame is over, we can call all the deferred callbacks on the game thread:
	// Deferred Unity callbacks:
	CRuntimeManager::Instance().ExecDelayedManagedToNativeMethods();
	CRuntimeManager::Instance().ExecDelayedNativeToManagedMethods();
	CRuntimeManager::Instance().ExecDelayedManagedToNativeMethods();
}

//----------------------------------------------------------------------------

bool	CPKFXScene::ShouldUpdatePopcorn()
{
	if (!m_WaitForUpdateOnRenderThread)
	{
		PK_ASSERT(m_ParticleMediumCollection != null);
		if (m_ParticleMediumCollection->CanSkipUpdate())
			return false;
	}
	else
	{
		PK_ASSERT(m_GameThreadMediumCollection != null);
		PK_ASSERT(m_ParticleMediumCollection != null);

		if (m_ParticleMediumCollection->CanSkipUpdate() &&
			m_GameThreadMediumCollection->CanSkipUpdate())
			return false;
	}

	return true;
}

//----------------------------------------------------------------------------

void	CPKFXScene::LaunchUpdate(float dt)
{
	PK_SCOPEDPROFILE();
#if		defined(PK_DURANGO)
	PK_TODO("Gore hack for Durango. revise the sample lib locks.");
	PK_SCOPEDLOCK(m_test);
#endif

#if	(PK_PARTICLES_HAS_STATS != 0)
	CLiveProfiler	&profiler = CRuntimeManager::Instance().GetProfiler();
	profiler.StartFrame();
#endif // (PK_PARTICLES_HAS_STATS != 0)

	//ResetSoundBuffer
	m_TotalSoundParticleCount = 0;
	m_SoundDatas.Clear();

	// ResetDecalBuffer
	m_TotalDecalParticleCount = 0;
	m_DecalDatas.Clear();

	if (UpdateMode() != UpdateMode_NoUpdate)
	{
		if (dt != 0.0f && ShouldUpdatePopcorn())
		{
			CRuntimeManager::Instance().BeforeUpdate();
			CRuntimeManager::Instance().Update(dt);

			if (!m_WaitForUpdateOnRenderThread)
			{
				PK_ASSERT(m_GameThreadMediumCollection == null);
#if	(PK_PARTICLES_HAS_STATS != 0)
				m_ParticleMediumCollection->Stats().Reset();
				PopcornFX::CTimer	updateTimer;
				updateTimer.Start();
#endif // (PK_PARTICLES_HAS_STATS != 0)

				m_ParticleMediumCollection->Update(dt);

				while (m_ParticleMediumCollection->UpdateFence(1)) { }
#if	(PK_PARTICLES_HAS_STATS != 0)
				float bbMediumCollectionTime = (float)updateTimer.Stop();
				profiler.AddReport("Particles", m_ParticleMediumCollection, bbMediumCollectionTime);
#endif // (PK_PARTICLES_HAS_STATS != 0)


				_PostUpdateEvents();

				if (!m_GameThreadCalled)
				{
					TMemoryView<SUnitySceneView>		views = m_SceneViews;
					SUnityDrawOutputs					dummy;

					if (m_GameThreadFrameCollector->UpdateThread_BeginCollectFrame())
					{
						m_GameThreadFrameCollector->UpdateThread_CollectFrame(m_ParticleMediumCollection);
						m_GameThreadFrameCollector->BuildNewFrame(m_GameThreadFrameCollector->UpdateThread_GetLastCollectedFrame());

						if (m_GameThreadFrameCollector->RenderedFrame() != null)
						{
							_RemoveUnloadedRenderers(m_GameThreadFrameCollector->RenderedFrame());
							m_GameThreadFrameCollector->GenerateDrawCalls(&m_RenderContext, &views, &dummy, true, kAllStepsMask);
						}
					}
					if (m_ParticleFrameCollector->UpdateThread_BeginCollectFrame())
					{
						m_ParticleFrameCollector->UpdateThread_CollectFrame(m_ParticleMediumCollection);
						m_ParticleFrameCollector->BuildNewFrame(m_ParticleFrameCollector->UpdateThread_GetLastCollectedFrame());
						if (m_ParticleFrameCollector->RenderedFrame() != null)
						{
							_RemoveUnloadedRenderers(m_ParticleFrameCollector->RenderedFrame());
							m_ParticleFrameCollector->GenerateDrawCalls(&m_RenderContext, &views, &dummy, true, kGameThreadGenDrawCallsSteps);
						}
						m_RenderDataFactory.CustomStepFlagInactive();
					}
				}
				CRuntimeManager::Instance().AfterUpdate();

#if	(PK_PARTICLES_HAS_STATS != 0)
				profiler.EndFrame();
#endif // (PK_PARTICLES_HAS_STATS != 0)
			}
			else
			{
				_PostUpdateEvents();

				PK_ASSERT(m_GameThreadMediumCollection != null);
#if	(PK_PARTICLES_HAS_STATS != 0)
				m_GameThreadMediumCollection->Stats().Reset();
				PopcornFX::CTimer	meshUpdateTimer;
				meshUpdateTimer.Start();
#endif // (PK_PARTICLES_HAS_STATS != 0)
				// Mesh update and collect frame:
				m_GameThreadMediumCollection->Update(dt);
				m_GameThreadMediumCollection->UpdateFence();
#if	(PK_PARTICLES_HAS_STATS != 0)
				float meshMediumCollectionTime = (float)meshUpdateTimer.Stop();
				profiler.AddReport("Meshes", m_GameThreadMediumCollection, meshMediumCollectionTime);
#endif // (PK_PARTICLES_HAS_STATS != 0)

				TMemoryView<SUnitySceneView>		views = m_SceneViews;
				SUnityDrawOutputs					dummy;

				if (m_GameThreadFrameCollector->UpdateThread_BeginCollectFrame())
				{
					m_GameThreadFrameCollector->UpdateThread_CollectFrame(m_GameThreadMediumCollection);
					m_GameThreadFrameCollector->BuildNewFrame(m_GameThreadFrameCollector->UpdateThread_GetLastCollectedFrame());

					if (m_GameThreadFrameCollector->RenderedFrame() != null)
					{
						_RemoveUnloadedRenderers(m_GameThreadFrameCollector->RenderedFrame());
						m_GameThreadFrameCollector->GenerateDrawCalls(&m_RenderContext, &views, &dummy, true, kAllStepsMask);
					}
				}
#if	(PK_PARTICLES_HAS_STATS != 0)
				m_ParticleMediumCollection->Stats().Reset();
				m_UpdateTimer.Start();
#endif // (PK_PARTICLES_HAS_STATS != 0)
				// Then other particles update:
				m_ParticleMediumCollection->Update(dt);
			}
		}
	}

	// SetSoundBuffer
	::OnSetSoundsBuffer(m_SoundDatas.RawDataPointer(), m_TotalSoundParticleCount);

	// SetDecalBuffer
	::OnSetDecalsBuffer(m_DecalDatas.RawDataPointer(), m_TotalDecalParticleCount);

	m_GameThreadCalled = true;
}

//----------------------------------------------------------------------------

void	CPKFXScene::BuildDrawCalls(const SUnitySceneView &view)
{
	(void)view;
	PK_NAMEDSCOPEDPROFILE("CPKFXScene: BuildDrawCalls");
#if		defined(PK_DURANGO)
	PK_TODO("Gore hack for Durango. revise the sample lib locks.");
	PK_SCOPEDLOCK(m_test);
#endif

	if (!m_RenderContext.m_RenderApiData)
	{
		CLog::Log(PK_ERROR, "CPKFXScene::BuildDrawCalls: RenderApiData is NULL");
		return;
	}
	m_RenderContext.m_RenderApiData->BeginFrame();

	TMemoryView<SUnitySceneView>		views = m_SceneViews;
	SUnityDrawOutputs					dummy;

	if (m_WaitForUpdateOnRenderThread)
	{
#if	(PK_PARTICLES_HAS_STATS != 0)
		CLiveProfiler &profiler = CRuntimeManager::Instance().GetProfiler();
#endif // (PK_PARTICLES_HAS_STATS != 0)
		if (!m_RenderThreadRegistered)
		{
			Threads::PAbstractPool	poolAbstract = Scheduler::ThreadPool();
			CWorkerThreadPool		*pool = checked_cast<CWorkerThreadPool*>(poolAbstract.Get());
			if (pool != null)
			{
				// We want to exclude the background tasks from the UpdateFence done here:
				pool->WriteExecutionFilter(CRuntimeManager::ExecFilter_BackgroundTask, CCurrentThread::ThreadID());
			}
			m_RenderThreadRegistered = true;
		}

		m_ParticleMediumCollection->UpdateFence();

#if	(PK_PARTICLES_HAS_STATS != 0)
		float bbMediumCollectionTime = (float)m_UpdateTimer.Stop();
		profiler.AddReport("Particles", m_ParticleMediumCollection, bbMediumCollectionTime);
#endif // (PK_PARTICLES_HAS_STATS != 0)

		m_ParticleFrameCollector->BuildNewFrame(m_ParticleFrameCollector->UpdateThread_GetLastCollectedFrame());
		if (m_ParticleFrameCollector->RenderedFrame() != null)
		{
			_RemoveUnloadedRenderers(m_ParticleFrameCollector->RenderedFrame());
			m_ParticleFrameCollector->GenerateDrawCalls(&m_RenderContext, &views, &dummy, true, kGameThreadGenDrawCallsSteps);
		}
		m_RenderDataFactory.CustomStepFlagInactive();

#if	(PK_PARTICLES_HAS_STATS != 0)
		profiler.EndFrame();
#endif // (PK_PARTICLES_HAS_STATS != 0)
	}
	if (m_ParticleFrameCollector->RenderedFrame() != null)
	{
		m_ParticleFrameCollector->GenerateDrawCalls(&m_RenderContext, &views, &dummy, true, kRenderThreadGenDrawCallsSteps);
	}
	m_GameThreadCalled = false;

	m_RenderContext.m_RenderApiData->EndFrame();

}

//----------------------------------------------------------------------------

CParticleMediumCollection	*CPKFXScene::GetParticleMediumCollection(bool requiresGameThreadCollect) const
{
	const bool	_requiresGameThreadCollect = m_WaitForUpdateOnRenderThread && requiresGameThreadCollect && !m_EnableRaycastCollisions;
	return _requiresGameThreadCollect ? m_GameThreadMediumCollection: m_ParticleMediumCollection;
}

//----------------------------------------------------------------------------

void	CPKFXScene::Reset()
{
	if (m_ParticleMediumCollection != null)
		m_ParticleMediumCollection->Clear();
	if (m_GameThreadMediumCollection != null)
		m_GameThreadMediumCollection->Clear();
	m_RenderDataFactory.EmptyAllBatches();
}

//----------------------------------------------------------------------------

void	CPKFXScene::HardReset()
{
	_ResetParticleMediumCollections();
	m_RenderDataFactory.EmptyAllBatches();
	m_RenderDataFactory.Reset();
}

//----------------------------------------------------------------------------

void	CPKFXScene::UnsetCollisionMesh()
{
	m_CollisionMesh = null;
	m_CollisionMeshUseTransforms = false;
}

//----------------------------------------------------------------------------

void	CPKFXScene::SetCollisionMesh(const PMeshNew &mesh)
{
	m_CollisionMesh = mesh;
	m_CollisionMeshUseTransforms = false;
}

//----------------------------------------------------------------------------

void	CPKFXScene::RayTracePacket(	const Colliders::STraceFilter &traceFilter,
									const Colliders::SRayPacket &packet,
									const Colliders::STracePacket &results)
{
	// pre-conditions
	PK_ASSERT(packet.m_RayOrigins_Aligned16.Count() == packet.m_RayDirectionsAndLengths_Aligned16.Count());
	PK_ASSERT(results.Count() == packet.m_RayOrigins_Aligned16.Count());

	if (!m_EnableRaycastCollisions)
	{
		PK_NAMEDSCOPEDPROFILE("CParticleSceneInterface: RayTracePacket Collision Mesh");

		if (m_CollisionMesh != null)
		{
			if (m_CollisionMeshUseTransforms)
				m_CollisionMesh->TracePacket(CMeshNew::STraceFlags(), packet, results, &m_CollisionMeshMatrixWithScale, &m_CollisionMeshInvMatrixWithScale, null);
			else
				m_CollisionMesh->TracePacket(CMeshNew::STraceFlags(), packet, results, null);
		}
	}
	else
	{
		PK_NAMEDSCOPEDPROFILE("CParticleSceneInterface: RayTracePacket Dynamic Collision");

		const bool		sphereSweepDisabled = packet.m_RaySweepRadii_Aligned16.Empty();

		if (sphereSweepDisabled)
		{
			void* cmd = null;

			::OnRaycastStart(packet.m_RayOrigins_Aligned16.Count(), &cmd);

			const CFloat4* origins = packet.m_RayOrigins_Aligned16.Data();
			const CFloat4* directions = packet.m_RayDirectionsAndLengths_Aligned16.Data();
			s32				layerMask = traceFilter.m_FilterFlags == 0 ? -5/*Physics.DefaultRaycastLayers*/ : (1 << traceFilter.m_FilterFlags);

			if (m_UnityVersion <= 2021)
			{
				RaycastCommand* cmdBuffer = (RaycastCommand*)cmd;
				for (u32 i = 0; i < packet.m_RayOrigins_Aligned16.Count(); ++i)
				{
					cmdBuffer[i].from = origins[i].xyz();
					cmdBuffer[i].direction = directions[i].xyz();
					cmdBuffer[i].layerMask = layerMask;
					cmdBuffer[i].distance = directions[i].w();
					cmdBuffer[i].maxHits = 1;
				}
			}
			else
			{
				RaycastCommand2022orNewer* cmdBuffer2022orNewer = (RaycastCommand2022orNewer*)cmd;
				for (u32 i = 0; i < packet.m_RayOrigins_Aligned16.Count(); ++i)
				{
					cmdBuffer2022orNewer[i].from = origins[i].xyz();
					cmdBuffer2022orNewer[i].direction = directions[i].xyz();
					cmdBuffer2022orNewer[i].queryParams.layerMask = layerMask;
					cmdBuffer2022orNewer[i].distance = directions[i].w();
				}
			}

			CGuid	currentThreadId = CCurrentThread::ThreadID();

			if (!currentThreadId.Valid()) // Cannot resolve the ray-cast without a threadID
				return;

			void* res = null;

			::OnRaycastPack(&res);



			if (m_UnityVersion <= 2021)
			{
				RaycastHit* resBuffer = (RaycastHit*)res;
				for (u32 i = 0; i < packet.m_RayOrigins_Aligned16.Count(); ++i)
				{
					if (resBuffer[i].collider != 0)
					{
						results.m_HitTimes_Aligned16[i] = resBuffer[i].distance;
						if (results.m_ContactNormals_Aligned16 != null)
							results.m_ContactNormals_Aligned16[i].xyz() = resBuffer[i].normal;
						if (results.m_ContactPoints_Aligned16 != null)
							results.m_ContactPoints_Aligned16[i].xyz() = resBuffer[i].point;

						if (results.m_ContactObjects_Aligned16 != null)
						{
							if (results.m_HitTimes_Aligned16[i] < directions[i].w())
							{
								results.m_ContactObjects_Aligned16[i] = CollidableObject::DEFAULT;
							}
							else
							{
								results.m_ContactObjects_Aligned16[i] = null;
							}
						}
					}
				}
			}
			else
			{
				RaycastHit2022orNewer* resBuffer = (RaycastHit2022orNewer*)res;
				for (u32 i = 0; i < packet.m_RayOrigins_Aligned16.Count(); ++i)
				{
					if (resBuffer[i].collider != 0)
					{
						results.m_HitTimes_Aligned16[i] = resBuffer[i].distance;
						if (results.m_ContactNormals_Aligned16 != null)
							results.m_ContactNormals_Aligned16[i].xyz() = resBuffer[i].normal;
						if (results.m_ContactPoints_Aligned16 != null)
							results.m_ContactPoints_Aligned16[i].xyz() = resBuffer[i].point;

						if (results.m_ContactObjects_Aligned16 != null)
						{
							if (results.m_HitTimes_Aligned16[i] < directions[i].w())
							{
								results.m_ContactObjects_Aligned16[i] = CollidableObject::DEFAULT;
							}
							else
							{
								results.m_ContactObjects_Aligned16[i] = null;
							}
						}
					}
				}
			}
		}
		else
		{
			void* cmd = null;

			::OnSpherecastStart(packet.m_RayOrigins_Aligned16.Count(), &cmd);

			const CFloat4	*origins = packet.m_RayOrigins_Aligned16.Data();
			const CFloat4	*directions = packet.m_RayDirectionsAndLengths_Aligned16.Data();
			const float		*radii = packet.m_RaySweepRadii_Aligned16.Data();
			s32				layerMask = traceFilter.m_FilterFlags == 0 ? -5/*Physics.DefaultRaycastLayers*/ : (1 << traceFilter.m_FilterFlags);

			if (m_UnityVersion <= 2021)
			{
				SphereCastCommand	*cmdBuffer = (SphereCastCommand*)cmd;
				for (u32 i = 0; i < packet.m_RayOrigins_Aligned16.Count(); ++i)
				{
					cmdBuffer[i].from = origins[i].xyz();
					cmdBuffer[i].direction = directions[i].xyz();
					cmdBuffer[i].layerMask = layerMask;
					cmdBuffer[i].distance = directions[i].w();
					cmdBuffer[i].radius = radii[i];
					cmdBuffer[i].maxHit = 1;
				}
			}
			else
			{
				SphereCastCommand2022orNewer	*cmdBuffer = (SphereCastCommand2022orNewer*)cmd;
				for (u32 i = 0; i < packet.m_RayOrigins_Aligned16.Count(); ++i)
				{
					cmdBuffer[i].from = origins[i].xyz();
					cmdBuffer[i].direction = directions[i].xyz();
					cmdBuffer[i].queryParams.layerMask = layerMask;
					cmdBuffer[i].distance = directions[i].w();
					cmdBuffer[i].radius = radii[i];
				}
			}

			CGuid	currentThreadId = CCurrentThread::ThreadID();

			if (!currentThreadId.Valid()) // Cannot resolve the ray-cast without a threadID
				return;

			void	*res = null;

			::OnSpherecastPack(&res);

			SphereCastHit	*resBuffer = (SphereCastHit*)res;
			for (u32 i = 0; i < packet.m_RayOrigins_Aligned16.Count(); ++i)
			{
				if (resBuffer[i].collider != 0)
				{
					results.m_HitTimes_Aligned16[i] = resBuffer[i].distance;
					if (results.m_ContactNormals_Aligned16 != null)
						results.m_ContactNormals_Aligned16[i].xyz() = resBuffer[i].normal;
					if (results.m_ContactPoints_Aligned16 != null)
						results.m_ContactPoints_Aligned16[i].xyz() = resBuffer[i].point;

					if (results.m_ContactObjects_Aligned16 != null)
					{
						if (results.m_HitTimes_Aligned16[i] < directions[i].w())
						{
							results.m_ContactObjects_Aligned16[i] = CollidableObject::DEFAULT;
						}
						else
						{
							results.m_ContactObjects_Aligned16[i] = null;
						}
					}
				}
			}
		}

	}
}

//----------------------------------------------------------------------------

TMemoryView<const float * const>	CPKFXScene::GetAudioSpectrum(CStringId channelGroup, u32 &outBaseCount) const
{
	return CRuntimeManager::Instance().GetSpectrumPyramid(channelGroup, outBaseCount);
}

//----------------------------------------------------------------------------

TMemoryView<const float * const>	CPKFXScene::GetAudioWaveform(CStringId channelGroup, u32 &outBaseCount) const
{
	return CRuntimeManager::Instance().GetWaveformPyramid(channelGroup, outBaseCount);
}

//----------------------------------------------------------------------------

bool	CPKFXScene::GameThread_WaitForUpdateEnd()
{
	// Can get called before update startup (med col creation)
	if (m_WaitForUpdateOnRenderThread && m_ParticleMediumCollection != null)
		m_ParticleMediumCollection->UpdateFence();
	else if (m_GameThreadMediumCollection != null)
		m_GameThreadMediumCollection->UpdateFence();
	return true;
}

//----------------------------------------------------------------------------
// Events
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

// static
CPKFXScene::SPopcornFXPayloadView	CPKFXScene::SPopcornFXPayloadView::Invalid = CPKFXScene::SPopcornFXPayloadView();
//

bool	operator==(const CPKFXScene::SPopcornFXPayload &other, const CStringId &payloadName)
{
	return payloadName == other.m_PayloadName;
}

template <typename _OutType>
void	CPKFXScene::_FillPayload(const PopcornFX::SPayloadElementView &srcPayloadElementData, SPopcornFXPayload &dstPayload)
{
	const u32		valueOffset = dstPayload.m_Values.Count();
	const u32		particleCount = srcPayloadElementData.m_Data.m_Count;
	const u32		particleStride = srcPayloadElementData.m_Data.m_Stride;

	if (!PK_VERIFY(dstPayload.m_Values.Resize(valueOffset + particleCount)))
		return;

	PK_ASSERT(particleStride == 0 || particleStride == sizeof(_OutType)); // runtime should enforce that

	_OutType * __restrict			dstPtr = reinterpret_cast<_OutType*>(&dstPayload.m_Values[valueOffset]);
	const _OutType * __restrict		srcPtr = reinterpret_cast<const _OutType*>(srcPayloadElementData.m_Data.m_RawDataPtr);
	if (particleStride == 0)
	{
		_OutType splat;
		memcpy(&splat, srcPtr, sizeof(_OutType));
		Mem::FillN<sizeof(dstPayload.m_Values[0])>(dstPtr, &splat, particleCount); // efficient mem fill when src is virtual
	}
	else
	{
		const _OutType *dstPtrStop = Mem::AdvanceRawPointer(dstPtr, particleCount * sizeof(dstPayload.m_Values[0]));
		while (dstPtr < dstPtrStop)
		{
			*dstPtr = *srcPtr;
			srcPtr++;
			dstPtr = Mem::AdvanceRawPointer(dstPtr, sizeof(dstPayload.m_Values[0]));
		}
	}
}

//----------------------------------------------------------------------------

void	CPKFXScene::_FillPayloadBool(const PopcornFX::SPayloadElementView &srcPayloadElementData, SPopcornFXPayload &dstPayload, u32 dim)
{
	const u32	valueOffset = dstPayload.m_Values.Count();
	const u32	particleCount = srcPayloadElementData.m_Data.m_Count;
	const u32	particleStride = srcPayloadElementData.m_Data.m_Stride;

	if (!PK_VERIFY(dstPayload.m_Values.Resize(valueOffset + particleCount)))
		return;

	for (u32 iParticle = 0; iParticle < particleCount; ++iParticle)
	{
		SPopcornFXPayloadValue	&value = dstPayload.m_Values[valueOffset + iParticle];
		const u32				*srcPtr = (u32*)&srcPayloadElementData.m_Data.m_RawDataPtr[iParticle * particleStride];

		for (u32 dimIdx = 0; dimIdx < dim; dimIdx++)
			value.m_ValueBool[dimIdx] = (srcPtr[dimIdx] != 0);
	}
}

void	CPKFXScene::_RetrievePayload(const PopcornFX::SPayloadView &srcPayloadView, SPopcornFXPayloadView &dstPayloadView)
{
	dstPayloadView.m_Payloads.Reserve(srcPayloadView.m_PayloadElements.Count());

	// loop on payloads
	const u32	payloadElementsCount = srcPayloadView.m_PayloadElements.Count();
	for (u32 iPayloadElement = 0; iPayloadElement < payloadElementsCount; ++iPayloadElement)
	{
		const PopcornFX::SParticleDeclaration::SEvent::SPayload	&srcPayload = srcPayloadView.m_EventDesc->m_Payload[iPayloadElement];

		// search for existing payload
		PopcornFX::CGuid					payloadIndex = dstPayloadView.m_Payloads.IndexOf(srcPayload.m_NameGUID);

		const EBaseTypeID	payloadElementType = srcPayload.m_Type;

		if (!payloadIndex.Valid()) // new payload
		{
			payloadIndex = dstPayloadView.m_Payloads.PushBack(SPopcornFXPayload(srcPayload.m_NameGUID, payloadElementType));
			if (!PK_VERIFY(payloadIndex.Valid()))
				continue;
		}

		switch (payloadElementType)
		{
		case	BaseType_Bool:
			_FillPayloadBool(srcPayloadView.m_PayloadElements[iPayloadElement], dstPayloadView.m_Payloads[payloadIndex], 1);
			break;
		case	BaseType_Bool2:
			_FillPayloadBool(srcPayloadView.m_PayloadElements[iPayloadElement], dstPayloadView.m_Payloads[payloadIndex], 2);
			break;
		case	BaseType_Bool3:
			_FillPayloadBool(srcPayloadView.m_PayloadElements[iPayloadElement], dstPayloadView.m_Payloads[payloadIndex], 3);
			break;
		case	BaseType_Bool4:
			_FillPayloadBool(srcPayloadView.m_PayloadElements[iPayloadElement], dstPayloadView.m_Payloads[payloadIndex], 4);
			break;
		case	BaseType_Float:
		case	BaseType_I32:
			_FillPayload<s32>(srcPayloadView.m_PayloadElements[iPayloadElement], dstPayloadView.m_Payloads[payloadIndex]);
			break;
		case	BaseType_Float2:
		case	BaseType_Int2:
			_FillPayload<CInt2>(srcPayloadView.m_PayloadElements[iPayloadElement], dstPayloadView.m_Payloads[payloadIndex]);
			break;
		case	BaseType_Float3:
		case	BaseType_Int3:
			_FillPayload<CInt3>(srcPayloadView.m_PayloadElements[iPayloadElement], dstPayloadView.m_Payloads[payloadIndex]);
			break;
		case	BaseType_Float4:
		case	BaseType_Quaternion:
		case	BaseType_Int4:
			_FillPayload<CInt4>(srcPayloadView.m_PayloadElements[iPayloadElement], dstPayloadView.m_Payloads[payloadIndex]);
			break;
		default:
			break;
		}
	}

	// Check that all SPopcornFXPayload::m_Values.Count() are the same for all SPopcornFXPayload
	PK_ONLY_IF_ASSERTS(
		const u32	payloadViewsCount = m_PayloadViews.Count();
		for (u32 iPayloadView = 0; iPayloadView < payloadViewsCount; ++iPayloadView)
		{
			const PopcornFX::TArray<SPopcornFXPayload>	&payloads = m_PayloadViews[iPayloadView].m_Payloads;

			if (payloads.Count() > 0)
			{
				const u32	firstPayloadValuesCount = payloads[0].m_Values.Count();

				const u32	payloadsCount = payloads.Count();
				for (u32 iPayloadIndex = 1; iPayloadIndex < payloadsCount; ++iPayloadIndex)
					PK_ASSERT(firstPayloadValuesCount == payloads[iPayloadIndex].m_Values.Count());
			}
		}
	);
}

void CPKFXScene::_ClearPendingEventsNoLock()
{
	const u32	payloadViewsCount = m_PayloadViews.Count();

	for (u32 iPayloadView = 0; iPayloadView < payloadViewsCount; ++iPayloadView)
	{
		SPopcornFXPayloadView	&payloadView = m_PayloadViews[iPayloadView];

		if (payloadView.Valid())
		{
			payloadView.m_PayloadMedium = null;
			payloadView.m_Payloads.Clear();
			payloadView.m_CurrentParticle = 0;
		}
	}
	m_PendingEventAssocs.Clear();
	u32 iEventCount = m_EventListeners.Count();
	for (u32 iEvent = 0; iEvent < iEventCount; ++iEvent)
	{
		m_EventListeners[iEvent].m_Triggered = false;
	}
}

void	CPKFXScene::_ClearEvents()
{
	PK_SCOPEDLOCK(m_RaiseEventLock);

	m_PayloadViews.Clean();
	m_PendingEventAssocs.Clear();
	m_EventListeners.Clear();
}

void	CPKFXScene::_PostUpdateEvents()
{
	PK_SCOPEDPROFILE();
	PK_ASSERT(CCurrentThread::IsMainThread());

	const u32	elAssocCount = m_PendingEventAssocs.Count();
	for (u32 iAssoc = 0; iAssoc < elAssocCount; ++iAssoc)
	{
		SPopcornFXEventListenerAssoc		&assoc = m_PendingEventAssocs[iAssoc];
		const TArray<CEffectID>				&effectIDs = assoc.m_EffectIDs;
		const u32							eventCount = effectIDs.Count();


		TArray<SPopcornFXPayloadValue>		payloadValues;
		TArray<SPopcornFXPayloadDesc>		payloadDescs;

		if (assoc.m_Event->m_Triggered)
			continue;

		m_CurrentPayloadView = &m_PayloadViews[assoc.m_PayloadViewIndex];

		for (u32 iEvent = 0; iEvent < eventCount; ++iEvent)
		{
			if (assoc.m_Event->m_Triggered)
				continue;
			// execute callbacks that match with effect ID
			const u32	keysCount = assoc.m_Event->m_Keys.Count();

			PK_ASSERT(keysCount == assoc.m_Event->m_EffectIDs.Count());
			for (u32 iKeys = 0; iKeys < keysCount; ++iKeys)
			{
				if (assoc.m_Event->m_Triggered)
					continue;

				CEffectID	effectId = assoc.m_Event->m_EffectIDs[iKeys];

				if (effectId == assoc.m_EffectIDs[iEvent] && !assoc.m_Event->m_Triggered)
				{
					const char						*eventName = assoc.m_Event->m_EventName.ToStringData();
					u32								insertOffset = 0;

					payloadValues.Clear();
					payloadDescs.Clear();

					payloadDescs.Resize(m_CurrentPayloadView->m_Payloads.Count());
					for (u32 iPayloads = 0; iPayloads < m_CurrentPayloadView->m_Payloads.Count(); ++iPayloads)
					{
						const u32				payloadCount = m_CurrentPayloadView->m_Payloads[iPayloads].m_Values.Count();
						SPopcornFXPayloadDesc	&currentPayloadDesc = payloadDescs[iPayloads];

						currentPayloadDesc.m_Name = m_CurrentPayloadView->m_Payloads[iPayloads].m_PayloadName.ToStringData();
						currentPayloadDesc.m_Type = m_CurrentPayloadView->m_Payloads[iPayloads].m_PayloadType;
						currentPayloadDesc.m_Length = payloadCount;
						currentPayloadDesc.m_StartOffSet = payloadValues.Count();

						payloadValues.InsertArray(m_CurrentPayloadView->m_Payloads[iPayloads].m_Values, insertOffset);
						insertOffset += payloadCount;
					}
					OnRaiseEvent(assoc.m_Event->m_EmittersIDs[iKeys], assoc.m_Event->m_Keys[iKeys], eventName, payloadDescs.Count(), payloadDescs.RawDataPointer(), payloadValues.RawDataPointer());
					assoc.m_Event->m_Triggered = true;
				}
			}
			++m_CurrentPayloadView->m_CurrentParticle;
		}
	}
	_ClearPendingEventsNoLock();
}

void	CPKFXScene::_RemoveUnloadedRenderers(const SParticleCollectedFrameToRender *renderedFrame)
{
	for (u32 i = 0; i < renderedFrame->m_PendingDeleteRendererCaches.Count(); ++i)
	{
		const CUnityRendererCache *cache = static_cast<const CUnityRendererCache*>(renderedFrame->m_PendingDeleteRendererCaches[i].Get());
		m_RenderDataFactory.RemoveRendererCache(cache);
	}
}

bool	CPKFXScene::RegisterEventListener(s32 guid, const CStringId &event, u32 unityKey)
{
	CRuntimeManager	&runtimeManager = CRuntimeManager::Instance();
	CPKFXEffect		*fx = runtimeManager.FxGet(guid);

	PK_ASSERT(fx != null);
	if (fx == null)
		return false;

	CEffectID effectId = fx->GetEffectInstance()->EffectID();
	if (!effectId.Valid())
		return false;
	const FastDelegate<CParticleEffect::EventCallback>	broadcastCallback(this, &CPKFXScene::BroadcastEvent);

	if (!fx->RegisterEventCallback(broadcastCallback, event))
	{
		CLog::Log(PK_ERROR, "Register Event \"%s\" Failed", event.ToStringData());
		return false;
	}


	CGuid	eventListenerIndex = m_EventListeners.IndexOf(event);
	if (!eventListenerIndex.Valid())
	{
		// First listener registered for this event, register it.
		eventListenerIndex = m_EventListeners.PushBack(event);
	}
	if (PK_VERIFY(eventListenerIndex.Valid()))
	{
		SPopcornFXEventListener	&eventListener = m_EventListeners[eventListenerIndex];

		if (!PK_VERIFY(eventListener.m_EmittersIDs.PushBack(guid).Valid()) ||
			!PK_VERIFY(eventListener.m_Keys.PushBack(unityKey).Valid()) ||
			!PK_VERIFY(eventListener.m_EffectIDs.PushBack(effectId).Valid()))
			return false;

		PK_ASSERT(eventListener.m_Keys.Count() == eventListener.m_EffectIDs.Count());
		return true;
	}
	return false;
}

bool	CPKFXScene::UnregisterEventListener(s32 guid, const CStringId &event, u32 unityKey)
{
	CRuntimeManager	&runtimeManager = CRuntimeManager::Instance();
	CPKFXEffect		*fx = runtimeManager.FxGet(guid);

	PK_ASSERT(fx != null);
	if (fx == null)
		return false;

	return UnregisterEventListener(fx, event, unityKey);
}

bool	CPKFXScene::UnregisterEventListener(CPKFXEffect* particleEffect, const CStringId &eventNameID, u32 unityKey)
{
	if (m_EventListeners.Empty())
		return false;
	const FastDelegate<CParticleEffect::EventCallback>	broadcastCallback(this, &CPKFXScene::BroadcastEvent);

	particleEffect->UnregisterEventCallback(broadcastCallback, eventNameID);

	const CGuid	eventListenerIndex = m_EventListeners.IndexOf(eventNameID);
	if (eventListenerIndex.Valid())
	{
		SPopcornFXEventListener	&eventListener = m_EventListeners[eventListenerIndex];
		const u32				keyCount = eventListener.m_Keys.Count();

		PK_ASSERT(eventListener.m_Keys.Count() == eventListener.m_EffectIDs.Count());
		for (u32 iKey = 0; iKey < keyCount; ++iKey)
		{
			if (eventListener.m_Keys[iKey] == unityKey)
			{
				PK_ASSERT(particleEffect->GetEffectInstance()->EffectID() == eventListener.m_EffectIDs[iKey]); // Should not happen
				eventListener.m_EmittersIDs.Remove(iKey);
				eventListener.m_Keys.Remove(iKey);
				eventListener.m_EffectIDs.Remove(iKey);
				_ClearPendingEventsNoLock();
				return true;
			}
		}
	}
	return false;
}

void CPKFXScene::UnregisterEffectAllEventsListeners(u32 guid)
{
	CRuntimeManager	&runtimeManager = CRuntimeManager::Instance();
	CPKFXEffect		*fx = runtimeManager.FxGet(guid);

	PK_ASSERT(fx != null);
	if (fx == null)
		return;

	return UnregisterEffectAllEventsListeners(fx);
}

void	CPKFXScene::UnregisterEffectAllEventsListeners(CPKFXEffect* particleEffect)
{
	if (m_EventListeners.Empty())
		return;

	const FastDelegate<CParticleEffect::EventCallback>		broadcastCallback(this, &CPKFXScene::BroadcastEvent);
	const u32												eventListenersCount = m_EventListeners.Count();

	for (u32 iEventListener = 0; iEventListener < eventListenersCount; ++iEventListener)
	{
		SPopcornFXEventListener	&eventListener = m_EventListeners[iEventListener];

		bool callbackUnregistered = false;

		PK_ASSERT(eventListener.m_Keys.Count() == eventListener.m_EffectIDs.Count());
		for (u32 iEffectID = 0; iEffectID < eventListener.m_EffectIDs.Count(); ++iEffectID)
		{
			if (eventListener.m_EffectIDs[iEffectID] == particleEffect->GetEffectInstance()->EffectID())
			{
				if (!callbackUnregistered)
				{
					callbackUnregistered = true;
					particleEffect->UnregisterEventCallback(broadcastCallback, eventListener.m_EventName);
				}
				eventListener.m_EmittersIDs.Remove(iEffectID);
				eventListener.m_Keys.Remove(iEffectID);
				eventListener.m_EffectIDs.Remove(iEffectID);
				--iEffectID;
			}
		}
	}
	_ClearPendingEventsNoLock();
}

void CPKFXScene::UnregisterAllEventsListeners()
{
	_ClearEvents();
}

void	CPKFXScene::BroadcastEvent(	Threads::SThreadContext				*threadCtx,
									CParticleMedium						*parentMedium,
									u32									eventID,
									CStringId							eventName,
									u32									count,
									const SUpdateTimeArgs				&timeArgs,
									const TMemoryView<const float>		&spawnDtToEnd,
									const TMemoryView<const CEffectID>	&effectIDs,
									const SPayloadView					&payloadView)
{
	(void)spawnDtToEnd;
	(void)count;
	(void)timeArgs;
	(void)threadCtx;

	PK_SCOPEDPROFILE();
	const PopcornFX::CGuid	eventListenerIndex = m_EventListeners.IndexOf(eventName);
	// Is an event listener was registered for this specific event, if not, return
	if (!eventListenerIndex.Valid())
		return;

	PK_TODO("Find a way to reduce this lock");
	PK_SCOPEDLOCK(m_RaiseEventLock);

	// search for existing payload view
	PopcornFX::CGuid		payloadViewIndex = PopcornFX::CGuid::INVALID;

	const u32				payloadViewsCount = m_PayloadViews.Count();
	for (u32 iPayloadView = 0; iPayloadView < payloadViewsCount; ++iPayloadView)
	{
		const SPopcornFXPayloadView	&currPayloadView = m_PayloadViews[iPayloadView];
		if (currPayloadView.Valid() && currPayloadView.m_EventID == eventID && currPayloadView.m_PayloadMedium == parentMedium)
		{
			payloadViewIndex = iPayloadView;
			break;
		}
	}

	if (!payloadViewIndex.Valid()) // new payload view
	{
		payloadViewIndex = m_PayloadViews.Insert(SPopcornFXPayloadView(parentMedium, eventID));
		if (!PK_VERIFY(payloadViewIndex.Valid()))
			return;
	}

	_RetrievePayload(payloadView, m_PayloadViews[payloadViewIndex]);

	// create new event
	SPopcornFXEventListenerAssoc	newAssoc(&m_EventListeners[eventListenerIndex], effectIDs, payloadViewIndex);

	if (!PK_VERIFY(m_PendingEventAssocs.PushBack(newAssoc).Valid())) // add event listener to pending array
		return;
}

//----------------------------------------------------------------------------

bool	CPKFXScene::_ResetParticleMediumCollections()
{
	m_IsMediumCollectionInitialized = false;
	if (m_GameThreadMediumCollection && m_GameThreadFrameCollector)
	{
		// Release the collected frame prior to deleting the medium collection
		m_GameThreadFrameCollector->ReleaseRenderedFrame();
		m_GameThreadFrameCollector->UpdateThread_UninstallFromMediumCollection(m_GameThreadMediumCollection);
	}
	if (m_ParticleMediumCollection && m_ParticleFrameCollector)
	{
		m_ParticleFrameCollector->ReleaseRenderedFrame();
		m_ParticleFrameCollector->UpdateThread_UninstallFromMediumCollection(m_ParticleMediumCollection);
	}
	PK_SAFE_DELETE(m_ParticleMediumCollection);
	PK_SAFE_DELETE(m_GameThreadMediumCollection);

	PK_SAFE_DELETE(m_GameThreadFrameCollector);
	PK_SAFE_DELETE(m_ParticleFrameCollector);

	// Remove the camera that are registered in the medium collection:
	m_SceneViews.Clear();

	// Do we need 2 separate medium collections?
	const bool		useMeshMedCol = m_WaitForUpdateOnRenderThread && !m_EnableRaycastCollisions;

	// If so, we need to delay the Unity callbacks to the next frame as the frame will be collected on a worker:
	m_RenderDataFactory.SetDelayUnityCallbacks(useMeshMedCol);

	// Initialize the standard medium collection for billboards and ribbons:
	m_ParticleMediumCollection = PK_NEW(CParticleMediumCollection);
	if (!PK_VERIFY(m_ParticleMediumCollection != null))
		return false;
	m_ParticleMediumCollection->SetScene(this);

	// Initialize the billboards/ribbons frame collector:
	m_ParticleFrameCollector = PK_NEW(CUnityFrameCollector);
	if (!PK_VERIFY(m_ParticleFrameCollector != null))
		return false;

	CUnityFrameCollector::SFrameCollectorInit	init(&m_RenderDataFactory, (1U << Renderer_Billboard) | (1U << Renderer_Ribbon) | (1U << Renderer_Triangle));
	PK_VERIFY(m_ParticleFrameCollector->UpdateThread_Initialize(init));

	// Initialize the meshes frame collector:
	m_GameThreadFrameCollector = PK_NEW(CUnityFrameCollector);
	if (!PK_VERIFY(m_GameThreadFrameCollector != null))
		return false;
	u32	enabledRenderer = (1U << Renderer_Mesh);
	if (CRuntimeManager::Instance().m_PopcornFXRuntimeData->m_LightRenderer)
		enabledRenderer = enabledRenderer | (1U << Renderer_Light);

	if(CRuntimeManager::Instance().m_PopcornFXRuntimeData->m_SoundRenderer)
		enabledRenderer = enabledRenderer | (1U << Renderer_Sound);

	if (CRuntimeManager::Instance().m_PopcornFXRuntimeData->m_DecalRenderer)
		enabledRenderer = enabledRenderer | (1U << Renderer_Decal);

	CUnityFrameCollector::SFrameCollectorInit	init_Meshes(&m_RenderDataFactory, enabledRenderer);
	PK_VERIFY(m_GameThreadFrameCollector->UpdateThread_Initialize(init_Meshes));

	m_ParticleFrameCollector->UpdateThread_InstallToMediumCollection(m_ParticleMediumCollection);

	if (useMeshMedCol)
	{
		// Initialize the mesh med col:
		m_GameThreadMediumCollection = PK_NEW(CParticleMediumCollection);
		if (!PK_VERIFY(m_GameThreadMediumCollection != null))
			return false;
		m_GameThreadMediumCollection->SetScene(this);

		// Add collect frame callback on the last worker thread:
		m_ParticleMediumCollection->m_OnUpdateComplete += FastDelegate<void(CParticleMediumCollection*)>(this, &CPKFXScene::_CollectFrame);

		// Instal the game thread frame collector:
		m_GameThreadFrameCollector->UpdateThread_InstallToMediumCollection(m_GameThreadMediumCollection);
		m_ParticleFrameCollector->UpdateThread_InstallToMediumCollection(m_GameThreadMediumCollection);
	}
	else
	{
		// Instal the game thread frame collector:
		m_GameThreadFrameCollector->UpdateThread_InstallToMediumCollection(m_ParticleMediumCollection);
	}
	if (m_MediumCollectionSettings.m_Initialized)
	{
		// Enable/Disable bounds
		m_ParticleMediumCollection->EnableBounds(m_MediumCollectionSettings.m_EnableDynamicEffectBounds);
		m_ParticleMediumCollection->EnableLocalizedPages(	m_MediumCollectionSettings.m_EnableLocalizedPages,
			m_MediumCollectionSettings.m_EnableLocalizedByDefault);
		if (m_GameThreadMediumCollection != null)
		{
			m_GameThreadMediumCollection->EnableBounds(m_MediumCollectionSettings.m_EnableDynamicEffectBounds);
			m_GameThreadMediumCollection->EnableLocalizedPages(	m_MediumCollectionSettings.m_EnableLocalizedPages,
				m_MediumCollectionSettings.m_EnableLocalizedByDefault);
		}

		// LOD
		LOD::SConfig	lodConfig = m_ParticleMediumCollection->LODConfig();
		lodConfig.m_MinDist = m_MediumCollectionSettings.m_LODMinDist;
		lodConfig.m_MaxDist = m_MediumCollectionSettings.m_LODMaxDist;
		lodConfig.m_MinMinDist = m_MediumCollectionSettings.m_LODMinMinDist;

		m_ParticleMediumCollection->SetLODConfig(&lodConfig);
		if (m_GameThreadMediumCollection != null)
			m_GameThreadMediumCollection->SetLODConfig(&lodConfig);
	}

	m_IsMediumCollectionInitialized = true;
	return true;
}

//----------------------------------------------------------------------------

void	CPKFXScene::_CollectFrame(CParticleMediumCollection *medCol)
{
	CRuntimeManager::Instance().AfterUpdate();
	(void)medCol;
	if (m_ParticleFrameCollector->UpdateThread_BeginCollectFrame())
	{
		m_ParticleFrameCollector->UpdateThread_CollectFrame(m_ParticleMediumCollection);
		m_ParticleFrameCollector->UpdateThread_CollectFrame(m_GameThreadMediumCollection);
	}
}

//----------------------------------------------------------------------------
__PK_API_END
