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

#ifndef	__PKFX_PLATFORM_INTERFACE_H__
#define	__PKFX_PLATFORM_INTERFACE_H__

#include "precompiled.h"

using namespace	PK_NAMESPACE;

namespace	PKFX
{
	bool	PlatformStartup();
	bool	PlatformShutdown();
}

#endif	// __PKFX_PLATFORM_INTERFACE_H__
