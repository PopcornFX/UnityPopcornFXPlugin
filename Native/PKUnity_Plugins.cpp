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
		// only works for 64bits CPU
#		define USE_FBX_PLUGIN	1
#	endif
#else
#	define USE_IMAGE_PLUGINS	0
#	define USE_FBX_PLUGIN		0
#endif

#ifdef	PK_PLUGINS_STATIC
#	if defined(USE_COMPILER_BACKEND)
		PK_PLUGIN_DECLARE(CCompilerBackendCPU_VM);
#	endif

#	if defined(USE_IMAGE_PLUGINS)
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

#	if defined(USE_FBX_PLUGIN)
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
#	ifndef	PK_PLUGINS_STATIC
		// plugins are .dll
		CPluginManager::RegisterDirectory("Plugins", false);

#	else
		// plugins are linked statically
		if (selected & EPlugin_CompilerBackendVM)
		{
			const char		*backendPath = "Plugins/CBCPU_VM_Win32" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendCPU_VM();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}
#		if USE_IMAGE_PLUGINS
		if (selected & EPlugin_ImageCodecDDS)
		{
			const char		*codecPathDDS = "Plugins/image_codec_dds" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codecDDS = StartupPlugin_CImageDDSCodec();
			success &= (codecDDS != null && CPluginManager::PluginRegister(codecDDS, true, codecPathDDS));
		}

		if (selected & EPlugin_ImageCodecPNG)
		{
			const char		*codecPathPNG = "Plugins/image_codec_png" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codecPNG = StartupPlugin_CImagePNGCodec();
			success &= (codecPNG != null && CPluginManager::PluginRegister(codecPNG, true, codecPathPNG));
		}

		if (selected & EPlugin_ImageCodecJPG)
		{
			const char		*codecPathJPG = "Plugins/image_codec_jpeg" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codecJPG = StartupPlugin_CImageJPEGCodec();
			success &= (codecJPG != null && CPluginManager::PluginRegister(codecJPG, true, codecPathJPG));
		}

		if (selected & EPlugin_ImageCodecTGA)
		{
			const char		*codecPathTGA = "Plugins/image_codec_tga" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codecTGA = StartupPlugin_CImageTGACodec();
			success &= (codecTGA != null && CPluginManager::PluginRegister(codecTGA, true, codecPathTGA));
		}

		if (selected & EPlugin_ImageCodecTIFF)
		{
			const char		*codecPathTIFF = "Plugins/image_codec_tiff" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codecTIFF = StartupPlugin_CImageTIFFCodec();
			success &= (codecTIFF != null && CPluginManager::PluginRegister(codecTIFF, true, codecPathTIFF));
		}

		if (selected & EPlugin_ImageCodecPKM)
		{
			const char		*codecPathPKM = "Plugins/image_codec_pkm" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codecPKM = StartupPlugin_CImagePKMCodec();
			success &= (codecPKM != null && CPluginManager::PluginRegister(codecPKM, true, codecPathPKM));
		}

		if (selected & EPlugin_ImageCodecPVR)
		{
			const char		*codecPathPVR = "Plugins/image_codec_pvr" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codecPVR = StartupPlugin_CImagePVRCodec();
			success &= (codecPVR != null && CPluginManager::PluginRegister(codecPVR, true, codecPathPVR));
		}

		if (selected & EPlugin_ImageCodecHDR)
		{
			const char		*codecPathHDR = "Plugins/image_codec_hdr" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codecHDR = StartupPlugin_CImageHDRCodec();
			success &= (codecHDR != null && CPluginManager::PluginRegister(codecHDR, true, codecPathHDR));
		}

		if (selected & EPlugin_ImageCodecEXR)
		{
			const char		*codecPathEXR = "Plugins/image_codec_hdr" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codecEXR = StartupPlugin_CImageEXRCodec();
			success &= (codecEXR != null && CPluginManager::PluginRegister(codecEXR, true, codecPathEXR));
		}
#		endif

#		if USE_FBX_PLUGIN
		if (selected & EPlugin_MeshCodecFBX)
		{
			const char		*codecPath = "Plugins/MeshCodecFBX" PK_PLUGIN_POSTFIX_BUILD;
			IPluginModule	*codec = StartupPlugin_CMeshCodecFBX();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#		endif
#	endif

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
#	ifdef	PK_PLUGINS_STATIC
		if (g_LoadedPlugins & EPlugin_CompilerBackendVM)
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendCPU_VM();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendCPU_VM();
		}

#		if	USE_IMAGE_PLUGINS
		if (g_LoadedPlugins & EPlugin_ImageCodecDDS)
		{
			IPluginModule	*codecDDS = GetPlugin_CImageDDSCodec();
			(codecDDS != null && CPluginManager::PluginRelease(codecDDS));
			ShutdownPlugin_CImageDDSCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecPNG)
		{
			IPluginModule	*codecPNG = GetPlugin_CImagePNGCodec();
			(codecPNG != null && CPluginManager::PluginRelease(codecPNG));
			ShutdownPlugin_CImagePNGCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecJPG)
		{
			IPluginModule	*codecJPG = GetPlugin_CImageJPEGCodec();
			(codecJPG != null && CPluginManager::PluginRelease(codecJPG));
			ShutdownPlugin_CImageJPEGCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecTGA)
		{
			IPluginModule	*codecTGA = GetPlugin_CImageTGACodec();
			(codecTGA != null && CPluginManager::PluginRelease(codecTGA));
			ShutdownPlugin_CImageTGACodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecTIFF)
		{
			IPluginModule	*codecTIFF = GetPlugin_CImageTIFFCodec();
			(codecTIFF != null && CPluginManager::PluginRelease(codecTIFF));
			ShutdownPlugin_CImageTIFFCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecPKM)
		{
			IPluginModule	*codecPKM = GetPlugin_CImagePKMCodec();
			(codecPKM != null && CPluginManager::PluginRelease(codecPKM));
			ShutdownPlugin_CImagePKMCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecPVR)
		{
			IPluginModule	*codecPVR = GetPlugin_CImagePVRCodec();
			(codecPVR != null && CPluginManager::PluginRelease(codecPVR));
			ShutdownPlugin_CImagePVRCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecHDR)
		{
			IPluginModule	*codecHDR = GetPlugin_CImageHDRCodec();
			(codecHDR != null && CPluginManager::PluginRelease(codecHDR));
			ShutdownPlugin_CImageHDRCodec();
		}

		if (g_LoadedPlugins & EPlugin_ImageCodecEXR)
		{
			IPluginModule	*codecEXR = GetPlugin_CImageEXRCodec();
			(codecEXR != null && CPluginManager::PluginRelease(codecEXR));
			ShutdownPlugin_CImageEXRCodec();
		}
#		endif

#		if USE_FBX_PLUGIN
		if (g_LoadedPlugins & EPlugin_MeshCodecFBX)
		{
			IPluginModule	*codec = GetPlugin_CMeshCodecFBX();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CMeshCodecFBX();
		}
#		endif
#	endif

		g_LoadedPlugins = 0;
	}

}

//----------------------------------------------------------------------------

