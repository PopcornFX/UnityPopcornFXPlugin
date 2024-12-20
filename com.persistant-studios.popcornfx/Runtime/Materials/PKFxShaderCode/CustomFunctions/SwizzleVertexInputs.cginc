
void	SwizzleVertexInputs_float(	in float4 texCoord0, in float4 texCoord1,
									in float4 texCoord2, in float4 texCoord3,
									out float2 uv0, out float2 uv1, out float frameLerp,
									out float2 uvFactors, out float2 uvScale, out float2 uvOffset,
									out float alphaCursor,
									out float4 emissiveColor, out float transformUvsRotate, out float4 transformUvsScaleAndOffset,
									out float2 alphaMasksAnimationCursors, out float2 uvDistortionsAnimationCursors, out float dissolveCursor, out float2 rawTexCoord0)
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
	emissiveColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	transformUvsRotate = 0.0f;
	transformUvsScaleAndOffset = float4(0.0f, 0.0f, 0.0f, 0.0f);
	alphaMasksAnimationCursors = float2(0.0f, 0.0f);
	uvDistortionsAnimationCursors = float2(0.0f, 0.0f);
	dissolveCursor = 0.0f;
	rawTexCoord0 = uv0;

# if PK_HAS_RIBBON_COMPLEX_ON
	uvFactors = vertexInputs[currentIdx].xy;

#	if	PK_HAS_ALPHA_REMAP_ON
	alphaCursor = vertexInputs[currentIdx].z;
#		if	PK_HAS_TRANSFORM_UVS_ON
	transformUvsRotate = vertexInputs[currentIdx].w;
#		endif
#	elif	PK_HAS_TRANSFORM_UVS_ON
	transformUvsRotate = vertexInputs[currentIdx].z;
#	endif
	currentIdx++;
	uvScale = vertexInputs[currentIdx].xy;
	uvOffset = vertexInputs[currentIdx++].zw;
# elif	PK_HAS_ANIM_BLEND_ON
	uv0 = vertexInputs[currentIdx].xy;
	uv1 = vertexInputs[currentIdx++].zw;
	int animBlendAlphaCursorIdx = currentIdx;
	frameLerp = vertexInputs[currentIdx++].x;

#	if	PK_HAS_ALPHA_REMAP_ON
	alphaCursor = vertexInputs[animBlendAlphaCursorIdx].y;
#	endif
#	if	PK_HAS_TRANSFORM_UVS_ON
	transformUvsRotate = vertexInputs[animBlendAlphaCursorIdx].z;
#	endif
# else
	int alphaCursorIdx = currentIdx;
	uv0 = vertexInputs[currentIdx++].xy;
#	if	PK_HAS_ALPHA_REMAP_ON
	alphaCursor = vertexInputs[alphaCursorIdx].z;
#	endif
# endif

# if	PK_HAS_EMISSIVE_BASIC || PK_HAS_EMISSIVE_WITH_RAMP
	emissiveColor = vertexInputs[currentIdx++];
# endif

#	if	PK_HAS_TRANSFORM_UVS_ON
#		if	!PK_HAS_RIBBON_COMPLEX_ON && !PK_HAS_ANIM_BLEND_ON
#			if PK_HAS_ATLAS_ON
				frameLerp = vertexInputs[currentIdx].y;
#			endif
			transformUvsRotate = vertexInputs[currentIdx++].x;
#		endif
		transformUvsScaleAndOffset = vertexInputs[currentIdx++];
#	endif

#	if PK_HAS_ALPHA_MASKS && PK_HAS_UV_DISTORTIONS
	alphaMasksAnimationCursors = vertexInputs[currentIdx].xy;
	uvDistortionsAnimationCursors = vertexInputs[currentIdx++].zw;
#	else
#		if PK_HAS_ALPHA_MASKS
	alphaMasksAnimationCursors = vertexInputs[currentIdx++].xy;
#		endif
#		if PK_HAS_UV_DISTORTIONS
	uvDistortionsAnimationCursors = vertexInputs[currentIdx++].xy;
#		endif // PK_HAS_UV_DISTORTIONS
#	endif

#if PK_HAS_DISSOLVE
	dissolveCursor = vertexInputs[currentIdx].x;
#	endif
#	if PK_HAS_ATLAS && (PK_HAS_ALPHA_MASKS || PK_HAS_UV_DISTORTIONS || PK_HAS_DISSOLVE)
#		if PK_HAS_DISSOLVE
	rawTexCoord0 = vertexInputs[currentIdx].yz;
#		else
	rawTexCoord0 = vertexInputs[currentIdx].xy;
#		endif
#	else
	rawTexCoord0 = texCoord0.xy;
#	endif
}
