//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "UnityBatchDrawer_Decal.h"
#include "FrameCollectorUnityTypes.h"
#include <RuntimeManager.h>
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <pk_particles/include/Storage/MainMemory/storage_ram.h>

//----------------------------------------------------------------------------

CUnityBatchDrawer_Decal::CUnityBatchDrawer_Decal()
:	m_BBox(CAABB::DEGENERATED)
,	m_VertexCount(0)
,	m_IndexCount(0)
,	m_ParticleCount(0)
,	m_PrevParticleCount(0)
,	m_MeshIsValid(false)
{
}

//----------------------------------------------------------------------------

CUnityBatchDrawer_Decal::~CUnityBatchDrawer_Decal()
{
}

//----------------------------------------------------------------------------

bool		CUnityBatchDrawer_Decal::Setup(const CRendererDataBase *renderer, const CParticleRenderMedium *owner, const PopcornFX::CFrameCollector *fc, const CStringId &storageClass)
{
	if (!CRendererBatchJobs_Decal_CPUBB::Setup(renderer, owner, fc, storageClass))
		return false;
	owner->m_OnRenderMediumActiveStateChanged += FastDelegate<void(CParticleRenderMedium*, bool)>(this, &CUnityBatchDrawer_Decal::_OnRenderMediumActiveStateChanged);
	
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Decal::AreRenderersCompatible(const PopcornFX::CRendererDataBase *rendererA, const PopcornFX::CRendererDataBase *rendererB) const
{
	const CUnityRendererCache	*firstMaterialDesc = static_cast<const CUnityRendererCache *>(rendererA->m_RendererCache.Get());
	const CUnityRendererCache	*secondMaterialDesc = static_cast<const CUnityRendererCache *>(rendererB->m_RendererCache.Get());
	return firstMaterialDesc == secondMaterialDesc || *firstMaterialDesc == *secondMaterialDesc;
}

bool CUnityBatchDrawer_Decal::CanRender(PopcornFX::SRenderContext &ctx) const
{
	(void)ctx;
	return true;
}

void CUnityBatchDrawer_Decal::BeginFrame(PopcornFX::SRenderContext &ctx)
{
	(void)ctx;
	_ClearBuffers();
}

bool CUnityBatchDrawer_Decal::EmitDrawCall(PopcornFX::SRenderContext &ctx, const PopcornFX::SDrawCallDesc & toEmit)
{
	(void)ctx;
	u32 &totalParticleCount = CRuntimeManager::Instance().GetScene().GetTotalDecalParticleCount();
	for (u32 i = 0; i < toEmit.m_DrawRequests.Count(); ++i)
	{
		const Drawers::SDecal_DrawRequest *DecalRequest = static_cast<const Drawers::SDecal_DrawRequest*>(toEmit.m_DrawRequests[i]);
		if (DecalRequest != null)
		{
			CUnityRendererCache *matCache = static_cast<CUnityRendererCache*>(toEmit.m_RendererCaches[i].Get());
			if (!PK_VERIFY(matCache != null))
				return true;
			_UpdateThread_IssueDrawCallDecal(DecalRequest, matCache, totalParticleCount);
		}
	}

	return true;
}

void CUnityBatchDrawer_Decal::Destroy()
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

void	CUnityBatchDrawer_Decal::_OnRenderMediumActiveStateChanged(CParticleRenderMedium *renderMedium, bool active)
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

void	CUnityBatchDrawer_Decal::_ClearBuffers()
{
	m_IndexCount = 0;
	m_VertexCount = 0;
	m_PrevParticleCount = m_ParticleCount;
	m_ParticleCount = 0;
}

bool CUnityBatchDrawer_Decal::_UpdateThread_IssueDrawCallDecal(const Drawers::SDecal_DrawRequest *decalRequest, CUnityRendererCache *rdrCache, u32 &totalParticleCount)
{
	TArray<SDecalInfo>  &decalDatas				 = CRuntimeManager::Instance().GetScene().GetDecalDatas();
	const u32			drawRequestParticleCount = decalRequest->RenderedParticleCount();

	PK_ASSERT(!decalRequest->Empty());
	PK_ASSERT(drawRequestParticleCount > 0);

	if (!decalDatas.Reserve(decalDatas.Count() + drawRequestParticleCount))
	{
		PK_ASSERT_NOT_REACHED();
		return false;
	}

	totalParticleCount += drawRequestParticleCount;

	const PopcornFX::Drawers::SDecal_BillboardingRequest& bbRequest = static_cast<const PopcornFX::Drawers::SDecal_BillboardingRequest&>(decalRequest->BaseBillboardingRequest());
	if (decalRequest->StorageClass() != PopcornFX::CParticleStorageManager_MainMemory::DefaultStorageClass())
	{
		PK_ASSERT_NOT_REACHED();
		return false;
	}

	const PopcornFX::CParticleStreamToRender_MainMemory *lockedStream = decalRequest->StreamToRender_MainMemory();

	const u32	pageCount = lockedStream->PageCount();
	for (u32 pagei = 0; pagei < pageCount; ++pagei)
	{
		const PopcornFX::CParticlePageToRender_MainMemory *page = lockedStream->Page(pagei);
		PK_ASSERT(page != null);
		const u32	pcount = page == null ? 0 : page->InputParticleCount();
		if (pcount == 0)
			continue;

		// Position
		TStridedMemoryView<const CFloat3>		positions	  = page->StreamForReading<CFloat3>(bbRequest.m_PositionStreamId);
		// Scales
		TStridedMemoryView<const CFloat3>		scales		  = page->StreamForReading<CFloat3>(bbRequest.m_ScaleStreamId);
		// Orientation
		TStridedMemoryView<const CQuaternion>	orientations  = page->StreamForReading<CQuaternion>(bbRequest.m_OrientationStreamId);


		CGuid legacyDiffuseColorId  = decalRequest->StreamId(BasicRendererProperties::SID_Diffuse_Color());
		CGuid diffuseColorId = decalRequest->StreamId(BasicRendererProperties::SID_Diffuse_DiffuseColor());
		CGuid emissiveColorId = decalRequest->StreamId(BasicRendererProperties::SID_Emissive_EmissiveColor());
		CGuid atlasTextureId = decalRequest->StreamId(BasicRendererProperties::SID_Atlas_TextureID());

		// Diffuse Color
		TStridedMemoryView<const CFloat4> diffuseColor;
		bool hasLegacyDiffuseColor = legacyDiffuseColorId.Valid();
		bool hasDiffuseColor = diffuseColorId.Valid();

		if (hasLegacyDiffuseColor)
			diffuseColor = page->StreamForReading<CFloat4>(legacyDiffuseColorId);

		if (hasDiffuseColor)
			diffuseColor = page->StreamForReading<CFloat4>(diffuseColorId);

		// Emissive Color
		TStridedMemoryView<const CFloat3> emissiveColor;
		bool hasEmissiveColor = emissiveColorId.Valid();
		
		if (hasEmissiveColor)
		{
			emissiveColor = page->StreamForReading<CFloat3>(emissiveColorId);
		}

		// Atlas ID
		TStridedMemoryView<const CFloat1> atlasId;
		bool hasAtlas = atlasTextureId.Valid();
		atlasId = page->StreamForReading<CFloat1>(atlasTextureId);

		PK_ASSERT(positions.Count() == pcount);

		const u8						enabledTrue = u8(-1);
		TStridedMemoryView<const u8>	enabledParticles = (bbRequest.m_EnabledStreamId.Valid()) ? page->StreamForReading<bool>(bbRequest.m_EnabledStreamId) : TStridedMemoryView<const u8>(&enabledTrue, pcount, 0);

		for (u32 parti = 0; parti < pcount; ++parti)
		{
			if (!enabledParticles[parti])
				continue;


			PopcornFX::CGuid ldatai = decalDatas.PushBack();
			SDecalInfo &decalData = decalDatas[ldatai];

			decalData.m_Position	  = positions[parti];

			// Invert y and z axis since in Unity the forward projection is inverted compare to popcornFX .
			CFloat3 tempScale		  = scales[parti];
			const float scaleY		  = tempScale.y();
			tempScale.y()			  = tempScale.z();
			tempScale.z()			  = scaleY;
			decalData.m_Scale		  = tempScale;

			decalData.m_Orientation	   = orientations[parti];

			if (rdrCache->m_MaterialDescDecal.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_DiffuseMap) && hasDiffuseColor)
			{
				decalData.m_DiffuseColor = diffuseColor[parti];
			}

			if (rdrCache->m_MaterialDescDecal.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Emissive) && hasEmissiveColor)
			{
				decalData.m_EmissiveColor = emissiveColor[parti];
			}

			if (rdrCache->m_MaterialDescDecal.m_Flags.HasShaderVariationFlags(ShaderVariationFlags::Has_Atlas) && hasAtlas)
			{
				decalData.m_AtlasID = atlasId[parti];
			}
			decalData.m_UID			  = rdrCache->m_UID;
		}
	}
	return true;
}

bool CUnityBatchDrawer_Decal::AllocBuffers(SRenderContext &ctx)
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
	PK_ASSERT(m_BB_Decal.TotalParticleCount() > 0);
	PK_ASSERT(drawPass.m_DrawRequests.First() != null);

	// The first time the batch is used, we store the material desc in the batch (all the material descs should be the same):
	if (m_UnityMeshInfoPerViews.Count() == 0)
	{
		const CUnityRendererCache	*rendererCache = static_cast<const CUnityRendererCache *>(drawPass.m_RendererCaches.First().Get());
		const u32					viewCount = 1;
		m_MaterialDescDecal = rendererCache->m_MaterialDescDecal;

		if (!PK_VERIFY(m_Exec_SAO2AOS.Resize(viewCount)) ||
			!PK_VERIFY(m_UnityMeshInfoPerViews.Resize(viewCount)))
			return false;

		for (u32 i = 0; i < viewCount; ++i)
		{
			m_Exec_SAO2AOS[i].Clear();
			m_UnityMeshInfoPerViews[i] = rendererCache->m_UnityMeshInfoPerViews[i];
		}

		m_UnityMeshInfo = rendererCache->m_UnityMeshInfo;
	}

	m_ParticleCount = m_BB_Decal.TotalParticleCount();

	return true;
}

bool CUnityBatchDrawer_Decal::MapBuffers(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}

bool	CUnityBatchDrawer_Decal::UnmapBuffers(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}

bool CUnityBatchDrawer_Decal::LaunchCustomTasks(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}
