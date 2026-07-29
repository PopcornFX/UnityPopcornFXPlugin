//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved.
// https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "UnityBatchDrawer_Sound.h"
#include "FrameCollectorUnityTypes.h"
#include <RuntimeManager.h>
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include <pk_particles/include/Storage/MainMemory/storage_ram.h>

//----------------------------------------------------------------------------

CUnityBatchDrawer_Sound::CUnityBatchDrawer_Sound()
{
}

//----------------------------------------------------------------------------

CUnityBatchDrawer_Sound::~CUnityBatchDrawer_Sound()
{
}

//----------------------------------------------------------------------------

bool		CUnityBatchDrawer_Sound::Setup(const CRendererDataBase *renderer, const CParticleRenderMedium *owner, const PopcornFX::CFrameCollector *fc, const CStringId &storageClass)
{
	if (!CRendererBatchJobs_Sound_Std::Setup(renderer, owner, fc, storageClass))
		return false;
	owner->m_OnRenderMediumActiveStateChanged += FastDelegate<void(CParticleRenderMedium*, bool)>(this, &CUnityBatchDrawer_Sound::_OnRenderMediumActiveStateChanged);
	
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityBatchDrawer_Sound::AreRenderersCompatible(const PopcornFX::CRendererDataBase *rendererA, const PopcornFX::CRendererDataBase *rendererB) const
{
	const CUnityRendererCache	*firstMaterialDesc = static_cast<const CUnityRendererCache *>(rendererA->m_RendererCache.Get());
	const CUnityRendererCache	*secondMaterialDesc = static_cast<const CUnityRendererCache *>(rendererB->m_RendererCache.Get());
	return firstMaterialDesc == secondMaterialDesc || *firstMaterialDesc == *secondMaterialDesc;
}

bool CUnityBatchDrawer_Sound::CanRender(PopcornFX::SRenderContext &ctx) const
{
	(void)ctx;
	return true;
}

void CUnityBatchDrawer_Sound::BeginFrame(PopcornFX::SRenderContext &ctx)
{
	(void)ctx;
}

bool CUnityBatchDrawer_Sound::EmitDrawCall(PopcornFX::SRenderContext &ctx, const PopcornFX::SDrawCallDesc & toEmit)
{
	(void)ctx;
	u32 &particleCount = CRuntimeManager::Instance().GetScene().GetTotalSoundParticleCount();
	for (u32 i = 0; i < toEmit.m_DrawRequests.Count(); ++i)
	{
		const Drawers::SSound_DrawRequest	*soundRequest = static_cast<const Drawers::SSound_DrawRequest*>(toEmit.m_DrawRequests[i]);
		if (soundRequest != null)
		{
			CUnityRendererCache		*matCache = static_cast<CUnityRendererCache*>(toEmit.m_RendererCaches[i].Get());
			if (!PK_VERIFY(matCache != null))
				return true;
			_UpdateThread_IssueDrawCallSound(soundRequest, matCache, particleCount);
		}
	}

	return true;
}

void CUnityBatchDrawer_Sound::Destroy()
{
}

bool CUnityBatchDrawer_Sound::AllocBuffers(SRenderContext &ctx)
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

void	CUnityBatchDrawer_Sound::_OnRenderMediumActiveStateChanged(CParticleRenderMedium *renderMedium, bool active)
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

bool	CUnityBatchDrawer_Sound::_UpdateThread_IssueDrawCallSound(const Drawers::SSound_DrawRequest *soundRequest, CUnityRendererCache *rdrCache, u32 &particleCount)
{
	PK_SCOPEDPROFILE();
	TArray<SSoundInfo>	&soundDatas = CRuntimeManager::Instance().GetScene().GetSoundDatas();

	const u32		drawRequestParticleCount = soundRequest->RenderedParticleCount();
	PK_ASSERT(!soundRequest->Empty());
	PK_ASSERT(drawRequestParticleCount > 0);

	if (!soundDatas.Reserve(soundDatas.Count() + drawRequestParticleCount))
	{
		PK_ASSERT_NOT_REACHED();
		return false;
	}
	particleCount += drawRequestParticleCount;

	const Drawers::SSound_BillboardingRequest	&bbRequest = static_cast<const Drawers::SSound_BillboardingRequest&>(soundRequest->BaseBillboardingRequest());
	if (soundRequest->StorageClass() != CParticleStorageManager_MainMemory::DefaultStorageClass())
	{
		PK_ASSERT_NOT_REACHED();
		return false;
	}
	const CParticleStreamToRender_MainMemory	*lockedStream = soundRequest->StreamToRender_MainMemory();
	if (!PK_VERIFY(lockedStream != null))
		return true;

	const u32			pageCount = lockedStream->PageCount();

	for (u32 pagei = 0; pagei < pageCount; ++pagei)
	{
		const CParticlePageToRender_MainMemory	*page = lockedStream->Page(pagei);
		PK_ASSERT(page != null && !page->Empty());
		const u32		pcount = page->InputParticleCount();
		const float		dopplerFactor = bbRequest.m_DopplerFactor;
		const bool		isBlended = false;

		TStridedMemoryView<const float>		lifeRatios = page->StreamForReading<float>(bbRequest.m_LifeRatioStreamId);
		TStridedMemoryView<const float>		invLives = page->StreamForReading<float>(bbRequest.m_InvLifeStreamId);
		TStridedMemoryView<const CInt2>		selfIDs = page->StreamForReading<CInt2>(bbRequest.m_SelfIDStreamId);
		TStridedMemoryView<const CFloat3>	positions = page->StreamForReading<CFloat3>(bbRequest.m_PositionStreamId);

		TStridedMemoryView<const CFloat3>	velocities = bbRequest.m_VelocityStreamId.Valid() ? page->StreamForReading<CFloat3>(bbRequest.m_VelocityStreamId) : TStridedMemoryView<const CFloat3>(&CFloat4::ZERO.xyz(), positions.Count(), 0);
		TStridedMemoryView<const float>		volumes = bbRequest.m_VolumeStreamId.Valid() ? page->StreamForReading<float>(bbRequest.m_VolumeStreamId) : TStridedMemoryView<const float>(&CFloat4::ONE.x(), positions.Count(), 0);
		TStridedMemoryView<const float>		radii = bbRequest.m_RangeStreamId.Valid() ? page->StreamForReading<float>(bbRequest.m_RangeStreamId) : TStridedMemoryView<const float>(&CFloat4::ONE.x(), positions.Count(), 0);

		const u8										enabledTrue = u8(-1);
		TStridedMemoryView<const u8>					enabledParticles = (bbRequest.m_EnabledStreamId.Valid()) ? page->StreamForReading<bool>(bbRequest.m_EnabledStreamId) : TStridedMemoryView<const u8>(&enabledTrue, pcount, 0);

#if defined(PK_DEBUG)
		const u32	posCount = positions.Count();
		PK_ASSERT(
			posCount == pcount &&
			posCount == lifeRatios.Count() &&
			posCount == invLives.Count() &&
			posCount == selfIDs.Count() &&
			posCount == velocities.Count() &&
			posCount == volumes.Count() &&
			posCount == radii.Count());
#endif


		for (u32 parti = 0; parti < pcount; ++parti)
		{
			if (!enabledParticles[parti])
				continue;

			const float		volume = volumes[parti];

			const CFloat3	pos = positions[parti];
			const float		radius = radii[parti];

			const bool		audible = true;


			const float		soundId = 0;
			const float		soundIdFrac = PKFrac(soundId) * isBlended;

			PopcornFX::CGuid	ldatai = soundDatas.PushBack();

			SSoundInfo &soundInfo = soundDatas[ldatai];
			soundInfo.m_SelfID = selfIDs[parti];
			soundInfo.m_Age = lifeRatios[parti] / invLives[parti];
			soundInfo.m_Position = pos;
			soundInfo.m_Velocity = velocities[parti];
			soundInfo.m_Radius = radius;
			soundInfo.m_DopplerLevel = dopplerFactor;
			soundInfo.m_Volume = volume * (1.0f - soundIdFrac);
			soundInfo.m_Audible = audible;
			soundInfo.m_SoundData = (char*)rdrCache->m_MaterialDescSound.m_SoundData.ToStringData();

			if (!isBlended)
				continue;

			soundInfo.m_Volume = volume * soundIdFrac;
		}
	}

	return true;
}

bool CUnityBatchDrawer_Sound::MapBuffers(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}

bool	CUnityBatchDrawer_Sound::UnmapBuffers(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}

bool CUnityBatchDrawer_Sound::LaunchCustomTasks(SRenderContext &ctx)
{
	(void)ctx;
	return true;
}
