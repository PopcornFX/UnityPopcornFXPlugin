//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "PKUnity_FileSystemController.h"
#include "NativeToManaged.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

CFileStreamFS_Unity::CFileStreamFS_Unity(PFilePack					pack,
										const CString				&path,
										IFileSystem::EAccessPolicy	mode,
										void						*fh,
										u64							size,
										bool						ownBuffer)
:	CFileStream(File::DefaultFileSystem(), pack, path, mode)
,	m_OwnBuffer(ownBuffer)
,	m_Cursor(0)
,	m_Size(0)
{
	PK_ASSERT(ownBuffer || mode == IFileSystem::Access_Read);
	if (mode == IFileSystem::Access_Read ||
		mode == IFileSystem::Access_ReadWrite ||
		mode == IFileSystem::Access_ReadWriteCreate)
	{
		if (!PK_VERIFY(fh != null))
		{
			CLog::Log(PK_ERROR, "File handler is null for '%s'", path.Data());
			m_PseudoFileHandle = null;
			m_Size = 0;
		}
		else if (m_OwnBuffer)
		{
			m_PseudoFileHandle = (u8 *)PK_MALLOC(checked_numcast<u32>(size));
			if (!PK_VERIFY(m_PseudoFileHandle != null))
			{
				CLog::Log(PK_ERROR, "Alloc failed on CFileStreamFS_Unity");
			}
			else
			{
				Mem::Copy(m_PseudoFileHandle, fh, checked_numcast<u32>(size));
			}
		}
		else
		{
			m_PseudoFileHandle = (u8*)fh;
		}
		m_Size = size;
	}
	else
	{
		m_PseudoFileHandle = null;
		m_Size = 0;
	}
}

//----------------------------------------------------------------------------

CFileStreamFS_Unity	*CFileStreamFS_Unity::Open(PFilePack pack, const CString &path, IFileSystem::EAccessPolicy mode)
{
	CFileStreamFS_Unity *fs = null;

	if (mode == IFileSystem::Access_Read ||
		mode == IFileSystem::Access_ReadWrite ||
		mode == IFileSystem::Access_ReadWriteCreate)
	{
		void *handle;

		u64 size = ::OnResourceLoad(path.Data(), &handle);
		if (size > 0)
			fs = PK_NEW(CFileStreamFS_Unity(pack, path, mode, handle, size));
		else
		{
			CLog::Log(PK_ERROR, "_OnResourceLoad could not read \'%s\'", path.Data());
			return null;
		}

	}
	if (mode == IFileSystem::Access_WriteCreate)
	{
		fs = PK_NEW(CFileStreamFS_Unity(pack, path, mode, null, 0));
	}
	return fs;
}

//----------------------------------------------------------------------------

u64	CFileStreamFS_Unity::Read(void *targetBuffer, u64 byteCount)
{
	if (m_Mode == IFileSystem::Access_WriteCreate)
	{
		PK_ASSERT_NOT_REACHED_MESSAGE("Filestream is write-only.");
		return 0;
	}
	PK_ASSERT_MESSAGE(m_PseudoFileHandle != null, "Filestream not initialized.");
	PK_ASSERT_MESSAGE(targetBuffer != null, "null pointer passed to Read.");
	PK_ASSERT(m_Cursor <= m_Size);

	u64		byteRead = byteCount;
	void	*readHead = m_PseudoFileHandle;

	readHead = Mem::AdvanceRawPointer(readHead, sreg(m_Cursor));
	byteRead = PKMin(byteRead, m_Size - m_Cursor);

	if (m_PseudoFileHandle != null && memcpy(targetBuffer, readHead, sreg(byteRead)) != null)
	{
		m_Cursor += byteRead;
		return byteRead;
	}
	return 0;
}

//----------------------------------------------------------------------------

u64	CFileStreamFS_Unity::Write(const void *sourceBuffer, u64 byteCount)
{
	if (m_Mode == IFileSystem::Access_Read)
	{
		PK_ASSERT_NOT_REACHED_MESSAGE("Filestream is read-only.");
		return 0;
	}
	if (m_Size - m_Cursor <= byteCount)
	{
		m_PseudoFileHandle = (u8 *)PK_REALLOC(m_PseudoFileHandle, ureg(m_Cursor + byteCount));
		m_Size += byteCount;
	}
	memcpy(Mem::AdvanceRawPointer<u8 *>(m_PseudoFileHandle, checked_numcast<u32>(m_Cursor)), sourceBuffer, sreg(byteCount));
	m_Cursor += byteCount;
	return byteCount;
}

//----------------------------------------------------------------------------

bool	CFileStreamFS_Unity::Seek(s64 offset, ESeekMode whence)
{
	if (whence == ESeekMode::SeekSet)
	{
		PK_ASSERT(checked_numcast<u32>(offset) <= m_Size);
		m_Cursor = checked_numcast<u32>(offset);
	}
	else if(whence == CStream::SeekCur)
	{
		PK_ASSERT(checked_numcast<u32>(m_Cursor + offset) <= m_Size);
		m_Cursor += offset;
	}

	if (m_Cursor <= m_Size)
		return true;

	m_Cursor = m_Size;	// keep a 'valid' pos (here, EOF)
	return false;
}

//----------------------------------------------------------------------------

u64	CFileStreamFS_Unity::Tell() const
{
	return m_Cursor;
}

//----------------------------------------------------------------------------

bool	CFileStreamFS_Unity::Eof() const
{
	return (m_Cursor >= m_Size);
}

//----------------------------------------------------------------------------

bool	CFileStreamFS_Unity::Flush()
{
	return false;
}

//----------------------------------------------------------------------------

void	CFileStreamFS_Unity::Close()
{
	_InternalCloseIFN();
}

//----------------------------------------------------------------------------

u64	CFileStreamFS_Unity::SizeInBytes() const
{
	return m_Size;
}

//----------------------------------------------------------------------------

void	CFileStreamFS_Unity::Timestamps(SFileTimes &)
{
	PK_ASSERT_NOT_REACHED();
}

//----------------------------------------------------------------------------
//	CUnityPseudoFileSystem
//----------------------------------------------------------------------------

CUnityPseudoFileSystem::~CUnityPseudoFileSystem()
{
}

//----------------------------------------------------------------------------

PFileStream	CUnityPseudoFileSystem::OpenStream(const CString &path, IFileSystem::EAccessPolicy mode, bool pathNotVirtual/* = false*/)
{
	(void)pathNotVirtual;
	PFileStream	streams;

	PK_ASSERT_MESSAGE(!path.Empty(), "null Path");
	PK_ASSERT(mode >= 0 && mode < IFileSystem::__MaxAccessPolicies);

	PFilePack	pack;
	return CFileStreamFS_Unity::Open(pack, path, mode);
}


//----------------------------------------------------------------------------

bool	CUnityPseudoFileSystem::Exists(const CString &path, bool pathNotVirtual/* = false*/)
{
	(void)path; (void)pathNotVirtual;
	return true;
}

//----------------------------------------------------------------------------

bool	CUnityPseudoFileSystem::FileDelete(const CString &path, bool pathNotVirtual/* = false*/)
{
	(void)path; (void)pathNotVirtual;
	PK_ASSERT_NOT_REACHED_MESSAGE("Can't delete Unity Assets");
	return false;
}

//----------------------------------------------------------------------------

bool	CUnityPseudoFileSystem::FileCopy(const CString &sourcePath, const CString &targetPath, bool pathsNotVirtual/* = false*/)
{
	(void)sourcePath; (void)targetPath; (void)pathsNotVirtual;
	PK_ASSERT_NOT_REACHED_MESSAGE("Can't copy Unity Assets");
	return false;
}

//----------------------------------------------------------------------------

void	CUnityPseudoFileSystem::GetDirectoryContents(char *dpath, char *virtualPath, u32 pathLength, CFileDirectoryWalker *walker, const CFilePack *pack)
{
	(void)dpath; (void)virtualPath; (void)pathLength; (void)walker; (void)pack;
	return;
}

//----------------------------------------------------------------------------

bool	CUnityPseudoFileSystem::CreateDirectoryChainIFN(const CString &directoryPath, bool pathNotVirtual/* = false*/)
{
	(void)directoryPath; (void)pathNotVirtual;
	PK_ASSERT_NOT_REACHED_MESSAGE("Can't create dir in Unity Assets");
	return false;
}

//----------------------------------------------------------------------------

bool	CUnityPseudoFileSystem::DirectoryDelete(const CString &path, bool pathNotVirtual)
{
	(void)path; (void)pathNotVirtual;
	PK_ASSERT_NOT_REACHED_MESSAGE("Can't create dir in Unity Assets");
	return false;
}

//----------------------------------------------------------------------------

void	CFileStreamFS_Unity::_InternalCloseIFN()
{
	if (m_Mode == IFileSystem::Access_WriteCreate ||
		m_Mode == IFileSystem::Access_ReadWrite ||
		m_Mode == IFileSystem::Access_ReadWriteCreate)
	{
		PK_ONLY_IF_ASSERTS(u64 writtenSize = )::OnResourceWrite(m_Path.Data(), m_PseudoFileHandle, 0, m_Size);
		PK_ASSERT(writtenSize == m_Size);
	}
	PK_ASSERT(::OnResourceUnload(m_Path.Data()));
	if (m_OwnBuffer)
		PK_FREE(m_PseudoFileHandle);
	m_PseudoFileHandle = null;
	m_Cursor = 0;
	m_Size = 0;
}

//----------------------------------------------------------------------------
__PK_API_END
