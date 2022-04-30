//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#include "VertexBillboarding.cginc"

SVertexOutput	vert(SVertexInput v)
{
	SVertexOutput	o = vertBillboardingImplem(v.VertexID);

	UNITY_SETUP_INSTANCE_ID(v);
	UNITY_TRANSFER_INSTANCE_ID(v, o);
	UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o);

	#if		USE_HDRP
		//Transforming vertex with HDRP
		AttributesMesh coreVertexInput;
		coreVertexInput.positionOS = o.vertex.xyz;
		VaryingsMeshType outVarying = VertMesh(coreVertexInput);
		o.vertex = float4(outVarying.positionCS);
		o.ProjPos = float4(outVarying.positionCS);
	#elif USE_URP
		o.vertex = o.ProjPos;
	#else
		// Generic outputs for all variations:
		// Vertex position:
		o.vertex = o.ProjPos;
	#endif

	return o;
}
