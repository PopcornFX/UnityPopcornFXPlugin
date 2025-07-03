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
extern class	IRenderAPIData* UNKNOWN2Data(bool);
#endif

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
#	if 0 // Removed in Unity 6
	case UnityGfxRenderer::kUnityGfxRendererOpenGLES20:
		m_Buffer = PK_NEW(CBufferAbstract);
		break;
#	endif
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
		case UnityGfxRenderer::kUnityGfxRendererUNKNOWN2NGGC:
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


struct	SFlagsToUseSemantic
{
	u32		m_Forbidden;
	u32		m_Required;
};

SFlagsToUseSemantic		flagsToUseSemantic[__Semantic_Count] =
{
	{ 0U, 0U },																															// Semantic_Position			// Always
	{ 0U, ShaderVariationFlags::Has_Lighting },																							// Semantic_Normal
	{ 0U, ShaderVariationFlags::Has_Lighting },																							// Semantic_Tangent
	{ 0U, ShaderVariationFlags::Has_Color },																							// Semantic_Color0
	{ 0U, ShaderVariationFlags::Has_CorrectDeformation },																				// Semantic_UvFactors
	{ 0U, ShaderVariationFlags::Has_CorrectDeformation },																				// Semantic_UvScaleAndOffset
	{ ShaderVariationFlags::Has_CorrectDeformation, ShaderVariationFlags::Has_DiffuseMap | ShaderVariationFlags::Has_DistortionMap | ShaderVariationFlags::Has_Emissive },	// Semantic_Uv0
	{ 0U, ShaderVariationFlags::Has_AnimBlend },																						// Semantic_Uv1
	{ 0U, ShaderVariationFlags::Has_AnimBlend | ShaderVariationFlags::Has_TransformUVs },												// Semantic_AtlasId
	{ 0U, ShaderVariationFlags::Has_AlphaRemap },																						// Semantic_AlphaCursor
	{ 0U, ShaderVariationFlags::Has_Emissive },																							// Semantic_EmissiveColor3
	{ 0U, ShaderVariationFlags::Has_Emissive },																							// Semantic_EmissiveColor4
	{ 0U, ShaderVariationFlags::Has_TransformUVs },																						// Semantic_TransformUvsRotate
	{ 0U, ShaderVariationFlags::Has_TransformUVs },																						// Semantic_TransformUvsScaleAndOffset
	{ ShaderVariationFlags::Has_TransformUVs, ShaderVariationFlags::Has_AlphaMasks },													// Semantic_AlphaMasksCursors
	{ ShaderVariationFlags::Has_TransformUVs, ShaderVariationFlags::Has_UVDistortions },												// Semantic_UVDistortionCursors
	{ ShaderVariationFlags::Has_TransformUVs | ShaderVariationFlags::Has_AlphaRemap, ShaderVariationFlags::Has_Dissolve },				// Semantic_DissolveCursor
};

u32		semanticSize[__Semantic_Count] =
{
	3 * sizeof(float),									// Semantic_Position
	3 * sizeof(float),									// Semantic_Normal
	4 * sizeof(float),									// Semantic_Tangent
	4 * sizeof(float),									// Semantic_Color0
	2 * sizeof(float),									// Semantic_UvFactors
	4 * sizeof(float),									// Semantic_UvScaleAndOffset
	2 * sizeof(float),									// Semantic_Uv0
	2 * sizeof(float),									// Semantic_Uv1
	1 * sizeof(float),									// Semantic_AtlasId
	1 * sizeof(float),									// Semantic_AlphaCursor
	3 * sizeof(float),									// Semantic_EmissiveColor3
	4 * sizeof(float),									// Semantic_EmissiveColor4
	1 * sizeof(float),									// Semantic_TransformUvsRotate
	4 * sizeof(float),									// Semantic_TransformUvsScaleAndOffset
	2 * sizeof(float),									// Semantic_AlphaMasksCursors
	2 * sizeof(float),									// Semantic_UVDistortionsCursors
	1 * sizeof(float),									// Semantic_DissolveCursor
	2 * sizeof(float),									// Semantic_RawUv0
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
#	if 0 // Removed in Unity 6
	case UnityGfxRenderer::kUnityGfxRendererOpenGLES20:
		return CreateGLES2Data();
		break;
#	endif
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
		return UNKNOWN2Data(false);
		break;
	case UnityGfxRenderer::kUnityGfxRendererUNKNOWN2NGGC:
		return UNKNOWN2Data(true);
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

u32	FillOffsetTableAndGetVertexBufferStride(u32 offsetTable[__Semantic_Count], u32 shaderVariationFlags, bool isEmissive3 /*= false*/)
{
	u32	vertexOffset = 0;

	for (u32 i = 0; i < __Semantic_Count; ++i)
	{
		offsetTable[i] = 0xFFFFFFFF;
	}
	offsetTable[Semantic_Position] = vertexOffset;
	vertexOffset += semanticSize[Semantic_Position];
	if ((shaderVariationFlags & ShaderVariationFlags::Has_Lighting) != 0)
	{
		offsetTable[Semantic_Normal] = vertexOffset;
		vertexOffset += semanticSize[Semantic_Normal];
		offsetTable[Semantic_Tangent] = vertexOffset;
		vertexOffset += semanticSize[Semantic_Tangent];
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_Color) != 0)
	{
		offsetTable[Semantic_Color] = vertexOffset;
		vertexOffset += semanticSize[Semantic_Color];
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_CorrectDeformation) != 0)
	{
		offsetTable[Semantic_UvFactors] = vertexOffset;
		vertexOffset += semanticSize[Semantic_UvFactors];
		if ((shaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0)
		{
			offsetTable[Semantic_AlphaCursor] = vertexOffset;
			vertexOffset += semanticSize[Semantic_AlphaCursor];
		}
		if ((shaderVariationFlags & ShaderVariationFlags::Has_TransformUVs) != 0)
		{
			offsetTable[Semantic_TransformUVsRotate] = vertexOffset;
			vertexOffset += semanticSize[Semantic_TransformUVsRotate];
		}
		offsetTable[Semantic_UvScaleAndOffset] = vertexOffset;
		vertexOffset += semanticSize[Semantic_UvScaleAndOffset];
	}
	else if ((shaderVariationFlags & ShaderVariationFlags::Has_AnimBlend) != 0)
	{
		offsetTable[Semantic_Uv0] = vertexOffset;
		vertexOffset += semanticSize[Semantic_Uv0];
		offsetTable[Semantic_Uv1] = vertexOffset;
		vertexOffset += semanticSize[Semantic_Uv1];
		offsetTable[Semantic_AtlasId] = vertexOffset;
		vertexOffset += semanticSize[Semantic_AtlasId];
		if ((shaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0)
		{
			offsetTable[Semantic_AlphaCursor] = vertexOffset;
			vertexOffset += semanticSize[Semantic_AlphaCursor];
		}
		else //Pad it anyway to simplify shader bindings later
		{
			vertexOffset += semanticSize[Semantic_AlphaCursor];
		}
		if ((shaderVariationFlags & ShaderVariationFlags::Has_TransformUVs) != 0)
		{
			offsetTable[Semantic_TransformUVsRotate] = vertexOffset;
			vertexOffset += semanticSize[Semantic_TransformUVsRotate];
		}
	}
	else
	{
		offsetTable[Semantic_Uv0] = vertexOffset;
		vertexOffset += semanticSize[Semantic_Uv0];
		if ((shaderVariationFlags & ShaderVariationFlags::Has_AlphaRemap) != 0)
		{
			offsetTable[Semantic_AlphaCursor] = vertexOffset;
			vertexOffset += semanticSize[Semantic_AlphaCursor];
		}
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_Emissive) != 0)
	{
		if (isEmissive3)
		{
			offsetTable[Semantic_EmissiveColor3] = vertexOffset;
			vertexOffset += semanticSize[Semantic_EmissiveColor3] + sizeof(float);
		}
		else
		{
			offsetTable[Semantic_EmissiveColor4] = vertexOffset;
			vertexOffset += semanticSize[Semantic_EmissiveColor4];
		}
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_TransformUVs) != 0)
	{
		if ((shaderVariationFlags & ShaderVariationFlags::Has_CorrectDeformation) == 0 &&
			(shaderVariationFlags & ShaderVariationFlags::Has_AnimBlend) == 0)
		{
			offsetTable[Semantic_TransformUVsRotate] = vertexOffset;
			vertexOffset += semanticSize[Semantic_TransformUVsRotate];
			if ((shaderVariationFlags & ShaderVariationFlags::Has_Atlas) != 0)
			{
				offsetTable[Semantic_AtlasId] = vertexOffset;
				vertexOffset += semanticSize[Semantic_AtlasId];
			}
		}
		offsetTable[Semantic_TransformUVsScaleAndOffset] = vertexOffset;
		vertexOffset += semanticSize[Semantic_TransformUVsScaleAndOffset];
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_AlphaMasks) != 0)
	{
		offsetTable[Semantic_AlphaMasksCursors] = vertexOffset;
		vertexOffset += semanticSize[Semantic_AlphaMasksCursors];
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_UVDistortions) != 0)
	{
		offsetTable[Semantic_UVDistortionsCursors] = vertexOffset;
		vertexOffset += semanticSize[Semantic_UVDistortionsCursors];
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_Dissolve) != 0)
	{
		offsetTable[Semantic_DissolveCursor] = vertexOffset;
		vertexOffset += semanticSize[Semantic_DissolveCursor];
	}
	if ((shaderVariationFlags & ShaderVariationFlags::Has_Atlas) != 0 &&
		(shaderVariationFlags & (ShaderVariationFlags::Has_AlphaMasks | ShaderVariationFlags::Has_UVDistortions | ShaderVariationFlags::Has_Dissolve)) != 0)
	{
		offsetTable[Semantic_RawUv0] = vertexOffset;
		vertexOffset += semanticSize[Semantic_RawUv0];
	}

	return vertexOffset;
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
#if 0
	//To implement when vertex billboarding will be possible with shadergraph
	if ((shaderVariationFlags & ShaderVariationFlags::Has_TransformUVs) != 0)
	{
		vertexStride += 1 * sizeof(float) + 4 * sizeof(float); // rotate + 2 x float2 scale and offset
	}
#endif
	return vertexStride;
}

//-------------------------------------------------------------------------------------
