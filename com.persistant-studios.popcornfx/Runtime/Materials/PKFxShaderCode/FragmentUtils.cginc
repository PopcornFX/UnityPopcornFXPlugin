//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#if	SHADER_API_D3D11 || SHADER_API_D3D9 || SHADER_API_XBOXONE || SHADER_API_METAL || SHADER_API_PSSL || SHADER_API_SWITCH
	#define	CLIP_Y_REVERSED		1
#else
	#define	CLIP_Y_REVERSED		0
#endif

#if	SHADER_API_D3D11 || SHADER_API_D3D9 || SHADER_API_XBOXONE || SHADER_API_METAL || SHADER_API_PSSL || SHADER_API_SWITCH
	#define	CLIP_Z_BETWEEN_MINUS_ONE_AND_ONE		0
#else
	#define	CLIP_Z_BETWEEN_MINUS_ONE_AND_ONE		1
#endif

//------------------------------------------
// Diffuse texture sampling
//------------------------------------------
sampler2D	_MainTex;

float4	SampleSpriteTexture(float2 uv)
{
	uv.y = 1 - uv.y;
	float4 color = tex2D(_MainTex, uv);
	return color;
}

//------------------------------------------


//------------------------------------------
// Alpha remap sampling
//------------------------------------------
#if	PK_HAS_ALPHA_REMAP
	sampler2D	_AlphaMap;

	float4	SampleAlphaRemapTexture(float2 uv)
	{
		uv.y = 1 - uv.y;
		float4 color = tex2D(_AlphaMap, uv);
		return color;
	}
#endif

//------------------------------------------
// Emissive texture sampling
//------------------------------------------
#if	!PK_HAS_EMISSIVE_NONE
	sampler2D	_EmissiveMap;

	float3	SampleEmissiveTexture(float2 uv)
	{
		uv.y = 1 - uv.y;
		float3 color = tex2D(_EmissiveMap, uv).rgb;
		return color;
	}
#endif

//------------------------------------------

//------------------------------------------
// Diffuse Ramp
//------------------------------------------
#if	PK_HAS_DIFFUSE_RAMP
	sampler2D	_DiffuseRampMap;

	float4	SampleDiffuseRampTexture(float2 uv)
	{
		uv.y = 1 - uv.y;
		float4 color = tex2D(_DiffuseRampMap, uv);
		return color;
	}
#endif
//------------------------------------------

//------------------------------------------
// Emissive Ramp
//------------------------------------------
#if	PK_HAS_EMISSIVE_WITH_RAMP
	sampler2D	_EmissiveRampMap;

	float3	SampleEmissiveRampTexture(float2 uv)
	{
		uv.y = 1 - uv.y;
		float3 color = tex2D(_EmissiveRampMap, uv).rgb;
		return color;
	}
#endif
//------------------------------------------

#if CUTOUT_OPAQUE
	float	_CutOutAlpha;
#endif

//------------------------------------------
// Depth texture sampling
//------------------------------------------
#if	(PK_HAS_DISTORTION || PK_HAS_SOFT)
	float	_InvSoftnessDistance;

	#if	USE_HDRP
		#define	LINEARIZE_DEPTH(x)		LinearEyeDepth(x, _ZBufferParams)
	#elif USE_URP
		TEXTURE2D_X(_CameraDepthTexture);
		SamplerState _pointClampSampler;
		#define	LINEARIZE_DEPTH(x)		LinearEyeDepth(x, _ZBufferParams)
	#else
		UNITY_DECLARE_SCREENSPACE_TEXTURE(_CameraDepthTexture);
		#define	LINEARIZE_DEPTH(x)		LinearEyeDepth(x)
	#endif

	float	PKSampleDepthTexture(float2 uv)
	{
		float2		depthTexcoord;

		#if	UNITY_SINGLE_PASS_STEREO
			depthTexcoord = TransformStereoScreenSpaceTex(uv, 1.0);
		#else
			depthTexcoord = uv;
		#endif
		
		#if	CLIP_Y_REVERSED && !USE_URP
			depthTexcoord.y = 1 - depthTexcoord.y;
		#endif
		#if	USE_HDRP
			float depth = SampleCameraDepth(depthTexcoord).r;
		#elif USE_URP
			float depth = SAMPLE_TEXTURE2D_X(_CameraDepthTexture, _pointClampSampler, depthTexcoord).r;
		#else
			float depth = UNITY_SAMPLE_SCREENSPACE_TEXTURE(_CameraDepthTexture, depthTexcoord).r;
		#endif
		#if !UNITY_REVERSED_Z
			depth = 1.0 - depth;
		#endif
		return depth;
	}
#endif

//------------------------------------------

#if	PK_HAS_RIBBON_COMPLEX
	int		_RotateUVs;
#endif