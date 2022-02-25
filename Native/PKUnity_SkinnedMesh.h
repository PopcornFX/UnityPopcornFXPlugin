//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_geometrics/include/ge_mesh_resource.h>
#include <pk_particles_toolbox/include/pt_mesh_deformers_skin.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CPkFxSkinnedMesh : public CRefCountedObject
{
public:
	CPkFxSkinnedMesh()
		: m_SkinningStreams(null)
		, m_OwnSkinningStreams(false)
		, m_SkeletonView(null)
	{
	}
	~CPkFxSkinnedMesh()
	{
		if (m_OwnSkinningStreams)
			PK_DELETE(m_SkinningStreams);
		PK_DELETE(m_SkeletonView);
	}

	bool		Init(	u32 samplerUsage,
						const SMeshProxy &mesh,
						const float *vtxBonesWeights,
						const u32 *vtxBonesIndices,
						u32 vertexCount,
						u32 influenceCount,
						u32 bonesCount);
	bool		Init(u32 samplerUsage, const SMeshProxy &mesh, CBaseSkinningStreams *skinStreams, int bonesCount, bool ownSkinningStreams = false);
	bool		Init(u32 samplerUsage, CMeshTriangleBatch &triBatch, CBaseSkinningStreams *skinStreams, int bonesCount, bool ownSkinningStreams = false);
	void		BeginUpdateSkinning(float dt);
	void		EndUpdateSkinning();

	CFloat4x4	*GetSkeletonMatrices();

	TStridedMemoryView<CFloat3>	&Velocities() { return m_Velocities; }
	SSamplerSourceOverride		*Override() { return &m_Override; }

private:

	bool	_Init(	u32 samplerUsage,
					u32 vertexCount,
					const TStridedMemoryView<const CFloat3>	&srcPositions,
					const TStridedMemoryView<const CFloat3>	&srcNormals,
					const TStridedMemoryView<const CFloat4>	&srcTangents,
					CBaseSkinningStreams *skinStreams,
					int bonesCount,
					bool ownSkinningStreams = false);

	void	_Skin_PreProcess(u32 vertexStart, u32 vertexCount, const SSkinContext &ctx);
	void	_Skin_PostProcess(u32 vertexStart, u32 vertexCount, const SSkinContext &ctx);

	SSamplerSourceOverride					m_Override;

	TArray<CFloat4, TArrayAligned16>		m_SrcPositionsData;
	TStridedMemoryView<const CFloat3>		m_SrcPositions;
	TArray<CFloat4, TArrayAligned16>		m_DstPositionsData;
	TStridedMemoryView<CFloat3>				m_DstPositions;

	TArray<CFloat4, TArrayAligned16>		m_OldPositionsData;
	TStridedMemoryView<CFloat3>				m_OldPositions;

	TArray<CFloat4, TArrayAligned16>		m_VelocitiesData;
	TStridedMemoryView<CFloat3>				m_Velocities;

	TArray<CFloat4, TArrayAligned16>		m_SrcNormalsData;
	TStridedMemoryView<const CFloat3>		m_SrcNormals;
	TArray<CFloat4, TArrayAligned16>		m_DstNormalsData;
	TStridedMemoryView<CFloat3>				m_DstNormals;

	TStridedMemoryView<const CFloat4>		m_SrcTangents;
	TArray<CFloat4, TArrayAligned16>		m_DstTangentsData;
	TStridedMemoryView<CFloat4>				m_DstTangents;

	CBaseSkinningStreams					*m_SkinningStreams;
	bool									m_OwnSkinningStreams;
	TArray<CFloat4x4, TArrayAligned16>		m_BindPoseMatrices;
	TArray<CFloat4x4, TArrayAligned16>		m_BoneMatrices;
	TArray<CFloat4x4, TArrayAligned16>		m_BoneInverseMatrices;
	float									m_SkinDt;
	CSkeletonView							*m_SkeletonView;
	CSkinAsyncContext						m_AsyncSkinContext;
	SSkinContext							m_SkinContext;

	bool									m_FirstFrameAfterLoad;
};
PK_DECLARE_REFPTRCLASS(PkFxSkinnedMesh);

__PK_API_END
