//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "UnityBatchDrawer_Light.h"
#include "FrameCollectorUnityTypes.h"
#include <RuntimeManager.h>
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <pk_particles/include/Storage/MainMemory/storage_ram.h>

//----------------------------------------------------------------------------

CUnityBatchDrawer_Light::CUnityBatchDrawer_Light()
{
}

//----------------------------------------------------------------------------

CUnityBatchDrawer_Light::~CUnityBatchDrawer_Light()
{
}

//----------------------------------------------------------------------------

bool		CUnityBatchDrawer_Light::Setup(const CRendererDataBase *renderer, const CParticleRenderMedium *owner, const PopcornFX::CFrameCollector *fc, const CStringId &storageClass)
{
	if (!CRendererBatchJobs_Light_Std::Setup(renderer, owner, fc, storageClass))
		return false;
	owner->m_OnRenderMediumActiveStateChanged += FastDelegate<void(CParticleRenderMedium*, bool)>(this, &CUnityBatchDrawer_Light::_OnRenderMediumActiveStateChanged);
	
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Light::AreRenderersCompatible(const PopcornFX::CRendererDataBase *rendererA, const PopcornFX::CRendererDataBase *rendererB) const
{
	const CUnityRendererCache	*firstMaterialDesc = static_cast<const CUnityRendererCache *>(rendererA->m_RendererCache.Get());
	const CUnityRendererCache	*secondMaterialDesc = static_cast<const CUnityRendererCache *>(rendererB->m_RendererCache.Get());
	return firstMaterialDesc == secondMaterialDesc || *firstMaterialDesc == *secondMaterialDesc;
}

bool CUnityBatchDrawer_Light::CanRender(PopcornFX::SRenderContext &ctx) const
{
	(void)ctx;
	return true;
}

void CUnityBatchDrawer_Light::BeginFrame(PopcornFX::SRenderContext &ctx)
{
	(void)ctx;
}

bool CUnityBatchDrawer_Light::EmitDrawCall(PopcornFX::SRenderContext &ctx, const PopcornFX::SDrawCallDesc & toEmit)
{
	(void)ctx;
	m_LightDatas.Clear();
	for (u32 i = 0; i < toEmit.m_DrawRequests.Count(); ++i)
	{
		const Drawers::SLight_DrawRequest *lightRequest = static_cast<const Drawers::SLight_DrawRequest *>(toEmit.m_DrawRequests[i]);
		if (lightRequest != null)
		{
			CUnityRendererCache *matCache = static_cast<CUnityRendererCache *>(toEmit.m_RendererCaches[i].Get());
			if (!PK_VERIFY(matCache != null))
				return true;
			_UpdateThread_IssueDrawCallLight(lightRequest, matCache);
		}
	}
	return true;
}

void CUnityBatchDrawer_Light::Destroy()
{
}

bool CUnityBatchDrawer_Light::AllocBuffers(SRenderContext &ctx)
{
	const SUnityRenderContext				&renderContext = static_cast<SUnityRenderContext&>(ctx);
	PK_SCOPEDPROFILE();

	if (!PK_VERIFY(renderContext.m_RenderApiData != null))
	{
		CLog::Log(PK_ERROR, "Unity render api data was not initialized: UnityPluginLoad was not called");
		return false;
	}

	PK_ASSERT(!DrawPass().m_DrawRequests.Empty());
	PK_ASSERT(DrawPass().m_DrawRequests.Count() == DrawPass().m_RendererCaches.Count());
	PK_ASSERT(DrawPass().m_DrawRequests.First() != null);

	return true;
}

void	CUnityBatchDrawer_Light::_OnRenderMediumActiveStateChanged(CParticleRenderMedium *renderMedium, bool active)
{
	(void) renderMedium; (void) active;
	//CRuntimeManager	&manager = CRuntimeManager::Instance();
	//if (m_PrevParticleCount != 0)
	//{
	//	for (u32 i = 0; i < m_UnityMeshInfoPerViews.Count(); ++i)
	//	{
	//		if (PK_VERIFY(m_UnityMeshInfoPerViews[i].m_RendererGUID != -1))
	//			manager.OnSetRendererActive(m_UnityMeshInfoPerViews[i].m_RendererGUID, ManagedBool_False);
	//	}
	//}
}

bool	CUnityBatchDrawer_Light::_UpdateThread_IssueDrawCallLight(const Drawers::SLight_DrawRequest *lightRequest, CUnityRendererCache *rdrCache)
{
	PK_SCOPEDPROFILE();
	(void)rdrCache;

	const u32		totalParticleCount = lightRequest->RenderedParticleCount();

	PK_ASSERT(!lightRequest->Empty());
	PK_ASSERT(totalParticleCount > 0);

	if (!m_LightDatas.Reserve(totalParticleCount))
	{
		PK_ASSERT_NOT_REACHED();
		return false;
	}

	const PopcornFX::Drawers::SLight_BillboardingRequest &bbRequest = static_cast<const PopcornFX::Drawers::SLight_BillboardingRequest &>(lightRequest->BaseBillboardingRequest());
	if (lightRequest->StorageClass() != PopcornFX::CParticleStorageManager_MainMemory::DefaultStorageClass())
	{
		PK_ASSERT_NOT_REACHED();
		return false;
	}

	PK_TODO("renderManager->CullTest");

	const PopcornFX::CParticleStreamToRender_MainMemory *lockedStream = lightRequest->StreamToRender_MainMemory();
	if (!PK_VERIFY(lockedStream != null)) // Light particles shouldn't handle GPU streams for now
		return true;

#if 0
	const PopcornFX::CGuid	volScatteringIntensityStreamId = bbRequest.StreamId(PopcornFX::CStringId("AffectsVolumetricFog.VolumetricScatteringIntensity")); // tmp
#endif

	static const float		kMinLightSize = 1e-3f;
	static const float		kLightRadiusMultiplier = 1.0f;

	const u32	pageCount = lockedStream->PageCount();
	for (u32 pagei = 0; pagei < pageCount; ++pagei)
	{
		const PopcornFX::CParticlePageToRender_MainMemory *page = lockedStream->Page(pagei);
		PK_ASSERT(page != null);
		const u32	pcount = page == null ? 0 : page->InputParticleCount();
		if (pcount == 0)
			continue;

		// Position
		TStridedMemoryView<const CFloat3>	positions = page->StreamForReading<CFloat3>(bbRequest.m_PositionStreamId);
		PK_ASSERT(positions.Count() == pcount);

		// Radius
		TStridedMemoryView<const float>		sizes;
		PK_ALIGN(0x10) float				defaultSize = 0.0f;
		if (PK_VERIFY(bbRequest.m_RangeStreamId.Valid()))
			sizes = page->StreamForReading<float>(bbRequest.m_RangeStreamId);
		else
			sizes = TStridedMemoryView<const float>(&defaultSize, pcount, 0);

		// Color
		TStridedMemoryView<const CFloat3>	colors(&CFloat3::ONE, pcount, 0);
		if (bbRequest.m_ColorStreamId.Valid())
			colors = TStridedMemoryView<const CFloat3>::Reinterpret(page->StreamForReading<CFloat4>(bbRequest.m_ColorStreamId));

		if (!PK_VERIFY(!positions.Empty()) ||
			!PK_VERIFY(!sizes.Empty()) ||
			!PK_VERIFY(!colors.Empty()))
			continue;

		const u8						enabledTrue = u8(-1);
		TStridedMemoryView<const u8>	enabledParticles = (bbRequest.m_EnabledStreamId.Valid()) ? page->StreamForReading<bool>(bbRequest.m_EnabledStreamId) : TStridedMemoryView<const u8>(&enabledTrue, pcount, 0);

		for (u32 parti = 0; parti < pcount; ++parti)
		{
			if (!enabledParticles[parti])
				continue;

			const float					radius = sizes[parti] * kLightRadiusMultiplier;
			if (radius < kMinLightSize)
				continue;

			PopcornFX::CGuid			ldatai = m_LightDatas.PushBack();
			SLightInfo					&lightdata = m_LightDatas[ldatai];

			lightdata.m_Position = positions[parti];
			lightdata.m_Color = CFloat4(colors[parti].Normalized(), 1.0f);
			lightdata.m_Range = radius;
			lightdata.m_Intensity = colors[parti].Length();
		}
	}

	::OnSetLightsBuffer(m_LightDatas.RawDataPointer(), totalParticleCount);
	return true;
}

bool CUnityBatchDrawer_Light::MapBuffers(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}

bool	CUnityBatchDrawer_Light::UnmapBuffers(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}

bool CUnityBatchDrawer_Light::LaunchCustomTasks(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}
