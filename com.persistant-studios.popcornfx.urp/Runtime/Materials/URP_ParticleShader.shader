//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

Shader "PopcornFX/URP/ParticleShader"
{
	Properties
	{
		 _MainTex("Sprite Texture", 2D) = "white" {}
		_AlphaMap("Alpha Remap Texture", 2D) = "white" {}
		_DiffuseRampMap("Diffuse Ramp Texture", 2D) = "white" {}
		_EmissiveMap("Emissive Texture", 2D) = "white" {}
		_EmissiveRampMap("Emissive Ramp Texture", 2D) = "white" {}
		_SrcBlendMode("Src Blend Mode", Int) = 0
		_DstBlendMode("Dst Blend Mode", Int) = 0
		_UniformFlags("Uniform Flags", Int) = 0
		_InvSoftnessDistance("Inverse Softness Distance", Float) = 1
		_RotateUVs("Rotate UVs (only used in shader for correct deformation ribbons)", Int) = 0
		_TransformUVs_RGBOnly("TransformUVs.RGBOnly (only used if transform UVs render feature is activated)", Int) = 0
		_DistortionFactor("Distortion factor [ 0 ; 1 ]", Range(0, 1)) = 0.2
	}

	SubShader
	{
		Tags
		{
			"Queue" = "Transparent"
			"IgnoreProjector" = "True"
			"RenderType" = "Transparent"
			"CanUseSpriteAtlas" = "True"
		}
				
		Cull Off
		Lighting Off
		ZWrite Off
		ZTest LEqual
		Blend [_SrcBlendMode] [_DstBlendMode]

		Pass
		{
			HLSLPROGRAM

			#pragma vertex vert
			#pragma fragment frag
			#pragma target 3.0

			//------------------------------------------------------------------------------------
			// Material variations:
			//------------------------------------------------------------------------------------
			#define PK_HAS_COLOR 1
			#pragma shader_feature PK_HAS_EMISSIVE_NONE PK_HAS_EMISSIVE_BASIC PK_HAS_EMISSIVE_WITH_RAMP
			#pragma shader_feature _ PK_HAS_ALPHA_REMAP
			#pragma shader_feature _ PK_HAS_TRANSFORM_UVS
			#pragma shader_feature _ PK_HAS_DIFFUSE_RAMP
			#pragma shader_feature _ PK_HAS_SOFT
			#pragma shader_feature _ PK_HAS_LIGHTING PK_HAS_DISTORTION
			#pragma shader_feature _ PK_HAS_ATLAS
			#pragma shader_feature _ PK_HAS_ANIM_BLEND PK_HAS_RIBBON_COMPLEX
			//------------------------------------------------------------------------------------
			// Unity defined keywords
			#pragma multi_compile_instancing

			#define	USE_URP		1

			//------------------------------------------------------------------------------------
			// Particle shader
			//------------------------------------------------------------------------------------
			#include "Packages/com.persistant-studios.popcornfx/Runtime/Materials/PKFxShaderCode/ParticleShader.cginc"
			
			ENDHLSL

		}
	}
}
