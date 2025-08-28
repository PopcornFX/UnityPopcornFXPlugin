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
#	include		"EditorOnly/AssetBaker.h"
#	include		"EditorOnly/EditorNativeToManaged.h"
#	include		"EditorOnly/EditorManager.h"
#endif

#include <pk_particles/include/ps_config.h>
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
		if (!PK_VERIFY(CRuntimeManager::InitializeInstanceIFN(null, unityInterfaces)))
		{
			CLog::Log(PK_ERROR, "InitializeInstanceIFN failed");
			return;
		}
		CLog::Log(PK_INFO, "UnityPluginLoad: Loading PopcornFX Plugin, Initialization success");
		IUnityGraphics	*unityGraphics = CRuntimeManager::Instance().GetUnityGraphics();
		if (unityGraphics == null)
			unityGraphics = UNITY_GET_INTERFACE(unityInterfaces, IUnityGraphics);
		if (unityGraphics != null)
		{
			unityGraphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
		}
		// Run OnGraphicsDeviceEvent(initialize) manually on plugin load
		OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
	}


#if defined(PK_IOS)
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API	PKIOSUnityPluginUnload()
#else
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API	UnityPluginUnload()
#endif
	{
		CLog::Log(PK_INFO, "UnityPluginUnload: Unloading PopcornFX Plugin");
		IUnityGraphics	*unityGraphics = CRuntimeManager::Instance().GetUnityGraphics();
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
			if (!CRuntimeManager::IsInstanceInitialized())
				return; // Wait the UnityPluginLoad, it will initialize the instance and call OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);

			CLog::Log(PK_INFO, "OnGraphicsDeviceEvent: UnityGfxDeviceEventType == kUnityGfxDeviceEventInitialize");
			if (!PK_VERIFY(CRuntimeManager::InitializeInstanceIFN(null, CRuntimeManager::Instance().GetUnityInterfaces())))
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

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	PopcornFXStartup(const SPopcornFxSettings *settings)
	{
		const bool	pluginIsInitialized = CRuntimeManager::IsInstanceInitialized();

		if (!PK_VERIFY(pluginIsInitialized))
			return ManagedBool_False;

		if (!PK_VERIFY(settings != null))
			return ManagedBool_False;

		// The plugin was already started so we just change the settings:
		if (!PK_VERIFY(CRuntimeManager::Instance().PopcornFXChangeSettings(*settings)))
		{
			CLog::Log(PK_ERROR, "Could not change the PopcornFX settings");
			return ManagedBool_False;
		}

		// Load the effect pack:
		CRuntimeManager::Instance().LoadPack();
		return ManagedBool_True;
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION const char	*GetRuntimeVersion()
	{
		NEED_PK_RUNTIME_STARTED(return "Runtime not started");

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

	MANAGED_TO_POPCORN_CONVENTION int	GetStackLogNumber()
	{
		if (!CRuntimeManager::IsInstanceInitialized())
			return 0;

		CUnityLog	*logger = CRuntimeManager::Instance().GetLogger();

		if (logger != null)
			return (int)logger->Count();
		return 0;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION int	UnstackLog(char *dstBuffer, int dstSize, int &logSeverity)
	{
		// DO NOT LOG ANYTHING IN THIS FUNCTION! INFINITE LOOP...
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

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	SetMaxCameraCount(int count)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);

		CRuntimeManager::Instance().SetMaxCameraCount(count);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	SetCurrentQualityLevel(const char *qualityLvl)
	{
		CRuntimeManager::Instance().SetCurrentQualityLevel(qualityLvl);
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
		g_RenderThreadID = CCurrentThread::RegisterUserThread();

		const CPU::CScoped_FpuEnableFastMode	_fm;
		const CPU::CScoped_FpuDisableExceptions	_de;

		CRuntimeManager	&manager = CRuntimeManager::Instance();
		CPKFXScene		&scene = manager.GetScene();

		if (manager.RenderThreadIsSet() == false || !manager.IsRenderThread())
		{
			manager.SetRenderThread();
		}
		PK_SCOPEDPROFILE();

		const u32	event_masked = static_cast<u32>(renderEvent);

		if ((event_masked & 0xFFFF0000) != POPCORN_MAGIC_NUMBER)
			return; // wrong eventId, possibly another native plugin.

		renderEvent = (event_masked &~0xFFFFF000);

		if (renderEvent < 0)
		{
			PK_ASSERT_NOT_REACHED_MESSAGE("Render event is not valid %d" PK_COMMA renderEvent);
			return;
		}

		const u32						camID = renderEvent;
		const TArray<SUnitySceneView>	&sceneViews = scene.SceneViews();

		if (camID >= sceneViews.Count()) 
			return; // Can happen at startup or on scene load, skip this frame

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

	MANAGED_TO_POPCORN_CONVENTION void	PreloadFxIFN(const char *path, ManagedBool requiresGameThreadCollect)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
		if (!PK_VERIFY(path != null))
			return;
		PK_SCOPEDPROFILE();

		CPKFXScene	&scene = CRuntimeManager::Instance().GetScene();

		// Needs simulation sync
		if (!PK_VERIFY(scene.GameThread_WaitForUpdateEnd()))
			return;

		CRuntimeManager::Instance().PreloadFxIFN(path, requiresGameThreadCollect != 0);
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
		CPKFXEffect	*fx = PK_NEW(CPKFXEffect(desc->m_RequiresGameThreadCollect == ManagedBool_True, desc->m_Transforms));
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

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	TerminateFx(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().TerminateFx(guid) ? ManagedBool_True : ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	StartFx(int guid, float dt, float prewarm)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().StartFx(guid, dt, prewarm) ? ManagedBool_True : ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	StopFx(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().StopFx(guid) ? ManagedBool_True : ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	KillFx(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().KillFx(guid) ? ManagedBool_True : ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	TeleportFx(int guid)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().TeleportFx(guid)  ? ManagedBool_True : ManagedBool_False;;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	SetFxVisibility(int guid, ManagedBool enabled)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		bool activate = enabled == ManagedBool_True ? true : false;
		return CRuntimeManager::Instance().SetFxVisibility(guid, activate) ? ManagedBool_True : ManagedBool_False;;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	SetFxTimescale(int guid, float timeScale)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		return CRuntimeManager::Instance().SetFxTimescale(guid, timeScale) ? ManagedBool_True : ManagedBool_False;;
	}

	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	RegisterExportedEvent(int guid, const char *eventName, unsigned int unityKey)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		CString			eventString = CString(eventName);
		const CStringId	event = CStringId(eventString);

		return CRuntimeManager::Instance().GetScene().RegisterEventListener(guid, event, unityKey) ? ManagedBool_True : ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	UnregisterExportedEvent(int guid, const char *eventName, unsigned int unityKey)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		CString			eventString = CString(eventName);
		const CStringId	event = CStringId(eventString);

		return CRuntimeManager::Instance().GetScene().UnregisterEventListener(guid, event, unityKey) ? ManagedBool_True : ManagedBool_False;
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

	MANAGED_TO_POPCORN_CONVENTION ManagedBool		EffectSetSamplerShapeTransform(int guid, int samplerId, CFloat4x4 transforms)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		CPKFXEffect		*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->SetSamplerShapeTransform(samplerId, transforms) ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
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

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	EffectSetMeshSampler(int guid, int samplerId, SMeshDataToFill *meshSampler, CFloat3 size, ManagedBool async)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		if (!PK_VERIFY(meshSampler != null))
			return ManagedBool_False;
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
			return success ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	EffectSetMeshSamplerBaked(int guid, int samplerId, SMeshSamplerBaked *meshSamplerBaked, CFloat3 size, ManagedBool async)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		if (!PK_VERIFY(meshSamplerBaked->m_PkmmFileContent != null))
			return ManagedBool_False;
		PK_SCOPEDPROFILE();

		CPKFXEffect		*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			if (async == ManagedBool_True)
				return effect->SetSamplerShapeMeshFromPkmm_Async(samplerId, meshSamplerBaked, size) ? ManagedBool_True : ManagedBool_False;
			else
				return effect->SetSamplerShapeMeshFromPkmm(samplerId, meshSamplerBaked, size) ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
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

	MANAGED_TO_POPCORN_CONVENTION ManagedBool								EffectBeginUpdateSamplerSkinning(int guid, int samplerId, float dt)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		const CPU::CScoped_FpuEnableFastMode	_fm;
		const CPU::CScoped_FpuDisableExceptions	_de;

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->EffectBeginUpdateSamplerSkinning(samplerId, dt) ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool								EffectEndUpdateSamplerSkinning(int guid, int samplerId)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		const CPU::CScoped_FpuEnableFastMode	_fm;
		const CPU::CScoped_FpuDisableExceptions	_de;

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->EffectEndUpdateSamplerSkinning(samplerId) ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool						EffectSetSamplerShape(int guid, int samplerId, EShapeType shapeType, CFloat3 size)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->SetSamplerShapeBasics(samplerId, shapeType, size) ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
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

	MANAGED_TO_POPCORN_CONVENTION ManagedBool						EffectSetCurveSampler(int guid, int samplerId, SCurveSamplerToFill *curveSampler)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		if (!PK_VERIFY(curveSampler != null))
			return ManagedBool_False;
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			bool		success;

			success = effect->SetSamplerCurve(samplerId, curveSampler);
			PK_FREE(curveSampler->m_KeyPoints);
			PK_DELETE(curveSampler);
			return success ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
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

	MANAGED_TO_POPCORN_CONVENTION ManagedBool								EffectSetTextureSampler(int guid, int samplerId, STextureSamplerToFill *textureSampler)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		if (!PK_VERIFY(textureSampler != null))
			return ManagedBool_False;
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			bool		success;

			success = effect->SetSamplerTexture(samplerId, textureSampler);
			PK_FREE(textureSampler->m_TextureData);
			PK_DELETE(textureSampler);
			return success ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	EffectSetTextSampler(int guid, int samplerId, const char *text)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		if (!PK_VERIFY(text != null))
			return ManagedBool_False;
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->SetSamplerText(samplerId, text) ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	EffectSetGridSampler(int guid, int samplerId, SSamplerGrid *gridSampler)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		if (!PK_VERIFY(gridSampler != null))
			return ManagedBool_False;
		PK_SCOPEDPROFILE();

		CPKFXEffect* effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			bool		success;

			success = effect->SetSamplerGrid(samplerId, gridSampler);
			return success ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
	}


	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	EffectResetDefaultSampler(int guid, int samplerId)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);

		if (effect != null)
		{
			return effect->ResetSamplerToDefault(samplerId) ? ManagedBool_True : ManagedBool_False;
		}
		return ManagedBool_False;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	EffectSetTransforms(int guid, CFloat4x4 transforms)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return ManagedBool_False);
		PK_SCOPEDPROFILE();

		CPKFXEffect	*effect = CRuntimeManager::Instance().FxGet(guid);
		if (effect != null)
		{
			effect->UpdateTransforms(transforms);
			return ManagedBool_True;
		}
		return ManagedBool_False;
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	 LoadPkmmAsSceneMesh(const char *pkmmVirtualPath)
	{
		NEED_PK_RUNTIME_STARTED(return ManagedBool_False);
		if (!PK_VERIFY(pkmmVirtualPath != null))
			return ManagedBool_False;
		PK_SCOPEDPROFILE();

		CString		pkmm = pkmmVirtualPath;
		if (pkmm.Empty())
			return ManagedBool_False;

		PMeshNew	mesh = CRuntimeManager::Instance().LoadPkmm(pkmm);
		if (mesh == null)
		{
			CLog::Log(PK_ERROR, "Fail to load \"%s\" as scene mesh", pkmm.Data());
			return ManagedBool_False;
		}
		CRuntimeManager::Instance().GetScene().SetCollisionMesh(mesh);
		return ManagedBool_True;
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

	MANAGED_TO_POPCORN_CONVENTION void	ProfilerSetEnable(ManagedBool enable)
	{
		NEED_PK_MEDIUM_COLLECTION_CREATED(return);
#if	(KR_PROFILER_ENABLED != 0)
		Profiler::CProfiler	*profiler = Profiler::MainEngineProfiler();

		if (profiler == null)
			return;

		profiler->GrabCallstacks(false);
		profiler->Activate(enable == ManagedBool_True ? true : false);
		profiler->Reset();
#endif	// (KR_PROFILER_ENABLED != 0)
	}

	// --------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void 		StatsEnableFrameStats(ManagedBool enable)
	{
		(void)enable;
#if	(PK_PARTICLES_HAS_STATS != 0)
		CRuntimeManager::Instance().GetProfiler().EnableFrameStats(enable == ManagedBool_True ? true : false);
#endif
	}

	// --------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void 		StatsEnableEffectsStats(ManagedBool enable)
	{
		(void)enable;
#if	(PK_PARTICLES_HAS_STATS != 0)
		CRuntimeManager::Instance().GetProfiler().EnableEffectsStats(enable == ManagedBool_True ? true : false);
#endif
	}


	// --------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool 		StatsPullFrameData(const char *reportName, SStatsToFill &data)
	{
		(void)reportName;
		(void)data;
#if	(PK_PARTICLES_HAS_STATS != 0)
		CRuntimeManager			&manager = CRuntimeManager::Instance();
		return manager.GetProfiler().FillFrameStats(reportName, data) ? ManagedBool_True : ManagedBool_False;
#else // (PK_PARTICLES_HAS_STATS != 0)
		return ManagedBool_False;
#endif	
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

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	SetQualityLevelSettings(const char **qualityLevelNames, int qualityLevelCount, int current, bool updateCookery)
	{
		CRuntimeManager::Instance().SetQualityLevelSettings(qualityLevelNames, qualityLevelCount, current);

		if (updateCookery)
		{
#if		PK_UNITY_EDITOR
			CEffectBaker *effectBaker = CEditorManager::Instance().GetEffectBaker();
			if (effectBaker->SetTargetPlatform(effectBaker->GetTargetPlatform(), (u32)qualityLevelCount), true)
				effectBaker->UpdateCookeryConfigFile();
#endif
		}
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	CanSkipUpdate()
	{
		NEED_PK_RUNTIME_STARTED(return ManagedBool_True);
		return CRuntimeManager::Instance().CanSkipUpdate() ? ManagedBool_True : ManagedBool_False;
	}


#if		defined(PK_COMPILER_CLANG) || defined(PK_COMPILER_GCC)
#	pragma GCC visibility pop
#endif
//----------------------------------------------------------------------------
} // extern "C"
