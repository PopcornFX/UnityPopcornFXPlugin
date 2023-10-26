//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "RuntimeManager.h"
#include "UnityBillboardingBatchPolicy.h"
#include "UnityRenderDataFactory.h"
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <pk_render_helpers/include/render_features/rh_features_vat_static.h>
#include <pk_render_helpers/include/render_features/rh_features_vat_skeletal.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

CUnityBillboardingBatchPolicy::CUnityBillboardingBatchPolicy()
:	m_RenderDataFactory(null)
,	m_RendererType(Renderer_Invalid)
,	m_BBox(CAABB::DEGENERATED)
,	m_GPUBillboarding(false)
,	m_VertexCount(0)
,	m_IndexCount(0)
,	m_ParticleCount(0)
,	m_PrevParticleCount(0)
,	m_MeshIsValid(false)
,	m_UseSkeletalAnimData(false)
,	m_UseSkeletalAnimInterpolTracksData(false)
,	m_UnusedFrameCount(0)
,	m_MeshBillboardingBuffer(null)
,	m_MappedIdxBuffer(null)
,	m_MappedInfoBillboardBuffer(null)
,	m_MappedAtlasesBuffer(null)
{
}

CUnityBillboardingBatchPolicy::~CUnityBillboardingBatchPolicy()
{
	CRuntimeManager	&manager = CRuntimeManager::Instance();
	for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
	{
		if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
			manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_False);
	}
	if (PK_VERIFY(m_RenderDataFactory != null))
		m_RenderDataFactory->RemoveBatch(this);
}

bool CUnityBillboardingBatchPolicy::Init(UnityGfxRenderer deviceType, CUnityRenderDataFactory *renderDataFactory)
{
	(void)deviceType;
	m_RenderDataFactory = renderDataFactory;
	//At least one
	if (!m_UnityMeshInfoPerViews.Resize(1) ||
		!m_Exec_SAO2AOS.Resize(1) ||
		!m_MappedVtxBuffer.Resize(1))
		return false;
	return true;
}

//----------------------------------------------------------------------------
// UPDATE THREAD:
//----------------------------------------------------------------------------

bool	CUnityBillboardingBatchPolicy::CanRender(const Drawers::SBillboard_DrawRequest *, const PRendererCacheBase &, SUnityRenderContext &)
{
	return true;
}

bool	CUnityBillboardingBatchPolicy::CanRender(const Drawers::SRibbon_DrawRequest *, const PRendererCacheBase &, SUnityRenderContext &)
{
	return true;
}

bool	CUnityBillboardingBatchPolicy::CanRender(const Drawers::SMesh_DrawRequest *, const PRendererCacheBase &, SUnityRenderContext &)
{
	return true;
}

bool	CUnityBillboardingBatchPolicy::CanRender(const Drawers::SLight_DrawRequest *, const PRendererCacheBase &, SUnityRenderContext &)
{
	return true;
}

bool	CUnityBillboardingBatchPolicy::CanRender(const Drawers::SSound_DrawRequest *, const PRendererCacheBase &, SUnityRenderContext &)
{
	return true;
}

bool	CUnityBillboardingBatchPolicy::CanRender(const Drawers::STriangle_DrawRequest *, const PRendererCacheBase &, SUnityRenderContext &)
{
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBillboardingBatchPolicy::Tick(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &)
{
	if (m_ParticleCount == 0)
		++m_UnusedFrameCount;
	else
		m_UnusedFrameCount = 0;
	return ctx.m_FreeUnusedBatches ? m_UnusedFrameCount < ctx.m_FrameCountBeforeFreeingUnusedBatches: true;
}

void	CUnityBillboardingBatchPolicy::ClearBatch()
{
	m_ParticleCount = 0;
	CustomStepFlagInactive();
}

void	CUnityBillboardingBatchPolicy::CustomStepFlagInactive()
{
	CRuntimeManager	&manager = CRuntimeManager::Instance();

	if (m_ParticleCount == 0 && m_PrevParticleCount != 0)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
			manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_False);
	}
}

bool	CUnityBillboardingBatchPolicy::AllocBuffers(SUnityRenderContext &ctx, const SBuffersToAlloc &allocBuffers, const TMemoryView<SUnitySceneView> &views, ERendererClass rendererType)
{
	PK_SCOPEDPROFILE();
	(void)views;
	PK_ASSERT(!allocBuffers.m_DrawRequests.Empty());
	PK_ASSERT(allocBuffers.m_DrawRequests.Count() == allocBuffers.m_RendererCaches.Count());
	PK_ASSERT((allocBuffers.m_TotalVertexCount > 0 && allocBuffers.m_TotalIndexCount > 0) || allocBuffers.m_TotalParticleCount > 0);
	PK_ASSERT(allocBuffers.m_DrawRequests.First() != null);
	if (!PK_VERIFY(ctx.m_RenderApiData != null))
	{
		CLog::Log(PK_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}

	// The first time the batch is used, we store the material desc in the batch (all the material descs should be the same):
	if (m_RendererType == Renderer_Invalid)
	{
		const CUnityRendererCache		*rendererCache = static_cast<const CUnityRendererCache *>(allocBuffers.m_RendererCaches.First().Get());

		m_RendererType = rendererCache->m_RendererType;
		m_MaterialDescBillboard = rendererCache->m_MaterialDescBillboard;
		m_MaterialDescMesh = rendererCache->m_MaterialDescMesh;

		u32 viewCount = m_RendererType == Renderer_Mesh ? 1 : views.Count();

		PK_ASSERT(viewCount > 0);
		if (!PK_VERIFY(m_Exec_SAO2AOS.Resize(viewCount)) ||
			!PK_VERIFY(m_UnityMeshInfoPerViews.Resize(viewCount)))
			return false;

		for (u32 i = 0; i < viewCount; ++i)
		{
			m_Exec_SAO2AOS[i].Clear();
			m_UnityMeshInfoPerViews[i] = rendererCache->m_UnityMeshInfoPerViews[i];
		}
		m_UnityMeshInfo = rendererCache->m_UnityMeshInfo;
	}

	m_ParticleCount = allocBuffers.m_TotalParticleCount;

	_UpdateThread_SetUnityMeshBounds(allocBuffers, views);

	if (rendererType == Renderer_Billboard || rendererType == Renderer_Ribbon || rendererType == Renderer_Triangle)
	{
		m_VertexCount = allocBuffers.m_TotalVertexCount;
		m_IndexCount = allocBuffers.m_TotalIndexCount;
		_UpdateThread_ResizeUnityMesh(allocBuffers, ctx.m_RenderApiData);

		if (!allocBuffers.m_DrawRequests.Empty())
		{
			const Drawers::SBase_DrawRequest	*dr = allocBuffers.m_DrawRequests.First();
			if (PK_VERIFY(dr != null))
			{
				if (rendererType == Renderer_Billboard)
				{
					const Drawers::SBillboard_BillboardingRequest	*bbRequest = checked_cast<const Drawers::SBillboard_BillboardingRequest*>(&dr->BaseBillboardingRequest());
					if (PK_VERIFY(bbRequest != null))
						m_AtlasList = bbRequest->m_Atlas;
				}
				else if (rendererType == Renderer_Ribbon)
				{
					const Drawers::SRibbon_BillboardingRequest		*bbRequest = checked_cast<const Drawers::SRibbon_BillboardingRequest*>(&dr->BaseBillboardingRequest());
					if (PK_VERIFY(bbRequest != null))
						m_AtlasList = bbRequest->m_Atlas;
				}
			}
		}
	}
	else if (rendererType == Renderer_Mesh)
	{
		_UpdateThread_ResizeUnityMeshInstanceCount(allocBuffers, ctx.m_RenderApiData);
	}
	return true;
}

//----------------------------------------------------------------------------
// RENDER THREAD:
//----------------------------------------------------------------------------

bool	CUnityBillboardingBatchPolicy::MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SBillboardBatchJobs *batchJobs, const SGeneratedInputs &toMap)
{
	PK_SCOPEDPROFILE();
	(void)views;
	if (!PK_VERIFY(ctx.m_RenderApiData != null))
	{
		CLog::Log(PK_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}

	if (!m_MeshIsValid)
		return true;

	// Here we map the Unity buffers:
	const u32	vboFullSize = m_UnityMeshInfo.m_VBElemCount * m_UnityMeshInfo.m_VertexStride;
	const u32	vboMapSize = m_VertexCount * m_UnityMeshInfo.m_VertexStride;

	// Alloc the buffers:
	if (!PK_VERIFY(_RenderThread_AllocBillboardingBuffers(toMap, ctx.m_RenderApiData)))
		return false;
	if (!PK_VERIFY(_RenderThread_AllocPerViewGeomBuffers(toMap, ctx.m_RenderApiData)))
		return false;

	// Setup the memory view in the jobs:
	if (!PK_VERIFY(_RenderThread_SetupBuffersBillboards(toMap, batchJobs)))
		return false;
	// Map the final buffers:
	if (m_MappedVtxBuffer.Count() < views.Count())
	{
		if (!PK_VERIFY(m_MappedVtxBuffer.Resize(views.Count())))
			return false;// To review, Can we do this alloc somewhere else ?
	}
	for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
	{
		m_MappedVtxBuffer[i] = ctx.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_VBHandler, false, vboFullSize, vboMapSize);
		if (!PK_VERIFY(m_MappedVtxBuffer[i] != null))
			return false;
	}
	return true;
}

bool	CUnityBillboardingBatchPolicy::MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SGPUBillboardBatchJobs *batchJobs, const SGeneratedInputs &toMap)
{
	PK_SCOPEDPROFILE();
	(void)views;
	if (!PK_VERIFY(ctx.m_RenderApiData != null))
	{
		CLog::Log(PK_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}

	if (!m_MeshIsValid)
		return true;

	m_GPUBillboarding = true;
	// VMN: Not sure if we want to alloc all of them
	if (!PK_VERIFY(_RenderThread_AllocPerViewGeomBuffers(toMap, ctx.m_RenderApiData)))
		return false;
	if (m_AtlasList != null)
	{
		//VMN: Should be done once.
		m_MappedAtlasesBuffer = ctx.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_AtlasesHandler, false, m_UnityMeshInfo.m_AtlasesSize, m_UnityMeshInfo.m_AtlasesSize);
		if (!PK_VERIFY(m_MappedAtlasesBuffer != null))
			return false;

		u32		*count = reinterpret_cast<u32*>(Mem::AdvanceRawPointer(m_MappedAtlasesBuffer, 0));
		CFloat4	*data = reinterpret_cast<CFloat4*>(Mem::AdvanceRawPointer(m_MappedAtlasesBuffer, sizeof(CUint4))); // Alignement
		*count = m_AtlasList->m_RectsFp32.Count();
		PK_ASSERT(*count < 0x100);
		PK_ASSERT(m_AtlasList->m_RectsFp32.Stride() == sizeof(CFloat4));
		Mem::Copy(data, m_AtlasList->m_RectsFp32.RawDataPointer(), m_AtlasList->m_RectsFp32.CoveredBytes());

		if (m_MappedAtlasesBuffer != null)
			ctx.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_AtlasesHandler, false);

		m_AtlasList = null;
	}

	// Here we map the Unity buffers:
	const u32	vboFullSize = m_UnityMeshInfo.m_VBElemCount * m_UnityMeshInfo.m_VertexStride;
	const u32	vboMapSize = m_VertexCount * m_UnityMeshInfo.m_VertexStride;
	const u32	iboFullSize = m_UnityMeshInfo.m_IBElemCount * (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True ? 4 : 2);
	const u32	iboMapSize = m_IndexCount * (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True ? 4 : 2);

	// Map the final buffers:
	m_MappedVtxBuffer[0] = ctx.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_VBHandler, false, vboFullSize, vboMapSize);
	if (!PK_VERIFY(m_MappedVtxBuffer[0] != null))
		return false;
	m_MappedIdxBuffer = ctx.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_IBHandler, true, iboFullSize, iboMapSize);
	if (!PK_VERIFY(m_MappedIdxBuffer != null))
		return false;
	m_MappedInfoBillboardBuffer = ctx.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_InfoHandler, false, m_UnityMeshInfo.m_InfoSize, m_UnityMeshInfo.m_InfoSize);
	if (!PK_VERIFY(m_MappedInfoBillboardBuffer != null))
		return false;

	if (!_RenderThread_SetupBuffersGeomBillboards(toMap, batchJobs))
		return false;

	return true;
}

bool	CUnityBillboardingBatchPolicy::MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SRibbonBatchJobs *batchJobs, const SGeneratedInputs &toMap)
{
	PK_SCOPEDPROFILE();
	(void)views;
	if (!PK_VERIFY(ctx.m_RenderApiData != null))
	{
		CLog::Log(PK_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}

	if (!m_MeshIsValid)
		return true;

	// Here we map the Unity buffers:
	const u32	vboFullSize = m_UnityMeshInfo.m_VBElemCount * m_UnityMeshInfo.m_VertexStride;
	const u32	vboMapSize = m_VertexCount * m_UnityMeshInfo.m_VertexStride;

	// Alloc the buffers:
	if (!PK_VERIFY(_RenderThread_AllocBillboardingBuffers(toMap, ctx.m_RenderApiData)))
		return false;
	if (!PK_VERIFY(_RenderThread_AllocPerViewGeomBuffers(toMap, ctx.m_RenderApiData)))
		return false;

	// Setup the memory view in the jobs:
	if (!PK_VERIFY(_RenderThread_SetupBuffersRibbons(toMap, batchJobs)))
		return false;

	// Map the final buffers:
	if (m_MappedVtxBuffer.Count() < views.Count())
	{
		if (!PK_VERIFY(m_MappedVtxBuffer.Resize(views.Count())))
			return false;
	}
		
	for (u32 i = 0; i < views.Count(); ++i)
	{
		m_MappedVtxBuffer[i] = ctx.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_VBHandler, false, vboFullSize, vboMapSize);
		if (!PK_VERIFY(m_MappedVtxBuffer[i] != null))
			return false;
	}
	return true;
}

bool	CUnityBillboardingBatchPolicy::MapBuffers(SUnityRenderContext & ctx, const TMemoryView<SUnitySceneView>& views, SGPURibbonBatchJobs * batchJobs, const SGeneratedInputs & toMap)
{
	(void)ctx;
	(void)views;
	(void)batchJobs;
	(void)toMap;
	return true;
}

bool	CUnityBillboardingBatchPolicy::MapBuffers(SUnityRenderContext &ctx, const TMemoryView<SUnitySceneView> &views, SMeshBatchJobs *batchJobs, const SGeneratedInputs &toMap)
{
	PK_SCOPEDPROFILE();
	(void)ctx; (void)views;
	if (!m_MeshIsValid)
		return true;
	return _RenderThread_SetupBuffersMeshes(toMap, batchJobs);
}

bool	CUnityBillboardingBatchPolicy::MapBuffers(SUnityRenderContext & ctx, const TMemoryView<SUnitySceneView>& views, STriangleBatchJobs * batchJobs, const SGeneratedInputs & toMap)
{
	PK_SCOPEDPROFILE();
	(void)views;
	if (!PK_VERIFY(ctx.m_RenderApiData != null))
	{
		CLog::Log(PK_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}

	if (!m_MeshIsValid)
		return true;

	// Here we map the Unity buffers:
	const u32	vboFullSize = m_UnityMeshInfo.m_VBElemCount * m_UnityMeshInfo.m_VertexStride;
	const u32	vboMapSize = m_VertexCount * m_UnityMeshInfo.m_VertexStride;

	// Alloc the buffers:
	if (!PK_VERIFY(_RenderThread_AllocBillboardingBuffers(toMap, ctx.m_RenderApiData)))
		return false;
	if (!PK_VERIFY(_RenderThread_AllocPerViewGeomBuffers(toMap, ctx.m_RenderApiData)))
		return false;

	// Setup the memory view in the jobs:
	if (!PK_VERIFY(_RenderThread_SetupBuffersTriangles(toMap, batchJobs)))
		return false;
	// Map the final buffers:
	if (m_MappedVtxBuffer.Count() < views.Count())
	{
		if (!PK_VERIFY(m_MappedVtxBuffer.Resize(views.Count())))
			return false;
	}
	for (u32 i = 0; i < views.Count(); ++i)
	{
		m_MappedVtxBuffer[i] = ctx.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_VBHandler, false, vboFullSize, vboMapSize);
		if (!PK_VERIFY(m_MappedVtxBuffer[i] != null))
			return false;
	}
	return true;
}

bool	CUnityBillboardingBatchPolicy::MapBuffers(SUnityRenderContext & ctx, const TMemoryView<SUnitySceneView>& views, SGPUTriangleBatchJobs * batchJobs, const SGeneratedInputs & toMap)
{
	(void)ctx; (void)views; (void)batchJobs; (void)toMap;
	return true;
}

bool	CUnityBillboardingBatchPolicy::LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SBillboard_DrawRequest * const> &drawRequests, Drawers::CCopyStream_CPU *batch)
{
	(void)ctx; (void)drawRequests; (void)batch;
	if (!m_MeshIsValid)
		return true;
	CBilboard_Exec_BillboardInfo::SBillboardInfo	*mappedInfo = static_cast<CBilboard_Exec_BillboardInfo::SBillboardInfo*>(m_MappedInfoBillboardBuffer);
	m_Exec_BillboardInfo.Clear();
	m_Exec_BillboardInfo.m_GeomDrawRequests = TMemoryView<CBilboard_Exec_BillboardInfo::SBillboardInfo>(mappedInfo, 256);
	batch->AddExecDuringPages(&m_Exec_BillboardInfo);
	return true;
}

bool	CUnityBillboardingBatchPolicy::LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::STriangle_DrawRequest * const> &drawRequests, Drawers::CTriangle_CPU *batch)
{
	(void)drawRequests;
	if (!m_MeshIsValid)
		return true;

	for (u32 i = 0; i < m_Exec_SAO2AOS.Count(); ++i)
	{
		if (_PrepareCopySOA2AOS(ctx, i) && m_Exec_SAO2AOS[i].m_MappedVertexBuffer[i] != null)
			batch->AddExecAfterPage(&(m_Exec_SAO2AOS[i]));
	}
	return true;
}

bool	CUnityBillboardingBatchPolicy::LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::STriangle_DrawRequest * const> &drawRequests, Drawers::CCopyStream_CPU *batch)
{
	(void)ctx; (void)drawRequests; (void)batch;
	return true;
}

bool	CUnityBillboardingBatchPolicy::LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SBillboard_DrawRequest * const> &drawRequests, Drawers::CBillboard_CPU *batch)
{
	(void)drawRequests;
	if (!m_MeshIsValid)
		return true;

	for (u32 i = 0; i < m_Exec_SAO2AOS.Count(); ++i)
	{
		if (_PrepareCopySOA2AOS(ctx, i) && m_Exec_SAO2AOS[i].m_MappedVertexBuffer[i] != null)
			batch->AddExecLatePage(&(m_Exec_SAO2AOS[i]));
	}
	return true;
}

bool	CUnityBillboardingBatchPolicy::LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SRibbon_DrawRequest * const> &drawRequests, Drawers::CRibbon_CPU *batch)
{
	(void)drawRequests;
	if (!m_MeshIsValid)
		return true;

	for (u32 i = 0; i < m_Exec_SAO2AOS.Count(); ++i)
	{
		if (_PrepareCopySOA2AOS(ctx, i) && m_Exec_SAO2AOS[i].m_MappedVertexBuffer[i] != null)
			batch->AddExecBatch(&(m_Exec_SAO2AOS[i]));
	}
	return true;
}

bool	CUnityBillboardingBatchPolicy::LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SRibbon_DrawRequest * const> &drawRequests, Drawers::CCopyStream_CPU *batch)
{
	// Not implemented 
	(void)ctx;
	(void)drawRequests;
	(void)batch;
	return true;
} 

bool	CUnityBillboardingBatchPolicy::LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SMesh_DrawRequest * const> &drawRequests, Drawers::CMesh_CPU *batch)
{
	(void)ctx; (void)drawRequests; (void)batch;
	return true;
}

bool	CUnityBillboardingBatchPolicy::LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SLight_DrawRequest * const> &drawRequests, Drawers::CBillboard_CPU *batch)
{
	(void)ctx; (void)drawRequests; (void)batch;
	return true;
}

bool	CUnityBillboardingBatchPolicy::LaunchCustomTasks(SUnityRenderContext &ctx, const TMemoryView<const Drawers::SSound_DrawRequest * const> &drawRequests, Drawers::CBillboard_CPU *batch)
{
	(void)ctx; (void)drawRequests; (void)batch;
	return true;
}

bool	CUnityBillboardingBatchPolicy::WaitForCustomTasks(SUnityRenderContext &ctx)
{
	(void)ctx;
	return true;
}

bool	CUnityBillboardingBatchPolicy::UnmapBuffers(SUnityRenderContext &ctx)
{
	PK_SCOPEDPROFILE();
	if (!m_MeshIsValid)
		return true;
	if (m_RendererType == Renderer_Billboard || m_RendererType == Renderer_Ribbon || m_RendererType == Renderer_Triangle)
	{
		// Unmap the final buffers:
		for (u32 i = 0; i < m_MappedVtxBuffer.Count(); ++i)
		{
			if (m_MappedVtxBuffer[i] != null)
			{
				ctx.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_VBHandler, false);
			}
		}
			

		if (!m_GPUBillboarding)
		{
			// Mem copy the indices:
			const u32	iboFullSize = m_UnityMeshInfo.m_IBElemCount * (m_UnityMeshInfo.m_LargeIndices ? sizeof(u32) : sizeof(u16));
			const u32	srcIdxSize = m_IndexCount * (m_UnityMeshInfo.m_LargeIndices ? sizeof(u32) : sizeof(u16));

			for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
			{
				void		*indices = ctx.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_IBHandler, true, iboFullSize, iboFullSize);

				if (!PK_VERIFY(indices != null))
					continue;

				Mem::Copy(indices, m_Exec_SAO2AOS[i].m_ParticleBuffers.m_Indices, srcIdxSize);
				Mem::Clear(Mem::AdvanceRawPointer(indices, srcIdxSize), iboFullSize - srcIdxSize);

				ctx.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_IBHandler, true);
			}
		}
		else
		{
			ctx.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_InfoHandler, false);
			m_MappedInfoBillboardBuffer = null;
			ctx.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_IBHandler, true);
			m_MappedIdxBuffer = null;
		}
		for (u32 i = 0; i < m_MappedVtxBuffer.Count(); ++i)
			m_MappedVtxBuffer[i] = null;
	}
	return true;
}

void	CUnityBillboardingBatchPolicy::ClearBuffers(SUnityRenderContext &ctx)
{
	(void)ctx;
	m_ParticleBuffers.m_AdditionalFieldsBuffers.Clear();
	m_IndexCount = 0;
	m_VertexCount = 0;
	m_PrevParticleCount = m_ParticleCount;
	m_ParticleCount = 0;
}

bool	CUnityBillboardingBatchPolicy::AreBillboardingBatchable(const PCRendererCacheBase &firstCache, const PCRendererCacheBase &secondCache) const
{
	const CUnityRendererCache	*firstMaterialDesc = static_cast<const CUnityRendererCache *>(firstCache.Get());
	const CUnityRendererCache	*secondMaterialDesc = static_cast<const CUnityRendererCache *>(secondCache.Get());
	return firstMaterialDesc == secondMaterialDesc || *firstMaterialDesc == *secondMaterialDesc;
}

bool	CUnityBillboardingBatchPolicy::EmitDrawCall(SUnityRenderContext &ctx, const SDrawCallDesc &toEmit, SUnityDrawOutputs &output)
{
	(void)ctx; (void)toEmit; (void)output;
	return true;
}

void	CUnityBillboardingBatchPolicy::_UpdateThread_ResizeUnityMesh(const SBuffersToAlloc &allocBuffers, IRenderAPIData *renderApiData)
{
	PK_SCOPEDPROFILE();
	(void)allocBuffers; (void)renderApiData;
	m_MeshIsValid = true;
	CRuntimeManager	&manager = CRuntimeManager::Instance();

	// If the Unity mesh is too small, we resize it:
	if ((u32)m_UnityMeshInfo.m_VBElemCount < m_VertexCount || (u32)m_UnityMeshInfo.m_IBElemCount < m_IndexCount)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			SRetrieveRendererInfo	rendererInfo;

			rendererInfo.m_VBHandler = &m_UnityMeshInfoPerViews[i].m_VBHandler.m_Buffer->m_DeviceLocal;
			rendererInfo.m_IBHandler = &m_UnityMeshInfoPerViews[i].m_IBHandler.m_Buffer->m_DeviceLocal;
			rendererInfo.m_VertexBufferSize = &m_UnityMeshInfo.m_VBElemCount;
			rendererInfo.m_IndexBufferSize = &m_UnityMeshInfo.m_IBElemCount;
			rendererInfo.m_IsIndex32 = &m_UnityMeshInfo.m_LargeIndices;

			rendererInfo.m_InfoBSize = &m_UnityMeshInfo.m_InfoSize;
			rendererInfo.m_InfoBHandler = &m_UnityMeshInfoPerViews[i].m_InfoHandler.m_Buffer->m_DeviceLocal;
			rendererInfo.m_AtlasesBSize = &m_UnityMeshInfo.m_AtlasesSize;
			rendererInfo.m_AtlasesBHandler = &m_UnityMeshInfoPerViews[i].m_AtlasesHandler.m_Buffer->m_DeviceLocal;

			rendererInfo.m_IndirectArgsBHandler = &m_UnityMeshInfoPerViews[i].m_IndirectArgsHandler.m_Buffer->m_DeviceLocal;
			rendererInfo.m_IndirectArgsParticleCountMultiplier = &m_UnityMeshInfo.m_IndirectArgsParticleCountMultiplier;

			// Only checked at creation of renderer cache:
			rendererInfo.m_UseComputeBuffers = null;
			rendererInfo.m_HasCustomMaterial = null;
			m_MeshIsValid = manager.OnResizeRenderer(m_UnityMeshInfoPerViews[i].m_RendererGUID, m_ParticleCount, m_VertexCount, m_IndexCount, &rendererInfo, &m_MeshIsValid);
		}
	}
	else if (m_GPUBillboarding)
	{
		if (CCurrentThread::IsMainThread())
		{
			::OnSetParticleCount(m_UnityMeshInfoPerViews[0].m_RendererGUID, m_ParticleCount);
		}
		else // is render thread
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[0].m_IndirectArgsHandler.m_Buffer->m_DeviceLocal != null))
			{
				const u32	indirectArgsSize = 5 * sizeof(u32);
				u32			*mappedArgs = static_cast<u32*>(renderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_IndirectArgsHandler, false, indirectArgsSize, indirectArgsSize));

				mappedArgs[0] = m_ParticleCount * m_UnityMeshInfo.m_IndirectArgsParticleCountMultiplier;
				mappedArgs[1] = 1;
				mappedArgs[2] = 0;
				mappedArgs[3] = 0;
				mappedArgs[4] = 0;
				renderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_IndirectArgsHandler, false);
			}
		}
	}
	if (m_PrevParticleCount == 0 && m_ParticleCount != 0)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
			manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_True);
	}
}

void	CUnityBillboardingBatchPolicy::_UpdateThread_SetUnityMeshBounds(const SBuffersToAlloc &allocBuffers, const TMemoryView<SUnitySceneView> &views)
{
	PK_SCOPEDPROFILE();
	// No need to update the bounds for the meshes: Unity doesn't need the global draw call bounds
	if (m_RendererType == Renderer_Mesh)
		return;

	CAABB	bbox = CAABB::DEGENERATED;

	for (u32 i = 0; i < allocBuffers.m_DrawRequests.Count(); ++i)
	{
		bbox.Add(allocBuffers.m_DrawRequests[i]->StreamToRender().BBox());
	}

	CRuntimeManager	&manager = CRuntimeManager::Instance();

	if (bbox == CAABB::DEGENERATED)
	{
		SUpdateRendererBounds	bounds;

		bounds.m_Min = CFloat3::ZERO;
		bounds.m_Max = CFloat3::ZERO;
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
			manager.OnUpdateRendererBounds(m_UnityMeshInfoPerViews[i].m_RendererGUID, &bounds);
		
		m_BBox = CAABB::ZERO;
	}
	else if (m_BBox != bbox)
	{
		SUpdateRendererBounds	bounds;
		// Apply the camera offset on unity mesh bounds. This allows to apply the camera sort offset in unity.
		// It may have repercussions on the culling
		float cameraSortOffset = allocBuffers.m_DrawRequests.First()->BaseBillboardingRequest().m_CameraSortOffset;

		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
			{
				const CFloat3 &viewDirection = views[i].m_InvViewMatrix.StrippedZAxis();
				bounds.m_Min = bbox.Min() - cameraSortOffset * viewDirection;
				bounds.m_Max = bbox.Max() - cameraSortOffset * viewDirection;
				manager.OnUpdateRendererBounds(m_UnityMeshInfoPerViews[i].m_RendererGUID, &bounds);
			}
		}
		m_BBox = bbox;
	}
}

void	CUnityBillboardingBatchPolicy::_UpdateThread_ResizeUnityMeshInstanceCount(const SBuffersToAlloc &allocBuffers, IRenderAPIData *renderApiData)
{
	(void)renderApiData;
	PK_SCOPEDPROFILE();
	u32		perMeshDataSize = sizeof(CFloat4x4);
	bool	hasDiffuseColor = false;
	bool	hasEmissiveColor = false;
	bool	hasAlphaRemap = false;
	bool	hasVAT = false;
	u32		skeletalAnimMask = 0;

	PK_ASSERT((allocBuffers.m_ToGenerate.m_GeneratedInputs & Drawers::GenInput_Matrices) != 0);
	for (u32 i = 0; i < allocBuffers.m_ToGenerate.m_AdditionalGeneratedInputs.Count(); ++i)
	{
		const SRendererFeatureFieldDefinition	&addInput = allocBuffers.m_ToGenerate.m_AdditionalGeneratedInputs[i];

		if (addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() && addInput.m_Type == BaseType_Float4)
		{
			perMeshDataSize += sizeof(CFloat4);
			hasDiffuseColor = true;
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Emissive_EmissiveColor() && addInput.m_Type == BaseType_Float3)
		{
			perMeshDataSize += sizeof(CFloat3);
			hasEmissiveColor = true;
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Atlas_TextureID() && addInput.m_Type == BaseType_Float)
		{
			perMeshDataSize += sizeof(float);
			m_HasAtlas = true;
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaRemap_Cursor() && addInput.m_Type == BaseType_Float)
		{
			perMeshDataSize += sizeof(float);
			hasAlphaRemap = true;
		}
		else if ((addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_Cursor()
				|| addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Soft_Cursor()
				|| addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_Cursor())
				&& addInput.m_Type == BaseType_Float)
		{
			perMeshDataSize += sizeof(float);
			hasVAT = true;
		}
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimation_AnimationCursor() && addInput.m_Type == BaseType_Float)
			skeletalAnimMask |= (1 << 0);
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimation_CurrentAnimTrack() && addInput.m_Type == BaseType_I32)
			skeletalAnimMask |= (1 << 1);
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_NextAnimationCursor() && addInput.m_Type == BaseType_Float)
			skeletalAnimMask |= (1 << 2);
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_NextAnimTrack() && addInput.m_Type == BaseType_I32)
			skeletalAnimMask |= (1 << 3);
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_TransitionRatio() && addInput.m_Type == BaseType_Float)
			skeletalAnimMask |= (1 << 4);
	}

	m_UseSkeletalAnimData = (skeletalAnimMask & 0x3) == 0x3;
	m_UseSkeletalAnimInterpolTracksData = (skeletalAnimMask & 0x1F) == 0x1F;

	if (m_UseSkeletalAnimData)
		perMeshDataSize += sizeof(CUint2);
	if (m_UseSkeletalAnimInterpolTracksData)
		perMeshDataSize += sizeof(CUint3);

	int	rdrGUID = m_UnityMeshInfoPerViews[0].m_RendererGUID;

	u32			overEstimatedInstanceCount = Mem::Align(allocBuffers.m_TotalParticleCount, 0x100);
	u32			buffSize = overEstimatedInstanceCount * perMeshDataSize;

	if (m_MeshBillboardingBuffer == null || m_MeshBillboardingBuffer->DataSizeInBytes() < buffSize)
		m_MeshBillboardingBuffer = CRefCountedMemoryBuffer::AllocAligned(buffSize, 0x10);

	// Allocations can fail below, if the function returns because of this, m_MeshIsValid will be false
	m_MeshIsValid = false;

	if (!PK_VERIFY(m_PerMeshBuffers.Resize(allocBuffers.m_PerMeshParticleCount.Count())))
		return;

	void *currentBuffPtr = m_MeshBillboardingBuffer->Data<void>();
	for (u32 subMesh = 0; subMesh < allocBuffers.m_PerMeshParticleCount.Count(); ++subMesh)
	{
		u32						particleCount = allocBuffers.m_PerMeshParticleCount[subMesh];
		SMeshParticleBuffers	&meshBuff = m_PerMeshBuffers[subMesh];
		void					*inputOffset = Mem::AdvanceRawPointer(currentBuffPtr, allocBuffers.m_PerMeshBufferOffset[subMesh] * perMeshDataSize);

		::OnSetMeshInstancesBuffer(rdrGUID, subMesh, inputOffset);
		// Transform matrices:
		meshBuff.m_Transforms = TMemoryView<CFloat4x4>(static_cast<CFloat4x4*>(inputOffset), particleCount);
		inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(CFloat4x4));
		if (hasDiffuseColor)
		{
			// Diffuse colors:
			meshBuff.m_Colors = TMemoryView<CFloat4>(static_cast<CFloat4*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(CFloat4));
		}
		if (hasEmissiveColor)
		{
			// Emissive colors:
			meshBuff.m_EmissiveColors = TMemoryView<CFloat3>(static_cast<CFloat3*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(CFloat3));
		}
		if (hasAlphaRemap)
		{
			// Alpharemap cursor:
			meshBuff.m_AlphaRemapCursor = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (hasVAT)
		{
			// VAT cursor:
			meshBuff.m_VATCursors = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (m_HasAtlas)
		{
			// Atlas ID:
			meshBuff.m_AtlasId = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (m_UseSkeletalAnimData)
		{
			meshBuff.m_AnimIdx0 = TMemoryView<u32>(static_cast<u32*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(u32));
			meshBuff.m_AnimCursor0 = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (m_UseSkeletalAnimInterpolTracksData)
		{
			meshBuff.m_AnimIdx1 = TMemoryView<u32>(static_cast<u32*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(u32));
			meshBuff.m_AnimCursor1 = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
			meshBuff.m_TransitionCursor = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		::OnSetMeshInstancesCount(rdrGUID, subMesh, particleCount);
	}

	// Restore m_MeshIsValid
	m_MeshIsValid = true;

	if (m_PrevParticleCount == 0 && m_ParticleCount != 0)
	{
		CRuntimeManager	&manager = CRuntimeManager::Instance();
		manager.OnSetRendererActive(m_UnityMeshInfoPerViews[0].m_RendererGUID, ManagedBool_True);
	}
}

bool	CUnityBillboardingBatchPolicy::_RenderThread_AllocPerViewGeomBuffers(const SGeneratedInputs &genInputs, IRenderAPIData *renderApiData)
{
	(void)renderApiData;
	PK_SCOPEDPROFILE();
	// -----------------------------------------
	// View dependent inputs:
	// -----------------------------------------
	u32		viewIdx = 0;

	if (!m_ParticleBuffers.m_PerViewGeom.Resize(genInputs.m_PerViewGeneratedInputs.Count()))
		return false;
	PK_FOREACH(perViewBuff, genInputs.m_PerViewGeneratedInputs)
	{
		SParticleBuffers::SPerView	 &curViewBuffs = m_ParticleBuffers.m_PerViewGeom[viewIdx];

		curViewBuffs.m_Positions.ResizeIFN(Drawers::GenInput_Position, perViewBuff->m_GeneratedInputs, curViewBuffs.m_GeneratedInputs, m_VertexCount);
		curViewBuffs.m_Normals.ResizeIFN(Drawers::GenInput_Normal, perViewBuff->m_GeneratedInputs, curViewBuffs.m_GeneratedInputs, m_VertexCount);
		curViewBuffs.m_Tangents.ResizeIFN(Drawers::GenInput_Tangent, perViewBuff->m_GeneratedInputs, curViewBuffs.m_GeneratedInputs, m_VertexCount);
		// TODO: implem tangents
		curViewBuffs.m_UVFactors.ResizeIFN(Drawers::GenInput_UVFactors, perViewBuff->m_GeneratedInputs, curViewBuffs.m_GeneratedInputs, m_VertexCount);

		curViewBuffs.m_GeneratedInputs = perViewBuff->m_GeneratedInputs;
		++viewIdx;
	}

	const u32	indexStride = (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True ? sizeof(u32) : sizeof(u16));

	// -----------------------------------------
	// View dependent indices:
	// -----------------------------------------
	if (m_IndexCount > m_ParticleBuffers.m_IndicesCount || m_ParticleBuffers.m_LargeIndices != m_UnityMeshInfo.m_LargeIndices)
	{
		u32		paddedIdxCount = Mem::Align<0x1000>(m_IndexCount);

		viewIdx = 0;
		if (!m_ParticleBuffers.m_PerViewGeom.Resize(genInputs.m_PerViewGeneratedInputs.Count()))
			return false;
		PK_FOREACH(perViewBuff, genInputs.m_PerViewGeneratedInputs)
		{
			PK_ASSERT((viewIdx < m_ParticleBuffers.m_PerViewGeom.Count()));
			SParticleBuffers::SPerView	 &curViewBuffs = m_ParticleBuffers.m_PerViewGeom[viewIdx];

			if ((perViewBuff->m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
			{
				PK_FREE(curViewBuffs.m_Indices);
				curViewBuffs.m_Indices = (CFloat4*)PK_MALLOC_ALIGNED(paddedIdxCount * indexStride, 0x10);
			}
			++viewIdx;
		}
		m_ParticleBuffers.m_IndicesCount = paddedIdxCount;
		m_ParticleBuffers.m_LargeIndices = m_UnityMeshInfo.m_LargeIndices;
	}
	return true;
}

bool	CUnityBillboardingBatchPolicy::_RenderThread_AllocBillboardingBuffers(const SGeneratedInputs &genInputs, IRenderAPIData *renderApiData)
{
	(void)renderApiData;
	PK_SCOPEDPROFILE();

	// -----------------------------------------
	// VERTEX INPUTS:
	// -----------------------------------------
	{
		// -----------------------------------------
		// View independent inputs:
		// -----------------------------------------
		m_ParticleBuffers.m_ViewIndependantGeom.m_Positions.ResizeIFN(Drawers::GenInput_Position, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);
		m_ParticleBuffers.m_ViewIndependantGeom.m_Normals.ResizeIFN(Drawers::GenInput_Normal, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);
		m_ParticleBuffers.m_ViewIndependantGeom.m_Tangents.ResizeIFN(Drawers::GenInput_Tangent, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);
		m_ParticleBuffers.m_ViewIndependantGeom.m_UVFactors.ResizeIFN(Drawers::GenInput_UVFactors, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);

		m_ParticleBuffers.m_TexCoords0.ResizeIFN(Drawers::GenInput_UV0, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);
		m_ParticleBuffers.m_TexCoords1.ResizeIFN(Drawers::GenInput_UV1, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);
		m_ParticleBuffers.m_AtlasId.ResizeIFN(Drawers::GenInput_AtlasId, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);
		m_ParticleBuffers.m_UVRemap.ResizeIFN(Drawers::GenInput_UVRemap, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);

		if (_FindAdditionalInput(BasicRendererProperties::SID_Diffuse_Color(), BaseType_Float4, genInputs))
				m_ParticleBuffers.m_Colors.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_Emissive_EmissiveColor(), BaseType_Float3, genInputs))
				m_ParticleBuffers.m_EmissiveColors.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_AlphaRemap_Cursor(), BaseType_Float, genInputs))
			m_ParticleBuffers.m_AlphaCursor.ResizeIFN(m_VertexCount);

		m_ParticleBuffers.m_GeneratedInputs = genInputs.m_GeneratedInputs;
	}

	// -----------------------------------------
	// INDEX BUFFER:
	// -----------------------------------------
	const u32	indexStride = (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True ? sizeof(u32) : sizeof(u16));

	if (m_IndexCount > m_ParticleBuffers.m_IndicesCount || m_ParticleBuffers.m_LargeIndices != m_UnityMeshInfo.m_LargeIndices)
	{
		// Allocate more to avoid resizing to often:
		u32		paddedIdxCount = Mem::Align<0x1000>(m_IndexCount);
		// -----------------------------------------
		// View independent indices:
		// -----------------------------------------
		if ((genInputs.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
		{
			PK_FREE(m_ParticleBuffers.m_ViewIndependantGeom.m_Indices);
			m_ParticleBuffers.m_ViewIndependantGeom.m_Indices = PK_MALLOC_ALIGNED(paddedIdxCount * indexStride, 0x10);
		}
	}
	return true;
}

bool	CUnityBillboardingBatchPolicy::_RenderThread_SetupBuffersBillboards(const SGeneratedInputs &toMap, SBillboardBatchJobs *billboardBatch)
{
	PK_SCOPEDPROFILE();
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Indices != null))
			return false;
		billboardBatch->m_Exec_Indices.m_IndexStream.m_Indices = m_ParticleBuffers.m_ViewIndependantGeom.m_Indices;
		billboardBatch->m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
		billboardBatch->m_Exec_Indices.m_IndexStream.m_Large = m_UnityMeshInfo.m_LargeIndices == ManagedBool_True;
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Position) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Positions.m_Ptr != null))
			return false;
		billboardBatch->m_Exec_PNT.m_Positions = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)m_ParticleBuffers.m_ViewIndependantGeom.m_Positions.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Normal) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Normals.m_Ptr != null))
			return false;
		billboardBatch->m_Exec_PNT.m_Normals = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)m_ParticleBuffers.m_ViewIndependantGeom.m_Normals.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Tangent) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Tangents.m_Ptr != null))
			return false;
		billboardBatch->m_Exec_PNT.m_Tangents = TStridedMemoryView<CFloat4, 0x10>((CFloat4*)m_ParticleBuffers.m_ViewIndependantGeom.m_Tangents.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_UV0) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_TexCoords0.m_Ptr != null))
			return false;
		billboardBatch->m_Exec_Texcoords.m_Texcoords = TStridedMemoryView<CFloat2>(m_ParticleBuffers.m_TexCoords0.m_Ptr, m_VertexCount);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_UV1) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_TexCoords1.m_Ptr != null))
			return false;
		billboardBatch->m_Exec_Texcoords.m_Texcoords2 = TStridedMemoryView<CFloat2>(m_ParticleBuffers.m_TexCoords1.m_Ptr, m_VertexCount);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_AtlasId) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_AtlasId.m_Ptr != null))
			return false;
		billboardBatch->m_Exec_Texcoords.m_AtlasIds = TMemoryView<float>(m_ParticleBuffers.m_AtlasId.m_Ptr, m_VertexCount);
	}

	// Map only the color and alpha cursor
	// We only handle max 8 additional fields:
	if (!m_ParticleBuffers.m_AdditionalFieldsBuffers.Reserve(2))
		return false;
	for (u32 i = 0; i < toMap.m_AdditionalGeneratedInputs.Count(); ++i)
	{
		const SRendererFeatureFieldDefinition &addInput = toMap.m_AdditionalGeneratedInputs[i];

		if (addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() && addInput.m_Type == BaseType_Float4)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;

			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_Colors.m_Ptr;
			field.m_Storage.m_Stride = sizeof(CFloat4);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Emissive_EmissiveColor() && addInput.m_Type == BaseType_Float3)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc &field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;

			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8 *)m_ParticleBuffers.m_EmissiveColors.m_Ptr;
			field.m_Storage.m_Stride = sizeof(CFloat3);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaRemap_Cursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_AlphaCursor.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
	}

	billboardBatch->m_Exec_CopyField.m_FieldsToCopy = m_ParticleBuffers.m_AdditionalFieldsBuffers;

	// -----------------------------------------
	// View dependent inputs:
	// -----------------------------------------

	for (u32 i = 0; i < billboardBatch->m_PerView.Count(); ++i)
	{
		PK_ASSERT((i < m_ParticleBuffers.m_PerViewGeom.Count()));

		const SParticleBuffers::SPerView		&src = m_ParticleBuffers.m_PerViewGeom[i];
		SBillboardBatchJobs::SPerView			&dst = billboardBatch->m_PerView[i];
		SGeneratedInputs::SViewGeneratedInputs	curViewInputs = toMap.m_PerViewGeneratedInputs[i];

		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
		{
			if (!PK_VERIFY(src.m_Indices != null))
				return false;
			dst.m_Exec_Indices.m_IndexStream.m_Indices = src.m_Indices;
			dst.m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
			dst.m_Exec_Indices.m_IndexStream.m_Large = m_UnityMeshInfo.m_LargeIndices == ManagedBool_True;
		}
		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_Position) != 0)
		{
			if (!PK_VERIFY(src.m_Positions.m_Ptr != null))
				return false;
			dst.m_Exec_PNT.m_Positions = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)src.m_Positions.m_Ptr, m_VertexCount, 0x10);
		}
		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_Normal) != 0)
		{
			if (!PK_VERIFY(src.m_Normals.m_Ptr != null))
				return false;
			dst.m_Exec_PNT.m_Normals = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)src.m_Normals.m_Ptr, m_VertexCount, 0x10);
		}
		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_Tangent) != 0)
		{
			if (!PK_VERIFY(src.m_Tangents.m_Ptr != null))
				return false;
			dst.m_Exec_PNT.m_Tangents = TStridedMemoryView<CFloat4, 0x10>((CFloat4 *)src.m_Tangents.m_Ptr, m_VertexCount, 0x10);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBillboardingBatchPolicy::_RenderThread_SetupBuffersGeomBillboards(const SGeneratedInputs &toMap, SGPUBillboardBatchJobs *billboardBatch)
{
	PK_SCOPEDPROFILE();
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
	{
		if (!PK_VERIFY(m_MappedIdxBuffer != null))
			return false;
		billboardBatch->m_Exec_Indices.m_IndexStream.m_Indices = m_MappedIdxBuffer;
		billboardBatch->m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
		PK_ASSERT(m_UnityMeshInfo.m_LargeIndices == ManagedBool_True);
		billboardBatch->m_Exec_Indices.m_IndexStream.m_Large = (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True);
	}
	u32		rawBufferOffset = 0;

	//Should be enforced by being in GPU Billboarding
	if (!PK_VERIFY(m_MappedVtxBuffer.Count() == 1 &&  m_MappedVtxBuffer[0] != null))
		return false;
	void	*mappedVtxBuffer = m_MappedVtxBuffer[0];
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_ParticlePosition) != 0)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		billboardBatch->m_Exec_CopyBillboardingStreams.m_PositionsDrIds = TMemoryView<Drawers::SVertex_PositionDrId>(static_cast<Drawers::SVertex_PositionDrId*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float) * 4;
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleSize)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Sizes = TMemoryView<float>(static_cast<float*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float);
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleSize2)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Sizes2 = TMemoryView<CFloat2>(static_cast<CFloat2*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float) * 2;
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleRotation)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Rotations = TMemoryView<float>(static_cast<float*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float);
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleAxis0)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Axis0 = TMemoryView<CFloat3>(static_cast<CFloat3*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float) * 3;
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleAxis1)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		billboardBatch->m_Exec_CopyBillboardingStreams.m_Axis1 = TMemoryView<CFloat3>(static_cast<CFloat3*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float) * 3;
	}
	// Additional inputs:
	// Map only the color and alpha cursor
	// We only handle max 8 additional fields:
	// We Add a third input for Atlases
	if (!m_ParticleBuffers.m_AdditionalFieldsBuffers.Reserve(2 + 1))
		return false;
	for (u32 i = 0; i < toMap.m_AdditionalGeneratedInputs.Count(); ++i)
	{
		void											*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		const SRendererFeatureFieldDefinition			&addInput = toMap.m_AdditionalGeneratedInputs[i];

		if (addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() && addInput.m_Type == BaseType_Float4)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)rawptr;
			field.m_Storage.m_Stride = sizeof(CFloat4);
			rawBufferOffset += sizeof(float) * 4;
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaRemap_Cursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)rawptr;
			field.m_Storage.m_Stride = sizeof(float);
			rawBufferOffset += sizeof(float);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Atlas_TextureID() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)rawptr;
			field.m_Storage.m_Stride = sizeof(float);
			rawBufferOffset += sizeof(float);
		}
	}
	billboardBatch->m_Exec_CopyAdditionalFields.m_FieldsToCopy = m_ParticleBuffers.m_AdditionalFieldsBuffers;

	// -----------------------------------------
	// View dependent inputs:
	// -----------------------------------------
	PK_TODO("Unsure about if perview billboarding is setup in this factory");
	PK_ASSERT(billboardBatch->m_PerView.Count() <= 1);
	for (u32 i = 0; i < billboardBatch->m_PerView.Count(); ++i)
	{
		const u32	viewGeneratedInputs = toMap.m_PerViewGeneratedInputs[i].m_GeneratedInputs;
		if (viewGeneratedInputs & Drawers::GenInput_Indices)
		{
			if (!PK_VERIFY(m_MappedIdxBuffer != null))
				return false;
			billboardBatch->m_PerView[i].m_Exec_Indices.m_IndexStream.m_Indices = m_MappedIdxBuffer;
			billboardBatch->m_PerView[i].m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
			PK_ASSERT(m_UnityMeshInfo.m_LargeIndices == ManagedBool_True);
			billboardBatch->m_PerView[i].m_Exec_Indices.m_IndexStream.m_Large = (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBillboardingBatchPolicy::_RenderThread_SetupBuffersRibbons(const SGeneratedInputs &toMap, SRibbonBatchJobs *ribbonBatch)
{
	PK_SCOPEDPROFILE();
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Indices != null))
			return false;
		ribbonBatch->m_Exec_Indices.m_IndexStream.m_Indices = m_ParticleBuffers.m_ViewIndependantGeom.m_Indices;
		ribbonBatch->m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
		ribbonBatch->m_Exec_Indices.m_IndexStream.m_Large = m_UnityMeshInfo.m_LargeIndices == ManagedBool_True;
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Position) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Positions.m_Ptr != null))
			return false;
		ribbonBatch->m_Exec_PNT.m_Positions = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)m_ParticleBuffers.m_ViewIndependantGeom.m_Positions.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_UVFactors) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_UVFactors.m_Ptr != null))
			return false;
		ribbonBatch->m_Exec_PNT.m_UVFactors = TStridedMemoryView<CFloat2>((CFloat2*)m_ParticleBuffers.m_ViewIndependantGeom.m_UVFactors.m_Ptr, m_VertexCount);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Normal) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Normals.m_Ptr != null))
			return false;
		ribbonBatch->m_Exec_PNT.m_Normals = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)m_ParticleBuffers.m_ViewIndependantGeom.m_Normals.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Tangent) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Tangents.m_Ptr != null))
			return false;
		ribbonBatch->m_Exec_PNT.m_Tangents = TStridedMemoryView<CFloat4, 0x10>((CFloat4*)m_ParticleBuffers.m_ViewIndependantGeom.m_Tangents.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_UV0) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_TexCoords0.m_Ptr != null))
			return false;
		ribbonBatch->m_Exec_Texcoords.m_Texcoords = TStridedMemoryView<CFloat2>(m_ParticleBuffers.m_TexCoords0.m_Ptr, m_VertexCount);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_UVRemap) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_UVRemap.m_Ptr != null))
			return false;
		ribbonBatch->m_Exec_UVRemap.m_UVRemap = TMemoryView<CFloat4>(m_ParticleBuffers.m_UVRemap.m_Ptr, m_VertexCount);
	}

	// Map only the color and alpha cursor
	// We only handle max 8 additional fields:
	if (!m_ParticleBuffers.m_AdditionalFieldsBuffers.Reserve(2))
		return false;
	for (u32 i = 0; i < toMap.m_AdditionalGeneratedInputs.Count(); ++i)
	{
		const SRendererFeatureFieldDefinition	&addInput = toMap.m_AdditionalGeneratedInputs[i];

		if (addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() && addInput.m_Type == BaseType_Float4)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_Colors.m_Ptr;
			field.m_Storage.m_Stride = sizeof(CFloat4);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Emissive_EmissiveColor() && addInput.m_Type == BaseType_Float3)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc& field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_EmissiveColors.m_Ptr;
			field.m_Storage.m_Stride = sizeof(CFloat3);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaRemap_Cursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_AlphaCursor.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
	}

	ribbonBatch->m_Exec_CopyField.m_FieldsToCopy = m_ParticleBuffers.m_AdditionalFieldsBuffers;

	// -----------------------------------------
	// View dependent inputs:
	// -----------------------------------------
	for (u32 i = 0; i < ribbonBatch->m_PerView.Count(); ++i)
	{
		PK_ASSERT((i < m_ParticleBuffers.m_PerViewGeom.Count()));

		const SParticleBuffers::SPerView		&src = m_ParticleBuffers.m_PerViewGeom[i];
		SRibbonBatchJobs::SPerView				&dst = ribbonBatch->m_PerView[i];
		SGeneratedInputs::SViewGeneratedInputs	curViewInputs = toMap.m_PerViewGeneratedInputs[i];

		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
		{
			if (!PK_VERIFY(src.m_Indices != null))
				return false;
			dst.m_Exec_Indices.m_IndexStream.m_Indices = src.m_Indices;
			dst.m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
			dst.m_Exec_Indices.m_IndexStream.m_Large = m_UnityMeshInfo.m_LargeIndices == ManagedBool_True;
		}
		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_Position) != 0)
		{
			if (!PK_VERIFY(src.m_Positions.m_Ptr != null))
				return false;
			dst.m_Exec_PNT.m_Positions = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)src.m_Positions.m_Ptr, m_VertexCount, 0x10);
		}
		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_Normal) != 0)
		{
			if (!PK_VERIFY(src.m_Normals.m_Ptr != null))
				return false;
			dst.m_Exec_PNT.m_Normals = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)src.m_Normals.m_Ptr, m_VertexCount, 0x10);
		}
		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_Tangent) != 0)
		{
			if (!PK_VERIFY(src.m_Tangents.m_Ptr != null))
				return false;
			dst.m_Exec_PNT.m_Tangents = TStridedMemoryView<CFloat4, 0x10>((CFloat4*)src.m_Tangents.m_Ptr, m_VertexCount, 0x10);
		}
		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_UVFactors) != 0)
		{
			if (!PK_VERIFY(src.m_UVFactors.m_Ptr != null))
				return false;
			dst.m_Exec_PNT.m_UVFactors = TStridedMemoryView<CFloat2>((CFloat2*)src.m_UVFactors.m_Ptr, m_VertexCount);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

bool    CUnityBillboardingBatchPolicy::_RenderThread_SetupBuffersMeshes(const SGeneratedInputs &toMap, SMeshBatchJobs *meshBatch)
{
	PK_SCOPEDPROFILE();
	if (m_PerMeshBuffers.Count() == 0)
		return false;

	m_MeshAdditionalField.Clear();
	for (u32 i = 0; i < toMap.m_AdditionalGeneratedInputs.Count(); ++i)
	{
		const SRendererFeatureFieldDefinition	 &addInput = toMap.m_AdditionalGeneratedInputs[i];

		if (addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() && addInput.m_Type == BaseType_Float4)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_Colors), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaRemap_Cursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AlphaRemapCursor), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Emissive_EmissiveColor() && addInput.m_Type == BaseType_Float3)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_EmissiveColors), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if ((addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_Cursor()
			|| addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Soft_Cursor()
			|| addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_Cursor())
			&& addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_VATCursors), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasAtlas && addInput.m_Name == BasicRendererProperties::SID_Atlas_TextureID() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage *>(&m_PerMeshBuffers.First().m_AtlasId), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimation_AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AnimCursor0), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimation_CurrentAnimTrack() && addInput.m_Type == BaseType_I32)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AnimIdx0), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimInterpolTracksData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_NextAnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AnimCursor1), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimInterpolTracksData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_NextAnimTrack() && addInput.m_Type == BaseType_I32)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AnimIdx1), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimInterpolTracksData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_TransitionRatio() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_TransitionCursor), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
	}
	meshBatch->m_Exec_Matrices.m_MatricesPerMesh = TStridedMemoryView<TStridedMemoryView<CFloat4x4> >(&m_PerMeshBuffers.First().m_Transforms, m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
	if (!m_MeshAdditionalField.Empty())
	{
		meshBatch->m_Exec_CopyField.m_FieldsToCopyPerMesh = m_MeshAdditionalField.View();
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBillboardingBatchPolicy::_RenderThread_SetupBuffersTriangles(const SGeneratedInputs &toMap, STriangleBatchJobs *triangleBatch)
{
	PK_SCOPEDPROFILE();
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
	{
		triangleBatch->m_Exec_Indices.m_IndexStream.m_Indices = m_ParticleBuffers.m_ViewIndependantGeom.m_Indices;
		triangleBatch->m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
		triangleBatch->m_Exec_Indices.m_IndexStream.m_Large = m_UnityMeshInfo.m_LargeIndices == ManagedBool_True;
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Position) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Positions.m_Ptr != null))
			return false;
		triangleBatch->m_Exec_PNT.m_Positions = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)(m_ParticleBuffers.m_ViewIndependantGeom.m_Positions.m_Ptr), m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Normal) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Normals.m_Ptr != null))
			return false;
		triangleBatch->m_Exec_PNT.m_Normals = TStridedMemoryView<CFloat3, 0x10>((CFloat3 *)m_ParticleBuffers.m_ViewIndependantGeom.m_Normals.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Tangent) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Tangents.m_Ptr != null))
			return false;
		triangleBatch->m_Exec_PNT.m_Tangents = TStridedMemoryView<CFloat4, 0x10>((CFloat4*)m_ParticleBuffers.m_ViewIndependantGeom.m_Tangents.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_UV0) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_TexCoords0.m_Ptr != null))
			return false;
		triangleBatch->m_Exec_PNT.m_Texcoords = TStridedMemoryView<CFloat2>(m_ParticleBuffers.m_TexCoords0.m_Ptr, m_VertexCount);
	}

	// View dependent inputs:
	for (u32 i = 0; i < triangleBatch->m_PerView.Count(); ++i)
	{
		const SParticleBuffers::SPerView		&src = m_ParticleBuffers.m_PerViewGeom[i];
		STriangleBatchJobs::SPerView			&dst = triangleBatch->m_PerView[i];
		SGeneratedInputs::SViewGeneratedInputs	curViewInputs = toMap.m_PerViewGeneratedInputs[i];

		if ((curViewInputs.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
		{
			if (!PK_VERIFY(src.m_Indices != null))
				return false;
			dst.m_Exec_Indices.m_IndexStream.m_Indices = src.m_Indices;
			dst.m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
			dst.m_Exec_Indices.m_IndexStream.m_Large = m_UnityMeshInfo.m_LargeIndices == ManagedBool_True;
		}
	}

	// Additional inputs:
	// Map only the color and alpha cursor
	// We only handle max 8 additional fields:
	if (!m_ParticleBuffers.m_AdditionalFieldsBuffers.Reserve(2))
		return false;
	for (u32 i = 0; i < toMap.m_AdditionalGeneratedInputs.Count(); ++i)
	{
		const SRendererFeatureFieldDefinition	&addInput = toMap.m_AdditionalGeneratedInputs[i];

		if (addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() && addInput.m_Type == BaseType_Float4)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_Colors.m_Ptr;
			field.m_Storage.m_Stride = sizeof(CFloat4);
		}
		if (addInput.m_Name == BasicRendererProperties::SID_Emissive_EmissiveColor() && addInput.m_Type == BaseType_Float3)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc &field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8 *)m_ParticleBuffers.m_EmissiveColors.m_Ptr;
			field.m_Storage.m_Stride = sizeof(CFloat3);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaRemap_Cursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_AlphaCursor.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
	}

	triangleBatch->m_Exec_CopyField.m_FieldsToCopy = m_ParticleBuffers.m_AdditionalFieldsBuffers;
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBillboardingBatchPolicy::_FindAdditionalInput(const CStringId &inputName, EBaseTypeID inputType, const SGeneratedInputs &genInputs)
{
	PK_FOREACH(addInput, genInputs.m_AdditionalGeneratedInputs)
	{
		if (addInput->m_Name == inputName && addInput->m_Type == inputType)
			return true;
	}
	return false;
}

bool	CUnityBillboardingBatchPolicy::_PrepareCopySOA2AOS(const SUnityRenderContext &ctx, u32 idx)
{
	(void)ctx;
	m_Exec_SAO2AOS[idx].Clear();

	PK_ASSERT(m_MappedVtxBuffer.Count() != 0);
	m_Exec_SAO2AOS[idx].m_MappedVertexBuffer = m_MappedVtxBuffer;

	if (m_Exec_SAO2AOS[idx].m_MappedVertexBuffer.Count() != 0)
	{
		m_Exec_SAO2AOS[idx].m_IdxView = idx;
		m_Exec_SAO2AOS[idx].m_ShaderVariationFlags = m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags;
		m_Exec_SAO2AOS[idx].m_ParticleBuffers.Clear();
		m_Exec_SAO2AOS[idx].m_ParticleBuffers.FromParticleBuffers(m_ParticleBuffers, idx);
		m_Exec_SAO2AOS[idx].m_SemanticOffsets = &m_UnityMeshInfo.m_SemanticOffsets;
		m_Exec_SAO2AOS[idx].m_VertexStride = m_UnityMeshInfo.m_VertexStride;
	}
	return true;
}

void	CUnityBillboardingBatchPolicy::CBillboard_Exec_SOA_OAS::operator()(const Drawers::SBillboard_ExecPage &batch)
{
	const u32		renderedParticleCount = batch.m_Page->RenderedParticleCount();
	const u32		vertexCount = renderedParticleCount * batch.m_Billboarder->BillboardVertexCount();

	_CopyData(batch.m_VertexOffset, vertexCount);
}

void	CUnityBillboardingBatchPolicy::CBillboard_Exec_SOA_OAS::operator()(const Drawers::SRibbon_ExecBatch &batch)
{
	_CopyData(batch.FullVertexOffset(), batch.m_VertexCount);
}

void CUnityBillboardingBatchPolicy::CBillboard_Exec_SOA_OAS::operator()(const Drawers::STriangle_ExecPage & batch)
{
	const u32		renderedParticleCount = batch.m_Page->RenderedParticleCount();
	const u32		vertexCount = renderedParticleCount * 3;

	_CopyData(batch.m_VertexOffset, vertexCount);
}

void	CUnityBillboardingBatchPolicy::CBillboard_Exec_SOA_OAS::_CopyData(u32 vertexOffset, u32 vertexCount)
{
	PK_NAMEDSCOPEDPROFILE("CBillboard_Exec_SOA_OAS::_CopyData VB Fill");

	PK_TODO("Of stride is a multiple of 16 bytes, store aligned the position");

	volatile void		*bfPtr = Mem::AdvanceRawPointer(m_MappedVertexBuffer[m_IdxView], m_VertexStride * vertexOffset);
	const u32	endVertex = vertexOffset + vertexCount;

	for (u32 vertexID = vertexOffset; vertexID < endVertex; vertexID++)
	{
		if (m_ParticleBuffers.m_Positions != null)
			FillPositions(&(m_ParticleBuffers.m_Positions[vertexID]), bfPtr, *m_SemanticOffsets);
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_Lighting) != 0 && m_ParticleBuffers.m_Normals != null)
		{
			FillNormals(&(m_ParticleBuffers.m_Normals[vertexID]), bfPtr, *m_SemanticOffsets);
			if (m_ParticleBuffers.m_Tangents != null)
				FillTangents(&(m_ParticleBuffers.m_Tangents[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_Color) != 0 && m_ParticleBuffers.m_Colors != null)
		{
			FillColors(&(m_ParticleBuffers.m_Colors[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_CorrectDeformation) != 0 && m_ParticleBuffers.m_UVFactors != null && m_ParticleBuffers.m_UVRemap != null)
		{
			FillUVFactors(&(m_ParticleBuffers.m_UVFactors[vertexID]), bfPtr, *m_SemanticOffsets);
			FillUVScalesAndOffsets(&(((CFloat4*)m_ParticleBuffers.m_UVRemap)[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		else if (m_ParticleBuffers.m_TexCoords0 != null && (*m_SemanticOffsets)[Semantic_Uv0] != (u32)0xFFFFFFFF)
		{
			FillUV0(&(m_ParticleBuffers.m_TexCoords0[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_AnimBlend) != 0 && m_ParticleBuffers.m_TexCoords1 != null && m_ParticleBuffers.m_AtlasId != null)
		{
			FillUV1(&(((CFloat2*)m_ParticleBuffers.m_TexCoords1)[vertexID]), bfPtr, *m_SemanticOffsets);
			FillAtlasId(&(m_ParticleBuffers.m_AtlasId[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0 && m_ParticleBuffers.m_AlphaCursor != null)
		{
			FillAlphaCursor(&(m_ParticleBuffers.m_AlphaCursor[vertexID]), bfPtr, *m_SemanticOffsets);
		}

		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_Emissive) != 0 && m_ParticleBuffers.m_EmissiveColors != null)
		{
			FillEmissiveColors(&(m_ParticleBuffers.m_EmissiveColors[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		bfPtr = Mem::AdvanceRawPointer(bfPtr, m_VertexStride);
	}
	
}

//----------------------------------------------------------------------------

void	CUnityBillboardingBatchPolicy::CBilboard_Exec_BillboardInfo::operator()(const Drawers::CCopyStream_CPU &bb)
{
	_Prepare(bb.DrawRequests());
}

//----------------------------------------------------------------------------

void	CUnityBillboardingBatchPolicy::CBilboard_Exec_BillboardInfo::operator()(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests)
{
	_Prepare(drawRequests);
}

//----------------------------------------------------------------------------

void	CUnityBillboardingBatchPolicy::CBilboard_Exec_BillboardInfo::_Prepare(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests)
{
	PK_SCOPEDPROFILE();
	PK_ASSERT(drawRequests.Count() > 0);
	PK_ASSERT(m_GeomDrawRequests.Count() >= drawRequests.Count());
	const TStridedMemoryView<const Drawers::SBillboard_DrawRequest * const>	&drs = TStridedMemoryView<const Drawers::SBillboard_DrawRequest * const>::Reinterpret(drawRequests);

	for (u32 dri = 0; dri < drawRequests.Count(); ++dri)
	{
		const Drawers::SBillboard_DrawRequest			*dr = drs[dri];
		const Drawers::SBillboard_BillboardingRequest	&br = dr->m_BB;

		SBillboardInfo			&info = m_GeomDrawRequests[dri];

		info.m_Flags = br.m_Mode + 1;
		if (br.m_Flags.m_FlipU &&  br.m_Flags.m_FlipV)
			info.m_Flags |= 8U;
		if (br.m_Flags.m_HasAtlasBlending)
			info.m_Flags |= 16U;
		info.m_NormalBendingFactor = br.m_NormalsBendingFactor;
	}

}

//----------------------------------------------------------------------------
__PK_API_END
