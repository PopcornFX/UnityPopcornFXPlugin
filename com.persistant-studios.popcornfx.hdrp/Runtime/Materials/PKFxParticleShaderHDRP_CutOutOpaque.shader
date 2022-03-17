//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

Shader "PopcornFX/HDRP/PKFxParticleShader_CutOutOpaque"
{
	Properties
	{
		_MainTex("Sprite Texture", 2D) = "white" {}
		_AlphaMap("Alpha Remap Texture", 2D) = "white" {}
		_DiffuseRampMap("Diffuse Ramp Texture", 2D) = "white" {}
		_SrcBlendMode("Src Blend Mode", Int) = 0
		_DstBlendMode("Dst Blend Mode", Int) = 0
		_ZTestMode("ZTest Mode", Int) = 0
		_UniformFlags("Uniform Flags", Int) = 0
		_InvSoftnessDistance("Inverse Softness Distance", Float) = 1
		_RotateUVs("Rotate UVs (only used in shader for correct deformation ribbons)", Int) = 0
		_CutOutAlpha("Cutout Alpha", Float) = 0.5
	}

	SubShader
	{
		Tags
		{
			"Queue" = "AlphaTest"
			"IgnoreProjector" = "True"
			"RenderType" = "TransparentCutout"
			"CanUseSpriteAtlas" = "True"
		}

		Cull Off
		Lighting Off
		ZWrite On
		Blend Off
		ZTest [_ZTestMode]

		Pass
		{
			HLSLPROGRAM

			#pragma vertex vert
			#pragma fragment frag
			#pragma target 4.5

			//------------------------------------------------------------------------------------
			// Material variations:
			//------------------------------------------------------------------------------------
			#define PK_HAS_COLOR 1
			#pragma multi_compile_local _ PK_HAS_ALPHA_REMAP
			#pragma multi_compile_local _ PK_HAS_DIFFUSE_RAMP
			//------------------------------------------------------------------------------------
	
			//------------------------------------------------------------------------------------
			// Billboarding variations:
			//------------------------------------------------------------------------------------
			#pragma multi_compile_local BB_FeatureC0 BB_FeatureC1 BB_FeatureC1_Capsule BB_FeatureC2
			#pragma multi_compile_local _ BB_SizeFloat2
			#pragma multi_compile_local _ BB_Feature_Atlas PK_HAS_ANIM_BLEND
			//------------------------------------------------------------------------------------
			
			#define	USE_HDRP		1
			#define	USE_URP			0
			#define CUTOUT_OPAQUE	1

			//------------------------------------------------------------------------------------
			// Particle shader
			//------------------------------------------------------------------------------------
			#include "Packages/com.persistant-studios.popcornfx/Runtime/Materials/PKFxShaderCode/ParticleShader.inc"

			ENDHLSL
		}
	}
}
