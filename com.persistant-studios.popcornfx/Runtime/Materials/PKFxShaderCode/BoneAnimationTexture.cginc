
#define	PK_GLUE(_a, _b)		_a ## _b

#if		!defined(PK_UNITY_SHADER_LAB)
#	define	PK_SAMPLE_TEXTURE2D_LOD(_TexName, _UVs, _LOD) SAMPLE_TEXTURE2D_LOD(UnityBuildTexture2DStructNoScale(_TexName).tex,  UnityBuildSamplerStateStruct(PK_GLUE(sampler, _TexName)).samplerstate, float2((_UVs).x, 1.0f - (_UVs).y), 0.);
#else
	sampler2D _SkeletalAnimTexture;
#	define	PK_SAMPLE_TEXTURE2D_LOD(_TexName, _UVs, _LOD) tex2Dlod(_TexName,  float4(_UVs, 0, _LOD));
#endif

#	define BUILD_MAT4(_x, _y, _z, _w)	transpose(float4x4(_x, _y, _z, _w))

#define VEC4_ZERO 	float4(0.0f, 0.0f, 0.0f, 0.0f)

struct 	SBoneTransform
{
	float4 	m_Rotation;
	float3 	m_Position;
#if 	defined(PK_HAS_SKELETAL_ANIM_BONES_SCALE)
	float3	m_Scale;
#endif
};

#if 	defined(PK_HAS_SKELETAL_ANIM_BONES_SCALE)
#	define PIXELS_PER_BONE 	3.0f
#else
#	define PIXELS_PER_BONE 	2.0f
#endif

SBoneTransform  LerpBoneTransforms(SBoneTransform a, SBoneTransform b, float v)
{
	SBoneTransform 	c;
	float4 			rotationB = b.m_Rotation;

	if (dot(a.m_Rotation, b.m_Rotation) < 0.0f)
		rotationB *= -1.0f;
	c.m_Rotation = normalize(lerp(a.m_Rotation, rotationB, v));
	c.m_Position = lerp(a.m_Position, b.m_Position, v);
#if 	defined(PK_HAS_SKELETAL_ANIM_BONES_SCALE)
	c.m_Scale = lerp(a.m_Scale, b.m_Scale, v);
#endif
	return c;
}

float3 RemapPositionsFromUnormToBounds(float3 value)
{
	float3 	minBounds = _SkeletalAnimTranslationBoundsMin;
	float3 	maxBounds = _SkeletalAnimTranslationBoundsMax;
	float3 	range = maxBounds - minBounds;
	return minBounds + value * range;
}

#if 	defined(PK_HAS_SKELETAL_ANIM_BONES_SCALE)
float3 RemapScalesFromUnormToBounds(float3 value)
{
	float3 	minBounds = _SkeletalAnimScaleBoundsMin;
	float3 	maxBounds = _SkeletalAnimScaleBoundsMax;
	float3 	range = maxBounds - minBounds;
	return minBounds + value * range;
}
#endif

SBoneTransform 	ReadBoneTransforms(int animationId, float boneId, float animationCursor)
{
	// Texture info:
	float2 animTexResol = _SkeletalAnimTextureResol;
	float2 pxlUvSize = 1.0f / animTexResol;
	float2 offsetToBottomPixel = float2(0.0f, pxlUvSize.y);
	// Remap anim cursor to start the sampling in the middle of the first pixel and finish in the middle of the last one:
	float animCursorToUvRange = animationCursor * (1.0f - pxlUvSize.x);
	float animCursor = animCursorToUvRange + pxlUvSize.x * 0.5;
	float animCursorToPrevPixel = fmod(animCursorToUvRange, pxlUvSize.x);
	float snappedAnimCursor = (animCursorToUvRange - animCursorToPrevPixel) + pxlUvSize.x * 0.5f;
	// UV offset to read from correct animation index:
	float animCount = float(_SkeletalAnimCount);
	float animationUVOffset = ((animTexResol.y / animCount) * pxlUvSize.y) * float(animationId);
	// Start pixel to read from:
	float2 animUV = float2(snappedAnimCursor, boneId * PIXELS_PER_BONE * pxlUvSize.y + animationUVOffset);
	SBoneTransform	boneTransform;

	// Sample the texture:
	float4 position = PK_SAMPLE_TEXTURE2D_LOD(_SkeletalAnimTexture, animUV + offsetToBottomPixel * 0.5f, 0);
	float4 rotation = PK_SAMPLE_TEXTURE2D_LOD(_SkeletalAnimTexture, animUV + offsetToBottomPixel * 1.5f, 0);
	boneTransform.m_Rotation = normalize(rotation * 2.0f - 1.0f);
	boneTransform.m_Position = RemapPositionsFromUnormToBounds(position.xyz);
#if 	defined(PK_HAS_SKELETAL_ANIM_BONES_SCALE)
	float4 scale = PK_SAMPLE_TEXTURE2D_LOD(_SkeletalAnimTexture, animUV + offsetToBottomPixel * 2.5f, 0);
	boneTransform.m_Scale = RemapScalesFromUnormToBounds(scale.xyz);
#endif

#if 	defined(PK_HAS_SKELETAL_ANIM_INTERPOL)
	SBoneTransform	boneTransform1;

	float snappedAnimCursor1 = (animCursorToUvRange + (pxlUvSize.x - animCursorToPrevPixel)) + pxlUvSize.x * 0.5f;
	float currentLerpRatio = animCursorToPrevPixel / pxlUvSize.x;
	float2 animUV1 = float2(snappedAnimCursor1, float(boneId) * PIXELS_PER_BONE * pxlUvSize.y + animationUVOffset);

	// Sample the texture:
	float4 position1 = PK_SAMPLE_TEXTURE2D_LOD(_SkeletalAnimTexture, animUV1 + offsetToBottomPixel * 0.5f, 0);
	float4 rotation1 = PK_SAMPLE_TEXTURE2D_LOD(_SkeletalAnimTexture, animUV1 + offsetToBottomPixel * 1.5f, 0);
	boneTransform1.m_Rotation = normalize(rotation1 * 2.0f - 1.0f);
	boneTransform1.m_Position = RemapPositionsFromUnormToBounds(position1.xyz);
#if 	defined(PK_HAS_SKELETAL_ANIM_BONES_SCALE)
	float4 scale1 = PK_SAMPLE_TEXTURE2D_LOD(_SkeletalAnimTexture, animUV1 + offsetToBottomPixel * 2.5f, 0);
	boneTransform1.m_Scale = RemapScalesFromUnormToBounds(scale1.xyz);
#endif
	boneTransform = LerpBoneTransforms(boneTransform, boneTransform1, currentLerpRatio);
#endif
	return boneTransform;
}

float RemapValue(float value, float min1, float max1, float min2, float max2)
{
	float 	value_01 = (value - min1) / (max1 - min1);
	return value_01 * (max2 - min2) + min2;
}

float4x4 QuatToMat4(float4 quat)
{
    float4x4 m = float4x4(VEC4_ZERO, VEC4_ZERO, VEC4_ZERO, VEC4_ZERO);
    float x = quat.x, y = quat.y, z = quat.z, w = quat.w;
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    m[0][0] = 1.0 - (yy + zz);
    m[1][0] = xy - wz;
    m[2][0] = xz + wy;

    m[0][1] = xy + wz;
    m[1][1] = 1.0 - (xx + zz);
    m[2][1] = yz - wx;

    m[0][2] = xz - wy;
    m[1][2] = yz + wx;
    m[2][2] = 1.0 - (xx + yy);

    m[3][3] = 1.0;
    return m;
}

float4x4  	BoneTransformToMatrix(SBoneTransform tr)
{
	float4x4 res = QuatToMat4(tr.m_Rotation);
	res[3] = float4(tr.m_Position, 1.0f);
#if 	defined(PK_HAS_SKELETAL_ANIM_BONES_SCALE)
	res[0].xyz *= tr.m_Scale;
	res[1].xyz *= tr.m_Scale;
	res[2].xyz *= tr.m_Scale;
#endif
	return BUILD_MAT4(res[0], res[1], res[2], res[3]);
}

float4x4 	LerpMatrix(float4x4 m0, float4x4 m1, float ratio)
{
	float4x4 	res;
	
	res[0] = lerp(m0[0], m1[0], ratio);
	res[1] = lerp(m0[1], m1[1], ratio);
	res[2] = lerp(m0[2], m1[2], ratio);
	res[3] = lerp(m0[3], m1[3], ratio);
	return res;
}

void BoneAnimationTexture_float(float4 inBoneIds, float4 inBoneWeights, float3 inPosition, float3 inNormal, float4 inTangent,
								float inCurrentAnimTrack, float inAnimationCursor, float inNextAnimTrack, float inNextCursor, float inTransitionRatio,
								out float3 outPosition, out float3 outNormal, out float3 outTangent)
{
	float4x4 animTr = BUILD_MAT4(VEC4_ZERO, VEC4_ZERO, VEC4_ZERO, VEC4_ZERO);

	int boneIdx = 0;
	while (boneIdx < 4)
	{

		SBoneTransform boneTr;
		
		boneTr = ReadBoneTransforms(inCurrentAnimTrack,
									inBoneIds[boneIdx],
									inAnimationCursor);
		float4x4 boneMat = BoneTransformToMatrix(boneTr);
#	if defined(PK_HAS_SKELETAL_ANIM_INTERPOL_TRACKS)
		SBoneTransform boneTr2 = ReadBoneTransforms(inNextAnimTrack,
													inBoneIds[boneIdx],
													inNextCursor);
		float4x4 boneMat2 = BoneTransformToMatrix(boneTr2);
		boneMat = LerpMatrix(boneMat, boneMat2, inTransitionRatio);
#	endif
		animTr[0] += boneMat[0] * inBoneWeights[boneIdx];
		animTr[1] += boneMat[1] * inBoneWeights[boneIdx];
		animTr[2] += boneMat[2] * inBoneWeights[boneIdx];
		animTr[3] += boneMat[3] * inBoneWeights[boneIdx];
		++boneIdx;
	}

	float4x4 	finalTransform = animTr;
	const float4x4	UnityToLHZ = float4x4(	float4(1, 0, 0, 0),
											float4(0, 0, 1, 0),
											float4(0, -1, 0, 0),
											float4(0, 0, 0, 1));
	const float4x4	LHZToUnity = float4x4(	float4(1, 0, 0, 0),
											float4(0, 0, -1, 0),
											float4(0, 1, 0, 0),
											float4(0, 0, 0, 1));
	finalTransform = mul(finalTransform, LHZToUnity);
	finalTransform = mul(UnityToLHZ, finalTransform);
	outPosition = mul(finalTransform, float4(inPosition, 1.0f)).xyz;
	outNormal = mul(finalTransform, float4(inNormal, 0.0f)).xyz;
	outTangent = mul(finalTransform, float4(inTangent.xyz, 0.0f)).xyz * inTangent.w;
}