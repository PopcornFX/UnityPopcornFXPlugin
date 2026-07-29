//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "UnityBatchDrawer_Billboard_GPU.h"
#include "FrameCollectorUnityTypes.h"
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <RuntimeManager.h>

//----------------------------------------------------------------------------

CUnityBatchDrawer_Billboard_GPU::CUnityBatchDrawer_Billboard_GPU()
:	m_BBox(CAABB::DEGENERATED)
,	m_VertexCount(0)
,	m_IndexCount(0)
,	m_ParticleCount(0)
,	m_PrevParticleCount(0)
,	m_MeshIsValid(false)
,	m_MappedIdxBuffer(null)
,	m_MappedInfoBillboardBuffer(null)
,	m_MappedAtlasesBuffer(null)
{
}

//----------------------------------------------------------------------------

CUnityBatchDrawer_Billboard_GPU::~CUnityBatchDrawer_Billboard_GPU()
{
}

//----------------------------------------------------------------------------

bool		CUnityBatchDrawer_Billboard_GPU::Setup(const CRendererDataBase *renderer, const CParticleRenderMedium *owner, const PopcornFX::CFrameCollector *fc, const CStringId &storageClass)
{
	if (!CRendererBatchJobs_Billboard_GPUBB::Setup(renderer, owner, fc, storageClass))
		return false;
	owner->m_OnRenderMediumActiveStateChanged += FastDelegate<void(CParticleRenderMedium*, bool)>(this, &CUnityBatchDrawer_Billboard_GPU::_OnRenderMediumActiveStateChanged);
	
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Billboard_GPU::AreRenderersCompatible(const PopcornFX::CRendererDataBase *rendererA, const PopcornFX::CRendererDataBase *rendererB) const
{
	const CUnityRendererCache	*firstMaterialDesc = static_cast<const CUnityRendererCache *>(rendererA->m_RendererCache.Get());
	const CUnityRendererCache	*secondMaterialDesc = static_cast<const CUnityRendererCache *>(rendererB->m_RendererCache.Get());
	return firstMaterialDesc == secondMaterialDesc || *firstMaterialDesc == *secondMaterialDesc;
}

bool CUnityBatchDrawer_Billboard_GPU::CanRender(PopcornFX::SRenderContext & ctx) const
{
	(void)ctx;
	return true;
}

void CUnityBatchDrawer_Billboard_GPU::BeginFrame(PopcornFX::SRenderContext & ctx)
{
	(void)ctx;
	_ClearBuffers();
}

bool CUnityBatchDrawer_Billboard_GPU::EmitDrawCall(PopcornFX::SRenderContext & ctx, const PopcornFX::SDrawCallDesc & toEmit)
{
	(void)ctx; (void)toEmit;
	return true;
}

void CUnityBatchDrawer_Billboard_GPU::Destroy()
{
	CRuntimeManager	&manager = CRuntimeManager::Instance();
	if (m_PrevParticleCount != 0)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
				manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_False);
		}
	}
}

void	CUnityBatchDrawer_Billboard_GPU::_OnRenderMediumActiveStateChanged(CParticleRenderMedium *renderMedium, bool active)
{
	(void) renderMedium;
	(void) active;
	CRuntimeManager	&manager = CRuntimeManager::Instance();
	if (m_PrevParticleCount != 0)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
				manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_False);
		}
	}
}

void	CUnityBatchDrawer_Billboard_GPU::_ClearBuffers()
{
	m_ParticleBuffers.m_AdditionalFieldsBuffers.Clear();
	m_IndexCount = 0;
	m_VertexCount = 0;
	m_PrevParticleCount = m_ParticleCount;
	m_ParticleCount = 0;
}

void	CUnityBatchDrawer_Billboard_GPU::_UpdateThread_ResizeUnityMesh(const SUnityRenderContext &ctx)
{
	PK_SCOPEDPROFILE();
	m_MeshIsValid = true;
	CRuntimeManager	&manager = CRuntimeManager::Instance();

	// If the Unity mesh is too small, we resize it:
	if ((u32)m_UnityMeshInfo.m_VBElemCount < m_VertexCount || (u32)m_UnityMeshInfo.m_IBElemCount < m_IndexCount)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
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
			else
				m_MeshIsValid = false;
		}
	}
	else
	{
		if (CCurrentThread::IsMainThread())
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[0].m_RendererGUID != -1))
				::OnSetParticleCount(m_UnityMeshInfoPerViews[0].m_RendererGUID, m_ParticleCount);
		}
		else // is render thread
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[0].m_IndirectArgsHandler.m_Buffer->m_DeviceLocal != null))
			{
				const u32	indirectArgsSize = 5 * sizeof(u32);
				u32			*mappedArgs = static_cast<u32*>(ctx.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_IndirectArgsHandler, false, indirectArgsSize, indirectArgsSize));

				mappedArgs[0] = m_ParticleCount * m_UnityMeshInfo.m_IndirectArgsParticleCountMultiplier;
				mappedArgs[1] = 1;
				mappedArgs[2] = 0;
				mappedArgs[3] = 0;
				mappedArgs[4] = 0;
				ctx.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_IndirectArgsHandler, false);
			}
		}
	}
	if (m_PrevParticleCount == 0 && m_ParticleCount != 0)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
				manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_True);
		}
		
	}
}

void	CUnityBatchDrawer_Billboard_GPU::_UpdateThread_SetUnityMeshBounds()
{
	PK_SCOPEDPROFILE();

	const PopcornFX::SRendererBatchDrawPass &drawPass = DrawPass();
	CAABB	bbox = CAABB::DEGENERATED;

	for (u32 i = 0; i < drawPass.m_DrawRequests.Count(); ++i)
	{
		bbox.Add(drawPass.m_DrawRequests[i]->StreamToRender().BBox());
	}

	CRuntimeManager	&manager = CRuntimeManager::Instance();

	if (bbox == CAABB::DEGENERATED)
	{
		SUpdateRendererBounds	bounds;

		bounds.m_Min = CFloat3::ZERO;
		bounds.m_Max = CFloat3::ZERO;
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
				manager.OnUpdateRendererBounds(m_UnityMeshInfoPerViews[i].m_RendererGUID, &bounds);
		}
		
		m_BBox = CAABB::ZERO;
	}
	else if (m_BBox != bbox)
	{
		SUpdateRendererBounds	bounds;
		// Apply the camera offset on unity mesh bounds. This allows to apply the camera sort offset in unity.
		// It may have repercussions on the culling
		float cameraSortOffset = drawPass.m_DrawRequests.First()->BaseBillboardingRequest().m_CameraSortOffset;

		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
			{
				const CFloat3 &viewDirection = drawPass.m_Views[i].m_InvViewMatrix.StrippedZAxis();
				bounds.m_Min = bbox.Min() - cameraSortOffset * viewDirection;
				bounds.m_Max = bbox.Max() - cameraSortOffset * viewDirection;
				manager.OnUpdateRendererBounds(m_UnityMeshInfoPerViews[i].m_RendererGUID, &bounds);
			}
		}
		m_BBox = bbox;
	}
}

bool CUnityBatchDrawer_Billboard_GPU::AllocBuffers(SRenderContext &ctx)
{
	const PopcornFX::SRendererBatchDrawPass &drawPass = DrawPass();
	const SUnityRenderContext				&renderContext = static_cast<SUnityRenderContext&>(ctx);
	PK_SCOPEDPROFILE();

	if (!PK_VERIFY(renderContext.m_RenderApiData != null))
	{
		CLog::Log(PK_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}

	PK_ASSERT(!drawPass.m_DrawRequests.Empty());
	PK_ASSERT(drawPass.m_DrawRequests.Count() == drawPass.m_RendererCaches.Count());
	PK_ASSERT(m_BB_Billoard.TotalParticleCount() > 0);
	PK_ASSERT(drawPass.m_DrawRequests.First() != null);

	// The first time the batch is used, we store the material desc in the batch (all the material descs should be the same):
	if (m_UnityMeshInfoPerViews.Count() == 0)
	{
		const CUnityRendererCache		*rendererCache = static_cast<const CUnityRendererCache *>(drawPass.m_RendererCaches.First().Get());

		//m_RendererType = rendererCache->m_RendererType;
		m_MaterialDescBillboard = rendererCache->m_MaterialDescBillboard;
		//m_MaterialDescMesh = rendererCache->m_MaterialDescMesh;
		u32 viewCount = 1;

		//if (!(m_RendererType == Renderer_Mesh || m_RendererType == Renderer_Light || m_RendererType == Renderer_Sound || m_RendererType == Renderer_Decal))
			viewCount = drawPass.m_Views.Count();

		//if (m_RendererType == Renderer_Light || m_RendererType == Renderer_Sound || m_RendererType == Renderer_Decal)
		//	viewCount = 0;

		if (viewCount > 0)
		{
			if (!PK_VERIFY(m_UnityMeshInfoPerViews.Resize(viewCount)))
				return false;

			for (u32 i = 0; i < viewCount; ++i)
				m_UnityMeshInfoPerViews[i] = rendererCache->m_UnityMeshInfoPerViews[i];
		}

		m_UnityMeshInfo = rendererCache->m_UnityMeshInfo;
	}

	m_ParticleCount = m_BB_Billoard.TotalParticleCount();

	{
		//m_VertexCount = m_BB_Billoard.TotalVertexCount();
		//m_IndexCount = m_BB_Billoard.TotalIndexCount();
		m_VertexCount = m_ParticleCount * 4;
		m_IndexCount = m_ParticleCount * 6;
		_UpdateThread_ResizeUnityMesh(renderContext);
		_UpdateThread_SetUnityMeshBounds();

		if (!drawPass.m_DrawRequests.Empty())
		{
			const Drawers::SBase_DrawRequest	*dr = drawPass.m_DrawRequests.First();
			if (PK_VERIFY(dr != null))
			{
				{
					const Drawers::SBillboard_BillboardingRequest	*bbRequest = static_cast<const Drawers::SBillboard_BillboardingRequest*>(&dr->BaseBillboardingRequest());
					if (PK_VERIFY(bbRequest != null))
						m_AtlasList = bbRequest->m_Atlas;
				}
				for (u32 i = 0; i < m_AdditionalInputs.Count(); ++i)
				{
					const SRendererFeatureFieldDefinition	&addInput = m_AdditionalInputs[i];
					if (addInput.m_Name == BasicRendererProperties::SID_Emissive_EmissiveColor() && (addInput.m_Type == BaseType_Float4 || addInput.m_Type == BaseType_Float3))
					{
						if (addInput.m_Type == BaseType_Float3)
							m_IsEmissive3 = true;
					}
				}
			}
		}
	}
	return true;
}

bool CUnityBatchDrawer_Billboard_GPU::MapBuffers(SRenderContext &ctx)
{
	const PopcornFX::SRendererBatchDrawPass &drawPass = DrawPass();
	const SUnityRenderContext				&renderContext = static_cast<SUnityRenderContext&>(ctx);
	PK_SCOPEDPROFILE();

	if (!PK_VERIFY(renderContext.m_RenderApiData != null))
	{
		CLog::Log(PK_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}
	
	if (!m_MeshIsValid)
		return true;

	// VMN: Not sure if we want to alloc all of them
	const PopcornFX::SGeneratedInputs	&toMap = drawPass.m_ToGenerate;

	if (!PK_VERIFY(_RenderThread_AllocPerViewGeomBuffers(toMap)))
		return false;
	if (m_AtlasList != null)
	{
		//VMN: Should be done once.
		m_MappedAtlasesBuffer = renderContext.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_AtlasesHandler, false, m_UnityMeshInfo.m_AtlasesSize, m_UnityMeshInfo.m_AtlasesSize);
		if (!PK_VERIFY(m_MappedAtlasesBuffer != null))
			return false;

		u32		*count = reinterpret_cast<u32*>(Mem::AdvanceRawPointer(m_MappedAtlasesBuffer, 0));
		CFloat4	*data = reinterpret_cast<CFloat4*>(Mem::AdvanceRawPointer(m_MappedAtlasesBuffer, sizeof(CUint4))); // Alignement
		*count = m_AtlasList->m_RectsFp32.Count();
		PK_ASSERT(*count < 0x100);
		PK_ASSERT(m_AtlasList->m_RectsFp32.Stride() == sizeof(CFloat4));
		Mem::Copy(data, m_AtlasList->m_RectsFp32.RawDataPointer(), m_AtlasList->m_RectsFp32.CoveredBytes());

		if (m_MappedAtlasesBuffer != null)
			renderContext.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_AtlasesHandler, false);

		m_AtlasList = null;
	}

	// Here we map the Unity buffers:
	const u32	vboFullSize = m_UnityMeshInfo.m_VBElemCount * m_UnityMeshInfo.m_VertexStride;
	const u32	vboMapSize = m_VertexCount * m_UnityMeshInfo.m_VertexStride;
	const u32	iboFullSize = m_UnityMeshInfo.m_IBElemCount * (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True ? 4 : 2);
	const u32	iboMapSize = m_IndexCount * (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True ? 4 : 2);

	// Map the final buffers:
	m_MappedVtxBuffer[0] = renderContext.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_VBHandler, false, vboFullSize, vboMapSize);
	if (!PK_VERIFY(m_MappedVtxBuffer[0] != null))
		return false;
	m_MappedIdxBuffer = renderContext.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_IBHandler, true, iboFullSize, iboMapSize);
	if (!PK_VERIFY(m_MappedIdxBuffer != null))
		return false;
	m_MappedInfoBillboardBuffer = renderContext.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_InfoHandler, false, m_UnityMeshInfo.m_InfoSize, m_UnityMeshInfo.m_InfoSize);
	if (!PK_VERIFY(m_MappedInfoBillboardBuffer != null))
		return false;

	if (!_RenderThread_SetupBuffersGeomBillboards(toMap))
		return false;

	return true;
}

bool	CUnityBatchDrawer_Billboard_GPU::UnmapBuffers(SRenderContext &ctx)
{
	const SUnityRenderContext				&renderContext = static_cast<SUnityRenderContext&>(ctx);
	PK_SCOPEDPROFILE();
	if (!m_MeshIsValid)
		return true;

	{
		// Unmap the final buffers:
		for (u32 i = 0; i < m_MappedVtxBuffer.Count(); ++i)
		{
			if (m_MappedVtxBuffer[i] != null)
			{
				renderContext.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_VBHandler, false);
			}
		}

		{
			renderContext.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_InfoHandler, false);
			m_MappedInfoBillboardBuffer = null;
			renderContext.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[0].m_IBHandler, true);
			m_MappedIdxBuffer = null;
		}
		for (u32 i = 0; i < m_MappedVtxBuffer.Count(); ++i)
			m_MappedVtxBuffer[i] = null;
	}
	return true;
}

bool CUnityBatchDrawer_Billboard_GPU::LaunchCustomTasks(SRenderContext &ctx)
{
	(void)ctx;
	if (!m_MeshIsValid)
		return true;
	CBilboard_Exec_BillboardInfo::SBillboardInfo	*mappedInfo = static_cast<CBilboard_Exec_BillboardInfo::SBillboardInfo*>(m_MappedInfoBillboardBuffer);
	m_Exec_BillboardInfo.Clear();
	m_Exec_BillboardInfo.m_GeomDrawRequests = TMemoryView<CBilboard_Exec_BillboardInfo::SBillboardInfo>(mappedInfo, 256);
	m_BB_Billoard.AddExecDuringPages(&m_Exec_BillboardInfo);
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Billboard_GPU::_RenderThread_AllocPerViewGeomBuffers(const SGeneratedInputs &genInputs)
{
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

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Billboard_GPU::_RenderThread_SetupBuffersGeomBillboards(const SGeneratedInputs &toMap)
{
	PK_SCOPEDPROFILE();
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
	{
		if (!PK_VERIFY(m_MappedIdxBuffer != null))
			return false;
		m_BBJobs_Billboard.m_Exec_Indices.m_IndexStream.m_Indices = m_MappedIdxBuffer;
		m_BBJobs_Billboard.m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
		PK_ASSERT(m_UnityMeshInfo.m_LargeIndices == ManagedBool_True);
		m_BBJobs_Billboard.m_Exec_Indices.m_IndexStream.m_Large = (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True);
	}
	u32		rawBufferOffset = 0;

	//Should be enforced by being in GPU Billboarding
	if (!PK_VERIFY(m_MappedVtxBuffer.Count() == 1 &&  m_MappedVtxBuffer[0] != null))
		return false;
	void	*mappedVtxBuffer = m_MappedVtxBuffer[0];
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_ParticlePosition) != 0)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		m_BBJobs_Billboard.m_Exec_CopyBillboardingStreams.m_PositionsDrIds = TMemoryView<Drawers::SVertex_PositionDrId>(static_cast<Drawers::SVertex_PositionDrId*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float) * 4;
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleSize)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		m_BBJobs_Billboard.m_Exec_CopyBillboardingStreams.m_Sizes = TMemoryView<float>(static_cast<float*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float);
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleSize2)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		m_BBJobs_Billboard.m_Exec_CopyBillboardingStreams.m_Sizes2 = TMemoryView<CFloat2>(static_cast<CFloat2*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float) * 2;
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleRotation)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		m_BBJobs_Billboard.m_Exec_CopyBillboardingStreams.m_Rotations = TMemoryView<float>(static_cast<float*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float);
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleAxis0)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		m_BBJobs_Billboard.m_Exec_CopyBillboardingStreams.m_Axis0 = TMemoryView<CFloat3>(static_cast<CFloat3*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float) * 3;
	}
	if (toMap.m_GeneratedInputs & Drawers::GenInput_ParticleAxis1)
	{
		void	*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		m_BBJobs_Billboard.m_Exec_CopyBillboardingStreams.m_Axis1 = TMemoryView<CFloat3>(static_cast<CFloat3*>(rawptr), m_VertexCount);
		rawBufferOffset += sizeof(float) * 3;
	}
	// Additional inputs:
	// Map only the color and alpha cursor
	// We only handle max 8 additional fields:
	// We Add a third input for Atlases
	if (!m_ParticleBuffers.m_AdditionalFieldsBuffers.Reserve(2 + 1))
		return false;
	for (u32 i = 0; i < m_AdditionalInputs.Count(); ++i)
	{
		void											*rawptr = Mem::AdvanceRawPointer(mappedVtxBuffer, rawBufferOffset * m_UnityMeshInfo.m_VBElemCount);
		const SRendererFeatureFieldDefinition			&addInput = m_AdditionalInputs[i];

		if ((addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() || addInput.m_Name == BasicRendererProperties::SID_Diffuse_DiffuseColor()) && addInput.m_Type == BaseType_Float4)
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
	m_BBJobs_Billboard.m_Exec_CopyAdditionalFields.m_FieldsToCopy = m_ParticleBuffers.m_AdditionalFieldsBuffers;

	// -----------------------------------------
	// View dependent inputs:
	// -----------------------------------------
	PK_TODO("Unsure about if perview billboarding is setup in this factory");
	PK_ASSERT(m_BBJobs_Billboard.m_PerView.Count() <= 1);
	for (u32 i = 0; i < m_BBJobs_Billboard.m_PerView.Count(); ++i)
	{
		const u32	viewGeneratedInputs = toMap.m_PerViewGeneratedInputs[i].m_GeneratedInputs;
		if (viewGeneratedInputs & Drawers::GenInput_Indices)
		{
			if (!PK_VERIFY(m_MappedIdxBuffer != null))
				return false;
			m_BBJobs_Billboard.m_PerView[i].m_Exec_Indices.m_IndexStream.m_Indices = m_MappedIdxBuffer;
			m_BBJobs_Billboard.m_PerView[i].m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
			PK_ASSERT(m_UnityMeshInfo.m_LargeIndices == ManagedBool_True);
			m_BBJobs_Billboard.m_PerView[i].m_Exec_Indices.m_IndexStream.m_Large = (m_UnityMeshInfo.m_LargeIndices == ManagedBool_True);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

void	CUnityBatchDrawer_Billboard_GPU::CBilboard_Exec_BillboardInfo::operator()(const Drawers::CCopyStream_CPU &bb)
{
	_Prepare(bb.DrawRequests());
}

//----------------------------------------------------------------------------

void	CUnityBatchDrawer_Billboard_GPU::CBilboard_Exec_BillboardInfo::operator()(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests)
{
	_Prepare(drawRequests);
}

//----------------------------------------------------------------------------

void	CUnityBatchDrawer_Billboard_GPU::CBilboard_Exec_BillboardInfo::_Prepare(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests)
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
		if (br.m_Flags.m_FlipU)
			info.m_Flags |= 32U;

		info.m_NormalBendingFactor = br.m_NormalsBendingFactor;
	}

}
