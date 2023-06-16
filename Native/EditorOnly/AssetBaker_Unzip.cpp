//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "precompiled.h"

#include "AssetBaker.h"

#include <zlib.h>
#include "unzip.h"

#if defined(__APPLE__) || defined(IOAPI_NO_64)
// In darwin and perhaps other BSD variants off_t is a 64 bit value, hence no need for specific 64 bit functions
#define FOPEN_FUNC(filename, mode) fopen(filename, mode)
#define FTELLO_FUNC(stream) ftello(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko(stream, offset, origin)
#else
#define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#define FTELLO_FUNC(stream) ftello64(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	ZLib archive extraction
//
//----------------------------------------------------------------------------

class	CWriteBuffer
{
public:
	CWriteBuffer(const u32 size = 8192) : m_Buffer(PK_MALLOC_ALIGNED(size, Memory::CacheLineSize)), m_Size(size) {}
	~CWriteBuffer() { if (m_Buffer != null) PK_FREE(m_Buffer); }

	bool	Valid() const { return m_Buffer != null; }

	void	*m_Buffer;
	u32		m_Size;
};

static void	_BindIOAPI_FStream(zlib_filefunc_def *pzlib_filefunc_def, IFileSystem *fs);

//----------------------------------------------------------------------------

static s32	_ExtractCurrentFile(IFileSystem *fs, unzFile uf, const CString &dstAbsPath, CString &outPkprojAbsPath, const CWriteBuffer &writeBuffer)
{
	unz_file_info	file_info;
	char			zipFilePath[1024];
	zipFilePath[0] = '\0';
	s32	err = unzGetCurrentFileInfo(uf, &file_info, zipFilePath, sizeof(zipFilePath), null, 0, null, 0);
	if (err != UNZ_OK)
	{
		CLog::Log(PK_ERROR, "Failed getting archive file info \"%s\" (err=%d)\n", zipFilePath, err);
		return err;
	}

	const CString	absFilePath = dstAbsPath / zipFilePath;
	const CString	absFileDir = CFilePath::StripFilename(absFilePath);

	if (!fs->CreateDirectoryChainIFN(absFileDir, true))
		CLog::Log(PK_ERROR, "Failed creating directory \"%s\"", absFileDir.Data());
	if (outPkprojAbsPath.Empty() && absFilePath.EndsWith(".pkproj"))
		outPkprojAbsPath = absFilePath;

	err = unzOpenCurrentFile(uf);
	if (err == UNZ_OK)
	{
		PFileStream	file = fs->OpenStream(absFilePath, IFileSystem::Access_ReadWriteCreate, true);
		if (file != null)
		{
			// Decompress & write blocks of data until we've written the entire file
			do
			{
				const s32	bytesRead = unzReadCurrentFile(uf, writeBuffer.m_Buffer, writeBuffer.m_Size);
				if (bytesRead <= 0)
				{
					if (bytesRead < 0)
					{
						err = bytesRead;
						CLog::Log(PK_ERROR, "Failed reading archive file \"%s\" (err=%d)\n", zipFilePath, err);
					}
					break;
				}

				const u64	bytesToWrite = bytesRead;
				if (file->Write(writeBuffer.m_Buffer, bytesToWrite) != bytesToWrite)
				{
					CLog::Log(PK_ERROR, "Failed writing file \"%s\"\n", absFilePath.Data());
					err = UNZ_ERRNO;
					break;
				}
			}
			while (true);

			// NOTE: We don't care about timestamps here
			// If we ever do, set 'file_info.dosDate' & 'file_info.tmu_date'
		}
		else
			CLog::Log(PK_ERROR, "Failed opening file \"%s\" for writing\n", absFilePath.Data());

		unzCloseCurrentFile(uf);
	}
	else
		CLog::Log(PK_ERROR, "Failed opening archive file \"%s\" (err=%d)\n", zipFilePath, err);
	return err;
}

//----------------------------------------------------------------------------

static bool _ExtractPackage_Impl(IFileSystem *fs, unzFile uf, const CString &dstAbsPath, CString &outPkprojAbsPath, const CWriteBuffer &writeBuffer)
{
	unz_global_info gi;
	if (!PK_VERIFY(unzGetGlobalInfo(uf, &gi) == UNZ_OK))
		return false;

	for (u32 i = 0; i < gi.number_entry; i++)
	{
		if (i != 0 && unzGoToNextFile(uf) != UNZ_OK)
			return false;

		if (_ExtractCurrentFile(fs, uf, dstAbsPath, outPkprojAbsPath, writeBuffer) != UNZ_OK)
			return false;
	}

	return true;
}

//----------------------------------------------------------------------------

bool	CEffectBaker::ExtractPackage(IFileSystem *fs, const CString &srcArchiveAbsPath, const CString &dstDirAbsPath, CString &outPkprojAbsPath)
{
	CLog::Log(PK_INFO, "Extracting package \"%s\"...\n", srcArchiveAbsPath.Data());
	zlib_filefunc_def	ioApi;
	_BindIOAPI_FStream(&ioApi, fs);
	unzFile	uf = unzOpen(srcArchiveAbsPath.Data(), &ioApi);
	if (uf == null)
	{
		CLog::Log(PK_ERROR, "Failed opening archive \"%s\"\n", srcArchiveAbsPath.Data());
		return false;
	}

	const CWriteBuffer	writeBuffer;
	if (!writeBuffer.Valid())
	{
		CLog::Log(PK_ERROR, "Failed allocating memory\n");
		return false;
	}

	const bool	success = _ExtractPackage_Impl(fs, uf, dstDirAbsPath, outPkprojAbsPath, writeBuffer);
	if (!success)
		CLog::Log(PK_ERROR, "Failed extracting archive\n");

	unzClose(uf);

	return success;
}

//----------------------------------------------------------------------------
//
//	IO API using PopcornFX filesystem
//
//----------------------------------------------------------------------------

static voidpf	_IOAPI_Open(voidpf opaque, const void *filename, int mode)
{
	(void)mode;	// Note: We only support reading archives with the hacked-down minizip implem we use here, no point in handling different modes there
	if (filename == null)
		return null;
	IFileSystem	*fs = static_cast<IFileSystem*>(opaque);
	PFileStream	fileRefed = fs->OpenStream(CString(static_cast<const char*>(filename)), IFileSystem::Access_Read, true);
	CFileStream	*file = fileRefed.Get();
	if (file != null)
		file->AddReference(ONLY_IN_UBER_REFPTR_DEBUG(0));
	return file;
}

//----------------------------------------------------------------------------

static int	_IOAPI_Close(voidpf opaque, voidpf stream)
{
	(void)opaque;
	CFileStream	*file = static_cast<CFileStream*>(stream);
	file->Close();
	file->RemoveReference(file ONLY_IN_UBER_REFPTR_DEBUG(PK_COMMA 0));
	return 0;
}

//----------------------------------------------------------------------------

static uLong	_IOAPI_Read(voidpf opaque, voidpf stream, void* buf, uLong size)
{
	(void)opaque;
	CFileStream	*file = static_cast<CFileStream*>(stream);
	return checked_numcast<uLong>(file->Read(buf, size));
}

//----------------------------------------------------------------------------

static ZPOS64_T	_IOAPI_Tell(voidpf opaque, voidpf stream)
{
	(void)opaque;
	CFileStream	*file = static_cast<CFileStream*>(stream);
	return file->Tell();
}

//----------------------------------------------------------------------------

static long	_IOAPI_Seek(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin)
{
	(void)opaque;
	CFileStream	*file = static_cast<CFileStream*>(stream);
	CStream::ESeekMode	seekOrigin = CStream::SeekSet;
	switch (origin)
	{
	case ZLIB_FILEFUNC_SEEK_CUR:
		seekOrigin = CStream::SeekCur;
		break;
	case ZLIB_FILEFUNC_SEEK_SET:
		seekOrigin = CStream::SeekSet;
		break;
	case ZLIB_FILEFUNC_SEEK_END:
		offset = file->SizeInBytes();
		seekOrigin = CStream::SeekSet;
		break;
	default:
		return -1;
	}

	return file->Seek(offset, seekOrigin) ? 0 : -1;
}

//----------------------------------------------------------------------------

static int	_IOAPI_Error(voidpf opaque, voidpf stream)
{
	(void)opaque; (void)stream;
	return 0;	// return nonzero for error
}

//----------------------------------------------------------------------------

static void	_BindIOAPI_FStream(zlib_filefunc_def *pzlib_filefunc_def, IFileSystem *fs)
{
	pzlib_filefunc_def->zopen_file = &_IOAPI_Open;
	pzlib_filefunc_def->zread_file = &_IOAPI_Read;
	pzlib_filefunc_def->ztell_file = &_IOAPI_Tell;
	pzlib_filefunc_def->zseek_file = &_IOAPI_Seek;
	pzlib_filefunc_def->zclose_file = &_IOAPI_Close;
	pzlib_filefunc_def->zerror_file = &_IOAPI_Error;
	pzlib_filefunc_def->opaque = fs;
}

//----------------------------------------------------------------------------
__PK_API_END
