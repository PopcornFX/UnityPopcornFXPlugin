#pragma once
//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include <pk_kernel/include/kr_file_watcher.h>
#include <pk_particles/include/ps_mediums.h>
#include <pk_particles/include/ps_scene.h>
#include <pk_geometrics/include/ge_mesh_resource.h>

#include "RenderingIntegration/UnityRenderDataFactory.h"
#include "RenderingIntegration/FrameCollectorUnityTypes.h"

#include "ManagedToNative.h"
#include "DelayedEffectsMethods.h"

class	IRenderAPIData;
struct	SRaycastPack;

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CPKFXEffect;

//----------------------------------------------------------------------------
//
//	Mesh assets
//
//----------------------------------------------------------------------------

struct	SSceneMeshAssets
{
	TPair<CStringId, u32>		m_Id;
	CString						m_Diffuse;

	SSceneMeshAssets(const CString &diffuse)
		: m_Diffuse(diffuse)
	{
	}

	const SSceneMeshAssets		&operator = (const SSceneMeshAssets &other)
	{
		m_Diffuse = other.m_Diffuse;
		return *this;
	}
};

//----------------------------------------------------------------------------
//
//	particle physical scene interface.
//
//----------------------------------------------------------------------------

enum	ESimulationState
{
	BeforeAnyUpdate,
	ParticlesUpdatingSpawn,
	ParticlesUpdatingEvolve,
};

enum
{
	kGameThreadGenDrawCallsSteps = (1U << Step_LockCollectedFrame) | (1U << Step_InsertInBatches) | (1U << Step_AllocBuffers),
	kRenderThreadGenDrawCallsSteps = kAllStepsMask ^ kGameThreadGenDrawCallsSteps
};

//----------------------------------------------------------------------------

class	CPKFXScene : public CRefCountedObject, public IParticleScene
{
private:
	friend CPKFXEffect;
public:
	enum	EUpdateMode
	{
		UpdateMode_Standard = 0,
		UpdateMode_NoUpdate,
		__MaxUpdateModes
	};

public:
	CPKFXScene();
	~CPKFXScene();
	const TArray<SUnitySceneView>	&SceneViews() const { return this->m_SceneViews; };
	TArray<SUnitySceneView>&		SceneViewsForUpdate()  { return this->m_SceneViews; };
	EUpdateMode						UpdateMode() const { return m_UpdateMode; }
	CParticleMediumCollection		*GetParticleMediumCollection(bool useMeshRenderer) const;

	// Setup medium collection
	bool						InitializeInstanceIFN(const SPopcornFxSettings *settings);
	bool						UnityAPIDeviceIsInitialized(UnityGfxRenderer deviceType);
	bool						PopcornFXChangeSettings(const SPopcornFxSettings &settings);
	bool						IsMediumCollectionInitialized() const { return m_IsMediumCollectionInitialized; }

	void						TransformAllParticles(const CFloat3 &worldOffset);

	void						ResolveDynamicCollisionsIFN();

	// Both of those need to be called for the particles to be correctly updated:
	void						SyncPreviousUpdateAndRunDeferredCallsIFN();
	void						LaunchUpdate(float dt);
	// Then build draw-calls:
	void						BuildDrawCalls(const SUnitySceneView &view);

	void						RaiseEventsIFN();
	void						Reset();
	void						HardReset();

	bool						RenderThreadIsSet() const { return m_RenderThreadIsSet; }
	bool						IsRenderThread() const { PK_ASSERT(m_RenderThreadIsSet); return CCurrentThread::ThreadID() == m_RenderThread; }
	void						SetRenderThread() { m_RenderThread = CCurrentThread::ThreadID(); m_RenderThreadIsSet = true; }
	void						UnsetCollisionMesh();
	void						SetCollisionMesh(const PMeshNew &mesh);

	// IParticleScene overrides:
	virtual void								RayTracePacket(	const Colliders::STraceFilter &traceFilter,
																const Colliders::SRayPacket &packet,
																const Colliders::STracePacket &results) override;

	virtual	TMemoryView<const float * const>	GetAudioSpectrum(CStringId channelGroup, u32 &outBaseCount) const override;
	virtual	TMemoryView<const float * const>	GetAudioWaveform(CStringId channelGroup, u32 &outBaseCount) const  override;

	CUnityFrameCollector						*GetFrameCollector() const { return m_ParticleFrameCollector; }

	bool										GameThread_WaitForUpdateEnd();

	//Events
private:
	struct	SPopcornFXEventListener
	{
		CStringId			m_EventName;
		TArray<u32>			m_Keys; // List of unique key
		TArray<u32>			m_EmittersIDs; // Can contain same effect ids, but bound to different delegates
		TArray<CEffectID>	m_EffectIDs; // Can contain same effect ids, but bound to different delegates

		bool				m_Triggered;

		SPopcornFXEventListener(const CStringId &eventName)
		:	m_EventName(eventName)
		,	m_Triggered(false)
		{
		}

		bool	operator==(const PopcornFX::CStringId &eventName) const
		{
			return eventName == m_EventName;
		}
	};

	struct	SPopcornFXEventListenerAssoc
	{
		SPopcornFXEventListener	*m_Event;
		TArray<u32>				m_EmittersIDs;
		TArray<CEffectID>		m_EffectIDs;
		CGuid					m_PayloadViewIndex;

		bool					m_Triggered;

		SPopcornFXEventListenerAssoc(SPopcornFXEventListener *eventListener)
		:	m_Event(eventListener)
		,	m_EffectIDs()
		,	m_PayloadViewIndex(PopcornFX::CGuid::INVALID)
		,	m_Triggered(false)
		{ }

		SPopcornFXEventListenerAssoc(SPopcornFXEventListener *eventListener, const PopcornFX::TArray<CEffectID> &effectIDs, const PopcornFX::CGuid &payloadViewIndex)
		:	m_Event(eventListener)
		,	m_EffectIDs(effectIDs)
		,	m_PayloadViewIndex(payloadViewIndex)
		,	m_Triggered(false)
		{ }
	};

public:
	struct	SPopcornFXPayloadValue
	{
		union
		{
			bool	m_ValueBool[4];
			u32		m_ValueInt[4];
			float	m_ValueFloat[4];
		};
	};

	struct	SPopcornFXPayload
	{
		CStringId						m_PayloadName;
		EBaseTypeID						m_PayloadType;

		TArray<SPopcornFXPayloadValue>	m_Values;

		SPopcornFXPayload(const CStringId &payloadName, const EBaseTypeID payloadType)
		:	m_PayloadName(payloadName)
		,	m_PayloadType(payloadType)
		{
		}

		bool	operator==(const CStringId &payloadName)
		{
			return payloadName == m_PayloadName;
		}
	};

	struct	SPopcornFXPayloadDesc
	{
		const char					*m_Name;
		u32							m_Type;
		u32							m_StartOffSet;
		u32							m_Length;
	};
private:

	struct	SPopcornFXPayloadView
	{
		const CParticleMedium		*m_PayloadMedium;
		u32							m_EventID;

		TArray<SPopcornFXPayload>	m_Payloads;

		u32							m_CurrentParticle;

		SPopcornFXPayloadView()
		:	m_PayloadMedium(null)
		,	m_EventID(0)
		,	m_CurrentParticle(0)
		{
		}

		SPopcornFXPayloadView(const CParticleMedium *medium, u32 eventID)
		:	m_PayloadMedium(medium)
		,	m_EventID(eventID)
		,	m_CurrentParticle(0)
		{
		}

		PK_FORCEINLINE bool						Valid() const { return m_PayloadMedium != null; }
		static SPopcornFXPayloadView			Invalid; // needed by TChunkedSlotArray
	};

	Threads::CCriticalSection					m_RaiseEventLock;
	TArray<SPopcornFXEventListener>				m_EventListeners;
	SPopcornFXPayloadView						*m_CurrentPayloadView;
	TChunkedSlotArray<SPopcornFXPayloadView>	m_PayloadViews;
	TArray<SPopcornFXEventListenerAssoc>		m_PendingEventAssocs;

	template <typename _OutType>
	void	_FillPayload(const PopcornFX::SPayloadElementView &srcPayloadElementData, SPopcornFXPayload &dstPayload);
	void	_FillPayloadBool(const PopcornFX::SPayloadElementView &srcPayloadElementData, SPopcornFXPayload &dstPayload, u32 dim);
	void	_RetrievePayload(const PopcornFX::SPayloadView &srcPayloadView, SPopcornFXPayloadView &dstPayloadView);

	void	_ClearPendingEventsNoLock();
	void	_ClearEvents();
	void	_PostUpdateEvents();
	void	_RemoveUnloadedRenderers(const SParticleCollectedFrameToRender *renderedFrame);

public:
	bool										RegisterEventListener(s32 guid, const CStringId &event, u32 unityKey);
	bool										UnregisterEventListener(s32 guid, const CStringId &event, u32 unityKey);
	bool										UnregisterEventListener(CPKFXEffect* particleEffect, const CStringId &eventNameID, u32 unityKey);
	void										UnregisterEffectAllEventsListeners(u32 guid);
	void										UnregisterEffectAllEventsListeners(CPKFXEffect* particleEffect);
	void										UnregisterAllEventsListeners();

	void										BroadcastEvent(	Threads::SThreadContext *threadCtx, CParticleMedium *parentMedium, u32 eventID,
																CStringId eventName, u32 count, const SUpdateTimeArgs &timeArgs, const TMemoryView<const float> &spawnDtToEnd,
																const TMemoryView<const CEffectID> &effectIDs, const SPayloadView &payloadView);

protected:

	bool								_ResetParticleMediumCollections();

	PMeshNew							m_CollisionMesh;
	float								m_CollisionMeshScale;
	CFloat3								m_CollisionMeshTranslate;
	CFloat4x4							m_CollisionMeshMatrixWithScale;
	CFloat4x4							m_CollisionMeshInvMatrixWithScale;
	bool								m_CollisionMeshUseTransforms;

	TArray<SSceneMeshAssets>			m_SceneMeshAssets;

private:
	bool										m_IsMediumCollectionInitialized;

	// Unity render context:
	SUnityRenderContext							m_RenderContext;
	// Render data factory used for both frame collector
	CUnityRenderDataFactory						m_RenderDataFactory;
	// For regular particles:
	CParticleMediumCollection					*m_ParticleMediumCollection;
	CUnityFrameCollector						*m_ParticleFrameCollector;
	// For mesh particles when the wait on render thread option is enabled:
	CParticleMediumCollection					*m_ParticleMeshMediumCollection;
	CUnityFrameCollector						*m_ParticleMeshFrameCollector;

	EUpdateMode									m_UpdateMode;

	CThreadID									m_RenderThread;
	bool										m_RenderThreadIsSet;

	TArray<SUnitySceneView>						m_SceneViews;

	bool										m_EnableRaycastCollisions;

	bool										m_IsSingleThreaded;
	bool										m_WaitForUpdateOnRenderThread;

	bool										m_GameThreadCalled;

	bool										m_RenderThreadRegistered;

	// EndCollectingFrame callback:
	void										_CollectFrame(CParticleMediumCollection *medCol);

	Threads::CCriticalSection					m_test;
};
PK_DECLARE_REFPTRCLASS(PKFXScene);

//----------------------------------------------------------------------------
__PK_API_END
