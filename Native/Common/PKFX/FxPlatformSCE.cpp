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

#if !defined(PK_ORBIS) && !defined(PK_UNKNOWN2)
#	error This file should only be compiled on PlayStation
#endif

#if defined(PK_ORBIS)
size_t				sceLibcHeapSize = 256*1024*1024;	// 256 Mb
#else
extern "C" size_t	sceLibcHeapSize = 256*1024*1024;	// 256 Mb
#endif

namespace	PKFX
{
	bool	PlatformStartup() { return true; }
	bool	PlatformShutdown() { return true; }
}
