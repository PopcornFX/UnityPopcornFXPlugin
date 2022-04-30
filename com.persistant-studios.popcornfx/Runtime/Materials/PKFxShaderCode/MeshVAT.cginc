//--------------------------------------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//--------------------------------------------------------------------------------------------------------
struct v2f
{
	float4 vertex : SV_POSITION;
	float2 uv : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

sampler2D _MainTex;
float4	_MainTex_ST;
float4	_Color;

int		_NumFrames;
int		_PackedData;		// bool
int		_NormalizedData;	// bool
int		_PadToPowerOf2;		// bool
float4	_Bounds;			// Vector2
float4	_PaddedRatio;		// Vector2


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