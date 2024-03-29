
void 	SampleTextureImplem(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float4 outValue, bool flipV)
{
	outValue = SAMPLE_TEXTURE2D(toSample.tex, toSample.samplerstate, float2(uv0.x, flipV ? 1.0f - uv0.y : uv0.y));
# if	PK_HAS_ANIM_BLEND_ON
	float4 	sampleUv1 = SAMPLE_TEXTURE2D(toSample.tex, toSample.samplerstate, float2(uv1.x, flipV ? 1.0f - uv1.y : uv1.y));
	outValue = lerp(outValue, sampleUv1, frameLerp);
# endif
}

void 	SampleTextureTransformedImplem(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float2 dUVdx, in float2 dUVdy, in float frameLerp, out float4 outValue, bool flipV)
{
	outValue = SAMPLE_TEXTURE2D_GRAD(toSample.tex, toSample.samplerstate, float2(uv0.x, flipV ? 1.0f - uv0.y : uv0.y), dUVdx, dUVdy);
# if	PK_HAS_ANIM_BLEND_ON
	float4 	sampleUv1 = SAMPLE_TEXTURE2D_GRAD(toSample.tex, toSample.samplerstate, float2(uv1.x, flipV ? 1.0f - uv1.y : uv1.y), dUVdx, dUVdy);
	outValue = lerp(outValue, sampleUv1, frameLerp);
# endif
}

void	SampleNormalMapImplem(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float3 outValue, bool flipV)
{
	float4	nMapvalue = SAMPLE_TEXTURE2D(toSample.tex, toSample.samplerstate, float2(uv0.x, flipV ? 1.0f - uv0.y : uv0.y));
# if	PK_HAS_ANIM_BLEND_ON
	float4 	nMapvalue1 = SAMPLE_TEXTURE2D(toSample.tex, toSample.samplerstate, float2(uv1.x, flipV ? 1.0f - uv1.y : uv1.y));
	nMapvalue = lerp(nMapvalue, nMapvalue1, frameLerp);
# endif
	outValue = UnpackNormal(nMapvalue);
	outValue.y *= -1.0f;
}

void 	SampleNormalMapTransformedImplem(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float2 dUVdx, in float2 dUVdy, in float frameLerp, out float3 outValue, bool flipV)
{
	float4	nMapvalue = SAMPLE_TEXTURE2D_GRAD(toSample.tex, toSample.samplerstate, float2(uv0.x, flipV ? 1.0f - uv0.y : uv0.y), dUVdx, dUVdy);
# if	PK_HAS_ANIM_BLEND_ON
	float4 	nMapvalue1 = SAMPLE_TEXTURE2D_GRAD(toSample.tex, toSample.samplerstate, float2(uv1.x, flipV ? 1.0f - uv1.y : uv1.y), dUVdx, dUVdy);
	nMapvalue = lerp(nMapvalue, nMapvalue1, frameLerp);
# endif
	outValue = UnpackNormal(nMapvalue);
	outValue.y *= -1.0f;
}

void 	SampleParticleTexture_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float4 outValue)
{
	SampleTextureImplem(toSample, uv0, uv1, frameLerp, outValue, true);
}

void 	SampleParticleTextureTransformed_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float2 dUVdx, in float2 dUVdy, in float frameLerp, in bool isRGBOnly, in float2 oldUV0, in float2 oldUV1,
	out float4 outValue)
{
#if PK_HAS_TRANSFORM_UVS_ON
	SampleTextureTransformedImplem(toSample, uv0, uv1, dUVdx, dUVdy, frameLerp, outValue, true);
	if (isRGBOnly)
	{
		float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
		SampleTextureImplem(toSample, oldUV0, oldUV1, frameLerp, color, true);
		outValue.a = color.a;
	}
#else
	SampleTextureImplem(toSample, uv0, uv1, frameLerp, outValue, true);
#endif
}

void 	SampleParticleTextureTransformed_Mesh_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float2 dUVdx, in float2 dUVdy, in float frameLerp, in bool isRGBOnly, in float2 oldUV0, in float2 oldUV1,
	out float4 outValue)
{
#if PK_HAS_TRANSFORM_UVS_ON
	SampleTextureTransformedImplem(toSample, uv0, uv1, dUVdx, dUVdy, frameLerp, outValue, false);
	if (isRGBOnly)
	{
		float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
		SampleTextureImplem(toSample, oldUV0, oldUV1, frameLerp, color, false);
		outValue.a = color.a;
	}
#else
	SampleTextureImplem(toSample, uv0, uv1, frameLerp, outValue, false);
#endif
}

void 	SampleParticleMeshTexture_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float4 outValue)
{
	SampleTextureImplem(toSample, uv0, uv1, frameLerp, outValue, false);
}

void 	SampleParticleNormalMapTransformed_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float2 dUVdx, in float2 dUVdy, in float frameLerp, in bool isRGBOnly, in float2 oldUV0, in float2 oldUV1,
	out float3 outValue)
{
#if PK_HAS_TRANSFORM_UVS_ON
	SampleNormalMapTransformedImplem(toSample, uv0, uv1, dUVdx, dUVdy, frameLerp, outValue, true);
#else
	SampleNormalMapImplem(toSample, uv0, uv1, frameLerp, outValue, true);
#endif
}

void 	SampleParticleNormalMapTransformed_Mesh_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float2 dUVdx, in float2 dUVdy, in float frameLerp, in bool isRGBOnly, in float2 oldUV0, in float2 oldUV1,
	out float3 outValue)
{
#if PK_HAS_TRANSFORM_UVS_ON
	SampleNormalMapTransformedImplem(toSample, uv0, uv1, dUVdx, dUVdy, frameLerp, outValue, false);
#else
	SampleNormalMapImplem(toSample, uv0, uv1, frameLerp, outValue, false);
#endif
}

void	SampleParticleNormalMap_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float3 outValue)
{
	SampleNormalMapImplem(toSample, uv0, uv1, frameLerp, outValue, true);
}

void	SampleParticleMeshNormalMap_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float3 outValue)
{
	SampleNormalMapImplem(toSample, uv0, uv1, frameLerp, outValue, false);
}