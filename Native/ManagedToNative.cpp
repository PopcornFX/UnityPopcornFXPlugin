//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "ManagedToNative.h"
#include "NativeToManaged.h"
#include "PKUnity_Scene.h"
#include "PKUnity_Log.h"
#include "PKUnity_FileSystemController.h"
#include "RuntimeManager.h"
#include "FxEffect.h"
#include "FxRendererSubView.h"

#include "UnityGraphicsAPI/IUnityInterface.h"

#if		PK_UNITY_EDITOR
#	include		"EditorOnly/EffectBaking.h"
#	include		"EditorOnly/EditorNativeToManaged.h"
#endif

#include <pk_kernel/include/kr_profiler_details.h>
#include <pk_kernel/include/kr_caps_cpu.h>
#include <pk_kernel/include/kr_streams_memory.h>

#include <cstdio>
#include <pk_version.h>

PK_LOG_MODULE_DECLARE();

extern "C"
{
	SMeshDataToFill::SMeshDataToFill()
	:	m_IdxCount(0)
	,	m_Indices(null)
	,	m_VtxCount(0)
	,	m_Positions(null)
	,	m_Normals(null)
	,	m_Tangents(null)
	,	m_UVs(null)
	,	m_Colors(null)
	,	m_VtxBonesWeights(null)
	,	m_VtxBonesIndices(null)
	,	m_BonesCount(0)
	{
	}

	//----------------------------------------------------------------------------

	SMeshSamplerBaked::SMeshSamplerBaked()
	:	m_PkmmFileContent(null)
	,	m_PkmmFileContentSize(0)
	,	m_SubmeshId(0)
	,	m_IsSkinned(ManagedBool_False)
	{
	}

	//----------------------------------------------------------------------------
	// Globals:
	//----------------------------------------------------------------------------

	CThreadID				g_RenderThreadID;

#if		defined(PK_COMPILER_CLANG) || defined(PK_COMPILER_GCC)
#	pragma GCC visibility push(default)
#endif

	//----------------------------------------------------------------------------
	// Callbacks:
	//----------------------------------------------------------------------------


#if defined(PK_IOS)
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API	PKIOSUnityPluginLoad(IUnityInterfaces *unityInterfaces)
#else
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API	UnityPluginLoad(IUnityInterfaces *unityInterfaces)
#endif
	{
		(void)unityInterfaces;
		if (!PK_VERIFY(CRuntimeManager::InitializeInstanceIFN(null)))
		{
			CLog::Log(PK_ERROR, "InitializeInstanceIFN failed");
			return;
		}
		CLog::Log(PK_INFO, "UnityPluginUnload: Loading PopcornFX Plugin, Initialization success");
		if (unityInterfaces != null)
		{
			if (!PK_VERIFY(CRuntimeManager::Instance().SetUnityInterfaces(unityInterfaces)))
				return;
		}
		IUnityGraphics	*unityGraphics = CRuntimeManager::Instance().GetUnityGraphics();
		if (unityGraphics == null)
			unityGraphics = UNITY_GET_INTERFACE(unityInterfaces, IUnityGraphics);
		if (unityGraphics != null)
		{
			unityGraphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
		}
		// Run OnGraphicsDeviceEvent(initialize) manually on plugin load in case Unity missed it:
		OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
	}


#if defined(PK_IOS)
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API	PKIOSUnityPluginUnload()
#else
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API	UnityPluginUnload()
#endif
	{
		CLog::Log(PK_INFO, "UnityPluginUnload: Unloading PopcornFX Plugin");
		IUnityGraphics* unityGraphics = CRuntimeManager::Instance().GetUnityGraphics();

		unityGraphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);

		PopcornFXShutdown();
	}

	void UNITY_INTERFACE_API	OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
	{
		if (eventType == kUnityGfxDeviceEventShutdown)
		{
			CLog::Log(PK_INFO, "OnGraphicsDeviceEvent: UnityGfxDeviceEventType == kUnityGfxDeviceEventShutdown");
			if (CRuntimeManager::IsInstanceInitialized())
				CRuntimeManager::Instance().UnityAPIDeviceUnInitialize();
		}
		else if (eventType == kUnityGfxDeviceEventInitialize)
		{
			CLog::Log(PK_INFO, "OnGraphicsDeviceEvent: UnityGfxDeviceEventType == kUnityGfxDeviceEventInitialize");
			if (!PK_VERIFY(CRuntimeManager::InitializeInstanceIFN(null)))
			{
				CLog::Log(PK_ERROR, "InitializeInstanceIFN failed");
				return;
			}

			CRuntimeManager::Instance().UnityAPIDeviceInitialize();
		}
	}

#if defined(PK_NX)
	typedef void	(UNITY_INTERFACE_API * PluginLoadFunc)(IUnityInterfaces* unityInterfaces);
	typedef void	(UNITY_INTERFACE_API * PluginUnloadFunc)();

	void	UnityRegisterRenderingPlugin(PluginLoadFunc loadPlugin, PluginUnloadFunc unloadPlugin);

	POPCORN_TO_MANAGED_CONVENTION MANAGED_TO_POPCORN_CONVENTION void	RegisterPlugin()
	{
		UnityRegisterRenderingPlugin(UnityPluginLoad, UnityPluginUnload);
	}
#endif

	MANAGED_TO_POPCORN_CONVENTION UnityRenderingEvent		GetRenderEventFunc()
	{
		return OnRenderEvent;
	}

	//----------------------------------------------------------------------------
	MANAGED_TO_POPCORN_CONVENTION int	PopcornFXDllLoaded()
	{
		return 42;
	}

	MANAGED_TO_POPCORN_CONVENTION bool	PopcornFXStartup(const SPopcornFxSettings *settings)
	{
		if (!PK_VERIFY(settings != null))
			return false;
		PK_SCOPEDPROFILE();
		bool		pluginIsInitialized = CRuntimeManager::IsInstanceInitialized();

		if (pluginIsInitialized)
		{
			// The plugin was already started so we just change the settings:
			if (!PK_VERIFY(CRuntimeManager::Instance().PopcornFXChangeSettings(*settings)))
			{
				CLog::Log(PK_ERROR, "Could not change the PopcornFX settings");
				return false;
			}
		}
		else
		{
			// We start the plugin directly with the correct settings:
			if (!PK_VERIFY(CRuntimeManager::InitializeInstanceIFN(settings)))
			{
				CLog::Log(PK_ERROR, "InitializeInstanceIFN failed");
				return false;
			}
		}
		// Load the effect pack:
		CRuntimeManager::Instance().LoadPack();
		return true;
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION const char	*GetRuntimeVersion()
	{
		NEED_PK_RUNTIME_STARTED(return "Runtime not started");
		PK_SCOPEDPROFILE();

		return PK_VERSION_CURRENT_STRING;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	PopcornFXShutdown()
	{
		NEED_PK_RUNTIME_STARTED(return);

		CLog::Log(PK_INFO, "PopcornFXShutdown");
		DeepReset();
		if (g_RenderThreadID.Valid() && g_RenderThreadID != 0) // If the g_RenderThreadID is the main thread we do not need to unregister it
		{
			CThreadManager::UnsafeUnregisterUserThread(g_RenderThreadID);
			g_RenderThreadID = CThreadID::INVALID;
		}
		ClearAllCallbacks();
		CRuntimeManager::Instance().PopcornFXShutdown();
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	SetMaxLogStack(int maxLogStack)
	{
		NEED_PK_RUNTIME_STARTED(return);

		CRuntimeManager::Instance().SetMaxLogStack(maxLogStack);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION int	UnstackLog(char *dstBuffer, int dstSize, int &logSeverity)
	{
		// DO NOT LOG AIN THIS FUNCTION! INFINITE LOOP...
		if (!CRuntimeManager::IsInstanceInitialized())
			return -1;

		CUnityLog	*logger = CRuntimeManager::Instance().GetLogger();

		if (logger != null)
		{
			CUnityLog::SPopcornLogData	curLog;
			s32							logsLeft = logger->PopFront(curLog);

			if (logsLeft < 0)
				return -1;
			logSeverity = (int)curLog.m_Level;
			strncpy(dstBuffer, curLog.m_Message.Data(), dstSize);
			return logsLeft;
		}
		return -1;
	}


	MANAGED_TO_POPCORN_CONVENTION void	SetMaxCameraCount(int count)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);

		CRuntimeManager::Instance().SetMaxCameraCount(count);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	UpdateCamDesc(int camID, const SCamDesc *desc, ManagedBool update)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		if (!PK_VERIFY(desc != null))
			return;
		PK_SCOPEDPROFILE();


		CRuntimeManager::Instance().UpdateCamDesc(camID, *desc, update == ManagedBool_True);
	}

	// --------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	UpdateParticles(float DT)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		PK_SCOPEDPROFILE();

		const CPU::CScoped_FpuEnableFastMode	_fm;
		const CPU::CScoped_FpuDisableExceptions	_de;

		CRuntimeManager::Instance().GetScene().LaunchUpdate(DT);
	}

	// --------------------------------------------------------------------------

	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API	OnRenderEvent(int renderEvent)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		PK_SCOPEDPROFILE();

		const CPU::CScoped_FpuEnableFastMode	_fm;
		const CPU::CScoped_FpuDisableExceptions	_de;

		g_RenderThreadID = CCurrentThread::RegisterUserThread();

		CRuntimeManager	&manager = CRuntimeManager::Instance();
		CPKFXScene		&scene = manager.GetScene();

		if (scene.RenderThreadIsSet() == false || !scene.IsRenderThread())
			scene.SetRenderThread();

		const u32	event_masked = static_cast<u32>(renderEvent);

		if ((event_masked & 0xFFFF0000) != POPCORN_MAGIC_NUMBER)
			return; // wrong eventId, possibly another native plugin.

		renderEvent = (event_masked &~0xFFFFF000);

		if (renderEvent < 0)
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("Render event is not valid %d" PK_COMMA renderEvent);
			return;
		}

		const u32					camID = renderEvent;
		const TArray<SUnitySceneView>	&sceneViews = scene.SceneViews();

		if (camID >= sceneViews.Count())
		{
			CLog::Log(PK_ERROR, "CamID %d out of bounds (%d). Check for scene reset.", camID, sceneViews.Count());
			return;
		}

		if (camID == 0)
			scene.BuildDrawCalls(sceneViews[camID]);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	SyncParticlesSimulation()
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		PK_SCOPEDPROFILE();

		CRuntimeManager::Instance().GetScene().SyncPreviousUpdateAndRunDeferredCallsIFN();
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	TransformAllParticles(CFloat3 worldOffset)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		PK_SCOPEDPROFILE();

		CRuntimeManager::Instance().GetScene().TransformAllParticles(worldOffset);
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	GetStats(SStats *stats)
	{
		(void)stats;
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		PK_SCOPEDPROFILE();

		//stats->UpdateTime = g_LastUpdateTime;
		//stats->RenderTime = g_LastRenderTime;
		//stats->TotalMemoryFootprint = CMemStats::RealFootprint();
		//// Here we only get the stats for the non-mesh particles
		//stats->TotalParticleMemory = g_Scene->GetParticleMediumCollection(false)->Stats().m_TotalPageMemory;
		//stats->UnusedParticleMemory = g_Scene->GetParticleMediumCollection(false)->Stats().m_TotalUnusedPageMemory;
		//stats->TotalParticleCount = g_Scene->GetParticleMediumCollection(false)->Stats().TotalParticleCount();
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	PreloadFxIFN(const char *path, ManagedBool usesMeshRenderer)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		if (!PK_VERIFY(path != null))
			return;
		PK_SCOPEDPROFILE();

		CPKFXScene	&scene = CRuntimeManager::Instance().GetScene();

		// Needs simulation sync
		if (!PK_VERIFY(scene.GameThread_WaitForUpdateEnd()))
			return;

		CRuntimeManager::Instance().PreloadFxIFN(path, usesMeshRenderer != 0);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION int	InstantiateFx(const SFxDesc *desc)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return -1);
		if (!PK_VERIFY(desc != null))
			return -1;
		PK_SCOPEDPROFILE();

		CPKFXScene	&scene = CRuntimeManager::Instance().GetScene();

		// Needs simulation sync
		if (!PK_VERIFY(scene.GameThread_WaitForUpdateEnd()))
		{
			CLog::Log(PK_ERROR, "GameThread_WaitForUpdateEnd failed...");
			return -1;
		}

		// Delete handled by fxmanager
		CPKFXEffect	*fx = PK_NEW(CPKFXEffect(desc->m_UsesMeshRenderer == ManagedBool_True, desc->m_Transforms));
		if (fx != null)
		{
			const CGuid	fxGUID = fx->LoadFX(desc->m_EffectPath);
			if (!fxGUID.Valid())
			{
				CLog::Log(PK_ERROR, "Could not load the effect %s", desc->m_EffectPath);
				PK_DELETE(fx);
				return -1;
			}
			else
				return fxGUID;
		}
		CLog::Log(PK_ERROR, "Could not allocate the effect %s", desc->m_EffectPath);
		return -1;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	TerminateFx(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().TerminateFx(guid);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	StartFx(int guid, float dt, float prewarm)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().StartFx(guid, dt, prewarm);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	StopFx(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().StopFx(guid);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	KillFx(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().KillFx(guid);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	RegisterExportedEvent(int guid, const char *eventName, unsigned int unityKey)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		CString			eventString = CString(eventName);
		const CStringId	event = CStringId(eventString);

		return CRuntimeManager::Instance().GetScene().RegisterEventListener(guid, event, unityKey);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	UnregisterExportedEvent(int guid, const char *eventName, unsigned int unityKey)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		CString			eventString = CString(eventName);
		const CStringId	event = CStringId(eventString);

		return CRuntimeManager::Instance().GetScene().UnregisterEventListener(guid, event, unityKey);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	UnregisterEffectAllExportedEvent(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		PK_SCOPEDPROFILE();

		CRuntimeManager::Instance().GetScene().UnregisterEffectAllEventsListeners(guid);
	}

	MANAGED_TO_POPCORN_CONVENTION void	UnregisterSceneAllExportedEvent()
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		PK_SCOPEDPROFILE();

		CRuntimeManager::Instance().GetScene().UnregisterAllEventsListeners();
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void		*EffectGetAttributesBuffer(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return null);
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->GetAttributesBuffer();
		}
		return null;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void		EffectUpdateAttributes(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		PK_SCOPEDPROFILE();
		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			effect->UpdateAttributesBuffer();
		}
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool		EffectSetSamplerShapeTransform(int guid, int samplerId, CFloat4x4 transforms)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		CPKFXEffect		*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->SetSamplerShapeTransform(samplerId, transforms);
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION SMeshDataToFill	*GetMeshDataToFillFromSampler(int vertexCount, int indexCount, int bonesCount, int samplerUsage)
	{
		NEED_PK_RUNTIME_STARTED(return null);
		PK_SCOPEDPROFILE();

		if (samplerUsage == 0)
		{
			CLog::Log(PK_ERROR, "Cannot get the sampler mesh data because the sampler usage is empty: please re-import your effect...");
			return null;
		}

		int		vertexAttrib = 0;

		if (samplerUsage & SParticleDeclaration::SSampler::UsageFlags_Mesh_Normal)
			vertexAttrib |= (1 << Attrib_Normal);
		if (samplerUsage & SParticleDeclaration::SSampler::UsageFlags_Mesh_Tangent)
			vertexAttrib |= (1 << Attrib_Tangent);
		if (samplerUsage & SParticleDeclaration::SSampler::UsageFlags_Mesh_UV0)
			vertexAttrib |= (1 << Attrib_Texcoord);
		if (samplerUsage & SParticleDeclaration::SSampler::UsageFlags_Mesh_Color0)
			vertexAttrib |= (1 << Attrib_Color);
		if (bonesCount != 0)
			vertexAttrib |= (1 << Attrib_BonesIndicesAndWeights);
		return CRuntimeManager::CreateMeshDataToFill(vertexCount, indexCount, bonesCount, vertexAttrib);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	EffectSetMeshSampler(int guid, int samplerId, SMeshDataToFill *meshSampler, CFloat3 size, ManagedBool async)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		if (!PK_VERIFY(meshSampler != null))
			return false;
		PK_SCOPEDPROFILE();

		CPKFXEffect		*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			bool	success = false;

			// meshSampler->m_MeshData.m_VtxBonesWeights is freed in the SetSamplerShapeMesh* methods:
			if (async == ManagedBool_True)
			{
				// The thread will take care of freeing the m_VtxBonesWeights - m_VtxBonesIndices
				success = effect->SetSamplerShapeMesh_Async(samplerId, meshSampler, size);
			}
			else
			{
				success = effect->SetSamplerShapeMesh(samplerId, meshSampler, size);
			}
			return success;
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	EffectSetMeshSamplerBaked(int guid, int samplerId, SMeshSamplerBaked *meshSamplerBaked, CFloat3 size, ManagedBool async)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		if (!PK_VERIFY(meshSamplerBaked->m_PkmmFileContent != null))
			return false;
		PK_SCOPEDPROFILE();

		CPKFXEffect		*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			if (async == ManagedBool_True)
				return effect->SetSamplerShapeMeshFromPkmm_Async(samplerId, meshSamplerBaked, size);
			else
				return effect->SetSamplerShapeMeshFromPkmm(samplerId, meshSamplerBaked, size);
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION CFloat4x4							*EffectUpdateSamplerSkinningSetMatrices(int guid, int samplerId)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return null);
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->EffectUpdateSamplerSkinningSetMatrices(samplerId);
		}
		return null;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool								EffectBeginUpdateSamplerSkinning(int guid, int samplerId, float dt)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		const CPU::CScoped_FpuEnableFastMode	_fm;
		const CPU::CScoped_FpuDisableExceptions	_de;

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->EffectBeginUpdateSamplerSkinning(samplerId, dt);
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool								EffectEndUpdateSamplerSkinning(int guid, int samplerId)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		const CPU::CScoped_FpuEnableFastMode	_fm;
		const CPU::CScoped_FpuDisableExceptions	_de;

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->EffectEndUpdateSamplerSkinning(samplerId);
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool						EffectSetSamplerShape(int guid, int samplerId, EShapeType shapeType, CFloat3 size)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->SetSamplerShapeBasics(samplerId, shapeType, size);
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION SCurveSamplerToFill		*GetCurveSamplerData(int keyPointsCount, int curveDimension)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return null);
		PK_SCOPEDPROFILE();

		SCurveSamplerToFill		*curveSamplerData = PK_NEW(SCurveSamplerToFill);

		if (!PK_VERIFY(curveSamplerData != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate SCurveSamplerToFill");
			return null;
		}

		// for each curve and for each key-point, we have a in tangent, out tangent and value + the time:
		curveSamplerData->m_KeyPoints = PK_MALLOC_ALIGNED(keyPointsCount * curveDimension * 4 * sizeof(float), 0x10);

		curveSamplerData->m_CurveDimension = curveDimension;
		curveSamplerData->m_KeyPointsCount = keyPointsCount;
		return curveSamplerData;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool						EffectSetCurveSampler(int guid, int samplerId, SCurveSamplerToFill *curveSampler)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		if (!PK_VERIFY(curveSampler != null))
			return false;
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			bool		success;

			success = effect->SetSamplerCurve(samplerId, curveSampler);
			PK_FREE(curveSampler->m_KeyPoints);
			PK_DELETE(curveSampler);
			return success;
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION STextureSamplerToFill				*GetTextureSamplerData(int byteSize)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return null);
		PK_SCOPEDPROFILE();

		STextureSamplerToFill		*textureSamplerData = PK_NEW(STextureSamplerToFill);

		if (!PK_VERIFY(textureSamplerData != null))
			return null;

		textureSamplerData->m_SizeInBytes = byteSize;
		textureSamplerData->m_TextureData = PK_MALLOC_ALIGNED(byteSize, 0x10);
		return textureSamplerData;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool								EffectSetTextureSampler(int guid, int samplerId, STextureSamplerToFill *textureSampler)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		if (!PK_VERIFY(textureSampler != null))
			return false;
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			bool		success;

			success = effect->SetSamplerTexture(samplerId, textureSampler);
			PK_FREE(textureSampler->m_TextureData);
			PK_DELETE(textureSampler);
			return success;
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	EffectSetTextSampler(int guid, int samplerId, const char *text)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		if (!PK_VERIFY(text != null))
			return false;
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->SetSamplerText(samplerId, text);
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	EffectResetDefaultSampler(int guid, int samplerId)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->ResetSamplerToDefault(samplerId);
		}
		return false;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	EffectSetTransforms(int guid, CFloat4x4 transforms)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return false);
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);
		if (effect != null)
		{
			effect->UpdateTransforms(transforms);
			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	 LoadPkmmAsSceneMesh(const char *pkmmVirtualPath)
	{
		NEED_PK_RUNTIME_STARTED(return false);
		if (!PK_VERIFY(pkmmVirtualPath != null))
			return false;
		PK_SCOPEDPROFILE();

		CString		pkmm = pkmmVirtualPath;
		if (pkmm.Empty())
			return false;

		PMeshNew	mesh = CRuntimeManager::Instance().LoadPkmm(pkmm);
		if (mesh == null)
		{
			CLog::Log(PK_ERROR, "Fail to load \"%s\" as scene mesh", pkmm.Data());
			return false;
		}
		CRuntimeManager::Instance().GetScene().SetCollisionMesh(mesh);
		return true;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	 SceneMeshClear()
	{
		NEED_PK_RUNTIME_STARTED(return);
		PK_SCOPEDPROFILE();

		CRuntimeManager::Instance().SceneMeshClear();
	}

	// --------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	WriteProfileReport(const char* path)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		if (!PK_VERIFY(path != null))
			return;
		PK_SCOPEDPROFILE();
#if	(KR_PROFILER_ENABLED != 0)

		Profiler::CProfiler	*profiler = Profiler::MainEngineProfiler();
		if (profiler == null)
			return;
		profiler->Reset();

		//profiler->BuildReport();
		CDynamicMemoryStream stream = CDynamicMemoryStream();
		Profiler::CProfilerReport	report;
		profiler->BuildReport(&report);
		Profiler::WriteProfileReport(report, stream);
		FILE	*f = fopen(path, "wb");
		if (f != null)
		{
			u32		size = 0;
			u8		*buffer = stream.ExportDataAndClose(size);
			fwrite(buffer, size, 1, f);
			fclose(f);
			PK_FREE(buffer);
		}
		else
			CLog::Log(PK_ERROR, "Failed to write profile report to %s", path);
		stream.Close();
#endif	// (KR_PROFILER_ENABLED != 0)
	}

	// --------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	ProfilerSetEnable(bool enable)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
#if	(KR_PROFILER_ENABLED != 0)
		Profiler::CProfiler	*profiler = Profiler::MainEngineProfiler();

		if (profiler == null)
			return;

		profiler->GrabCallstacks(false);
		profiler->Activate(enable);
		profiler->Reset();
#endif	// (KR_PROFILER_ENABLED != 0)
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	ClearAllCallbacks()
	{
		ClearNativeToManagedCallbacks();
#if		PK_UNITY_EDITOR
		ClearNativeToManagedEditorCallbacks();
#endif
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	UnloadFx(const char *path)
	{
		NEED_PK_RUNTIME_STARTED(return);
		PK_SCOPEDPROFILE();

		CPKFXScene	&scene = CRuntimeManager::Instance().GetScene();

		// Needs simulation sync
		if (!PK_VERIFY(scene.GameThread_WaitForUpdateEnd()))
			return;

		CRuntimeManager::Instance().ClearFxInstances(path);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	Reset()
	{
		NEED_PK_RUNTIME_STARTED(return);
		PK_SCOPEDPROFILE();

		CPKFXScene	&scene = CRuntimeManager::Instance().GetScene();

		// Needs simulation sync
		if (!PK_VERIFY(scene.GameThread_WaitForUpdateEnd()))
			return;

		CRuntimeManager::Instance().ClearAllInstances();
		CRuntimeManager::Instance().SceneMeshClear();
		scene.Reset();
	}

	MANAGED_TO_POPCORN_CONVENTION void	DeepReset()
	{
		NEED_PK_RUNTIME_STARTED(return);
		PK_SCOPEDPROFILE();

		CPKFXScene	&scene = CRuntimeManager::Instance().GetScene();

		// Needs simulation sync
		if (!PK_VERIFY(scene.GameThread_WaitForUpdateEnd()))
			return;

		// Do not call the Cb when unloading all the effects because the renderers and components are already destroyed on the managed side:
		CRuntimeManager::Instance().
		CRuntimeManager::Instance().ClearAllInstances(true);
		CRuntimeManager::Instance().SceneMeshClear();
		scene.HardReset();

		HBO::g_Context->UnloadAllFiles();
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	SetApplicationLoopbackAudioVolume(float volume)
	{
		NEED_PK_RUNTIME_STARTED(return);
		PK_SCOPEDPROFILE();
		const PLoopbackCapture	&loopback = CRuntimeManager::Instance().m_LoopbackAudio;
		if (loopback != null)
			loopback->SetVolumeMultiplier(volume);
	}


#if		defined(PK_COMPILER_CLANG) || defined(PK_COMPILER_GCC)
#	pragma GCC visibility pop
#endif
//----------------------------------------------------------------------------
} // extern "C"
