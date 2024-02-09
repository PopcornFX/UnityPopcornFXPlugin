//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
struct SVertexOutput
{
	float4 vertex					: SV_POSITION;
	float4 ProjPos					: TEXCOORD0;

	#if	PK_HAS_COLOR
		float4 Color				: COLOR;
	#endif


	#if	PK_HAS_LIGHTING
		float3 Normal				: NORMAL;
		float3 Tangent				: TANGENT;
	#endif

	#if	PK_HAS_RIBBON_COMPLEX
		float2 UV0					: TEXCOORD1;
		float4 UVScaleAndOffset		: TEXCOORD2;
		float4 UVFactors			: TEXCOORD3;
	
		#if	PK_HAS_ALPHA_REMAP
			float  AlphaCursor 		: TEXCOORD4;
		#endif
	#elif	PK_HAS_ANIM_BLEND
		float2 UV0					: TEXCOORD1;
		float2 UV1					: TEXCOORD2;
		float  FrameLerp			: TEXCOORD3;
	
		#if		PK_HAS_ALPHA_REMAP
			float  AlphaCursor 		: TEXCOORD4;
		#endif
	#else
		float2 UV0					: TEXCOORD1;
	
		#if		PK_HAS_ALPHA_REMAP
			float  AlphaCursor 		: TEXCOORD2;
		#endif
			
	#endif
	
	#if !PK_HAS_EMISSIVE_NONE
		float3 EmissiveColor		: TEXCOORD5;
	#endif
		
	#if	PK_HAS_TRANSFORM_UVS
		float2   TransformUVs_Rotate			: TEXCOORD6;
		float4   TransformUVs_ScaleAndOffset	: TEXCOORD7;
	#endif

	UNITY_VERTEX_INPUT_INSTANCE_ID
	UNITY_VERTEX_OUTPUT_STEREO
};
