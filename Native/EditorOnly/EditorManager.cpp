//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "EditorOnly/EditorManager.h"
#include "EditorOnly/FileWatcher.h"
#include "EditorOnly/EffectBaking.h"
#include "EditorOnly/BrowseEffect.h"
#include "EditorOnly/MeshBaking.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

CEditorManager			*CEditorManager::m_Instance = null;

//----------------------------------------------------------------------------

CEditorManager::CEditorManager()
{
}

//----------------------------------------------------------------------------

CEditorManager::~CEditorManager()
{
}

//----------------------------------------------------------------------------

bool				CEditorManager::IsInstanceInitialized()
{
	return m_Instance != null;
}

//----------------------------------------------------------------------------

bool				CEditorManager::InitializeInstanceIFN()
{
	if (!IsInstanceInitialized())
	{
		// Create the CPKFX manager instance:
		m_Instance = PK_NEW(CEditorManager());
		if (!PK_VERIFY(m_Instance != null))
		{
			CLog::Log(PK_ERROR, "Could not allocate CEditorManager");
			return false;
		}

		m_Instance->m_EffectBaker = PK_NEW(CEffectBaker());
		if (!PK_VERIFY(m_Instance->m_EffectBaker != null))
		{
			PK_SAFE_DELETE(m_Instance);
			CLog::Log(PK_ERROR, "Could not allocate CEffectBaker");
			return false;
		}
		m_Instance->m_EffectBrowser = PK_NEW(CEffectBrowser());
		if (!PK_VERIFY(m_Instance->m_EffectBrowser != null))
		{
			PK_SAFE_DELETE(m_Instance);
			CLog::Log(PK_ERROR, "Could not allocate CEffectBaker");
			return false;
		}
		m_Instance->m_FileWatcher = PK_NEW(CFileWatcherUnity());
		if (!PK_VERIFY(m_Instance->m_FileWatcher != null))
		{
			PK_SAFE_DELETE(m_Instance);
			CLog::Log(PK_ERROR, "Could not allocate CFileWatcherUnity");
			return false;
		}
		m_Instance->m_MeshBaker = PK_NEW(CMeshBaker());
		if (!PK_VERIFY(m_Instance->m_FileWatcher != null))
		{
			PK_SAFE_DELETE(m_Instance);
			CLog::Log(PK_ERROR, "Could not allocate CFileWatcherUnity");
			return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------

CEditorManager		&CEditorManager::Instance()
{
	return *m_Instance;
}

//----------------------------------------------------------------------------

void				CEditorManager::FileAdded(const CString &path)
{
	m_Instance->m_EffectBaker->FileAdded(path);
}

//----------------------------------------------------------------------------

void				CEditorManager::FileRemoved(const CString &path)
{
	m_Instance->m_EffectBaker->FileRemoved(path);
}

//----------------------------------------------------------------------------

void				CEditorManager::FileChanged(const CString &path)
{
	m_Instance->m_EffectBaker->FileChanged(path);
}

//----------------------------------------------------------------------------

void				CEditorManager::FileChangedRelativePath(const CString &path)
{
	m_Instance->m_EffectBaker->FileChangedRelativePath(path);
}

//----------------------------------------------------------------------------

void				CEditorManager::FileRenamed(const CString &oldPath, const CString &newPath)
{
	m_Instance->m_EffectBaker->FileRenamed(oldPath, newPath);
}

//----------------------------------------------------------------------------

CFileWatcherUnity	*CEditorManager::GetFileWatcher() const
{
	return m_FileWatcher.Get();
}

//----------------------------------------------------------------------------

CMeshBaker			*CEditorManager::GetMeshBaker() const
{
	return m_MeshBaker.Get();
}

//----------------------------------------------------------------------------

CEffectBaker		*CEditorManager::GetEffectBaker() const
{
	return m_EffectBaker.Get();
}

//----------------------------------------------------------------------------

CEffectBrowser *CEditorManager::GetEffectBrowser() const
{
	m_EffectBrowser->InitializeIFN();
	return m_EffectBrowser.Get();
}

//----------------------------------------------------------------------------
__PK_API_END
