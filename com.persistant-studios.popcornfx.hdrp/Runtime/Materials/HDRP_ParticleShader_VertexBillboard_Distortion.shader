//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
Shader "PopcornFX/HDRP/PKFxParticleProceduralShader_Distortion"
{
	Properties
	{
		_MainTex("Sprite Texture", 2D) = "white" {}
		_AlphaMap("Alpha Remap Texture", 2D) = "white" {}
		_DistortionScale("Distortion Scale", Float) = 35
		_DistortionVectorScale("Distortion Vector Scale", Float) = 2
		_DistortionVectorBias("Distortion Vector Bias", Float) = 1
		_DistortionBlurScale("Distortion Blur Scale", Float) = 1
		_UniformFlags("Uniform Flags", Int) = 0
		_InvSoftnessDistance("Inverse Softness Distance", Float) = 1
		[HideInInspector] _StencilRefDistortionVec("_StencilRefDistortionVec", Int) = 64 // StencilBitMask.DistortionVectors
	}
	SubShader
	{
		Tags
		{
			"Queue" = "Transparent+0"
			"IgnoreProjector" = "True"
			"RenderType" = "Transparent"
			"CanUseSpriteAtlas" = "True"
		}
		Pass
		{
			Name "DistortionVectors"
			Tags { "LightMode" = "DistortionVectors" }
		
			Stencil
			{
				WriteMask [_StencilRefDistortionVec]
				Ref [_StencilRefDistortionVec]
				Comp Always
				Pass Replace
			}
		
			Blend One One, One One
			BlendOp Add, Add
			ZTest LEqual
			ZWrite off
			Cull Off

			HLSLPROGRAM

			float _DistortionScale;
			float _DistortionVectorScale;
			float _DistortionVectorBias;
			float _DistortionBlurScale;

			#define PK_HAS_DISTORTION 1

			#pragma multi_compile_local _ PK_HAS_ALPHA_REMAP
			#pragma multi_compile_local _ PK_HAS_SOFT

			#pragma multi_compile_local BB_FeatureC0 BB_FeatureC1 BB_FeatureC1_Capsule BB_FeatureC2
			#pragma multi_compile_local _ BB_SizeFloat2
			#pragma multi_compile_local _ BB_Feature_Atlas PK_HAS_ANIM_BLEND

			#pragma vertex vert
			#pragma fragment frag


			#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/ShaderPass.cs.hlsl"
			#define SHADERPASS SHADERPASS_DISTORTION
			#define USE_HDRP 1

			#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/Common.hlsl"
			#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/UnityInstancing.hlsl"

			#include "Packages/com.persistant-studios.popcornfx/Runtime/Materials/PKFxShaderCode/VertexProceduralInput.cginc"
			#include "Packages/com.persistant-studios.popcornfx/Runtime/Materials/PKFxShaderCode/VertexOutput.cginc"

			#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/FragInputs.hlsl"
			#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VaryingMesh.hlsl"
			#include "Packages/com.unity.render-pipelines.high-definition/Runtime/ShaderLibrary/ShaderVariables.hlsl"
			#include "Packages/com.unity.render-pipelines.high-definition/Runtime/ShaderLibrary/ShaderVariablesFunctions.hlsl"

			#include "Packages/com.unity.render-pipelines.core/ShaderLibrary/SpaceTransforms.hlsl"

			#include "Packages/com.unity.render-pipelines.high-definition/Runtime/RenderPipeline/ShaderPass/VertMesh.hlsl"
			#include "Packages/com.unity.render-pipelines.high-definition/Runtime/Material/Material.hlsl"

			//------------------------------------------------------------------------------------
			// Vertex shader
			//------------------------------------------------------------------------------------
			#include "Packages/com.persistant-studios.popcornfx/Runtime/Materials/PKFxShaderCode/VertexProceduralShader.cginc"

			//------------------------------------------------------------------------------------
			// Fragment
			//------------------------------------------------------------------------------------
			#include "Packages/com.persistant-studios.popcornfx/Runtime/Materials/PKFxShaderCode/FragmentUtils.cginc"

			float4	frag(SVertexOutput i) : SV_Target
			{ 
				UNITY_SETUP_INSTANCE_ID(i);
				// Non OpenGlES implementation:
				float4	diffuse = float4(1.0, 1.0, 1.0, 1.0);

				#if	PK_HAS_ANIM_BLEND
					float4	diffuseA = SampleSpriteTexture(i.UV0);
					float4	diffuseB = SampleSpriteTexture(i.UV1);
					diffuse = lerp(diffuseA, diffuseB, i.FrameLerp);
				#elif !PK_HAS_ANIM_BLEND
					float2	uv = i.UV0;
					diffuse = SampleSpriteTexture(uv);
				#endif

				#if	PK_HAS_ALPHA_REMAP
					float2	alphaCoord = float2(diffuse.a, i.AlphaCursor);
					float	newAlpha = SampleAlphaRemapTexture(alphaCoord).x;
					diffuse = float4(diffuse.xyz, newAlpha);
				#endif

				float4 result = float4(1,1,1,1);

				#if CUTOUT_OPAQUE
					float cutout = diffuse.a - _CutOutAlpha;
					clip(cutout);
				#endif

				float3	transformedPosition = i.ProjPos.xyz / i.ProjPos.w;

				#if	CLIP_Z_BETWEEN_MINUS_ONE_AND_ONE
					transformedPosition.z = transformedPosition.z * 0.5f + 0.5f;
				#endif

				float2	screenUV = transformedPosition.xy * 0.5f + 0.5f;
				float	sceneDepth = LINEARIZE_DEPTH(PKSampleDepthTexture(screenUV));
				float	fragDepth = LINEARIZE_DEPTH(transformedPosition.z);
				float	depthfade = saturate(_InvSoftnessDistance * (sceneDepth - fragDepth));

				float3	baseDisto = diffuse.xyz * float3(_DistortionVectorScale, _DistortionVectorScale, 1.0) - float3(_DistortionVectorBias, _DistortionVectorBias, 0.0);

				#if	PK_HAS_SOFT
					baseDisto = float3(depthfade, depthfade, 1) * baseDisto / (fragDepth);
				#endif

				baseDisto.rg *= float2(_DistortionScale, _DistortionScale);
				baseDisto.b = clamp(baseDisto.b * _DistortionBlurScale, 0.0, 1.0);

				return float4(baseDisto.rg, 1.0f, baseDisto.b);
			}

			ENDHLSL
		}
	}
}
