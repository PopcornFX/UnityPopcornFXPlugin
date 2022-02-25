//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include <pk_particles/include/ps_descriptor.h>
#include <pk_particles/include/ps_attributes.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------
struct		SResourceDependency;

bool		LoadAndBrowseEffect(void *pkfxContentPtr, int contentByteSize, const char *path);
bool		BrowseEffect(const PParticleEffect &effectContent);
bool		BrowseObjectForDependencies(TArray<SResourceDependency> &dependencies, bool &usesMeshRenderer);
bool		BrowseRenderers(CParticleEffect* particleEffect);
bool		BrowseExportedEvents(CParticleEffect* particleEffect);
bool		BrowseAttributes(const CParticleAttributeList::_TypeOfAttributeList &attributeList, const TMemoryView<const u32> &remapIds);
bool		BrowseSamplers(const CParticleAttributeList::_TypeOfSamplerList &samplerList);

//----------------------------------------------------------------------------
__PK_API_END
