//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "FxEffect.h"
#include "RuntimeManager.h"

#include "NativeToManaged.h"
#include "PKUnity_SkinnedMesh.h"
#include "PKUnity_FileSystemController.h"

#include <pk_particles/include/ps_mediums.h>
#include <pk_particles/include/ps_samplers.h>
#include <pk_particles/include/ps_samplers_image.h>
#include <pk_particles/include/ps_samplers_text.h>
#include <pk_particles/include/ps_samplers_curve.h>

#include <pk_geometrics/include/ge_matrix_tools.h>
#include <pk_geometrics/include/ge_mesh_kdtree.h>
#include <pk_geometrics/include/ge_mesh_sampler_accel.h>

#include <pk_kernel/include/kr_units.h>
#include <pk_maths/include/pk_maths_transforms.h>
#include <pk_kernel/include/kr_refcounted_buffer.h>
#include <pk_maths/include/pk_maths_simd.h>
#include <pk_imaging/include/im_samplers.h>
#include <pk_base_object/include/hbo_details.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace
{
	CFloat4x4		RemoveScale(const CFloat4x4 &matrix)
	{
		CFloat4x4	m = matrix;

		//m.StrippedXAxis() = -m.StrippedXAxis();

		m.StrippedXAxis().Normalize();
		m.StrippedYAxis().Normalize();
		m.StrippedZAxis().Normalize();
		m.XAxis().w() = 0;
		m.YAxis().w() = 0;
		m.ZAxis().w() = 0;
		m.WAxis().w() = 1;
		return m;
	}
}

//----------------------------------------------------------------------------

CPKFXEffect::CPKFXEffect(bool usesMeshRenderer, const CFloat4x4 &transforms /*= CFloat4x4::IDENTITY*/)
:	m_WorldVelCurrent(CFloat3::ZERO)
,	m_WorldVelPrevious(CFloat3::ZERO)
,	m_Effect(null)
,	m_Emitter(null)
,	m_AttributesDescriptor(null)
{
	m_WorldTransformsCurrent = RemoveScale(transforms);
	m_WorldTransformsPrevious = m_WorldTransformsCurrent;

	m_Bounds.SetMin(CFloat3(-0.1f));
	m_Bounds.SetMax(CFloat3(0.1f));

	m_MediumCollection = CRuntimeManager::Instance().GetScene().GetParticleMediumCollection(usesMeshRenderer);
}

//----------------------------------------------------------------------------

CPKFXEffect::~CPKFXEffect()
{
	if (m_Emitter != null)
	{
		UnregisterAllEventCallback();
		// We remove all the death notifier to avoid gave them called on freed object:
		m_Emitter->m_DeathNotifier.Clear();
		// /!\ This will crash at next update as the effect is not terminated and the matrices are free,
		// This is meant to be terminated or killed before being destroyed OR destroyed just before a mediumCollection.Clear();
	}
}

//----------------------------------------------------------------------------

int	CPKFXEffect::LoadFX(const CString &fx)
{
	// get the spawner
	m_Emitter = null;
	m_Effect = CParticleEffect::Load(fx);

	if (m_Effect == null)
	{
		CLog::Log(PK_ERROR, "Effect could not be loaded.");
		return -1;
	}

	m_AttributesDescriptor = m_Effect->AttributeFlatList().Get();
	if (m_AttributesDescriptor == null)
	{
		CLog::Log(PK_WARN, "No attributes descriptor");
	}

	PK_ASSERT(m_AttributesDescriptor != null);
	if (m_AttributesDescriptor != null)
		_RebuildAttributes(m_AttributesDescriptor.Get());

	// instantiate and add the new effect
	m_Emitter = m_Effect->Instantiate(m_MediumCollection);
	// Fix #12945: Caused by fix #12901,
	// GetAllAttributes doesn't check if m_EffectID is valid now
	// and return the parent effect default container instead of null for newly instantiated emitters.
	// So force create the attribute container here
	m_Emitter->SetAllAttributes(null);	

	if (CRuntimeManager::Instance().GetIsUnitTesting())
	{
		m_Emitter->EnableSeed(true);
		m_Emitter->Seed(0);
	}

	if (m_Emitter == null)
	{
		CLog::Log(PK_ERROR, "Cannot instantiate effect.");
		return -1;
	}

	if (!m_GUID.Valid())
	{
		CRuntimeManager::Instance().FxRegister(this);
		PK_ASSERT(m_GUID.Valid());
		m_Emitter->m_DeathNotifier += FastDelegate<void(const PParticleEffectInstance&) >(this, &CPKFXEffect::_OnFXStoppedAutoDestroy);
	}

	PK_ASSERT(m_GUID.Valid());
	return m_GUID;
}

//----------------------------------------------------------------------------

void	CPKFXEffect::RunFX(float delay, float prewarm)
{
	// Setup the effect instance basic parameters
	PopcornFX::SEffectStartCtl	effectStartCtl;
	effectStartCtl.m_TimeFromStartOfFrame = delay;
	effectStartCtl.m_PrewarmTime = prewarm;
	effectStartCtl.m_TimeToEndOfFrame = 0.0f;		// TODO: Proper values !
	effectStartCtl.m_SpawnTransformsPack.m_WorldTr_Current = &m_WorldTransformsCurrent;
	effectStartCtl.m_SpawnTransformsPack.m_WorldTr_Previous = &m_WorldTransformsPrevious;
	effectStartCtl.m_SpawnTransformsPack.m_WorldVel_Current = &m_WorldVelCurrent;
	effectStartCtl.m_SpawnTransformsPack.m_WorldVel_Previous = &m_WorldVelPrevious;

	m_Emitter->Start(effectStartCtl);
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::StopFX()
{
	// no effect loaded
	if (m_Emitter != null)
	{
		m_Emitter->Stop();
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::TerminateFX()
{
	// no effect loaded
	if (m_Emitter != null)
	{
		UnregisterAllEventCallback();
		m_Emitter->m_DeathNotifier.Clear();
		m_Emitter->Terminate();
		_OnFXStopped(m_Emitter); // Free 'this'
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::KillFX()
{
	// no effect loaded
	if (m_Emitter != null)
	{
		UnregisterAllEventCallback();
		m_Emitter->m_DeathNotifier.Clear();
		m_Emitter->Terminate(); // Maybe not useful...
		m_Emitter->KillDeferred();
		_OnFXStopped(m_Emitter); // Free 'this'
	}
	return true;
}

//----------------------------------------------------------------------------
// Event
//----------------------------------------------------------------------------

bool	CPKFXEffect::RegisterEventCallback(const FastDelegate<CParticleEffect::EventCallback> &callback, const CStringId &eventNameID)
{
	// Register broadcast callback only if this event never got registered
	if (!m_RegisteredEventNameIDs.Contains(eventNameID) &&
		!m_Effect.Get()->RegisterEventCallback(callback, eventNameID))
		return false;

	// add event name ID to keep track of how many effect instance registered to this event
	if (!PK_VERIFY(m_RegisteredEventNameIDs.PushBack(eventNameID).Valid()))
		return false;

	return true;
}

//----------------------------------------------------------------------------

void	CPKFXEffect::UnregisterEventCallback(const FastDelegate<CParticleEffect::EventCallback> &callback, const CStringId &eventNameID)
{
	m_RegisteredEventNameIDs.RemoveElement(eventNameID);

	// Unregister broadcast callback only if there is no effect instance registered to it
	if (!m_RegisteredEventNameIDs.Contains(eventNameID))
		m_Effect.Get()->UnregisterEventCallback(callback, eventNameID);
}

//----------------------------------------------------------------------------

void	 CPKFXEffect::UnregisterAllEventCallback()
{
	if (m_Effect == null)
		return;

	CRuntimeManager::Instance().GetScene().UnregisterEffectAllEventsListeners(this);
}

//----------------------------------------------------------------------------
//
//	Xforms
//
//----------------------------------------------------------------------------

void	CPKFXEffect::UpdateTransforms(const CFloat4x4 &newTransforms)
{
	m_WorldTransformsPrevious = m_WorldTransformsCurrent;
	m_WorldTransformsCurrent = RemoveScale(newTransforms);
}

//----------------------------------------------------------------------------

void	CPKFXEffect::Update(float dt)
{
	m_WorldVelPrevious = m_WorldVelCurrent;
	if (dt > 1.0e-6f)
	{
		const float		invDt = PKRcp(dt);
		const CFloat3	posDelta = m_WorldTransformsCurrent.StrippedTranslations() - m_WorldTransformsPrevious.StrippedTranslations();
		m_WorldVelCurrent = posDelta * invDt;
	}
}

//----------------------------------------------------------------------------
//
//	Attributes
//
//----------------------------------------------------------------------------

SAttributesContainer::SAttrib	*CPKFXEffect::GetAttributesBuffer()
{
	if (m_Emitter->GetAllAttributes() == null)
	{
		m_Emitter->SetAllAttributes(null);
	}
	return const_cast<SAttributesContainer::SAttrib	*>(m_Emitter->GetAllAttributes()->AttributesBegin());
}

void	CPKFXEffect::UpdateAttributesBuffer()
{
	m_Emitter->SetAllAttributes(m_Emitter->GetAllAttributes());
}

//----------------------------------------------------------------------------
// Shape desc helpers
//----------------------------------------------------------------------------

typedef CShapeDescriptor*(*CbNewShapeDesc)(const CFloat3 &size);
typedef void(*CbChangeShapeDesc)(CShapeDescriptor &shapeDesc, const CFloat3 &size);

namespace
{
	template <typename _ShapeType>
	struct	TShapeDesc
	{
	};

	template<>
	struct	TShapeDesc<CShapeDescriptor_Box>
	{
		static CShapeDescriptor	*New(const CFloat3 &size) { return PK_NEW(CShapeDescriptor_Box(size)); }
		static void	Change(CShapeDescriptor &shapeDesc, const CFloat3 &size)
		{
			static_cast<CShapeDescriptor_Box*>(&shapeDesc)->SetDimensions(size);
		}
	};

	template<>
	struct	TShapeDesc<CShapeDescriptor_Sphere>
	{
		static CShapeDescriptor	*New(const CFloat3 &size) { return PK_NEW(CShapeDescriptor_Sphere(size.x(), size.y())); }
		static void	Change(CShapeDescriptor &shapeDesc, const CFloat3 &size)
		{
			CShapeDescriptor_Sphere	*sphereDesc = static_cast<CShapeDescriptor_Sphere*>(&shapeDesc);
			sphereDesc->SetRadius(size.x());
			sphereDesc->SetInnerRadius(size.y());
		}
	};

	template<>
	struct	TShapeDesc<CShapeDescriptor_Cylinder>
	{
		static CShapeDescriptor	*New(const CFloat3 &size) { return PK_NEW(CShapeDescriptor_Cylinder(size.x(), size.z(), size.y())); }
		static void	Change(CShapeDescriptor &shapeDesc, const CFloat3 &size)
		{
			CShapeDescriptor_Cylinder	*cylinderDesc = static_cast<CShapeDescriptor_Cylinder*>(&shapeDesc);
			cylinderDesc->SetRadius(size.x());
			cylinderDesc->SetHeight(size.z());
			cylinderDesc->SetInnerRadius(size.y());
		}
	};

	template<>
	struct	TShapeDesc<CShapeDescriptor_Capsule>
	{
		static CShapeDescriptor	*New(const CFloat3 &size) { return PK_NEW(CShapeDescriptor_Capsule(size.x(), size.z(), size.y())); }
		static void	Change(CShapeDescriptor &shapeDesc, const CFloat3 &size)
		{
			CShapeDescriptor_Capsule	*cylinderDesc = static_cast<CShapeDescriptor_Capsule*>(&shapeDesc);
			cylinderDesc->SetRadius(size.x());
			cylinderDesc->SetHeight(size.z());
			cylinderDesc->SetInnerRadius(size.y());
		}
	};

	template<>
	struct	TShapeDesc<CShapeDescriptor_Mesh>
	{
		static void	Change(CShapeDescriptor &shapeDesc, const CFloat3 &size)
		{
			CShapeDescriptor_Mesh	*meshDesc = static_cast<CShapeDescriptor_Mesh*>(&shapeDesc);
			meshDesc->SetScale(size);
		}
	};
}	// anonymous namespace

//----------------------------------------------------------------------------
// New shape desc
//----------------------------------------------------------------------------

CbNewShapeDesc const	kCbNewShapeDescriptors[] =
{													// ESShapeType :
	&TShapeDesc<CShapeDescriptor_Box>::New,			// BoxShape = 0,
	&TShapeDesc<CShapeDescriptor_Sphere>::New,		// SphereShape,
	&TShapeDesc<CShapeDescriptor_Cylinder>::New,	// CylinderShape,
	&TShapeDesc<CShapeDescriptor_Capsule>::New,		// CapsuleShape,
	null,											// Mesh,
	null,											// MeshFilter,
	null,											// SkinnedMesh,
	null,											// BakedMesh,
};

PK_STATIC_ASSERT(PK_ARRAY_COUNT(kCbNewShapeDescriptors) == ShapeUnsupported);

//----------------------------------------------------------------------------
// Change shape desc
//----------------------------------------------------------------------------

CbChangeShapeDesc const	kCbChangeShapeDescriptors[] =
{													// ESShapeType :
	&TShapeDesc<CShapeDescriptor_Box>::Change,		// BoxShape = 0,
	&TShapeDesc<CShapeDescriptor_Sphere>::Change,	// SphereShape,
	&TShapeDesc<CShapeDescriptor_Cylinder>::Change,	// CylinderShape,
	&TShapeDesc<CShapeDescriptor_Capsule>::Change,	// CapsuleShape,
	null,											// Mesh,
	null,											// MeshFilter,
	null,											// SkinnedMesh,
	null,											// BakedMesh,
};

PK_STATIC_ASSERT(PK_ARRAY_COUNT(kCbChangeShapeDescriptors) == ShapeUnsupported);

//----------------------------------------------------------------------------
//
//	Mesh / Skinned mesh sampler:
//
//----------------------------------------------------------------------------

bool	CPKFXEffect::ResetSamplerToDefault(u32 smpID)
{
	return m_Emitter->SetAttributeSampler(smpID, null);
}

//----------------------------------------------------------------------------

CFloat4x4	*CPKFXEffect::EffectUpdateSamplerSkinningSetMatrices(u32 smpID)
{
	const SSamplerData	&samplerData = m_SamplersData[smpID];

	{
		PK_SCOPEDLOCK(samplerData.m_AsyncStateLock);
		if (samplerData.m_AsyncState != SSamplerData::Sampler_Ready)
			return null;
	}

	if (!PK_VERIFY(samplerData.m_SkinnedMesh != null))
	{
		CLog::Log(PK_ERROR, "Set matrices on a non skinned sampler");
		return null;
	}
	return samplerData.m_SkinnedMesh->GetSkeletonMatrices();
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::EffectBeginUpdateSamplerSkinning(u32 smpID, float dt)
{
	const SSamplerData	&samplerData = m_SamplersData[smpID];

	{
		PK_SCOPEDLOCK(samplerData.m_AsyncStateLock);
		if (samplerData.m_AsyncState != SSamplerData::Sampler_Ready)
			return true;
	}

	if (!PK_VERIFY(samplerData.m_SkinnedMesh != null))
	{
		CLog::Log(PK_ERROR, "Begin update skinning on a non skinned sampler");
		return false;
	}
	samplerData.m_SkinnedMesh->BeginUpdateSkinning(dt);
	return true;
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::EffectEndUpdateSamplerSkinning(u32 smpID)
{
	const SSamplerData	&samplerData = m_SamplersData[smpID];

	{
		PK_SCOPEDLOCK(samplerData.m_AsyncStateLock);
		if (samplerData.m_AsyncState != SSamplerData::Sampler_Ready)
			return true;
	}

	if (!PK_VERIFY(samplerData.m_SkinnedMesh != null))
	{
		CLog::Log(PK_ERROR, "End update skinning on a non skinned sampler");
		return false;
	}
	samplerData.m_SkinnedMesh->EndUpdateSkinning();
	return true;
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SetSamplerShapeBasics(u32 smpID, EShapeType type, const CFloat3 &size)
{
	SSamplerData	&samplerData = m_SamplersData[smpID];
	// Wait for the sampler change thread:
	_WaitForSwitchSamplers(&samplerData);

	if (!PK_VERIFY(samplerData.m_SamplerType == SamplerShape))
	{
		CLog::Log(PK_ERROR, "Set sampler shape on another type of sampler");
		return false;
	}
	if (!PK_VERIFY(kCbNewShapeDescriptors[type] != null))
	{
		CLog::Log(PK_ERROR, "Cannot create the shape descriptor for this sampler");
		return false;
	}

	// Clean the mesh data if present:
	samplerData.m_MeshData.Clean();

	CParticleSamplerDescriptor_Shape_Default	*shapeSmpDesc = static_cast<CParticleSamplerDescriptor_Shape_Default*>(samplerData.m_SamplerDescriptor.Get());
	CShapeDescriptor							*shapeSampler = samplerData.m_ShapeDesc.Get();

	if (samplerData.m_ShapeType == type && shapeSampler != null)
	{
		// Update shape descriptor:
		kCbChangeShapeDescriptors[type](*shapeSampler, size);
	}
	else
	{
		// Create new shape descriptor:
		shapeSampler = kCbNewShapeDescriptors[type](size);
		samplerData.m_ShapeDesc = shapeSampler;
	}

	if (shapeSmpDesc == null)
	{
		shapeSmpDesc = PK_NEW(CParticleSamplerDescriptor_Shape_Default(shapeSampler));
		shapeSmpDesc->m_WorldTr_Current = &samplerData.m_CurrentMat;
		shapeSmpDesc->m_WorldTr_Previous = &samplerData.m_PreviousMat;
		samplerData.m_SamplerDescriptor = shapeSmpDesc;
	}
	else
	{
		shapeSmpDesc->m_Shape = shapeSampler;
	}

	// Sampler data:
	samplerData.m_SkinnedMesh = null;
	samplerData.m_ShapeType = type;

	return m_Emitter->SetAttributeSampler(smpID, shapeSmpDesc);
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SetSamplerShapeMesh(u32 smpID, SMeshDataToFill *meshSampler, const CFloat3 &size)
{
	SSamplerData	&samplerData = m_SamplersData[smpID];
	// Wait for the sampler change thread:
	_WaitForSwitchSamplers(&samplerData);

	PPkFxSkinnedMesh		skinnedMesh = null;
	u32						usageFlags = samplerData.m_SamplerUsage;

	// Create the new sampler shape descriptor:
	PShapeDescriptor_Mesh	shapeDesc = PK_NEW(CShapeDescriptor_Mesh());

	if (!PK_VERIFY(shapeDesc != null))
	{
		CLog::Log(PK_ERROR, "Could not allocate CShapeDescriptor_Mesh");
		return false;
	}

	SMeshProxy								meshProxy;

	_MeshUnityDataToMeshProxy(*meshSampler, meshProxy);
	shapeDesc->SetMeshProxy(meshProxy);

	// Clear the old acceleration structures:
	samplerData.m_MeshData.Clean();
	samplerData.m_MeshData.m_MeshDataBuffer = meshSampler; // The SMeshDataToFill* is the pointer to free
	// ------------------------------------------
	// Build KD Tree IFN
	if ((usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_Intersect) != 0 ||
		(usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_Project) != 0 ||
		(usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_Contains) != 0 ||
		(usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_DistanceField) != 0)
	{
#if	(PK_GEOMETRICS_BUILD_KDTREE != 0)
		if (!samplerData.m_MeshData.CreateKdTree(meshProxy))
			CLog::Log(PK_WARN, "Failed building mesh kdTree acceleration structure");
		else
			shapeDesc->SetKdTree(samplerData.m_MeshData.m_KdTree);
#endif
	}

#if	(PK_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
	// Build sampling info IFN
	if ((usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_Sample) != 0)
	{
		if (!samplerData.m_MeshData.CreateSurfaceSamplingStructs(meshProxy))
			CLog::Log(PK_WARN, "Failed building mesh surface-sampling acceleration structure");
		else
			shapeDesc->SetSamplingStructs(samplerData.m_MeshData.m_SurfaceSamplingStructs, null);
#if		0	// We disable the volume sampling acceleration structure for now which seems a little bit more complex to build
		if (!samplerData.m_MeshSmpAccel.CreateVolumeSamplingStructs(meshProxy))
			CLog::Log(PK_WARN, "Failed building mesh surface-sampling acceleration structure");
#endif
	}
	// Build UV 2 PCoords info IFN
	if ((usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_SampleFromUV) != 0)
	{
		if (!samplerData.m_MeshData.CreateSurfaceUVSamplingStructs(meshProxy))
			CLog::Log(PK_WARN, "Failed building mesh uv-to-pcoords acceleration structure");
		else
			shapeDesc->SetUVSamplingStructs(samplerData.m_MeshData.m_SurfaceUVSamplingStructs, 0);
	}
#endif

	//-------------------------------
	if (meshSampler->m_VtxBonesWeights != null &&
		meshSampler->m_VtxBonesIndices != null)
	{
		skinnedMesh = PK_NEW(CPkFxSkinnedMesh);

		if (!PK_VERIFY(skinnedMesh != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate CPkFxSkinnedMesh");
			return false;
		}
		if (!PK_VERIFY(skinnedMesh->Init(	usageFlags,
											meshProxy,
											meshSampler->m_VtxBonesWeights,
											meshSampler->m_VtxBonesIndices,
											meshSampler->m_VtxCount,
											4,
											meshSampler->m_BonesCount)))
		{
			CLog::Log(PK_ERROR, "Could not initialize the CPkFxSkinnedMesh");
			return false;
		}
	}
	if (skinnedMesh != null)
		shapeDesc->SetMesh(null, skinnedMesh->Override());
	shapeDesc->SetScale(size);
	return _SetSamplerShapeMesh(smpID, samplerData, shapeDesc, null, skinnedMesh);
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SetSamplerShapeMesh_Async(u32 smpID, SMeshDataToFill *sampler, const CFloat3 &size)
{
	SSamplerData	&samplerData = m_SamplersData[smpID];
	// Wait for the sampler change thread:
	_WaitForSwitchSamplers(&samplerData);

	SSetSamplerTask		*task = &samplerData.m_AsyncSetSamplerFunctor;

	samplerData.m_AsyncState = SSamplerData::Sampler_Loading;

	// Fill up the task here:
	_MeshUnityDataToMeshProxy(*sampler, task->m_Mesh);
	task->m_MeshData.m_MeshDataBuffer = sampler;
	task->m_SamplerId = smpID;
	task->m_SamplerData = &samplerData;
	task->m_VtxBonesWeights = sampler->m_VtxBonesWeights;
	task->m_VtxBonesIndices = sampler->m_VtxBonesIndices;
	task->m_VtxCount = sampler->m_VtxCount;
	task->m_BonesCount = sampler->m_BonesCount;
	task->m_Size = size;

	if (!PK_VERIFY(m_RunningAsyncSetSamplers.PushBack(&samplerData).Valid()))
	{
		CLog::Log(PK_ERROR, "Could not push back in the m_SamplerDataAsync array");
		return false;
	}
	return CRuntimeManager::Instance().AddBackgroundTask(task);
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SetSamplerShapeMeshFromPkmm(u32 smpID, SMeshSamplerBaked *meshSamplerBaked, const CFloat3 &size)
{
	SSamplerData	&samplerData = m_SamplersData[smpID];
	// Wait for the sampler change thread:
	_WaitForSwitchSamplers(&samplerData);

	CConstMemoryStream			memStream(meshSamplerBaked->m_PkmmFileContent, meshSamplerBaked->m_PkmmFileContentSize);
	PResourceMeshBatch			meshBatch = null;
	PPkFxSkinnedMesh			skinnedMesh = null;
	PShapeDescriptor_Mesh		descMesh = _LoadSamplerShapeMeshFromPkmm(	memStream,
																			meshSamplerBaked->m_SubmeshId,
																			meshSamplerBaked->m_BonesCount,
																			meshSamplerBaked->m_IsSkinned == ManagedBool_True,
																			samplerData.m_SamplerUsage,
																			size,
																			meshBatch,
																			skinnedMesh);
	if (descMesh == null)
		return false;
	return _SetSamplerShapeMesh(smpID, samplerData, descMesh, meshBatch, skinnedMesh);
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SetSamplerShapeMeshFromPkmm_Async(u32 smpID, SMeshSamplerBaked *meshSamplerBaked, const CFloat3 &size)
{
	SSamplerData	&samplerData = m_SamplersData[smpID];
	// Wait for the sampler change thread:
	_WaitForSwitchSamplers(&samplerData);

	SSetBakedSamplerTask	*task = &samplerData.m_AsyncSetBakedSamplerFunctor;

	samplerData.m_AsyncState = SSamplerData::Sampler_Loading;

	// Fill up the task here:
	task->m_SamplerId = smpID;
	task->m_SamplerData = &samplerData;
	{
		PK_NAMEDSCOPEDPROFILE("Allocate CFileStreamFS_Unity and CResourceMesh");

		task->m_PkmmContent = PK_MALLOC_ALIGNED(meshSamplerBaked->m_PkmmFileContentSize, 0x10);
		Mem::Copy(task->m_PkmmContent, meshSamplerBaked->m_PkmmFileContent, meshSamplerBaked->m_PkmmFileContentSize);
		task->m_MemStream = CConstMemoryStream(task->m_PkmmContent, meshSamplerBaked->m_PkmmFileContentSize);
		task->m_SubmeshId = meshSamplerBaked->m_SubmeshId;
		task->m_BonesCount = meshSamplerBaked->m_BonesCount;
		task->m_IsSkinned = meshSamplerBaked->m_IsSkinned == ManagedBool_True;
		task->m_Size = size;
	}

	if (!PK_VERIFY(m_RunningAsyncSetSamplers.PushBack(&samplerData).Valid()))
	{
		CLog::Log(PK_ERROR, "Could not push back in the m_SamplerDataAsync array");
		return false;
	}
	return CRuntimeManager::Instance().AddBackgroundTask(task);
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SetSamplerShapeTransform(u32 smpID, const CFloat4x4 &transforms)
{
	SSamplerData	&samplerData = m_SamplersData[smpID];

	if (!PK_VERIFY(samplerData.m_SamplerType == SamplerShape))
	{
		CLog::Log(PK_ERROR, "Set transforms on a sampler that does not allow transforms");
		return false;
	}
	// Switch previous with current:
	samplerData.m_PreviousMat = samplerData.m_CurrentMat;
	samplerData.m_CurrentMat = transforms;
	return true;
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SetSamplerCurve(u32 smpID, const SCurveSamplerToFill *curveData)
{
	SSamplerData	&samplerData = m_SamplersData[smpID];
	const float		CURVE_MINIMUM_DELTA = 0.0001f;

	if (!PK_VERIFY(samplerData.m_SamplerType == SamplerCurve))
	{
		CLog::Log(PK_ERROR, "Set sampler shape on another type of sampler");
		return false;
	}

	const u32	keyCount = curveData->m_KeyPointsCount;
	const u32	curvesCount = curveData->m_CurveDimension;
	const u32	keyDataOffset = 1 + curvesCount * 3;
	u32			totalKeyCount = keyCount;
	float		*curvesDatas = static_cast<float*>(curveData->m_KeyPoints);

	bool	prevHasInfOut = false;
	for (u32 iKey = 0; iKey < keyCount; ++iKey)
	{
		const u32	prevKeyIdx = (iKey - 1) * keyDataOffset;
		const u32	keyIdx = iKey * keyDataOffset;

		bool			hasInfIn = false;
		bool			hasInfOut = false;

		for (u32 iCurve = 0; iCurve < curvesCount; ++iCurve)
		{
			const u32	prevDataIdx = prevKeyIdx + 1 + iCurve * 3;
			const u32	dataIdx = keyIdx + 1 + iCurve * 3;

			if (iKey > 0 && !prevHasInfOut && !hasInfIn &&
				TNumericTraits<float>::IsInfinite(curvesDatas[dataIdx + 1]) &&
				!TNumericTraits<float>::IsInfinite(curvesDatas[prevDataIdx + 2]))
			{
				hasInfIn = true;
				totalKeyCount++;
			}
			if (!hasInfOut && TNumericTraits<float>::IsInfinite(curvesDatas[dataIdx + 2]))
			{
				hasInfOut = true;
				totalKeyCount++;
			}
		}
		prevHasInfOut = hasInfOut;
	}

	CParticleSamplerDescriptor_Curve_Default	*curveSmpDesc = static_cast<CParticleSamplerDescriptor_Curve_Default*>(samplerData.m_SamplerDescriptor.Get());
	CCurveDescriptor							*curveDesc = samplerData.m_CurveDesc;

	if (curveDesc == null)
	{
		curveDesc = PK_NEW(CCurveDescriptor());
		if (!PK_VERIFY(curveDesc != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate CCurveDescriptor");
			return false;
		}
		samplerData.m_CurveDesc = curveDesc;
	}

	curveDesc->m_Interpolator = CInterpolableVectorArray::Interpolator_Hermite;	// smooth curve
	curveDesc->m_Order = curvesCount;
	curveDesc->SetParametricDomain(CFloat2(0.0f, 1.0f));												// default range of time values.
	curveDesc->SetEvalLimits(TNumericTraits<CFloat4>::NegativeInfinity(), TNumericTraits<CFloat4>::PositiveInfinity());
	if (!curveDesc->Resize(totalKeyCount))	// control-points
		return false;

	PK_ASSERT(curveDesc->m_Times.Count() == totalKeyCount);
	PK_ASSERT(curveDesc->m_FloatValues.Count() == totalKeyCount * curveDesc->m_Order);		// one float value
	PK_ASSERT(curveDesc->m_FloatTangents.Count() == totalKeyCount * 2 * curveDesc->m_Order);	// two float tangents per control-point

	int				outKeyOffset = 0;
	TArray<bool>	hasInfInArray;
	TArray<bool>	hasInfOutArray;
	TArray<bool>	prevHasInfOutArray;

	if (!hasInfInArray.Resize(curvesCount))
		return false;
	if (!hasInfOutArray.Resize(curvesCount))
		return false;
	if (!prevHasInfOutArray.Resize(curvesCount))
		return false;
	for (u32 i = 0; i < curvesCount; ++i)
		prevHasInfOutArray[i] = false;

	for (u32 iKey = 0; iKey < keyCount; ++iKey)
	{
		const u32	timeIdx = iKey * keyDataOffset;
		const u32	prevTimeIdx = (iKey - 1) * keyDataOffset;
		const u32	nextTimeIdx = (iKey + 1) * keyDataOffset;
		const u32	nextNextTimeIdx = (iKey + 2) * keyDataOffset;

		const float	deltaIn = iKey > 0 ? PKAbs(curvesDatas[timeIdx] - curvesDatas[prevTimeIdx]) : 0.0f;
		const float	deltaOut = iKey < keyCount - 1 ? PKAbs(curvesDatas[nextTimeIdx] - curvesDatas[timeIdx]) : 0.0f;
		const float	nextDeltaOut = iKey < keyCount - 2 ? PKAbs(curvesDatas[nextNextTimeIdx] - curvesDatas[nextTimeIdx]) : 0.0f;

		for (u32 i = 0; i < curvesCount; ++i)
			hasInfInArray[i] = false;
		for (u32 i = 0; i < curvesCount; ++i)
			hasInfOutArray[i] = false;

		//insert key for inTangent infinite IFN
		for (u32 iCurve = 0; iCurve < curvesCount; ++iCurve)
		{
			const u32	inIdx = timeIdx + 1 + iCurve * 3;
			const u32	prevInIdx = prevTimeIdx + 1 + iCurve * 3;

			if (iKey > 0 &&
				TNumericTraits<float>::IsInfinite(curvesDatas[inIdx + 1]) &&
				!TNumericTraits<float>::IsInfinite(curvesDatas[prevInIdx + 2]))
			{
				if (hasInfInArray.Contains(true) || prevHasInfOutArray.Contains(true))
				{
					//change fake key values already inserted (by an other curve || by a previous key)
					const u32	outFakeIdxValue = ((iKey + outKeyOffset - 1) * curvesCount) + iCurve;
					const u32	outFakeIdxInTangents = ((iKey + outKeyOffset - 1) * curvesCount * 2) + iCurve;
					const u32	outFakeIdxOutTangents = outFakeIdxInTangents + curvesCount;
					const u32	prevOutIdxValue = ((iKey - 1 + outKeyOffset - 1) * curvesCount) + iCurve;
					const u32	prevOutIdxInTangents = ((iKey - 1 + outKeyOffset - 1) * curvesCount * 2) + iCurve;
					const u32	prevOutIdxOutTangents = prevOutIdxInTangents + curvesCount;

					curveDesc->m_FloatValues[outFakeIdxValue] = curveDesc->m_FloatValues[prevOutIdxValue];
					curveDesc->m_FloatTangents[outFakeIdxInTangents] = 0.0f;
					curveDesc->m_FloatTangents[outFakeIdxOutTangents] = 0.0f;
					curveDesc->m_FloatTangents[prevOutIdxOutTangents] = 0.0f;
				}
				else
				{
					//insert fake keys for current curve
					const u32	prevOutIdxValue = ((iKey - 1 + outKeyOffset) * curvesCount) + iCurve;
					const u32	prevOutIdxInTangents = ((iKey - 1 + outKeyOffset) * curvesCount * 2) + iCurve;
					const u32	prevOutIdxOutTangents = prevOutIdxInTangents + curvesCount;
					const u32	outIdxValue = ((iKey + outKeyOffset) * curvesCount) + iCurve;
					const u32	outIdxInTangents = ((iKey + outKeyOffset) * curvesCount * 2) + iCurve;
					const u32	outIdxOutTangents = outIdxInTangents + curvesCount;

					curveDesc->m_Times[iKey + outKeyOffset] = curvesDatas[timeIdx] - CURVE_MINIMUM_DELTA;
					curveDesc->m_FloatValues[outIdxValue] = curveDesc->m_FloatValues[prevOutIdxValue];
					curveDesc->m_FloatTangents[outIdxInTangents] = 0.0f;
					curveDesc->m_FloatTangents[outIdxOutTangents] = 0.0f;
					curveDesc->m_FloatTangents[prevOutIdxOutTangents] = 0.0f;
					//insert fake keys for the other curves
					for (u32 iFakeKey = 0; iFakeKey < curvesCount; ++iFakeKey)
					{
						if (iFakeKey == iCurve)
							continue;
						const u32	inFakeIdx = timeIdx + 1 + iFakeKey * 3;
						const u32	outFakeIdxValue = ((iKey + outKeyOffset) * curvesCount) + iFakeKey;
						const u32	outFakeIdxInTangents = ((iKey + outKeyOffset) * curvesCount * 2) + iFakeKey;
						const u32	outFakeIdxOutTangents = outFakeIdxInTangents + curvesCount;

						curveDesc->m_FloatValues[outFakeIdxValue] = curvesDatas[inFakeIdx + 0];
						curveDesc->m_FloatTangents[outFakeIdxInTangents] = curvesDatas[inFakeIdx + 1] * deltaIn;
						curveDesc->m_FloatTangents[outFakeIdxOutTangents] = curvesDatas[inFakeIdx + 2] * deltaOut;
					}
					outKeyOffset++;
				}
				hasInfInArray[iCurve] = true;
			}
			if (TNumericTraits<float>::IsInfinite(curvesDatas[inIdx + 2]))
				hasInfOutArray[iCurve] = true;
		}
		//insert real key
		for (u32 iCurve = 0; iCurve < curvesCount; ++iCurve)
		{
			const u32	inIdx = timeIdx + 1 + iCurve * 3;
			const u32	outIdxValue = ((iKey + outKeyOffset) * curvesCount) + iCurve;
			const u32	outIdxInTangents = ((iKey + outKeyOffset) * curvesCount * 2) + iCurve;
			const u32	outIdxOutTangents = outIdxInTangents + curvesCount;

			curveDesc->m_Times[iKey + outKeyOffset] = curvesDatas[timeIdx];
			curveDesc->m_FloatValues[outIdxValue] = curvesDatas[inIdx + 0];
			if (iKey == 0 || hasInfInArray[iCurve] || prevHasInfOutArray[iCurve])
				curveDesc->m_FloatTangents[outIdxInTangents] = 0.0f;		//InTangents first key => 0
			else
				curveDesc->m_FloatTangents[outIdxInTangents] = curvesDatas[inIdx + 1] * deltaIn;
			if (iKey == keyCount - 1 || hasInfOutArray[iCurve])
				curveDesc->m_FloatTangents[outIdxOutTangents] = 0.0f;	//OutTangents last key => 0
			else
				curveDesc->m_FloatTangents[outIdxOutTangents] = curvesDatas[inIdx + 2] * deltaOut;
		}
		for (u32 i = 0; i < curvesCount; ++i)
			prevHasInfOutArray[i] = false;
		//insert key for outTangent infinite IFN
		for (u32 iCurve = 0; iCurve < curvesCount; ++iCurve)
		{
			const u32	inIdx = timeIdx + 1 + iCurve * 3;
			if (hasInfOutArray[iCurve])
			{
				if (!prevHasInfOutArray.Contains(true))
				{
					outKeyOffset++;
					const u32	outIdxValue = ((iKey + outKeyOffset) * curvesCount) + iCurve;
					const u32	outIdxInTangents = ((iKey + outKeyOffset) * curvesCount * 2) + iCurve;
					const u32	outIdxOutTangents = outIdxInTangents + curvesCount;

					curveDesc->m_Times[iKey + outKeyOffset] = curvesDatas[nextTimeIdx] - CURVE_MINIMUM_DELTA;
					curveDesc->m_FloatValues[outIdxValue] = curvesDatas[inIdx + 0];
					curveDesc->m_FloatTangents[outIdxInTangents] = 0.0f;
					curveDesc->m_FloatTangents[outIdxOutTangents] = 0.0f;
					//insert fake keys for the other curves
					for (u32 iFakeKey = 0; iFakeKey < curvesCount; ++iFakeKey)
					{
						if (iFakeKey == iCurve)
							continue;
						const u32	inFakeIdx = nextTimeIdx + 1 + iFakeKey * 3;
						const u32	outFakeIdxValue = ((iKey + outKeyOffset) * curvesCount) + iFakeKey;
						const u32	outFakeIdxInTangents = ((iKey + outKeyOffset) * curvesCount * 2) + iFakeKey;
						const u32	outFakeIdxOutTangents = outFakeIdxInTangents + curvesCount;

						curveDesc->m_FloatValues[outFakeIdxValue] = curvesDatas[inFakeIdx + 0];
						curveDesc->m_FloatTangents[outFakeIdxInTangents] = curvesDatas[inFakeIdx + 1] * deltaOut;
						curveDesc->m_FloatTangents[outFakeIdxOutTangents] = curvesDatas[inFakeIdx + 2] * nextDeltaOut;
					}
				}
				else
				{
					//change fake key values already inserted
					const u32	outFakeIdxValue = ((iKey + outKeyOffset) * curvesCount) + iCurve;
					const u32	outFakeIdxInTangents = ((iKey + outKeyOffset) * curvesCount * 2) + iCurve;
					const u32	outFakeIdxOutTangents = outFakeIdxInTangents + curvesCount;
					const u32	prevOutIdxValue = ((iKey - 1 + outKeyOffset) * curvesCount) + iCurve;

					curveDesc->m_FloatValues[outFakeIdxValue] = curveDesc->m_FloatValues[prevOutIdxValue];
					curveDesc->m_FloatTangents[outFakeIdxInTangents] = 0.0f;
					curveDesc->m_FloatTangents[outFakeIdxOutTangents] = 0.0f;

				}
				prevHasInfOutArray[iCurve] = true;
			}
			else
				prevHasInfOutArray[iCurve] = false;
		}
	}

	if (curveSmpDesc == null)
	{
		curveSmpDesc = PK_NEW(CParticleSamplerDescriptor_Curve_Default());
		samplerData.m_SamplerDescriptor = curveSmpDesc;
	}
	curveSmpDesc->m_Curve0 = curveDesc;

	return m_Emitter->SetAttributeSampler(smpID, curveSmpDesc);
}

//----------------------------------------------------------------------------
//
//	Image samplers:
//
//----------------------------------------------------------------------------

bool	CPKFXEffect::SetSamplerTexture(u32 smpID, const STextureSamplerToFill *texture)
{
	const u32				width = texture->m_Width;
	const u32				height = texture->m_Height;
	const u32				sizeInBytes = texture->m_SizeInBytes;
	const CImage::EFormat	format = static_cast<CImage::EFormat>(texture->m_PixelFormat);
	SSamplerData			&samplerData = m_SamplersData[smpID];

	if (!PK_VERIFY(samplerData.m_SamplerType == SamplerImage))
	{
		CLog::Log(PK_ERROR, "Set sampler image on another type of sampler");
		return false;
	}

	if (sizeInBytes > 0 && texture->m_TextureData != null)
	{
		samplerData.m_ImageDataBuffer = CRefCountedMemoryBuffer::AllocAligned(sizeInBytes, 0x10);

		if (!PK_VERIFY(samplerData.m_ImageDataBuffer != null))
		{
			CLog::Log(PK_ERROR, "Cannot allocate the image sampler data");
			return false;
		}

		Mem::Copy(samplerData.m_ImageDataBuffer->Data<u8*>(), texture->m_TextureData, sizeInBytes);

		CImageMap		map;

		map.m_RawBuffer = samplerData.m_ImageDataBuffer;
		map.m_Dimensions = CUint3(width, height, 1);

		CImageSampler	*imageDesc = samplerData.m_ImageDesc;
		CImageSurface	surface(map, format);

		if (imageDesc == null)
		{
			imageDesc = PK_NEW(CImageSamplerBilinear);
			samplerData.m_ImageDesc = imageDesc;
		}
		if (!imageDesc->SetupFromSurface(surface))
		{
			surface.Convert(CImage::Format_BGRA8);
			if (!PK_VERIFY(imageDesc->SetupFromSurface(surface)))
			{
				CLog::Log(PK_ERROR, "Could not setup the image sampler");
				return false;
			}
		}
		CParticleSamplerDescriptor_Image_Default	*smpDesc = static_cast<CParticleSamplerDescriptor_Image_Default*>(samplerData.m_SamplerDescriptor.Get());

		if (smpDesc == null)
		{
			smpDesc = PK_NEW(CParticleSamplerDescriptor_Image_Default(imageDesc));
			samplerData.m_SamplerDescriptor = smpDesc;
		}
		else
		{
			smpDesc->m_Sampler = imageDesc;
			smpDesc->m_ImageDimensions = imageDesc->Dimensions();
		}

		if (!PK_VERIFY(samplerData.m_ImageDesc != null))
		{
			CLog::Log(PK_ERROR, "Could not create the image sampler");
			return false;
		}

		if (samplerData.m_SamplerUsage & SParticleDeclaration::SSampler::UsageFlags_Image_Density)
		{
			if (samplerData.m_DensitySampler == null)
			{
				samplerData.m_DensitySampler = PK_NEW(SDensitySamplerData);

				PK_ASSERT(samplerData.m_DensitySampler != null);
			}
			SDensitySamplerBuildSettings	densityBuildSettings;

			if (!samplerData.m_DensitySampler->Build(surface, densityBuildSettings))
			{
				CLog::Log(PK_ERROR, "Could not build the density sampler");
				return false;
			}
			if (!smpDesc->SetupDensity(samplerData.m_DensitySampler))
			{
				CLog::Log(PK_ERROR, "Could not setup the density image sampler");
				return false;
			}
		}
		return m_Emitter->SetAttributeSampler(smpID, smpDesc);
	}
	return false;
}

//----------------------------------------------------------------------------
//
//	Text sampler:
//
//----------------------------------------------------------------------------

bool	CPKFXEffect::SetSamplerText(u32 smpID, const char *text)
{
	CString			textStr(text);
	SSamplerData	&samplerData = m_SamplersData[smpID];

	if (!PK_VERIFY(samplerData.m_SamplerType == SamplerText))
	{
		CLog::Log(PK_ERROR, "Set sampler text on another type of sampler");
		return false;
	}
	CParticleSamplerDescriptor_Text_Default			*desc = static_cast<CParticleSamplerDescriptor_Text_Default*>(samplerData.m_SamplerDescriptor.Get());

	textStr = textStr.Replace("\\n", "\n");
	textStr = textStr.Replace("\\r", "\r");

	if (desc == null)
	{
		desc = PK_NEW(CParticleSamplerDescriptor_Text_Default());
		samplerData.m_SamplerDescriptor = desc;
	}

	const CFontMetrics	*fontKerning = null;
	bool				useKerning = false;

	if (desc != null)
	{
		if (!desc->_Setup(textStr, fontKerning, useKerning))
		{
			CLog::Log(PK_ERROR, "Could not setup the text descriptor");
			return false;
		}
	}
	return m_Emitter->SetAttributeSampler(smpID, desc);
}

//----------------------------------------------------------------------------

void	CPKFXEffect::AsyncSwitchIFN()
{
	u32		i = 0;
	while (i < m_RunningAsyncSetSamplers.Count())
	{
		m_RunningAsyncSetSamplers[i]->AsyncSwitchIFN();
		++i;
	}
}

//----------------------------------------------------------------------------

void	CPKFXEffect::WaitSwitchIFN()
{
	u32		i = 0;
	while (i < m_RunningAsyncSetSamplers.Count())
	{
		if (m_RunningAsyncSetSamplers[i]->WaitSwitchIFN())
			m_RunningAsyncSetSamplers.Remove(i);
		else
			++i;
	}
}

//----------------------------------------------------------------------------

void	CPKFXEffect::WaitForAll()
{
	for (u32 i = 0; i < m_RunningAsyncSetSamplers.Count(); ++i)
	{
		m_RunningAsyncSetSamplers[i]->WaitForAll();
	}
	m_RunningAsyncSetSamplers.Clear();
}

//----------------------------------------------------------------------------

void	CPKFXEffect::_OnFXStoppedAutoDestroy(const PParticleEffectInstance &effectInstance)
{
	_OnFXStopped(effectInstance);
	CRuntimeManager::Instance().OnFxStopped(m_GUID);
}

//----------------------------------------------------------------------------

void	CPKFXEffect::_OnFXStopped(const PParticleEffectInstance &effectInstance)
{
	(void)effectInstance;
	WaitForAll();
	m_Emitter = null;
}

//----------------------------------------------------------------------------

void	CPKFXEffect::_RebuildAttributes(CParticleAttributeList *descriptor)
{
	PK_ASSERT(descriptor != null);
	const u32	attrCount = descriptor->UniqueAttributeList().Count();
	const u32	sampCount = descriptor->SamplerList().Count();

	if (attrCount != 0 || sampCount != 0)
	{
		u32		samplerCount = 0;

		// just clone the default attributes:
		if (*(descriptor->DefaultAttributes()) != null)
			samplerCount = (*descriptor->DefaultAttributes())->Samplers().Count();

		m_SamplersData.HardResize(samplerCount);

		for (u32 i = 0; i < sampCount; ++i)
		{
			CParticleAttributeSamplerDeclaration			*samplerDecl = m_AttributesDescriptor->SamplerList()[i];
			SParticleDeclaration::SSampler::ESamplerType	type = (SParticleDeclaration::SSampler::ESamplerType)samplerDecl->ExportedType();

			// We set the effect ptr on the sampler class so that the async set samplers can access the effect instance:
			m_SamplersData[i].m_Effect = this;
			m_SamplersData[i].m_SamplerUsage = samplerDecl->UsageFlags();
			switch (type)
			{
			case SParticleDeclaration::SSampler::ESamplerType::Sampler_Geometry:
				m_SamplersData[i].m_SamplerType = SamplerShape;
				break;
			case SParticleDeclaration::SSampler::ESamplerType::Sampler_Image:
				m_SamplersData[i].m_SamplerType = SamplerImage;
				break;
			case SParticleDeclaration::SSampler::ESamplerType::Sampler_Text:
				m_SamplersData[i].m_SamplerType = SamplerText;
				break;
			case SParticleDeclaration::SSampler::ESamplerType::Sampler_Curve:
				m_SamplersData[i].m_SamplerType = SamplerCurve;
				break;
			default:
				m_SamplersData[i].m_SamplerType = SamplerUnsupported;
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------

PShapeDescriptor_Mesh	CPKFXEffect::_LoadSamplerShapeMeshFromPkmm(	CConstMemoryStream &memStream,
																	int subMeshId,
																	int bonesCount,
																	bool isSkinned,
																	u32 samplerUsage,
																	const CFloat3 &size,
																	PResourceMeshBatch &outMeshBatch,
																	PPkFxSkinnedMesh &outSkinnedMesh)
{
	CMessageStream	loadReport;
	PResourceMesh	mesh = CResourceMesh::Load(memStream, loadReport);
	loadReport.Log();
	if (!PK_VERIFY(mesh != null))
	{
		CLog::Log(PK_ERROR, "Fail loading the CMeshResource from the pkmm content");
		return null;
	}
	const u32		uSubMeshId = static_cast<u32>(subMeshId);
	const u32		batchCount = mesh->BatchList().Count();
	if (!PK_VERIFY(uSubMeshId < batchCount))
	{
		CLog::Log(PK_ERROR, "Cannot use the submesh ID %d: the mesh only has %d submeshes", uSubMeshId, batchCount);
		return null;
	}
	outMeshBatch = mesh->BatchList()[uSubMeshId];
	if (!PK_VERIFY(!isSkinned || outMeshBatch->IsSkinned()))
	{
		CLog::Log(PK_ERROR, "The submesh %d does not have skinning data", uSubMeshId);
		return null;
	}
	if (isSkinned)
	{
		PK_NAMEDSCOPEDPROFILE("Setup skinned mesh");
		outSkinnedMesh = PK_NEW(CPkFxSkinnedMesh);
		if (!PK_VERIFY(outSkinnedMesh != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate CPkFxSkinnedMesh");
			return null;
		}
		if (!PK_VERIFY(outSkinnedMesh->Init(samplerUsage, outMeshBatch->RawMesh()->TriangleBatch(), outMeshBatch->m_OptimizedStreams, bonesCount)))
		{
			CLog::Log(PK_ERROR, "Could not initialize the CPkFxSkinnedMesh");
			return null;
		}
	}
	PShapeDescriptor_Mesh	shapeDesc = PK_NEW(CShapeDescriptor_Mesh());
	if (!PK_VERIFY(shapeDesc != null))
	{
		CLog::Log(PK_ERROR, "Could not allocate CShapeDescriptor_Mesh");
		return null;
	}
	if (outSkinnedMesh != null)
		shapeDesc->SetMesh(outMeshBatch->RawMesh(), outSkinnedMesh->Override());
	else
		shapeDesc->SetMesh(outMeshBatch->RawMesh());
	shapeDesc->SetScale(size);
	return shapeDesc;
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::_SetSamplerShapeMesh(u32 smpID, SSamplerData &samplerData, const PShapeDescriptor_Mesh &shapeDescriptor, const PResourceMeshBatch &meshBatch, const PPkFxSkinnedMesh &skinnedMesh)
{
	if (!PK_VERIFY(samplerData.m_SamplerType == SamplerShape))
	{
		CLog::Log(PK_ERROR, "Set sampler mesh on another type of sampler");
		return false;
	}

	PParticleSamplerDescriptor_Shape_Default	shapeSmpDesc = static_cast<CParticleSamplerDescriptor_Shape_Default*>(samplerData.m_SamplerDescriptor.Get());
	if (shapeSmpDesc == null)
	{
		shapeSmpDesc = PK_NEW(CParticleSamplerDescriptor_Shape_Default(shapeDescriptor.Get()));
		shapeSmpDesc->m_WorldTr_Current = &samplerData.m_CurrentMat;
		shapeSmpDesc->m_WorldTr_Previous = &samplerData.m_PreviousMat;
		samplerData.m_SamplerDescriptor = shapeSmpDesc;
	}
	else
	{
		shapeSmpDesc->m_Shape = shapeDescriptor;
	}

	samplerData.m_ShapeType = MeshShape;
	samplerData.m_MeshBatch = meshBatch;
	samplerData.m_SkinnedMesh = skinnedMesh;
	samplerData.m_ShapeDesc = shapeDescriptor;
	return m_Emitter->SetAttributeSampler(smpID, shapeSmpDesc);
}

//----------------------------------------------------------------------------

void		CPKFXEffect::_WaitForSwitchSamplers(SSamplerData *samplerData)
{
	CGuid	idx = m_RunningAsyncSetSamplers.IndexOf(samplerData);

	if (idx.Valid())
	{
		samplerData->WaitForAll();
		m_RunningAsyncSetSamplers.Remove(idx);
	}
}

//----------------------------------------------------------------------------

void		CPKFXEffect::_MeshUnityDataToMeshProxy(const SMeshDataToFill &from, SMeshProxy &to)
{
	Mem::Reinit(to);
	to.m_PrimitiveType = CMeshIStream::Triangles;
	to.m_IndexFormat = CMeshIStream::U32Indices;

	to.m_Indices = from.m_Indices;
	to.m_IndexCount = from.m_IdxCount;

	to.m_VertexCount = from.m_VtxCount;
	to.m_Positions = TStridedMemoryView<const CFloat3>(from.m_Positions, from.m_VtxCount);
//	to.m_Velocities;
	to.m_Normals = TStridedMemoryView<const CFloat3>(from.m_Normals, from.m_VtxCount);
	to.m_Tangents = TStridedMemoryView<const CFloat4>(from.m_Tangents, from.m_VtxCount);
	to.m_Texcoords[0] = TStridedMemoryView<const CFloat2>(from.m_UVs, from.m_VtxCount);
	to.m_Colors[0] = TStridedMemoryView<const CFloat4>(from.m_Colors, from.m_VtxCount);
}

//----------------------------------------------------------------------------

void		CPKFXEffect::SSamplerData::AsyncSwitchIFN()
{
	PK_SCOPEDLOCK(m_AsyncStateLock);
	if (m_AsyncState == SSamplerData::Sampler_Loaded)
	{
		m_AsyncState = SSamplerData::Sampler_CanSwitch;
		m_AsyncStateCondVar.NotifyOne();
	}
}

//----------------------------------------------------------------------------

bool		CPKFXEffect::SSamplerData::WaitSwitchIFN()
{
	CRuntimeManager::Instance().KickBackgroundTasksIFN();
	PK_SCOPEDLOCK(m_AsyncStateLock);
	while (m_AsyncState == SSamplerData::Sampler_CanSwitch)
	{
		// Wait for the thread to finish the sampler switch:
		m_AsyncStateCondVar.Wait(m_AsyncStateLock);
	}
	return m_AsyncState == SSamplerData::Sampler_Ready;
}

//----------------------------------------------------------------------------

void		CPKFXEffect::SSamplerData::WaitForAll()
{
	CRuntimeManager::Instance().KickBackgroundTasksIFN();
	// We check if we do not already have a sampler loading:
	PK_SCOPEDLOCK(m_AsyncStateLock);
	while (m_AsyncState == SSamplerData::Sampler_Loading)
	{
		// Wait for the thread to finish the load:
		m_AsyncStateCondVar.Wait(m_AsyncStateLock);
	}
	if (m_AsyncState == SSamplerData::Sampler_Loaded)
	{
		m_AsyncState = SSamplerData::Sampler_CanSwitch;
		m_AsyncStateCondVar.NotifyOne();
		while (m_AsyncState == SSamplerData::Sampler_CanSwitch)
		{
			// Wait for the thread to finish the sampler switch:
			m_AsyncStateCondVar.Wait(m_AsyncStateLock);
		}
	}
}

//----------------------------------------------------------------------------

CPKFXEffect::SSamplerData::SSamplerData()
{
	m_SamplerType = SamplerUnsupported;
	m_SamplerUsage = 0;
	m_SamplerDescriptor = null;
	// Sampler shapes:
	m_ShapeType = ShapeUnsupported;
	m_BonesCount = 0;
	m_MeshBatch = null;
	m_SkinnedMesh = null;
	m_CurrentMat = CFloat4x4::IDENTITY;
	m_PreviousMat = CFloat4x4::IDENTITY;
	// Async switch sampler:
	m_AsyncState = Sampler_Ready;
	// descs:
	m_ShapeDesc = null;
	m_ImageDesc = null;
	m_CurveDesc = null;
	m_DensitySampler = null;
}

//----------------------------------------------------------------------------

CPKFXEffect::SSamplerData::~SSamplerData()
{
	PK_SAFE_DELETE(m_CurveDesc);
	PK_SAFE_DELETE(m_ImageDesc);
	PK_SAFE_DELETE(m_DensitySampler);
}

//----------------------------------------------------------------------------

CPKFXEffect::SRuntimeMeshData::SRuntimeMeshData()
#if	(PK_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
:	m_SurfaceSamplingStructs(null)
,	m_SurfaceUVSamplingStructs(null)
#endif
#if 0
,	m_VolumeSamplingStructs(null)
,	m_ProjectionStructs(null)
#endif
#if (PK_GEOMETRICS_BUILD_KDTREE != 0)
,	m_KdTree(null)
#endif
,	m_MeshDataBuffer(null)
{
}

//----------------------------------------------------------------------------

CPKFXEffect::SRuntimeMeshData::~SRuntimeMeshData()
{
	Clean();
}

//----------------------------------------------------------------------------

#if	(PK_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)

bool	CPKFXEffect::SRuntimeMeshData::CreateSurfaceSamplingStructs(const SMeshProxy &mesh)
{
	m_SurfaceSamplingStructs = PK_NEW(CMeshSurfaceSamplerStructuresRandom);
	if (!PK_VERIFY(m_SurfaceSamplingStructs != null))
		return false;
	return m_SurfaceSamplingStructs->Build(mesh);
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SRuntimeMeshData::CreateSurfaceUVSamplingStructs(const SMeshProxy &mesh)
{
	m_SurfaceUVSamplingStructs = PK_NEW(CMeshSurfaceSamplerStructuresFromUV);
	if (!PK_VERIFY(m_SurfaceUVSamplingStructs != null))
		return false;
	return m_SurfaceUVSamplingStructs->Build(SMeshUV2PCBuildConfig(), mesh);
}

#endif

//----------------------------------------------------------------------------

#if 0

bool	CPKFXEffect::SRuntimeMeshData::CreateVolumeSamplingStructs(const SMeshProxy &mesh)
{
	m_VolumeSamplingStructs = PK_NEW(CMeshVolumeSamplerStructuresRandom);
	if (!PK_VERIFY(m_VolumeSamplingStructs != null))
		return false;
	return m_VolumeSamplingStructs->Build(mesh);
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SRuntimeMeshData::CreateProjectionStructs(const SMeshProxy &mesh)
{
	m_ProjectionStructs = PK_NEW(CMeshProjection);
	if (!PK_VERIFY(m_ProjectionStructs != null))
		return false;
	return m_ProjectionStructs->Build(mesh);
}

#endif

//----------------------------------------------------------------------------

#if (PK_GEOMETRICS_BUILD_KDTREE != 0)

bool	CPKFXEffect::SRuntimeMeshData::CreateKdTree(const SMeshProxy &mesh)
{
	m_KdTree = PK_NEW(CMeshKdTree);
	if (!PK_VERIFY(m_KdTree != null))
		return false;
	SMeshKdTreeBuildConfig	buildConfig;
	buildConfig.m_Flags |= SMeshKdTreeBuildConfig::LowQualityButFasterBuild;
	return m_KdTree->Build(mesh, buildConfig);
}

#endif

//----------------------------------------------------------------------------

void	CPKFXEffect::SRuntimeMeshData::Clean()
{
	// Acceleration structures are not owned by the integration:
#if	(PK_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
	PK_SAFE_DELETE(m_SurfaceSamplingStructs);
	PK_SAFE_DELETE(m_SurfaceUVSamplingStructs);
#endif
#if 0
	PK_SAFE_DELETE(m_VolumeSamplingStructs);
	PK_SAFE_DELETE(m_ProjectionStructs);
#endif
#if (PK_GEOMETRICS_BUILD_KDTREE != 0)
	PK_SAFE_DELETE(m_KdTree);
#endif
	if (m_MeshDataBuffer != null)
	{
		// Call dtor on SMeshDataToFill (default dtor for now, but just in case this changes one day):
		SMeshDataToFill		*meshData = static_cast<SMeshDataToFill*>(m_MeshDataBuffer);
		Mem::Destruct(*meshData);
		// Free buffer:
		PK_FREE(m_MeshDataBuffer);
		m_MeshDataBuffer = null;
	}
}

//----------------------------------------------------------------------------

CPKFXEffect::SSetSamplerTaskBase::SSetSamplerTaskBase()
:	m_SamplerId(0)
,	m_SamplerData(null)
{
}

//----------------------------------------------------------------------------

CPKFXEffect::SSetSamplerTaskBase::~SSetSamplerTaskBase()
{
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SSetSamplerTaskBase::BeforeLoad()
{
	// Skip the set sampler if it is on the main thread to avoid deadlocks (should never happen):
	if (!PK_VERIFY(!CCurrentThread::IsMainThread()))
	{
		PK_SCOPEDLOCK(m_SamplerData->m_AsyncStateLock);
		m_SamplerData->m_AsyncState = SSamplerData::Sampler_Ready;
		CLog::Log(PK_ERROR, "SSetBakedSamplerTask executed on the main thread: failed changing the sampler");
		return false;
	}
	{
		PK_ONLY_IF_ASSERTS
		(
			PK_SCOPEDLOCK(m_SamplerData->m_AsyncStateLock);
			PK_ASSERT(m_SamplerData->m_AsyncState == SSamplerData::Sampler_Loading);
		);
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CPKFXEffect::SSetSamplerTaskBase::BeforeSwitch()
{
	// Wait for the notification that we can switch the sampler:
	{
		PK_SCOPEDLOCK(m_SamplerData->m_AsyncStateLock);
		m_SamplerData->m_AsyncState = SSamplerData::Sampler_Loaded;
		m_SamplerData->m_AsyncStateCondVar.NotifyOne();
		while (m_SamplerData->m_AsyncState != SSamplerData::Sampler_CanSwitch)
		{
			m_SamplerData->m_AsyncStateCondVar.Wait(m_SamplerData->m_AsyncStateLock);
		}
		PK_ASSERT(m_SamplerData->m_AsyncState == SSamplerData::Sampler_CanSwitch);
	}
	return true;
}

//----------------------------------------------------------------------------

void	CPKFXEffect::SSetSamplerTaskBase::SamplerReady()
{
	PK_SCOPEDLOCK(m_SamplerData->m_AsyncStateLock);
	m_SamplerData->m_AsyncState = SSamplerData::Sampler_Ready;
	m_SamplerData->m_AsyncStateCondVar.NotifyOne();
}

//----------------------------------------------------------------------------

CPKFXEffect::SSetSamplerTask::SSetSamplerTask()
:	m_VtxBonesWeights(null)
,	m_VtxBonesIndices(null)
,	m_VtxCount(0)
,	m_BonesCount(0)
,	m_Size(CFloat3::ONE)
{

}

//----------------------------------------------------------------------------

CPKFXEffect::SSetSamplerTask::~SSetSamplerTask()
{
}

//----------------------------------------------------------------------------

void	CPKFXEffect::SSetSamplerTask::operator()()
{
	if (!BeforeLoad())
		return;

	PPkFxSkinnedMesh		skinnedMesh = null;
	u32						usageFlags = m_SamplerData->m_SamplerUsage;

	// Create the new sampler shape descriptor:
	PShapeDescriptor_Mesh	shapeDesc = PK_NEW(CShapeDescriptor_Mesh());

	if (!PK_VERIFY(shapeDesc != null))
	{
		CLog::Log(PK_ERROR, "Could not allocate CShapeDescriptor_Mesh");
		return;
	}

	shapeDesc->SetMeshProxy(m_Mesh);

	// ------------------------------------------
	// Build KD Tree IFN
	if ((usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_Intersect) != 0 ||
		(usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_Project) != 0 ||
		(usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_Contains) != 0 ||
		(usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_DistanceField) != 0)
	{
#if	(PK_GEOMETRICS_BUILD_KDTREE != 0)
		if (!m_MeshData.CreateKdTree(m_Mesh))
			CLog::Log(PK_WARN, "Failed building mesh kdTree acceleration structure");
		else
			shapeDesc->SetKdTree(m_MeshData.m_KdTree);
#endif
	}

#if	(PK_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
	// Build sampling info IFN
	if ((usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_Sample) != 0)
	{
		if (!m_MeshData.CreateSurfaceSamplingStructs(m_Mesh))
			CLog::Log(PK_WARN, "Failed building mesh surface-sampling acceleration structure");
		else
			shapeDesc->SetSamplingStructs(m_MeshData.m_SurfaceSamplingStructs, null);
#if		0	// We disable the volume sampling acceleration structure for now which seems a little bit more complex to build
		if (!samplerData.m_MeshData.CreateVolumeSamplingStructs(m_Mesh))
			CLog::Log(PK_WARN, "Failed building mesh surface-sampling acceleration structure");
#endif
	}
	// Build UV 2 PCoords info IFN
	if ((usageFlags & SParticleDeclaration::SSampler::UsageFlags_Mesh_SampleFromUV) != 0)
	{
		if (!m_MeshData.CreateSurfaceUVSamplingStructs(m_Mesh))
			CLog::Log(PK_WARN, "Failed building mesh uv-to-pcoords acceleration structure");
		else
			shapeDesc->SetUVSamplingStructs(m_MeshData.m_SurfaceUVSamplingStructs, 0);
	}
#endif

	//-------------------------------
	if (m_VtxBonesWeights != null &&
		m_VtxBonesIndices != null)
	{
		skinnedMesh = PK_NEW(CPkFxSkinnedMesh);

		if (!PK_VERIFY(skinnedMesh != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate CPkFxSkinnedMesh");
			return;
		}
		if (!PK_VERIFY(skinnedMesh->Init(	usageFlags,
											m_Mesh,
											m_VtxBonesWeights,
											m_VtxBonesIndices,
											m_VtxCount,
											4,
											m_BonesCount)))
		{
			CLog::Log(PK_ERROR, "Could not initialize the CPkFxSkinnedMesh");
			return;
		}
		// Now we can free the weights and indices buffer:
		PK_FREE(m_VtxBonesWeights);
		m_VtxBonesWeights = null;
		m_VtxBonesIndices = null;
	}
	if (skinnedMesh != null)
		shapeDesc->SetMesh(null, skinnedMesh->Override());
	shapeDesc->SetScale(m_Size);

	if (!BeforeSwitch())
		return;

	// Clear the old acceleration structures:
	m_SamplerData->m_MeshData.Clean();
	m_SamplerData->m_MeshData = m_MeshData;
	// Switch the sampler:
	PK_VERIFY(m_SamplerData->m_Effect->_SetSamplerShapeMesh(m_SamplerId, *m_SamplerData, shapeDesc, null, skinnedMesh));

	SamplerReady();
}

//----------------------------------------------------------------------------

CPKFXEffect::SSetBakedSamplerTask::SSetBakedSamplerTask()
:	m_SubmeshId(0)
,	m_BonesCount(0)
,	m_IsSkinned(false)
,	m_Size(CFloat3::ONE)
,	m_PkmmContent(null)
,	m_MemStream(null, 0)
{
}

//----------------------------------------------------------------------------

CPKFXEffect::SSetBakedSamplerTask::~SSetBakedSamplerTask()
{
}

//----------------------------------------------------------------------------

void		CPKFXEffect::SSetBakedSamplerTask::operator()()
{
	if (!BeforeLoad())
		return;

	PResourceMeshBatch			meshBatch = null;
	PPkFxSkinnedMesh			skinnedMesh = null;

	// Load the pkmm file:
	PShapeDescriptor_Mesh		descMesh = _LoadSamplerShapeMeshFromPkmm(	m_MemStream,
																			m_SubmeshId,
																			m_BonesCount,
																			m_IsSkinned,
																			m_SamplerData->m_SamplerUsage,
																			m_Size,
																			meshBatch,
																			skinnedMesh);

	PK_FREE(m_PkmmContent);
	m_PkmmContent = null;
	Mem::Destruct(m_MemStream);

	if (!BeforeSwitch())
		return;

	// Switch the sampler:
	PK_VERIFY(m_SamplerData->m_Effect->_SetSamplerShapeMesh(m_SamplerId, *m_SamplerData, descMesh, meshBatch, skinnedMesh));

	SamplerReady();
}

//----------------------------------------------------------------------------
__PK_API_END
