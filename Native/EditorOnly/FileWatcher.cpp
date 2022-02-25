//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "EditorOnly/FileWatcher.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

bool	CFileWatcherUnity::CreateWatcherIFN()
{
	if (m_FileWatcher == null)
	{
		m_FileWatcher = CFileSystemWatcher::NewWatcher();
		if (!PK_VERIFY(m_FileWatcher != null))
		{
			CLog::Log(PK_ERROR, "Could not create the file watcher");
			return false;
		}
	}
	return true;
}

void	CFileWatcherUnity::RemoveWatchIFN()
{
	if (m_FileWatcher != null)
		m_FileWatcher->RemoveWatch(m_PathToWatch);
}

CFileWatcherUnity::CFileWatcherUnity()
{
}

CFileWatcherUnity::~CFileWatcherUnity()
{
	RemoveWatchIFN();
	m_FileWatcher = null;
}

void	CFileWatcherUnity::PauseFileWatcher()
{
	RemoveWatchIFN();
}

void	CFileWatcherUnity::RestartFileWatcher()
{
	SetWatchPack(m_PathToWatch);
}

bool	CFileWatcherUnity::SetWatchPack(const CString &pathToWatch)
{
	CString	purifiedPath = pathToWatch;
	CFilePath::Purify(purifiedPath);
	purifiedPath.AppendSlash();

	RemoveWatchIFN();
	CreateWatcherIFN();

	m_PathToWatch = pathToWatch;
	m_FileWatcher->AddWatch(m_PathToWatch);
	return true;
}

void	CFileWatcherUnity::SetNotifierAdd(void(*callback)(const CString &filePath))
{
	CreateWatcherIFN();
	m_FileWatcher->m_NotifierAdd.Clear();
	m_FileWatcher->m_NotifierAdd += callback;
}

void	CFileWatcherUnity::SetNotifierRemove(void(*callback)(const CString &filePath))
{
	CreateWatcherIFN();
	m_FileWatcher->m_NotifierRemove.Clear();
	m_FileWatcher->m_NotifierRemove += callback;
}

void	CFileWatcherUnity::SetNotifierModify(void(*callback)(const CString &filePath))
{
	CreateWatcherIFN();
	m_FileWatcher->m_NotifierModify.Clear();
	m_FileWatcher->m_NotifierModify += callback;
}

void	CFileWatcherUnity::SetNotifierRename(void(*callback)(const CString &oldFilePath, const CString &newFilePath))
{
	CreateWatcherIFN();
	m_FileWatcher->m_NotifierRename.Clear();
	m_FileWatcher->m_NotifierRename += callback;
}

//----------------------------------------------------------------------------
__PK_API_END
