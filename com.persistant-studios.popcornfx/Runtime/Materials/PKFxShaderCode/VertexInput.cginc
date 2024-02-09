//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
struct SVertexInput
{
	float3 vertex					: POSITION;
#	if	PK_HAS_COLOR
		float4 Color				: COLOR0;
#	endif


#	if	PK_HAS_LIGHTING
		float4 Normal				: NORMAL;
		float3 Tangent				: TANGENT;
#	endif

#	if	PK_HAS_RIBBON_COMPLEX
		//------------------------------------------

#		if	PK_HAS_ALPHA_REMAP && PK_HAS_TRANSFORM_UVS
		float4 UVFactorsAlphaRotate		: TEXCOORD0; // UVFactors + AlphaCursor + RotateUVs
#		elif PK_HAS_ALPHA_REMAP || PK_HAS_TRANSFORM_UVS
		float3 UVFactorsAlphaRotate		: TEXCOORD0; // UVFactors + (AlphaCursor | RotateUVs)
#		else
		float2 UVFactorsAlphaRotate		: TEXCOORD0;
#		endif
		float4 UVScaleOffset				: TEXCOORD1;
		uint   VertexID						: SV_VertexID;

#		if	!PK_HAS_EMISSIVE_NONE
#		define EMISSIVETEXCOORD				TEXCOORD2
#		define TRANSFORMUV1_TEXCOORD		TEXCOORD3
#		else
#		define TRANSFORMUV0_TEXCOORD		TEXCOORD2
#		define TRANSFORMUV1_TEXCOORD		TEXCOORD3
#		endif
		//------------------------------------------
#	elif	PK_HAS_ANIM_BLEND
		//------------------------------------------
		float4 UVs							: TEXCOORD0; // UV0 + UV1

#		if PK_HAS_TRANSFORM_UVS
		float3 AtlasIdAlphaCursorRotate		: TEXCOORD1; // Atlasid + AlphaCursor + rotate
#		else
		float2 AtlasIdAlphaCursorRotate		: TEXCOORD1; // Atlasid + AlphaCursor
#		endif

#		if	!PK_HAS_EMISSIVE_NONE
#		define EMISSIVETEXCOORD				TEXCOORD2
#		define TRANSFORMUV1_TEXCOORD		TEXCOORD3
#		else
#		define TRANSFORMUV0_TEXCOORD		TEXCOORD2
#		define TRANSFORMUV1_TEXCOORD		TEXCOORD3
#		endif
		//------------------------------------------
#	else
		//------------------------------------------
#		if	PK_HAS_ALPHA_REMAP
		float3 UV0AlphaCursor			: TEXCOORD0; // UV0 + AlphaCursor
#		else
		float2 UV0AlphaCursor			: TEXCOORD0; // UV0
#		endif
#		if	!PK_HAS_EMISSIVE_NONE
#		define EMISSIVETEXCOORD				TEXCOORD1
#		define TRANSFORMUV0_TEXCOORD		TEXCOORD2
#		define TRANSFORMUV1_TEXCOORD		TEXCOORD3
#		else
#		define TRANSFORMUV0_TEXCOORD		TEXCOORD1
#		define TRANSFORMUV1_TEXCOORD		TEXCOORD2
#		endif
		//------------------------------------------
#	endif
#	if	!PK_HAS_EMISSIVE_NONE
		float3 EmissiveColor				: EMISSIVETEXCOORD;
#	endif

#	if	PK_HAS_TRANSFORM_UVS
#		if	!PK_HAS_RIBBON_COMPLEX && !PK_HAS_ANIM_BLEND
		float2 TransformUVs_Rotate			: TRANSFORMUV0_TEXCOORD; // Only one float
#		endif
		float4 TransformUVs_ScaleAndOffset	: TRANSFORMUV1_TEXCOORD;
#	endif
	UNITY_VERTEX_INPUT_INSTANCE_ID
};
