//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "pk_kernel/include/kr_threads_basics.h"
#include "PKUnity_InterfaceCommon.h"
#include "UnityGraphicsAPI/IUnityGraphics.h"

#define POPCORN_MAGIC_NUMBER		0x5AFE0000

__PK_API_BEGIN
	PK_FORWARD_DECLARE(MeshNew);
__PK_API_END

extern "C"
{
	//----------------------------------------------------------------------------
	// Data structures:
	//----------------------------------------------------------------------------

	struct SPopcornFxSettings
	{
		int						m_DeviceType;

		ManagedBool				m_EnableFileLog;

		ManagedBool				m_EnableRaycastForCollisions;
		ManagedBool				m_DisableDynamicEffectBounds;
		ManagedBool				m_EnableGPUBillboarding;

		ManagedBool				m_EnableLocalizedPages;
		ManagedBool				m_EnableLocalizedByDefault;

		ManagedBool				m_FreeUnusedBatches;
		u32						m_FrameCountBeforeFreeingUnusedBatches;

		ManagedBool				m_IsUnitTesting;
		ManagedBool				m_UseApplicationAudioLoopback;
		ManagedBool				m_LightRendererEnabled;

		ManagedBool				m_SoundRendererEnabled;

		// Threading:
		ManagedBool				m_SingleThreadedExecution;
		ManagedBool				m_OverrideThreadPool;
		int						m_WorkerCount;
		u32						*m_WorkerAffinities;

		// Raycast structs
		int						m_RaycastHitSize;
		int						m_RaycastCommandSize;

		unsigned int			m_CPPMarkerMaxDepth;

		void					PrettyPrintSettings() const
		{
			CString	settings = "";

			settings += "PopcornFX Unity plugin settings : \r\n";
			settings += "[PKFX Unity Settings] EnableFileLog: ...................... %s\r\n";
			settings += "[PKFX Unity Settings] EnableRaycastForCollisions: ......... %s\r\n";
			settings += "[PKFX Unity Settings] DisableDynamicEffectBounds: ......... %s\r\n";
			settings += "[PKFX Unity Settings] EnableGPUBillboard: ................. %s\r\n";
			settings += "[PKFX Unity Settings] SingleThreadedExecution: ............ %s\r\n";
			settings += "[PKFX Unity Settings] OverrideThreadPool: ................. %s\r\n";
			settings += "[PKFX Unity Settings] LocalizedPages: ..................... %s, %s\r\n";
			settings += "[PKFX Unity Settings] FreeUnusedBatches: ...................%s\r\n";
			settings += "[PKFX Unity Settings] FrameCountBeforeFreeingUnusedBatches: %u";


			if (m_OverrideThreadPool == ManagedBool_True)
			{
				settings += "\r\n";
				settings += "[PKFX Unity Settings] Explicit worker count: .............. %u\r\n";
				settings += "[PKFX Unity Settings] Explicit worker affinities: ......... %s";
				CLog::Log(PK_INFO, settings.Data()
					, MANAGED_BOOL_2_STR(m_EnableFileLog)
					, MANAGED_BOOL_2_STR(m_EnableRaycastForCollisions)
					, MANAGED_BOOL_2_STR(m_DisableDynamicEffectBounds)
					, MANAGED_BOOL_2_STR(m_EnableGPUBillboarding)
					, MANAGED_BOOL_2_STR(m_SingleThreadedExecution)
					, MANAGED_BOOL_2_STR(m_OverrideThreadPool)
					, MANAGED_BOOL_2_STR(m_EnableLocalizedPages), MANAGED_BOOL_2_STR(m_EnableLocalizedByDefault)
					, MANAGED_BOOL_2_STR(m_FreeUnusedBatches)
					, m_FrameCountBeforeFreeingUnusedBatches
					, m_WorkerCount
					, m_WorkerAffinities != null ? "true" : "false");
			}
			else
			{
					CLog::Log(PK_INFO, settings.Data()
					, MANAGED_BOOL_2_STR(m_EnableFileLog)
					, MANAGED_BOOL_2_STR(m_EnableRaycastForCollisions)
					, MANAGED_BOOL_2_STR(m_DisableDynamicEffectBounds)
					, MANAGED_BOOL_2_STR(m_EnableGPUBillboarding)
					, MANAGED_BOOL_2_STR(m_SingleThreadedExecution)
					, MANAGED_BOOL_2_STR(m_OverrideThreadPool)
					, MANAGED_BOOL_2_STR(m_EnableLocalizedPages), MANAGED_BOOL_2_STR(m_EnableLocalizedByDefault)
					, MANAGED_BOOL_2_STR(m_FreeUnusedBatches)
					, m_FrameCountBeforeFreeingUnusedBatches);
			}
		}
	};

	//----------------------------------------------------------------------------

	struct	SCamDesc
	{
		CFloat4x4	m_ViewMatrix;
		CFloat4x4	m_ProjectionMatrix;
		u32			m_ViewportWidth;
		u32			m_ViewportHeight;
		int			m_RenderPass;
		float		m_NearClip;
		float		m_FarClip;
	};

	//----------------------------------------------------------------------------

	struct	SFxDesc
	{
		const char	*m_EffectPath;
		CFloat4x4	m_Transforms;
		ManagedBool	m_RequiresGameThreadCollect;
	};

	//----------------------------------------------------------------------------

	enum EShapeType : int
	{
		BoxShape = 0,
		SphereShape,
		CylinderShape,
		CapsuleShape,
		MeshShape,
		MeshFilterShape,
		SkinnedMeshShape,
		BakedMeshShape,
		ShapeUnsupported
	};

	//----------------------------------------------------------------------------

	struct	SStats
	{
		float					m_UpdateTime;
		float					m_RenderTime;
		int						m_TotalMemoryFootprint;
		int						m_TotalParticleMemory;
		int						m_UnusedParticleMemory;
	};

	//----------------------------------------------------------------------------

	struct	SMeshDataToFill
	{
		int						m_IdxCount;
		u32						*m_Indices;
		int						m_VtxCount;
		CFloat3					*m_Positions;
		CFloat3					*m_Normals;
		CFloat4					*m_Tangents;
		CFloat2					*m_UVs;
		CFloat4					*m_Colors;

		// Always 4 bones influence per vertex:
		float					*m_VtxBonesWeights;
		u32						*m_VtxBonesIndices;
		int						m_BonesCount;

		SMeshDataToFill();
	};

	//----------------------------------------------------------------------------

	enum	EMeshVertexAttributes
	{
		Attrib_Normal = 0,
		Attrib_Tangent = 1,
		Attrib_Color = 2,
		Attrib_Texcoord = 3,
		Attrib_BonesIndicesAndWeights = 4,
	};

	//----------------------------------------------------------------------------

	struct	SMeshSamplerBaked
	{
		void			*m_PkmmFileContent;
		int				m_PkmmFileContentSize;
		int				m_SubmeshId;
		int				m_BonesCount;
		ManagedBool		m_IsSkinned;

		SMeshSamplerBaked();
	};

	//----------------------------------------------------------------------------

	struct	SMeshSamplerToFillFromPkmm
	{
		void	*m_PkmmData;
		u32		m_PkmmDataSize;
	};

	//----------------------------------------------------------------------------

	struct	SCurveSamplerToFill
	{
		void		*m_KeyPoints;
		int			m_CurveDimension;
		int			m_KeyPointsCount;

		SCurveSamplerToFill()
			:	m_KeyPoints(null)
			,	m_CurveDimension(0)
			,	m_KeyPointsCount(0)
		{
		}
	};

	//----------------------------------------------------------------------------

	struct	STextureSamplerToFill
	{
		void		*m_TextureData;
		int			m_Width;
		int			m_Height;
		int			m_SizeInBytes;
		int			m_PixelFormat;
		int			m_WrapMode;

		STextureSamplerToFill()
			:	m_TextureData(null)
			,	m_Width(0)
			,	m_Height(0)
			,	m_SizeInBytes(0)
			,	m_PixelFormat(0)
			,	m_WrapMode(0)
		{
		}
	};

	//----------------------------------------------------------------------------

	struct SLightInfo
	{
		CFloat3	m_Position;
		CFloat4	m_Color;
		float	m_Range;
		float	m_Intensity;
	};

	//----------------------------------------------------------------------------

	struct SSoundInfo
	{
		CInt2		m_SelfID;
		float		m_Age;
		CFloat3		m_Position;
		CFloat3		m_Velocity;
		float		m_Radius;
		float		m_DopplerLevel;
		float		m_Volume;
		bool		m_Audible;
		char*		m_SoundData;
	};

	//----------------------------------------------------------------------------

	struct SEffectStatsToFill
	{
		unsigned int			m_InstanceCount;
		unsigned int			m_TotalParticleCount;
		double					m_TotalTimeAverage;
		double					m_TotalTimeAverageRaw;
		const char*				m_Unit;
	};

	struct SStatsToFill
	{
		float					m_CollectionUpdateTimeAverage;
		unsigned int			m_ParticleCountAverage;
		unsigned int			m_EffectsUsedCountAverage;

		unsigned int			m_EffectsStatsCount;
		void*					m_EffectsStats; //SEffectStatsToFill
		const char				*m_EffectNames;
	};

	//----------------------------------------------------------------------------
	// Globals:
	//----------------------------------------------------------------------------

	extern CThreadID				g_RenderThreadID;

	//----------------------------------------------------------------------------
	// Callbacks:
	//----------------------------------------------------------------------------
#if		defined(PK_COMPILER_CLANG) || defined(PK_COMPILER_GCC)
#	pragma GCC visibility push(default)
#endif

	// Unity standard callbacks:
#if defined(PK_IOS)
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API					PKIOSUnityPluginLoad(IUnityInterfaces* unityInterfaces);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API					PKIOSUnityPluginUnload();
#else
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API					UnityPluginLoad(IUnityInterfaces* unityInterfaces); // Can be the first function called, create scene here IFN
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API					UnityPluginUnload();
#endif
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API					OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType); // Can be the first function called, create scene here IFN

	MANAGED_TO_POPCORN_CONVENTION void								RegisterPlugin();

	MANAGED_TO_POPCORN_CONVENTION UnityRenderingEvent				GetRenderEventFunc();

	// Ours:
	MANAGED_TO_POPCORN_CONVENTION int								PopcornFXDllLoaded();
	MANAGED_TO_POPCORN_CONVENTION bool 								PopcornFXStartup(const SPopcornFxSettings *settings); // Can be the first function called, create scene here IFN
	MANAGED_TO_POPCORN_CONVENTION void 								PopcornFXShutdown();
	//Log
	MANAGED_TO_POPCORN_CONVENTION void								SetMaxLogStack(int maxLogStack);
	MANAGED_TO_POPCORN_CONVENTION int								UnstackLog(char *dstBuffer, int dstSize, int &logSeverity);
	// Update and rendering:
	MANAGED_TO_POPCORN_CONVENTION void								SetMaxCameraCount(int number);
	MANAGED_TO_POPCORN_CONVENTION void								SetCurrentQualityLevel(const char *qualityLvl);
	MANAGED_TO_POPCORN_CONVENTION void								UpdateCamDesc(int camID, const SCamDesc *desc, ManagedBool update); // Update camera
	MANAGED_TO_POPCORN_CONVENTION void								UpdateParticles(float dt); // Update evolve
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API					OnRenderEvent(int eventID); // Render function

	MANAGED_TO_POPCORN_CONVENTION void								SyncParticlesSimulation();
	MANAGED_TO_POPCORN_CONVENTION void								TransformAllParticles(CFloat3 worldOffset);
	MANAGED_TO_POPCORN_CONVENTION void								GetStats(SStats *stats);
	MANAGED_TO_POPCORN_CONVENTION void								PreloadFxIFN(const char *path, ManagedBool requiresGameThreadCollect);
	MANAGED_TO_POPCORN_CONVENTION int								InstantiateFx(const SFxDesc *desc);
	MANAGED_TO_POPCORN_CONVENTION bool								StartFx(int guid, float dt, float prewarm);
	MANAGED_TO_POPCORN_CONVENTION bool								TerminateFx(int guid);
	MANAGED_TO_POPCORN_CONVENTION bool								StopFx(int guid);
	MANAGED_TO_POPCORN_CONVENTION bool								KillFx(int guid);

	//Broadcasted event
	MANAGED_TO_POPCORN_CONVENTION bool								RegisterExportedEvent(int guid, const char *eventName, unsigned int unityKey);
	MANAGED_TO_POPCORN_CONVENTION bool								UnregisterExportedEvent(int guid, const char *eventName, unsigned int unityKey);
	MANAGED_TO_POPCORN_CONVENTION void								UnregisterEffectAllExportedEvent(int guid);
	MANAGED_TO_POPCORN_CONVENTION void								UnregisterSceneAllExportedEvent();

	// Set attribute:
	MANAGED_TO_POPCORN_CONVENTION void								*EffectGetAttributesBuffer(int guid);

	// For the samplers shape:
	MANAGED_TO_POPCORN_CONVENTION bool								EffectSetSamplerShapeTransform(int guid, int samplerId, CFloat4x4 transform);
	// Mesh sampler:
	MANAGED_TO_POPCORN_CONVENTION SMeshDataToFill					*GetMeshDataToFillFromSampler(int vertexCount, int indexCount, int bonesCount, int flags);
	MANAGED_TO_POPCORN_CONVENTION bool								EffectSetMeshSampler(int guid, int samplerId, SMeshDataToFill *meshSampler, CFloat3 size, ManagedBool async);
	MANAGED_TO_POPCORN_CONVENTION bool								EffectSetMeshSamplerBaked(int guid, int samplerId, SMeshSamplerBaked *meshSamplerBaked, CFloat3 size, ManagedBool async);
	// Skinning:
	MANAGED_TO_POPCORN_CONVENTION CFloat4x4							*EffectUpdateSamplerSkinningSetMatrices(int guid, int samplerId);
	MANAGED_TO_POPCORN_CONVENTION bool								EffectBeginUpdateSamplerSkinning(int guid, int samplerId, float dt);
	MANAGED_TO_POPCORN_CONVENTION bool								EffectEndUpdateSamplerSkinning(int guid, int samplerId);
	// Other sampler shapes:
	MANAGED_TO_POPCORN_CONVENTION bool								EffectSetSamplerShape(int guid, int samplerId, EShapeType shapeType, CFloat3 size);
	// Curve sampler:
	MANAGED_TO_POPCORN_CONVENTION SCurveSamplerToFill				*GetCurveSamplerData(int keyPointsCount, int curveDimension);
	MANAGED_TO_POPCORN_CONVENTION bool								EffectSetCurveSampler(int guid, int samplerId, SCurveSamplerToFill *meshSampler);
	// Texture sampler:
	MANAGED_TO_POPCORN_CONVENTION STextureSamplerToFill				*GetTextureSamplerData(int byteSize);
	MANAGED_TO_POPCORN_CONVENTION bool								EffectSetTextureSampler(int guid, int samplerId, STextureSamplerToFill *meshSampler);
	// Text sampler:
	MANAGED_TO_POPCORN_CONVENTION bool								EffectSetTextSampler(int guid, int samplerId, const char *text);
	// Any sampler:
	MANAGED_TO_POPCORN_CONVENTION bool								EffectResetDefaultSampler(int guid, int samplerId);

	MANAGED_TO_POPCORN_CONVENTION bool								EffectSetTransforms(int guid, CFloat4x4 transforms);
	//Scene Mesh
	MANAGED_TO_POPCORN_CONVENTION bool								LoadPkmmAsSceneMesh(const char *pkmmVirtualPath);
	MANAGED_TO_POPCORN_CONVENTION void								SceneMeshClear();
	//Profiler
	MANAGED_TO_POPCORN_CONVENTION void								WriteProfileReport(const char* path);
	MANAGED_TO_POPCORN_CONVENTION void								ProfilerSetEnable(bool enable);

	//Stats
	MANAGED_TO_POPCORN_CONVENTION void 								StatsEnableFrameStats(bool enable);
	MANAGED_TO_POPCORN_CONVENTION void 								StatsEnableEffectsStats(bool enable);
	MANAGED_TO_POPCORN_CONVENTION bool								StatsPullFrameData(const char *reportName, SStatsToFill &data);
	// Reset:
	MANAGED_TO_POPCORN_CONVENTION void								Reset();
	MANAGED_TO_POPCORN_CONVENTION void								DeepReset();
	MANAGED_TO_POPCORN_CONVENTION void								ClearAllCallbacks();

	MANAGED_TO_POPCORN_CONVENTION void								UnloadFx(const char *path);

	MANAGED_TO_POPCORN_CONVENTION const char						*GetRuntimeVersion();

	MANAGED_TO_POPCORN_CONVENTION void								SetApplicationLoopbackAudioVolume(float volume);

	MANAGED_TO_POPCORN_CONVENTION void								SetQualityLevelSettings(const char **qualityLevelNames, int qualityLevelCount, int current, bool updateCookery);

#if		defined(PK_COMPILER_CLANG) || defined(PK_COMPILER_GCC)
#	pragma GCC visibility pop
#endif
}
