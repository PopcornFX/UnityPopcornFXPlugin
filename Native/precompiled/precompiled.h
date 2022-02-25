//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#if defined(_WIN32) && !MONOLITHIC
#	define	WIN32_LEAN_AND_MEAN		1
#	define	WIN32_EXTRA_LEAN
#	define	VC_EXTRALEAN
#	include <windows.h>
#endif

//----------------------------------------------------------------------------

#define	PV_MODULE_NAME		"UnityPlugin"
#define	PV_MODULE_SYM		UnityPlugin

#include <pkapi/include/pk_precompiled_default.h>

PK_LOG_MODULE_DEFINE();

#define	__UNITY_API_BEGIN	namespace PKUnity {
#define	__UNITY_API_END		};

using namespace PopcornFX;

//----------------------------------------------------------------------------
