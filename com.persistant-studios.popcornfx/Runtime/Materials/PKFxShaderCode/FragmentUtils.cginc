//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

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
	Texture2D _AlphaMap;
	SamplerState linear_clamp_sampler;

	float4	SampleAlphaRemapTexture(float2 uv)
	{
		uv.y = 1 - uv.y;
		float4 color = _AlphaMap.Sample(linear_clamp_sampler, uv);
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
		
		#if	UNITY_UV_STARTS_AT_TOP
			depthTexcoord.y = 1 - depthTexcoord.y;
		#endif
		#if	USE_HDRP
			float depth = SampleCameraDepth(depthTexcoord).r;
		#elif USE_URP
			float depth = SAMPLE_TEXTURE2D_X(_CameraDepthTexture, _pointClampSampler, depthTexcoord).r;
		#else
			float depth = UNITY_SAMPLE_SCREENSPACE_TEXTURE(_CameraDepthTexture, depthTexcoord).r;
		#endif
		return depth;
	}
#endif

//------------------------------------------

#if	PK_HAS_RIBBON_COMPLEX
	int		_RotateUVs;
#endif