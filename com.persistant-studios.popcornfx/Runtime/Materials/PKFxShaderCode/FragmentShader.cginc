//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#include "FragmentUtils.cginc"

float4	frag(SVertexOutput i) : SV_Target
{
	UNITY_SETUP_INSTANCE_ID(i);
	// Non OpenGlES implementation:
	float4	diffuse = float4(1.0, 1.0, 1.0, 1.0);
	float2	uv = i.UV0;

	#if	PK_HAS_LIGHTING
		// Do something with i.Normal...
	#endif

	#if	PK_HAS_RIBBON_COMPLEX
		if (i.UV0.x + i.UV0.y < 1.0f)
			uv = i.UV0.xy / i.UVFactors.xy;
		else
			uv = 1.0f - ((1.0f - i.UV0.xy) / i.UVFactors.zw);
	
		if (_RotateUVs != 0)
			uv = uv.yx;

		uv = uv * i.UVScaleAndOffset.xy + i.UVScaleAndOffset.zw;
	#elif	defined(PK_HAS_ANIM_BLEND)
		float2	uv1 = i.UV1;
	#endif

	#if defined(PK_HAS_TRANSFORM_UVS)
		
		float		sinR = sin(-i.TransformUVs_Rotate.x);
		float		cosR = cos(-i.TransformUVs_Rotate.x);
		float2x2	UVRotation = float2x2(cosR, sinR, -sinR, cosR);
		float2		UVScale = i.TransformUVs_ScaleAndOffset.xy;
		float2		UVOffset = i.TransformUVs_ScaleAndOffset.zw;
		float4		rect0 = float4(1.0f, 1.0f, 0.0f, 0.0f);
		#	if	defined(PK_HAS_ANIM_BLEND)
			float	idf = abs(i.FrameLerp);
			uint	maxAtlasIdx = _Atlas.Load(0) - 1;
			uint	idA = min(uint(floor(idf)), maxAtlasIdx);
			rect0 = asfloat(_Atlas.Load4((idA + 1) * 4 * 4));
			uint	idB = min(idA + 1U, maxAtlasIdx);
			float4	rect1 = asfloat(_Atlas.Load4((idB + 1) * 4 * 4));
			uv1 = ((uv1 - rect1.zw) / rect1.xy); // normalize (if atlas)

			uv1 = transformUV(uv1, UVScale, UVRotation, UVOffset); // scale then rotate then translate UV
			uv1 = frac(uv1) * rect1.xy + rect1.zw; // undo normalize

		#	endif
		uv = ((uv - rect0.zw) / rect0.xy); // normalize (if atlas)
		uv = transformUV(uv, UVScale, UVRotation, UVOffset); // scale then rotate then translate UV
		// For clean derivatives:
		float2	_uv = uv * rect0.xy + rect0.zw;
		float2	dUVdx = ddx(_uv);
		float2	dUVdy = ddy(_uv);
		uv = frac(uv) * rect0.xy + rect0.zw; // undo normalize
		bool	RGBOnly = _TransformUVs_RGBOnly != 0;
	#endif

	#if	PK_HAS_ANIM_BLEND
		#if defined(PK_HAS_TRANSFORM_UVS)
			float4	diffuseA = SampleSpriteGradTexture(uv, dUVdx, dUVdy);
			float4	diffuseB = SampleSpriteGradTexture(uv1, dUVdx, dUVdy);
			if (RGBOnly)
			{
				diffuseA.a =  SampleSpriteTexture(i.UV0).a;
				diffuseB.a =  SampleSpriteTexture(i.UV1).a;
			}
		#else
			float4	diffuseA = SampleSpriteTexture(uv);
			float4	diffuseB = SampleSpriteTexture(uv1);
		#endif
		diffuse = lerp(diffuseA, diffuseB, frac(i.FrameLerp));
	#else
		#if defined(PK_HAS_TRANSFORM_UVS)
			diffuse = SampleSpriteGradTexture(uv, dUVdx, dUVdy);
			if (RGBOnly)
			{
				diffuse.a =  SampleSpriteTexture(i.UV0).a;
			}
		#else
			diffuse = SampleSpriteTexture(uv);
		#endif
	#endif

	#if	PK_HAS_ALPHA_REMAP
		float2	alphaCoord = float2(diffuse.a, i.AlphaCursor);
		float	newAlpha = SampleAlphaRemapTexture(alphaCoord).x;
		diffuse = float4(diffuse.xyz, newAlpha);
	#endif
	
	float4 result = float4(1,1,1,1);

	#if CUTOUT_OPAQUE
		float cutout = (i.Color.a * diffuse.a) - _CutOutAlpha;
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
		float4	color = i.Color;
		#if USE_URP		//Forward
			float3	baseDisto = diffuse.xyz - float3(0.50196, 0.50196, 0.0);
			#if	PK_HAS_SOFT
				diffuse.rg = float2(depthfade, depthfade) * baseDisto.xy / (fragDepth);
			#else
				diffuse.rg = baseDisto.rg;
			#endif
			float2 distoScreenUV = screenUV + (diffuse.xy * _DistortionFactor);
			#if	UNITY_UV_STARTS_AT_TOP
				distoScreenUV.y = 1 - distoScreenUV.y;
			#endif
			distoScreenUV = UnityStereoTransformScreenSpaceTex(distoScreenUV);
			diffuse.rgb = SAMPLE_TEXTURE2D_X(_CameraOpaqueTexture, sampler_CameraOpaqueTexture, distoScreenUV).rgb;
		#else			// Deferred

			float2 distoScreenUV = screenUV + diffuse.xy;
			float3	baseDisto = color.xyz * (diffuse.xyz * float3(2.0, 2.0, 1.0) - float3(0.50196, 0.50196, 0.0));

			#if	PK_HAS_SOFT
				baseDisto = float3(depthfade, depthfade, 1) * baseDisto / (fragDepth);
			#endif
			diffuse = float4(baseDisto, 1);
		#endif

		float	distDepth = LINEARIZE_DEPTH(PKSampleDepthTexture(distoScreenUV));
		if (fragDepth > distDepth)
			discard;
		result.rgb = diffuse.rgb;

	#elif	PK_HAS_COLOR
		result = diffuse * i.Color;
	#endif
	
	#if PK_HAS_DIFFUSE_RAMP
		result.rgb = SampleDiffuseRampTexture(float2(result.x,0.0)).rgb;
	#endif


	#if	!PK_HAS_EMISSIVE_NONE
		#if defined(PK_HAS_TRANSFORM_UVS)
			float3	emissiveColor1 = SampleGradEmissiveTexture(uv, dUVdx, dUVdy);
		#else
			float3	emissiveColor1 = SampleEmissiveTexture(uv).rgb;
		#endif

		#if PK_HAS_ANIM_BLEND
			#if PK_HAS_TRANSFORM_UVS
				float3	emissiveColor2 = SampleGradEmissiveTexture(uv1, dUVdx, dUVdy);
			#else
				float3	emissiveColor2 = SampleEmissiveTexture(uv1).rgb;
			#endif
			emissiveColor1 = lerp(emissiveColor1, emissiveColor2, i.FrameLerp);
		#endif
		#if	PK_HAS_EMISSIVE_WITH_RAMP
			emissiveColor1 = SampleEmissiveRampTexture(float2(emissiveColor1.x,0.0)).rgb;
		#endif
		emissiveColor1 *= i.EmissiveColor;

		result.rgb += emissiveColor1.rgb;
	#endif
	
#if CUTOUT_OPAQUE
		return float4((result * depthfade).rgb, 1);
#else
		return result * depthfade;
#endif
}
