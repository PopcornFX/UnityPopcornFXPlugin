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

#ifndef __PKFX_PLUGINS_H__
#define __PKFX_PLUGINS_H__

using namespace PK_NAMESPACE;

//----------------------------------------------------------------------------
//
//	Simple utilities to load/unload the PKFX runtime plugins
//
//----------------------------------------------------------------------------

namespace	PKFX
{

	enum	ERuntimePlugin
	{
		EPlugin_ImageCodecDDS			= 1 << 0,
		EPlugin_ImageCodecPNG			= 1 << 1,
		EPlugin_ImageCodecJPG			= 1 << 2,
		EPlugin_ImageCodecTGA			= 1 << 3,
		EPlugin_ImageCodecTIFF			= 1 << 4,
		EPlugin_ImageCodecPKM			= 1 << 5,
		EPlugin_ImageCodecPVR			= 1 << 6,
		EPlugin_ImageCodecHDR			= 1 << 7,
		EPlugin_ImageCodecEXR			= 1 << 8,
		EPlugin_ImageCodecPKIM			= 1 << 9,

		EPlugin_MeshCodecFBX			= 1 << 16,
		EPlugin_MeshCodecGranny			= 1 << 17,

		EPlugin_CompilerBackendVM		= 1 << 20,
		EPlugin_CompilerBackendISPC		= 1 << 21,
		EPlugin_CompilerBackendD3D		= 1 << 22,
		EPlugin_CompilerBackendPSSLC	= 1 << 23,

		// All "base" plugins
		EPlugin_Default =	EPlugin_CompilerBackendVM |
							EPlugin_ImageCodecPKIM |
							EPlugin_ImageCodecDDS |
							EPlugin_ImageCodecPNG |
							EPlugin_ImageCodecJPG |
							EPlugin_ImageCodecTGA,
	};

	// loads selected plugins. set 'selected' to a bitwise combinations of the above plugin flags. must be matched by a call to 'UnregisterPlugins'
	bool	RegisterPlugins(u32 selected = u32(-1));	// By default: Register everything we can

	// unloads all previously loaded plugins
	void	UnregisterPlugins();

	// Deprecated, to remove
#if defined(PK_COMPILER_MSVC)
	PK_DEPRECATED("Deprecated v1.19.0.19915: Use explicit bitwise combination instead, or no argument to 'RegisterPlugins' to register everything that was compiled in")
	enum EPlugin_All_Deprecated { EPlugin_All = EPlugin_CompilerBackendVM | EPlugin_ImageCodecPKIM | EPlugin_ImageCodecDDS | EPlugin_ImageCodecPNG | EPlugin_ImageCodecJPG | EPlugin_ImageCodecTGA };
	PK_DEPRECATED("Deprecated v1.19.0.19915: Use EPlugin_CompilerBackendD3D instead")
	enum EPlugin_D3D11_Deprecated { EPlugin_CompilerBackendD3D11 = EPlugin_CompilerBackendD3D };
#else
	enum PK_DEPRECATED("Deprecated v1.19.0.19915: Use explicit bitwise combination instead, or no argument to 'RegisterPlugins' to register everything that was compiled in")
	EPlugin_All_Deprecated { EPlugin_All = EPlugin_CompilerBackendVM | EPlugin_ImageCodecPKIM | EPlugin_ImageCodecDDS | EPlugin_ImageCodecPNG | EPlugin_ImageCodecJPG | EPlugin_ImageCodecTGA };
	enum PK_DEPRECATED("Deprecated v1.19.0.19915: Use EPlugin_CompilerBackendD3D instead")
	EPlugin_D3D11_Deprecated { EPlugin_CompilerBackendD3D11 = EPlugin_CompilerBackendD3D };
#endif
}

//----------------------------------------------------------------------------

#endif	// __PKFX_PLUGINS_H__
