//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "PKUnity_Plugins.h"

#include <pk_kernel/include/kr_plugins.h>

#define	USE_COMPILER_BACKEND

//----------------------------------------------------------------------------
//
//	toolkit to load/unload PKFX runtime plugins
//
//----------------------------------------------------------------------------

#if	PK_UNITY_EDITOR
	// Only used for asset baking:
#	define USE_IMAGE_PLUGINS	1
#	if	defined(PK_CPU_64BITS) 
#		if defined(PK_LINUX)
#			define USE_FBX_PLUGIN	0
#		else
			// only works for 64bits CPU
#			define USE_FBX_PLUGIN	1
#		endif
#	endif
#else
#	define USE_IMAGE_PLUGINS	0
#	define USE_FBX_PLUGIN		0
#endif

#ifdef	PK_PLUGINS_STATIC
#	if defined(USE_COMPILER_BACKEND)
		PK_PLUGIN_DECLARE(CCompilerBackendCPU_VM);
#	endif

#	if (USE_IMAGE_PLUGINS != 0)
		PK_PLUGIN_DECLARE(CImagePKIMCodec);
		PK_PLUGIN_DECLARE(CImageDDSCodec);
		PK_PLUGIN_DECLARE(CImagePNGCodec);
		PK_PLUGIN_DECLARE(CImageTGACodec);
		PK_PLUGIN_DECLARE(CImageJPEGCodec);
		PK_PLUGIN_DECLARE(CImageTIFFCodec);
		PK_PLUGIN_DECLARE(CImagePKMCodec);
		PK_PLUGIN_DECLARE(CImagePVRCodec);
		PK_PLUGIN_DECLARE(CImageHDRCodec);
		PK_PLUGIN_DECLARE(CImageEXRCodec);
#	endif

#	if (USE_FBX_PLUGIN != 0)
		PK_PLUGIN_DECLARE(CMeshCodecFBX);
#	endif

#	if defined(PK_DEBUG)
#		define	PK_PLUGIN_POSTFIX_BUILD		"_D"
#	else
#		define	PK_PLUGIN_POSTFIX_BUILD		""
#	endif
#endif

//--------------------------------------------------------------------------------------

namespace	PKFX
{
	static u32	g_LoadedPlugins = 0;

	//--------------------------------------------------------------------------------------
	//
	//	Loads selected plugins
	//
	//--------------------------------------------------------------------------------------

	bool	RegisterPlugins(u32 selected /*= 0*/)
	{
		PK_ASSERT(g_LoadedPlugins == 0);

		bool	success = true;
#ifndef	PK_PLUGINS_STATIC
		// plugins are .dll
		CPluginManager::RegisterDirectory("Plugins", false);

#else
		// plugins are linked statically
		if (selected & EPlugin_CompilerBackendVM)
		{
			const char		*backendPath = "Plugins/CBCPU_VM_Win32" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendCPU_VM();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}
#	if (USE_IMAGE_PLUGINS != 0)
		if (selected & EPlugin_ImageCodecPKIM)
		{
			const char		*codecPath = "Plugins/image_codec_pkim" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImagePKIMCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecDDS)
		{
			const char		*codecPath = "Plugins/image_codec_dds" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImageDDSCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecPNG)
		{
			const char		*codecPath = "Plugins/image_codec_png" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImagePNGCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecJPG)
		{
			const char		*codecPath = "Plugins/image_codec_jpeg" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImageJPEGCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecTGA)
		{
			const char		*codecPath = "Plugins/image_codec_tga" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImageTGACodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecTIFF)
		{
			const char		*codecPath = "Plugins/image_codec_tiff" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImageTIFFCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecPKM)
		{
			const char		*codecPath = "Plugins/image_codec_pkm" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImagePKMCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecPVR)
		{
			const char		*codecPath = "Plugins/image_codec_pvr" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImagePVRCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecHDR)
		{
			const char		*codecPath = "Plugins/image_codec_hdr" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImageHDRCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecEXR)
		{
			const char		*codecPath = "Plugins/image_codec_hdr" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CImageEXRCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#	endif	// (USE_IMAGE_PLUGINS != 0)

#	if (USE_FBX_PLUGIN != 0)
		if (selected & EPlugin_MeshCodecFBX)
		{
			const char		*codecPath = "Plugins/MeshCodecFBX" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CMeshCodecFBX();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#	endif	// (USE_FBX_PLUGIN != 0)
#endif

		g_LoadedPlugins = selected;
		return success;
	}

	//--------------------------------------------------------------------------------------
	//
	//	Uloads all plugins
	//
	//--------------------------------------------------------------------------------------

	void	UnregisterPlugins()
	{
		// unregister plugins:
#ifdef	PK_PLUGINS_STATIC
		if (g_LoadedPlugins & EPlugin_CompilerBackendVM)
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendCPU_VM();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendCPU_VM();
		}

#	if (USE_IMAGE_PLUGINS != 0)
		if (g_LoadedPlugins & EPlugin_ImageCodecPKIM)
		{
			IPluginModule	*codec = GetPlugin_CImagePKIMCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImagePKIMCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecDDS)
		{
			IPluginModule	*codec = GetPlugin_CImageDDSCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImageDDSCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecPNG)
		{
			IPluginModule	*codec = GetPlugin_CImagePNGCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImagePNGCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecJPG)
		{
			IPluginModule	*codec = GetPlugin_CImageJPEGCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImageJPEGCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecTGA)
		{
			IPluginModule	*codec = GetPlugin_CImageTGACodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImageTGACodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecTIFF)
		{
			IPluginModule	*codec = GetPlugin_CImageTIFFCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImageTIFFCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecPKM)
		{
			IPluginModule	*codec = GetPlugin_CImagePKMCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImagePKMCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecPVR)
		{
			IPluginModule	*codec = GetPlugin_CImagePVRCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImagePVRCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecHDR)
		{
			IPluginModule	*codec = GetPlugin_CImageHDRCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImageHDRCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecEXR)
		{
			IPluginModule	*codec = GetPlugin_CImageEXRCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImageEXRCodec();
		}
#	endif	// (USE_IMAGE_PLUGINS != 0)

#	if (USE_FBX_PLUGIN != 0)
		if (g_LoadedPlugins & EPlugin_MeshCodecFBX)
		{
			IPluginModule	*codec = GetPlugin_CMeshCodecFBX();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CMeshCodecFBX();
		}
#	endif	// (USE_FBX_PLUGIN != 0)
#endif

		g_LoadedPlugins = 0;
	}

}

//----------------------------------------------------------------------------

