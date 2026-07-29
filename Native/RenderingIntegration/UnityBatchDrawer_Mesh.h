//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once

#include <pk_kernel/include/kr_refcounted_buffer.h>
#include <pk_particles/include/Renderers/ps_renderer_base.h>
#include <pk_render_helpers/include/batch_jobs/rh_batch_jobs_mesh_cpu.h>
#include "UnityRendererCache.h"
#include "FrameCollectorUnityTypes.h"

//----------------------------------------------------------------------------

class	CUnityBatchDrawer_Mesh : public PopcornFX::CRendererBatchJobs_Mesh_CPUBB
{
public:
	CUnityBatchDrawer_Mesh();
	~CUnityBatchDrawer_Mesh();

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
	void				_UpdateThread_ResizeUnityMeshInstanceCount(const PopcornFX::SRendererBatchDrawPass &drawPasss, IRenderAPIData *renderApiData);
	bool				_RenderThread_SetupBuffersMeshes(const SGeneratedInputs &toMap);

private:
	TArray<SUnityMeshInfoPerView>	m_UnityMeshInfoPerViews;
	CParticleMaterialDescMesh		m_MaterialDescMesh;
	SUnityMeshInfo					m_UnityMeshInfo;

	// --------------------------------------
	// Current exact vertex and index count:
	// --------------------------------------
	u32								m_ParticleCount;
	u32								m_PrevParticleCount;

	// Can we fill the buffer?
	bool							m_MeshIsValid;
	bool							m_HasAtlas;
	bool							m_UseSkeletalAnimData;
	bool							m_UseSkeletalAnimInterpolTracksData;

	bool							m_HasTransformUV;
	bool							m_HasAlphaMasks;
	bool							m_HasUVDistortions;
	bool							m_HasDissolve;
	bool							m_IsEmissive3 = false;

	struct		SMeshParticleBuffers
	{
		TStridedMemoryView<CFloat4x4>			m_Transforms;
		TStridedMemoryView<CFloat4>				m_Colors;
		TStridedMemoryView<CFloat4>				m_EmissiveColors;
		TStridedMemoryView<float>				m_AlphaRemapCursor;
		TStridedMemoryView<float>				m_VATCursors;

		TStridedMemoryView<float>				m_AtlasId;

		// Skeletal anim:
		TStridedMemoryView<u32>					m_AnimIdx0;
		TStridedMemoryView<float>				m_AnimCursor0;
		TStridedMemoryView<u32>					m_AnimIdx1;
		TStridedMemoryView<float>				m_AnimCursor1;
		TStridedMemoryView<float>				m_TransitionCursor;

		TStridedMemoryView<float>				m_TransformUVRotate;
		TStridedMemoryView<CFloat2>				m_TransformUVOffset;
		TStridedMemoryView<CFloat2>				m_TransformUVScale;

		TStridedMemoryView<float>				m_AlphaMask1AnimationCursor;
		TStridedMemoryView<float>				m_AlphaMask2AnimationCursor;
		TStridedMemoryView<float>				m_UVDistortion1AnimationCursor;
		TStridedMemoryView<float>				m_UVDistortion2AnimationCursor;
		TStridedMemoryView<float>				m_DissolveCursor;
		TStridedMemoryView<CFloat2>				m_RawUV0;
	};

	TArray<Drawers::SCopyFieldDescPerMesh>		m_MeshAdditionalField;
	TArray<SMeshParticleBuffers>				m_PerMeshBuffers;
	PRefCountedMemoryBuffer						m_MeshBillboardingBuffer;
};

//----------------------------------------------------------------------------
