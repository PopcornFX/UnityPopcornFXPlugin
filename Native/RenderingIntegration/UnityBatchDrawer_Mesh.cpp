//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "UnityBatchDrawer_Mesh.h"
#include "FrameCollectorUnityTypes.h"
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <RuntimeManager.h>
#include <pk_render_helpers/include/render_features/rh_features_vat_static.h>
#include <pk_render_helpers/include/render_features/rh_features_vat_skeletal.h>

//----------------------------------------------------------------------------

CUnityBatchDrawer_Mesh::CUnityBatchDrawer_Mesh()
:	m_ParticleCount(0)
,	m_PrevParticleCount(0)
,	m_MeshIsValid(false)
,	m_HasAtlas(false)
,	m_HasAlphaMasks(false)
,	m_HasDissolve(false)
,	m_HasTransformUV(false)
,	m_HasUVDistortions(false)
,	m_UseSkeletalAnimData(false)
,	m_UseSkeletalAnimInterpolTracksData(false)
{
}

//----------------------------------------------------------------------------

CUnityBatchDrawer_Mesh::~CUnityBatchDrawer_Mesh()
{
}

//----------------------------------------------------------------------------

bool		CUnityBatchDrawer_Mesh::Setup(const CRendererDataBase *renderer, const CParticleRenderMedium *owner, const PopcornFX::CFrameCollector *fc, const CStringId &storageClass)
{
	if (!CRendererBatchJobs_Mesh_CPUBB::Setup(renderer, owner, fc, storageClass))
		return false;
	owner->m_OnRenderMediumActiveStateChanged += FastDelegate<void(CParticleRenderMedium*, bool)>(this, &CUnityBatchDrawer_Mesh::_OnRenderMediumActiveStateChanged);
	
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Mesh::AreRenderersCompatible(const PopcornFX::CRendererDataBase *rendererA, const PopcornFX::CRendererDataBase *rendererB) const
{
	const CUnityRendererCache	*firstMaterialDesc = static_cast<const CUnityRendererCache *>(rendererA->m_RendererCache.Get());
	const CUnityRendererCache	*secondMaterialDesc = static_cast<const CUnityRendererCache *>(rendererB->m_RendererCache.Get());
	return firstMaterialDesc == secondMaterialDesc || *firstMaterialDesc == *secondMaterialDesc;
}

bool CUnityBatchDrawer_Mesh::CanRender(PopcornFX::SRenderContext &ctx) const
{
	(void)ctx;
	return true;
}

void CUnityBatchDrawer_Mesh::BeginFrame(PopcornFX::SRenderContext &ctx)
{
	(void)ctx;
	_ClearBuffers();
}

bool CUnityBatchDrawer_Mesh::EmitDrawCall(PopcornFX::SRenderContext &ctx, const PopcornFX::SDrawCallDesc & toEmit)
{
	(void)ctx; (void)toEmit;
	return true;
}

void CUnityBatchDrawer_Mesh::Destroy()
{
	CRuntimeManager	&manager = CRuntimeManager::Instance();
	if (m_PrevParticleCount != 0)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
				manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_False);
		}
	}
}

void	CUnityBatchDrawer_Mesh::_OnRenderMediumActiveStateChanged(CParticleRenderMedium *renderMedium, bool active)
{
	(void) renderMedium; (void) active;
	CRuntimeManager	&manager = CRuntimeManager::Instance();
	if (m_PrevParticleCount != 0)
	{
		for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
		{
			if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
				manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_False);
		}
	}
}

void	CUnityBatchDrawer_Mesh::_ClearBuffers()
{
	m_PrevParticleCount = m_ParticleCount;
	m_ParticleCount = 0;
}

bool CUnityBatchDrawer_Mesh::AllocBuffers(SRenderContext &ctx)
{
	const PopcornFX::SRendererBatchDrawPass &drawPass = DrawPass();
	const SUnityRenderContext				&renderContext = static_cast<SUnityRenderContext&>(ctx);
	PK_SCOPEDPROFILE();

	if (!PK_VERIFY(renderContext.m_RenderApiData != null))
	{
		CLog::Log(PK_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}

	PK_ASSERT(!drawPass.m_DrawRequests.Empty());
	PK_ASSERT(drawPass.m_DrawRequests.Count() == drawPass.m_RendererCaches.Count());
	PK_ASSERT(m_BB_Mesh.MeshCount() > 0 || m_BB_Mesh.TotalParticleCount() > 0);
	PK_ASSERT(drawPass.m_DrawRequests.First() != null);

	// The first time the batch is used, we store the material desc in the batch (all the material descs should be the same):
	if (m_UnityMeshInfoPerViews.Count() == 0)
	{
		const CUnityRendererCache		*rendererCache = static_cast<const CUnityRendererCache *>(drawPass.m_RendererCaches.First().Get());
		const u32						viewCount = 1;
		m_MaterialDescMesh = rendererCache->m_MaterialDescMesh;

		if (!PK_VERIFY(m_UnityMeshInfoPerViews.Resize(viewCount)))
			return false;

		for (u32 i = 0; i < viewCount; ++i)
		{
			m_UnityMeshInfoPerViews[i] = rendererCache->m_UnityMeshInfoPerViews[i];
		}

		m_UnityMeshInfo = rendererCache->m_UnityMeshInfo;
	}

	m_ParticleCount = m_BB_Mesh.TotalParticleCount();

	{
		_UpdateThread_ResizeUnityMeshInstanceCount(drawPass, renderContext.m_RenderApiData);
	}
	return true;
}

bool CUnityBatchDrawer_Mesh::MapBuffers(SRenderContext &ctx)
{
	PK_SCOPEDPROFILE();
	(void)ctx;

	if (!m_MeshIsValid)
		return true;
	const PopcornFX::SRendererBatchDrawPass &drawPass = DrawPass();
	const PopcornFX::SGeneratedInputs	&toMap = drawPass.m_ToGenerate;
	return _RenderThread_SetupBuffersMeshes(toMap);
}

bool	CUnityBatchDrawer_Mesh::UnmapBuffers(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}

bool CUnityBatchDrawer_Mesh::LaunchCustomTasks(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}

//----------------------------------------------------------------------------

void CUnityBatchDrawer_Mesh::_UpdateThread_ResizeUnityMeshInstanceCount(const PopcornFX::SRendererBatchDrawPass &drawPass, IRenderAPIData *renderApiData)
{
	(void)renderApiData;
	PK_SCOPEDPROFILE();
	u32		perMeshDataSize = sizeof(CFloat4x4);
	bool	hasDiffuseColor = false;
	bool	hasEmissiveColor = false;
	bool	hasAlphaRemap = false;
	bool	hasVAT = false;
	u32		skeletalAnimMask = 0;
	bool	alphaMasksOrUVDistortionsAdded = false;

	PK_ASSERT((drawPass.m_ToGenerate.m_GeneratedInputs & Drawers::GenInput_Matrices) != 0);
	for (u32 i = 0; i < m_AdditionalInputs.Count(); ++i)
	{
		const SRendererFeatureFieldDefinition	&addInput = m_AdditionalInputs[i];

		if ((addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() || addInput.m_Name == BasicRendererProperties::SID_Diffuse_DiffuseColor()) && addInput.m_Type == BaseType_Float4)
		{
			perMeshDataSize += sizeof(CFloat4);
			hasDiffuseColor = true;
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Emissive_EmissiveColor() && (addInput.m_Type == BaseType_Float4 || addInput.m_Type == BaseType_Float3))
		{
			if (addInput.m_Type == BaseType_Float3)
				m_IsEmissive3 = true;
			perMeshDataSize += sizeof(CFloat4);
			hasEmissiveColor = true;
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Atlas_TextureID() && addInput.m_Type == BaseType_Float)
		{
			perMeshDataSize += sizeof(float);
			m_HasAtlas = true;
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaRemap_Cursor() && addInput.m_Type == BaseType_Float)
		{
			perMeshDataSize += sizeof(float);
			hasAlphaRemap = true;
		}
		else if ((addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_Cursor()
				|| addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Soft_Cursor()
				|| addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_Cursor())
				&& addInput.m_Type == BaseType_Float)
		{
			perMeshDataSize += sizeof(float);
			hasVAT = true;
		}
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimation_AnimationCursor() && addInput.m_Type == BaseType_Float)
			skeletalAnimMask |= (1 << 0);
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimation_CurrentAnimTrack() && addInput.m_Type == BaseType_I32)
			skeletalAnimMask |= (1 << 1);
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_NextAnimationCursor() && addInput.m_Type == BaseType_Float)
			skeletalAnimMask |= (1 << 2);
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_NextAnimTrack() && addInput.m_Type == BaseType_I32)
			skeletalAnimMask |= (1 << 3);
		else if (addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_TransitionRatio() && addInput.m_Type == BaseType_Float)
			skeletalAnimMask |= (1 << 4);
		else if (addInput.m_Name == BasicRendererProperties::SID_TransformUVs_UVRotate() && addInput.m_Type == BaseType_Float)
		{
			perMeshDataSize += sizeof(float);
			m_HasTransformUV = true;
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_TransformUVs_UVOffset() && addInput.m_Type == BaseType_Float2)
			perMeshDataSize += sizeof(CFloat2);
		else if (addInput.m_Name == BasicRendererProperties::SID_TransformUVs_UVScale() && addInput.m_Type == BaseType_Float2)
			perMeshDataSize += sizeof(CFloat2);
		// AlphaMasks and UVDistortion cursors are packed in a float4 even if both aren't enabled
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaMasks_Mask1AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			m_HasAlphaMasks = true;
			if (!alphaMasksOrUVDistortionsAdded)
			{
				perMeshDataSize += sizeof(CFloat4);
				alphaMasksOrUVDistortionsAdded = true;
			}
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaMasks_Mask2AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			m_HasAlphaMasks = true;
			if (!alphaMasksOrUVDistortionsAdded)
			{
				perMeshDataSize += sizeof(CFloat4);
				alphaMasksOrUVDistortionsAdded = true;
			}
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_UVDistortions_Distortion1AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			m_HasUVDistortions = true;
			if (!alphaMasksOrUVDistortionsAdded)
			{
				perMeshDataSize += sizeof(CFloat4);
				alphaMasksOrUVDistortionsAdded = true;	
			}	
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_UVDistortions_Distortion2AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			m_HasUVDistortions = true;
			if (!alphaMasksOrUVDistortionsAdded)
			{
				perMeshDataSize += sizeof(CFloat4);
				alphaMasksOrUVDistortionsAdded = true;	
			}
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Dissolve_DissolveAnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			perMeshDataSize += sizeof(float);
			m_HasDissolve = true;
		}
	}

	m_UseSkeletalAnimData = (skeletalAnimMask & 0x3) == 0x3;
	m_UseSkeletalAnimInterpolTracksData = (skeletalAnimMask & 0x1F) == 0x1F;

	if (m_UseSkeletalAnimData)
		perMeshDataSize += sizeof(CUint2);
	if (m_UseSkeletalAnimInterpolTracksData)
		perMeshDataSize += sizeof(CUint3);

	const int	rdrGUID = m_UnityMeshInfoPerViews[0].m_RendererGUID;
	if (!PK_VERIFY(rdrGUID != -1))
		return;

	const u32	overEstimatedInstanceCount = Mem::Align(drawPass.m_TotalParticleCount, 0x100);
	const u32	buffSize = overEstimatedInstanceCount * perMeshDataSize;

	if (m_MeshBillboardingBuffer == null || m_MeshBillboardingBuffer->DataSizeInBytes() < buffSize)
		m_MeshBillboardingBuffer = CRefCountedMemoryBuffer::AllocAligned(buffSize, 0x10);

	// Allocations can fail below, if the function returns because of this, m_MeshIsValid will be false
	m_MeshIsValid = false;

	if (!PK_VERIFY(m_PerMeshBuffers.Resize(m_BB_Mesh.PerMeshParticleCount().Count())))
		return;
	
	void *currentBuffPtr = m_MeshBillboardingBuffer->Data<void>();
	for (u32 subMesh = 0; subMesh < m_BB_Mesh.PerMeshParticleCount().Count(); ++subMesh)
	{
		u32						particleCount = m_BB_Mesh.PerMeshParticleCount()[subMesh];
		SMeshParticleBuffers	&meshBuff = m_PerMeshBuffers[subMesh];
		void					*inputOffset = Mem::AdvanceRawPointer(currentBuffPtr, m_BB_Mesh.PerMeshBufferOffset()[subMesh] * perMeshDataSize);

		::OnSetMeshInstancesBuffer(rdrGUID, subMesh, inputOffset);
		// Transform matrices:
		meshBuff.m_Transforms = TMemoryView<CFloat4x4>(static_cast<CFloat4x4*>(inputOffset), particleCount);
		inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(CFloat4x4));
		if (hasDiffuseColor)
		{
			// Diffuse colors:
			meshBuff.m_Colors = TMemoryView<CFloat4>(static_cast<CFloat4*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(CFloat4));
		}
		if (hasEmissiveColor)
		{
			// Emissive colors:
			meshBuff.m_EmissiveColors = TMemoryView<CFloat4>(static_cast<CFloat4*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(CFloat4));
		}
		if (hasAlphaRemap)
		{
			// Alpharemap cursor:
			meshBuff.m_AlphaRemapCursor = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (hasVAT)
		{
			// VAT cursor:
			meshBuff.m_VATCursors = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (m_HasAtlas)
		{
			// Atlas ID:
			meshBuff.m_AtlasId = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (m_UseSkeletalAnimData)
		{
			meshBuff.m_AnimIdx0 = TMemoryView<u32>(static_cast<u32*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(u32));
			meshBuff.m_AnimCursor0 = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (m_UseSkeletalAnimInterpolTracksData)
		{
			meshBuff.m_AnimIdx1 = TMemoryView<u32>(static_cast<u32*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(u32));
			meshBuff.m_AnimCursor1 = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
			meshBuff.m_TransitionCursor = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (m_HasTransformUV)
		{
			meshBuff.m_TransformUVRotate = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
			meshBuff.m_TransformUVOffset = TMemoryView<CFloat2>(static_cast<CFloat2*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(CFloat2));
			meshBuff.m_TransformUVScale = TMemoryView<CFloat2>(static_cast<CFloat2*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(CFloat2));

		}
		// AlphaMasks and UVDistortions cursors are packed in a Vector4 to be used as a vertex attribute, even if there is only one feature enabled
		if (m_HasAlphaMasks || m_HasUVDistortions) 
		{
			meshBuff.m_AlphaMask1AnimationCursor = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
			meshBuff.m_AlphaMask2AnimationCursor = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
			meshBuff.m_UVDistortion1AnimationCursor = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
			meshBuff.m_UVDistortion2AnimationCursor = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (m_HasDissolve)
		{
			meshBuff.m_DissolveCursor = TMemoryView<float>(static_cast<float*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}
		if (m_HasAtlas && (m_HasAlphaMasks || m_HasUVDistortions || m_HasDissolve))
		{
			meshBuff.m_RawUV0 = TMemoryView<CFloat2>(static_cast<CFloat2*>(inputOffset), particleCount);
			inputOffset = Mem::AdvanceRawPointer(inputOffset, particleCount * sizeof(float));
		}

		::OnSetMeshInstancesCount(rdrGUID, subMesh, particleCount);
	}

	// Restore m_MeshIsValid
	m_MeshIsValid = true;

	if (m_PrevParticleCount == 0 && m_ParticleCount != 0)
	{
		CRuntimeManager	&manager = CRuntimeManager::Instance();

		if (PK_VERIFY(m_UnityMeshInfoPerViews[0].m_RendererGUID != -1))
		{
			manager.OnSetRendererActive(m_UnityMeshInfoPerViews[0].m_RendererGUID, ManagedBool_True);
		}
	}
}

bool	CUnityBatchDrawer_Mesh::_RenderThread_SetupBuffersMeshes(const SGeneratedInputs &toMap)
{
	(void)toMap;

	PK_SCOPEDPROFILE();
	if (m_PerMeshBuffers.Count() == 0)
		return false;

	m_MeshAdditionalField.Clear();
	for (u32 i = 0; i < m_AdditionalInputs.Count(); ++i)
	{
		const SRendererFeatureFieldDefinition	 &addInput = m_AdditionalInputs[i];

		if ((addInput.m_Name == BasicRendererProperties::SID_Diffuse_Color() || addInput.m_Name == BasicRendererProperties::SID_Diffuse_DiffuseColor()) && addInput.m_Type == BaseType_Float4)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_Colors), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_AlphaRemap_Cursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AlphaRemapCursor), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (addInput.m_Name == BasicRendererProperties::SID_Emissive_EmissiveColor() && (addInput.m_Type == BaseType_Float3 || addInput.m_Type == BaseType_Float4))
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_EmissiveColors), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if ((addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Fluid_Cursor()
			|| addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Soft_Cursor()
			|| addInput.m_Name == VertexAnimationRendererProperties::SID_VertexAnimation_Rigid_Cursor())
			&& addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_VATCursors), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasAtlas && addInput.m_Name == BasicRendererProperties::SID_Atlas_TextureID() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage *>(&m_PerMeshBuffers.First().m_AtlasId), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimation_AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AnimCursor0), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimation_CurrentAnimTrack() && addInput.m_Type == BaseType_I32)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AnimIdx0), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimInterpolTracksData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_NextAnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AnimCursor1), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimInterpolTracksData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_NextAnimTrack() && addInput.m_Type == BaseType_I32)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AnimIdx1), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_UseSkeletalAnimInterpolTracksData && addInput.m_Name == SkeletalAnimationTexture::SID_SkeletalAnimationInterpolateTracks_TransitionRatio() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_TransitionCursor), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasTransformUV && addInput.m_Name == BasicRendererProperties::SID_TransformUVs_UVRotate() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_TransformUVRotate), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasTransformUV && addInput.m_Name == BasicRendererProperties::SID_TransformUVs_UVOffset() && addInput.m_Type == BaseType_Float2)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_TransformUVOffset), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasTransformUV && addInput.m_Name == BasicRendererProperties::SID_TransformUVs_UVScale() && addInput.m_Type == BaseType_Float2)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_TransformUVScale), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasAlphaMasks && addInput.m_Name == BasicRendererProperties::SID_AlphaMasks_Mask1AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AlphaMask1AnimationCursor), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasAlphaMasks && addInput.m_Name == BasicRendererProperties::SID_AlphaMasks_Mask2AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_AlphaMask2AnimationCursor), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasUVDistortions && addInput.m_Name == BasicRendererProperties::SID_UVDistortions_Distortion1AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_UVDistortion1AnimationCursor), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasUVDistortions && addInput.m_Name == BasicRendererProperties::SID_UVDistortions_Distortion2AnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_UVDistortion2AnimationCursor), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
		else if (m_HasDissolve && addInput.m_Name == BasicRendererProperties::SID_Dissolve_DissolveAnimationCursor() && addInput.m_Type == BaseType_Float)
		{
			if (!PK_VERIFY(m_MeshAdditionalField.PushBack().Valid()))
				return false;
			m_MeshAdditionalField.Last().m_AdditionalInputIndex = i;
			m_MeshAdditionalField.Last().m_Storage = TStridedMemoryView<SStridedMemoryViewRawStorage>(reinterpret_cast<SStridedMemoryViewRawStorage*>(&m_PerMeshBuffers.First().m_DissolveCursor), m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
		}
	}
	
	m_BBJobs_Mesh.m_Exec_Matrices.m_MatricesPerMesh = TStridedMemoryView<TStridedMemoryView<CFloat4x4> >(&m_PerMeshBuffers.First().m_Transforms, m_PerMeshBuffers.Count(), sizeof(SMeshParticleBuffers));
	if (!m_MeshAdditionalField.Empty())
	{
		m_BBJobs_Mesh.m_Exec_CopyField.m_FieldsToCopyPerMesh = m_MeshAdditionalField.View();
	}
	return true;
}
