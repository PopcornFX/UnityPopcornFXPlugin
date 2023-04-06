//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "EditorManagedToNative.h"
#include "ManagedToNative.h"
#include "MeshBaking.h"

#include <pk_geometrics/include/ge_mesh_kdtree.h>
#include <pk_geometrics/include/ge_mesh_sampler_accel.h>

__PK_API_BEGIN

//----------------------------------------------------------------------------
//
//	CPKFXMeshBaker
//
//----------------------------------------------------------------------------

CMeshBaker::CMeshBaker()
:	m_ConcatenatedMeshCount(0)
,	m_SubMeshCount(0)
{
}

//----------------------------------------------------------------------------

CMeshBaker::~CMeshBaker()
{
}

//----------------------------------------------------------------------------

void	CMeshBaker::Clear()
{
	m_ConcatenatedMeshCount = 0;
	m_SubMeshCount = 0;
	m_CurrentSubmeshName.Clear();
	m_CurrentSubmesh.Clear();
	Mem::Reinit(m_Mesh);
	// Free all meshes data:
	for (u32 i = 0; i < m_AllMeshesData.Count(); ++i)
		m_AllMeshesData[i].Clean();
	m_AllMeshesData.Clear();
}

//----------------------------------------------------------------------------

bool	CMeshBaker::NextSubmesh(const CString &name, int vertexAttributes)
{
	if (!_AddSubmeshIFP())
		return false;

	SVertexDeclaration	decl;
	decl.AddStreamCodeIFN(SVStreamCode(CVStreamSemanticDictionnary::Ordinal_Position, SVStreamCode::Element_Float3, SVStreamCode::Mask_SIMDStride));
	if (vertexAttributes & (1 << Attrib_Normal))
		decl.AddStreamCodeIFN(SVStreamCode(CVStreamSemanticDictionnary::Ordinal_Normal, SVStreamCode::Element_Float3, SVStreamCode::Mask_SIMDStride | SVStreamCode::Mask_NormalizedValue));
	if (vertexAttributes & (1 << Attrib_Tangent))
		decl.AddStreamCodeIFN(SVStreamCode(CVStreamSemanticDictionnary::Ordinal_Tangent, SVStreamCode::Element_Float4, SVStreamCode::Mask_SIMDStride | SVStreamCode::Mask_NormalizedValue));
	if (vertexAttributes & (1 << Attrib_Color))
		decl.AddStreamCodeIFN(SVStreamCode(CVStreamSemanticDictionnary::Ordinal_Color, SVStreamCode::Element_Float4, SVStreamCode::Mask_SIMDStride));
	if (vertexAttributes & (1 << Attrib_Texcoord))
		decl.AddStreamCodeIFN(SVStreamCode(CVStreamSemanticDictionnary::Ordinal_Texcoord, SVStreamCode::Element_Float2));

	m_CurrentSubmesh.m_VStream.Reformat(decl);
	m_CurrentSubmesh.m_IStream.SetPrimitiveType(CMeshIStream::Triangles);

	m_Cache.m_VStream.Reformat(decl);
	m_Cache.m_IStream.SetPrimitiveType(CMeshIStream::Triangles);

	m_CurrentSubmeshName = name;
	return true;
}

//----------------------------------------------------------------------------

bool	CMeshBaker::AppendMeshToBake(SMeshDataToFill *meshToBake, const CFloat4x4 &meshTransform)
{
	if (m_Cache.m_VStream.VertexDeclaration().Empty())
	{
		CLog::Log(PK_ERROR, "[MeshBaker] You need to call NextSubmesh before AppendMeshToBake");
		return false;
	}
	if (!PK_VERIFY(meshToBake->m_Indices != null && meshToBake->m_IdxCount != 0))
	{
		CLog::Log(PK_INFO, "Cannot bake a mesh that has no indices...");
		return false;
	}
	if (!PK_VERIFY(meshToBake->m_Positions != null && meshToBake->m_VtxCount != 0))
	{
		CLog::Log(PK_INFO, "Cannot bake a mesh that has no vertices...");
		return false;
	}
	if (!PK_VERIFY(m_SkinCache.Count() == 0))
	{
		CLog::Log(PK_INFO, "Previously added mesh was skinned, cannot batch a skinned mesh and a static mesh in the same submesh...");
		return false;
	}
	if (!PK_VERIFY(m_AllMeshesData.PushBack().Valid()))
		return false;

	SSubMeshData					&submeshData = m_AllMeshesData.Last();
	TMemoryView<const u32>			indices = TMemoryView<const u32>(meshToBake->m_Indices, meshToBake->m_IdxCount);
	TMemoryView<const CFloat3>		positions = TMemoryView<const CFloat3>(meshToBake->m_Positions, meshToBake->m_VtxCount);
	TMemoryView<const CFloat3>		normals = TMemoryView<const CFloat3>(meshToBake->m_Normals, meshToBake->m_Normals != null ? meshToBake->m_VtxCount : 0);
	TMemoryView<const CFloat4>		tangents = TMemoryView<const CFloat4>(meshToBake->m_Tangents, meshToBake->m_Tangents != null ? meshToBake->m_VtxCount : 0);
	TMemoryView<const CFloat2>		UVs = TMemoryView<const CFloat2>(meshToBake->m_UVs, meshToBake->m_UVs != null ? meshToBake->m_VtxCount : 0);
	TMemoryView<const CFloat4>		colors = TMemoryView<const CFloat4>(meshToBake->m_Colors, meshToBake->m_Colors != null ? meshToBake->m_VtxCount : 0);

	// We need to convert the indices to 16bits because the serializer does not handle 32bits indices:
	TMemoryView<const float>		bonesWeights;
	TMemoryView<const u16>			bonesIndices;

	submeshData.m_MeshDataToFillBuffer = meshToBake;
	if (meshToBake->m_VtxBonesIndices != null && meshToBake->m_VtxBonesWeights != null)
	{
		const u32 	totalBonesInfluences = meshToBake->m_VtxCount * 4;
		submeshData.m_IndicesSkinBuffer = PK_TALLOC<u16>(totalBonesInfluences * sizeof(u16), 0x10);

		if (!PK_VERIFY(submeshData.m_IndicesSkinBuffer != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate the per vertices bones indices and weights");
			return false;
		}

		// Copy the indices and the weights:
		for (u32 i = 0; i < totalBonesInfluences; ++i)
			submeshData.m_IndicesSkinBuffer[i] = checked_numcast<u16>(meshToBake->m_VtxBonesIndices[i]);
		// Create the memory views:
		bonesIndices = TMemoryView<const u16>(submeshData.m_IndicesSkinBuffer, totalBonesInfluences);
		bonesWeights = TMemoryView<const float>(meshToBake->m_VtxBonesWeights, totalBonesInfluences);
	}

	// Info log:
	CString		meshInfo = "positions";
	if (!normals.Empty())
		meshInfo += "|normals";
	if (!tangents.Empty())
		meshInfo += "|tangents";
	if (!UVs.Empty())
		meshInfo += "|texcoords";
	if (!colors.Empty())
		meshInfo += "|colors";
	if (!bonesWeights.Empty())
		meshInfo += "|bones-weights";
	if (!bonesIndices.Empty())
		meshInfo += "|bones-indices";

	CLog::Log(PK_INFO, "[MeshBaker] Add Mesh: %d indices %d vertices with %s", meshToBake->m_IdxCount, meshToBake->m_VtxCount, meshInfo.Data());

	// Fill the mesh cache:
	CMeshVStream		&vStream = m_Cache.m_VStream;
	CMeshIStream		&iStream = m_Cache.m_IStream;

	vStream.Resize(positions.Count());
	vStream.ImportPositions(positions);
	if (!normals.Empty())
	{
		if (!PK_VERIFY(vStream.Has(CVStreamSemanticDictionnary::Ordinal_Normal)))
		{
			CLog::Log(PK_ERROR, "[MeshBaker] Could not add the mesh for baking: current baked mesh does not have normals");
			return false;
		}
		vStream.ImportNormals(normals);
	}
	if (!tangents.Empty())
	{
		if (!PK_VERIFY(vStream.Has(CVStreamSemanticDictionnary::Ordinal_Tangent)))
		{
			CLog::Log(PK_ERROR, "[MeshBaker] Could not add the mesh for baking: current baked mesh does not have tangents");
			return false;
		}
		vStream.ImportTangents(tangents);
	}
	if (!UVs.Empty())
	{
		if (!PK_VERIFY(vStream.Has(CVStreamSemanticDictionnary::Ordinal_Texcoord)))
		{
			CLog::Log(PK_ERROR, "[MeshBaker] Could not add the mesh for baking: current baked mesh does not have texcoords");
			return false;
		}
		vStream.ImportTexcoords(UVs);
	}
	if (!colors.Empty())
	{
		if (!PK_VERIFY(vStream.Has(CVStreamSemanticDictionnary::Ordinal_Color)))
		{
			CLog::Log(PK_ERROR, "[MeshBaker] Could not add the mesh for baking: current baked mesh does not have colors");
			return false;
		}
		vStream.ImportColors(colors);
	}
	if (!bonesWeights.Empty() && !bonesIndices.Empty())
	{
		if (!PK_VERIFY(m_SkinCache.Setup(meshToBake->m_VtxCount, bonesWeights, bonesIndices)))
			return false;
	}

	iStream.SetPrimitiveType(CMeshIStream::Triangles);

	if (!iStream.MergeAndOffsetIndices(indices.Data(), indices.Count(), 0).Valid())
	{
		CLog::Log(PK_ERROR, "[MeshBaker] fail to setup indices");
		return false;
	}
	if (!m_CurrentSubmesh.Merge(m_Cache, meshTransform).Valid())
	{
		CLog::Log(PK_ERROR, "[MeshBaker] fail to merge mesh");
		return false;
	}
	m_Cache.Clear(); // We keep the cache empty
	++m_ConcatenatedMeshCount;
	return true;
}

//----------------------------------------------------------------------------

bool	CMeshBaker::BakeToFile(const char *outputMeshPath)
{
	if (!_AddSubmeshIFP())
		return false;
	// get the real path
	CString		path = "Assets" / CString(outputMeshPath);
	CFilePath::Purify(path);
	if (path.Empty())
	{
		CLog::Log(PK_ERROR, "[MeshBaker] invalid output path \"%s\"", outputMeshPath);
		return false;
	}
	// Save the mesh
	bool		success = SavePkmm(path);
	Clear();
	return success;
}

//----------------------------------------------------------------------------

bool	CMeshBaker::SavePkmm(const CString &pkmmPath)
{
	PK_ASSERT(CCurrentThread::IsMainThread());

	SMeshWriteSettings	writeSettings;
	writeSettings.m_GeometryCompression = 0;
	writeSettings.m_Positions = SVStreamCode::Type_F32;
	writeSettings.m_Normals = SVStreamCode::Type_F32;
	writeSettings.m_Tangents = SVStreamCode::Type_F32;
	writeSettings.m_Texcoords = SVStreamCode::Type_F32;

	if (m_Mesh.BatchList().Empty())
	{
		CLog::Log(PK_ERROR, "[MeshBaker] Cannot save an empty mesh to pkmm file");
		return false;
	}
	CMessageStream	outReport(true);	// true: auto-dump to CLog::Log()
	if (!PK_VERIFY(m_Mesh.WriteToFile(File::DefaultFileSystem(), pkmmPath, outReport, &writeSettings)))
	{
		CLog::Log(PK_ERROR, "[MeshBaker] save to \"%s\": FAILED to write mesh", pkmmPath.Data());
		return false;
	}
	CLog::Log(PK_INFO, "[MeshBaker] save to \"%s\": OK", pkmmPath.Data());
	return true;
}

//----------------------------------------------------------------------------

bool	CMeshBaker::_AddSubmeshIFP()
{
	// Add the current submesh to the mesh resource IFP:
	if (!m_CurrentSubmesh.Empty())
	{
		// Create a new mesh from the triangle batch
		PMeshNew	mesh = PK_NEW(CMeshNew(m_CurrentSubmesh));
		if (!PK_VERIFY(mesh != null))
		{
			CLog::Log(PK_ERROR, "[MeshBaker] Alloc mesh new fail");
			return false;
		}
		mesh->RebuildSurfaceAndVolume();
#if	(PK_GEOMETRICS_BUILD_KDTREE_COMPILER != 0)
		// Build the runtime acceleration data structures
		if (!mesh->BuildKdTree(SMeshKdTreeBuildConfig(), true))
		{
			CLog::Log(PK_ERROR, "[MeshBaker] Could not create the mesh kd-tree");
			return false;
		}
#endif
#if	(PK_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
		if (!mesh->SetupDefaultSurfaceSamplingAccelStructsIFN(true))
			CLog::Log(PK_WARN, "[MeshBaker] Failed building mesh surface-sampling acceleration structure");
#endif
#if	(PK_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME != 0)
		if (!mesh->SetupDefaultVolumeSamplingAccelStructsIFN(true))
			CLog::Log(PK_WARN, "[MeshBaker] Failed building mesh volume-sampling acceleration structure");
#endif
#if	(PK_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
		SMeshUV2PCBuildConfig	conf;
		conf.m_MaxDepth = 6;
		conf.m_MaxTrianglesPerLeaf = 6;
		if (!mesh->SetupDefaultSurfaceSamplingAccelStructsUVIFN(conf, true))
			CLog::Log(PK_WARN, "Failed building mesh uv-to-pcoords acceleration structure");
#endif

		// Add submesh to complete mesh
		CLog::Log(PK_INFO, "[MeshBaker] Add submesh '%s' to the mesh resource to bake. This submesh was created by concatenating %u mesh data.", m_CurrentSubmeshName.Data(), m_ConcatenatedMeshCount);

		if (m_SkinCache.Count() != 0)
		{
			if (!PK_VERIFY(m_Mesh.AddBatch(m_CurrentSubmeshName, mesh, &m_SkinCache).Valid()))
			{
				CLog::Log(PK_ERROR, "[MeshBaker] AddBatch with skinning failed: cannot add submesh %s", m_CurrentSubmeshName.Data());
				// Not fatal, do not return...
			}
		}
		else
		{
			if (!PK_VERIFY(m_Mesh.AddBatch(m_CurrentSubmeshName, mesh).Valid()))
			{
				CLog::Log(PK_ERROR, "[MeshBaker] AddBatch failed: cannot add submesh %s", m_CurrentSubmeshName.Data());
				// Not fatal, do not return...
			}
		}
		m_CurrentSubmesh.Clear();
		Mem::Reinit(m_SkinCache);
	}
	return true;
}

//----------------------------------------------------------------------------

CMeshBaker::SSubMeshData::SSubMeshData()
:	m_MeshDataToFillBuffer(null)
,	m_IndicesSkinBuffer(null)
{
}

//----------------------------------------------------------------------------

CMeshBaker::SSubMeshData::~SSubMeshData()
{

}

//----------------------------------------------------------------------------

void	CMeshBaker::SSubMeshData::Clean()
{
	if (m_MeshDataToFillBuffer != null)
	{
		SMeshDataToFill		*meshData = static_cast<SMeshDataToFill*>(m_MeshDataToFillBuffer);

		Mem::Destruct(*meshData);
		PK_FREE(m_MeshDataToFillBuffer);
		m_MeshDataToFillBuffer = null;
	}
	if (m_IndicesSkinBuffer != null)
	{
		PK_FREE(m_IndicesSkinBuffer);
		m_IndicesSkinBuffer = null;
	}
}

//----------------------------------------------------------------------------
__PK_API_END
