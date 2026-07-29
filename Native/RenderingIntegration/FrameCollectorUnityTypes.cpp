//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL.
// https://popcornfx.com/popcornfx-community-license/
//----------------------------------------------------------------------------


#include "precompiled.h"
#include "FrameCollectorUnityTypes.h"
#include <RuntimeManager.h>
#include <pk_particles/include/ps_event_map.h>
#include <pk_render_helpers/include/render_features/rh_features_basic.h>
#include "UnityBatchDrawer_Billboard.h"
#include "UnityBatchDrawer_Billboard_GPU.h"
#include "UnityBatchDrawer_Ribbon.h"
#include "UnityBatchDrawer_Decal.h"
#include "UnityBatchDrawer_Triangle.h"
#include "UnityBatchDrawer_Light.h"
#include "UnityBatchDrawer_Sound.h"
#include "UnityBatchDrawer_Mesh.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

CUnityFrameCollector::CUnityFrameCollector()
	: m_UseGPUBillboarding(false)
{
}

//----------------------------------------------------------------------------

PRendererCacheBase	CUnityFrameCollector::CreateRendererCache(const PRendererDataBase &renderer, const CParticleDescriptor *particleDesc)
{
	PUnityRendererCache		rendererCache = PK_NEW(CUnityRendererCache(this));
	CRuntimeManager			&RTManager = CRuntimeManager::Instance();

	const TArray<SUnitySceneView>	&views = RTManager.GetScene().SceneViews();
	UnityGfxRenderer				deviceType = RTManager.GetDeviceType();

	if (renderer->m_RendererType == Renderer_Mesh || renderer->m_RendererType == Renderer_Light || renderer->m_RendererType == Renderer_Sound || renderer->m_RendererType == Renderer_Decal)
	{
		if (!PK_VERIFY(rendererCache->m_UnityMeshInfoPerViews.Resize(1)))
			return null;
		rendererCache->m_UnityMeshInfoPerViews[0].Init(deviceType);
	}
	else
	{
		u32 count = views.Empty() ? 1 : views.Count();
		if (!PK_VERIFY(rendererCache->m_UnityMeshInfoPerViews.Resize(count)))
			return null;
		for (u32 i = 0; i < count; ++i)
		{
			rendererCache->m_UnityMeshInfoPerViews[i].Init(deviceType);
		}
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
	else if (renderer->m_RendererType == Renderer_Light)
	{
		succeeded = rendererCache->GameThread_SetupRenderer(static_cast<const CRendererDataLight*>(renderer.Get()));
	}
	else if (renderer->m_RendererType == Renderer_Sound)
	{
		succeeded = rendererCache->GameThread_SetupRenderer(static_cast<const CRendererDataSound*>(renderer.Get()));
	}
	else if (renderer->m_RendererType == Renderer_Decal)
	{
		succeeded = rendererCache->GameThread_SetupRenderer(static_cast<const CRendererDataDecal*>(renderer.Get()));
	}

	rendererCache->SetAssetName(particleDesc->ParentEffect()->HandlerName());

	if (renderer->m_RendererType == ERendererClass::Renderer_Billboard ||
		renderer->m_RendererType == ERendererClass::Renderer_Ribbon ||
		renderer->m_RendererType == ERendererClass::Renderer_Mesh ||
		renderer->m_RendererType == ERendererClass::Renderer_Decal)
	{
		CResourceManager	*resourceManager = particleDesc->ParentEffect()->Context()->ResourceManager();
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
				else if (renderer->m_RendererType == Renderer_Decal)
				{
					SDecalRendererDesc	desc;
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
				const SRendererDeclaration::SAdditionalFieldDefinition *emissive = renderer->m_Declaration.FindAdditionalFieldDefinition(BasicRendererProperties::SID_Emissive_EmissiveColor());
				bool isEmissive3 = (emissive != null &&  emissive->m_Type == BaseType_Float3) ? true : false;
				rendererCache->CreateUnityMesh(rendererCount, m_UseGPUBillboarding, isEmissive3);
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

//----------------------------------------------------------------------------

CRendererBatchDrawer	*CUnityFrameCollector::CreateBillboardingBatch(ERendererClass rendererType, const PRendererCacheBase &rendererCache, bool gpuStorage)
{
	(void)rendererCache;

	if (gpuStorage)
		return null;

	CRendererBatchDrawer	*retValue = null;

	switch (rendererType)
	{
	case	Renderer_Billboard:
		if (m_UseGPUBillboarding)
			retValue = PK_NEW(CUnityBatchDrawer_Billboard_GPU);
		else
			retValue = PK_NEW(CUnityBatchDrawer_Billboard);
		break;
	case	Renderer_Ribbon:
		retValue = PK_NEW(CUnityBatchDrawer_Ribbon);
		break;
	case	Renderer_Triangle:
		retValue = PK_NEW(CUnityBatchDrawer_Triangle);
		break;
	case	Renderer_Mesh:
		retValue = PK_NEW(CUnityBatchDrawer_Mesh);
		break;
	case	Renderer_Decal:
		retValue = PK_NEW(CUnityBatchDrawer_Decal);
		break;
	case	Renderer_Light:
		retValue = PK_NEW(CUnityBatchDrawer_Light);
		break;
	case	Renderer_Sound:
		retValue = PK_NEW(CUnityBatchDrawer_Sound);
		break;
	default:
		return null;
	}
	if (retValue != null)
	{
		PK_SCOPEDLOCK_WRITE(m_CacheLock);
		m_Batches.PushBack(retValue);
	}
	return retValue;
}

//----------------------------------------------------------------------------

void	CUnityFrameCollector::Reset()
{
	PK_SCOPEDLOCK_WRITE(m_CacheLock);
	m_RendererCaches.Clear();
	m_Batches.Clear();
}

//----------------------------------------------------------------------------

void	CUnityFrameCollector::RemoveRendererCache(const PCUnityRendererCache &cache)
{
	PK_SCOPEDLOCK_WRITE(m_CacheLock);
	m_RendererCaches.RemoveElement(cache);
}

//----------------------------------------------------------------------------

void	CUnityFrameCollector::RemoveBatch(CRendererBatchDrawer *batch)
{
	PK_SCOPEDLOCK_WRITE(m_CacheLock);
	m_Batches.RemoveElement(batch);
}

//----------------------------------------------------------------------------
__PK_API_END
