//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
struct SVertexInput
{
	float3 vertex					: POSITION;
#	if	PK_HAS_COLOR
		float4 Color				: COLOR;
#	endif

#	if	PK_HAS_LIGHTING
		float4 Normal				: NORMAL;
#	endif

#	if	PK_HAS_RIBBON_COMPLEX
		//------------------------------------------
		float2 UVFactors			: TEXCOORD0;
		float2 UVScale				: TEXCOORD1;
		float2 UVOffset				: TEXCOORD2;
		uint   VertexID				: SV_VertexID;

#		if	PK_HAS_ALPHA_REMAP
			float2 AlphaCursor		: TEXCOORD3;
#		endif
		//------------------------------------------
#	elif	PK_HAS_ANIM_BLEND
		//------------------------------------------
		float2 UV0					: TEXCOORD0;
		float2 UV1					: TEXCOORD1;
		float2 AtlasIdAlphaCursor	: TEXCOORD2;
		//------------------------------------------
#	else
		//------------------------------------------
		float2 UV0					: TEXCOORD0;

#		if	PK_HAS_ALPHA_REMAP
			float2 AlphaCursor		: TEXCOORD1;
#		endif
		//------------------------------------------
#	endif

	UNITY_VERTEX_INPUT_INSTANCE_ID
};
