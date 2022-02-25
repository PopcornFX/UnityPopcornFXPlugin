//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "ParticleBaseDrawCall.h"

#include "NativeToManaged.h"

#if		PK_UNITY_EDITOR
#	include "EditorOnly/EditorNativeToManaged.h"
#endif

#include "FrameCollector/FrameCollector.h"
#include "FxRendererSubView.h"
#include "ImplemGraphicsAPI/RenderAPI_Data.h"

#include <hh_geometrics/include/ge_billboards.h>

//----------------------------------------------------------------------------

CParticleBaseDrawCall::CParticleBaseDrawCall()
	: m_UnityCallbacksDelayed(false)
	, m_GameThreadVertexCount(0)
	, m_GameThreadIndexCount(0)
	, m_GameThreadBBox(CAABB::DEGENERATED)
	, m_RendererType(Drawers::Renderer_Invalid)
	, m_FrameCollector(null)
	, m_DrawCallReady(false)
	, m_StartCleanIndexByteOffset(0)
	, m_CleanIndexByteCount(0)
	, m_LargeIndices(ManagedBool_False)
	, m_RendererGUID(-1)
{
}

//----------------------------------------------------------------------------

CParticleBaseDrawCall::~CParticleBaseDrawCall()
{
	m_ParticleBuffers.FreeAll();
	Clear();
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::Clear()
{
	m_DrawCallReady = false;
	m_VertexCount = 0;
	m_IndexCount = 0;
}

//----------------------------------------------------------------------------

bool		CParticleBaseDrawCall::RenderThread_AllocBuffers(hh_u32 vertexCount, hh_u32 indexCount)
{
	HH_SCOPEDPROFILE();
	m_VertexCount = vertexCount;
	m_IndexCount = indexCount;

	CParticleMaterialDescFlags	&materialDescFlags = m_MaterialDesc.m_Flags;

	if (vertexCount > m_ParticleBuffers.m_VertexCount)
	{
		// Allocate more to avoid resizing to often:
		vertexCount = Mem::Align<0x1000>(vertexCount);
		m_ParticleBuffers.m_VertexCount = vertexCount;
		// -----------------------------------------
		HH_FREE(m_ParticleBuffers.m_Positions);
		m_ParticleBuffers.m_Positions = (CFloat4*)HH_MALLOC_ALIGNED(vertexCount * sizeof(CFloat4), 0x10);
		if (materialDescFlags.HasMaterialFlags(MaterialFlags::Has_Color))
		{
			HH_FREE(m_ParticleBuffers.m_Colors);
			m_ParticleBuffers.m_Colors = (CFloat4*)HH_MALLOC_ALIGNED(vertexCount * sizeof(CFloat4), 0x10);
		}
		if (materialDescFlags.HasMaterialFlags(MaterialFlags::Has_Lighting))
		{
			HH_FREE(m_ParticleBuffers.m_Normals);
			m_ParticleBuffers.m_Normals = (CFloat4*)HH_MALLOC_ALIGNED(vertexCount * sizeof(CFloat4), 0x10);
		}
		HH_FREE(m_ParticleBuffers.m_TexCoords0);
		m_ParticleBuffers.m_TexCoords0 = (CFloat2*)HH_MALLOC_ALIGNED(vertexCount * sizeof(CFloat2), 0x10);
		if (materialDescFlags.HasMaterialFlags(MaterialFlags::Has_RibbonComplex))
		{
			HH_FREE(m_ParticleBuffers.m_TexCoords1);
			m_ParticleBuffers.m_TexCoords1 = HH_MALLOC_ALIGNED(vertexCount * sizeof(CFloat4), 0x10);
		}
		else if (materialDescFlags.HasMaterialFlags(MaterialFlags::Has_AnimBlend))
		{
			HH_FREE(m_ParticleBuffers.m_TexCoords1);
			m_ParticleBuffers.m_TexCoords1 = HH_MALLOC_ALIGNED(vertexCount * sizeof(CFloat2), 0x10);
			HH_FREE(m_ParticleBuffers.m_AtlasId);
			m_ParticleBuffers.m_AtlasId = (float*)HH_MALLOC_ALIGNED(vertexCount * sizeof(float), 0x10);
		}
		if (materialDescFlags.HasMaterialFlags(MaterialFlags::Has_AlphaRemap))
		{
			HH_FREE(m_ParticleBuffers.m_AlphaCursor);
			m_ParticleBuffers.m_AlphaCursor = (float*)HH_MALLOC_ALIGNED(vertexCount * sizeof(float), 0x10);
		}
	}

	const hh_u32	indexStride = (m_LargeIndices == ManagedBool_True ? sizeof(hh_u32) : sizeof(hh_u16));
	const hh_u32	vertexStride = FillOffsetTableAndGetVertexBufferStride(m_SemanticOffsets, m_MaterialDesc.m_Flags.m_MaterialFlags);
	const hh_u32	mappedIndexBufferSize = m_Slice.m_IBElemCount * indexStride;
	const hh_u32	mappedVertexBufferSize = m_Slice.m_VBElemCount * vertexStride;
	hh_u32			requiredIndexBufferSize = indexCount * indexStride;
	IRenderAPIData	*renderApiData = m_FrameCollector->GetRenderApiData();

	if (!HH_VERIFY(renderApiData != null))
	{
		CLog::Log(HH_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}

	if (m_FrameCollector->GetRenderApiData()->NeedTempBuffer())
	{

		if (mappedVertexBufferSize > m_ParticleBuffers.m_TempVBSize)
		{
			HH_FREE(m_ParticleBuffers.m_TempVB);
			HH_ONLY_IF_ASSERTS(const hh_u32	requiredVertexBufferSize = vertexCount * vertexStride);
			HH_ASSERT(requiredVertexBufferSize <= mappedVertexBufferSize);
			m_ParticleBuffers.m_TempVB = HH_MALLOC_ALIGNED(mappedVertexBufferSize, 0x10);
			m_ParticleBuffers.m_TempVBSize = mappedVertexBufferSize;
		}

		if (mappedIndexBufferSize > m_ParticleBuffers.m_IndexBufferSize)
		{
			HH_FREE(m_ParticleBuffers.m_Indices);
			HH_ASSERT(requiredIndexBufferSize <= mappedIndexBufferSize);
			m_ParticleBuffers.m_Indices = HH_MALLOC_ALIGNED(mappedIndexBufferSize, 0x10);
			m_ParticleBuffers.m_IndexBufferSize = mappedIndexBufferSize;
			m_ParticleBuffers.m_MappedIndices = m_ParticleBuffers.m_Indices;
			m_ParticleBuffers.m_MappedIndexBufferSize = m_ParticleBuffers.m_IndexBufferSize;
		}
		m_StartCleanIndexByteOffset = requiredIndexBufferSize;
		m_CleanIndexByteCount = mappedIndexBufferSize - requiredIndexBufferSize;
	}
	else
	{
		// We map the indices:
		void	*mappedIdxBuffer = m_FrameCollector->GetRenderApiData()->BeginModifyNativeBuffer(m_Slice.m_IBHandler, true, mappedIndexBufferSize, mappedIndexBufferSize);
		if (!HH_VERIFY(mappedIdxBuffer != null))
			return false;

		const bool		wasBillboardingInMappedBuffer = m_ParticleBuffers.BillboardInMappedIdxBuffer();
		const bool		needToConvertIdx = false;// m_LargeIndices && m_FrameCollector->UseShortIndices();
		const bool		idxBufferNotAligned = !Mem::IsAligned<0x10>(mappedIdxBuffer);

		if (needToConvertIdx || idxBufferNotAligned) // Billboard in a temporary index buffer to convert idx or buffer not aligned
		{
			if (wasBillboardingInMappedBuffer || requiredIndexBufferSize > m_ParticleBuffers.m_IndexBufferSize)
			{
				if (!wasBillboardingInMappedBuffer)
					HH_FREE(m_ParticleBuffers.m_Indices);

				// Allocate more to avoid resizing to often:
				requiredIndexBufferSize = Mem::Align<0x1000>(requiredIndexBufferSize);
				m_ParticleBuffers.m_Indices = HH_MALLOC_ALIGNED(requiredIndexBufferSize, 0x10);
				m_ParticleBuffers.m_IndexBufferSize = requiredIndexBufferSize;

				m_StartCleanIndexByteOffset = 0;
				m_CleanIndexByteCount = 0;
			}
		}
		else // Billboard in the mapped index buffer
		{
			if (!wasBillboardingInMappedBuffer)
				HH_FREE(m_ParticleBuffers.m_Indices);

			HH_ASSERT(requiredIndexBufferSize <= mappedIndexBufferSize);

			HH_TODO("Try to partially map the GPU buffers to avoid filling the whole index buffer with zeros each frame");

			m_StartCleanIndexByteOffset = requiredIndexBufferSize;
			m_CleanIndexByteCount = mappedIndexBufferSize - requiredIndexBufferSize;

			m_ParticleBuffers.m_IndexBufferSize = mappedIndexBufferSize;
			m_ParticleBuffers.m_Indices = mappedIdxBuffer;
		}
		m_ParticleBuffers.m_MappedIndexBufferSize = mappedIndexBufferSize;
		m_ParticleBuffers.m_MappedIndices = mappedIdxBuffer;
	}
	return true;
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::GameThread_UpdateVertexAndIndexCount(const Drawers::SBillboard_DrawRequest &dr)
{
	m_GameThreadVertexCount += Drawers::BBModeVPP(dr.m_Mode) * dr.ParticleCount();
	m_GameThreadIndexCount += Drawers::BBModeIPP(dr.m_Mode) * dr.ParticleCount();
	m_GameThreadBBox.Add(dr.StreamToRender().BBox());
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::GameThread_UpdateVertexAndIndexCount(const Drawers::SRibbon_DrawRequest &dr)
{
	m_GameThreadVertexCount += 4 * dr.ParticleCount();
	m_GameThreadIndexCount += 6 * dr.ParticleCount();
	m_GameThreadBBox.Add(dr.StreamToRender().BBox());
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::GameThread_UpdateVertexAndIndexCount(const Drawers::SMesh_DrawRequest &dr)
{
	m_GameThreadVertexCount += dr.ParticleCount();
	m_GameThreadBBox.Add(dr.StreamToRender().BBox());
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::GameThread_CreateUnityRenderers(bool delayCallbacks)
{
	// Create the renderer:
	if (m_RendererGUID == -1)
	{
		SPopcornRendererDesc	desc;

		desc.m_UserData = m_MaterialDesc.m_UserData.ToStringData();
		desc.m_EffectNames = m_MaterialDesc.m_EffectNamePath.Data();
		desc.m_MaterialFlags = m_MaterialDesc.m_Flags.m_MaterialFlags;
		desc.m_UniformFlags = m_MaterialDesc.m_Flags.m_UniformFlags;
		desc.m_DrawOrder = m_MaterialDesc.m_DrawOrder;
		desc.m_DiffuseMap = m_MaterialDesc.m_Diffuse.ToStringData();
		desc.m_NormalMap = null;
		desc.m_AlphaRemap = m_MaterialDesc.m_AlphaRemap.ToStringData();
		desc.m_InvSofnessDistance = m_MaterialDesc.m_InvSoftnessDistance;

		SRetrieveRendererInfo	rendererInfo;

		rendererInfo.m_VBHandler = &m_Slice.m_VBHandler;
		rendererInfo.m_IBHandler = &m_Slice.m_IBHandler;
		rendererInfo.m_VertexBufferSize = &m_Slice.m_VBElemCount;
		rendererInfo.m_IndexBufferSize = &m_Slice.m_IBElemCount;
		rendererInfo.m_IsIndex32 = &m_LargeIndices;

		m_FrameCollector->CreatePopcornRenderer(delayCallbacks, desc, m_RendererType, &m_RendererGUID, rendererInfo);
		m_UnityCallbacksDelayed = delayCallbacks ? true : m_UnityCallbacksDelayed;
	}
	else
	{
		HH_ASSERT_NOT_REACHED();
	}
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::GameThread_AddRendererNameToUnity(const CString &rendererName)
{
#if		PK_UNITY_EDITOR
	CString		concatenateRendererNames = ";" + rendererName;

	m_MaterialDesc.m_EffectNamePath += concatenateRendererNames;
	if (HH_VERIFY(_OnRendererNamesAdded != null))
	{
		_OnRendererNamesAdded(m_RendererGUID, concatenateRendererNames.Data());
	}
#endif
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::GameThread_ResizeUnityRenderers(bool delayUnityCallbacks, bool hasBounds)
{
	HH_SCOPEDPROFILE();

	m_UnityCallbacksDelayed = false;

	// Create the renderer IFN
	if (m_RendererGUID == -1)
	{
		CLog::Log(HH_WARNING, "Performance info: some effects where not preloaded");
		GameThread_CreateUnityRenderers(delayUnityCallbacks);
	}

	if (m_GameThreadVertexCount == 0)
	{
		if (m_Slice.m_RendererActive)
		{
			m_FrameCollector->SetRendererActive(delayUnityCallbacks, m_RendererGUID, false);
			m_UnityCallbacksDelayed = delayUnityCallbacks ? true : m_UnityCallbacksDelayed;
			m_Slice.m_RendererActive = false;
		}
	}
	else if (	m_GameThreadVertexCount > (hh_u32)m_Slice.m_VBElemCount ||
				m_GameThreadIndexCount > (hh_u32)m_Slice.m_IBElemCount)
	{
		if (!m_Slice.m_RendererActive)
		{
			m_FrameCollector->SetRendererActive(delayUnityCallbacks, m_RendererGUID, true);
			m_UnityCallbacksDelayed = delayUnityCallbacks ? true : m_UnityCallbacksDelayed;
			m_Slice.m_RendererActive = true;
		}

		SRetrieveRendererInfo	rendererInfo;

		rendererInfo.m_IsIndex32 = &m_LargeIndices;
		rendererInfo.m_VertexBufferSize = &m_Slice.m_VBElemCount;
		rendererInfo.m_IndexBufferSize = &m_Slice.m_IBElemCount;
		rendererInfo.m_VBHandler = &m_Slice.m_VBHandler;
		rendererInfo.m_IBHandler = &m_Slice.m_IBHandler;

		m_FrameCollector->ResizeRenderer(delayUnityCallbacks, m_RendererGUID, m_GameThreadVertexCount, m_GameThreadIndexCount, rendererInfo);
		m_UnityCallbacksDelayed = delayUnityCallbacks ? true : m_UnityCallbacksDelayed;
	}
	else if (!m_Slice.m_RendererActive)
	{
		m_FrameCollector->SetRendererActive(delayUnityCallbacks, m_RendererGUID, true);
		m_UnityCallbacksDelayed = delayUnityCallbacks ? true : m_UnityCallbacksDelayed;
		m_Slice.m_RendererActive = true;
	}

	// Update the bounding box:
	if (m_GameThreadBBox != CAABB::DEGENERATED && hasBounds)
	{
		SUpdateRendererBounds	bounds;

		bounds.m_Min = m_GameThreadBBox.Min();
		bounds.m_Max = m_GameThreadBBox.Max();

		m_FrameCollector->UpdateRendererBounds(delayUnityCallbacks, m_RendererGUID, bounds);
	}

	// Reset the values for the next frame:
	m_GameThreadVertexCount = 0;
	m_GameThreadIndexCount = 0;
	m_GameThreadBBox = CAABB::DEGENERATED;
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::RenderThread_CleanUnusedIndices()
{
	HH_SCOPEDPROFILE();
	if (m_ParticleBuffers.BillboardInMappedIdxBuffer() || m_FrameCollector->GetRenderApiData()->NeedTempBuffer())
	{
		void		*startPtr = Mem::AdvanceRawPointer(m_ParticleBuffers.m_Indices, m_StartCleanIndexByteOffset);
		Mem::Clear(startPtr, m_CleanIndexByteCount);
		m_StartCleanIndexByteOffset = 0;
		m_CleanIndexByteCount = 0;
	}
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::RenderThread_FillIndexBuffer()
{
	if (m_ParticleBuffers.m_MappedIndices != null)
	{
		const hh_u32	srcIndexBufferSize = m_IndexCount * (m_LargeIndices == ManagedBool_True ? sizeof(hh_u32) : sizeof(hh_u16));
		void			*IdxBufferEnd = Mem::AdvanceRawPointer(m_ParticleBuffers.m_MappedIndices, srcIndexBufferSize);

		memcpy(m_ParticleBuffers.m_MappedIndices, m_ParticleBuffers.m_Indices, srcIndexBufferSize);
		Mem::Clear_Uncached(IdxBufferEnd, m_ParticleBuffers.m_MappedIndexBufferSize - srcIndexBufferSize);
	}
}

//----------------------------------------------------------------------------

void			CParticleBaseDrawCall::VertexBufferSAO2AOSPrepareCopy()
{
	HH_SCOPEDPROFILE();

	if (!HH_VERIFY_MESSAGE(m_Slice.m_VBHandler != nullptr && m_Slice.m_IBHandler != nullptr,
		"Native rendering seems to be not supported"))
		return;

	m_VertexStride = FillOffsetTableAndGetVertexBufferStride(m_SemanticOffsets, m_MaterialDesc.m_Flags.m_MaterialFlags);

	const hh_u32	vbSize = m_VertexStride * m_Slice.m_VBElemCount;

	void	*mappedVertexBuffer;
	if (m_FrameCollector->GetRenderApiData()->NeedTempBuffer())
		mappedVertexBuffer = m_ParticleBuffers.m_TempVB;
	else
		mappedVertexBuffer = m_FrameCollector->GetRenderApiData()->BeginModifyNativeBuffer(m_Slice.m_VBHandler, false, vbSize, vbSize);

	m_Exec_SAO2AOS.Clear();
	m_Exec_SAO2AOS.m_MaterialFlags = m_MaterialDesc.m_Flags.m_MaterialFlags;
	m_Exec_SAO2AOS.m_FrameCollector = m_FrameCollector;
	m_Exec_SAO2AOS.m_ParticleBuffers = &m_ParticleBuffers;
	m_Exec_SAO2AOS.m_MappedVertexBuffer = mappedVertexBuffer;
	m_Exec_SAO2AOS.m_SemanticOffsets = &m_SemanticOffsets;
	m_Exec_SAO2AOS.m_VertexStride = m_VertexStride;
}

void	CParticleBaseDrawCall::CBillboard_Exec_SOA_OAS::operator()(const Drawers::SBillboard_ExecPage &batch)
{
	const hh_u32		particleCount = batch.m_Page->ParticleCount();
	const hh_u32		vertexCount = particleCount * batch.m_Billboarder->BillboardVertexCount();

	_CopyData(batch.m_VertexOffset, vertexCount);
}

void	CParticleBaseDrawCall::CBillboard_Exec_SOA_OAS::operator()(const Drawers::SRibbon_ExecBatch &batch)
{
	_CopyData(batch.FullVertexOffset(), batch.m_VertexCount);
}

void	CParticleBaseDrawCall::CBillboard_Exec_SOA_OAS::_CopyData(hh_u32 vertexOffset, hh_u32 vertexCount)
{
	HH_NAMEDSCOPEDPROFILE("CBillboard_Exec_SOA_OAS::_CopyData VB Fill");

	HH_TODO("Of stride is a multiple of 16 bytes, store aligned the position");

	void			*bfPtr = Mem::AdvanceRawPointer(m_MappedVertexBuffer, m_VertexStride * vertexOffset);
	const hh_u32	endVertex = vertexOffset + vertexCount;

	for (hh_u32 vertexID = vertexOffset; vertexID < endVertex; vertexID++)
	{
		FillPositions(&(m_ParticleBuffers->m_Positions[vertexID]), bfPtr, *m_SemanticOffsets);
		if ((m_MaterialFlags & MaterialFlags::Has_Lighting) != 0)
		{
			FillNormals(&(m_ParticleBuffers->m_Normals[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_MaterialFlags & MaterialFlags::Has_Color) != 0)
		{
			FillColors(&(m_ParticleBuffers->m_Colors[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_MaterialFlags & MaterialFlags::Has_RibbonComplex) != 0)
		{
			FillUVFactors(&(m_ParticleBuffers->m_TexCoords0[vertexID]), bfPtr, *m_SemanticOffsets);
			FillUVScalesAndOffsets(&(((CFloat4*)m_ParticleBuffers->m_TexCoords1)[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		else
		{
			FillUV0(&(m_ParticleBuffers->m_TexCoords0[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_MaterialFlags & MaterialFlags::Has_AnimBlend) != 0)
		{
			FillUV1(&(((CFloat2*)m_ParticleBuffers->m_TexCoords1)[vertexID]), bfPtr, *m_SemanticOffsets);
			FillAtlasId(&(m_ParticleBuffers->m_AtlasId[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		if ((m_MaterialFlags & MaterialFlags::Has_AlphaRemap) != 0)
		{
			FillAlphaCursor(&(m_ParticleBuffers->m_AlphaCursor[vertexID]), bfPtr, *m_SemanticOffsets);
		}
		bfPtr = Mem::AdvanceRawPointer(bfPtr, m_VertexStride);
	}
}
