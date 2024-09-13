//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "EditorNativeToManaged.h"
#include "NativeToManaged.h"

#include <pk_kernel/include/kr_scheduler.h>

extern "C"
{
	// Import effect callbacks:
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnEffectDependencyFound)(const char *dependencyPath, int isLinearTexture) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnEffectRendererFound)(const void *rendererDesc, int type, int idx) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION* _OnEffectRendererLink)(int globalIdx, const char *currentQualityLevel, unsigned int uid) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnEffectEventFound)(const SFxEventDesc *attribDesc) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnEffectAttributeFound)(const SFxAttributeDesc *attribDesc) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnEffectSamplerFound)(const SFxSamplerDesc *samplerDesc) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnGetEffectInfo)(int usesMeshParticles) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnEffectAboutToBake)(const char *effectName) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnAssetChange)(const SAssetChangesDesc *assetChanges) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnGetAllAssetPath)(const char **pathArray, int size) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnPkkgExtracted)(const char *effectName) = null;
	void				(POPCORN_TO_MANAGED_CONVENTION *_OnProjectSettingsUpdated)(float lodMinDistance, float lodMaxDistance, float lodMinMinDistance) = null;

	//----------------------------------------------------------------------------

	// Import effect callbacks:
	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectDependencyFound(void *delegatePtr)
	{
		_OnEffectDependencyFound = (void (POPCORN_TO_MANAGED_CONVENTION *)(const char *dependencyPath, int isLinearTexture))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectRendererFound(void *delegatePtr)
	{
		_OnEffectRendererFound = (void (POPCORN_TO_MANAGED_CONVENTION *)(const void *rendererDesc, int type, int idx))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectRendererLink(void *delegatePtr)
	{
		_OnEffectRendererLink = (void (POPCORN_TO_MANAGED_CONVENTION*)(int globalIdx, const char *currentQualityLevel, unsigned int uid))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectEventFound(void *delegatePtr)
	{
		_OnEffectEventFound = (void (POPCORN_TO_MANAGED_CONVENTION *)(const SFxEventDesc *eventDesc))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectAttributeFound(void *delegatePtr)
	{
		_OnEffectAttributeFound = (void (POPCORN_TO_MANAGED_CONVENTION *)(const SFxAttributeDesc *attribDesc))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectSamplerFound(void *delegatePtr)
	{
		_OnEffectSamplerFound = (void (POPCORN_TO_MANAGED_CONVENTION *)(const SFxSamplerDesc *samplerDesc))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetEffectInfo(void *delegatePtr)
	{
		_OnGetEffectInfo = (void (POPCORN_TO_MANAGED_CONVENTION *)(int usesMeshParticles))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnEffectAboutToBake(void *delegatePtr)
	{
		_OnEffectAboutToBake = (void (POPCORN_TO_MANAGED_CONVENTION *)(const char *effectName))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnAssetChange(void *delegatePtr)
	{
		_OnAssetChange = (void (POPCORN_TO_MANAGED_CONVENTION *)(const SAssetChangesDesc *assetChanges))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnGetAllAssetPath(void *delegatePtr)
	{
		_OnGetAllAssetPath = (void (POPCORN_TO_MANAGED_CONVENTION *)(const char **pathArray, int size))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnPkkgExtracted(void *delegatePtr)
	{
		_OnPkkgExtracted = (void (POPCORN_TO_MANAGED_CONVENTION *)(const char *dirPath))delegatePtr;
	}

	MANAGED_TO_POPCORN_CONVENTION void			SetDelegateOnProjectSettingsUpdated(void *delegatePtr)
	{
		_OnProjectSettingsUpdated = (void (POPCORN_TO_MANAGED_CONVENTION *)(float lodMinDistance, float lodMaxDistance, float lodMinMinDistance))delegatePtr;
	}

	//----------------------------------------------------------------------------

	void										OnEffectDependencyFound(const char *dependencyPath, int useInfoFlags)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnEffectDependencyFound != null))
		{
			_OnEffectDependencyFound(dependencyPath, useInfoFlags);
		}
	}

	void										OnEffectRendererFound(const void *effectRdrDesc, int type, int idx)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnEffectRendererFound != null))
		{
			_OnEffectRendererFound(effectRdrDesc, type, idx);
		}
	}

	void										OnEffectRendererLink(int globalIdx, const char *currentQualityLevel, unsigned int uid)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnEffectRendererLink != null))
		{

			_OnEffectRendererLink(globalIdx, currentQualityLevel, uid);
		}
	}

	void										OnEffectEventFound(const SFxEventDesc *effectEventDesc)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnEffectEventFound != null))
		{
			_OnEffectEventFound(effectEventDesc);
		}

	}

	void										OnEffectAttributeFound(const SFxAttributeDesc *attribDesc)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnEffectAttributeFound != null))
		{
			_OnEffectAttributeFound(attribDesc);
		}
	}

	void										OnEffectSamplerFound(const SFxSamplerDesc *samplerDesc)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnEffectSamplerFound != null))
		{
			_OnEffectSamplerFound(samplerDesc);
		}
	}

	void										OnGetEffectInfo(int usesMeshParticles)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnGetEffectInfo != null))
		{
			_OnGetEffectInfo(usesMeshParticles);
		}
	}

	void										OnEffectAboutToBake(const char *effectName)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnEffectAboutToBake != null))
		{
			_OnEffectAboutToBake(effectName);
		}
	}

	void										OnAssetChange(const SAssetChangesDesc *assetChanges)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnAssetChange != null))
		{
			_OnAssetChange(assetChanges);
		}
	}

	void										OnGetAllAssetPath(const char **pathArray, int size)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnGetAllAssetPath != null))
		{
			_OnGetAllAssetPath(pathArray, size);
		}
	}

	void										OnPkkgExtracted(const char *dirPath)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnPkkgExtracted != null))
		{
			_OnPkkgExtracted(dirPath);
		}
	}

	void										OnProjectSettingsUpdated(float lodMinDistance, float lodMaxDistance, float lodMinMinDistance)
	{
		PK_SCOPEDPROFILE();
		PK_ASSERT(CCurrentThread::IsMainThread());
		if (PK_VERIFY(_OnProjectSettingsUpdated != null))
		{
			_OnProjectSettingsUpdated(lodMinDistance, lodMaxDistance, lodMinMinDistance);
		}
	}

	void										ClearNativeToManagedEditorCallbacks()
	{
		_OnEffectDependencyFound = null;
		_OnEffectAttributeFound = null;
		_OnEffectSamplerFound = null;
		_OnGetEffectInfo = null;
		_OnEffectAboutToBake = null;
		_OnAssetChange = null;
		_OnGetAllAssetPath = null;
		_OnPkkgExtracted = null;
		_OnProjectSettingsUpdated = null;
	}
}
