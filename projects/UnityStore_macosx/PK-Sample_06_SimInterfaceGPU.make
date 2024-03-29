# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

ifeq ($(origin CC), default)
  CC = clang
endif
ifeq ($(origin CXX), default)
  CXX = clang++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
PCH = ../../SDK/Samples/PK-Samples/precompiled/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../SDK/Samples/PK-Samples/PK-Sample_06_SimInterfaceGPU -I../../SDK/Samples/PK-Samples/precompiled -I../../SDK/Samples/PK-Samples/PK-Sample_02_FullIntegration/Scene -I../../SDK/Samples/PK-Samples -I../../ExternalLibs/GL/include -I../../SDK/Samples/External/imgui -I/usr/local/include/SDL2
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-Sample_06_SimInterfaceGPU_macosx_d
OBJDIR = ../intermediate/UnityStore/GM/x64/Debug/PK-Sample_06_SimInterfaceGPU
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_06_SimInterfaceGPU -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=1 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RHI_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RenderHelpers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-MCPP_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PKM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PVR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_TGA_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_HDR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Discretizers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ZLib_d.a -framework quartzcore -framework cocoa -framework metal -lpthread -lm -ldl -lSDL2
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RHI_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RenderHelpers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-MCPP_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PKM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PVR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_TGA_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_HDR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Discretizers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ZLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64 -m64 -target x86_64-apple-macos10.14 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-Sample_06_SimInterfaceGPU_macosx_d
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Debug/PK-Sample_06_SimInterfaceGPU
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_06_SimInterfaceGPU -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=1 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RHI_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RenderHelpers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-MCPP_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PKM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PVR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_TGA_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_HDR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Discretizers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ZLib_d.a -framework quartzcore -framework cocoa -framework metal -lpthread -lm -ldl -lSDL2
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RHI_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RenderHelpers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-MCPP_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PKM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PVR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_TGA_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_HDR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Discretizers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ZLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64 -target arm64-apple-macos11.0 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),release_x64)
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-Sample_06_SimInterfaceGPU_macosx_r
OBJDIR = ../intermediate/UnityStore/GM/x64/Release/PK-Sample_06_SimInterfaceGPU
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_06_SimInterfaceGPU -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=1 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RHI_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RenderHelpers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-MCPP_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PKM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PVR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_TGA_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_HDR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Discretizers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ZLib_r.a -framework quartzcore -framework cocoa -framework metal -lpthread -lm -ldl -lSDL2
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RHI_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RenderHelpers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-MCPP_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PKM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PVR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_TGA_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_HDR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Discretizers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ZLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64 -m64 -target x86_64-apple-macos10.14 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),release_arm64)
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-Sample_06_SimInterfaceGPU_macosx_r
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Release/PK-Sample_06_SimInterfaceGPU
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_06_SimInterfaceGPU -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=1 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RHI_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RenderHelpers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-MCPP_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PKM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PVR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_TGA_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_HDR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Discretizers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ZLib_r.a -framework quartzcore -framework cocoa -framework metal -lpthread -lm -ldl -lSDL2
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RHI_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RenderHelpers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-MCPP_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PKM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PVR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_TGA_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_HDR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Discretizers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ZLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64 -target arm64-apple-macos11.0 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),retail_x64)
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-Sample_06_SimInterfaceGPU_macosx_s
OBJDIR = ../intermediate/UnityStore/GM/x64/Retail/PK-Sample_06_SimInterfaceGPU
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_06_SimInterfaceGPU -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=1 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RHI_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RenderHelpers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-MCPP_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PKM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PVR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_TGA_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_HDR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Discretizers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ZLib_s.a -framework quartzcore -framework cocoa -framework metal -lpthread -lm -ldl -lSDL2
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RHI_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-RenderHelpers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-MCPP_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PKM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_PVR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_TGA_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Plugin_CodecImage_HDR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Discretizers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ZLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64 -m64 -target x86_64-apple-macos10.14 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),retail_arm64)
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-Sample_06_SimInterfaceGPU_macosx_s
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Retail/PK-Sample_06_SimInterfaceGPU
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_06_SimInterfaceGPU -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=1 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RHI_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RenderHelpers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-MCPP_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PKM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PVR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_TGA_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_HDR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Discretizers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ZLib_s.a -framework quartzcore -framework cocoa -framework metal -lpthread -lm -ldl -lSDL2
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RHI_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-RenderHelpers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-MCPP_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PKM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_PVR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_TGA_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Plugin_CodecImage_HDR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Discretizers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ZLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64 -target arm64-apple-macos11.0 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/GPUContext_D3D.o
GENERATED += $(OBJDIR)/Main.o
GENERATED += $(OBJDIR)/SampleScene.o
GENERATED += $(OBJDIR)/SimInterface.o
GENERATED += $(OBJDIR)/SimInterfaceImplem_GPU.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/GPUContext_D3D.o
OBJECTS += $(OBJDIR)/Main.o
OBJECTS += $(OBJDIR)/SampleScene.o
OBJECTS += $(OBJDIR)/SimInterface.o
OBJECTS += $(OBJDIR)/SimInterfaceImplem_GPU.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET) $(dir $(TARGETDIR))PkgInfo $(dir $(TARGETDIR))Info.plist
	@:

$(dir $(TARGETDIR))PkgInfo:
$(dir $(TARGETDIR))Info.plist:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Sample_06_SimInterfaceGPU
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning PK-Sample_06_SimInterfaceGPU
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/precompiled.o: ../../SDK/Samples/PK-Samples/precompiled/precompiled.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Main.o: ../../SDK/Samples/PK-Samples/PK-Sample_06_SimInterfaceGPU/Main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SimInterface.o: ../../SDK/Samples/PK-Samples/PK-Sample_06_SimInterfaceGPU/SimInterface.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SimInterfaceImplem_GPU.o: ../../SDK/Samples/PK-Samples/PK-Sample_06_SimInterfaceGPU/SimInterfaceImplem_GPU.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GPUContext_D3D.o: ../../SDK/Samples/PK-Samples/PK-Sample_02_FullIntegration/GPUContext_D3D.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SampleScene.o: ../../SDK/Samples/PK-Samples/PK-Sample_02_FullIntegration/Scene/SampleScene.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
