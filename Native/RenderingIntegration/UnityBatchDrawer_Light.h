//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once

#include <pk_particles/include/Renderers/ps_renderer_base.h>
#include <pk_render_helpers/include/batch_jobs/rh_batch_jobs_light_std.h>
#include "UnityRendererCache.h"
#include "ManagedToNative.h"
#include "FrameCollectorUnityTypes.h"

//----------------------------------------------------------------------------

class	CUnityBatchDrawer_Light : public PopcornFX::CRendererBatchJobs_Light_Std
{
public:
	CUnityBatchDrawer_Light();
	~CUnityBatchDrawer_Light();

	virtual bool		Setup(const CRendererDataBase *renderer, const CParticleRenderMedium *owner, const PopcornFX::CFrameCollector *fc, const CStringId &storageClass) override;

	virtual bool		AreRenderersCompatible(const PopcornFX::CRendererDataBase *rendererA, const PopcornFX::CRendererDataBase *rendererB) const override;

	virtual bool		AllocBuffers(SRenderContext &ctx) override;
	virtual bool		CanRender(PopcornFX::SRenderContext &ctx) const override;
	virtual bool		MapBuffers(SRenderContext &ctx) override;
	virtual bool		UnmapBuffers(SRenderContext &ctx) override;
	virtual bool		LaunchCustomTasks(SRenderContext &ctx) override;

	virtual void		BeginFrame(PopcornFX::SRenderContext &ctx) override;
	virtual bool		EmitDrawCall(PopcornFX::SRenderContext &ctx, const PopcornFX::SDrawCallDesc &toEmit) override;
	virtual void		Destroy() override;

private:
	void				_OnRenderMediumActiveStateChanged(CParticleRenderMedium *renderMedium, bool active);
	bool				_UpdateThread_IssueDrawCallLight(const Drawers::SLight_DrawRequest *lightRequest, CUnityRendererCache *rdrCache);

private:
	TArray<SLightInfo>				m_LightDatas;

};

//----------------------------------------------------------------------------
