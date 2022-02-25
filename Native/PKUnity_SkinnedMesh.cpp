//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "PKUnity_SkinnedMesh.h"

#include "ManagedToNative.h"

#include <pk_particles/include/ps_declaration.h>
#include <pk_geometrics/include/ge_mesh.h>
#include <pk_maths/include/pk_maths_simd.h>
#include <pk_kernel/include/kr_memoryviews_utils.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

bool	CPkFxSkinnedMesh::Init(	u32 samplerUsage,
								const SMeshProxy &mesh,
								const float *vtxBonesWeights,
								const u32 *vtxBonesIndices,
								u32 vertexCount,
								u32 influenceCount,
								u32 bonesCount)
{
	TMemoryView<const float>	bonesWeightsView(reinterpret_cast<const float*>(vtxBonesWeights), vertexCount * influenceCount);
	TMemoryView<const u32>		bonesIndicesView(reinterpret_cast<const u32*>(vtxBonesIndices), vertexCount * influenceCount);

	CBaseSkinningStreams	*skinningStreams = CBaseSkinningStreams::BuildFromUnpackedStreams(vertexCount, bonesWeightsView, bonesIndicesView, TMemoryView<const u32>(), bonesCount);
	if (!PK_VERIFY(skinningStreams != null))
		return false;
	return Init(samplerUsage, mesh, skinningStreams, bonesCount, true);
}

//----------------------------------------------------------------------------

bool	CPkFxSkinnedMesh::Init(u32 samplerUsage, const SMeshProxy &mesh, CBaseSkinningStreams *skinStreams, int bonesCount, bool ownSkinningStreams)
{
	u32					vertexCount = mesh.m_VertexCount;

	const TStridedMemoryView<const CFloat3>	srcPositions = mesh.m_Positions;
	const TStridedMemoryView<const CFloat3>	srcNormals = mesh.m_Normals;
	const TStridedMemoryView<const CFloat4>	srcTangents = mesh.m_Tangents;
	return _Init(samplerUsage, vertexCount, srcPositions, srcNormals, srcTangents, skinStreams, bonesCount, ownSkinningStreams);
}

//----------------------------------------------------------------------------

bool	CPkFxSkinnedMesh::Init(u32 samplerUsage, CMeshTriangleBatch &triBatch, CBaseSkinningStreams *skinStreams, int bonesCount, bool ownSkinningStreams)
{
	CMeshVStream		&vstream = triBatch.m_VStream;
	u32					vertexCount = vstream.VertexCount();

	const TStridedMemoryView<const CFloat3>	srcPositions = vstream.Positions();
	const TStridedMemoryView<const CFloat3>	srcNormals = vstream.Normals();
	const TStridedMemoryView<const CFloat4>	srcTangents = vstream.Tangents();
	return _Init(samplerUsage, vertexCount, srcPositions, srcNormals, srcTangents, skinStreams, bonesCount, ownSkinningStreams);
}

//----------------------------------------------------------------------------

void	CPkFxSkinnedMesh::BeginUpdateSkinning(float dt)
{
	if (!m_VelocitiesData.Empty())
	{
		m_SkinDt = dt;

		// hook our pre-skin callback where we'll copy the positions to m_OldPositions so that the skin job can correctly
		// differentiate the two and compute the instantaneous mesh surface velocities using 'asyncSkinContext.m_SrcDt'
		m_SkinContext.m_CustomProcess_PreSkin = SSkinContext::CbCustomProcess(this, &CPkFxSkinnedMesh::_Skin_PreProcess);
		m_SkinContext.m_CustomProcess_PostSkin = SSkinContext::CbCustomProcess(this, &CPkFxSkinnedMesh::_Skin_PostProcess);
	}

	// Launch skinning tasks
	CSkeletalSkinnerSimple::AsyncSkinStart(m_AsyncSkinContext, *m_SkeletonView, m_SkinContext);
}

//----------------------------------------------------------------------------

void	CPkFxSkinnedMesh::EndUpdateSkinning()
{
	// Wait for skinning tasks to end
	CSkeletalSkinnerSimple::AsyncSkinWait(m_AsyncSkinContext, null);

//	if (!m_VelocitiesData.Empty() && m_FirstFrameAfterLoad)
//	{
//		Mem::Clear(m_Velocities.Data(), m_Velocities.CoveredBytes());
		m_FirstFrameAfterLoad = false;
//	}
}

//----------------------------------------------------------------------------

CFloat4x4	*CPkFxSkinnedMesh::GetSkeletonMatrices()
{
	return m_BoneInverseMatrices.RawDataPointer();
}

//----------------------------------------------------------------------------

bool		CPkFxSkinnedMesh::_Init(u32 samplerUsage,
									u32 vertexCount,
									const TStridedMemoryView<const CFloat3>	&srcPositions,
									const TStridedMemoryView<const CFloat3>	&srcNormals,
									const TStridedMemoryView<const CFloat4>	&srcTangents,
									CBaseSkinningStreams *skinStreams,
									int bonesCount,
									bool ownSkinningStreams)
{
	if (!PK_VERIFY(m_DstPositionsData.Resize(vertexCount)) ||
		!PK_VERIFY(m_OldPositionsData.Resize(vertexCount)))
		return false;

	m_DstPositions = TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_DstPositionsData.RawDataPointer()), m_DstPositionsData.Count(), 0x10);
	m_OldPositions = TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_OldPositionsData.RawDataPointer()), m_OldPositionsData.Count(), 0x10);

	Mem::CopyStreamToStream(m_DstPositions, srcPositions);

	if (Mem::IsAligned<0x10>(srcPositions.Data()) && srcPositions.Stride() == 0x10)
		m_SrcPositions = srcPositions;
	else
	{
		if (!PK_VERIFY(m_SrcPositionsData.Resize(vertexCount)))
			return false;
		TStridedMemoryView<CFloat3> srcPosData = TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_SrcPositionsData.RawDataPointer()), m_SrcPositionsData.Count(), 0x10);
		Mem::CopyStreamToStream(srcPosData, srcPositions);
		m_SrcPositions = srcPosData;
	}

	// No need to copy or clear the velocities:
	if (samplerUsage & SParticleDeclaration::SSampler::UsageFlags_Mesh_Velocity)
	{
		if (!PK_VERIFY(m_VelocitiesData.Resize(vertexCount)))
			return false;
		m_Velocities = TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_VelocitiesData.RawDataPointer()), m_VelocitiesData.Count(), 0x10);
	}
	else
	{
		m_VelocitiesData.Clear();
		m_Velocities = TStridedMemoryView<CFloat3>();
	}

	if (samplerUsage & SParticleDeclaration::SSampler::UsageFlags_Mesh_Normal)
	{
		if (!PK_VERIFY(m_DstNormalsData.Resize(vertexCount)))
			return false;
		m_DstNormals = TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_DstNormalsData.RawDataPointer()), m_DstNormalsData.Count(), 0x10);
		Mem::CopyStreamToStream(m_DstNormals, srcNormals);

		if (Mem::IsAligned<0x10>(srcNormals.Data()) && srcNormals.Stride() == 0x10)
			m_SrcNormals = srcNormals;
		else
		{
			if (!PK_VERIFY(m_SrcNormalsData.Resize(vertexCount)))
				return false;
			TStridedMemoryView<CFloat3> srcNormData = TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_SrcNormalsData.RawDataPointer()), m_SrcNormalsData.Count(), 0x10);
			Mem::CopyStreamToStream(srcNormData, srcNormals);
			m_SrcPositions = srcNormData;
		}
	}
	else
	{
		m_DstNormalsData.Clear();
		m_DstNormals = TStridedMemoryView<CFloat3>();
	}

	if (samplerUsage & SParticleDeclaration::SSampler::UsageFlags_Mesh_Tangent)
	{
		if (!PK_VERIFY(m_DstTangentsData.Resize(vertexCount)))
			return false;
		m_SrcTangents = srcTangents;
		m_DstTangents = TStridedMemoryView<CFloat4>(m_DstTangentsData.RawDataPointer(), m_DstTangentsData.Count(), 0x10);
		Mem::CopyStreamToStream(m_DstTangents, srcTangents);
	}
	else
	{
		m_DstTangentsData.Clear();
		m_DstTangents = TStridedMemoryView<CFloat4>();
	}

	m_SkinningStreams = skinStreams;
	if (!PK_VERIFY(m_SkinningStreams != null))
		return false;
	m_OwnSkinningStreams = ownSkinningStreams;

	// init skeleton
	if (!PK_VERIFY(m_BoneMatrices.Resize(bonesCount)) || !PK_VERIFY(m_BoneInverseMatrices.Resize(bonesCount)))
		return false;

	m_SkeletonView = PK_NEW(CSkeletonView(	bonesCount,
											m_BoneMatrices.RawDataPointer(),
											m_BoneInverseMatrices.RawDataPointer()));
	if (!PK_VERIFY(m_SkeletonView != null))
		return false;

	//init skin context
	m_SkinContext.m_SkinningStreams = m_SkinningStreams;
	m_SkinContext.m_SrcPositions = m_SrcPositions;
	m_SkinContext.m_DstPositions = m_DstPositions;

	m_SkinContext.m_SrcNormals = m_SrcNormals;
	m_SkinContext.m_DstNormals = m_DstNormals;

	m_SkinContext.m_SrcTangents = TStridedMemoryView<const CFloat3>(reinterpret_cast<const CFloat3*>(m_SrcTangents.Data()), m_SrcTangents.Count(), 0x10);
	m_SkinContext.m_DstTangents = m_DstTangents;

	m_Override.m_PositionsOverride = m_DstPositions;
	m_Override.m_VelocitiesOverride = m_Velocities;
	m_Override.m_NormalsOverride = m_DstNormals;
	m_Override.m_TangentsOverride = m_DstTangents;

	m_FirstFrameAfterLoad = true;
	return true;
}

//----------------------------------------------------------------------------

void	CPkFxSkinnedMesh::_Skin_PreProcess(u32 vertexStart, u32 vertexCount, const SSkinContext &ctx)
{
	(void)ctx;
	TStridedMemoryView<const CFloat3>	src = m_DstPositions.Slice(vertexStart, vertexCount);
	TStridedMemoryView<CFloat3>			dst = m_OldPositions.Slice(vertexStart, vertexCount);

	PK_ASSERT(src.Stride() == 0x10 && dst.Stride() == 0x10);
	Mem::Copy(dst.Data(), src.Data(), dst.Count() * dst.Stride());
}

//----------------------------------------------------------------------------

void	CPkFxSkinnedMesh::_Skin_PostProcess(u32 vertexStart, u32 vertexCount, const SSkinContext &ctx)
{
	// compute instantaneous surface velocities IFN:
	TStridedMemoryView<const CFloat3>	posCur = ctx.m_DstPositions.Slice(vertexStart, vertexCount);
	TStridedMemoryView<const CFloat3>	posOld = m_OldPositions.Slice(vertexStart, vertexCount);
	TStridedMemoryView<CFloat3>			vel = m_Velocities.Slice(vertexStart, vertexCount);
	const float							dt = m_SkinDt;

	// NOTE(Julien): switched 'const bool' to 'volatile bool' to silence annoying constant conditional expression compiler warnings until this is properly set
	volatile bool	continuousAnim = true;//(m_Skeleton != null) && (m_Skeleton->LastUpdateFrameID() == m_LastSkinnedSkeletonFrameID + 1);

	if (!continuousAnim && !vel.Empty())
	{
		Mem::Clear(vel.Data(), vel.CoveredBytes());
	}
	else if (!posCur.Empty() && !posOld.Empty() && !vel.Empty())
	{
		PK_ASSERT(posCur.Stride() == 0x10);
		PK_ASSERT(posOld.Stride() == 0x10);
		PK_ASSERT(vel.Stride() == 0x10);

		CFloat3			* __restrict dstVel = vel.Data();
		const CFloat3	*dstVelEnd = Mem::AdvanceRawPointer(dstVel, vertexCount * 0x10);
		const CFloat3	*srcPosCur = posCur.Data();
		const CFloat3	*srcPosOld = posOld.Data();

		PK_ASSERT(Mem::IsAligned<0x10>(dstVel));
		PK_ASSERT(Mem::IsAligned<0x10>(dstVelEnd));
		PK_ASSERT(Mem::IsAligned<0x10>(srcPosCur));
		PK_ASSERT(Mem::IsAligned<0x10>(srcPosOld));

		const SIMD::Float4	invDt = SIMD::Float4(1.0f / dt);
		dstVelEnd = Mem::AdvanceRawPointer(dstVelEnd, -0x40);
		while (dstVel < dstVelEnd)
		{
			const SIMD::Float4	pA0 = SIMD::Float4::LoadAligned16(srcPosOld, 0x00);
			const SIMD::Float4	pA1 = SIMD::Float4::LoadAligned16(srcPosOld, 0x10);
			const SIMD::Float4	pA2 = SIMD::Float4::LoadAligned16(srcPosOld, 0x20);
			const SIMD::Float4	pA3 = SIMD::Float4::LoadAligned16(srcPosOld, 0x30);
			const SIMD::Float4	pB0 = SIMD::Float4::LoadAligned16(srcPosCur, 0x00);
			const SIMD::Float4	pB1 = SIMD::Float4::LoadAligned16(srcPosCur, 0x10);
			const SIMD::Float4	pB2 = SIMD::Float4::LoadAligned16(srcPosCur, 0x20);
			const SIMD::Float4	pB3 = SIMD::Float4::LoadAligned16(srcPosCur, 0x30);
			const SIMD::Float4	v0 = (pB0 - pA0) * invDt;
			const SIMD::Float4	v1 = (pB1 - pA1) * invDt;
			const SIMD::Float4	v2 = (pB2 - pA2) * invDt;
			const SIMD::Float4	v3 = (pB3 - pA3) * invDt;
			v0.StoreAligned16(dstVel, 0x00);
			v1.StoreAligned16(dstVel, 0x10);
			v2.StoreAligned16(dstVel, 0x20);
			v3.StoreAligned16(dstVel, 0x30);

			dstVel = Mem::AdvanceRawPointer(dstVel, 0x40);
			srcPosCur = Mem::AdvanceRawPointer(srcPosCur, 0x40);
			srcPosOld = Mem::AdvanceRawPointer(srcPosOld, 0x40);
		}
		dstVelEnd = Mem::AdvanceRawPointer(dstVelEnd, +0x40);

		while (dstVel < dstVelEnd)
		{
			const SIMD::Float4	pA = SIMD::Float4::LoadAligned16(srcPosOld);
			const SIMD::Float4	pB = SIMD::Float4::LoadAligned16(srcPosCur);
			const SIMD::Float4	v = (pB - pA) * invDt;
			v.StoreAligned16(dstVel);

			dstVel = Mem::AdvanceRawPointer(dstVel, 0x10);
			srcPosCur = Mem::AdvanceRawPointer(srcPosCur, 0x10);
			srcPosOld = Mem::AdvanceRawPointer(srcPosOld, 0x10);
		}
	}
}

//----------------------------------------------------------------------------
__PK_API_END
