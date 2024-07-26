
#if PK_HAS_TRANSFORM_UVS_ON
ByteAddressBuffer	_Atlas;

float2 transformUV(float2 UV, float2 scale, float2x2 rotation, float2 offset)
{
	return mul(UV * scale, rotation) + offset;
}

#endif

void	TransformUVs_float(in float2 uv, in float2 uv1, in float frameLerp, in float rotate, in float4 scaleAndOffset,
						   out float2 uvOut, out float2 uv1Out, out float2 dUVdx, out float2 dUVdy, out float frameLerpOut, out float2 oldUV0, out float2 oldUV1)
{
	uvOut = uv;
	uv1Out = uv1;
	dUVdx = float2(0.0f, 0.0f);
	dUVdy = float2(0.0f, 0.0f);
	frameLerpOut = frac(frameLerp);
	oldUV0 = uv;
	oldUV1 = uv1;

#if PK_HAS_TRANSFORM_UVS_ON
	float		sinR = sin(-rotate);
	float		cosR = cos(-rotate);
	float2x2	UVRotation = float2x2(cosR, sinR, -sinR, cosR);
	float2		UVScale = scaleAndOffset.xy;
	float2		UVOffset = scaleAndOffset.zw;
	float4		rect0 = float4(1.0f, 1.0f, 0.0f, 0.0f);
#	if PK_HAS_ATLAS_ON
	float	idf = abs(frameLerp);
	uint	maxAtlasIdx = _Atlas.Load(0) - 1;
	uint	idA = min(uint(floor(idf)), maxAtlasIdx);
	rect0 = asfloat(_Atlas.Load4((idA + 1) * 4 * 4));
	#	if	PK_HAS_ANIM_BLEND_ON
		uint	idB = min(idA + 1U, maxAtlasIdx);
		float4	rect1 = asfloat(_Atlas.Load4((idB + 1) * 4 * 4));
		uv1 = ((uv1 - rect1.zw) / rect1.xy); // normalize (if atlas)

		uv1 = transformUV(uv1, UVScale, UVRotation, UVOffset); // scale then rotate then translate UV
		uv1Out = frac(uv1) * rect1.xy + rect1.zw; // undo normalize
	#	endif
#endif
	uv = ((uv - rect0.zw) / rect0.xy); // normalize (if atlas)
	uv = transformUV(uv, UVScale, UVRotation, UVOffset); // scale then rotate then translate UV
														 // For clean derivatives:
	float2	_uv = uv * rect0.xy + rect0.zw;
	dUVdx = ddx(_uv);
	dUVdy = ddy(_uv);
	uvOut = frac(uv) * rect0.xy + rect0.zw; // undo normalize
#endif
}