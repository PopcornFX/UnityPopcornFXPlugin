//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#include "FragmentUtils.inc"

float4	frag(SVertexOutput i) : SV_Target
{
	UNITY_SETUP_INSTANCE_ID(i);
	// Non OpenGlES implementation:
	float4	diffuse = float4(1.0, 1.0, 1.0, 1.0);

	#if	PK_HAS_LIGHTING
		// Do something with i.Normal...
	#endif

	#if	PK_HAS_RIBBON_COMPLEX
		float2	uv;

		if (i.UV0.x + i.UV0.y < 1.0f)
			uv = i.UV0.xy / i.UVFactors.xy;
		else
			uv = 1.0f - ((1.0f - i.UV0.xy) / i.UVFactors.zw);
	
		if (_RotateUVs != 0)
			uv = uv.yx;

		uv = uv * i.UVScaleAndOffset.xy + i.UVScaleAndOffset.zw;

	#elif	!PK_HAS_ANIM_BLEND
		float2	uv = i.UV0;
	#endif

	#if	PK_HAS_ANIM_BLEND
		float4	diffuseA = SampleSpriteTexture(i.UV0);
		float4	diffuseB = SampleSpriteTexture(i.UV1);
		diffuse = lerp(diffuseA, diffuseB, i.FrameLerp);
	#else
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

	#if	(PK_HAS_DISTORTION || PK_HAS_SOFT)
		float3	transformedPosition = i.ProjPos.xyz / i.ProjPos.w;

		#if	CLIP_Z_BETWEEN_MINUS_ONE_AND_ONE
			transformedPosition.z = transformedPosition.z * 0.5f + 0.5f;
		#endif

		float2	screenUV = transformedPosition.xy * 0.5f + 0.5f;
		float	sceneDepth = LINEARIZE_DEPTH(PKSampleDepthTexture(screenUV));
		float	fragDepth = LINEARIZE_DEPTH(transformedPosition.z);
		float	depthfade = saturate( _InvSoftnessDistance * (sceneDepth - fragDepth));
	#else
		float	depthfade = 1.f;
	#endif
	

	#if	PK_HAS_DISTORTION
		#if	PK_HAS_COLOR
			float4	color = i.Color;
		#else
			float4	color = float4(1, 1, 1, 1);
		#endif

		float3	baseDisto = color.xyz * (diffuse.xyz * float3(2.0, 2.0, 1.0) - float3(0.50196, 0.50196, 0.0));

		#if	PK_HAS_SOFT
			baseDisto = float3(depthfade, depthfade, 1) * baseDisto / (fragDepth);
		#endif
		diffuse = float4(baseDisto, 1);
		float	distDepth = LINEARIZE_DEPTH(PKSampleDepthTexture(screenUV + diffuse.xy ));
		if (fragDepth > distDepth)
			discard;
		result = diffuse;

	#elif	PK_HAS_COLOR
		result = diffuse * i.Color;
	#endif
	
	#if PK_HAS_DIFFUSE_RAMP
		result.rgb = SampleDiffuseRampTexture(float2(result.x,0.0)).rgb;
	#endif

	#if CUTOUT_OPAQUE
		return float4((result * depthfade).rgb, 1);
	#else
		return result * depthfade;
	#endif
}
