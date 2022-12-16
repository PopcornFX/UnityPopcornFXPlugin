
void 	SwizzleVertexInputs_float(	in float4 texCoord0, in float4 texCoord1,
									in float4 texCoord2, in float4 texCoord3,
									out float2 uv0, out float2 uv1, out float frameLerp,
									out float2 uvFactors, out float2 uvScale, out float2 uvOffset,
									out float alphaCursor,
									out float3 emissiveColor)
{
	float4 	vertexInputs[4] = { texCoord0, texCoord1, texCoord2, texCoord3 };
	int 	currentIdx = 0;

	uv0 = float2(0.0f, 0.0f);
	uv1 = float2(0.0f, 0.0f);
	frameLerp = 0.0f;
	uvFactors = float2(0.0f, 0.0f);
	uvScale = float2(0.0f, 0.0f);
	uvOffset = float2(0.0f, 0.0f);
	alphaCursor = 0.0f;
	emissiveColor = float3(0.0f, 0.0f, 0.0f);

# if	PK_HAS_RIBBON_COMPLEX_ON
	uvFactors = vertexInputs[currentIdx++].xy;
	uvScale = vertexInputs[currentIdx++].xy;
	uvOffset = vertexInputs[currentIdx++].xy;
# elif	PK_HAS_ANIM_BLEND_ON
	uv0 = vertexInputs[currentIdx++].xy;
	uv1 = vertexInputs[currentIdx++].xy;
	frameLerp = frac(vertexInputs[currentIdx++].x);
# else
	uv0 = vertexInputs[currentIdx++].xy;
# endif

# if	PK_HAS_ALPHA_REMAP_ON
#	if		PK_HAS_ANIM_BLEND_ON
	alphaCursor = vertexInputs[currentIdx - 1].y;
#	else
	alphaCursor = vertexInputs[currentIdx++].x;
#endif
# endif

# if	PK_HAS_EMISSIVE_BASIC || PK_HAS_EMISSIVE_WITH_RAMP
	emissiveColor = vertexInputs[currentIdx++].xyz;
# endif
}
