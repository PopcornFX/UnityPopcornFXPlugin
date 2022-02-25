//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "pkapi/include/pk_precompiled_default.h"
#include "FxPlugins.h"

#include <pk_kernel/include/kr_plugins.h>
#include <pk_particles/include/ps_config.h>

#define	USE_COMPILER_BACKEND
#define	USE_IMAGE_PLUGINS
// The following are set in the project config (-D commandline)
//#define	USE_IMAGE_PLUGIN_HDR
//#define	USE_IMAGE_PLUGIN_EXR
//#define	USE_IMAGE_PLUGIN_TIFF
//#define	USE_IMAGE_PLUGIN_PKM
//#define	USE_IMAGE_PLUGIN_PVR
//#define	USE_FBXIMPORTER
//#define	USE_GRANNYIMPORTER

#if !defined(PK_COMPILER_BUILD_COMPILER_D3D) || \
	!defined(PK_COMPILER_BUILD_COMPILER_UNKNOWN2)
#	error Configuration error: Should be defined in ps_config.h
#endif

#if	defined(USE_COMPILER_BACKEND)
#	if !defined(USE_COMPILER_BACKEND_D3D)
#		if	(PK_COMPILER_BUILD_COMPILER_D3D != 0)
#			define	USE_COMPILER_BACKEND_D3D
#		endif
#	endif
#	if !defined(USE_COMPILER_BACKEND_UNKNOWN2)
#		if	(PK_COMPILER_BUILD_COMPILER_UNKNOWN2 != 0)
#			define	USE_COMPILER_BACKEND_UNKNOWN2
#		endif
#	endif
#endif

//----------------------------------------------------------------------------
//
//	toolkit to load/unload PKFX runtime plugins
//
//----------------------------------------------------------------------------

#ifdef	PK_PLUGINS_STATIC
#	if defined(USE_COMPILER_BACKEND)
		PK_PLUGIN_DECLARE(CCompilerBackendCPU_VM);
#		if defined(USE_COMPILER_BACKEND_D3D)
		PK_PLUGIN_DECLARE(CCompilerBackendGPU_D3D);
#		endif
#		if defined(USE_COMPILER_BACKEND_UNKNOWN2)
		PK_PLUGIN_DECLARE(CCompilerBackendGPU_PSSLC);
#		endif
#	endif

#	if defined(USE_IMAGE_PLUGINS)
		PK_PLUGIN_DECLARE(CImageDDSCodec);
		PK_PLUGIN_DECLARE(CImagePNGCodec);
		PK_PLUGIN_DECLARE(CImageTGACodec);
		PK_PLUGIN_DECLARE(CImageJPEGCodec);
#		if defined(USE_IMAGE_PLUGIN_HDR)
		PK_PLUGIN_DECLARE(CImageHDRCodec);
#		endif
#		if defined(USE_IMAGE_PLUGIN_EXR)
		PK_PLUGIN_DECLARE(CImageEXRCodec);
#		endif
#		if defined(USE_IMAGE_PLUGIN_TIFF)
		PK_PLUGIN_DECLARE(CImageTIFFCodec);
#		endif
#		if defined(USE_IMAGE_PLUGIN_PKM)
		PK_PLUGIN_DECLARE(CImagePKMCodec);
#		endif
#		if defined(USE_IMAGE_PLUGIN_PVR)
		PK_PLUGIN_DECLARE(CImagePVRCodec);
#		endif
#	endif

#	if defined(USE_FBXIMPORTER)
		PK_PLUGIN_DECLARE(CMeshCodecFBX);
#	endif
#	if defined(USE_GRANNYIMPORTER)
		PK_PLUGIN_DECLARE(CMeshCodecGranny);
#	endif

#	if defined(PK_DEBUG)
#		define	PK_PLUGIN_POSTFIX_BUILD		"_D"
#	else
#		define	PK_PLUGIN_POSTFIX_BUILD		""
#	endif
#	if	defined(PK_WINAPI)
#		define	PK_PLUGIN_POSTFIX_EXT		".dll"
#	else
#		define	PK_PLUGIN_POSTFIX_EXT		""
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
			const char	*backendPath = "Plugins/CBCPU_VM" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendCPU_VM();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}

#		if defined(USE_COMPILER_BACKEND_D3D)
		if (selected & EPlugin_CompilerBackendD3D11)
		{
			const char	*backendPath = "Plugins/CBGPU_D3D" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendGPU_D3D();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}
#		endif

#		if defined(USE_COMPILER_BACKEND_UNKNOWN2)
		if (selected & EPlugin_CompilerBackendPSSLC)
		{
			const char	*backendPath = "Plugins/CBGPU_PSSLC" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendGPU_PSSLC();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}
#		endif

		if (selected & EPlugin_ImageCodecDDS)
		{
			const char	*codecPathDDS = "Plugins/image_codec_dds" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codecDDS = StartupPlugin_CImageDDSCodec();
			success &= (codecDDS != null && CPluginManager::PluginRegister(codecDDS, true, codecPathDDS));
		}

		if (selected & EPlugin_ImageCodecPNG)
		{
			const char	*codecPathPNG = "Plugins/image_codec_png" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codecPNG = StartupPlugin_CImagePNGCodec();
			success &= (codecPNG != null && CPluginManager::PluginRegister(codecPNG, true, codecPathPNG));
		}

		if (selected & EPlugin_ImageCodecJPG)
		{
			const char	*codecPathJPG = "Plugins/image_codec_jpeg" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codecJPG = StartupPlugin_CImageJPEGCodec();
			success &= (codecJPG != null && CPluginManager::PluginRegister(codecJPG, true, codecPathJPG));
		}

		if (selected & EPlugin_ImageCodecTGA)
		{
			const char	*codecPathTGA = "Plugins/image_codec_tga" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codecTGA = StartupPlugin_CImageTGACodec();
			success &= (codecTGA != null && CPluginManager::PluginRegister(codecTGA, true, codecPathTGA));
		}

#		if defined(USE_IMAGE_PLUGIN_HDR)
		if (selected & EPlugin_ImageCodecHDR)
		{
			const char	*codecPathHDR = "Plugins/image_codec_hdr" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codecHDR = StartupPlugin_CImageHDRCodec();
			success &= (codecHDR != null && CPluginManager::PluginRegister(codecHDR, true, codecPathHDR));
		}
#		endif

#		if defined(USE_IMAGE_PLUGIN_EXR)
		if (selected & EPlugin_ImageCodecEXR)
		{
			const char	*codecPathEXR = "Plugins/image_codec_exr" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codecEXR = StartupPlugin_CImageEXRCodec();
			success &= (codecEXR != null && CPluginManager::PluginRegister(codecEXR, true, codecPathEXR));
		}
#		endif

#		if defined(USE_IMAGE_PLUGIN_TIFF)
		if (selected & EPlugin_ImageCodecTIFF)
		{
			const char	*codecPathTIFF = "Plugins/image_codec_tiff" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codecTIFF = StartupPlugin_CImageTIFFCodec();
			success &= (codecTIFF != null && CPluginManager::PluginRegister(codecTIFF, true, codecPathTIFF));
		}
#		endif

#		if defined(USE_IMAGE_PLUGIN_PKM)
		if (selected & EPlugin_ImageCodecPKM)
		{
			const char	*codecPathPKM = "Plugins/image_codec_pkm" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codecPKM = StartupPlugin_CImagePKMCodec();
			success &= (codecPKM != null && CPluginManager::PluginRegister(codecPKM, true, codecPathPKM));
		}
#		endif

#		if defined(USE_IMAGE_PLUGIN_PVR)
		if (selected & EPlugin_ImageCodecPVR)
		{
			const char	*codecPathPVR = "Plugins/image_codec_pvr" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codecPVR = StartupPlugin_CImagePVRCodec();
			success &= (codecPVR != null && CPluginManager::PluginRegister(codecPVR, true, codecPathPVR));
		}
#		endif

#		if defined(USE_FBXIMPORTER)
		if (selected & EPlugin_MeshCodecFBX)
		{
			const char	*codecPath = "Plugins/MeshCodecFBX" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CMeshCodecFBX();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#		endif

#		if defined(USE_GRANNYIMPORTER)
//		if (selected & EPlugin_MeshCodecGranny)
		{
			const char	*codecPath = "Plugins/MeshCodecGranny" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CMeshCodecGranny();
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

#		if defined(USE_COMPILER_BACKEND_D3D)
		if (g_LoadedPlugins & EPlugin_CompilerBackendD3D11)
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendGPU_D3D();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendGPU_D3D();
		}
#		endif

#		if defined(USE_COMPILER_BACKEND_UNKNOWN2)
		if (g_LoadedPlugins & EPlugin_CompilerBackendPSSLC)
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendGPU_PSSLC();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendGPU_PSSLC();
		}
#		endif

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

#		if defined(USE_IMAGE_PLUGIN_HDR)
		if (g_LoadedPlugins & EPlugin_ImageCodecHDR)
		{
			IPluginModule	*codecHDR = GetPlugin_CImageHDRCodec();
			(codecHDR != null && CPluginManager::PluginRelease(codecHDR));
			ShutdownPlugin_CImageHDRCodec();
		}
#		endif

#		if defined(USE_IMAGE_PLUGIN_EXR)
		if (g_LoadedPlugins & EPlugin_ImageCodecEXR)
		{
			IPluginModule	*codecEXR = GetPlugin_CImageEXRCodec();
			(codecEXR != null && CPluginManager::PluginRelease(codecEXR));
			ShutdownPlugin_CImageEXRCodec();
		}
#		endif

#		if defined(USE_IMAGE_PLUGIN_TIFF)
		if (g_LoadedPlugins & EPlugin_ImageCodecTIFF)
		{
			IPluginModule	*codecTIFF = GetPlugin_CImageTIFFCodec();
			(codecTIFF != null && CPluginManager::PluginRelease(codecTIFF));
			ShutdownPlugin_CImageTIFFCodec();
		}
#		endif

#		if defined(USE_IMAGE_PLUGIN_PKM)
		if (g_LoadedPlugins & EPlugin_ImageCodecPKM)
		{
			IPluginModule	*codecPKM = GetPlugin_CImagePKMCodec();
			(codecPKM != null && CPluginManager::PluginRelease(codecPKM));
			ShutdownPlugin_CImagePKMCodec();
		}
#		endif

#		if defined(USE_IMAGE_PLUGIN_PVR)
		if (g_LoadedPlugins & EPlugin_ImageCodecPVR)
		{
			IPluginModule	*codecPVR = GetPlugin_CImagePVRCodec();
			(codecPVR != null && CPluginManager::PluginRelease(codecPVR));
			ShutdownPlugin_CImagePVRCodec();
		}
#		endif

#		if defined(USE_FBXIMPORTER)
		if (g_LoadedPlugins & EPlugin_MeshCodecFBX)
		{
			IPluginModule	*codec = GetPlugin_CMeshCodecFBX();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CMeshCodecFBX();
		}
#		endif

#		if defined(USE_GRANNYIMPORTER)
//		if (g_LoadedPlugins & EPlugin_MeshCodecGranny)
		{
			IPluginModule	*codec = GetPlugin_CMeshCodecGranny();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CMeshCodecGranny();
		}
#		endif
#	endif

		g_LoadedPlugins = 0;
	}

}

//----------------------------------------------------------------------------

