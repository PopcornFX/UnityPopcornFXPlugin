//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once

__PK_API_BEGIN
//----------------------------------------------------------------------------

PK_FORWARD_DECLARE(FileWatcherUnity);
PK_FORWARD_DECLARE(EffectBaker);
PK_FORWARD_DECLARE(EffectBrowser);
PK_FORWARD_DECLARE(MeshBaker);

class	CEditorManager : public CNonCopyable
{
private:
	CEditorManager();
public:
	~CEditorManager();

	// Handle singleton startup etc...
	static bool					IsInstanceInitialized();
	static bool					InitializeInstanceIFN();
	static CEditorManager		&Instance();

	// func ptr casllbacks for the file watcher
	static void				FileAdded(const CString &path);
	static void				FileRemoved(const CString &path);
	static void				FileChanged(const CString &path);
	static void				FileChangedRelativePath(const CString &path);
	static void				FileRenamed(const CString &oldPath, const CString &newPath);

	CFileWatcherUnity		*GetFileWatcher() const;
	CMeshBaker				*GetMeshBaker() const;
	CEffectBaker			*GetEffectBaker() const;
	CEffectBrowser			*GetEffectBrowser() const;

private:
	PFileWatcherUnity		m_FileWatcher;
	PMeshBaker				m_MeshBaker;
	PEffectBaker			m_EffectBaker;
	PEffectBrowser			m_EffectBrowser;

	static CEditorManager	*m_Instance;
};

//----------------------------------------------------------------------------
__PK_API_END
