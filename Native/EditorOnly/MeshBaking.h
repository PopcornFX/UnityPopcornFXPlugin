//--------------------------------------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//--------------------------------------------------------------------------------------------------------
#pragma once
#include <pk_kernel/include/kr_threads.h>
#include <pk_geometrics/include/ge_mesh.h>
#include <pk_geometrics/include/ge_mesh_resource.h>
#include <pk_particles_toolbox/include/pt_mesh_deformers_skin.h>

struct SMeshDataToFill;

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CMeshBaker : public CRefCountedObject
{
public:
	CMeshBaker();
	~CMeshBaker();

	void				Clear();
	bool				NextSubmesh(const CString &name, int vertexAttributes);
	bool				AppendMeshToBake(SMeshDataToFill *meshToBake, const CFloat4x4 &meshTransform); // transfer ownership of meshToBake pointer
	bool				BakeToFile(const char *outputMeshPath);
	bool				SavePkmm(const CString &pkmmVirtualPath, PFilePack pack = null);

private:
	bool				_AddSubmeshIFP();

	TBaseSkinningStreamsProxy<u16>	m_SkinCache;
	CMeshTriangleBatch				m_Cache;

	u32							m_ConcatenatedMeshCount;
	u32							m_SubMeshCount;

	struct	SSubMeshData
	{
		void		*m_MeshDataToFillBuffer;
		u16			*m_IndicesSkinBuffer;

		SSubMeshData();
		~SSubMeshData();
		void		Clean();
	};

	CString						m_CurrentSubmeshName;
	CMeshTriangleBatch			m_CurrentSubmesh;

	CResourceMesh				m_Mesh;
	TArray<SSubMeshData>		m_AllMeshesData;
};
PK_DECLARE_REFPTRCLASS(MeshBaker);

//----------------------------------------------------------------------------
__PK_API_END
