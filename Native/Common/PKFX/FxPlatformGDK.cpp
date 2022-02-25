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

#if !defined(PK_GDK)
#	error This file should only be compiled on GDK platforms
#endif

#include <Windows.h>
#include <XGameRuntimeInit.h>

namespace	PKFX
{
	bool	PlatformStartup()
	{
#if !defined(PK_WINDOWS)
		if (FAILED(XGameRuntimeInitialize()))
			return false;
#endif
		return true;
	}

	bool	PlatformShutdown()
	{
#if !defined(PK_WINDOWS)
		XGameRuntimeUninitialize();
#endif
		return true;
	}
}
