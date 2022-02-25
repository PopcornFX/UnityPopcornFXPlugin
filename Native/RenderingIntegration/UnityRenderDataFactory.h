//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "FrameCollectorUnityTypes.h"
#include "RenderingIntegration/UnityRendererCache.h"
#include <pk_render_helpers/include/frame_collector/rh_particle_render_data_factory.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CUnityBillboardingBatchPolicy;

class	CUnityRenderDataFactory : public TParticleRenderDataFactory<CUnityParticleBatchTypes>
{
public:
	CUnityRenderDataFactory()
	:	m_DelayUnityCallbacks(false)
	,	m_UseGPUBillboarding(false)
	{ }
	virtual ~CUnityRenderDataFactory() { }

	virtual PRendererCacheBase			UpdateThread_CreateRendererCache(const PRendererDataBase &renderer, const CParticleDescriptor *particleDesc) override;
	virtual void						UpdateThread_CollectedForRendering(const PRendererCacheBase &rendererCache) override;
	virtual CBillboardingBatchInterface	*CreateBillboardingBatch(ERendererClass rendererType, const PRendererCacheBase &rendererCache, bool gpuStorage) override;

	void								SetDelayUnityCallbacks(bool delayUnityCallbacks);
	bool								GetDelayUnityCallbacks() const;

	inline void							SetGPUBillboarding(bool use) { m_UseGPUBillboarding = use; }
	inline bool							GetGPUBillboarding() const { return m_UseGPUBillboarding; }

	void								Reset();

	void								EmptyAllBatches();
	void								CustomStepFlagInactive();

private:
	bool								m_DelayUnityCallbacks;
	bool								m_UseGPUBillboarding;

	TArray<PUnityRendererCache>				m_RendererCaches;
	TArray<CUnityBillboardingBatchPolicy*>	m_Batches;
};

//----------------------------------------------------------------------------
__PK_API_END
