
#if PK_HAS_TRANSFORM_UVS_ON

float2 transformUV(float2 UV, float2 scale, float2x2 rotation, float2 offset)
{
	return mul(UV * scale, rotation) + offset;
}

#endif

void	TransformUVs_float(in float2 uv, in float2 uv1, in float frameLerp, in float rotate, in float4 offsetScale, in float4 rect0in, in float4 rect1in,
						   out float2 uvOut, out float2 uv1Out, out float2 dUVdx, out float2 dUVdy, out float frameLerpOut)
{
	uvOut = uv;
	uv1Out = uv1;
	dUVdx = float2(0.0f, 0.0f);
	dUVdy = float2(0.0f, 0.0f);
	frameLerpOut = frameLerp;

#if PK_HAS_TRANSFORM_UVS_ON
	float		sinR = sin(-rotate);
	float		cosR = cos(-rotate);
	float2x2	UVRotation = float2x2(cosR, sinR, -sinR, cosR);
	float2		UVScale = offsetScale.zw;
	float2		UVOffset = offsetScale.xy;
	float4		rect0 = float4(1.0f, 1.0f, 0.0f, 0.0f);
#	if	PK_HAS_ANIM_BLEND_ON
	rect0 = rect0in;
	float4	rect1 = rect1in;
	uv1 = ((uv1 - rect1.zw) / rect1.xy); // normalize (if atlas)

	uv1 = transformUV(uv1, UVScale, UVRotation, UVOffset); // scale then rotate then translate UV
	uv1Out = frac(uv1) * rect1.xy + rect1.zw; // undo normalize

#	endif
	uv = ((uv - rect0.zw) / rect0.xy); // normalize (if atlas)
	uv = transformUV(uv, UVScale, UVRotation, UVOffset); // scale then rotate then translate UV
														 // For clean derivatives:
	float2	_uv = uv * rect0.xy + rect0.zw;
	dUVdx = ddx(_uv);
	dUVdy = ddy(_uv);
	uvOut = frac(uv) * rect0.xy + rect0.zw; // undo normalize
#endif
}