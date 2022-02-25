//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_maths/include/pk_maths_primitives.h>
#include <pk_particles/include/ps_effect.h>
#include <pk_particles/include/ps_samplers.h>
#include <pk_particles/include/ps_descriptor.h>
#include <pk_kernel/include/kr_string_id.h>
#include <pk_kernel/include/kr_threads_job_pool.h>

#include <pk_geometrics/include/ge_coordinate_frame.h>
#include <pk_particles/include/ps_samplers_shape.h>
#include <pk_particles/include/ps_attributes.h>

#include "PKUnity_SkinnedMesh.h"
#include "ManagedToNative.h"

struct	SFxAttributeDesc;

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CImageSampler;
class	CCurveDescriptor;
struct	SDensitySamplerData;
PK_FORWARD_DECLARE(ParticleAttributeList);
PK_FORWARD_DECLARE(ParticleAttributeDeclaration);
PK_FORWARD_DECLARE(ParticleMedium);
PK_FORWARD_DECLARE(ParticleMediumCollection);
PK_FORWARD_DECLARE(PKFXScene);
PK_FORWARD_DECLARE(FileStreamFS_Unity);

//----------------------------------------------------------------------------

class	CPKFXEffect : public CNonCopyable
{
public:
	CPKFXEffect(bool usesMeshRenderer, const CFloat4x4 &transforms = CFloat4x4::IDENTITY);
	virtual	~CPKFXEffect();

	PK_FORCEINLINE const CAABB		&Bounds() const { return m_Bounds; }

	// load an fx, the pack containing the fx has to be loaded
	int								LoadFX(const CString &fx);
	void							RunFX(float delay = 0.0f, float prewarm = 0.0f);
	bool							StopFX();
	bool							TerminateFX();
	bool							KillFX();

	//Events
	bool							RegisterEventCallback(const FastDelegate<CParticleEffect::EventCallback> &callback, const CStringId &eventNameID);
	void							UnregisterEventCallback(const FastDelegate<CParticleEffect::EventCallback> &callback, const CStringId &eventNameID);

	void							UnregisterAllEventCallback();

	void							Update(float dt);
	void							UpdateTransforms(const CFloat4x4 &newTransforms);

	SAttributesContainer::SAttrib	*GetAttributesBuffer();
	void							UpdateAttributesBuffer();

	// Samplers:
	// Reset to default:
	bool							ResetSamplerToDefault(u32 smpID);
	// Skinning:
	CFloat4x4						*EffectUpdateSamplerSkinningSetMatrices(u32 smpID);
	bool							EffectBeginUpdateSamplerSkinning(u32 smpID, float dt);
	bool							EffectEndUpdateSamplerSkinning(u32 smpID);
	// Shapes:
	bool							SetSamplerShapeBasics(u32 smpID, EShapeType type, const CFloat3 &size);
	bool							SetSamplerShapeMesh(u32 smpID, SMeshDataToFill *sampler, const CFloat3 &size);
	bool							SetSamplerShapeMesh_Async(u32 smpID, SMeshDataToFill *sampler, const CFloat3 &size);
	bool							SetSamplerShapeMeshFromPkmm(u32 smpID, SMeshSamplerBaked *meshSamplerBaked, const CFloat3 &size);
	bool							SetSamplerShapeMeshFromPkmm_Async(u32 smpID, SMeshSamplerBaked *meshSamplerBaked, const CFloat3 &size);
	bool							SetSamplerShapeTransform(u32 smpID, const CFloat4x4 &transform);
	// Curves:
	bool							SetSamplerCurve(u32 smpID, const SCurveSamplerToFill *curve);
	// Textures:
	bool							SetSamplerTexture(u32 smpID, const STextureSamplerToFill *texture);
	// Text:
	bool							SetSamplerText(u32 smpID, const char *text);

	void							SetGUID(CGuid guid) { m_GUID = guid; }
	CGuid							GUID() const { return m_GUID; }

	const CFloat4x4					&WorldTransformCurrent() { return m_WorldTransformsCurrent; }
	const CString					&Path() const { PK_ASSERT(m_Effect != null && m_Effect->File() != null); return m_Effect->File()->Path(); }

	void							AsyncSwitchIFN();
	void							WaitSwitchIFN();
	void							WaitForAll();

	const PParticleEffectInstance	GetEffectInstance() const { return m_Emitter;  }

private:
	struct	SSamplerData;

	struct	SRuntimeMeshData
	{
		// Acceleration structures are now owned by the integration:
#if	(PK_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
		CMeshSurfaceSamplerStructuresRandom	*m_SurfaceSamplingStructs;
		CMeshSurfaceSamplerStructuresFromUV	*m_SurfaceUVSamplingStructs;

		bool	CreateSurfaceSamplingStructs(const SMeshProxy &mesh);
		bool	CreateSurfaceUVSamplingStructs(const SMeshProxy &mesh);
#endif
#if 0
		CMeshVolumeSamplerStructuresRandom	*m_VolumeSamplingStructs;

		bool	CreateVolumeSamplingStructs(const SMeshProxy &mesh);

		CMeshProjection						*m_ProjectionStructs;

		bool	CreateProjectionStructs(const SMeshProxy &mesh);
#endif
#if (PK_GEOMETRICS_BUILD_KDTREE != 0)
		CMeshKdTree							*m_KdTree;

		bool	CreateKdTree(const SMeshProxy &mesh);
#endif

		void		*m_MeshDataBuffer;

		SRuntimeMeshData();
		~SRuntimeMeshData();
		void	Clean();
	};

	struct	SSetSamplerTaskBase
	{
		SSetSamplerTaskBase();
		~SSetSamplerTaskBase();

		bool		BeforeLoad();
		bool		BeforeSwitch();
		void		SamplerReady();

		// Sampler data ptr:
		u32						m_SamplerId;
		SSamplerData			*m_SamplerData;
	};

	// Functors to set the sampler asynchronously:
	struct	SSetSamplerTask : public SSetSamplerTaskBase
	{
	public:
		SSetSamplerTask();
		~SSetSamplerTask();

		void	operator()();

		// Mesh info:
		SMeshProxy				m_Mesh;
		SRuntimeMeshData		m_MeshData;
		// Skin info:
		float					*m_VtxBonesWeights;
		u32						*m_VtxBonesIndices;
		u32						m_VtxCount;
		u32						m_BonesCount;

		CFloat3					m_Size;
	};

	struct	SSetBakedSamplerTask : public SSetSamplerTaskBase
	{
	public:
		SSetBakedSamplerTask();
		~SSetBakedSamplerTask();

		void	operator()();

		// Baked mesh info:
		int						m_SubmeshId;
		int						m_BonesCount;
		bool					m_IsSkinned;
		CFloat3					m_Size;
		// Pkmm file content:
		void					*m_PkmmContent;
		CConstMemoryStream		m_MemStream;
	};

	struct	SSamplerData
	{
		enum EAsyncSamplerState
		{
			Sampler_Loading,
			Sampler_Loaded,
			Sampler_CanSwitch,
			Sampler_Ready
		};

		EUnitySupportedSamplerType			m_SamplerType;
		u32									m_SamplerUsage;
		PParticleSamplerDescriptor			m_SamplerDescriptor;

		// Sampler shapes:
		EShapeType							m_ShapeType;
		u32									m_BonesCount;
		SRuntimeMeshData					m_MeshData;		// Runtime mesh data (acceleration structures for sampling + buffer for mesh data)
		PResourceMeshBatch					m_MeshBatch;	// Current submesh loaded, need to keep a ref on this when mesh is loaded from pkmm
		PPkFxSkinnedMesh					m_SkinnedMesh;	// Object that handles the skinning for this sampler
		CFloat4x4							m_CurrentMat;	// Current transform of the shape
		CFloat4x4							m_PreviousMat;	// Previous transform of the shape
		// Sampler images:
		PRefCountedMemoryBuffer				m_ImageDataBuffer;

		// Async sampler switch:
		SSetSamplerTask						m_AsyncSetSamplerFunctor;
		SSetBakedSamplerTask				m_AsyncSetBakedSamplerFunctor;
		EAsyncSamplerState					m_AsyncState;
		Threads::CCriticalSection			m_AsyncStateLock;
		Threads::CConditionalVar			m_AsyncStateCondVar;
		// Effect ptr:
		CPKFXEffect							*m_Effect;

		PShapeDescriptor					m_ShapeDesc;
		CImageSampler						*m_ImageDesc;
		CCurveDescriptor					*m_CurveDesc;
		SDensitySamplerData					*m_DensitySampler;

		void			AsyncSwitchIFN();
		bool			WaitSwitchIFN();
		void			WaitForAll();

		SSamplerData();
		~SSamplerData();
	};

	void							_OnFXStoppedAutoDestroy(const PParticleEffectInstance &effectInstance);
	void							_OnFXStopped(const PParticleEffectInstance &effectInstance);
	void							_RebuildAttributes(CParticleAttributeList *descriptor);
	static PShapeDescriptor_Mesh	_LoadSamplerShapeMeshFromPkmm(	CConstMemoryStream &fileStream,
																	int subMeshId,
																	int bonesCount,
																	bool isSkinned,
																	u32 samplerUsage,
																	const CFloat3 &size,
																	PResourceMeshBatch &outMeshBatch,
																	PPkFxSkinnedMesh &outSkinnedMesh);
	bool							_SetSamplerShapeMesh(u32 smpID, SSamplerData &samplerData, const PShapeDescriptor_Mesh &shapeDescriptor, const PResourceMeshBatch &meshBatch, const PPkFxSkinnedMesh &skinnedMesh);
	void							_WaitForSwitchSamplers(SSamplerData *samplerData);
	static void						_MeshUnityDataToMeshProxy(const SMeshDataToFill &from, SMeshProxy &to);

	CGuid							m_GUID;
	CFloat4x4						m_WorldTransformsCurrent;
	CFloat4x4						m_WorldTransformsPrevious;
	CFloat3							m_WorldVelCurrent;
	CFloat3							m_WorldVelPrevious;

	PParticleEffect					m_Effect;
	PParticleEffectInstance			m_Emitter;
	PParticleAttributeList			m_AttributesDescriptor;
	TArray<SSamplerData>			m_SamplersData;
	CAABB							m_Bounds;
	CParticleMediumCollection		*m_MediumCollection;

	TArray<SSamplerData*>			m_RunningAsyncSetSamplers;

	TArray<CStringId>				m_RegisteredEventNameIDs;
};

//----------------------------------------------------------------------------
__PK_API_END
