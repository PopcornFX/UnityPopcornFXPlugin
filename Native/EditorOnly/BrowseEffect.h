//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include <pk_particles/include/ps_descriptor.h>
#include <pk_particles/include/ps_attributes.h>

#include "pk_kernel/include/kr_resources.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
struct		SResourceDependency;

class CEffectBrowser : public CRefCountedObject
{

	PopcornFX::IResourceHandler				*m_BrowseResourceMeshHandler = null;
	PopcornFX::IResourceHandler				*m_BrowseResourceImageHandler = null;
	PopcornFX::IResourceHandler				*m_BrowseResourceRectangleListHandler = null;
	PopcornFX::IResourceHandler				*m_BrowseResourceFontMetricsHandler = null;
	PopcornFX::IResourceHandler				*m_BrowseResourceVectorFieldHandler = null;
	PopcornFX::IFileSystem					*m_BrowseFSController = null;
	PopcornFX::CResourceManager				*m_BrowseResourceManager = null;
	PopcornFX::HBO::CContext				*m_BrowseContext = null;

	PFilePack								m_PKSourcePack = null;

	bool									m_Initialized = false;

	TArray<u32>								m_RendererUIDs;
	TArray<u32>								m_DependenciesUIDs;
	u32										m_UniqueRendererCount;
	s32										m_CurrentQualityLevel = -1;
	
public:
			CEffectBrowser();
			~CEffectBrowser();

	bool	InitializeIFN();
	void	Destroy();

	void	ClearForNewEffect();
	bool	LoadAndBrowseEffect(void *pkfxContentPtr, int contentByteSize, const char *path);

	bool	BrowseEffect(const PParticleEffect &effectContent, PBaseObjectFile &file, bool browseAttributes, bool &requiresGameThreadCollect);
	bool	BrowseObjectForDependencies(TArray<SResourceDependency> &dependencies, bool &requiresGameThreadCollect);
	bool	BrowseRenderers(CParticleEffect *particleEffect, CBaseObjectFile *file, bool &requiresGameThreadCollect);
	bool	BrowseExportedEvents(CParticleEffect *particleEffect);
	bool	BrowseAttributes(const CParticleAttributeList::_TypeOfAttributeList &attributeList, const TMemoryView<const u32> &remapIds);
	bool	BrowseSamplers(const CParticleAttributeList::_TypeOfSamplerList &samplerList);
};


//----------------------------------------------------------------------------
__PK_API_END
