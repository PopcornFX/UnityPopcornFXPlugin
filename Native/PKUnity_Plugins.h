//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once


#ifndef __PKUNITY_PLUGINS_H__
#	define __PKUNITY_PLUGINS_H__

using namespace PopcornFX;

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

		EPlugin_MeshCodecFBX			= 1 << 16,
		EPlugin_MeshCodecGranny			= 1 << 17,

		EPlugin_CompilerBackendVM		= 1 << 20,
		EPlugin_CompilerBackendISPC		= 1 << 21,
		EPlugin_CompilerBackendD3D		= 1 << 22,
		EPlugin_CompilerBackendPSSLC	= 1 << 23,

		EPlugin_Editor				=	EPlugin_CompilerBackendVM |
										EPlugin_ImageCodecDDS |
										EPlugin_ImageCodecPNG |
										EPlugin_ImageCodecJPG |
										EPlugin_ImageCodecTGA |
										EPlugin_ImageCodecTIFF |
										EPlugin_ImageCodecPKM |
										EPlugin_ImageCodecPVR |
										EPlugin_ImageCodecHDR |
										EPlugin_ImageCodecEXR |
										EPlugin_MeshCodecFBX,

		EPlugin_Runtime				=	EPlugin_CompilerBackendVM
	};

	// loads selected plugins. set 'selected' to a bitwise combinations of the above plugin flags. must be matched by a call to 'UnregisterPlugins'
	bool	RegisterPlugins(u32 selected = 0);

	// unloads all previously loaded plugins
	void	UnregisterPlugins();

}

//----------------------------------------------------------------------------

#endif	// __PKUNITY_PLUGINS_H__
