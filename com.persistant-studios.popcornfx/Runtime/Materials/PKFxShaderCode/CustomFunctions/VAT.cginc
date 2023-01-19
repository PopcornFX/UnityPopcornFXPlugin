
#define	PK_GLUE(_a, _b)		_a ## _b

#if		!defined(PK_UNITY_SHADER_LAB)
#	define	PK_SAMPLE_TEXTURE2D_LOD(_TexName, _UVs, _LOD) SAMPLE_TEXTURE2D_LOD(UnityBuildTexture2DStructNoScale(_TexName).tex,  UnityBuildSamplerStateStruct(PK_GLUE(sampler, _TexName)).samplerstate, _UVs, 0.);
#else
	sampler2D	_VATPositionMap;
	sampler2D	_VATColorMap;
	sampler2D	_VATRotationMap;
	sampler2D	_VATNormalMap;

	float		_VATNumFrames;
	bool		_VATPackedData;
	bool		_VATNormalizedData;
	bool		_VATPadToPowerOf2;
	float2		_VATBounds;
	float4		_VATPaddedRatio;
	float2		_VATPivot;

#	define	PK_SAMPLE_TEXTURE2D_LOD(_TexName, _UVs, _LOD) tex2Dlod(_TexName,  float4(_UVs, 0, _LOD));
#endif


float3 RotateVector(float3 v, float4 q)
{
	return v + 2.0 * cross(cross(v, q.xyz) + q.w * v, q.xyz);
}

// Coordinate system conversion (right hand Z-up -> left hand Y-up)
float3 ConvertSpace(float3 v)
{
	return v.xzy * float3(-1.f, 1.f, 1.f);
}

float3 UnpackAlpha(float a)
{
	float a_hi = floor(a * 32.f);
	float a_lo = a * 32.f * 32.f - a_hi * 32.f;

	float2 n2 = float2(a_hi, a_lo) / 31.5f * 4.f - 2.f;
	float n2_n2 = dot(n2, n2);
	float3 n3 = float3(sqrt(1 - n2_n2 / 4) * n2, 1.f - n2_n2 / 2.f);

	return clamp(n3, -1.f, 1.f);
}

void VAT_float(float2 inTexCoords0, float2 inTexCoords1, float3 inPosition, float3 inNormal, float3 inTangent, float4 inColor, float VATCursor, out float3 outPosition, out float3 outNormal, out float3 outTangent, out float4 outColor)
{
#if defined(PK_HAS_VAT_RIGID)

	float cursor = VATCursor;
	cursor = (ceil(saturate(cursor) * (_VATNumFrames - 1))) / _VATNumFrames;
	
	float2 samplingUVs = float2(inTexCoords1.x, inTexCoords1.y - cursor);

	if (_VATPadToPowerOf2)
		samplingUVs *= _VATPaddedRatio.xy;

	float4			p = PK_SAMPLE_TEXTURE2D_LOD(_VATPositionMap, samplingUVs, 0.0f);
	float4			r = PK_SAMPLE_TEXTURE2D_LOD(_VATRotationMap, samplingUVs, 0.0f);

	float3 offsets = ConvertSpace(lerp(_VATBounds.x, _VATBounds.y, p.xyz));
	float3 pivot = ConvertSpace(lerp(_VATPivot.x, _VATPivot.y, inColor.xyz));

	float4 q = r * 2.f - 1.f;

	q.xyz = ConvertSpace(q.xyz);
	
	outPosition = RotateVector(inPosition - pivot, q) + offsets;
	outNormal = RotateVector(inNormal, q);
	outTangent = RotateVector(inTangent, q);
	outColor = float4(1, 1, 1, 1);

#elif defined(PK_HAS_VAT_SOFT)

	float cursor = VATCursor;
	cursor = (ceil(saturate(cursor) * (_VATNumFrames - 1))) / _VATNumFrames;
	
	float2 samplingUVs = float2(inTexCoords1.x, inTexCoords1.y - cursor);

	if (_VATPadToPowerOf2)
		samplingUVs *= _VATPaddedRatio.xy;

	float4			p = PK_SAMPLE_TEXTURE2D_LOD(_VATPositionMap, samplingUVs, 0.0f);

	float3			n;
	if (_VATPackedData)
		n = UnpackAlpha(p.w);
	else
	{
		n = PK_SAMPLE_TEXTURE2D_LOD(_VATNormalMap, samplingUVs, 0.0f);
	}

	outPosition = inPosition + ConvertSpace(_VATNormalizedData ? lerp(_VATBounds.x, _VATBounds.y, p.xyz) : p.xyz);
	outNormal = ConvertSpace(n.xyz);
	outTangent = normalize(cross(outNormal, float3(1.0, -1.0, 1.0)));
	outColor = float4(1, 1, 1, 1);

#elif defined(PK_HAS_VAT_FLUID)

	float cursor = VATCursor;
	cursor = (ceil(saturate(cursor) * (_VATNumFrames - 1))) / _VATNumFrames;
	
	float2 samplingUVs = float2(inTexCoords0.x, inTexCoords0.y - cursor);

	if (_VATPadToPowerOf2)
		samplingUVs *= _VATPaddedRatio.xy;

	float4			p = PK_SAMPLE_TEXTURE2D_LOD(_VATPositionMap, samplingUVs, 0.0f);

	float3			n;
	if (_VATPackedData)
		n = UnpackAlpha(p.w);
	else
	{
		float4	sampledNormal = PK_SAMPLE_TEXTURE2D_LOD(_VATNormalMap, samplingUVs, 0.0f);
		n = sampledNormal.xyz;
	}

	float4	c = PK_SAMPLE_TEXTURE2D_LOD(_VATColorMap, samplingUVs, 0.0f);

	outPosition = ConvertSpace(_VATNormalizedData ? lerp(_VATBounds.x, _VATBounds.y, p.xyz) : p.xyz);
	outNormal = ConvertSpace(n.xyz);
	outTangent = normalize(cross(outNormal, float3(1.0, -1.0, 1.0)));
	outColor = c;

#else

	outPosition = inPosition;
	outNormal = inNormal;
	outTangent = inTangent;
	outColor = inColor;

#endif
}