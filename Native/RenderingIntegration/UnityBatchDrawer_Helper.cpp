//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "UnityBatchDrawer_Helper.h"
#include "FrameCollectorUnityTypes.h"
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <RuntimeManager.h>


//----------------------------------------------------------------------------

void	CBillboard_Exec_SOA_OAS::operator()(const Drawers::SBillboard_ExecPage &batch)
{
	const u32		renderedParticleCount = batch.m_Page->RenderedParticleCount();
	const u32		vertexCount = renderedParticleCount * batch.m_Billboarder->BillboardVertexCount();

	_CopyData(batch.m_VertexOffset, vertexCount);
}

//----------------------------------------------------------------------------

void	CBillboard_Exec_SOA_OAS::operator()(const Drawers::SRibbon_ExecBatch &batch)
{
	_CopyData(batch.FullVertexOffset(), batch.m_VertexCount);
}

//----------------------------------------------------------------------------

void	CBillboard_Exec_SOA_OAS::operator()(const Drawers::STriangle_ExecPage & batch)
{
	const u32		renderedParticleCount = batch.m_Page->RenderedParticleCount();
	const u32		vertexCount = renderedParticleCount * 3;

	_CopyData(batch.m_VertexOffset, vertexCount);
}

//----------------------------------------------------------------------------

void	CBillboard_Exec_SOA_OAS::_CopyData(u32 vertexOffset, u32 vertexCount)
{
	PK_NAMEDSCOPEDPROFILE("CBillboard_Exec_SOA_OAS::_CopyData VB Fill");

	PK_TODO("Of stride is a multiple of 16 bytes, store aligned the position");

	volatile void		*bfPtr = Mem::AdvanceRawPointer(m_MappedVertexBuffer[m_IdxView], m_VertexStride * vertexOffset);
	const u32	endVertex = vertexOffset + vertexCount;

	for (u32 vertexID = vertexOffset; vertexID < endVertex; vertexID++)
	{
		if (m_ParticleBuffers.m_Positions != null)
			FillPositions(&(m_ParticleBuffers.m_Positions[vertexID]), bfPtr, *m_SemanticOffsets);
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_Lighting) != 0)
		{
			FillNormals(&(m_ParticleBuffers.m_Normals[vertexID]), bfPtr, *m_SemanticOffsets);
			if (m_ParticleBuffers.m_Tangents != null)
				FillTangents(&(m_ParticleBuffers.m_Tangents[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_Color) != 0)
		{
			FillColors(&(m_ParticleBuffers.m_Colors[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_CorrectDeformation) != 0)
		{
			FillUVFactors(&(m_ParticleBuffers.m_UVFactors[vertexID]), bfPtr, *m_SemanticOffsets);
			if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0)
				FillAlphaCursor(&(m_ParticleBuffers.m_AlphaCursor[vertexID]), bfPtr, *m_SemanticOffsets); // Packed with UVFactor
			if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_TransformUVs) != 0)
				FillTransformUVsRotate(&(m_ParticleBuffers.m_TransformUVsRotate[vertexID]), bfPtr, *m_SemanticOffsets); // Packed with UVFactor

			FillUVScalesAndOffsets(&(((CFloat4*)m_ParticleBuffers.m_UVRemap)[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		else if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_AnimBlend) != 0)
		{
			FillUV0(&(m_ParticleBuffers.m_TexCoords0[vertexID]), bfPtr, *m_SemanticOffsets);
			FillUV1(&(m_ParticleBuffers.m_TexCoords1[vertexID]), bfPtr, *m_SemanticOffsets);
			FillAtlasId(&(m_ParticleBuffers.m_AtlasId[vertexID]), bfPtr, *m_SemanticOffsets);
			if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0)
				FillAlphaCursor(&(m_ParticleBuffers.m_AlphaCursor[vertexID]), bfPtr, *m_SemanticOffsets); // Packed with AtlasID
			if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_TransformUVs) != 0)
				FillTransformUVsRotate(&(m_ParticleBuffers.m_TransformUVsRotate[vertexID]), bfPtr, *m_SemanticOffsets); // Packed with AtlasID
		}
		else
		{
			if (PK_VERIFY(m_ParticleBuffers.m_TexCoords0 != null))
			{
				FillUV0(&(m_ParticleBuffers.m_TexCoords0[vertexID]), bfPtr, *m_SemanticOffsets);
				if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0)
					FillAlphaCursor(&(m_ParticleBuffers.m_AlphaCursor[vertexID]), bfPtr, *m_SemanticOffsets); // Packed with UV0
			}
		}

		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_Emissive) != 0)
		{
			if (m_IsEmissive3)
				FillEmissiveColors3(&(m_ParticleBuffers.m_EmissiveColors3[vertexID]), bfPtr, *m_SemanticOffsets);
			else
				FillEmissiveColors(&(m_ParticleBuffers.m_EmissiveColors4[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_TransformUVs) != 0)
		{
			if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_CorrectDeformation) == 0 &&
				(m_ShaderVariationFlags & ShaderVariationFlags::Has_AnimBlend) == 0)
			{
				FillTransformUVsRotate(&(m_ParticleBuffers.m_TransformUVsRotate[vertexID]), bfPtr, *m_SemanticOffsets);
				if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_Atlas) != 0)
				{
					FillAtlasId(&(m_ParticleBuffers.m_AtlasId[vertexID]), bfPtr, *m_SemanticOffsets);
				}
			}
			FillTransformUVsScaleAndOffset(&(((CFloat2*)m_ParticleBuffers.m_TransformUVsScale)[vertexID]), &(((CFloat2*)m_ParticleBuffers.m_TransformUVsOffset)[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_AlphaMasks))
		{
			FillAnimatedMaskedCursors(&(m_ParticleBuffers.m_AlphaMasksCursor1[vertexID]), &(m_ParticleBuffers.m_AlphaMasksCursor2[vertexID]), bfPtr, *m_SemanticOffsets);
		}

		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_UVDistortions))
		{
			FillUVDistortionsCursors(&(m_ParticleBuffers.m_UVDistortionsCursor1[vertexID]), &(m_ParticleBuffers.m_UVDistortionsCursor2[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_ShaderVariationFlags & ShaderVariationFlags::Has_Dissolve))
		{
			FillDissolveCursor(&(m_ParticleBuffers.m_DissolveCursor[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if (m_ParticleBuffers.m_RawTexCoords0 != null)
		{
			FillRawUV0(&(m_ParticleBuffers.m_RawTexCoords0[vertexID]), bfPtr, *m_SemanticOffsets);
		}
			
		bfPtr = Mem::AdvanceRawPointer(bfPtr, m_VertexStride);
	}
}

//----------------------------------------------------------------------------

void	CBilboard_Exec_BillboardInfo::operator()(const Drawers::CCopyStream_CPU &bb)
{
	_Prepare(bb.DrawRequests());
}

//----------------------------------------------------------------------------

void	CBilboard_Exec_BillboardInfo::operator()(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests)
{
	_Prepare(drawRequests);
}

//----------------------------------------------------------------------------

void	CBilboard_Exec_BillboardInfo::_Prepare(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests)
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
