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
#		if defined(USE_COMPILER_BACKEND_ISPC)
		PK_PLUGIN_DECLARE(CCompilerBackendCPU_ISPC);
#		endif
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

	bool	RegisterPlugins(u32 selected /*= u32(-1)*/)
	{
		PK_ASSERT(g_LoadedPlugins == 0);

		bool	success = true;
#if	!defined(PK_PLUGINS_STATIC)
		// plugins are .dll
		CPluginManager::RegisterDirectory("Plugins", false);

#else
		// plugins are linked statically
		if (selected & EPlugin_CompilerBackendVM)
		{
			const char		*backendPath = "Plugins/CBCPU_VM" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendCPU_VM();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}

#	if	defined(USE_COMPILER_BACKEND_ISPC)
		if (selected & EPlugin_CompilerBackendISPC)
		{
			const char		*backendPath = "Plugins/CBCPU_ISPC" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendCPU_ISPC();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}
#	endif	// defined(USE_COMPILER_BACKEND_ISPC)

#	if	defined(USE_COMPILER_BACKEND_D3D)
		if (selected & EPlugin_CompilerBackendD3D)
		{
			const char		*backendPath = "Plugins/CBGPU_D3D" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendGPU_D3D();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}
#	endif	// defined(USE_COMPILER_BACKEND_D3D)

#	if	defined(USE_COMPILER_BACKEND_UNKNOWN2)
		if (selected & EPlugin_CompilerBackendPSSLC)
		{
			const char		*backendPath = "Plugins/CBGPU_PSSLC" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendGPU_PSSLC();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}
#	endif	// defined(USE_COMPILER_BACKEND_UNKNOWN2)

		if (selected & EPlugin_ImageCodecDDS)
		{
			const char		*codecPath = "Plugins/image_codec_dds" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CImageDDSCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecPNG)
		{
			const char		*codecPath = "Plugins/image_codec_png" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CImagePNGCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecJPG)
		{
			const char		*codecPath = "Plugins/image_codec_jpeg" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CImageJPEGCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

		if (selected & EPlugin_ImageCodecTGA)
		{
			const char		*codecPath = "Plugins/image_codec_tga" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CImageTGACodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}

#	if	defined(USE_IMAGE_PLUGIN_HDR)
		if (selected & EPlugin_ImageCodecHDR)
		{
			const char		*codecPath = "Plugins/image_codec_hdr" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CImageHDRCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#	endif	// defined(USE_IMAGE_PLUGIN_HDR)

#	if	defined(USE_IMAGE_PLUGIN_EXR)
		if (selected & EPlugin_ImageCodecEXR)
		{
			const char		*codecPath = "Plugins/image_codec_exr" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CImageEXRCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#	endif	// defined(USE_IMAGE_PLUGIN_EXR)

#	if	defined(USE_IMAGE_PLUGIN_TIFF)
		if (selected & EPlugin_ImageCodecTIFF)
		{
			const char		*codecPath = "Plugins/image_codec_tiff" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CImageTIFFCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#	endif	// defined(USE_IMAGE_PLUGIN_TIFF)

#	if	defined(USE_IMAGE_PLUGIN_PKM)
		if (selected & EPlugin_ImageCodecPKM)
		{
			const char		*codecPath = "Plugins/image_codec_pkm" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CImagePKMCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#	endif	// defined(USE_IMAGE_PLUGIN_PKM)

#	if	defined(USE_IMAGE_PLUGIN_PVR)
		if (selected & EPlugin_ImageCodecPVR)
		{
			const char		*codecPath = "Plugins/image_codec_pvr" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CImagePVRCodec();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#	endif	// defined(USE_IMAGE_PLUGIN_PVR)

#	if	defined(USE_FBXIMPORTER)
		if (selected & EPlugin_MeshCodecFBX)
		{
			const char		*codecPath = "Plugins/MeshCodecFBX" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CMeshCodecFBX();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#	endif	// defined(USE_FBXIMPORTER)

#	if	defined(USE_GRANNYIMPORTER)
		if (selected & EPlugin_MeshCodecGranny)
		{
			const char		*codecPath = "Plugins/MeshCodecGranny" PK_PLUGIN_POSTFIX_BUILD PK_PLUGIN_POSTFIX_EXT;
			IPluginModule	*codec = StartupPlugin_CMeshCodecGranny();
			success &= (codec != null && CPluginManager::PluginRegister(codec, true, codecPath));
		}
#	endif	// defined(USE_GRANNYIMPORTER)
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
#if	defined(PK_PLUGINS_STATIC)
		if (g_LoadedPlugins & EPlugin_CompilerBackendVM)
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendCPU_VM();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendCPU_VM();
		}

#	if	defined(USE_COMPILER_BACKEND_ISPC)
		if (g_LoadedPlugins & EPlugin_CompilerBackendISPC)
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendCPU_ISPC();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendCPU_ISPC();
		}
#	endif	// defined(USE_COMPILER_BACKEND_ISPC)

#	if	defined(USE_COMPILER_BACKEND_D3D)
		if (g_LoadedPlugins & EPlugin_CompilerBackendD3D)
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendGPU_D3D();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendGPU_D3D();
		}
#	endif	// defined(USE_COMPILER_BACKEND_D3D)

#	if	defined(USE_COMPILER_BACKEND_UNKNOWN2)
		if (g_LoadedPlugins & EPlugin_CompilerBackendPSSLC)
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendGPU_PSSLC();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendGPU_PSSLC();
		}
#	endif	// defined(USE_COMPILER_BACKEND_UNKNOWN2)

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

#	if	defined(USE_IMAGE_PLUGIN_HDR)
		if (g_LoadedPlugins & EPlugin_ImageCodecHDR)
		{
			IPluginModule	*codec = GetPlugin_CImageHDRCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImageHDRCodec();
		}
#	endif	// defined(USE_IMAGE_PLUGIN_HDR)

#	if	defined(USE_IMAGE_PLUGIN_EXR)
		if (g_LoadedPlugins & EPlugin_ImageCodecEXR)
		{
			IPluginModule	*codec = GetPlugin_CImageEXRCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImageEXRCodec();
		}
#	endif	// defined(USE_IMAGE_PLUGIN_EXR)

#	if	defined(USE_IMAGE_PLUGIN_TIFF)
		if (g_LoadedPlugins & EPlugin_ImageCodecTIFF)
		{
			IPluginModule	*codec = GetPlugin_CImageTIFFCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImageTIFFCodec();
		}
#	endif	// defined(USE_IMAGE_PLUGIN_TIFF)

#	if	defined(USE_IMAGE_PLUGIN_PKM)
		if (g_LoadedPlugins & EPlugin_ImageCodecPKM)
		{
			IPluginModule	*codec = GetPlugin_CImagePKMCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImagePKMCodec();
		}
#	endif	// defined(USE_IMAGE_PLUGIN_PKM)

#	if	defined(USE_IMAGE_PLUGIN_PVR)
		if (g_LoadedPlugins & EPlugin_ImageCodecPVR)
		{
			IPluginModule	*codec = GetPlugin_CImagePVRCodec();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CImagePVRCodec();
		}
#	endif	// defined(USE_IMAGE_PLUGIN_PVR)

#	if	defined(USE_FBXIMPORTER)
		if (g_LoadedPlugins & EPlugin_MeshCodecFBX)
		{
			IPluginModule	*codec = GetPlugin_CMeshCodecFBX();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CMeshCodecFBX();
		}
#	endif	// defined(USE_FBXIMPORTER)

#	if	defined(USE_GRANNYIMPORTER)
		if (g_LoadedPlugins & EPlugin_MeshCodecGranny)
		{
			IPluginModule	*codec = GetPlugin_CMeshCodecGranny();
			(codec != null && CPluginManager::PluginRelease(codec));
			ShutdownPlugin_CMeshCodecGranny();
		}
#	endif	// defined(USE_GRANNYIMPORTER)
#endif

		g_LoadedPlugins = 0;
	}

}

//----------------------------------------------------------------------------

