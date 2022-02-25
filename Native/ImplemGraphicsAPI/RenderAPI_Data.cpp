//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "RenderAPI_Data.h"
#include "PKUnity_Scene.h"
#include "RuntimeManager.h"

#include "RenderingIntegration/UnityRendererCache.h"

#if (PK_BUILD_WITH_VULKAN_SUPPORT != 0)
#include <ImplemGraphicsAPI/RenderAPI_VulkanData.h>
extern class	IRenderAPIData* CreateVulkanData();
#endif
#if (PK_BUILD_WITH_D3D11_SUPPORT != 0)
#include <ImplemGraphicsAPI/RenderAPI_DX11Data.h>
extern class	IRenderAPIData* CreateDX11Data();
#endif
#if (PK_BUILD_WITH_D3D12_SUPPORT != 0)
#include <ImplemGraphicsAPI/RenderAPI_DX12Data.h>
extern class	IRenderAPIData* CreateDX12Data();
#endif
#if (PK_BUILD_WITH_GL_SUPPORT != 0)
extern class	IRenderAPIData* CreateGLData();
#endif
#if (PK_BUILD_WITH_GLES_SUPPORT != 0)
extern class	IRenderAPIData* CreateGLES2Data();
#endif
#if (PK_BUILD_WITH_GLES3_SUPPORT != 0)
extern class	IRenderAPIData* CreateGLES3Data();
#endif
#if (PK_BUILD_WITH_METAL_SUPPORT != 0)
#include <ImplemGraphicsAPI/RenderAPI_MetalData.h>
extern class	IRenderAPIData* CreateMetalData();
#endif
#if (PK_BUILD_WITH_NVN_SUPPORT != 0)
#include <ImplemGraphicsAPI/RenderAPI_NVNData.h>
extern class	IRenderAPIData* CreateNvnData();
#endif
#if defined(PK_ORBIS)
#include <ImplemGraphicsAPI/RenderAPI_GNMData.h>
extern class	IRenderAPIData* CreateGNMData();
#endif
#if defined(PK_UNKNOWN2)
#include <ImplemGraphicsAPI/RenderAPI_AGCData.h>
extern class	IRenderAPIData* UNKNOWN2Data();
#endif
#define	SHOULD_NOT_HAVE_FLAG	(1U << 31)
#define	SHOULD_NOT_HAVE(x)		(((unsigned int)x << ShaderVariationFlags::ShaderVariation_Count) | SHOULD_NOT_HAVE_FLAG)

//-------------------------------------------------------------------------------------

using namespace PopcornFX;


bool SBufferHandles::Init(UnityGfxRenderer deviceType)
{
	bool ret = true;
	switch (deviceType)
	{
#if (PK_BUILD_WITH_VULKAN_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererVulkan:
		m_Buffer = PK_NEW(CVulkanBufferHandles);
		break;
#endif
#if (PK_BUILD_WITH_D3D11_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererD3D11:
		m_Buffer = PK_NEW(CDX11BufferHandles);
		break;
#endif
#if (PK_BUILD_WITH_D3D12_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererD3D12:
		m_Buffer = PK_NEW(CDX12BufferHandles);
		break;
#endif
#if (PK_BUILD_WITH_GL_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererOpenGLCore:
		m_Buffer = PK_NEW(CBufferAbstract);
		break;
#endif
#if (PK_BUILD_WITH_GLES_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererOpenGLES20:
		m_Buffer = PK_NEW(CBufferAbstract);
		break;
#	if (PK_BUILD_WITH_GLES3_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererOpenGLES30:
		m_Buffer = PK_NEW(CBufferAbstract);
		break;
#	endif
#endif
#if (PK_BUILD_WITH_NVN_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererNvn:
		m_Buffer = PK_NEW(CNVNBufferHandles);
		break;
#endif
#if (PK_BUILD_WITH_METAL_SUPPORT != 0)
		case UnityGfxRenderer::kUnityGfxRendererMetal:
			m_Buffer = PK_NEW(CMetalBufferHandles);
			break;
#endif
#if defined(PK_ORBIS)
		case UnityGfxRenderer::kUnityGfxRendererPS4:
			m_Buffer = PK_NEW(CGNMBufferHandles);
			break;
#endif
#if defined(PK_UNKNOWN2)
		case UnityGfxRenderer::kUnityGfxRendererUNKNOWN2:
			m_Buffer = PK_NEW(CUNKNOWN2BufferHandles);
			break;
#endif
		case UnityGfxRenderer::kUnityGfxRendererNull:
			ret = false;
			break;
		default:
			PK_ASSERT_NOT_REACHED();
			CLog::Log(PK_ERROR, "SBufferHandles::Init : Unsupported API %d", deviceType);
			ret = false;
			break;
	}
	return ret;
}


u32		flagsToUseSemantic[__Semantic_Count] =
{
	0U,																																				// Semantic_Position			// Always
	ShaderVariationFlags::Has_Lighting,																												// Semantic_Normal
	ShaderVariationFlags::Has_Color,																												// Semantic_Color
	ShaderVariationFlags::Has_CorrectDeformation,																									// Semantic_UvFactors
	ShaderVariationFlags::Has_CorrectDeformation,																									// Semantic_UvScaleAndOffset
	SHOULD_NOT_HAVE(ShaderVariationFlags::Has_CorrectDeformation) | ShaderVariationFlags::Has_DiffuseMap | ShaderVariationFlags::Has_DistortionMap,	// Semantic_Uv0
	ShaderVariationFlags::Has_AnimBlend,																											// Semantic_Uv1
	ShaderVariationFlags::Has_AnimBlend,																											// Semantic_AtlasId
	ShaderVariationFlags::Has_AlphaRemap,																											// Semantic_AlphaCursor
};

u32		semanticSize[__Semantic_Count] =
{
	3 * sizeof(float),									// Semantic_Position
	3 * sizeof(float),									// Semantic_Normal
	4 * sizeof(float),									// Semantic_Color
	2 * sizeof(float),									// Semantic_UvFactors
	4 * sizeof(float),									// Semantic_UvScaleAndOffset
	2 * sizeof(float),									// Semantic_Uv0
	2 * sizeof(float),									// Semantic_Uv1
	sizeof(float),										// Semantic_AtlasId
	sizeof(float),										// Semantic_AlphaCursor
};

//----------------------------------------------------------------------------

IRenderAPIData *IRenderAPIData::GetRenderAPISpecificData(UnityGfxRenderer deviceType)
{
	switch (deviceType)
	{
#if (PK_BUILD_WITH_VULKAN_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererVulkan:
		return CreateVulkanData();
		break;
#endif
#if (PK_BUILD_WITH_D3D11_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererD3D11:
		return CreateDX11Data();
		break;
#endif
#if (PK_BUILD_WITH_D3D12_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererD3D12:
		return CreateDX12Data();
		break;
#endif
#if (PK_BUILD_WITH_GL_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererOpenGLCore:
		return CreateGLData();
		break;
#endif
#if (PK_BUILD_WITH_GLES_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererOpenGLES20:
		return CreateGLES2Data();
		break;
#	if (PK_BUILD_WITH_GLES3_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererOpenGLES30:
		return CreateGLES3Data();
		break;
#	endif
#endif
#if (PK_BUILD_WITH_NVN_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererNvn:
		return CreateNvnData();
		break;
#endif
#if (PK_BUILD_WITH_METAL_SUPPORT != 0)
	case UnityGfxRenderer::kUnityGfxRendererMetal:
		return CreateMetalData();
		break;
#endif
#if defined(PK_ORBIS)
	case UnityGfxRenderer::kUnityGfxRendererPS4:
		return CreateGNMData();
		break;
#endif
#if defined(PK_UNKNOWN2)
	case UnityGfxRenderer::kUnityGfxRendererUNKNOWN2:
		return UNKNOWN2Data();
		break;
#endif
	case UnityGfxRenderer::kUnityGfxRendererNull:
		break;
	default:
		PK_ASSERT_NOT_REACHED();
		CLog::Log(PK_ERROR, "GetRenderAPISpecificData : Unsupported API %d", deviceType);
		break;
	}
	return null;
}

//----------------------------------------------------------------------------

static	bool	AreFlagsCompatible(u32 flagToUseSemantic, u32 materialFlags)
{
	// flag is 0 == always
	if (flagToUseSemantic == 0)
		return true;
	// otherwise it should have all the flags
	if ((flagToUseSemantic & materialFlags) == 0)
		return false;
	if ((flagToUseSemantic & SHOULD_NOT_HAVE_FLAG) != 0 &&
		((materialFlags << ShaderVariationFlags::ShaderVariation_Count) & flagToUseSemantic) != 0)
		return false;
	return true;
}

//----------------------------------------------------------------------------

u32	FillOffsetTableAndGetVertexBufferStride(u32 offsetTable[__Semantic_Count], u32 shaderVariationFlags)
{
	u32	vertexStride = 0;
	u32	vertexOffset = 0;

	for (u32 i = 0; i < __Semantic_Count; ++i)
	{
		offsetTable[i] = 0xFFFFFFFF;
		if (AreFlagsCompatible(flagsToUseSemantic[i], shaderVariationFlags))
		{
			offsetTable[i] = vertexOffset;
			vertexOffset += semanticSize[i];
			// minimum stride of 2 * sizeof(float), we can pack 2 floats in a float2
			if (vertexOffset + semanticSize[i] > vertexStride)
				vertexStride += semanticSize[i];
		}
	}
	const bool	hasAnimBlend = (shaderVariationFlags & ShaderVariationFlags::Has_AnimBlend) != 0;
	const bool	hasAlphaRemap = (shaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0;

	// Both are single floats, so they require a padding of 1 if they are not present at the same time
	if (hasAnimBlend != hasAlphaRemap)
	{
		vertexStride += sizeof(float);
	}
	return vertexStride;
}

//----------------------------------------------------------------------------

u32		GetGeomBillboardVertexBufferStride(EBillboardMode bbMode, u32 shaderVariationFlags)
{
	u32		vertexStride = 0;

	vertexStride += 4 * sizeof(float); // float4 position


	if ((shaderVariationFlags & ShaderVariationFlags::Has_Size2) != 0)
	{
		vertexStride += 2 * sizeof(float); // float2 Size2
	}
	else
	{
		vertexStride += 1 * sizeof(float); // float size
	}
	switch (bbMode)
	{
	case BillboardMode_AxisAligned:
	case BillboardMode_AxisAlignedSpheroid:
		vertexStride += 3 * sizeof(float); // float3 Axis0
		break;
	case BillboardMode_AxisAlignedCapsule:
		vertexStride += 3 * sizeof(float); // float3 Axis0
		break;
	case BillboardMode_PlaneAligned:
		vertexStride += 6 * sizeof(float); // float3 Axis0, Axis1
		vertexStride += 1 * sizeof(float); // float rotation
		break;
	default:
		vertexStride += 1 * sizeof(float); // float rotation
		break;
	}

	if ((shaderVariationFlags & ShaderVariationFlags::Has_Color) != 0)
	{
		vertexStride += 4 * sizeof(float); // float4 Color
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0)
	{
		vertexStride += 1 * sizeof(float); // float AlphaRemap
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_Atlas) != 0)
	{
		vertexStride += 1 * sizeof(float); // float Atlas
	}
	return vertexStride;
}

//-------------------------------------------------------------------------------------
