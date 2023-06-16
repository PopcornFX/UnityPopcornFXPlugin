/* unzip.h -- IO for uncompress .zip files using zlib
   Version 1.1, February 14h, 2010
   part of the MiniZip project - ( http://www.winimage.com/zLibDll/minizip.html )

         Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

         Modifications of Unzip for Zip64
         Copyright (C) 2007-2008 Even Rouault

         Modifications for Zip64 support on both zip and unzip
         Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )

         2023: PopcornFX: Some simplifications & trimming down for PopcornFX AssetBaker

         ---------------------------------------------------------------------------------

        Condition of use and distribution are the same than zlib :

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  ---------------------------------------------------------------------------------

        Changes

        See header of unzip64.c

*/

#ifndef _unz64_H
#define _unz64_H

#include "zlib.h"

#if (!defined(_WIN32)) && (!defined(WIN32)) && (!defined(__APPLE__))
// Linux needs this to support file operation on files larger then 4+GB
// But might need better if/def to select just the platforms that needs them.
#	ifndef __USE_FILE_OFFSET64
#		define __USE_FILE_OFFSET64
#	endif
#	ifndef __USE_LARGEFILE64
#		define __USE_LARGEFILE64
#	endif
#	ifndef _LARGEFILE64_SOURCE
#		define _LARGEFILE64_SOURCE
#	endif
#	ifndef _FILE_OFFSET_BIT
#		define _FILE_OFFSET_BIT 64
#	endif
#endif

#include <stdio.h>
#include <stdlib.h>

#if defined(USE_FILE32API)
#	define fopen64 fopen
#	define ftello64 ftell
#	define fseeko64 fseek
#else
#	if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#		define fopen64 fopen
#		define ftello64 ftello
#		define fseeko64 fseeko
#	endif
#	ifdef _MSC_VER
#		define fopen64 fopen
#		if (_MSC_VER >= 1400) && (!(defined(NO_MSCVER_FILE64_FUNC)))
#			define ftello64 _ftelli64
#			define fseeko64 _fseeki64
#		else // old MSC
#			define ftello64 ftell
#			define fseeko64 fseek
#		endif
#	endif	// _MSC_VER
#endif

/* a type choosen by DEFINE */
#ifdef HAVE_64BIT_INT_CUSTOM
typedef  64BIT_INT_CUSTOM_TYPE ZPOS64_T;
#else
#	ifdef HAS_STDINT_H
#		include "stdint.h"
typedef uint64_t ZPOS64_T;
#	else
#		if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 ZPOS64_T;
#		else
typedef unsigned long long int ZPOS64_T;
#		endif
#	endif
#endif

#define ZLIB_FILEFUNC_SEEK_CUR					(1)
#define ZLIB_FILEFUNC_SEEK_END					(2)
#define ZLIB_FILEFUNC_SEEK_SET					(0)
#define ZLIB_FILEFUNC_MODE_READ					(1)

typedef uLong    (*read_file_func)(voidpf opaque, voidpf stream, void *buf, uLong size);
typedef int      (*close_file_func)(voidpf opaque, voidpf stream);
typedef int      (*testerror_file_func)(voidpf opaque, voidpf stream);
typedef ZPOS64_T (*tell_file_func)(voidpf opaque, voidpf stream);
typedef long     (*seek_file_func)(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin);
typedef voidpf   (*open_file_func)(voidpf opaque, const void *filename, int mode);

typedef struct zlib_filefunc_def_s
{
    open_file_func		zopen_file;
    read_file_func		zread_file;
    tell_file_func		ztell_file;
    seek_file_func		zseek_file;
    close_file_func		zclose_file;
    testerror_file_func	zerror_file;
	voidpf				opaque;
} zlib_filefunc_def;

#define Z_BZIP2ED 12

#if defined(STRICTUNZIP) || defined(STRICTZIPUNZIP)
/* like the STRICT of WIN32, we define a pointer that cannot be converted
    from (void*) without cast */
typedef struct TagunzFile__ { int unused; } unzFile__;
typedef unzFile__ *unzFile;
#else
typedef voidp unzFile;
#endif

#define UNZ_OK                          (0)
#define UNZ_END_OF_LIST_OF_FILE         (-100)
#define UNZ_ERRNO                       (Z_ERRNO)
#define UNZ_EOF                         (0)
#define UNZ_PARAMERROR                  (-102)
#define UNZ_BADZIPFILE                  (-103)
#define UNZ_INTERNALERROR               (-104)
#define UNZ_CRCERROR                    (-105)

/* tm_unz contain date/time info */
typedef struct tm_unz_s
{
    int tm_sec;             /* seconds after the minute - [0,59] */
    int tm_min;             /* minutes after the hour - [0,59] */
    int tm_hour;            /* hours since midnight - [0,23] */
    int tm_mday;            /* day of the month - [1,31] */
    int tm_mon;             /* months since January - [0,11] */
    int tm_year;            /* years - [1980..2044] */
} tm_unz;

/* unz_global_info structure contain global data about the ZIPfile
   These data comes from the end of central dir */
typedef struct unz_global_info_s
{
    ZPOS64_T number_entry;         /* total number of entries in
                                     the central dir on this disk */
    uLong size_comment;         /* size of the global comment of the zipfile */
} unz_global_info;

/* unz_file_info contain information about a file in the zipfile */
typedef struct unz_file_info_s
{
    uLong version;              /* version made by                 2 bytes */
    uLong version_needed;       /* version needed to extract       2 bytes */
    uLong flag;                 /* general purpose bit flag        2 bytes */
    uLong compression_method;   /* compression method              2 bytes */
    uLong dosDate;              /* last mod file date in Dos fmt   4 bytes */
    uLong crc;                  /* crc-32                          4 bytes */
    ZPOS64_T compressed_size;   /* compressed size                 8 bytes */
    ZPOS64_T uncompressed_size; /* uncompressed size               8 bytes */
    uLong size_filename;        /* filename length                 2 bytes */
    uLong size_file_extra;      /* extra field length              2 bytes */
    uLong size_file_comment;    /* file comment length             2 bytes */

    uLong disk_num_start;       /* disk number start               2 bytes */
    uLong internal_fa;          /* internal file attributes        2 bytes */
    uLong external_fa;          /* external file attributes        4 bytes */

    tm_unz tmu_date;
} unz_file_info;

extern unzFile	unzOpen(const void *path, zlib_filefunc_def *io_api);

extern int		unzClose(unzFile file);

extern int		unzGetGlobalInfo(unzFile file, unz_global_info *pglobal_info);

extern int		unzGoToNextFile(unzFile file);

extern int		unzGetCurrentFileInfo(	unzFile file,
										unz_file_info *pfile_info,
										char *szFileName,
										uLong fileNameBufferSize,
										void *extraField,
										uLong extraFieldBufferSize,
										char *szComment,
										uLong commentBufferSize);

extern int		unzOpenCurrentFile(unzFile file);

extern int		unzCloseCurrentFile(unzFile file);

extern int		unzReadCurrentFile(unzFile file, voidp buf, unsigned len);


#endif /* _unz64_H */
