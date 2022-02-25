//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include "ManagedToNative.h"
#include "UnityGraphicsAPI/IUnityGraphics.h"

extern "C"
{
	//----------------------------------------------------------------------------

	struct	SMirrorPackFxSettings
	{
		const char	*m_PopcornPackFxPath;
		const char	*m_UnityPackFxPath;
	};

	//----------------------------------------------------------------------------
	// Callbacks:
	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool					PopcornFXEditorStartup();
	MANAGED_TO_POPCORN_CONVENTION ManagedBool			BrowseEffectContent(void *pkfxContentPtr, int contentByteSize, const char *path);
	MANAGED_TO_POPCORN_CONVENTION const char			*GetRuntimeVersion();
	MANAGED_TO_POPCORN_CONVENTION bool					SetPackSettings(const SMirrorPackFxSettings *settings);
	MANAGED_TO_POPCORN_CONVENTION bool					StartPackWatcher();
	MANAGED_TO_POPCORN_CONVENTION void					PausePackWatcher();
	MANAGED_TO_POPCORN_CONVENTION void					RestartPackWatcher();
	MANAGED_TO_POPCORN_CONVENTION bool					UnstackPackWatcherChanges(int& assetRemaining);
	MANAGED_TO_POPCORN_CONVENTION void					CancelPackWatcherChanges();
	MANAGED_TO_POPCORN_CONVENTION void					LockPackWatcher();
	MANAGED_TO_POPCORN_CONVENTION void					UnlockPackWatcher();
	MANAGED_TO_POPCORN_CONVENTION void					GetAllAssetPath();
	MANAGED_TO_POPCORN_CONVENTION void					SetForceDeterminismOnBakeNative(bool enable);
	MANAGED_TO_POPCORN_CONVENTION void					ReimportAssets(int size, const char** paths);
	MANAGED_TO_POPCORN_CONVENTION SMeshDataToFill		*GetMeshDataToFillFromAttribs(int vtxCount, int idxCount, int meshVertexAttributes);
	MANAGED_TO_POPCORN_CONVENTION bool					NextSubmeshToBake(const char *name, int meshVertexAttributes);
	MANAGED_TO_POPCORN_CONVENTION bool					AppendMeshToBake(SMeshDataToFill *mesh, CFloat4x4 transform);
	MANAGED_TO_POPCORN_CONVENTION bool					BakeMeshes(const char *outputMeshPath);
}
