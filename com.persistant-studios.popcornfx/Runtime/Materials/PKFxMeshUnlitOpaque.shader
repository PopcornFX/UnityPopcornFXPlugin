//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

Shader "PopcornFX/PKFxMeshUnlitOpaque"
{
	Properties
	{
		_Color ("Color", Color) = (1,1,1,1)
		_MainTex("Albedo (RGB)", 2D) = "white" {}
		_EmissiveMap("Emissive (RGB)", 2D) = "white" {}
		_EmissiveRampMap("Emissive ramp (RGB)", 2D) = "white" {}
		_AlphaMap("Alpha remap (RGB)", 2D) = "white" {}
		_DiffuseRampMap("Diffuse ramp (RGB)", 2D) = "white" {}
		_CutOutAlpha("Cutout Alpha", Range(0, 1)) = 0.0
		_VATPositionMap("VAT Position map (RGB)", 2D) = "white" {}
		_VATColorMap("VAT Color map (RGB)", 2D) = "white" {}
		_VATRotationMap("VAT Rotation map (RGB)", 2D) = "white" {}
		_VATNormalMap("VAT Normal map (RGB)", 2D) = "white" {}
	}

	SubShader
	{
		Tags
		{
			"Queue" = "Geometry"
			"RenderType" = "Opaque"
		}
		LOD 200
	
		CGPROGRAM
		#pragma surface surf NoLighting noambient vertex:vertMain

		#pragma multi_compile PK_HAS_EMISSIVE_NONE PK_HAS_EMISSIVE_BASIC PK_HAS_EMISSIVE_WITH_RAMP
		#pragma multi_compile PK_HAS_VAT_NONE PK_HAS_VAT_SOFT PK_HAS_VAT_RIGID PK_HAS_VAT_FLUID
		#pragma multi_compile _ PK_HAS_ALPHA_REMAP
		#pragma multi_compile _ PK_HAS_DIFFUSE_RAMP

		#define PK_IS_TRANSPARENT	0
		#define PK_IS_LIT			0
		#define PK_UNITY_SHADER_LAB	1

		// Use shader model 3.0 target, to get nicer looking lighting
		#pragma target 3.5

		// Enable instancing for this shader
		#pragma multi_compile_instancing

		#include "PKFxShaderCode/MeshInstanceSurface.cginc"

		ENDCG
	}
	FallBack "Diffuse"
}
