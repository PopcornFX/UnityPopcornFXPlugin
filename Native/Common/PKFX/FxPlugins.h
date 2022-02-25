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
		EPlugin_CompilerBackendVM	= 1 << 0,
		EPlugin_CompilerBackendD3D11= 1 << 1,	// TODO: Rename into 'EPlugin_CompilerBackendD3D', used by both D3D11 and D3D12
		EPlugin_ImageCodecDDS		= 1 << 2,
		EPlugin_ImageCodecPNG		= 1 << 3,
		EPlugin_ImageCodecJPG		= 1 << 4,
		EPlugin_ImageCodecTGA		= 1 << 5,
		EPlugin_ImageCodecTIFF		= 1 << 6,
		EPlugin_ImageCodecPKM		= 1 << 7,
		EPlugin_ImageCodecPVR		= 1 << 8,
		EPlugin_ImageCodecHDR		= 1 << 11,
		EPlugin_ImageCodecEXR		= 1 << 12,

		EPlugin_MeshCodecFBX		= 1 << 16,

		EPlugin_CompilerBackendPSSLC = 1 << 17,

		EPlugin_All					=	EPlugin_CompilerBackendVM |
										EPlugin_ImageCodecDDS |
										EPlugin_ImageCodecPNG |
										EPlugin_ImageCodecJPG |
										EPlugin_ImageCodecTGA
	};

	// loads selected plugins. set 'selected' to a bitwise combinations of the above plugin flags. must be matched by a call to 'UnregisterPlugins'
	bool	RegisterPlugins(u32 selected = 0);

	// unloads all previously loaded plugins
	void	UnregisterPlugins();

}

//----------------------------------------------------------------------------

#endif	// __PKFX_PLUGINS_H__
