//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

Shader "PopcornFX/Particle_VertexBillboarding"
{
	Properties
	{
		_MainTex("Sprite Texture", 2D) = "white" {}
		_AlphaMap("Alpha Remap Texture", 2D) = "white" {}
		_DiffuseRampMap("Diffuse Ramp Texture", 2D) = "white" {}
		_SrcBlendMode("Src Blend Mode", Int) = 0
		_DstBlendMode("Dst Blend Mode", Int) = 0
		_InvSoftnessDistance("Inverse Softness Distance", Float) = 1.0
		_RotateUVs("Rotate UVs (only used in shader for correct deformation ribbons)", Int) = 0
	}

	SubShader
	{
		Tags
		{
			"DisableBatching" = "True"
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
			#pragma target 4.5

			//------------------------------------------------------------------------------------
			// Material variations:
			//------------------------------------------------------------------------------------
			#define PK_HAS_COLOR 1
			#pragma shader_feature _ PK_HAS_ALPHA_REMAP
			#pragma shader_feature _ PK_HAS_DIFFUSE_RAMP
			#pragma shader_feature _ PK_HAS_SOFT
			#pragma shader_feature _ PK_HAS_LIGHTING PK_HAS_DISTORTION
			//------------------------------------------------------------------------------------
	
			//------------------------------------------------------------------------------------
			// Billboarding variations:
			//------------------------------------------------------------------------------------
			#pragma shader_feature BB_FeatureC0 BB_FeatureC1 BB_FeatureC1_Capsule BB_FeatureC2
			#pragma shader_feature _ BB_SizeFloat2
			#pragma shader_feature _ BB_Feature_Atlas PK_HAS_ANIM_BLEND
			//------------------------------------------------------------------------------------

			#define	USE_HDRP	0
			#define	USE_URP		0

			//------------------------------------------------------------------------------------
			// Particle shader
			//------------------------------------------------------------------------------------
			#include "PKFxShaderCode/ParticleProceduralShader.cginc"
			
			ENDCG
		}
	}




}
