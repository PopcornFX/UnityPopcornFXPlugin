//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

Shader "PopcornFX/Particle"
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
		_InvSoftnessDistance("Inverse Softness Distance", Float) = 1
		_RotateUVs("Rotate UVs (only used in shader for correct deformation ribbons)", Int) = 0
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
			CGPROGRAM

			#pragma vertex vert
			#pragma fragment frag
			#pragma target 3.0

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
			#define CUTOUT_OPAQUE	0

			//------------------------------------------------------------------------------------
			// Particle shader
			//------------------------------------------------------------------------------------
			#include "PKFxShaderCode/ParticleShader.cginc"
			
			ENDCG
		}
	}




}
