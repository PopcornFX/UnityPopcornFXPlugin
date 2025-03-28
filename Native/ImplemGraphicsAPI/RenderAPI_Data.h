//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------

#include "UnityGraphicsAPI/IUnityGraphics.h"
#include <pk_maths/include/pk_maths_simd.h>
#include <pk_particles/include/Renderers/ps_renderer_enums.h>

#include <pk_kernel/include/kr_refptr.h>

//-------------------------------------------------------------------------------------

class CBufferAbstract : public CRefCountedObject
{
public:
	void	*m_DeviceLocal;

	CBufferAbstract()
	: m_DeviceLocal(null)
	{
	}

	virtual ~CBufferAbstract()
	{
	}
};
PK_DECLARE_REFPTRCLASS(BufferAbstract);

struct	SBufferHandles
{
#if defined(PK_MACOSX) || defined(PK_IOS) || defined(PK_NX) || defined(PK_ORBIS)
	static const u32 HOST_VISIBLE_BUFFER_NUMBER = 3;
#else
	static const u32 HOST_VISIBLE_BUFFER_NUMBER = 2;
#endif

	PBufferAbstract	m_Buffer;

	SBufferHandles()
	: m_Buffer(null)
	{
	}

	SBufferHandles(const SBufferHandles &other)
	{
		m_Buffer = other.m_Buffer;
	}

	virtual ~SBufferHandles()
	{
		m_Buffer = null;
	}

	bool Init(UnityGfxRenderer deviceType);
};

//-------------------------------------------------------------------------------------

class	IRenderAPIData
{
public:
	virtual ~IRenderAPIData() {}

	virtual void BeginFrame() {}
	virtual void EndFrame() {}
	virtual void *BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize) = 0;
	virtual void EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff) = 0;
	virtual void LoadContextFunctions() {}

	static IRenderAPIData	*GetRenderAPISpecificData(UnityGfxRenderer deviceType);
};

//-------------------------------------------------------------------------------------

enum	EVertexSemantic
{
	Semantic_Position = 0,
	Semantic_Normal,
	Semantic_Tangent,
	Semantic_Color,

	Semantic_UvFactors,
	Semantic_UvScaleAndOffset,

	Semantic_Uv0,
	Semantic_Uv1,
	Semantic_AtlasId,

	Semantic_AlphaCursor,
	Semantic_EmissiveColor3,
	Semantic_EmissiveColor4,

	Semantic_TransformUVsRotate,
	Semantic_TransformUVsScaleAndOffset,

	Semantic_AlphaMasksCursors,
	Semantic_UVDistortionsCursors,
	Semantic_DissolveCursor,
	Semantic_RawUv0,

	__Semantic_Count
};

u32		FillOffsetTableAndGetVertexBufferStride(u32 offsetTable[__Semantic_Count], u32 shaderVariationFlags, bool isEmissive3 = false);
u32		GetGeomBillboardVertexBufferStride(EBillboardMode bbMode, u32 shaderVariationFlags);

#define SIMDFY	0

PK_TODO("Implement Simd version with volatile");

//-------------------------------------------------------------------------------------

PK_INLINE void	FillPositions(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_Position]);
#if	(SIMDFY != 0)
	const SIMD::Float4 pos_s = SIMD::Float4::LoadAligned16(stream);

	//pos_s.StoreUnaligned(dst);
	// Saves 500K cycles/call for CParticleBillboardDrawCall::FinishBillboarding
	pos_s.StoreFloat3(dst);
#else
	*(CFloat3*)dst = *(const CFloat3*)stream;
#endif
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillNormals(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_Normal]);
#if	(SIMDFY != 0)
	const SIMD::Float4 norm_s = SIMD::Float4::LoadAligned16(stream);

	norm_s.StoreFloat3(dst);
#else
	*(CFloat3*)dst = *(const CFloat3*)stream;
#endif
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillTangents(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_Tangent]);
#if	(SIMDFY != 0)
	const SIMD::Float4 norm_s = SIMD::Float4::LoadAligned16(stream);

	norm_s.StoreUnaligned(dst);
#else
	*(CFloat4*)dst = *(const CFloat4*)stream;
#endif
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillColors(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_Color]);
#if	(SIMDFY != 0)
	const SIMD::Float4 col_s = SIMD::Float4::LoadAligned16(stream);

	col_s.StoreUnaligned(dst);
#else
	*(CFloat4*)dst = *(const CFloat4*)stream;
#endif
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillEmissiveColors(void *const stream, volatile void *const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void *dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_EmissiveColor4]);
#if	(SIMDFY != 0)
	const SIMD::Float4 norm_s = SIMD::Float4::LoadAligned16(stream);

	norm_s.StoreFloat3(dst);
#else
	*(CFloat4 *)dst = *(const CFloat4 *)stream;
#endif
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillEmissiveColors3(void *const stream, volatile void *const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void *dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_EmissiveColor3]);
#if	(SIMDFY != 0)
	const SIMD::Float4 norm_s = SIMD::Float4::LoadAligned16(stream);

	norm_s.StoreFloat3(dst);
#else
	*(CFloat3 *)dst = *(const CFloat3 *)stream;
#endif
}


//-------------------------------------------------------------------------------------

PK_INLINE void	FillUVFactors(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_UvFactors]);
	*(CFloat2*)dst = *(const CFloat2*)stream;
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillUVScalesAndOffsets(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_UvScaleAndOffset]);
#if	(SIMDFY != 0)
	const SIMD::Float4 uv_s = SIMD::Float4::LoadAligned16(stream);

	uv_s.StoreUnaligned(dst);
#else
	*(CFloat4*)dst = *(const CFloat4*)stream;
#endif
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillUV0(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_Uv0]);
	*(CFloat2*)dst = *(const CFloat2*)stream;
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillUV1(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_Uv1]);
	*(CFloat2*)dst = *(const CFloat2*)stream;
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillRawUV0(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_RawUv0]);
	*(CFloat2*)dst = *(const CFloat2*)stream;
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillAtlasId(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_AtlasId]);
	*(float*)dst = *(const float*)stream;
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillAlphaCursor(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_AlphaCursor]);
	*(float*)dst = *(const float*)stream;
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillTransformUVsRotate(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_TransformUVsRotate]);
	*(float*)dst = *(const float*)stream;
}

//-------------------------------------------------------------------------------------

PK_INLINE void	FillTransformUVsScaleAndOffset(void * const streamScale, void * const streamOffset, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_TransformUVsScaleAndOffset]);
	*(CFloat2*)dst = *(const CFloat2*)streamScale;
	dst = Mem::AdvanceRawPointer(dst, sizeof(CFloat2));
	*(CFloat2*)dst = *(const CFloat2*)streamOffset;
}

PK_INLINE void	FillAnimatedMaskedCursors(void * const streamOne, void * const streamTwo, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_AlphaMasksCursors]);
	*(float*)dst = *(const float*)streamOne;
	dst = Mem::AdvanceRawPointer(dst, sizeof(float));
	*(float*)dst = *(const float*)streamTwo;
}

PK_INLINE void	FillUVDistortionsCursors(void * const streamOne, void * const streamTwo, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_UVDistortionsCursors]);
	*(float*)dst = *(const float*)streamOne;
	dst = Mem::AdvanceRawPointer(dst, sizeof(float));
	*(float*)dst = *(const float*)streamTwo;
}

PK_INLINE void	FillDissolveCursor(void * const stream, volatile void * const dstPtr, const u32(&offsetTable)[__Semantic_Count])
{
	volatile void	*dst = Mem::AdvanceRawPointer(dstPtr, offsetTable[Semantic_DissolveCursor]);
	*(float*)dst = *(const float*)stream;
}
#undef SIMDFY
//-------------------------------------------------------------------------------------
