//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once

#include <pk_kernel/include/kr_threads_job_pool.h>
#include <pk_kernel/include/kr_string_id.h>
#include <pk_kernel/include/kr_log_listeners_file.h>
#include <pk_particles/include/ps_mediums.h>
#include <pk_particles/include/ps_descriptor.h>
#include <pk_particles/include/ps_config.h>

#include "ManagedToNative.h"
#include "PKUnity_Scene.h"
#include "PKUnity_Log.h"
#include "PKUnity_Profiler.h"
#include "FxEffect.h"

#include "UnityGraphicsAPI/IUnityInterface.h"
#include "UnityGraphicsAPI/IUnityProfiler.h"
#include "FxWindowsGraphicsDefs.h"

#include "AudioApplicationLoopback/ILoopbackCapture.h"

#if	defined(PK_NX)
struct	IUnityGraphicsNvn;
#elif defined(PK_DURANGO)
#include "FxDurangoGraphicsDefs.h"
#elif defined(PK_ORBIS)
#include "FxOrbisGraphicsDefs.h"
struct	IUnityGraphicsPS4;
#elif defined(PK_UNKNOWN2)
#include "FxUNKNOWN2GraphicsDefs.h"
struct	IUnityGraphicsUNKNOWN2;
#endif	// PK_NX
#if	(PK_BUILD_WITH_VULKAN_SUPPORT != 0)
struct	IUnityGraphicsVulkan;
#endif
#if	(PK_BUILD_WITH_D3D12_SUPPORT != 0)
#include "ImplemGraphicsAPI/RenderAPI_DX12Data.h"
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CUnityResourceHandlerImage;
class	CUnityPseudoFileSystem;
PK_FORWARD_DECLARE(SetBakedSamplerTask);

//----------------------------------------------------------------------------

class	CRuntimeManager : public CNonCopyable
{
public:
	struct SSpectrumDescriptor
	{
		CStringId							m_RegisteredName;
		float								*m_Spectrum;
		float								*m_Waveform;
		TArray<float*>						m_SpectrumPyramid;
		TArray<float*>						m_WaveformPyramid;
		bool								m_NeedsSpectrum;
		bool								m_NeedsWaveform;
		bool								m_HasSpectrum;
		bool								m_HasWaveform;
		u32									m_SpectrumSize;
		u32									m_WaveformSize;

		SSpectrumDescriptor(CStringId name);
		~SSpectrumDescriptor();

		static void	_CleanAudioPyramid(TArray<float*> &pyramid);
	};

	enum	EExecFilterSetSampler
	{
		ExecFilter_BackgroundTask = (SimDataState_User << 1)
	};

private:
	CRuntimeManager();
public:
	~CRuntimeManager();

	// Handle singleton startup etc...
	static bool							IsInstanceInitialized();
	static bool							InitializeInstanceIFN(const SPopcornFxSettings *settings, IUnityInterfaces *unityInterfaces); // Create the scene and startup PopcornFX

	void								SetMaxCameraCount(int count);
	static CRuntimeManager				&Instance();

	static SMeshDataToFill				*CreateMeshDataToFill(int vertexCount, int indexCount, int bonesCount, int vertexAttributes);

	bool								PopcornFXChangeSettings(const SPopcornFxSettings &settings); // Change the config
	bool								PopcornFXShutdown();

	// Rendering:
	bool								SetUnityInterfaces(IUnityInterfaces* unityInterfaces);
	void								SetUnityRendererType(UnityGfxRenderer type);
	IUnityInterfaces					*GetUnityInterfaces();
	IUnityGraphics						*GetUnityGraphics();
#if	defined(PK_NX)
	IUnityGraphicsNvn					*GetNVNUnityGraphics();
#endif
	bool								UnityAPIDeviceInitialize();
	bool								UnityAPIDeviceUnInitialize();

	// API specific rendering:
	UnityGfxRenderer					GetDeviceType() const;
#if		(PK_BUILD_WITH_D3D11_SUPPORT != 0)
	D3DDevice							*GetDeviceD3D11() const;
	D3DDeviceContext					*GetContextD3D11() const;
#endif
#if		(PK_BUILD_WITH_D3D12_SUPPORT != 0)
	ID3D12Device						*GetDeviceD3D12() const;
#endif
#if		(PK_BUILD_WITH_VULKAN_SUPPORT != 0)
	IUnityGraphicsVulkan				*GetUnityGraphicsVulkan() { return m_UnityVulkan; }
#endif

#if	defined(PK_ORBIS)
	IUnityGraphicsPS4					*GetOrbisUnityGraphics();
	GnmPS4ContextLW						*GetContextPS4() const;
#elif defined(PK_UNKNOWN2)

	IUnityGraphicsUNKNOWN2					*GetUNKNOWN2UnityGraphics();
	UNKNOWN2ContextLW						*GetContextUNKNOWN2() const;
#endif
	CPKFXScene							&GetScene() const;
	bool								GetIsUnitTesting() const { return m_IsUnitTesting; }
	const TArray<float*>				*GetSpectrum(CStringId name) const;
	IFileSystem							*GetFileSystem() const;

	u32									GetCurrentQualityLevel() const;
	const CString						GetCurrentQualityLevelName() const;
	void								SetCurrentQualityLevel(const char *qualityLvl);
	u32									GetQualityLevelCount() const { return m_QualityLevelNames.Count(); } 
	const CString						&GetQualityLevel(u32 idx) const { PK_ASSERT(idx < m_QualityLevelNames.Count()); return m_QualityLevelNames[idx]; }
	const TArray<CString>				&GetQualityLevels() const { return m_QualityLevelNames; }
	void								SetQualityLevelSettings(const char **qualityLevelNames, unsigned int qualityLevelCount, unsigned int current);

	bool								IsUnloading() { return m_Unloading; }
#if	(PK_PARTICLES_HAS_STATS != 0)
	CLiveProfiler						&GetProfiler() { return m_Profiler; }
#endif
	static void							ProfileRecordMemoryTransaction(void *arg, sreg bytes);
	static void							ProfileRecordThreadDependency(void *arg, PopcornFX::CThreadID other, u32 dFlags);

	bool								LoadPack();

	// FX actions:
	CGuid								FxRegister(CPKFXEffect *fx);
	CPKFXEffect							*FxGet(CGuid guid);
	void								ClearAllInstances(bool managedIsCleared = false);
	void								ClearFxInstances(const char *fxPath);
	u32									GetInstanceCount(const PopcornFX::CParticleEffect *effect);

	TMemoryView<const float * const>	GetSpectrumPyramid(CStringId name, u32 &outBaseCount);
	TMemoryView<const float * const>	GetWaveformPyramid(CStringId name, u32 &outBaseCount);
	void								Update(float dt);

	// Set scene mesh:
	void								SceneMeshClear();

	// CAN BE DELAYED: Managed to native
	void								PreloadFxIFN(const char *fxPath, bool requiresGameThreadCollect);
	bool								StartFx(int guid, float dt, float prewarm = 0.0f);
	bool								TerminateFx(int guid);
	bool								StopFx(int guid);
	bool								KillFx(int guid);
	bool								TeleportFx(int guid);
	bool								SetFxTimescale(int guid, float timeScale);
	bool								SetFxVisibility(int guid, bool enable);
	void								UpdateCamDesc(int camID, const SCamDesc &desc, bool update);

	void								ExecDelayedManagedToNativeMethods();

	// CAN BE DELAYED: Native to managed (return false if delayed)
	bool								OnResizeRenderer(int rendererGUID, int particleCount, int reservedVertexCount, int reservedIndexCount, const SRetrieveRendererInfo *info, bool* delayedResult);
	bool								OnSetParticleCount(int rendererGUID, int particleCount);
	bool								OnSetRendererActive(int rendererGUID, ManagedBool active);
	bool								OnUpdateRendererBounds(int rendererGUID, const SUpdateRendererBounds *bounds);
	void								OnFxStopped(int guid);

	void								ExecDelayedNativeToManagedMethods();

	void								SetLogger(CUnityLog *logger);
	CUnityLog							*GetLogger() const;
	void								SetMaxLogStack(u32 maxLogStack);

	bool								RenderThreadIsSet() const { return m_RenderThreadIsSet; }
	bool								IsRenderThread() const { PK_ASSERT(m_RenderThreadIsSet); return CCurrentThread::ThreadID() == m_RenderThread; }
	void								SetRenderThread() { m_RenderThread = CCurrentThread::ThreadID(); m_RenderThreadIsSet = true; }

	static PMeshNew						LoadPkmm(const CString &pkmmVirtualPath);

	void								BeforeUpdate();
	void								AfterUpdate();
	bool								CanSkipUpdate();

	template<class T>
	bool								AddBackgroundTask(T *functor)
	{
		PBackgroundTask			task = m_BackgroundTasks.PullJob();
		if (!PK_VERIFY(task != null))
		{
			CLog::Log(PK_ERROR, "Could not pull background task");
			return false;
		}
		task->m_Cb = FastDelegate<void()>(functor, &T::operator());
		Threads::PAbstractPool	pool = Scheduler::ThreadPool();
		if (!PK_VERIFY(pool != null))
		{
			CLog::Log(PK_ERROR, "Could not add background task to thread pool");
			return false;
		}
		task->AddToPool(pool);
		m_HasBackgroundTasksToKick = true;
		return true;
	}

	bool								KickBackgroundTasksIFN();

	struct	SPopcornFXRuntimeData
	{
	public:
		bool					PopcornFXStartup(IUnityInterfaces *unityInterfaces);
		bool					PopcornFXShutdown();

		static const SPopcornFxSettings		*m_Settings;
		CUnityResourceHandlerImage			*m_ImageHandler;

#if	(PK_LOG_ENABLED != 0)
		CUnityLog							*m_UnityLogger;
		CLogListenerFile					*m_FileLogger;
#endif

		bool								m_GPUBillboarding;
		bool								m_LightRenderer;
		bool								m_SoundRenderer;
		bool								m_DecalRenderer;
	};
	SPopcornFXRuntimeData				*m_PopcornFXRuntimeData;

	PLoopbackCapture					m_LoopbackAudio;
	bool								m_ReadAudioFromProcessOutput;

private:
	static IFileSystem					*_CreateCustomFileSystem();
	static Threads::PAbstractPool		_CreateCustomThreadPool();

	static CRuntimeManager				*m_Instance;

	bool								m_Unloading;

	CThreadID							m_RenderThread;
	bool								m_RenderThreadIsSet;

	Threads::CCriticalSection			m_FxToDeleteLock;
	TArray<CPKFXEffect*>				m_FxToDelete; // queue of FXs to run when sync'd with the rendering thread.

	TSlotArray<CPKFXEffect*>			m_Effects;
	THashMap<CString, CStringId>		m_PreloadedFx;

	Threads::CCriticalSection			m_SpectrumLock;
	TArray<SSpectrumDescriptor>			m_SpectrumDescriptorList;

	PPKFXScene							m_ParticleScene;
	bool								m_IsUnitTesting;

	// Unity rendering:
	IUnityInterfaces					*m_UnityInterfaces;
	IUnityGraphics						*m_UnityGraphics;
#if	defined(PK_NX)
	IUnityGraphicsNvn					*m_UnityNVNGraphics;
#endif
	UnityGfxRenderer					m_DeviceType;

	// All devices:
	// Graphics API handling
#if		(PK_BUILD_WITH_D3D11_SUPPORT != 0)
	D3DDevice							*m_DeviceD3D11;
	D3DDeviceContext					*m_ContextD3D11;
#endif
#if		(PK_BUILD_WITH_D3D12_SUPPORT != 0)
	ID3D12Device						*m_DeviceD3D12;
#endif
#if		(PK_BUILD_WITH_VULKAN_SUPPORT != 0)
	IUnityGraphicsVulkan				*m_UnityVulkan;
#endif // (PK_BUILD_WITH_VULKAN_SUPPORT != 0)

#if	defined(PK_ORBIS)
	GnmPS4ContextLW						*m_ContextPS4;
	IUnityGraphicsPS4					*m_PS4Interface;
#elif defined(PK_UNKNOWN2)
	UNKNOWN2ContextLW						*m_ContextUNKNOWN2;
	IUnityGraphicsUNKNOWN2					*m_UNKNOWN2Interface;
#endif

	// Managed to native delayed calls:
	void										_ExecPreloadFxIFN(const char *fxPath, bool requiresGameThreadCollect);
	bool										_ExecStartFx(int guid, float delay = 0.0f, float prewarm = 0.0f);
	bool										_ExecTerminateFx(int guid);
	bool										_ExecStopFx(int guid);
	bool										_ExecKillFx(int guid);
	void										_ExecUpdateCamDesc(int camID, SCamDesc desc, bool update);

	TArray<SPreloadFx>							m_ToPreload;
	TArray<SStartFx>							m_ToStart;
	TArray<STerminateFx>						m_ToTerminate;
	TArray<SStopFx>								m_ToStop;
	TArray<SKillFx>								m_ToKill;
	TArray<SUpdateCamDesc>						m_UpdateCamDesc;
	TArray<CGuid>								m_FxStopped;

	// Native to managed delayed calls:
	void										_ExecOnFxStopped(CGuid fxId);

	TArray<SOnResizeRenderer>					m_OnResizeRenderer;
	TArray<SOnSetParticleCount>					m_OnSetParticleCount;
	TArray<SOnSetRendererActive>				m_OnSetRendererActive;
	TArray<SOnUpdateRendererBounds>				m_OnUpdateRendererBounds;
	TArray<SOnFxStopped>						m_OnFxStopped;
	Threads::CCriticalSection					m_Native2ManagedLock;

	// Copy of the arrays above to be able to call those C# methods without locking at the same time:
	TArray<SOnResizeRenderer>					m_GameThread_OnResizeRenderer;
	TArray<SOnSetParticleCount>					m_GameThread_OnSetParticleCount;
	TArray<SOnSetRendererActive>				m_GameThread_OnSetRendererActive;
	TArray<SOnUpdateRendererBounds>				m_GameThread_OnUpdateRendererBounds;
	TArray<SOnFxStopped>						m_GameThread_OnFxStopped;

	u32											m_CurrentQualityLevel = 0;
	TArray<CString>								m_QualityLevelNames;

	// We keep a job pool here to set the samplers in asynchronous tasks:
	class	CBackgroundTask : public CAsynchronousPooledJob
	{
	public:
		CBackgroundTask();
		~CBackgroundTask();

		virtual void			_VirtualLaunch(Threads::SThreadContext &threadContext) override;

		FastDelegate<void()>	m_Cb;
	};
	PK_DECLARE_REFPTRCLASS(BackgroundTask);

	TAsynchronousJobPool<CBackgroundTask, 0x10>	m_BackgroundTasks;
	bool										m_HasBackgroundTasksToKick;


#if	(PK_PARTICLES_HAS_STATS != 0)
	CLiveProfiler								m_Profiler;
#endif
};

//----------------------------------------------------------------------------
__PK_API_END
