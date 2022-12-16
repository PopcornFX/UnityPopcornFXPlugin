
void	VertexRibbonCorrectDeformation_float(	in float2 uvFactors, in float2 uvScale, in float2 uvOffset, in float floatVertexID,
												out float4 outUvScaleAndOffset, out float2 outUv0, out float4 outUvFactors)
{
	uint	vertexId = (uint)floatVertexID;

	outUvScaleAndOffset = float4(uvScale, uvOffset);
	if (vertexId % 4 == 0)
	{
		outUv0 = float2(0.0, 0.0);
		outUvFactors = float4(1.0, 1.0, 1.0, 1.0);
	}
	else if (vertexId % 4 == 1)
	{
		outUv0 = float2(0.0, 1.0);
		outUvFactors = float4(uvFactors.x, 1.0, 1.0, 1.0 / uvFactors.y);
	}
	else if (vertexId % 4 == 2)
	{
		outUv0 = float2(1.0, 0.0);
		outUvFactors = float4(1.0, uvFactors.y, 1.0 / uvFactors.x, 1.0);
	}
	else
	{
		outUv0 = float2(1.0, 1.0);
		outUvFactors = float4(1.0, 1.0, 1.0, 1.0);
	}
}

void	FragmentRibbonCorrectDeformation_float(	in float4 uvScaleAndOffset, in float2 uv0, in float4 uvFactors, in bool rotateUv,
												out float2 outUv)
{
	if (uv0.x + uv0.y < 1.0f)
		outUv = uv0.xy / uvFactors.xy;
	else
		outUv = 1.0f - ((1.0f - uv0.xy) / uvFactors.zw);
	if (rotateUv)
		outUv = outUv.yx;
	outUv = outUv * uvScaleAndOffset.xy + uvScaleAndOffset.zw;
}