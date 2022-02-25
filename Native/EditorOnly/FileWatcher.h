//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include <pk_kernel/include/kr_file_watcher.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CFileWatcherUnity : public CRefCountedObject
{
private:
	PFileSystemWatcher			m_FileWatcher;
	CString						m_PathToWatch;

	bool	CreateWatcherIFN();
	void	RemoveWatchIFN();

public:
	CFileWatcherUnity();
	~CFileWatcherUnity();

	void	PauseFileWatcher();
	void	RestartFileWatcher();

	bool	SetWatchPack(const CString &pathToWatch);
	void	SetNotifierAdd(void(*callback)(const CString &filePath));
	void	SetNotifierRemove(void(*callback)(const CString &filePath));
	void	SetNotifierModify(void(*callback)(const CString &filePath));
	void	SetNotifierRename(void(*callback)(const CString &oldFilePath, const CString &newFilePath));
};
PK_DECLARE_REFPTRCLASS(FileWatcherUnity);

//----------------------------------------------------------------------------
__PK_API_END
