//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------

#pragma once

#include <pk_render_helpers/include/frame_collector/rh_frame_collector.h>
#include <pk_render_helpers/include/frame_collector/rh_batch_drawer.h>
#include <RenderingIntegration/UnityRendererCache.h>

class	IRenderAPIData;

__PK_API_BEGIN
//----------------------------------------------------------------------------

struct	SUnityRenderContext : public PopcornFX::SRenderContext
{
	IRenderAPIData	*m_RenderApiData;
	bool			m_FreeUnusedBatches;
	u32				m_FrameCountBeforeFreeingUnusedBatches;

	SUnityRenderContext()
	:	m_RenderApiData(null)
	,	m_FreeUnusedBatches(false)
	,	m_FrameCountBeforeFreeingUnusedBatches(0)
	{
	}
};

struct	SUnityDrawOutputs
{
};

struct	SViewUserData : public PopcornFX::SSceneView
{
	CGuid		m_CamSlotIdxInMedCol;
	CGuid		m_CamSlotIdxInMeshMedCol;
};

class	CUnityParticleBatchTypes
{
public:
	typedef SUnityRenderContext		CRenderContext;
	typedef SUnityDrawOutputs		CFrameOutputData;
	typedef SViewUserData			CViewUserData;

	enum { kMaxQueuedCollectedFrame = 2U };
};

class	CUnityFrameCollector : public PopcornFX::CFrameCollector
{
public:
	CUnityFrameCollector();

	inline void							SetGPUBillboarding(bool use) { m_UseGPUBillboarding = use; }
	inline bool							GetGPUBillboarding() const { return m_UseGPUBillboarding; }

	PRendererCacheBase					CreateRendererCache(const PRendererDataBase & renderer, const CParticleDescriptor * particleDesc);
	CRendererBatchDrawer				*CreateBillboardingBatch(ERendererClass rendererType, const PRendererCacheBase &rendererCache, bool gpuStorage);

	void								Reset();

	void								RemoveRendererCache(const PCUnityRendererCache &batch);
	void								RemoveBatch(CRendererBatchDrawer *batch);

private:
	bool									m_UseGPUBillboarding;
	Threads::CRWLock						m_CacheLock;
	TArray<PUnityRendererCache>				m_RendererCaches;
	TArray<PRendererBatchDrawer>			m_Batches;
};

// Camera data with custom user data:
typedef SViewUserData				SUnitySceneView;

//----------------------------------------------------------------------------
__PK_API_END
