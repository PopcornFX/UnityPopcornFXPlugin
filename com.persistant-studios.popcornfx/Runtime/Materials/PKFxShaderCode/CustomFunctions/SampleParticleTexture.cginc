
void 	SampleTextureImplem(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float4 outValue, bool flipV)
{
	outValue = SAMPLE_TEXTURE2D(toSample.tex, toSample.samplerstate, float2(uv0.x, flipV ? 1.0f - uv0.y : uv0.y));
# if	PK_HAS_ANIM_BLEND_ON
	float4 	sampleUv1 = SAMPLE_TEXTURE2D(toSample.tex, toSample.samplerstate, float2(uv1.x, flipV ? 1.0f - uv1.y : uv1.y));
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

void 	SampleParticleTexture_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float4 outValue)
{
	SampleTextureImplem(toSample, uv0, uv1, frameLerp, outValue, true);
}

void	SampleParticleNormalMap_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float3 outValue)
{
	SampleNormalMapImplem(toSample, uv0, uv1, frameLerp, outValue, true);
}

void 	SampleParticleMeshTexture_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float4 outValue)
{
	SampleTextureImplem(toSample, uv0, uv1, frameLerp, outValue, false);
}

void	SampleParticleMeshNormalMap_float(in UnityTexture2D toSample, in float2 uv0, in float2 uv1, in float frameLerp, out float3 outValue)
{
	SampleNormalMapImplem(toSample, uv0, uv1, frameLerp, outValue, false);
}
