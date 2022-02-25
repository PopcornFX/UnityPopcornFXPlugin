//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
/* Use this pragma to use different lvl features
	#pragma multi_compile_local BB_FeatureC0 BB_FeatureC1 BB_FeatureC1_Capsule BB_FeatureC2
	#pragma multi_compile_local _ BB_SizeFloat2
	#pragma multi_compile_local _ BB_Feature_Atlas PK_HAS_ANIM_BLEND
*/

// Should match the PKFxProceduralRenderer.EBillboardingInputs:
#define		GenInput_GeomPosition	0
#define		GenInput_GeomSize		1
#define		GenInput_GeomRotation	2
#define		GenInput_GeomAxis0		3
#define		GenInput_GeomAxis1		4
#define		GenInput_Color			5
#define		GenInput_TextureID		6
#define		GenInput_AlphaCursor	7

#if defined(BB_FeatureC0)
#define BB_FeatureLevel 0
#elif defined(BB_FeatureC1) || defined(BB_FeatureC1_Capsule)
# define BB_FeatureLevel 1
#elif defined(BB_FeatureC2)
# define BB_FeatureLevel 2
#endif

#if BB_FeatureLevel >= 0
#	define BB_ScreenAligned				1U
#	define BB_ViewposAligned			2U
#else
#	error config error
#endif

#if BB_FeatureLevel >= 1
#	define NORMALIZER_EPSILON		1.0e-8f
#	define BB_AxisAligned			3U
#	define BB_AxisAlignedSpheroid	4U
#endif

#if defined(BB_FeatureC1_Capsule)
#	define BB_AxisAlignedCapsule	5U
#endif

#if BB_FeatureLevel >= 2
#	define BB_PlaneAligned			6U
#endif

#define	FLIP_BILLBOARDING_AXIS				1

#define	BB_Flag_BillboardMask	7U	// 3 firsts bytes
#define	BB_Flag_VFlipUVs		8U	// 4th byte
#define	BB_Flag_SoftAnimBlend	16U // 5th byte

#define	GET_MATRIX_X_AXIS(_mat)					float4(_mat[0].x, _mat[1].x, _mat[2].x, _mat[3].x)
#define	GET_MATRIX_Y_AXIS(_mat)					float4(_mat[0].y, _mat[1].y, _mat[2].y, _mat[3].y)
#define	GET_MATRIX_Z_AXIS(_mat)					float4(_mat[0].z, _mat[1].z, _mat[2].z, _mat[3].z)
#define	GET_MATRIX_W_AXIS(_mat)					float4(_mat[0].w, _mat[1].w, _mat[2].w, _mat[3].w)

struct SPrimitives
{
	float4	VertexPosition;
#if		defined(BB_SizeFloat2)
	float2	Size;
#else
	float	Size;
#endif
	float	Rotation;
	float3	Axis0;
	float3	Axis1;
	float4	Color;
#if defined(BB_Feature_Atlas) || defined(PK_HAS_ANIM_BLEND)
	float	TextureID;
#endif
};

//Should be cbuffer but durango
ByteAddressBuffer	_InputOffsets;
ByteAddressBuffer	_Atlas;
ByteAddressBuffer	_BillboardInfo;

//
ByteAddressBuffer	_In_RawData;
ByteAddressBuffer	_In_Indexes;

// LeftHanded:
#define	myCross(a, b)  cross(b, a)

static const float4 SideVector = float4(1, 0, 0, 0);
static const float4 DepthVector = float4(0, 0,-1, 0);

void	swap(inout float2 a, inout float2 b)
{
	float2	s = a;
	a = b;
	b = s;
}

void	swap(inout float4 a, inout float4 b)
{
	float4	s = a;
	a = b;
	b = s;
}

float2	get_radius(in SPrimitives gInput)
{
#if		defined(BB_SizeFloat2)
	return gInput.Size;
#else
	return float2(gInput.Size, gInput.Size);
#endif
}

#if defined(BB_ScreenAligned) || defined(BB_ViewposAligned)
void	rotate_axis(in SPrimitives gInput, inout float3 xAxis, inout float3 yAxis)
{
	float	c = cos(gInput.Rotation);
	float	s = sin(gInput.Rotation);
	#if FLIP_BILLBOARDING_AXIS
		s *= -1.f;
	#endif
	float3	xa = xAxis;
	xAxis = yAxis * s + xAxis * c;
	yAxis = yAxis * c - xa * s;

}
#endif

#if	defined(BB_ScreenAligned)
void	bb_ScreenAlignedQuad(in SPrimitives gInput, out float3 xAxis, out float3 yAxis, out float3 nAxis)
{
	float4x4 invView = transpose(UNITY_MATRIX_V);
	float2	radius = get_radius(gInput);
	xAxis = GET_MATRIX_X_AXIS(invView).xyz;
	yAxis = GET_MATRIX_Y_AXIS(invView).xyz;
	nAxis = GET_MATRIX_Z_AXIS(invView).xyz;
	
	rotate_axis(gInput, xAxis, yAxis);

	xAxis *= radius.x;
	yAxis *= radius.y;
}
#endif

#if	defined(BB_ViewposAligned)
void	bb_ViewposAlignedQuad(in SPrimitives gInput, out float3 xAxis, out float3 yAxis, out float3 nAxis)
{
	float3	viewPos = _WorldSpaceCameraPos;
	float3	camToParticle = normalize(gInput.VertexPosition.xyz - viewPos);
	float3	upVector = myCross(DepthVector.xyz, SideVector.xyz);
	float3	upAxis = upVector + 0.01f * dot(upVector, camToParticle) * DepthVector.xyz;

	xAxis = normalize(myCross(camToParticle, upAxis));
	yAxis = myCross(xAxis, camToParticle);
	float2	radius = get_radius(gInput);

	rotate_axis(gInput, xAxis, yAxis);

	xAxis *= radius.x;
	yAxis *= radius.y;
	nAxis = -camToParticle;
}
#endif

#if	defined(BB_AxisAligned)
void	bb_VelocityAxisAligned(in SPrimitives gInput, out float3 xAxis, out float3 yAxis, out float3 nAxis)
{
	float3	viewPos = _WorldSpaceCameraPos;

	float3	camToParticle = normalize(gInput.VertexPosition.xyz - viewPos);
	float2	radius = get_radius(gInput);
	float3	side = SideVector.xyz;
	float3	depth = DepthVector.xyz;

	xAxis = myCross(camToParticle, gInput.Axis0);
	if (dot(xAxis, xAxis) > NORMALIZER_EPSILON)
		xAxis = normalize(xAxis);
	else
	{
		float3	v = -side * dot(camToParticle, depth) + depth * (dot(camToParticle, side) + 0.01f);
		xAxis = normalize(myCross(camToParticle, v));
	}
	xAxis *= radius.x;

	yAxis = gInput.Axis0 * 0.5f;
	nAxis = -camToParticle;
}
#endif

#if	defined(BB_AxisAlignedSpheroid)
void	bb_VelocitySpheroidalAlign(in SPrimitives gInput, out float3 xAxis, out float3 yAxis, out float3 nAxis)
{
	float3	viewPos = _WorldSpaceCameraPos;

	float3	camToParticle = normalize(gInput.VertexPosition.xyz - viewPos);
	float2	radius = get_radius(gInput);
	float3	side = SideVector.xyz;
	float3	depth = DepthVector.xyz;

	float3	sideVec = myCross(camToParticle, gInput.Axis0);
	if (dot(sideVec, sideVec) > NORMALIZER_EPSILON)
		sideVec = normalize(sideVec);
	else
	{
		float3	v = -side * dot(camToParticle, depth) + depth * (dot(camToParticle, side) + 0.01f);
		sideVec = normalize(myCross(camToParticle, v));
	}
	sideVec *= radius.x;

	float3	upVec = myCross(sideVec, camToParticle);
	xAxis = sideVec;
	yAxis = gInput.Axis0 * 0.5f + upVec;
	nAxis = -camToParticle;
}
#endif

#if	defined(BB_PlaneAligned)
void	bb_PlanarAlignedQuad(in SPrimitives gInput, out float3 xAxis, out float3 yAxis, out float3 nAxis)
{
	float3	center = gInput.VertexPosition.xyz;
	float3	axis_fwd = gInput.Axis0;
	float3	axis_nrm = gInput.Axis1;
	float3	side = SideVector.xyz;
	float3	depth = DepthVector.xyz;

	xAxis = myCross(axis_nrm, axis_fwd);
	if (dot(xAxis, xAxis) > NORMALIZER_EPSILON)
		xAxis = normalize(xAxis);
	else
	{
		float3	v = -side * dot(axis_nrm, depth) + depth * (dot(axis_nrm, side) + 0.01f);
		xAxis = normalize(cross(axis_nrm, v));
	}

	yAxis = myCross(xAxis, axis_nrm);
	float2	scale = get_radius(gInput);

	rotate_axis(gInput, xAxis, yAxis);

	xAxis *= scale.x;
	yAxis *= scale.y;
	nAxis = axis_nrm;

	// Specific to planar aligned quads, flip X
	xAxis = -xAxis;
}
#endif

#if	defined(BB_AxisAlignedCapsule)
void	bb_VelocityCapsuleAlign(in SPrimitives gInput, out float3 xAxis, out float3 yAxis, out float3 upVec, out float3 nAxis)
{
	float4x4 invView = transpose(UNITY_MATRIX_V);
	float3	viewPos = _WorldSpaceCameraPos;

	float3	camToParticle = normalize(gInput.VertexPosition.xyz - viewPos);
	float2	radius = get_radius(gInput);
	float3	side = SideVector.xyz;
	float3	depth = DepthVector.xyz;

	float3	sideVec = myCross(camToParticle, gInput.Axis0);
	if (dot(sideVec, sideVec) > NORMALIZER_EPSILON)
		sideVec = normalize(sideVec);
	else
	{
		float3	v = -side * dot(camToParticle, depth) + depth * (dot(camToParticle, side) + 0.01f);
		sideVec = normalize(myCross(camToParticle, v));
	}
	sideVec *= radius.x * sqrt(2.f);

	upVec = myCross(sideVec, camToParticle);
	xAxis = sideVec;
#if FLIP_BILLBOARDING_AXIS
	xAxis *= -1.0f;
#endif
	yAxis = gInput.Axis0 * 0.5f;
	nAxis = normalize(myCross(sideVec, yAxis));
}
#endif

void	billboard_quad(in SPrimitives gInput, in uint bb, out float3 xAxis, out float3 yAxis, out float3 nAxis)
{
#if		0
	bb_ViewposAlignedQuad(gInput, xAxis, yAxis, nAxis);
#else
	switch (bb)
	{
#if defined(BB_ScreenAligned)
	case BB_ScreenAligned:
		bb_ScreenAlignedQuad(gInput, xAxis, yAxis, nAxis);
		break;
#endif
#if defined(BB_ViewposAligned)
	case BB_ViewposAligned:
		bb_ViewposAlignedQuad(gInput, xAxis, yAxis, nAxis);
		break;
#endif
#if defined(BB_AxisAligned)
	case BB_AxisAligned:
		bb_VelocityAxisAligned(gInput, xAxis, yAxis, nAxis);
		break;
#endif
#if defined(BB_AxisAlignedSpheroid)
	case BB_AxisAlignedSpheroid:
		bb_VelocitySpheroidalAlign(gInput, xAxis, yAxis, nAxis);
		break;
#endif
#if defined(BB_PlaneAligned)
	case BB_PlaneAligned:
		bb_PlanarAlignedQuad(gInput, xAxis, yAxis, nAxis);
		break;
#endif
	default:
		bb_ScreenAlignedQuad(gInput, xAxis, yAxis, nAxis);
		break;
	}
#endif
#if FLIP_BILLBOARDING_AXIS
	yAxis *= -1.0f; // We need to flip the y axis to get the same result as the CPU billboarders
#endif
}

float4	proj_position(float3 position)
{
	#if		USE_HDRP
		//Transforming vertex with HDRP
		AttributesMesh coreVertexInput;
		coreVertexInput.positionOS = position;
		VaryingsMeshType outVarying = VertMesh(coreVertexInput);
		return float4(outVarying.positionCS);
	#elif	USE_URP
		return UnityObjectToClipPos(position);
		//Transforming vertex with URP
		///Attributes coreVertexInput;
		///coreVertexInput.positionOS = float4(position.xyz, 1);
		///Varyings outVarying = LightweightVertexMeta(coreVertexInput);
		///return float4(outVarying.positionCS);
	#else
		return UnityObjectToClipPos(position);
	#endif
}

#if defined(BB_Feature_Atlas) || defined(PK_HAS_ANIM_BLEND)
void	bb_billboardUV(in SPrimitives gInput, in uint bbModeAndFlags, inout float4 c00, inout float4 c01, inout float4 c10, inout float4 c11, inout float atlasId)
#else
void	bb_billboardUV(in SPrimitives gInput, in uint bbModeAndFlags, inout float2 c00, inout float2 c01, inout float2 c10, inout float2 c11)
#endif
{
	uint	drId = asuint(gInput.VertexPosition.w);
#if defined(BB_Feature_Atlas) || defined(PK_HAS_ANIM_BLEND)
	float	idf = abs(gInput.TextureID);
	uint	maxAtlasIdx = _Atlas.Load(0) - 1;
	uint	idA = min(uint(floor(idf)), maxAtlasIdx);
	float4	rectA = asfloat(_Atlas.Load4((idA + 1) * 4 * 4));
	uint	idB = min(idA + 1U, maxAtlasIdx);
	float4	rectB = asfloat(_Atlas.Load4((idB + 1) * 4 * 4));
	float4	maddm = float4(rectA.xy, rectB.xy);
	float4	madda = float4(rectA.zw, rectB.zw);
	float	blendWeight = (bbModeAndFlags & BB_Flag_SoftAnimBlend) != 0U ? 1.0f : 0.0f;
	atlasId = frac(idf) * blendWeight;
	c00 = c00 * maddm + madda;
	c01 = c01 * maddm + madda;
	c10 = c10 * maddm + madda;
	c11 = c11 * maddm + madda;
#endif

	if ((bbModeAndFlags & BB_Flag_VFlipUVs) != 0U)
	{
		swap(c00, c01);
		swap(c10, c11);
	}
}

void	bb_billboardNormal(
	float nFactor,
	out float3 n0, out float3 n1, out float3 n2, out float3 n3,
#if BB_AxisAlignedCapsule
	out float3 n4, out float3 n5,
#endif
	in float3 xAxis, in float3 yAxis, in float3 nAxis)
{
	float	nw = (1.0f - nFactor); // weight
	float3	xAxisNorm = normalize(xAxis);
	float3	yAxisNorm = normalize(yAxis);
	float3	n = nAxis * nw; // normal weighted

#if BB_AxisAlignedCapsule
	float	rlen = rsqrt(nw * nw + nFactor * nFactor);
	xAxisNorm *= nFactor * rlen;
	yAxisNorm *= nFactor * rlen;
	n *= rlen;
	n0 = n + xAxisNorm;
	n1 = n - xAxisNorm;
	n2 = n + xAxisNorm;
	n3 = n - xAxisNorm;
	n4 = n - yAxisNorm;
	n5 = n + yAxisNorm;
#else
	float	rlen = rsqrt(nw * nw + 2.0f * nFactor * nFactor);
	float3	xpy = (xAxisNorm + yAxisNorm) * nFactor * rlen;
	float3	xmy = (xAxisNorm - yAxisNorm) * nFactor * rlen;
	n *= rlen;
	n0 = n + xpy;
	n1 = n + xmy;
	n2 = n - xmy;
	n3 = n - xpy;
#endif
}

void	bb_billboardTangent(
	float nFactor,
	out float3 t0, out float3 t1, out float3 t2, out float3 t3,
#if BB_AxisAlignedCapsule
	out float3 t4, out float3 t5,
#endif
	in float3 xAxis, in float3 yAxis, in float3 nAxis)
{
	float	nw = (1.0f - nFactor); // weight
	float3	xAxisNorm = normalize(xAxis);
	float3	yAxisNorm = normalize(yAxis);
	float3	n = nAxis; // normal
	float3	t = -xAxisNorm * nw;

#if BB_AxisAlignedCapsule
	float	rlen = rsqrt(nw * nw + 2.0f * nFactor * nFactor);
	t *= rlen;
	n *= nFactor * rlen;
	xAxisNorm *= nFactor * rlen;
	yAxisNorm *= nFactor * rlen;
	t0 = t - n + yAxisNorm;
	t2 = t0;
	t1 = t + n + yAxisNorm;
	t3 = t1;
	t4 = t - n + xAxisNorm;
	t5 = t + n + xAxisNorm;
#else
	float	rlen = rsqrt(nw * nw + 3.0f * nFactor * nFactor);
	t *= rlen;
	n *= nFactor * rlen;
	xAxisNorm *= nFactor * rlen;
	yAxisNorm *= nFactor * rlen;
	t0 = t + n - xAxisNorm + yAxisNorm;
	t1 = t + n - xAxisNorm - yAxisNorm;
	t2 = t - n - xAxisNorm - yAxisNorm;
	t3 = t - n - xAxisNorm + yAxisNorm;
#endif
}

SVertexOutput	Billboard(in SPrimitives primitive, in int primitiveID)
{
	SVertexOutput o;

	uint	drId = asuint(primitive.VertexPosition.w);
	float2	billboardInfos = asfloat(_BillboardInfo.Load2(2 * 4 * drId));
	uint	billboardModeAndFlags = asuint(billboardInfos.x);
	float	normalBendingFactor = billboardInfos.y;

	// UV
#if defined(BB_Feature_Atlas) || defined(PK_HAS_ANIM_BLEND)
# define	setUV0(_uv, _value)	_uv = _value.xy;

# if PK_HAS_ANIM_BLEND
#  define	setUV1(_uv, _value)	_uv = _value.zw;
#  define	setFrameLerp(_frameLerp, _value)	_frameLerp = frac(_value);
# else
#  define	setUV1(_, _v)
#  define	setFrameLerp(_, _v)
# endif

	float4	c00 = float4(0, 0, 0, 0);
	float4	c01 = float4(0, 1, 0, 1);
	float4	c10 = float4(1, 0, 1, 0);
	float4	c11 = float4(1, 1, 1, 1);
	float	atlasID = 0.f;
	bb_billboardUV(primitive, billboardModeAndFlags, c00, c01, c10, c11, atlasID);
#else
# define	setUV0(_uv, _value)	_uv = _value;
# define	setUV1(_, _v)
# define	setFrameLerp(_, _v)
	float2	c00 = float2(0, 0);
	float2	c01 = float2(0, 1);
	float2	c10 = float2(1, 0);
	float2	c11 = float2(1, 1);
	float	atlasID = 0.f;
	bb_billboardUV(primitive, billboardModeAndFlags, c00, c01, c10, c11);
#endif

	// Axis y/x to create the quad + normal
	float3	center = primitive.VertexPosition.xyz;
	float3	xAxis = float3(0, 0, 0);
	float3	yAxis = float3(0, 0, 0);
	float3	nAxis = float3(0, 0, 0);

#if defined(BB_AxisAlignedCapsule)
	float3	upVec = float3(0, 0, 0);
	bb_VelocityCapsuleAlign(primitive, xAxis, yAxis, upVec, nAxis);
#else
	billboard_quad(primitive, billboardModeAndFlags & BB_Flag_BillboardMask, xAxis, yAxis, nAxis);
#endif

	float3	xpy = xAxis + yAxis;
	float3	xmy = xAxis - yAxis;

	// Normals
#if	PK_HAS_LIGHTING
# define	setNormal(_normal, _value)	_normal = float4(_value, 0.0);
	float3	n0, n1, n2, n3;
# if defined(BB_AxisAlignedCapsule)
	float3	n4, n5;
	bb_billboardNormal(normalBendingFactor, n0, n1, n2, n3, n4, n5, xAxis, yAxis, nAxis);
# else
	bb_billboardNormal(normalBendingFactor, n0, n1, n2, n3, xAxis, yAxis, nAxis);
# endif
#else
# define	setNormal(_, _v)
#endif

	// Tangent
#if PK_HAS_LIGHTING // Disabled until needed
# define	setTangent(_tangent, _value)	_tangent = _value;
	float3	t0, t1, t2, t3;
# if defined(BB_AxisAlignedCapsule)
	float3	t4, t5;
	bb_billboardTangent(normalBendingFactor, t0, t1, t2, t3, t4, t5, xAxis, yAxis, nAxis);
# else
	bb_billboardTangent(normalBendingFactor, t0, t1, t2, t3, xAxis, yAxis, nAxis);
# endif
#else
# define	setTangent(_, _v)
#endif

	// Emit 4 / 6 vertex

#define emittr(_pos, _uv, _normal, _tangent)	\
	o.vertex = float4(_pos, 1.0f); \
	o.ProjPos = proj_position(_pos); \
	setUV0(o.UV0, _uv) \
	setUV1(o.UV1, _uv) \
	setFrameLerp(o.FrameLerp, atlasID) \
	setNormal(o.Normal, _normal) \
	setTangent(o.Tangent, _tangent)

#if defined(BB_AxisAlignedCapsule)
	switch (primitiveID)
	{
		default:
		case 0:
			emittr(center + yAxis + upVec, c10, n5, t5);
			break;
		case 2:
		case 3:
		case 6:
			emittr(center + xpy, c11, n0, t0); // +x+y
			break;
		case 1:
		case 4:
			emittr(center - xmy, c00, n1, t1); // -x+y
			break;
		case 8:
		case 9:
			emittr(center + xmy, c11, n2, t2); // +x-y
			break;
		case 5:
		case 7:
		case 10:
			emittr(center - xpy, c00, n3, t3); // -x-y
			break;
		case 11:
			emittr(center - yAxis - upVec, c01, n4, t4);
			break;
	}
#else
	switch (primitiveID)
	{
		default:
		case 0:
			emittr(center + xpy, c11, n0, t0); // +x+y
			break;
		case 2:
		case 3:
			emittr(center + xmy, c10, n1, t0); // +x-y
			break;
		case 1:
		case 4:
			emittr(center - xmy, c01, n2, t0); // -x+y
			break;
		case 5:
			emittr(center - xpy, c00, n3, t0); // -x-y
			break;
	}
#endif
	return o;
}

SVertexOutput vertBillboardingImplem(uint vidx)
{
#if defined(BB_AxisAlignedCapsule)
	const uint particleIdxIdx = vidx / 12;
	const uint primitiveIdx = vidx % 12;
#else
	const uint particleIdxIdx = vidx / 6;
	const uint primitiveIdx = vidx % 6;
#endif

	uint particleIdx = asuint(_In_Indexes.Load(4 * particleIdxIdx));

	SPrimitives	primitive;

	// Extract position:
	uint	positionOffset = asuint(_InputOffsets.Load(GenInput_GeomPosition * 4));
	primitive.VertexPosition = asfloat(_In_RawData.Load4(4 * (positionOffset + 4 * particleIdx)));

	// Extract size:
	uint	sizeOffset = asuint(_InputOffsets.Load(GenInput_GeomSize * 4));
#if		defined(BB_SizeFloat2)
	primitive.Size = asfloat(_In_RawData.Load2(4 * (sizeOffset + 2 * particleIdx)));
#else
	primitive.Size = asfloat(_In_RawData.Load(4 * (sizeOffset + 1 * particleIdx)));
#endif

	// Extract rotation:
	uint rotationOffset = asuint(_InputOffsets.Load(GenInput_GeomRotation * 4));
	if (rotationOffset != 0)
		primitive.Rotation = asfloat(_In_RawData.Load(4 * (rotationOffset + 1 * particleIdx)));
	else
		primitive.Rotation = 0.0f;

#if		(BB_FeatureLevel > 0)
	uint	axis0Offset = asuint(_InputOffsets.Load(GenInput_GeomAxis0 * 4));
	primitive.Axis0 = asfloat(_In_RawData.Load3(4 * (axis0Offset + 3 * particleIdx)));
#endif
#if		(BB_FeatureLevel > 1)
	uint	axis1Offset = asuint(_InputOffsets.Load(GenInput_GeomAxis1 * 4));
	primitive.Axis1 = asfloat(_In_RawData.Load3(4 * (axis1Offset + 3 * particleIdx)));
#endif

#if defined(BB_Feature_Atlas) || defined(PK_HAS_ANIM_BLEND)
	uint	textureIdOffset = asuint(_InputOffsets.Load(GenInput_TextureID * 4));
	primitive.TextureID = asfloat(_In_RawData.Load(4 * (textureIdOffset + 1 * particleIdx)));
#endif

	SVertexOutput o = Billboard(primitive, primitiveIdx);

	// Additional elements
#if	PK_HAS_COLOR
	uint	colorOffset = asuint(_InputOffsets.Load(GenInput_Color * 4));
	o.Color = asfloat(_In_RawData.Load4(4 * (colorOffset + 4 * particleIdx)));
#endif
#if	PK_HAS_ALPHA_REMAP
	uint	alphaCursorOffset = asuint(_InputOffsets.Load(GenInput_AlphaCursor * 4));
	o.AlphaCursor = asfloat(_In_RawData.Load(4 * (colorOffset + 1 * particleIdx)));
#endif
	return o;
}