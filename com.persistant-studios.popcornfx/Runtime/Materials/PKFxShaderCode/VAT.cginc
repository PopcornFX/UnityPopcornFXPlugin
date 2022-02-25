
int		_VATNumFrames;
int		_VATPackedData;
int		_VATNormalizedData;
int		_VATPadToPowerOf2;
float2	_VATBounds;
float4	_VATPaddedRatio;
float2	_VATPivot;


TEXTURE2D(_VATPositionMap);
SAMPLER(sampler_VATPositionMap);
float4	_VATPositionMap_TexelSize;
TEXTURE2D(_VATRotationMap);
SAMPLER(sampler_VATRotationMap);
float4	_VATRotationMap_TexelSize;
TEXTURE2D(_VATNormalMap);
SAMPLER(sampler_VATNormalMap);
float4	_VATNormalMap_TexelSize;
TEXTURE2D(_VATColorMap);
SAMPLER(sampler_VATColorMap);
float4	_VATColorMap_TexelSize;

#include "InstancingBufferDecl.cginc"

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

void VAT_float(float2 inTexCoords0, float2 inTexCoords1, float3 inPosition, float3 inNormal, float3 inTangent, float4 inColor, out float3 outPosition, out float3 outNormal, out float3 outTangent, out float4 outColor)
{
#if defined(_HAS_VAT_RIGID)

	float cursor = UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _VATCursor);
	cursor = (ceil(saturate(cursor) * (_VATNumFrames - 1))) / _VATNumFrames;
	
	float4 samplingUVs = float4(inTexCoords1.x, inTexCoords1.y - cursor, 0, 0);

	if (_VATPadToPowerOf2)
		samplingUVs *= float4(_VATPaddedRatio.x, _VATPaddedRatio.y, 1, 1);

	UnityTexture2D	positionMap = UnityBuildTexture2DStructNoScale(_VATPositionMap);
	UnityTexture2D	rotationMap = UnityBuildTexture2DStructNoScale(_VATRotationMap);
	float4			p = SAMPLE_TEXTURE2D_LOD(positionMap.tex,  UnityBuildSamplerStateStruct(sampler_VATPositionMap).samplerstate, samplingUVs, 0.);
	float4			r = SAMPLE_TEXTURE2D_LOD(rotationMap.tex,  UnityBuildSamplerStateStruct(sampler_VATRotationMap).samplerstate, samplingUVs, 0.);

	float3 offsets = ConvertSpace(lerp(_VATBounds.x, _VATBounds.y, p.xyz));
	float3 pivot = ConvertSpace(lerp(_VATPivot.x, _VATPivot.y, inColor.xyz));

	float4 q = r * 2.f - 1.f;

	q.xyz = ConvertSpace(q.xyz);
	
	outPosition = RotateVector(inPosition - pivot, q) + offsets;
	outNormal = RotateVector(inNormal, q);
	outTangent = RotateVector(inTangent, q);
	outColor = float4(1, 1, 1, 1);

#elif defined(_HAS_VAT_SOFT)

	float cursor = UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _VATCursor);
	cursor = (ceil(saturate(cursor) * (_VATNumFrames - 1))) / _VATNumFrames;
	
	float4 samplingUVs = float4(inTexCoords1.x, inTexCoords1.y - cursor, 0, 0);

	if (_VATPadToPowerOf2)
		samplingUVs *= float4(_VATPaddedRatio.x, _VATPaddedRatio.y, 1, 1);

	UnityTexture2D	positionMap = UnityBuildTexture2DStructNoScale(_VATPositionMap);
	float4			p = SAMPLE_TEXTURE2D_LOD(positionMap.tex,  UnityBuildSamplerStateStruct(sampler_VATPositionMap).samplerstate, samplingUVs, 0.);

	float3			n;
	if (_VATPackedData)
		n = UnpackAlpha(p.w);
	else
	{
		UnityTexture2D	normalMap = UnityBuildTexture2DStructNoScale(_VATNormalMap);
		n = SAMPLE_TEXTURE2D_LOD(normalMap.tex,  UnityBuildSamplerStateStruct(sampler_VATNormalMap).samplerstate, samplingUVs, 0.).xyz;
	}

	outPosition = inPosition + ConvertSpace(lerp(_VATBounds.x, _VATBounds.y, p.xyz));
	outNormal = ConvertSpace(n.xyz);
	outTangent = normalize(cross(outNormal, float3(1.0, -1.0, 1.0)));
	outColor = float4(1, 1, 1, 1);

#elif defined(_HAS_VAT_FLUID)

	float cursor = UNITY_ACCESS_INSTANCED_PROP(InstanceProperties, _VATCursor);
	cursor = (ceil(saturate(cursor) * (_VATNumFrames - 1))) / _VATNumFrames;
	
	float4 samplingUVs = float4(inTexCoords0.x, inTexCoords0.y - cursor, 0, 0);

	if (_VATPadToPowerOf2)
		samplingUVs *= float4(_VATPaddedRatio.x, _VATPaddedRatio.y, 1, 1);

	UnityTexture2D	positionMap = UnityBuildTexture2DStructNoScale(_VATPositionMap);
	float4			p = SAMPLE_TEXTURE2D_LOD(positionMap.tex,  UnityBuildSamplerStateStruct(sampler_VATPositionMap).samplerstate, samplingUVs, 0.);

	float3			n;
	if (_VATPackedData)
		n = UnpackAlpha(p.w);
	else
	{
		UnityTexture2D	normalMap = UnityBuildTexture2DStructNoScale(_VATNormalMap);
		n = SAMPLE_TEXTURE2D_LOD(normalMap.tex,  UnityBuildSamplerStateStruct(sampler_VATNormalMap).samplerstate, samplingUVs, 0.).xyz;
	}

	UnityTexture2D	colorMap = UnityBuildTexture2DStructNoScale(_VATColorMap);
	float4			c = SAMPLE_TEXTURE2D_LOD(colorMap.tex,  UnityBuildSamplerStateStruct(sampler_VATColorMap).samplerstate, samplingUVs, 0.);

	outPosition = ConvertSpace(lerp(_VATBounds.x, _VATBounds.y, p.xyz));
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