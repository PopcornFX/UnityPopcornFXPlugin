/* unzip.c -- IO for uncompress .zip files using zlib
	Version 1.1, February 14h, 2010
	part of the MiniZip project - ( http://www.winimage.com/zLibDll/minizip.html )

			Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

			Modifications of Unzip for Zip64
			Copyright (C) 2007-2008 Even Rouault

			Modifications for Zip64 support on both zip and unzip
			Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )

			For more info read MiniZip_info.txt


	------------------------------------------------------------------------------------
	Decryption code comes from crypt.c by Info-ZIP but has been greatly reduced in terms of
	compatibility with older software. The following is from the original crypt.c.
	Code woven in by Terry Thorsen 1/2003.

	Copyright (c) 1990-2000 Info-ZIP.  All rights reserved.

	See the accompanying file LICENSE, version 2000-Apr-09 or later
	(the contents of which are also included in zip.h) for terms of use.
	If, for some reason, all these files are missing, the Info-ZIP license
	also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html

		crypt.c (full version) by Info-ZIP.      Last revised:  [see crypt.h]

	The encryption/decryption parts of this source code (as opposed to the
	non-echoing password parts) were originally written in Europe.  The
	whole source package can be freely distributed, including from the USA.
	(Prior to January 2000, re-export from the US was a violation of US law.)

		This encryption code is a direct transcription of the algorithm from
	Roger Schlafly, described by Phil Katz in the file appnote.txt.  This
	file (appnote.txt) is distributed with the PKZIP program (even in the
	version without encryption capabilities).

		------------------------------------------------------------------------------------

		Changes in unzip.c

		2007-2008 - Even Rouault - Addition of cpl_unzGetCurrentFileZStreamPos
	2007-2008 - Even Rouault - Decoration of symbol names unz* -> cpl_unz*
	2007-2008 - Even Rouault - Remove old C style function prototypes
	2007-2008 - Even Rouault - Add unzip support for ZIP64

		Copyright (C) 2007-2008 Even Rouault


		Oct-2009 - Mathias Svensson - Removed cpl_* from symbol names (Even Rouault added them but since this is now moved to a new project (minizip64) I renamed them again).
	Oct-2009 - Mathias Svensson - Fixed problem if uncompressed size was > 4G and compressed size was <4G
								should only read the compressed/uncompressed size from the Zip64 format if
								the size from normal header was 0xFFFFFFFF
	Oct-2009 - Mathias Svensson - Applied some bug fixes from paches recived from Gilles Vollant
		Oct-2009 - Mathias Svensson - Applied support to unzip files with compression mathod BZIP2 (bzip2 lib is required)
								Patch created by Daniel Borca

	Jan-2010 - back to unzip and minizip 1.0 name scheme, with compatibility layer

	Copyright (C) 1998 - 2010 Gilles Vollant, Even Rouault, Mathias Svensson

*/

#include <precompiled.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zlib.h"
#include "unzip.h"

#ifdef STDC
#	include <stddef.h>
#	include <string.h>
#	include <stdlib.h>
#endif
#ifdef NO_ERRNO_H
	extern int errno;
#else
#	include <errno.h>
#endif

#ifndef UNZ_BUFSIZE
#define UNZ_BUFSIZE (16384)
#endif

#ifndef UNZ_MAXFILENAMEINZIP
#define UNZ_MAXFILENAMEINZIP (256)
#endif

#ifndef ALLOC
# define ALLOC(size) (malloc(size))
#endif
#ifndef TRYFREE
# define TRYFREE(p) { free(p); }
#endif

#define SIZECENTRALDIRITEM (0x2e)
#define SIZEZIPLOCALHEADER (0x1e)

//----------------------------------------------------------------------------

/* Maximum unsigned 32-bit value used as placeholder for zip64 */
#ifndef MAXU32
#	define MAXU32	(0xffffffff)
#endif

#define ZREAD64(filefunc,filestream,buf,size)		((*((filefunc).zread_file))		((filefunc).opaque, filestream,buf,size))
#define ZCLOSE64(filefunc,filestream)				((*((filefunc).zclose_file))	((filefunc).opaque, filestream))
#define ZERROR64(filefunc,filestream)				((*((filefunc).zerror_file))	((filefunc).opaque, filestream))
#define ZOPEN64(filefunc,filename,mode)				((*((filefunc).zopen_file))		((filefunc).opaque, filename,mode))
#define ZTELL64(filefunc,filestream)				((*((filefunc).ztell_file))		((filefunc).opaque, filestream))
#define ZSEEK64(filefunc,filestream,pos,mode)		((*((filefunc).zseek_file))		((filefunc).opaque, filestream,pos,mode))

//----------------------------------------------------------------------------

const char unz_copyright[] =
	" unzip 1.01 Copyright 1998-2004 Gilles Vollant - http://www.winimage.com/zLibDll";

/* unz_file_info_interntal contain internal info about a file in zipfile*/
typedef struct unz_file_info_internal_s
{
	ZPOS64_T offset_curfile;/* relative offset of local header 8 bytes */
} unz_file_info_internal;


/* file_in_zip_read_info_s contain internal information about a file in zipfile,
	when reading and decompress it */
typedef struct
{
	char  *read_buffer;         /* internal buffer for compressed data */
	z_stream stream;            /* zLib stream structure for inflate */

#ifdef HAVE_BZIP2
	bz_stream bstream;          /* bzLib stream structure for bziped */
#endif

	ZPOS64_T pos_in_zipfile;       /* position in byte on the zipfile, for fseek*/
	uLong stream_initialised;   /* flag set if stream structure is initialised*/

	ZPOS64_T offset_local_extrafield;/* offset of the local extra field */
	uInt  size_local_extrafield;/* size of the local extra field */
	ZPOS64_T pos_local_extrafield;   /* position in the local extra field in read*/
	ZPOS64_T total_out_64;

	uLong crc32;                /* crc32 of all data uncompressed */
	uLong crc32_wait;           /* crc32 we must obtain after decompress all */
	ZPOS64_T rest_read_compressed; /* number of byte to be decompressed */
	ZPOS64_T rest_read_uncompressed;/*number of byte to be obtained after decomp*/
	zlib_filefunc_def z_filefunc;
	voidpf filestream;        /* io structore of the zipfile */
	uLong compression_method;   /* compression method (0==store) */
	ZPOS64_T byte_before_the_zipfile;/* byte before the zipfile, (>0 for sfx)*/
	int   raw;
} file_in_zip64_read_info_s;


/* unz64_s contain internal information about the zipfile
*/
typedef struct
{
	zlib_filefunc_def z_filefunc;
	voidpf filestream;        /* io structore of the zipfile */
	unz_global_info gi;       /* public global information */
	ZPOS64_T byte_before_the_zipfile;/* byte before the zipfile, (>0 for sfx)*/
	ZPOS64_T num_file;             /* number of the current file in the zipfile*/
	ZPOS64_T pos_in_central_dir;   /* pos of the current file in the central dir*/
	ZPOS64_T current_file_ok;      /* flag about the usability of the current file*/
	ZPOS64_T central_pos;          /* position of the beginning of the central dir*/

	ZPOS64_T size_central_dir;     /* size of the central directory  */
	ZPOS64_T offset_central_dir;   /* offset of start of central directory with
									respect to the starting disk number */

	unz_file_info cur_file_info; /* public info about the current file in zip*/
	unz_file_info_internal cur_file_info_internal; /* private info about it*/
	file_in_zip64_read_info_s* pfile_in_zip_read; /* structure about the current
										file if we are decompressing it */
	int encrypted;

	int isZip64;
} unz64_s;


/* ===========================================================================
		Read a byte from a gz_stream; update next_in and avail_in. Return EOF
	for end of file.
	IN assertion: the stream s has been successfully opened for reading.
*/

static int unz64local_getByte(const zlib_filefunc_def* pzlib_filefunc_def, voidpf filestream, int *pi)
{
	unsigned char c;
	int err = (int)ZREAD64(*pzlib_filefunc_def,filestream,&c,1);
	if (err==1)
	{
		*pi = (int)c;
		return UNZ_OK;
	}
	else
	{
		if (ZERROR64(*pzlib_filefunc_def,filestream))
			return UNZ_ERRNO;
		else
			return UNZ_EOF;
	}
}

/* ===========================================================================
	Reads a long in LSB order from the given gz_stream. Sets
*/

static int	unz64local_getShort(const zlib_filefunc_def* pzlib_filefunc_def,
								voidpf filestream,
								uLong *pX)
{
	uLong x ;
	int i = 0;
	int err;

	err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x = (uLong)i;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((uLong)i)<<8;

	if (err==UNZ_OK)
		*pX = x;
	else
		*pX = 0;
	return err;
}

static int	unz64local_getLong(	const zlib_filefunc_def* pzlib_filefunc_def,
								voidpf filestream,
								uLong *pX)
{
	uLong x ;
	int i = 0;
	int err;

	err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x = (uLong)i;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((uLong)i)<<8;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((uLong)i)<<16;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x += ((uLong)i)<<24;

	if (err==UNZ_OK)
		*pX = x;
	else
		*pX = 0;
	return err;
}

static int	unz64local_getLong64(	const zlib_filefunc_def* pzlib_filefunc_def,
									voidpf filestream,
									ZPOS64_T *pX)
{
	ZPOS64_T x ;
	int i = 0;
	int err;

	err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x = (ZPOS64_T)i;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((ZPOS64_T)i)<<8;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((ZPOS64_T)i)<<16;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((ZPOS64_T)i)<<24;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((ZPOS64_T)i)<<32;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((ZPOS64_T)i)<<40;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((ZPOS64_T)i)<<48;

	if (err==UNZ_OK)
		err = unz64local_getByte(pzlib_filefunc_def,filestream,&i);
	x |= ((ZPOS64_T)i)<<56;

	if (err==UNZ_OK)
		*pX = x;
	else
		*pX = 0;
	return err;
}

#ifndef BUFREADCOMMENT
#	define BUFREADCOMMENT (0x400)
#endif

/*
	Locate the Central directory of a zipfile (at the end, just before
	the global comment)
*/
static ZPOS64_T	unz64local_SearchCentralDir(const zlib_filefunc_def* pzlib_filefunc_def, voidpf filestream)
{
	unsigned char* buf;
	ZPOS64_T uSizeFile;
	ZPOS64_T uBackRead;
	ZPOS64_T uMaxBack=0xffff; /* maximum size of global comment */
	ZPOS64_T uPosFound=0;

	if (ZSEEK64(*pzlib_filefunc_def,filestream,0,ZLIB_FILEFUNC_SEEK_END) != 0)
		return 0;

	uSizeFile = ZTELL64(*pzlib_filefunc_def,filestream);

	if (uMaxBack>uSizeFile)
		uMaxBack = uSizeFile;

	buf = (unsigned char*)ALLOC(BUFREADCOMMENT+4);
	if (buf==NULL)
		return 0;

	uBackRead = 4;
	while (uBackRead<uMaxBack)
	{
		uLong uReadSize;
		ZPOS64_T uReadPos ;
		int i;
		if (uBackRead+BUFREADCOMMENT>uMaxBack)
			uBackRead = uMaxBack;
		else
			uBackRead+=BUFREADCOMMENT;
		uReadPos = uSizeFile-uBackRead ;

		uReadSize = ((BUFREADCOMMENT+4) < (uSizeFile-uReadPos)) ?
						(BUFREADCOMMENT+4) : (uLong)(uSizeFile-uReadPos);
		if (ZSEEK64(*pzlib_filefunc_def,filestream,uReadPos,ZLIB_FILEFUNC_SEEK_SET)!=0)
			break;

		if (ZREAD64(*pzlib_filefunc_def,filestream,buf,uReadSize)!=uReadSize)
			break;

		for (i=(int)uReadSize-3; (i--)>0;)
			if (((*(buf+i))==0x50) && ((*(buf+i+1))==0x4b) &&
				((*(buf+i+2))==0x05) && ((*(buf+i+3))==0x06))
			{
				uPosFound = uReadPos+(unsigned)i;
				break;
			}

		if (uPosFound!=0)
			break;
	}
	TRYFREE(buf);
	return uPosFound;
}


/*
	Locate the Central directory 64 of a zipfile (at the end, just before
	the global comment)
*/

static ZPOS64_T		unz64local_SearchCentralDir64(const zlib_filefunc_def* pzlib_filefunc_def, voidpf filestream)
{
	unsigned char* buf;
	ZPOS64_T uSizeFile;
	ZPOS64_T uBackRead;
	ZPOS64_T uMaxBack=0xffff; /* maximum size of global comment */
	ZPOS64_T uPosFound=0;
	uLong uL;
	ZPOS64_T relativeOffset;

	if (ZSEEK64(*pzlib_filefunc_def,filestream,0,ZLIB_FILEFUNC_SEEK_END) != 0)
		return 0;

	uSizeFile = ZTELL64(*pzlib_filefunc_def,filestream);

	if (uMaxBack>uSizeFile)
		uMaxBack = uSizeFile;

	buf = (unsigned char*)ALLOC(BUFREADCOMMENT+4);
	if (buf==NULL)
		return 0;

	uBackRead = 4;
	while (uBackRead<uMaxBack)
	{
		uLong uReadSize;
		ZPOS64_T uReadPos;
		int i;
		if (uBackRead+BUFREADCOMMENT>uMaxBack)
			uBackRead = uMaxBack;
		else
			uBackRead+=BUFREADCOMMENT;
		uReadPos = uSizeFile-uBackRead ;

		uReadSize = ((BUFREADCOMMENT+4) < (uSizeFile-uReadPos)) ?
						(BUFREADCOMMENT+4) : (uLong)(uSizeFile-uReadPos);
		if (ZSEEK64(*pzlib_filefunc_def,filestream,uReadPos,ZLIB_FILEFUNC_SEEK_SET)!=0)
			break;

		if (ZREAD64(*pzlib_filefunc_def,filestream,buf,uReadSize)!=uReadSize)
			break;

		for (i=(int)uReadSize-3; (i--)>0;)
			if (((*(buf+i))==0x50) && ((*(buf+i+1))==0x4b) &&
				((*(buf+i+2))==0x06) && ((*(buf+i+3))==0x07))
			{
				uPosFound = uReadPos+(unsigned)i;
				break;
			}

		if (uPosFound!=0)
			break;
	}
	TRYFREE(buf);
	if (uPosFound == 0)
		return 0;

	/* Zip64 end of central directory locator */
	if (ZSEEK64(*pzlib_filefunc_def,filestream, uPosFound,ZLIB_FILEFUNC_SEEK_SET)!=0)
		return 0;

	/* the signature, already checked */
	if (unz64local_getLong(pzlib_filefunc_def,filestream,&uL)!=UNZ_OK)
		return 0;

	/* number of the disk with the start of the zip64 end of  central directory */
	if (unz64local_getLong(pzlib_filefunc_def,filestream,&uL)!=UNZ_OK)
		return 0;
	if (uL != 0)
		return 0;

	/* relative offset of the zip64 end of central directory record */
	if (unz64local_getLong64(pzlib_filefunc_def,filestream,&relativeOffset)!=UNZ_OK)
		return 0;

	/* total number of disks */
	if (unz64local_getLong(pzlib_filefunc_def,filestream,&uL)!=UNZ_OK)
		return 0;
	if (uL != 1)
		return 0;

	/* Goto end of central directory record */
	if (ZSEEK64(*pzlib_filefunc_def,filestream, relativeOffset,ZLIB_FILEFUNC_SEEK_SET)!=0)
		return 0;

		/* the signature */
	if (unz64local_getLong(pzlib_filefunc_def,filestream,&uL)!=UNZ_OK)
		return 0;

	if (uL != 0x06064b50)
		return 0;

	return relativeOffset;
}

static int	unz64local_GetCurrentFileInfoInternal(	unzFile file,
													unz_file_info *pfile_info,
													unz_file_info_internal *pfile_info_internal,
													char *szFileName,
													uLong fileNameBufferSize,
													void *extraField,
													uLong extraFieldBufferSize,
													char *szComment,
													uLong commentBufferSize);

/*
	Open a Zip file. path contain the full pathname (by example,
		on a Windows NT computer "c:\\test\\zlib114.zip" or on an Unix computer
		"zlib/zlib114.zip".
		If the zipfile cannot be opened (file doesn't exist or in not valid), the
		return value is NULL.
		Else, the return value is a unzFile Handle, usable with other function
		of this unzip package.
*/
extern unzFile	unzOpen(const void *path, zlib_filefunc_def *io_api)
{
	unz64_s us;
	unz64_s *s;
	ZPOS64_T central_pos;
	uLong   uL;

	uLong number_disk;          /* number of the current dist, used for
									spaning ZIP, unsupported, always 0*/
	uLong number_disk_with_CD;  /* number the the disk with central dir, used
									for spaning ZIP, unsupported, always 0*/
	ZPOS64_T number_entry_CD;      /* total number of entries in
									the central dir
									(same than number_entry on nospan) */

	int err = UNZ_OK;

	if (unz_copyright[0] != ' ')
		return NULL;

	us.z_filefunc = *io_api;
	us.filestream = ZOPEN64(us.z_filefunc,
							path,
							ZLIB_FILEFUNC_MODE_READ);
	if (us.filestream==NULL)
		return NULL;

	central_pos = unz64local_SearchCentralDir64(&us.z_filefunc,us.filestream);
	if (central_pos)
	{
		uLong uS;
		ZPOS64_T uL64;

		us.isZip64 = 1;

		if (ZSEEK64(us.z_filefunc, us.filestream,
										central_pos,ZLIB_FILEFUNC_SEEK_SET)!=0)
		err=UNZ_ERRNO;

		/* the signature, already checked */
		if (unz64local_getLong(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* size of zip64 end of central directory record */
		if (unz64local_getLong64(&us.z_filefunc, us.filestream,&uL64)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* version made by */
		if (unz64local_getShort(&us.z_filefunc, us.filestream,&uS)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* version needed to extract */
		if (unz64local_getShort(&us.z_filefunc, us.filestream,&uS)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* number of this disk */
		if (unz64local_getLong(&us.z_filefunc, us.filestream,&number_disk)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* number of the disk with the start of the central directory */
		if (unz64local_getLong(&us.z_filefunc, us.filestream,&number_disk_with_CD)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* total number of entries in the central directory on this disk */
		if (unz64local_getLong64(&us.z_filefunc, us.filestream,&us.gi.number_entry)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* total number of entries in the central directory */
		if (unz64local_getLong64(&us.z_filefunc, us.filestream,&number_entry_CD)!=UNZ_OK)
			err=UNZ_ERRNO;

		if ((number_entry_CD!=us.gi.number_entry) ||
			(number_disk_with_CD!=0) ||
			(number_disk!=0))
			err=UNZ_BADZIPFILE;

		/* size of the central directory */
		if (unz64local_getLong64(&us.z_filefunc, us.filestream,&us.size_central_dir)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* offset of start of central directory with respect to the
			starting disk number */
		if (unz64local_getLong64(&us.z_filefunc, us.filestream,&us.offset_central_dir)!=UNZ_OK)
			err=UNZ_ERRNO;

		us.gi.size_comment = 0;
	}
	else
	{
		central_pos = unz64local_SearchCentralDir(&us.z_filefunc,us.filestream);
		if (central_pos==0)
			err=UNZ_ERRNO;

		us.isZip64 = 0;

		if (ZSEEK64(us.z_filefunc, us.filestream,
										central_pos,ZLIB_FILEFUNC_SEEK_SET)!=0)
			err=UNZ_ERRNO;

		/* the signature, already checked */
		if (unz64local_getLong(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* number of this disk */
		if (unz64local_getShort(&us.z_filefunc, us.filestream,&number_disk)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* number of the disk with the start of the central directory */
		if (unz64local_getShort(&us.z_filefunc, us.filestream,&number_disk_with_CD)!=UNZ_OK)
			err=UNZ_ERRNO;

		/* total number of entries in the central dir on this disk */
		if (unz64local_getShort(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
			err=UNZ_ERRNO;
		us.gi.number_entry = uL;

		/* total number of entries in the central dir */
		if (unz64local_getShort(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
			err=UNZ_ERRNO;
		number_entry_CD = uL;

		if ((number_entry_CD!=us.gi.number_entry) ||
			(number_disk_with_CD!=0) ||
			(number_disk!=0))
			err=UNZ_BADZIPFILE;

		/* size of the central directory */
		if (unz64local_getLong(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
			err=UNZ_ERRNO;
		us.size_central_dir = uL;

		/* offset of start of central directory with respect to the
			starting disk number */
		if (unz64local_getLong(&us.z_filefunc, us.filestream,&uL)!=UNZ_OK)
			err=UNZ_ERRNO;
		us.offset_central_dir = uL;

		/* zipfile comment length */
		if (unz64local_getShort(&us.z_filefunc, us.filestream,&us.gi.size_comment)!=UNZ_OK)
			err=UNZ_ERRNO;
	}

	if ((central_pos<us.offset_central_dir+us.size_central_dir) &&
		(err==UNZ_OK))
		err=UNZ_BADZIPFILE;

	if (err!=UNZ_OK)
	{
		ZCLOSE64(us.z_filefunc, us.filestream);
		return NULL;
	}

	us.byte_before_the_zipfile = central_pos - (us.offset_central_dir+us.size_central_dir);
	us.central_pos = central_pos;
	us.pfile_in_zip_read = NULL;
	us.encrypted = 0;

	s=(unz64_s*)ALLOC(sizeof(unz64_s));
	if (s != NULL)
	{
		*s=us;

		s->pos_in_central_dir=s->offset_central_dir;
		s->num_file=0;
		err=unz64local_GetCurrentFileInfoInternal((unzFile)s, &s->cur_file_info,
													&s->cur_file_info_internal,
													NULL, 0, NULL, 0, NULL, 0);
		s->current_file_ok = (err == UNZ_OK);
	}
	return (unzFile)s;
}

/*
	Close a ZipFile opened with unzOpen.
	If there is files inside the .Zip opened with unzOpenCurrentFile (see later),
	these files MUST be closed with unzCloseCurrentFile before call unzClose.
	return UNZ_OK if there is no problem. */
extern int	unzClose(unzFile file)
{
	unz64_s* s;
	if (file==NULL)
		return UNZ_PARAMERROR;
	s=(unz64_s*)file;

	if (s->pfile_in_zip_read!=NULL)
		unzCloseCurrentFile(file);

	ZCLOSE64(s->z_filefunc, s->filestream);
	TRYFREE(s);
	return UNZ_OK;
}


/*
	Write info about the ZipFile in the *pglobal_info structure.
	No preparation of the structure is needed
	return UNZ_OK if there is no problem. */
extern int	unzGetGlobalInfo(unzFile file, unz_global_info* pglobal_info)
{
	unz64_s* s;
	if (file==NULL)
		return UNZ_PARAMERROR;
	s=(unz64_s*)file;
	*pglobal_info=s->gi;
	return UNZ_OK;
}

/*
	Translate date/time from Dos format to tm_unz (readable more easilty)
*/
static void	unz64local_DosDateToTmuDate(ZPOS64_T ulDosDate, tm_unz* ptm)
{
	ZPOS64_T uDate;
	uDate = (ZPOS64_T)(ulDosDate>>16);
	ptm->tm_mday = (int)(uDate&0x1f);
	ptm->tm_mon =  (int)((((uDate)&0x1E0)/0x20)-1);
	ptm->tm_year = (int)(((uDate&0x0FE00)/0x0200)+1980);

	ptm->tm_hour = (int) ((ulDosDate &0xF800)/0x800);
	ptm->tm_min =  (int) ((ulDosDate&0x7E0)/0x20);
	ptm->tm_sec =  (int) (2*(ulDosDate&0x1f));
}

/*
	Get Info about the current file in the zipfile, with internal only info
*/

static int	unz64local_GetCurrentFileInfoInternal(	unzFile file,
													unz_file_info *pfile_info,
													unz_file_info_internal *pfile_info_internal,
													char *szFileName,
													uLong fileNameBufferSize,
													void *extraField,
													uLong extraFieldBufferSize,
													char *szComment,
													uLong commentBufferSize)
{
	unz64_s* s;
	unz_file_info file_info;
	unz_file_info_internal file_info_internal;
	int err=UNZ_OK;
	uLong uMagic;
	long lSeek=0;
	uLong uL;

	if (file==NULL)
		return UNZ_PARAMERROR;
	s=(unz64_s*)file;
	if (ZSEEK64(s->z_filefunc, s->filestream,
				s->pos_in_central_dir+s->byte_before_the_zipfile,
				ZLIB_FILEFUNC_SEEK_SET)!=0)
		err=UNZ_ERRNO;

	/* we check the magic */
	if (err==UNZ_OK)
	{
		if (unz64local_getLong(&s->z_filefunc, s->filestream,&uMagic) != UNZ_OK)
			err=UNZ_ERRNO;
		else if (uMagic!=0x02014b50)
			err=UNZ_BADZIPFILE;
	}

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.version) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.version_needed) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.flag) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.compression_method) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getLong(&s->z_filefunc, s->filestream,&file_info.dosDate) != UNZ_OK)
		err=UNZ_ERRNO;

	unz64local_DosDateToTmuDate(file_info.dosDate,&file_info.tmu_date);

	if (unz64local_getLong(&s->z_filefunc, s->filestream,&file_info.crc) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getLong(&s->z_filefunc, s->filestream,&uL) != UNZ_OK)
		err=UNZ_ERRNO;
	file_info.compressed_size = uL;

	if (unz64local_getLong(&s->z_filefunc, s->filestream,&uL) != UNZ_OK)
		err=UNZ_ERRNO;
	file_info.uncompressed_size = uL;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.size_filename) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.size_file_extra) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.size_file_comment) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.disk_num_start) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&file_info.internal_fa) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getLong(&s->z_filefunc, s->filestream,&file_info.external_fa) != UNZ_OK)
		err=UNZ_ERRNO;

				// relative offset of local header
	if (unz64local_getLong(&s->z_filefunc, s->filestream,&uL) != UNZ_OK)
		err=UNZ_ERRNO;
	file_info_internal.offset_curfile = uL;

	lSeek+=file_info.size_filename;
	if ((err==UNZ_OK) && (szFileName!=NULL))
	{
		uLong uSizeRead ;
		if (file_info.size_filename<fileNameBufferSize)
		{
			*(szFileName+file_info.size_filename)='\0';
			uSizeRead = file_info.size_filename;
		}
		else
			uSizeRead = fileNameBufferSize;

		if ((file_info.size_filename>0) && (fileNameBufferSize>0))
			if (ZREAD64(s->z_filefunc, s->filestream,szFileName,uSizeRead)!=uSizeRead)
				err=UNZ_ERRNO;
		lSeek -= uSizeRead;
	}

	// Read extrafield
	if ((err==UNZ_OK) && (extraField!=NULL))
	{
		ZPOS64_T uSizeRead ;
		if (file_info.size_file_extra<extraFieldBufferSize)
			uSizeRead = file_info.size_file_extra;
		else
			uSizeRead = extraFieldBufferSize;

		if (lSeek!=0)
		{
			if (ZSEEK64(s->z_filefunc, s->filestream,(ZPOS64_T)lSeek,ZLIB_FILEFUNC_SEEK_CUR)==0)
				lSeek=0;
			else
				err=UNZ_ERRNO;
		}

		if ((file_info.size_file_extra>0) && (extraFieldBufferSize>0))
			if (ZREAD64(s->z_filefunc, s->filestream,extraField,(uLong)uSizeRead)!=uSizeRead)
				err=UNZ_ERRNO;

		lSeek += file_info.size_file_extra - (uLong)uSizeRead;
	}
	else
		lSeek += file_info.size_file_extra;

	if ((err==UNZ_OK) && (file_info.size_file_extra != 0))
	{
								uLong acc = 0;

		// since lSeek now points to after the extra field we need to move back
		lSeek -= file_info.size_file_extra;

		if (lSeek!=0)
		{
			if (ZSEEK64(s->z_filefunc, s->filestream,(ZPOS64_T)lSeek,ZLIB_FILEFUNC_SEEK_CUR)==0)
				lSeek=0;
			else
				err=UNZ_ERRNO;
		}

		while(acc < file_info.size_file_extra)
		{
			uLong headerId;
												uLong dataSize;

			if (unz64local_getShort(&s->z_filefunc, s->filestream,&headerId) != UNZ_OK)
				err=UNZ_ERRNO;

			if (unz64local_getShort(&s->z_filefunc, s->filestream,&dataSize) != UNZ_OK)
				err=UNZ_ERRNO;

			/* ZIP64 extra fields */
			if (headerId == 0x0001)
			{
				uLong uL2;

				if (file_info.uncompressed_size == MAXU32)
				{
					if (unz64local_getLong64(&s->z_filefunc, s->filestream,&file_info.uncompressed_size) != UNZ_OK)
						err=UNZ_ERRNO;
				}

				if (file_info.compressed_size == MAXU32)
				{
					if (unz64local_getLong64(&s->z_filefunc, s->filestream,&file_info.compressed_size) != UNZ_OK)
						err=UNZ_ERRNO;
				}

				if (file_info_internal.offset_curfile == MAXU32)
				{
					/* Relative Header offset */
					if (unz64local_getLong64(&s->z_filefunc, s->filestream,&file_info_internal.offset_curfile) != UNZ_OK)
						err=UNZ_ERRNO;
				}

				if (file_info.disk_num_start == MAXU32)
				{
					/* Disk Start Number */
					if (unz64local_getLong(&s->z_filefunc, s->filestream,&uL2) != UNZ_OK)
						err=UNZ_ERRNO;
				}
			}
			else
			{
				if (ZSEEK64(s->z_filefunc, s->filestream,dataSize,ZLIB_FILEFUNC_SEEK_CUR)!=0)
					err=UNZ_ERRNO;
			}

			acc += 2 + 2 + dataSize;
		}
	}

	if ((err==UNZ_OK) && (szComment!=NULL))
	{
		uLong uSizeRead ;
		if (file_info.size_file_comment<commentBufferSize)
		{
			*(szComment+file_info.size_file_comment)='\0';
			uSizeRead = file_info.size_file_comment;
		}
		else
			uSizeRead = commentBufferSize;

		if (lSeek!=0)
		{
			if (ZSEEK64(s->z_filefunc, s->filestream,(ZPOS64_T)lSeek,ZLIB_FILEFUNC_SEEK_CUR)==0)
				lSeek=0;
			else
				err=UNZ_ERRNO;
		}

		if ((file_info.size_file_comment>0) && (commentBufferSize>0))
			if (ZREAD64(s->z_filefunc, s->filestream,szComment,uSizeRead)!=uSizeRead)
				err=UNZ_ERRNO;
		lSeek+=file_info.size_file_comment - uSizeRead;
	}
	else
		lSeek+=file_info.size_file_comment;


	if ((err==UNZ_OK) && (pfile_info!=NULL))
		*pfile_info=file_info;

	if ((err==UNZ_OK) && (pfile_info_internal!=NULL))
		*pfile_info_internal=file_info_internal;

	return err;
}


/*
	Write info about the ZipFile in the *pglobal_info structure.
	No preparation of the structure is needed
	return UNZ_OK if there is no problem.
*/
extern int	unzGetCurrentFileInfo(unzFile file,
									unz_file_info * pfile_info,
									char * szFileName, uLong fileNameBufferSize,
									void *extraField, uLong extraFieldBufferSize,
									char* szComment,  uLong commentBufferSize)
{
	return unz64local_GetCurrentFileInfoInternal(file,pfile_info,NULL,
												szFileName,fileNameBufferSize,
												extraField,extraFieldBufferSize,
												szComment,commentBufferSize);
}

/*
	Set the current file of the zipfile to the next file.
	return UNZ_OK if there is no problem
	return UNZ_END_OF_LIST_OF_FILE if the actual file was the latest.
*/
extern int  unzGoToNextFile(unzFile file)
{
	unz64_s* s;
	int err;

	if (file==NULL)
		return UNZ_PARAMERROR;
	s=(unz64_s*)file;
	if (!s->current_file_ok)
		return UNZ_END_OF_LIST_OF_FILE;
	if (s->gi.number_entry != 0xffff)    /* 2^16 files overflow hack */
		if (s->num_file+1==s->gi.number_entry)
		return UNZ_END_OF_LIST_OF_FILE;

	s->pos_in_central_dir += SIZECENTRALDIRITEM + s->cur_file_info.size_filename +
							s->cur_file_info.size_file_extra + s->cur_file_info.size_file_comment ;
	s->num_file++;
	err = unz64local_GetCurrentFileInfoInternal(file,&s->cur_file_info,
												&s->cur_file_info_internal,
												NULL,0,NULL,0,NULL,0);
	s->current_file_ok = (err == UNZ_OK);
	return err;
}


/*
// Unzip Helper Functions - should be here?
///////////////////////////////////////////
*/

/*
	Read the local header of the current zipfile
	Check the coherency of the local header and info in the end of central
		directory about this file
	store in *piSizeVar the size of extra info in local header
		(filename and size of extra field data)
*/
static int	unz64local_CheckCurrentFileCoherencyHeader(	unz64_s* s, uInt* piSizeVar,
														ZPOS64_T * poffset_local_extrafield,
														uInt  * psize_local_extrafield)
{
	uLong uMagic,uData,uFlags;
	uLong size_filename;
	uLong size_extra_field;
	int err=UNZ_OK;

	*piSizeVar = 0;
	*poffset_local_extrafield = 0;
	*psize_local_extrafield = 0;

	if (ZSEEK64(s->z_filefunc, s->filestream,s->cur_file_info_internal.offset_curfile +
								s->byte_before_the_zipfile,ZLIB_FILEFUNC_SEEK_SET)!=0)
		return UNZ_ERRNO;

	if (err==UNZ_OK)
	{
		if (unz64local_getLong(&s->z_filefunc, s->filestream,&uMagic) != UNZ_OK)
			err=UNZ_ERRNO;
		else if (uMagic!=0x04034b50)
			err=UNZ_BADZIPFILE;
	}

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&uData) != UNZ_OK)
		err=UNZ_ERRNO;
/*
	else if ((err==UNZ_OK) && (uData!=s->cur_file_info.wVersion))
		err=UNZ_BADZIPFILE;
*/
	if (unz64local_getShort(&s->z_filefunc, s->filestream,&uFlags) != UNZ_OK)
		err=UNZ_ERRNO;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&uData) != UNZ_OK)
		err=UNZ_ERRNO;
	else if ((err==UNZ_OK) && (uData!=s->cur_file_info.compression_method))
		err=UNZ_BADZIPFILE;

	if ((err==UNZ_OK) && (s->cur_file_info.compression_method!=0) &&
/* #ifdef HAVE_BZIP2 */
							(s->cur_file_info.compression_method!=Z_BZIP2ED) &&
/* #endif */
							(s->cur_file_info.compression_method!=Z_DEFLATED))
		err=UNZ_BADZIPFILE;

	if (unz64local_getLong(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) /* date/time */
		err=UNZ_ERRNO;

	if (unz64local_getLong(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) /* crc */
		err=UNZ_ERRNO;
	else if ((err==UNZ_OK) && (uData!=s->cur_file_info.crc) && ((uFlags & 8)==0))
		err=UNZ_BADZIPFILE;

	if (unz64local_getLong(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) /* size compr */
		err=UNZ_ERRNO;
	else if (uData != 0xFFFFFFFF && (err==UNZ_OK) && (uData!=s->cur_file_info.compressed_size) && ((uFlags & 8)==0))
		err=UNZ_BADZIPFILE;

	if (unz64local_getLong(&s->z_filefunc, s->filestream,&uData) != UNZ_OK) /* size uncompr */
		err=UNZ_ERRNO;
	else if (uData != 0xFFFFFFFF && (err==UNZ_OK) && (uData!=s->cur_file_info.uncompressed_size) && ((uFlags & 8)==0))
		err=UNZ_BADZIPFILE;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&size_filename) != UNZ_OK)
		err=UNZ_ERRNO;
	else if ((err==UNZ_OK) && (size_filename!=s->cur_file_info.size_filename))
		err=UNZ_BADZIPFILE;

	*piSizeVar += (uInt)size_filename;

	if (unz64local_getShort(&s->z_filefunc, s->filestream,&size_extra_field) != UNZ_OK)
		err=UNZ_ERRNO;
	*poffset_local_extrafield= s->cur_file_info_internal.offset_curfile +
									SIZEZIPLOCALHEADER + size_filename;
	*psize_local_extrafield = (uInt)size_extra_field;

	*piSizeVar += (uInt)size_extra_field;

	return err;
}

/*
	Open for reading data the current file in the zipfile.
	If there is no error and the file is opened, the return value is UNZ_OK.
*/
extern int	unzOpenCurrentFile(unzFile file)
{
	int	*method = NULL;
	int	*level = NULL;
	int	raw = 0;

	int err=UNZ_OK;
	uInt iSizeVar;
	unz64_s* s;
	file_in_zip64_read_info_s* pfile_in_zip_read_info;
	ZPOS64_T offset_local_extrafield;  /* offset of the local extra field */
	uInt  size_local_extrafield;    /* size of the local extra field */

	if (file==NULL)
		return UNZ_PARAMERROR;
	s=(unz64_s*)file;
	if (!s->current_file_ok)
		return UNZ_PARAMERROR;

	if (s->pfile_in_zip_read != NULL)
		unzCloseCurrentFile(file);

	if (unz64local_CheckCurrentFileCoherencyHeader(s,&iSizeVar, &offset_local_extrafield,&size_local_extrafield)!=UNZ_OK)
		return UNZ_BADZIPFILE;

	pfile_in_zip_read_info = (file_in_zip64_read_info_s*)ALLOC(sizeof(file_in_zip64_read_info_s));
	if (pfile_in_zip_read_info==NULL)
		return UNZ_INTERNALERROR;

	pfile_in_zip_read_info->read_buffer=(char*)ALLOC(UNZ_BUFSIZE);
	pfile_in_zip_read_info->offset_local_extrafield = offset_local_extrafield;
	pfile_in_zip_read_info->size_local_extrafield = size_local_extrafield;
	pfile_in_zip_read_info->pos_local_extrafield=0;
	pfile_in_zip_read_info->raw=raw;

	if (pfile_in_zip_read_info->read_buffer==NULL)
	{
		TRYFREE(pfile_in_zip_read_info);
		return UNZ_INTERNALERROR;
	}

	pfile_in_zip_read_info->stream_initialised=0;

	if (method!=NULL)
		*method = (int)s->cur_file_info.compression_method;

	if (level!=NULL)
	{
		*level = 6;
		switch (s->cur_file_info.flag & 0x06)
		{
			case 6 : *level = 1; break;
			case 4 : *level = 2; break;
			case 2 : *level = 9; break;
		}
	}

	if ((s->cur_file_info.compression_method!=0) &&
/* #ifdef HAVE_BZIP2 */
		(s->cur_file_info.compression_method!=Z_BZIP2ED) &&
/* #endif */
		(s->cur_file_info.compression_method!=Z_DEFLATED))

		err=UNZ_BADZIPFILE;

	pfile_in_zip_read_info->crc32_wait=s->cur_file_info.crc;
	pfile_in_zip_read_info->crc32=0;
	pfile_in_zip_read_info->total_out_64=0;
	pfile_in_zip_read_info->compression_method = s->cur_file_info.compression_method;
	pfile_in_zip_read_info->filestream=s->filestream;
	pfile_in_zip_read_info->z_filefunc=s->z_filefunc;
	pfile_in_zip_read_info->byte_before_the_zipfile=s->byte_before_the_zipfile;

	pfile_in_zip_read_info->stream.total_out = 0;

	if ((s->cur_file_info.compression_method==Z_BZIP2ED) && (!raw))
	{
#ifdef HAVE_BZIP2
		pfile_in_zip_read_info->bstream.bzalloc = (void *(*) (void *, int, int))0;
		pfile_in_zip_read_info->bstream.bzfree = (free_func)0;
		pfile_in_zip_read_info->bstream.opaque = (voidpf)0;
		pfile_in_zip_read_info->bstream.state = (voidpf)0;

		pfile_in_zip_read_info->stream.zalloc = (alloc_func)0;
		pfile_in_zip_read_info->stream.zfree = (free_func)0;
		pfile_in_zip_read_info->stream.opaque = (voidpf)0;
		pfile_in_zip_read_info->stream.next_in = (voidpf)0;
		pfile_in_zip_read_info->stream.avail_in = 0;

		err=BZ2_bzDecompressInit(&pfile_in_zip_read_info->bstream, 0, 0);
		if (err == Z_OK)
			pfile_in_zip_read_info->stream_initialised=Z_BZIP2ED;
		else
		{
			TRYFREE(pfile_in_zip_read_info->read_buffer);
			TRYFREE(pfile_in_zip_read_info);
			return err;
		}
#else
		pfile_in_zip_read_info->raw=1;
#endif
	}
	else if ((s->cur_file_info.compression_method==Z_DEFLATED) && (!raw))
	{
		pfile_in_zip_read_info->stream.zalloc = (alloc_func)0;
		pfile_in_zip_read_info->stream.zfree = (free_func)0;
		pfile_in_zip_read_info->stream.opaque = (voidpf)0;
		pfile_in_zip_read_info->stream.next_in = 0;
		pfile_in_zip_read_info->stream.avail_in = 0;

		err=inflateInit2(&pfile_in_zip_read_info->stream, -MAX_WBITS);
		if (err == Z_OK)
			pfile_in_zip_read_info->stream_initialised=Z_DEFLATED;
		else
		{
			TRYFREE(pfile_in_zip_read_info->read_buffer);
			TRYFREE(pfile_in_zip_read_info);
			return err;
		}
		/* windowBits is passed < 0 to tell that there is no zlib header.
			* Note that in this case inflate *requires* an extra "dummy" byte
			* after the compressed stream in order to complete decompression and
			* return Z_STREAM_END.
			* In unzip, i don't wait absolutely Z_STREAM_END because I known the
			* size of both compressed and uncompressed data
			*/
	}
	pfile_in_zip_read_info->rest_read_compressed = s->cur_file_info.compressed_size ;
	pfile_in_zip_read_info->rest_read_uncompressed = s->cur_file_info.uncompressed_size ;

	pfile_in_zip_read_info->pos_in_zipfile = s->cur_file_info_internal.offset_curfile + SIZEZIPLOCALHEADER + iSizeVar;
	pfile_in_zip_read_info->stream.avail_in = (uInt)0;

	s->pfile_in_zip_read = pfile_in_zip_read_info;
				s->encrypted = 0;

	return UNZ_OK;
}

/*
	Read bytes from the current file.
	buf contain buffer where data must be copied
	len the size of buf.

	return the number of byte copied if somes bytes are copied
	return 0 if the end of file was reached
	return <0 with error code if there is an error
	(UNZ_ERRNO for IO error, or zLib error for uncompress error)
*/
extern int	unzReadCurrentFile(unzFile file, voidp buf, unsigned len)
{
	int err=UNZ_OK;
	uInt iRead = 0;
	unz64_s* s;
	file_in_zip64_read_info_s* pfile_in_zip_read_info;
	if (file==NULL)
		return UNZ_PARAMERROR;
	s=(unz64_s*)file;
	pfile_in_zip_read_info=s->pfile_in_zip_read;

	if (pfile_in_zip_read_info==NULL)
		return UNZ_PARAMERROR;

	if (pfile_in_zip_read_info->read_buffer == NULL)
		return UNZ_END_OF_LIST_OF_FILE;
	if (len==0)
		return 0;

	pfile_in_zip_read_info->stream.next_out = (Bytef*)buf;

	pfile_in_zip_read_info->stream.avail_out = (uInt)len;

	if ((len>pfile_in_zip_read_info->rest_read_uncompressed) &&
		(!(pfile_in_zip_read_info->raw)))
		pfile_in_zip_read_info->stream.avail_out = (uInt)pfile_in_zip_read_info->rest_read_uncompressed;

	if ((len>pfile_in_zip_read_info->rest_read_compressed+
			pfile_in_zip_read_info->stream.avail_in) &&
			(pfile_in_zip_read_info->raw))
		pfile_in_zip_read_info->stream.avail_out = (uInt)pfile_in_zip_read_info->rest_read_compressed+
			pfile_in_zip_read_info->stream.avail_in;

	while (pfile_in_zip_read_info->stream.avail_out>0)
	{
		if ((pfile_in_zip_read_info->stream.avail_in==0) &&
			(pfile_in_zip_read_info->rest_read_compressed>0))
		{
			uInt uReadThis = UNZ_BUFSIZE;
			if (pfile_in_zip_read_info->rest_read_compressed<uReadThis)
				uReadThis = (uInt)pfile_in_zip_read_info->rest_read_compressed;
			if (uReadThis == 0)
				return UNZ_EOF;
			if (ZSEEK64(pfile_in_zip_read_info->z_filefunc,
						pfile_in_zip_read_info->filestream,
						pfile_in_zip_read_info->pos_in_zipfile +
							pfile_in_zip_read_info->byte_before_the_zipfile,
							ZLIB_FILEFUNC_SEEK_SET)!=0)
				return UNZ_ERRNO;
			if (ZREAD64(pfile_in_zip_read_info->z_filefunc,
						pfile_in_zip_read_info->filestream,
						pfile_in_zip_read_info->read_buffer,
						uReadThis)!=uReadThis)
				return UNZ_ERRNO;

			pfile_in_zip_read_info->pos_in_zipfile += uReadThis;
			pfile_in_zip_read_info->rest_read_compressed -= uReadThis;

			pfile_in_zip_read_info->stream.next_in = (Bytef*)pfile_in_zip_read_info->read_buffer;
			pfile_in_zip_read_info->stream.avail_in = (uInt)uReadThis;
		}

		if ((pfile_in_zip_read_info->compression_method==0) || (pfile_in_zip_read_info->raw))
		{
			uInt uDoCopy,i ;

			if ((pfile_in_zip_read_info->stream.avail_in == 0) &&
				(pfile_in_zip_read_info->rest_read_compressed == 0))
				return (iRead==0) ? UNZ_EOF : (int)iRead;

			if (pfile_in_zip_read_info->stream.avail_out < pfile_in_zip_read_info->stream.avail_in)
				uDoCopy = pfile_in_zip_read_info->stream.avail_out;
			else
				uDoCopy = pfile_in_zip_read_info->stream.avail_in;

			for (i=0;i<uDoCopy;i++)
				*(pfile_in_zip_read_info->stream.next_out+i) = *(pfile_in_zip_read_info->stream.next_in+i);

			pfile_in_zip_read_info->total_out_64 = pfile_in_zip_read_info->total_out_64 + uDoCopy;
			pfile_in_zip_read_info->crc32 = crc32(	pfile_in_zip_read_info->crc32,
													pfile_in_zip_read_info->stream.next_out,
													uDoCopy);
			pfile_in_zip_read_info->rest_read_uncompressed-=uDoCopy;
			pfile_in_zip_read_info->stream.avail_in -= uDoCopy;
			pfile_in_zip_read_info->stream.avail_out -= uDoCopy;
			pfile_in_zip_read_info->stream.next_out += uDoCopy;
			pfile_in_zip_read_info->stream.next_in += uDoCopy;
			pfile_in_zip_read_info->stream.total_out += uDoCopy;
			iRead += uDoCopy;
		}
		else if (pfile_in_zip_read_info->compression_method==Z_BZIP2ED)
		{
#ifdef HAVE_BZIP2
			uLong uTotalOutBefore,uTotalOutAfter;
			const Bytef *bufBefore;
			uLong uOutThis;

			pfile_in_zip_read_info->bstream.next_in        = (char*)pfile_in_zip_read_info->stream.next_in;
			pfile_in_zip_read_info->bstream.avail_in       = pfile_in_zip_read_info->stream.avail_in;
			pfile_in_zip_read_info->bstream.total_in_lo32  = pfile_in_zip_read_info->stream.total_in;
			pfile_in_zip_read_info->bstream.total_in_hi32  = 0;
			pfile_in_zip_read_info->bstream.next_out       = (char*)pfile_in_zip_read_info->stream.next_out;
			pfile_in_zip_read_info->bstream.avail_out      = pfile_in_zip_read_info->stream.avail_out;
			pfile_in_zip_read_info->bstream.total_out_lo32 = pfile_in_zip_read_info->stream.total_out;
			pfile_in_zip_read_info->bstream.total_out_hi32 = 0;

			uTotalOutBefore = pfile_in_zip_read_info->bstream.total_out_lo32;
			bufBefore = (const Bytef *)pfile_in_zip_read_info->bstream.next_out;

			err=BZ2_bzDecompress(&pfile_in_zip_read_info->bstream);

			uTotalOutAfter = pfile_in_zip_read_info->bstream.total_out_lo32;
			uOutThis = uTotalOutAfter-uTotalOutBefore;

			pfile_in_zip_read_info->total_out_64 = pfile_in_zip_read_info->total_out_64 + uOutThis;

			pfile_in_zip_read_info->crc32 = crc32(pfile_in_zip_read_info->crc32,bufBefore, (uInt)(uOutThis));
			pfile_in_zip_read_info->rest_read_uncompressed -= uOutThis;
			iRead += (uInt)(uTotalOutAfter - uTotalOutBefore);

			pfile_in_zip_read_info->stream.next_in   = (Bytef*)pfile_in_zip_read_info->bstream.next_in;
			pfile_in_zip_read_info->stream.avail_in  = pfile_in_zip_read_info->bstream.avail_in;
			pfile_in_zip_read_info->stream.total_in  = pfile_in_zip_read_info->bstream.total_in_lo32;
			pfile_in_zip_read_info->stream.next_out  = (Bytef*)pfile_in_zip_read_info->bstream.next_out;
			pfile_in_zip_read_info->stream.avail_out = pfile_in_zip_read_info->bstream.avail_out;
			pfile_in_zip_read_info->stream.total_out = pfile_in_zip_read_info->bstream.total_out_lo32;

			if (err==BZ_STREAM_END)
				return (iRead==0) ? UNZ_EOF : iRead;
			if (err!=BZ_OK)
				break;
#endif
		} // end Z_BZIP2ED
		else
		{
			ZPOS64_T uTotalOutBefore,uTotalOutAfter;
			const Bytef *bufBefore;
			ZPOS64_T uOutThis;
			int flush=Z_SYNC_FLUSH;

			uTotalOutBefore = pfile_in_zip_read_info->stream.total_out;
			bufBefore = pfile_in_zip_read_info->stream.next_out;

			/*
			if ((pfile_in_zip_read_info->rest_read_uncompressed ==
						pfile_in_zip_read_info->stream.avail_out) &&
				(pfile_in_zip_read_info->rest_read_compressed == 0))
				flush = Z_FINISH;
			*/
			err=inflate(&pfile_in_zip_read_info->stream,flush);

			if ((err>=0) && (pfile_in_zip_read_info->stream.msg!=NULL))
				err = Z_DATA_ERROR;

			uTotalOutAfter = pfile_in_zip_read_info->stream.total_out;
			/* Detect overflow, because z_stream.total_out is uLong (32 bits) */
			if (uTotalOutAfter<uTotalOutBefore)
				uTotalOutAfter += 1LL << 32; /* Add maximum value of uLong + 1 */
			uOutThis = uTotalOutAfter-uTotalOutBefore;

			pfile_in_zip_read_info->total_out_64 = pfile_in_zip_read_info->total_out_64 + uOutThis;

			pfile_in_zip_read_info->crc32 = crc32(pfile_in_zip_read_info->crc32,bufBefore, (uInt)(uOutThis));

			pfile_in_zip_read_info->rest_read_uncompressed -= uOutThis;

			iRead += (uInt)(uTotalOutAfter - uTotalOutBefore);

			if (err==Z_STREAM_END)
				return (iRead==0) ? UNZ_EOF : (int)iRead;
			if (err!=Z_OK)
				break;
		}
	}

	if (err==Z_OK)
		return (int)iRead;
	return err;
}

/*
	Close the file in zip opened with unzOpenCurrentFile
	Return UNZ_CRCERROR if all the file was read but the CRC is not good
*/
extern int  unzCloseCurrentFile (unzFile file)
{
	int err=UNZ_OK;

	unz64_s* s;
	file_in_zip64_read_info_s* pfile_in_zip_read_info;
	if (file==NULL)
		return UNZ_PARAMERROR;
	s=(unz64_s*)file;
	pfile_in_zip_read_info=s->pfile_in_zip_read;

	if (pfile_in_zip_read_info==NULL)
		return UNZ_PARAMERROR;

	if ((pfile_in_zip_read_info->rest_read_uncompressed == 0) &&
		(!pfile_in_zip_read_info->raw))
	{
		if (pfile_in_zip_read_info->crc32 != pfile_in_zip_read_info->crc32_wait)
			err=UNZ_CRCERROR;
	}


	TRYFREE(pfile_in_zip_read_info->read_buffer);
	pfile_in_zip_read_info->read_buffer = NULL;
	if (pfile_in_zip_read_info->stream_initialised == Z_DEFLATED)
		inflateEnd(&pfile_in_zip_read_info->stream);
#ifdef HAVE_BZIP2
	else if (pfile_in_zip_read_info->stream_initialised == Z_BZIP2ED)
		BZ2_bzDecompressEnd(&pfile_in_zip_read_info->bstream);
#endif

	pfile_in_zip_read_info->stream_initialised = 0;
	TRYFREE(pfile_in_zip_read_info);

	s->pfile_in_zip_read=NULL;

	return err;
}

//----------------------------------------------------------------------------