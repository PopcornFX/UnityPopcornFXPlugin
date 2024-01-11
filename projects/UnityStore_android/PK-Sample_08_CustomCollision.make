# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_android
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

PCH = ../../SDK/Samples/PK-Samples/precompiled/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../SDK/Samples/PK-Samples/PK-Sample_08_CustomCollision -I../../SDK/Samples/PK-Samples/precompiled -I../../SDK/Samples/PK-Samples/PK-Sample_02_FullIntegration/Scene -I../../SDK/Samples/PK-Samples -I../../ExternalLibs/GL/include -I../../SDK/Samples/External/imgui
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

ifeq ($(config),debug_android)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../../release/builds/Android_Samples
TARGET = $(TARGETDIR)/PK-Sample_08_CustomCollision_d
OBJDIR = ../intermediate/UnityStore/GM/android/Debug/PK-Sample_08_CustomCollision
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_08_CustomCollision -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RHI_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RenderHelpers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PKM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PVR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_TGA_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Discretizers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_d.a -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RHI_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RenderHelpers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PKM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PVR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_TGA_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Discretizers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android -shared

else ifeq ($(config),debug_android64)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../../release/builds/Android64_Samples
TARGET = $(TARGETDIR)/PK-Sample_08_CustomCollision_d
OBJDIR = ../intermediate/UnityStore/GM/android64/Debug/PK-Sample_08_CustomCollision
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_08_CustomCollision -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RHI_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RenderHelpers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PKM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PVR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_TGA_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Discretizers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_d.a -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RHI_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RenderHelpers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PKM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PVR_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_TGA_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Discretizers_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64 -shared

else ifeq ($(config),release_android)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../../release/builds/Android_Samples
TARGET = $(TARGETDIR)/PK-Sample_08_CustomCollision_r
OBJDIR = ../intermediate/UnityStore/GM/android/Release/PK-Sample_08_CustomCollision
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_08_CustomCollision -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RHI_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RenderHelpers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PKM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PVR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_TGA_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Discretizers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_r.a -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RHI_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RenderHelpers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PKM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PVR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_TGA_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Discretizers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android -shared

else ifeq ($(config),release_android64)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../../release/builds/Android64_Samples
TARGET = $(TARGETDIR)/PK-Sample_08_CustomCollision_r
OBJDIR = ../intermediate/UnityStore/GM/android64/Release/PK-Sample_08_CustomCollision
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_08_CustomCollision -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RHI_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RenderHelpers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PKM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PVR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_TGA_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Discretizers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_r.a -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RHI_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RenderHelpers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PKM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PVR_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_TGA_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Discretizers_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64 -shared

else ifeq ($(config),retail_android)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../../release/builds/Android_Samples
TARGET = $(TARGETDIR)/PK-Sample_08_CustomCollision_s
OBJDIR = ../intermediate/UnityStore/GM/android/Retail/PK-Sample_08_CustomCollision
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_08_CustomCollision -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RHI_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RenderHelpers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PKM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PVR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_TGA_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Discretizers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_s.a -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RHI_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-RenderHelpers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PKM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PVR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_TGA_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Discretizers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android -s -shared

else ifeq ($(config),retail_android64)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../../release/builds/Android64_Samples
TARGET = $(TARGETDIR)/PK-Sample_08_CustomCollision_s
OBJDIR = ../intermediate/UnityStore/GM/android64/Retail/PK-Sample_08_CustomCollision
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_08_CustomCollision -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RHI_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RenderHelpers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PKM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PVR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_TGA_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Discretizers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_s.a -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RHI_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-RenderHelpers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PKM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PVR_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_TGA_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Discretizers_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64 -s -shared

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/CustomCollision.o
GENERATED += $(OBJDIR)/CustomCollision_Scene.o
GENERATED += $(OBJDIR)/CustomCollision_SimInterface_Decl.o
GENERATED += $(OBJDIR)/CustomCollision_SimInterface_Impl.o
GENERATED += $(OBJDIR)/GPUContext_D3D.o
GENERATED += $(OBJDIR)/Main.o
GENERATED += $(OBJDIR)/SampleScene.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/CustomCollision.o
OBJECTS += $(OBJDIR)/CustomCollision_Scene.o
OBJECTS += $(OBJDIR)/CustomCollision_SimInterface_Decl.o
OBJECTS += $(OBJDIR)/CustomCollision_SimInterface_Impl.o
OBJECTS += $(OBJDIR)/GPUContext_D3D.o
OBJECTS += $(OBJDIR)/Main.o
OBJECTS += $(OBJDIR)/SampleScene.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Sample_08_CustomCollision
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
	@echo Cleaning PK-Sample_08_CustomCollision
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
$(OBJDIR)/CustomCollision.o: ../../SDK/Samples/PK-Samples/PK-Sample_08_CustomCollision/CustomCollision.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/CustomCollision_Scene.o: ../../SDK/Samples/PK-Samples/PK-Sample_08_CustomCollision/CustomCollision_Scene.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/CustomCollision_SimInterface_Decl.o: ../../SDK/Samples/PK-Samples/PK-Sample_08_CustomCollision/CustomCollision_SimInterface_Decl.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/CustomCollision_SimInterface_Impl.o: ../../SDK/Samples/PK-Samples/PK-Sample_08_CustomCollision/CustomCollision_SimInterface_Impl.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Main.o: ../../SDK/Samples/PK-Samples/PK-Sample_08_CustomCollision/Main.cpp
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
