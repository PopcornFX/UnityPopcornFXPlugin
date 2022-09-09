//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "EditorManagedToNative.h"
#include "EditorNativeToManaged.h"
#include "RuntimeManager.h"
#include "EditorManager.h"
#include "EffectBaking.h"
#include "MeshBaking.h"
#include "FileWatcher.h"
#include "PKUnity_FileSystemController.h"
#include "FxEffect.h"
#include "FxRendererSubView.h"
#include "BrowseEffect.h"

#include "UnityGraphicsAPI/IUnityInterface.h"

#include <pk_version.h>
#include <pk_kernel/include/kr_streams_memory.h>

extern "C"
{
	MANAGED_TO_POPCORN_CONVENTION bool	PopcornFXEditorStartup()
	{
		return CEditorManager::InitializeInstanceIFN();
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION ManagedBool	BrowseEffectContent(void *pkfxContentPtr, int contentByteSize, const char *path)
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return ManagedBool_False);
		if (!PK_VERIFY(pkfxContentPtr != null))
			return ManagedBool_False;

		CEffectBrowser *effectBrowser = CEditorManager::Instance().GetEffectBrowser();

		return effectBrowser->LoadAndBrowseEffect(pkfxContentPtr, contentByteSize, path) ? ManagedBool_True : ManagedBool_False;
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION const char	*GetRuntimeVersion()
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return "Runtime not started");
		PK_SCOPEDPROFILE();

		return PK_VERSION_CURRENT_STRING;
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	SetPackSettings(const SMirrorPackFxSettings *settings)
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return false);
		PK_SCOPEDPROFILE();
		if (!PK_VERIFY(settings != null))
			return false;

		CEffectBaker		*effectBaker = CEditorManager::Instance().GetEffectBaker();

		effectBaker->Initialize(settings->m_PopcornPackFxPath);
		effectBaker->Clear();
		return true;
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	StartPackWatcher()
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return false);

		PK_SCOPEDPROFILE();
#if !defined(PK_MACOSX)
		CFileWatcherUnity	*fileWatcher = CEditorManager::Instance().GetFileWatcher();

		CEffectBaker		*effectBaker = CEditorManager::Instance().GetEffectBaker();

		bool	watchPackSucceed = fileWatcher->SetWatchPack(effectBaker->GetPopcornFXPackPath());

		if (!watchPackSucceed)
			return false;

		fileWatcher->SetNotifierAdd(&CEditorManager::FileAdded);
		fileWatcher->SetNotifierRemove(&CEditorManager::FileRemoved);
		fileWatcher->SetNotifierModify(&CEditorManager::FileChanged);
		fileWatcher->SetNotifierRename(&CEditorManager::FileRenamed);
#endif
		return true;
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	PausePackWatcher()
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return);

#if !defined(PK_MACOSX)
		CFileWatcherUnity	*fileWatcher = CEditorManager::Instance().GetFileWatcher();
		fileWatcher->PauseFileWatcher();
#endif
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	RestartPackWatcher()
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return);

#if !defined(PK_MACOSX)
		CFileWatcherUnity	*fileWatcher = CEditorManager::Instance().GetFileWatcher();
		fileWatcher->RestartFileWatcher();
#endif
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	UnstackPackWatcherChanges(int& assetRemaining)
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return false);

		CEffectBaker		*effectBaker = CEditorManager::Instance().GetEffectBaker();
		assetRemaining = effectBaker->PopFileChanges();
		if (assetRemaining < 0)
			return false;
		return true;
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	CancelPackWatcherChanges()
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return);

		CEffectBaker		*effectBaker = CEditorManager::Instance().GetEffectBaker();
		effectBaker->CancelAllFileChanges();
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void		LockPackWatcher()
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return);

		CEffectBaker		*effectBaker = CEditorManager::Instance().GetEffectBaker();
		effectBaker->Lock();
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	UnlockPackWatcher()
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return);

		CEffectBaker		*effectBaker = CEditorManager::Instance().GetEffectBaker();
		effectBaker->Unlock();
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	GetAllAssetPath()
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return);

		CEffectBaker		*effectBaker = CEditorManager::Instance().GetEffectBaker();
		effectBaker->GetAllAssetPath();
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	SetForceDeterminismOnBakeNative(bool enable)
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return);

		CEffectBaker		*effectBaker = CEditorManager::Instance().GetEffectBaker();
		effectBaker->SetForceDeterminismFlag(enable);
	}

	//--------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION void	ReimportAssets(int size, const char** paths)
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return);

		CEffectBaker		*effectBaker = CEditorManager::Instance().GetEffectBaker();
		TArray<CString>		pathArr;
		for (int i = 0; i< size; i++)
		{
			pathArr.PushBack(*(paths + i));
		}
		effectBaker->ReimportAssets(pathArr);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION SMeshDataToFill	*GetMeshDataToFillFromAttribs(int vtxCount, int idxCount, int meshVertexAttributes)
	{
		return CRuntimeManager::CreateMeshDataToFill(vtxCount, idxCount, 0, meshVertexAttributes);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	NextSubmeshToBake(const char *name, int meshVertexAttributes)
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return false);

		CMeshBaker		*meshBaker = CEditorManager::Instance().GetMeshBaker();

		return meshBaker->NextSubmesh(name, meshVertexAttributes);
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	AppendMeshToBake(SMeshDataToFill *mesh, CFloat4x4 transform)
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return false);

		CMeshBaker		*meshBaker = CEditorManager::Instance().GetMeshBaker();
		bool			success = meshBaker->AppendMeshToBake(mesh, transform);
		return success;
	}

	//----------------------------------------------------------------------------

	MANAGED_TO_POPCORN_CONVENTION bool	BakeMeshes(const char *outputMeshPath)
	{
		NEED_PK_RUNTIME_AND_EDITOR_STARTED(return false);

		CMeshBaker		*meshBaker = CEditorManager::Instance().GetMeshBaker();
		return meshBaker->BakeToFile(outputMeshPath);
	}

//----------------------------------------------------------------------------
} // extern "C"
