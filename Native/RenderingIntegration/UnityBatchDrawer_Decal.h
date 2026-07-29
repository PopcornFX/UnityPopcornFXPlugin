//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once

#include <pk_particles/include/Renderers/ps_renderer_base.h>
#include <pk_render_helpers/include/batch_jobs/rh_batch_jobs_decal_cpu.h>
#include "UnityRendererCache.h"
#include "FrameCollectorUnityTypes.h"
#include "UnityBatchDrawer_Helper.h"

//----------------------------------------------------------------------------

class	CUnityBatchDrawer_Decal : public PopcornFX::CRendererBatchJobs_Decal_CPUBB
{
public:
	CUnityBatchDrawer_Decal();
	~CUnityBatchDrawer_Decal();

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
	void				_ClearBuffers();
	bool				_UpdateThread_IssueDrawCallDecal(const Drawers::SDecal_DrawRequest *decalRequest, CUnityRendererCache *rdrCache, u32 &totalParticleCount);

private:
	TArray<SUnityMeshInfoPerView>	m_UnityMeshInfoPerViews;
	CParticleMaterialDescDecal		m_MaterialDescDecal;
	SUnityMeshInfo					m_UnityMeshInfo;
	CAABB							m_BBox;

	// --------------------------------------
	// Current exact vertex and index count:
	// --------------------------------------
	u32								m_VertexCount;
	u32								m_IndexCount;
	u32								m_ParticleCount;
	u32								m_PrevParticleCount;

	// Can we fill the buffer?
	bool							m_MeshIsValid;

	// --------------------------------------
	// FINAL SOA TO AOS COPY:
	// --------------------------------------

	TArray<CBillboard_Exec_SOA_OAS>		m_Exec_SAO2AOS;

};

//----------------------------------------------------------------------------
