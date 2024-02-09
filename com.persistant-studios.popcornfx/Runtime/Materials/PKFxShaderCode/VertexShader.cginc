//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
SVertexOutput	vert(SVertexInput v)
{
	SVertexOutput	o;

	UNITY_SETUP_INSTANCE_ID(v);
 	UNITY_TRANSFER_INSTANCE_ID(v, o);
	UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o);

	#if	USE_HDRP
		//Transforming vertex with HDRP
		AttributesMesh coreVertexInput;
		coreVertexInput.positionOS = v.vertex;
		VaryingsMeshType outVarying = VertMesh(coreVertexInput);
		o.vertex = float4(outVarying.positionCS);
		o.ProjPos = float4(outVarying.positionCS);
	#elif USE_URP

		VertexPositionInputs vertexInput = GetVertexPositionInputs(v.vertex);
		o.vertex = vertexInput.positionCS;
		o.ProjPos = o.vertex;
	#else
		o.vertex = UnityObjectToClipPos(v.vertex);
		o.ProjPos = o.vertex;
	#endif

	#if	PK_HAS_COLOR
		o.Color = v.Color;
	#endif
	
	#if	!PK_HAS_EMISSIVE_NONE
		o.EmissiveColor = v.EmissiveColor;
	#endif

	#if	PK_HAS_LIGHTING
		o.Normal = v.Normal.xyz;
		o.Tangent = v.Tangent;
	#endif

	#if	PK_HAS_RIBBON_COMPLEX

		o.UVScaleAndOffset = v.UVScaleOffset;

		uint		id = v.VertexID;

		if (id % 4 == 0)
		{
			o.UV0 = float2(0.0, 0.0);
			o.UVFactors = float4(1.0, 1.0, 1.0, 1.0);
		}
		else if (id % 4 == 1)
		{
			o.UV0 = float2(0.0, 1.0);
			o.UVFactors = float4(v.UVFactorsAlphaRotate.x, 1.0, 1.0, 1.0 / v.UVFactorsAlphaRotate.y);
		}
		else if (id % 4 == 2)
		{
			o.UV0 = float2(1.0, 0.0);
			o.UVFactors = float4(1.0, v.UVFactorsAlphaRotate.y, 1.0 / v.UVFactorsAlphaRotate.x, 1.0);
		}
		else
		{
			o.UV0 = float2(1.0, 1.0);
			o.UVFactors = float4(1.0, 1.0, 1.0, 1.0);
		}

		#if	PK_HAS_ALPHA_REMAP
			o.AlphaCursor = v.UVFactorsAlphaRotate.z;
		#endif

	#elif	PK_HAS_ANIM_BLEND
		//------------------------------------------
		//	Animation blend
		//------------------------------------------

		o.UV0 = v.UVs.xy;
		o.UV1 = v.UVs.zw;
		o.FrameLerp = /*frac(*/v.AtlasIdAlphaCursorRotate.x;//);

		#if	PK_HAS_ALPHA_REMAP
			o.AlphaCursor = v.AtlasIdAlphaCursorRotate.y;
		#endif
	#else
		//------------------------------------------
		//	No animation blending
		//------------------------------------------

		o.UV0 = v.UV0AlphaCursor.xy;

		#if	PK_HAS_ALPHA_REMAP
			o.AlphaCursor = v.UV0AlphaCursor.z;
		#endif
	#endif

	#if	PK_HAS_TRANSFORM_UVS
		#if	PK_HAS_RIBBON_COMPLEX && PK_HAS_ALPHA_REMAP
			o.TransformUVs_Rotate = v.UVFactorsAlphaRotate.w;
		#elif PK_HAS_RIBBON_COMPLEX
			o.TransformUVs_Rotate = v.UVFactorsAlphaRotate.z;
		#elif PK_HAS_ANIM_BLEND
			o.TransformUVs_Rotate = v.AtlasIdAlphaCursorRotate.z;
		#else
			o.TransformUVs_Rotate = v.TransformUVs_Rotate;
		#endif
		o.TransformUVs_ScaleAndOffset = v.TransformUVs_ScaleAndOffset;
	#endif
	return o;
}
