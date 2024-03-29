//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

Shader "PopcornFX/Mesh_LitTransparent"
{
	Properties
	{
		_Color ("Color", Color) = (1,1,1,1)
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
		_EmissiveMap("Emissive (RGB)", 2D) = "white" {}
		_EmissiveRampMap("Emissive (RGB)", 2D) = "white" {}
		_AlphaMap("Alpha remap (RGB)", 2D) = "white" {}
		_DiffuseRampMap("Diffuse ramp (RGB)", 2D) = "white" {}
		_InvSoftnessDistance("Inverse softness distance (Float)", Float) = 1
		_VATPositionMap("VAT Position map (RGB)", 2D) = "white" {}
		_VATColorMap("VAT Color map (RGB)", 2D) = "white" {}
		_VATRotationMap("VAT Rotation map (RGB)", 2D) = "white" {}
		_VATNormalMap("VAT Normal map (RGB)", 2D) = "white" {}
		_VATNumFrames("VAT Numframes", Float) = 1
		_VATPackedData("VAT PackedData", Int) = 0
		_VATNormalizedData("VAT NormalizedData", Int) = 0
		_VATPadToPowerOf2("VAT PadToPowerOf2", Int) = 0
		_VATBounds("VAT Bounds", Vector) = (1,1,1,1)
		_VATPaddedRatio("VAT PaddedRatio", Vector) = (1,1,1,1)
		_VATPivot("VAT Pivot", Vector) = (1,1,1,1)
		[Enum(UnityEngine.Rendering.CullMode)] _Cull("Cull", Int) = 0
		_Roughness("Roughness (Float)", Range(0, 1)) = 0.0
		_Metalness("Metalness (Float)", Range(0, 1)) = 0.0
		_NormalMap("Normal map (RGB)", 2D) = "bump" {}
	}

	SubShader
	{
		Tags
		{
			"Queue" = "Transparent"
			"RenderType" = "Transparent"
		}
		LOD 200

		Cull [_Cull]
		Lighting Off
		ZWrite Off
		ZTest LEqual
		Blend One One
	
		CGPROGRAM
		#pragma surface surf Standard vertex:vertMain

		#pragma shader_feature PK_HAS_EMISSIVE_NONE PK_HAS_EMISSIVE_BASIC PK_HAS_EMISSIVE_WITH_RAMP
		#pragma shader_feature PK_HAS_VAT_NONE PK_HAS_VAT_SOFT PK_HAS_VAT_RIGID PK_HAS_VAT_FLUID
		#pragma shader_feature _ PK_HAS_ALPHA_REMAP
		#pragma shader_feature _ PK_HAS_DIFFUSE_RAMP
		#pragma shader_feature _ PK_HAS_SOFT

		#define PK_IS_TRANSPARENT	1
		#define PK_IS_LIT			1
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
