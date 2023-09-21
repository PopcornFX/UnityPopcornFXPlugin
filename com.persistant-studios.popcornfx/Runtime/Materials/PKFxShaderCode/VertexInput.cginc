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
		float2 UVFactors			: TEXCOORD0;
		float2 UVScale				: TEXCOORD1;
		float2 UVOffset				: TEXCOORD2;
		uint   VertexID				: SV_VertexID;

#		if	PK_HAS_ALPHA_REMAP
			float2 AlphaCursor		: TEXCOORD3;
#			define EMISSIVETEXCOORD TEXCOORD4
#		else
#			define EMISSIVETEXCOORD TEXCOORD3
#		endif
		//------------------------------------------
#	elif	PK_HAS_ANIM_BLEND
		//------------------------------------------
		float2 UV0					: TEXCOORD0;
		float2 UV1					: TEXCOORD1;
		float2 AtlasIdAlphaCursor	: TEXCOORD2;
		
#		define EMISSIVETEXCOORD TEXCOORD3
		//------------------------------------------
#	else
		//------------------------------------------
		float2 UV0					: TEXCOORD0;

#		if	PK_HAS_ALPHA_REMAP
			float2 AlphaCursor		: TEXCOORD1;
			
#			define EMISSIVETEXCOORD TEXCOORD2
#		else
#			define EMISSIVETEXCOORD TEXCOORD1
#		endif
		//------------------------------------------
#	endif
#	if	!PK_HAS_EMISSIVE_NONE
		float3 EmissiveColor		: EMISSIVETEXCOORD;
#	endif

#if	PK_HAS_TRANSFORM_UVS
		float2  TransformUVs_Rotate				: TEXCOORD6;
		float4  TransformUVs_ScaleAndOffset		: TEXCOORD7;
#endif
	UNITY_VERTEX_INPUT_INSTANCE_ID
};
