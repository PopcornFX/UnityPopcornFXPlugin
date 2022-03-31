//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

Shader "PopcornFX/HDRP/PKFxParticleShader"
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
//		Only for debug:
//		_MaterialFlags("Material Flags", Int) = 0
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
		ZTest [_ZTestMode]
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
			#pragma multi_compile_local _ PK_HAS_ALPHA_REMAP
			#pragma multi_compile_local _ PK_HAS_DIFFUSE_RAMP
			#pragma multi_compile_local _ PK_HAS_SOFT
			#pragma multi_compile_local _ PK_HAS_LIGHTING PK_HAS_DISTORTION
			// Bug on opengles: SV_VertexID not working so need to remove the PK_HAS_RIBBON_COMPLEX variation...
			#pragma multi_compile_local _ PK_HAS_ANIM_BLEND PK_HAS_RIBBON_COMPLEX
			//------------------------------------------------------------------------------------

			#define	USE_HDRP	1

			//------------------------------------------------------------------------------------
			// Particle shader
			//------------------------------------------------------------------------------------
			#include "Packages/com.persistant-studios.popcornfx/Runtime/Materials/PKFxShaderCode/ParticleShader.inc"
			
			//ENDCG
			ENDHLSL

		}
	}
}
