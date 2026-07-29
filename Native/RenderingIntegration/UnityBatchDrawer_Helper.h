//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once

#include <pk_particles/include/Renderers/ps_renderer_base.h>
#include <pk_render_helpers/include/batch_jobs/rh_batch_jobs_billboard_cpu.h>
#include <pk_render_helpers/include/batch_jobs/rh_batch_jobs_ribbon_cpu.h>
#include <pk_render_helpers/include/batch_jobs/rh_batch_jobs_triangle_cpu.h>
#include <pk_render_helpers/include/draw_requests/rh_ribbon_cpu.h>
#include <pk_render_helpers/include/draw_requests/rh_billboard_cpu.h>
#include <pk_render_helpers/include/draw_requests/rh_triangle_cpu.h>
#include <pk_render_helpers/include/draw_requests/rh_copystream_cpu.h>
#include "UnityRendererCache.h"
#include "FrameCollectorUnityTypes.h"

//----------------------------------------------------------------------------

template<class _Type>
struct	SSizedBuffer
{
	_Type	*m_Ptr;
	u32		m_Count;

	SSizedBuffer()
	:	m_Ptr(null)
	,	m_Count(0)
	{
	}

	~SSizedBuffer()
	{
		FreeIFN();
	}

	void	ResizeIFN(Drawers::EGeneratedInput curInputFlag, u32 toAllocFlags, u32 prevToAllocFlags, u32 newVertexCount)
	{
		(void)prevToAllocFlags;
		if ((toAllocFlags & curInputFlag) != 0) // If we need this input buffer
		{
			if (m_Count < newVertexCount || m_Ptr == null)
			{
				u32		paddedVertexCount = Mem::Align<0x1000>(newVertexCount);
				PK_FREE(m_Ptr);
				m_Ptr = (_Type*)PK_MALLOC_ALIGNED(paddedVertexCount * sizeof(_Type), 0x10);
				m_Count = paddedVertexCount;
			}
		}
	}

	void	ResizeIFN(u32 newVertexCount)
	{
		if (m_Count < newVertexCount || m_Ptr == null)
		{
			u32			paddedVertexCount = Mem::Align<0x1000>(newVertexCount);
			PK_FREE(m_Ptr);
			m_Ptr = (_Type*)PK_MALLOC_ALIGNED(paddedVertexCount * sizeof(_Type), 0x10);
			m_Count = paddedVertexCount;
		}
	}

	void	FreeIFN()
	{
		PK_FREE(m_Ptr);
		m_Ptr = null;
		m_Count = 0;
	}
};

// --------------------------------------
// Billboarding buffers:
// --------------------------------------
// Allocated buffers:
struct	SParticleBuffers
{
	struct	SPerView
	{
		u32						m_GeneratedInputs;
		SSizedBuffer<CFloat4>	m_Positions;
		SSizedBuffer<CFloat4>	m_Normals;
		SSizedBuffer<CFloat4>	m_Tangents;
		SSizedBuffer<CFloat2>	m_UVFactors;

		// We do not use Sized buffers for indices as they are void*
		void					*m_Indices;

		SPerView()
		:	m_GeneratedInputs(0)
		,	m_Indices(null)
		{
		}

		~SPerView()
		{
			m_Positions.FreeIFN();
			m_Normals.FreeIFN();
			m_Tangents.FreeIFN();
			m_UVFactors.FreeIFN();
			PK_FREE(m_Indices);
		}
	};

	// Buffers info:
	u32						m_IndicesCount;
	u32						m_GeneratedInputs;

	ManagedBool				m_LargeIndices;

	// View dependent buffers:
	TArray<SPerView>		m_PerViewGeom;

	// View independent buffers:
	SPerView				m_ViewIndependantGeom;
	SSizedBuffer<CFloat2>	m_TexCoords0;
	SSizedBuffer<CFloat2>	m_TexCoords1;
	SSizedBuffer<CFloat2>	m_RawTexCoords0;
	SSizedBuffer<CFloat4>	m_UVRemap;

	// Additional fields that we handle:
	SSizedBuffer<float>		m_AtlasId;
	SSizedBuffer<CFloat4>	m_Colors;
	SSizedBuffer<CFloat3>	m_EmissiveColors3;
	SSizedBuffer<CFloat4>	m_EmissiveColors4;
	SSizedBuffer<float>		m_AlphaCursor;
	SSizedBuffer<float>		m_TransformUVsRotate;
	SSizedBuffer<CFloat2>	m_TransformUVsScale;
	SSizedBuffer<CFloat2>	m_TransformUVsOffset;
	SSizedBuffer<float>		m_AlphaMask1Cursor;
	SSizedBuffer<float>		m_AlphaMask2Cursor;
	SSizedBuffer<float>		m_UVDistortion1Cursor;
	SSizedBuffer<float>		m_UVDistortion2Cursor;
	SSizedBuffer<float>		m_DissolveCursor;

	// View on the additional fields, all are null except for color and alpha cursor:
	TArray<Drawers::SCopyFieldDesc>		m_AdditionalFieldsBuffers;

	SParticleBuffers()
	:	m_IndicesCount(0)
	,	m_GeneratedInputs(0)
	,	m_LargeIndices(ManagedBool_False)
	{
	}

	void	FreeAll()
	{
		m_PerViewGeom.Clear();
		m_TexCoords0.FreeIFN();
		m_TexCoords1.FreeIFN();
		m_RawTexCoords0.FreeIFN();
		m_AtlasId.FreeIFN();
		m_UVRemap.FreeIFN();
		m_Colors.FreeIFN();
		m_EmissiveColors3.FreeIFN();
		m_EmissiveColors4.FreeIFN();
		m_AlphaCursor.FreeIFN();

		m_TransformUVsRotate.FreeIFN();
		m_TransformUVsScale.FreeIFN();
		m_TransformUVsOffset.FreeIFN();

		m_AlphaMask1Cursor.FreeIFN();
		m_AlphaMask2Cursor.FreeIFN();
		m_UVDistortion1Cursor.FreeIFN();
		m_UVDistortion2Cursor.FreeIFN();
		m_DissolveCursor.FreeIFN();

	}
};

// --------------------------------------
// FINAL SOA TO AOS COPY:
// --------------------------------------

// Temp struct to gather the buffers of the first billboarded view (we only gave one mesh per batch for the moment):
struct		SParticleSourceBuffers
{
	void 					*m_Indices;
	CFloat4					*m_Positions;
	CFloat4					*m_Normals;
	CFloat4					*m_Tangents;
	CFloat2					*m_UVFactors;
	CFloat2					*m_TexCoords0;
	CFloat2					*m_TexCoords1;
	CFloat2					*m_RawTexCoords0;
	float					*m_AtlasId;
	CFloat4					*m_UVRemap;
	CFloat4					*m_Colors;
	CFloat3					*m_EmissiveColors3;
	CFloat4					*m_EmissiveColors4;
	float					*m_AlphaCursor;

	float					*m_TransformUVsRotate;
	CFloat2					*m_TransformUVsOffset;
	CFloat2					*m_TransformUVsScale;

	float					*m_AlphaMasksCursor1;
	float					*m_AlphaMasksCursor2;
	float					*m_UVDistortionsCursor1;
	float					*m_UVDistortionsCursor2;
	float					*m_DissolveCursor;

	SParticleSourceBuffers()
	{
		Clear();
	}

	void	FromParticleBuffers(const SParticleBuffers &buffers, u32 viewIdx)
	{
		if (buffers.m_GeneratedInputs & Drawers::GenInput_Indices)
			m_Indices = buffers.m_ViewIndependantGeom.m_Indices;
		else
			m_Indices = null;
		if (buffers.m_GeneratedInputs & Drawers::GenInput_Position)
			m_Positions = buffers.m_ViewIndependantGeom.m_Positions.m_Ptr;
		else
			m_Positions = null;
		if (buffers.m_GeneratedInputs & Drawers::GenInput_Normal)
			m_Normals = buffers.m_ViewIndependantGeom.m_Normals.m_Ptr;
		else
			m_Normals = null;
		if (buffers.m_GeneratedInputs & Drawers::GenInput_Tangent)
			m_Tangents = buffers.m_ViewIndependantGeom.m_Tangents.m_Ptr;
		else
			m_Tangents = null;
		if (buffers.m_GeneratedInputs & Drawers::GenInput_UVFactors)
			m_UVFactors = buffers.m_ViewIndependantGeom.m_UVFactors.m_Ptr;
		else
			m_UVFactors = null;

		m_TexCoords0 = buffers.m_TexCoords0.m_Ptr;
		m_TexCoords1 = buffers.m_TexCoords1.m_Ptr;
		m_RawTexCoords0 = buffers.m_RawTexCoords0.m_Ptr;
		m_AtlasId = buffers.m_AtlasId.m_Ptr;
		m_UVRemap = buffers.m_UVRemap.m_Ptr;
		m_Colors = buffers.m_Colors.m_Ptr;
		m_EmissiveColors3 = buffers.m_EmissiveColors3.m_Ptr;
		m_EmissiveColors4 = buffers.m_EmissiveColors4.m_Ptr;
		m_AlphaCursor = buffers.m_AlphaCursor.m_Ptr;
		m_TransformUVsRotate = buffers.m_TransformUVsRotate.m_Ptr;
		m_TransformUVsOffset = buffers.m_TransformUVsOffset.m_Ptr;
		m_TransformUVsScale = buffers.m_TransformUVsScale.m_Ptr;
		m_AlphaMasksCursor1 = buffers.m_AlphaMask1Cursor.m_Ptr;
		m_AlphaMasksCursor2 = buffers.m_AlphaMask2Cursor.m_Ptr;
		m_UVDistortionsCursor1 = buffers.m_UVDistortion1Cursor.m_Ptr;
		m_UVDistortionsCursor2 = buffers.m_UVDistortion2Cursor.m_Ptr;
		m_DissolveCursor = buffers.m_DissolveCursor.m_Ptr;

		if (buffers.m_PerViewGeom.Count() > viewIdx)
		{
			if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_Indices)
				m_Indices = buffers.m_PerViewGeom[viewIdx].m_Indices;
			if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_Position)
				m_Positions = buffers.m_PerViewGeom[viewIdx].m_Positions.m_Ptr;
			if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_Normal)
				m_Normals = buffers.m_PerViewGeom[viewIdx].m_Normals.m_Ptr;
			if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_Tangent)
				m_Tangents = buffers.m_PerViewGeom[viewIdx].m_Tangents.m_Ptr;
			if (buffers.m_PerViewGeom[viewIdx].m_GeneratedInputs & Drawers::GenInput_UVFactors)
				m_UVFactors = buffers.m_PerViewGeom[viewIdx].m_UVFactors.m_Ptr;
		}
	}

	void	Clear()
	{
		m_Indices = null;
		m_Positions = null;
		m_Normals = null;
		m_Tangents = null;
		m_UVFactors = null;
		m_TexCoords0 = null;
		m_TexCoords1 = null;
		m_RawTexCoords0 = null;
		m_AtlasId = null;
		m_UVRemap = null;
		m_Colors = null;
		m_EmissiveColors3 = null;
		m_EmissiveColors4 = null;
		m_AlphaCursor = null;
		m_TransformUVsRotate = null;
		m_TransformUVsOffset = null;
		m_TransformUVsScale = null;
		m_AlphaMasksCursor1 = null;
		m_AlphaMasksCursor2 = null;
		m_UVDistortionsCursor1 = null;
		m_UVDistortionsCursor2 = null;
		m_DissolveCursor = null;
	}
};

class	CBillboard_Exec_SOA_OAS
{
public:
	u32						m_IdxView = 0;
	u32						m_ShaderVariationFlags;
	bool					m_IsEmissive3 = false;

	SParticleSourceBuffers	m_ParticleBuffers;		// Src
	TArray<void*>			m_MappedVertexBuffer;	// Dst vtx

	// Dst buffers info:
	u32						(*m_SemanticOffsets)[__Semantic_Count];
	u32						m_VertexStride;

	CBillboard_Exec_SOA_OAS() : m_ShaderVariationFlags(0), m_VertexStride(0) { Mem::Clear(m_SemanticOffsets); }
	void	Clear() { Mem::Reinit(*this); }
	void	operator()(const Drawers::SBillboard_ExecPage &batch);
	void	operator()(const Drawers::SRibbon_ExecBatch &batch);
	void	operator()(const Drawers::STriangle_ExecPage &batch);

	void	_CopyData(u32 vertexOffset, u32 vertexCount);
};

class	CBilboard_Exec_BillboardInfo
{
public:
	struct	SBillboardInfo
	{
		u32			m_Flags;
		float		m_NormalBendingFactor;
	};

	TMemoryView<SBillboardInfo>		m_GeomDrawRequests;

	void	Clear() { Mem::Reinit(*this); }
	bool	Empty() const { return m_GeomDrawRequests.Empty(); }
	void	operator()(const Drawers::CCopyStream_CPU &bb);
	void	operator()(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests);
private:
	void	_Prepare(const TStridedMemoryView<const Drawers::SBase_DrawRequest * const> &drawRequests);
};


//----------------------------------------------------------------------------
