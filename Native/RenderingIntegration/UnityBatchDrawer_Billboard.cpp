//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "UnityBatchDrawer_Billboard.h"
#include "FrameCollectorUnityTypes.h"
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <RuntimeManager.h>

//----------------------------------------------------------------------------

CUnityBatchDrawer_Billboard::CUnityBatchDrawer_Billboard()
:	m_BBox(CAABB::DEGENERATED)
,	m_VertexCount(0)
,	m_IndexCount(0)
,	m_ParticleCount(0)
,	m_PrevParticleCount(0)
,	m_MeshIsValid(false)
,	m_MappedAtlasesBuffer(null)
{
}

//----------------------------------------------------------------------------

CUnityBatchDrawer_Billboard::~CUnityBatchDrawer_Billboard()
{
}

//----------------------------------------------------------------------------

bool		CUnityBatchDrawer_Billboard::Setup(const CRendererDataBase *renderer, const CParticleRenderMedium *owner, const PopcornFX::CFrameCollector *fc, const CStringId &storageClass)
{
	if (!CRendererBatchJobs_Billboard_CPUBB::Setup(renderer, owner, fc, storageClass))
		return false;
	owner->m_OnRenderMediumActiveStateChanged += FastDelegate<void(CParticleRenderMedium*, bool)>(this, &CUnityBatchDrawer_Billboard::_OnRenderMediumActiveStateChanged);

	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Billboard::AreRenderersCompatible(const PopcornFX::CRendererDataBase *rendererA, const PopcornFX::CRendererDataBase *rendererB) const
{
	const CUnityRendererCache	*firstMaterialDesc = static_cast<const CUnityRendererCache *>(rendererA->m_RendererCache.Get());
	const CUnityRendererCache	*secondMaterialDesc = static_cast<const CUnityRendererCache *>(rendererB->m_RendererCache.Get());
	return firstMaterialDesc == secondMaterialDesc || *firstMaterialDesc == *secondMaterialDesc;
}

bool CUnityBatchDrawer_Billboard::CanRender(PopcornFX::SRenderContext & ctx) const
{
	(void)ctx;
	return true;
}

void CUnityBatchDrawer_Billboard::BeginFrame(PopcornFX::SRenderContext & ctx)
{
	(void)ctx;
	_ClearBuffers();
}

bool CUnityBatchDrawer_Billboard::EmitDrawCall(PopcornFX::SRenderContext & ctx, const PopcornFX::SDrawCallDesc & toEmit)
{
	(void)ctx; (void)toEmit;
	return true;
}

void CUnityBatchDrawer_Billboard::Destroy()
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

void	CUnityBatchDrawer_Billboard::_OnRenderMediumActiveStateChanged(CParticleRenderMedium *renderMedium, bool active)
{
	(void) renderMedium; (void)active;
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

void	CUnityBatchDrawer_Billboard::_ClearBuffers()
{
	m_ParticleBuffers.m_AdditionalFieldsBuffers.Clear();
	m_IndexCount = 0;
	m_VertexCount = 0;
	m_PrevParticleCount = m_ParticleCount;
	m_ParticleCount = 0;
}

void	CUnityBatchDrawer_Billboard::_UpdateThread_ResizeUnityMesh(const SUnityRenderContext &ctx)
{
	(void)ctx;
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
	if (m_PrevParticleCount == 0 && m_ParticleCount != 0)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
				manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_True);
		}
		
	}
}

void	CUnityBatchDrawer_Billboard::_UpdateThread_SetUnityMeshBounds()
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

bool CUnityBatchDrawer_Billboard::AllocBuffers(SRenderContext &ctx)
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
	PK_ASSERT((m_BB_Billboard.TotalVertexCount() > 0 && m_BB_Billboard.TotalIndexCount() > 0) || m_BB_Billboard.TotalParticleCount() > 0);
	PK_ASSERT(drawPass.m_DrawRequests.First() != null);

	// The first time the batch is used, we store the material desc in the batch (all the material descs should be the same):
	if (m_UnityMeshInfoPerViews.Count() == 0)
	{
		const CUnityRendererCache	*rendererCache = static_cast<const CUnityRendererCache *>(drawPass.m_RendererCaches.First().Get());
		const u32					viewCount = drawPass.m_Views.Count();
		m_MaterialDescBillboard = rendererCache->m_MaterialDescBillboard;

		if (viewCount > 0)
		{
			if (!PK_VERIFY(m_Exec_SAO2AOS.Resize(viewCount)) ||
				!PK_VERIFY(m_UnityMeshInfoPerViews.Resize(viewCount)))
				return false;

			for (u32 i = 0; i < viewCount; ++i)
			{
				m_Exec_SAO2AOS[i].Clear();
				m_UnityMeshInfoPerViews[i] = rendererCache->m_UnityMeshInfoPerViews[i];
			}
		}

		m_UnityMeshInfo = rendererCache->m_UnityMeshInfo;
	}

	m_ParticleCount = m_BB_Billboard.TotalParticleCount();

	{
		m_VertexCount = m_BB_Billboard.TotalVertexCount();
		m_IndexCount = m_BB_Billboard.TotalIndexCount();
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

bool CUnityBatchDrawer_Billboard::MapBuffers(SRenderContext &ctx)
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

	// Here we map the Unity buffers:
	const u32	vboFullSize = m_UnityMeshInfo.m_VBElemCount * m_UnityMeshInfo.m_VertexStride;
	const u32	vboMapSize = m_BB_Billboard.TotalVertexCount() * m_UnityMeshInfo.m_VertexStride;

	// Alloc the buffers:
	const PopcornFX::SGeneratedInputs	&toMap = drawPass.m_ToGenerate;

	if (!PK_VERIFY(_RenderThread_AllocBillboardingBuffers(toMap)))
		return false;
	if (!PK_VERIFY(_RenderThread_AllocPerViewGeomBuffers(toMap)))
		return false;

	if (m_AtlasList != null && m_UnityMeshInfo.m_AtlasesSize != 0)
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
	// Setup the memory view in the jobs:
	if (!PK_VERIFY(_RenderThread_SetupBuffersBillboards(toMap)))
		return false;
	// Map the final buffers:
	if (m_MappedVtxBuffer.Count() < drawPass.m_Views.Count())
	{
		if (!PK_VERIFY(m_MappedVtxBuffer.Resize(drawPass.m_Views.Count())))
			return false;// To review, Can we do this alloc somewhere else ?
	}
	for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
	{
		m_MappedVtxBuffer[i] = renderContext.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_VBHandler, false, vboFullSize, vboMapSize);
		if (!PK_VERIFY(m_MappedVtxBuffer[i] != null))
			return false;
	}
	return true;
}

bool	CUnityBatchDrawer_Billboard::UnmapBuffers(SRenderContext &ctx)
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

		// Mem copy the indices:
		const u32	iboFullSize = m_UnityMeshInfo.m_IBElemCount * (m_UnityMeshInfo.m_LargeIndices ? sizeof(u32) : sizeof(u16));
		const u32	srcIdxSize = m_IndexCount * (m_UnityMeshInfo.m_LargeIndices ? sizeof(u32) : sizeof(u16));

		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			void		*indices = renderContext.m_RenderApiData->BeginModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_IBHandler, true, iboFullSize, iboFullSize);

			if (!PK_VERIFY(indices != null && m_Exec_SAO2AOS[i].m_ParticleBuffers.m_Indices != null))
				continue;

			Mem::Copy(indices, m_Exec_SAO2AOS[i].m_ParticleBuffers.m_Indices, srcIdxSize);
			Mem::Clear(Mem::AdvanceRawPointer(indices, srcIdxSize), iboFullSize - srcIdxSize);

			renderContext.m_RenderApiData->EndModifyNativeBuffer(m_UnityMeshInfoPerViews[i].m_IBHandler, true);
		}

		for (u32 i = 0; i < m_MappedVtxBuffer.Count(); ++i)
			m_MappedVtxBuffer[i] = null;
	}
	return true;
}

bool CUnityBatchDrawer_Billboard::LaunchCustomTasks(SRenderContext &ctx)
{
	(void)ctx;
	if (!m_MeshIsValid)
		return true;

	for (u32 i = 0; i < m_Exec_SAO2AOS.Count(); ++i)
	{
		if (_PrepareCopySOA2AOS(i) && m_Exec_SAO2AOS[i].m_MappedVertexBuffer[i] != null)
			m_BB_Billboard.AddExecLatePage(&(m_Exec_SAO2AOS[i]));
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Billboard::_PrepareCopySOA2AOS(u32 idx)
{
	m_Exec_SAO2AOS[idx].Clear();

	PK_ASSERT(m_MappedVtxBuffer.Count() != 0);
	m_Exec_SAO2AOS[idx].m_MappedVertexBuffer = m_MappedVtxBuffer;

	if (m_Exec_SAO2AOS[idx].m_MappedVertexBuffer.Count() != 0)
	{
		m_Exec_SAO2AOS[idx].m_IdxView = idx;
		m_Exec_SAO2AOS[idx].m_ShaderVariationFlags = m_MaterialDescBillboard.m_Flags.m_ShaderVariationFlags;
		m_Exec_SAO2AOS[idx].m_IsEmissive3 = m_IsEmissive3;
		m_Exec_SAO2AOS[idx].m_ParticleBuffers.Clear();
		m_Exec_SAO2AOS[idx].m_ParticleBuffers.FromParticleBuffers(m_ParticleBuffers, idx);
		m_Exec_SAO2AOS[idx].m_SemanticOffsets = &m_UnityMeshInfo.m_SemanticOffsets;
		m_Exec_SAO2AOS[idx].m_VertexStride = m_UnityMeshInfo.m_VertexStride;
	}
	return true;
}

bool	CUnityBatchDrawer_Billboard::_RenderThread_AllocPerViewGeomBuffers(const SGeneratedInputs &genInputs)
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

static bool	_FindAdditionalInput(const CStringId &inputName, EBaseTypeID inputType, const TMemoryView<const SRendererFeatureFieldDefinition> &additionalInputs)
{
	PK_FOREACH(addInput, additionalInputs)
	{
		if (addInput->m_Name == inputName && addInput->m_Type == inputType)
			return true;
	}
	return false;
}


bool	CUnityBatchDrawer_Billboard::_RenderThread_AllocBillboardingBuffers(const SGeneratedInputs &genInputs)
{
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
		m_ParticleBuffers.m_UVRemap.ResizeIFN(Drawers::GenInput_UVRemap, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);
		m_ParticleBuffers.m_RawTexCoords0.ResizeIFN(Drawers::GenInput_RawUV0, genInputs.m_GeneratedInputs, m_ParticleBuffers.m_GeneratedInputs, m_VertexCount);

		if (_FindAdditionalInput(BasicRendererProperties::SID_Diffuse_Color(), BaseType_Float4, m_AdditionalInputs))
			m_ParticleBuffers.m_Colors.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_Diffuse_DiffuseColor(), BaseType_Float4, m_AdditionalInputs))
			m_ParticleBuffers.m_Colors.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_Atlas_TextureID(), BaseType_Float, m_AdditionalInputs))
			m_ParticleBuffers.m_AtlasId.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_Emissive_EmissiveColor(), BaseType_Float4, m_AdditionalInputs))
			m_ParticleBuffers.m_EmissiveColors4.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_Emissive_EmissiveColor(), BaseType_Float3, m_AdditionalInputs))
			m_ParticleBuffers.m_EmissiveColors3.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_AlphaRemap_Cursor(), BaseType_Float, m_AdditionalInputs))
			m_ParticleBuffers.m_AlphaCursor.ResizeIFN(m_VertexCount);

		if (_FindAdditionalInput(BasicRendererProperties::SID_TransformUVs_UVRotate(), BaseType_Float, m_AdditionalInputs))
			m_ParticleBuffers.m_TransformUVsRotate.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_TransformUVs_UVOffset(), BaseType_Float2, m_AdditionalInputs))
			m_ParticleBuffers.m_TransformUVsOffset.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_TransformUVs_UVScale(), BaseType_Float2, m_AdditionalInputs))
			m_ParticleBuffers.m_TransformUVsScale.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_AlphaMasks_Mask1AnimationCursor(), BaseType_Float, m_AdditionalInputs))
			m_ParticleBuffers.m_AlphaMask1Cursor.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_AlphaMasks_Mask2AnimationCursor(), BaseType_Float, m_AdditionalInputs))
			m_ParticleBuffers.m_AlphaMask2Cursor.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_UVDistortions_Distortion1AnimationCursor(), BaseType_Float, m_AdditionalInputs))
			m_ParticleBuffers.m_UVDistortion1Cursor.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_UVDistortions_Distortion2AnimationCursor(), BaseType_Float, m_AdditionalInputs))
			m_ParticleBuffers.m_UVDistortion2Cursor.ResizeIFN(m_VertexCount);
		if (_FindAdditionalInput(BasicRendererProperties::SID_Dissolve_DissolveAnimationCursor(), BaseType_Float, m_AdditionalInputs))
			m_ParticleBuffers.m_DissolveCursor.ResizeIFN(m_VertexCount);

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

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Billboard::_RenderThread_SetupBuffersBillboards(const SGeneratedInputs &toMap)
{
	PK_SCOPEDPROFILE();
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Indices) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Indices != null))
			return false;
		m_BBJobs_Billboard.m_Exec_Indices.m_IndexStream.m_Indices = m_ParticleBuffers.m_ViewIndependantGeom.m_Indices;
		m_BBJobs_Billboard.m_Exec_Indices.m_IndexStream.m_Count = m_IndexCount;
		m_BBJobs_Billboard.m_Exec_Indices.m_IndexStream.m_Large = m_UnityMeshInfo.m_LargeIndices == ManagedBool_True;
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Position) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Positions.m_Ptr != null))
			return false;
		m_BBJobs_Billboard.m_Exec_PNT.m_Positions = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)m_ParticleBuffers.m_ViewIndependantGeom.m_Positions.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Normal) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Normals.m_Ptr != null))
			return false;
		m_BBJobs_Billboard.m_Exec_PNT.m_Normals = TStridedMemoryView<CFloat3, 0x10>((CFloat3*)m_ParticleBuffers.m_ViewIndependantGeom.m_Normals.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_Tangent) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_ViewIndependantGeom.m_Tangents.m_Ptr != null))
			return false;
		m_BBJobs_Billboard.m_Exec_PNT.m_Tangents = TStridedMemoryView<CFloat4, 0x10>((CFloat4*)m_ParticleBuffers.m_ViewIndependantGeom.m_Tangents.m_Ptr, m_VertexCount, 0x10);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_UV0) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_TexCoords0.m_Ptr != null))
			return false;
		m_BBJobs_Billboard.m_Exec_Texcoords.m_Texcoords = TStridedMemoryView<CFloat2>(m_ParticleBuffers.m_TexCoords0.m_Ptr, m_VertexCount);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_UV1) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_TexCoords1.m_Ptr != null))
			return false;
		m_BBJobs_Billboard.m_Exec_Texcoords.m_Texcoords2 = TStridedMemoryView<CFloat2>(m_ParticleBuffers.m_TexCoords1.m_Ptr, m_VertexCount);
	}
	if ((toMap.m_GeneratedInputs & Drawers::GenInput_RawUV0) != 0)
	{
		if (!PK_VERIFY(m_ParticleBuffers.m_RawTexCoords0.m_Ptr != null))
			return false;
		m_BBJobs_Billboard.m_Exec_Texcoords.m_RawTexcoords = TStridedMemoryView<CFloat2>(m_ParticleBuffers.m_RawTexCoords0.m_Ptr, m_VertexCount);
	}

	// Map only the color and alpha cursor
	// We only handle max 8 additional fields:
	if (!m_ParticleBuffers.m_AdditionalFieldsBuffers.Reserve(2))
		return false;
	for (u32 i = 0; i < m_AdditionalInputs.Count(); ++i)
	{
		const SRendererFeatureFieldDefinition &addInput = m_AdditionalInputs[i];

		if ((addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() || addInput.m_Name == BasicRendererProperties::SID_Diffuse_DiffuseColor()) && addInput.m_Type == BaseType_Float4)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;

			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_Colors.m_Ptr;
			field.m_Storage.m_Stride = sizeof(CFloat4);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Atlas_TextureID() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc &field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;

			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8 *)m_ParticleBuffers.m_AtlasId.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Emissive_EmissiveColor())
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc &field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;

			field.m_Storage.m_Count = m_VertexCount;
			if (addInput.m_Type == BaseType_Float3)
			{
				field.m_Storage.m_RawDataPtr = (u8 *)m_ParticleBuffers.m_EmissiveColors3.m_Ptr;
				field.m_Storage.m_Stride = sizeof(CFloat3);
			}
			else if (addInput.m_Type == BaseType_Float4)
			{
				field.m_Storage.m_RawDataPtr = (u8 *)m_ParticleBuffers.m_EmissiveColors4.m_Ptr;
				field.m_Storage.m_Stride = sizeof(CFloat4);
			}
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
		else if (addInput.m_Name == BasicRendererProperties::SID_TransformUVs_UVOffset() && addInput.m_Type == BaseType_Float2)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_TransformUVsOffset.m_Ptr;
			field.m_Storage.m_Stride = sizeof(CFloat2);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_TransformUVs_UVScale() && addInput.m_Type == BaseType_Float2)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_TransformUVsScale.m_Ptr;
			field.m_Storage.m_Stride = sizeof(CFloat2);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_TransformUVs_UVRotate() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_TransformUVsRotate.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaMasks_Mask1AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_AlphaMask1Cursor.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaMasks_Mask2AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_AlphaMask2Cursor.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_UVDistortions_Distortion1AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_UVDistortion1Cursor.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_UVDistortions_Distortion2AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_UVDistortion2Cursor.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Dissolve_DissolveAnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_ParticleBuffers.m_AdditionalFieldsBuffers.PushBack().Valid()))
				return false;
			Drawers::SCopyFieldDesc		&field = m_ParticleBuffers.m_AdditionalFieldsBuffers.Last();
			field.m_AdditionalInputIndex = i;
			field.m_Storage.m_Count = m_VertexCount;
			field.m_Storage.m_RawDataPtr = (u8*)m_ParticleBuffers.m_DissolveCursor.m_Ptr;
			field.m_Storage.m_Stride = sizeof(float);
		}
	}
	m_BBJobs_Billboard.m_Exec_CopyField.m_FieldsToCopy = m_ParticleBuffers.m_AdditionalFieldsBuffers;

	// -----------------------------------------
	// View dependent inputs:
	// -----------------------------------------

	for (u32 i = 0; i < m_BBJobs_Billboard.m_PerView.Count(); ++i)
	{
		PK_ASSERT((i < m_ParticleBuffers.m_PerViewGeom.Count()));

		const SParticleBuffers::SPerView		&src = m_ParticleBuffers.m_PerViewGeom[i];
		SBillboardBatchJobs::SPerView			&dst = m_BBJobs_Billboard.m_PerView[i];
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
