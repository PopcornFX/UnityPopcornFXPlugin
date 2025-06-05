#pragma once

//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PKFX_STARTUP_H__
#define	__PKFX_STARTUP_H__

using namespace	PK_NAMESPACE;

//----------------------------------------------------------------------------

namespace	PKFX
{
	bool	RuntimeStartup(const char *assertScriptFilePath = null, bool installDefaultLogger = true, u32 maxThreads = 0);	// maxThreads only used on PK_DESKTOP
	void	RuntimeShutdown();
}

//----------------------------------------------------------------------------

#endif	// __PKFX_STARTUP_H__
