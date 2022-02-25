//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_kernel/include/kr_file_controller.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CUnityPseudoFileSystem : public CFileSystemBase
{
public:
	~CUnityPseudoFileSystem();

	virtual PFileStream				OpenStream(const CString &path, IFileSystem::EAccessPolicy mode, bool pathNotVirtual = false) override;
	virtual bool					Exists(const CString &path, bool pathNotVirtual = false) override;
	virtual bool					FileDelete(const CString &path, bool pathNotVirtual = false) override;
	virtual bool					FileCopy(const CString &sourcePath, const CString &targetPath, bool pathsNotVirtual = false) override;

	virtual void					GetDirectoryContents(char *dpath, char *virtualPath, u32 pathLength, CFileDirectoryWalker *walker, const CFilePack *pack) override;
	virtual bool					CreateDirectoryChainIFN(const CString &directoryPath, bool pathNotVirtual = false) override;

	virtual bool					DirectoryDelete(const CString &path, bool pathNotVirtual = false) override;
};

//----------------------------------------------------------------------------

class	CFileStreamFS_Unity : public CFileStream
{
public:
	CFileStreamFS_Unity(PFilePack					pack,
						const CString				&path,
						IFileSystem::EAccessPolicy	mode,
						void						*fh,
						u64							size,
						bool						ownBuffer = true);

	~CFileStreamFS_Unity() { _InternalCloseIFN();}

	static CFileStreamFS_Unity	*Open(PFilePack pack, const CString &path, IFileSystem::EAccessPolicy mode);

	virtual u64			Read(void *targetBuffer, u64 byteCount) override;
	virtual u64			Write(const void *sourceBuffer, u64 byteCount) override;
	virtual bool		Seek(s64 offset, ESeekMode whence) override;
	virtual u64			Tell() const override;
	virtual bool		Eof() const override;
	virtual bool		Flush() override;
	virtual void		Close() override;
	virtual u64			SizeInBytes() const override;
	virtual void		Timestamps(SFileTimes &timestamps) override;

private:
	bool			m_OwnBuffer;
	u8				*m_PseudoFileHandle;
	u64				m_Cursor;
	u64				m_Size;

	void				_InternalCloseIFN();
};
PK_DECLARE_REFPTRCLASS(FileStreamFS_Unity);

//----------------------------------------------------------------------------
__PK_API_END

