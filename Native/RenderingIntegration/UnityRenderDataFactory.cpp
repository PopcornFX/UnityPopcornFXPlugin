//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------


#include "precompiled.h"
#include "UnityRenderDataFactory.h"
#include "UnityRendererCache.h"
#include "UnityBillboardingBatchPolicy.h"

#include <pk_render_helpers/include/batches/rh_ribbon_batch.h>
#include <pk_render_helpers/include/batches/rh_billboard_batch.h>
#include <pk_render_helpers/include/batches/rh_mesh_batch.h>
#include <pk_render_helpers/include/batches/rh_triangle_batch.h>

#include <pk_particles/include/ps_event_map.h>

#include <RuntimeManager.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

PRendererCacheBase	CUnityRenderDataFactory::UpdateThread_CreateRendererCache(const PRendererDataBase &renderer, const CParticleDescriptor *particleDesc)
{
	PUnityRendererCache		rendererCache = PK_NEW(CUnityRendererCache(this));
	CRuntimeManager			&RTManager = CRuntimeManager::Instance();

	const TArray<SUnitySceneView>	&views = RTManager.GetScene().SceneViews();
	UnityGfxRenderer			deviceType = RTManager.GetDeviceType();

	if (renderer->m_RendererType != Renderer_Mesh)
	{
		u32 count = views.Empty() ? 1 : views.Count();
		if (!PK_VERIFY(rendererCache->m_UnityMeshInfoPerViews.Resize(count)))
			return null;
		for (u32 i = 0; i < count; ++i)
		{
			rendererCache->m_UnityMeshInfoPerViews[i].Init(deviceType);
		}
	}
	else
	{
		if (!PK_VERIFY(rendererCache->m_UnityMeshInfoPerViews.Resize(1)))
			return null;
		rendererCache->m_UnityMeshInfoPerViews[0].Init(deviceType);
	}

	
	renderer->m_RendererCache = rendererCache;

	bool succeeded = false;

	if (renderer->m_RendererType == Renderer_Billboard)
	{
		succeeded = rendererCache->GameThread_SetupRenderer(static_cast<const CRendererDataBillboard*>(renderer.Get()));
	}
	else if (renderer->m_RendererType == Renderer_Ribbon)
	{
		succeeded = rendererCache->GameThread_SetupRenderer(static_cast<const CRendererDataRibbon*>(renderer.Get()));
	}
	else if (renderer->m_RendererType == Renderer_Mesh)
	{
		succeeded = rendererCache->GameThread_SetupRenderer(static_cast<const CRendererDataMesh*>(renderer.Get()));
	}
	else if (renderer->m_RendererType == Renderer_Triangle)
	{
		succeeded = rendererCache->GameThread_SetupRenderer(static_cast<const CRendererDataTriangle*>(renderer.Get()));
	}
	rendererCache->SetAssetName(particleDesc->ParentEffect()->HandlerName());

	CResourceManager	*resourceManager = particleDesc->ParentEffect()->Context()->ResourceManager();

	if (renderer->m_RendererType == ERendererClass::Renderer_Billboard ||
		renderer->m_RendererType == ERendererClass::Renderer_Ribbon ||
		renderer->m_RendererType == ERendererClass::Renderer_Mesh ||
		renderer->m_RendererType == ERendererClass::Renderer_Decal)
	{
		rendererCache->UpdateThread_LoadRendererAtlas(renderer, resourceManager); // Returns false if atlas couldn't be created but was required by rfeature
	}
	if (!succeeded)
		return null;

	const PCEventConnectionMap	eventConnectionMap = particleDesc->ParentEffect()->EventConnectionMap();
	u32							rendererCount = 0;

	for (const CEventConnectionMap::SLayerDefinition	&layerDef : eventConnectionMap->m_LayerSlots)
	{
		PParticleDescriptor	descriptor = layerDef.m_ParentDescriptor;
		auto	it = descriptor->Renderers().Begin();
		while (it != descriptor->Renderers().End())
		{
			const PRendererDataBase			&currentRenderer = *it;

			if (currentRenderer.Get() == renderer.Get())
			{
				//Check if rdr got custom material
				ManagedBool	hasCustomMaterial = ManagedBool_False;
				int			customMaterialID = -1;

				if (renderer->m_RendererType == Renderer_Billboard ||
					renderer->m_RendererType == Renderer_Ribbon ||
					renderer->m_RendererType == Renderer_Triangle)
				{
					SPopcornRendererDesc	desc;
					if (!rendererCache->GetRendererInfo(desc))
						continue;

					::OnRetrieveCustomMaterialInfo(renderer->m_RendererType, &desc, rendererCount, &hasCustomMaterial, &customMaterialID);
				}
				else if (renderer->m_RendererType == Renderer_Mesh)
				{
					SMeshRendererDesc	desc;
					if (!rendererCache->GetRendererInfo(desc))
						continue;

					::OnRetrieveCustomMaterialInfo(renderer->m_RendererType, &desc, rendererCount, &hasCustomMaterial, &customMaterialID);

				}

				rendererCache->m_HasCustomMat = (hasCustomMaterial == ManagedBool_True ? true : false);
				rendererCache->m_CustomMatID = customMaterialID;
				//Check if a batchable renderer cache exist
				{
					PK_SCOPEDLOCK_READ(m_CacheLock);
					for (u32 j = 0; j < m_RendererCaches.Count(); ++j)
					{
						if (*m_RendererCaches[j] == *rendererCache)
						{
							return m_RendererCaches[j];
						}
					}
				}
				rendererCache->CreateUnityMesh(rendererCount, m_UseGPUBillboarding);
				{
					PK_SCOPEDLOCK_WRITE(m_CacheLock);
					m_RendererCaches.PushBack(rendererCache);
				}
				return rendererCache;
			}
			++rendererCount;
			++it;
		}
	}
	return null;
}

void	CUnityRenderDataFactory::UpdateThread_CollectedForRendering(const PRendererCacheBase &rendererCache)
{
	(void)rendererCache;
}

CUnityRenderDataFactory::CBillboardingBatchInterface	*CUnityRenderDataFactory::CreateBillboardingBatch(ERendererClass rendererType, const PRendererCacheBase &rendererCache, bool gpuStorage)
{
	(void)rendererCache;
	CUnityBillboardingBatchPolicy	*policy = null;
	CBillboardingBatchInterface		*retValue = null;

	// Default billboarding batch implementations:
	typedef	TBillboardBatch<CUnityParticleBatchTypes,	CUnityBillboardingBatchPolicy>	CBillboardBillboardingBatch;
	typedef	TRibbonBatch<	CUnityParticleBatchTypes,	CUnityBillboardingBatchPolicy>	CRibbonBillboardingBatch;
	typedef	TMeshBatch<		CUnityParticleBatchTypes,	CUnityBillboardingBatchPolicy>	CMeshBillboardingBatch;
	typedef TTriangleBatch<	CUnityParticleBatchTypes,	CUnityBillboardingBatchPolicy>	CTriangleBillboardingBatch;

	if (!gpuStorage)
	{
		switch (rendererType)
		{
		case	Renderer_Billboard:
		{
			CBillboardBillboardingBatch	*batch = PK_NEW(CBillboardBillboardingBatch);

			batch->SetBillboardingLocation(m_UseGPUBillboarding ? Drawers::BillboardingLocation_GeometryShader /* VertexShader*/ : Drawers::BillboardingLocation_CPU);

			policy = static_cast<CUnityBillboardingBatchPolicy*>(batch);
			retValue = static_cast<CBillboardingBatchInterface*>(batch);
		}
		break;
		case	Renderer_Ribbon:
		{
			CRibbonBillboardingBatch	*batch = PK_NEW(CRibbonBillboardingBatch);

			policy = static_cast<CUnityBillboardingBatchPolicy*>(batch);
			retValue = static_cast<CBillboardingBatchInterface*>(batch);
		}
		break;
		case	Renderer_Mesh:
		{
			CMeshBillboardingBatch		*batch = PK_NEW(CMeshBillboardingBatch);

			policy = static_cast<CUnityBillboardingBatchPolicy*>(batch);
			retValue = static_cast<CBillboardingBatchInterface*>(batch);
		}
		break;
		case	Renderer_Triangle:
		{
			CTriangleBillboardingBatch	*batch = PK_NEW(CTriangleBillboardingBatch);

			policy = static_cast<CUnityBillboardingBatchPolicy*>(batch);
			retValue = static_cast<CBillboardingBatchInterface*>(batch);
		}
		break;
		default:
			return null;
		}
	}
	if (policy != null)
	{
		policy->Init(CRuntimeManager::Instance().GetDeviceType(), this);
		PK_SCOPEDLOCK_WRITE(m_CacheLock);
		m_Batches.PushBack(policy);
	}
	return retValue;
}

void	CUnityRenderDataFactory::SetDelayUnityCallbacks(bool delayUnityCallbacks)
{
	m_DelayUnityCallbacks = delayUnityCallbacks;
}

bool	CUnityRenderDataFactory::GetDelayUnityCallbacks() const
{
	return m_DelayUnityCallbacks;
}

void	CUnityRenderDataFactory::Reset()
{
	PK_SCOPEDLOCK_WRITE(m_CacheLock);
	m_RendererCaches.Clear();
	m_Batches.Clear();
}

void	CUnityRenderDataFactory::EmptyAllBatches()
{
	PK_SCOPEDLOCK_READ(m_CacheLock);
	for (u32 i = 0; i < m_Batches.Count(); ++i)
	{
		m_Batches[i]->ClearBatch();
	}
}

void	CUnityRenderDataFactory::CustomStepFlagInactive()
{
	PK_SCOPEDLOCK_READ(m_CacheLock);
	for (u32 i = 0; i < m_Batches.Count(); ++i)
	{
		m_Batches[i]->CustomStepFlagInactive();
	}
}

void	CUnityRenderDataFactory::RemoveRendererCache(const PCUnityRendererCache &cache)
{
	PK_SCOPEDLOCK_WRITE(m_CacheLock);
	m_RendererCaches.RemoveElement(cache);
}

void	CUnityRenderDataFactory::RemoveBatch(CUnityBillboardingBatchPolicy *batch)
{
	PK_SCOPEDLOCK_WRITE(m_CacheLock);
	m_Batches.RemoveElement(batch);
}

//----------------------------------------------------------------------------
__PK_API_END
