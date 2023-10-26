//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

Shader "PopcornFX/Particle_CutOutOpaque"
{
	Properties
	{
		_MainTex("Sprite Texture", 2D) = "white" {}
		_AlphaMap("Alpha Remap Texture", 2D) = "white" {}
		_DiffuseRampMap("Diffuse Ramp Texture", 2D) = "white" {}
		_SrcBlendMode("Src Blend Mode", Int) = 0
		_DstBlendMode("Dst Blend Mode", Int) = 0
		_UniformFlags("Uniform Flags", Int) = 0
		_InvSoftnessDistance("Inverse Softness Distance", Float) = 1
		_RotateUVs("Rotate UVs (only used in shader for correct deformation ribbons)", Int) = 0
		_CutOutAlpha("Cutout Alpha", Float) = 0.5
	}

	SubShader
	{
		Tags
		{
			"DisableBatching" = "True"
			"Queue" = "AlphaTest"
			"IgnoreProjector" = "True"
			"RenderType" = "TransparentCutout"
			"CanUseSpriteAtlas" = "True"
		}

		Cull Off
		Lighting Off
		ZWrite On
		Blend Off
		ZTest LEqual

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
			#pragma shader_feature PK_HAS_EMISSIVE_NONE PK_HAS_EMISSIVE_BASIC PK_HAS_EMISSIVE_WITH_RAMP
			#pragma shader_feature _ PK_HAS_ALPHA_REMAP
			#pragma shader_feature _ PK_HAS_DIFFUSE_RAMP
			#pragma shader_feature _ PK_HAS_SOFT
			#pragma shader_feature _ PK_HAS_LIGHTING PK_HAS_DISTORTION
			// Bug on opengles: SV_VertexID not working so need to remove the PK_HAS_RIBBON_COMPLEX variation...
			#pragma shader_feature _ PK_HAS_ANIM_BLEND PK_HAS_RIBBON_COMPLEX
			//------------------------------------------------------------------------------------
			
			#define	USE_HDRP		0
			#define	USE_URP			0
			#define CUTOUT_OPAQUE	1

			//------------------------------------------------------------------------------------
			// Particle shader
			//------------------------------------------------------------------------------------
			#include "PKFxShaderCode/ParticleShader.cginc"

			ENDHLSL
		}
	}
}
