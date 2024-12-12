# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_android
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq ($(shell echo "test"), "test")
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

PCH = ../../SDK/Samples/PK-Samples/precompiled/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../SDK/Samples/PK-Samples/PK-Sample_11_ThreadPool -I../../SDK/Samples/PK-Samples/precompiled -I../../SDK/Samples/PK-Samples/PK-Sample_02_FullIntegration/Scene -I../../SDK/Samples/PK-Samples -I../../ExternalLibs/GL/include -I"$(VULKAN_SDK)/include" -I../../SDK/Samples/External/imgui
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
TARGET = $(TARGETDIR)/PK-Sample_11_ThreadPool_d
OBJDIR = ../intermediate/UnityStore/GM/android/Debug/PK-Sample_11_ThreadPool
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_11_ThreadPool -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_d.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-Plugin_CodecImage_PKIM_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Plugin_CodecImage_HDR_d -lPK-Discretizers_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lPK-ZLib_d -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_d.a
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
TARGET = $(TARGETDIR)/PK-Sample_11_ThreadPool_d
OBJDIR = ../intermediate/UnityStore/GM/android64/Debug/PK-Sample_11_ThreadPool
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_11_ThreadPool -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_d.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-Plugin_CodecImage_PKIM_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Plugin_CodecImage_HDR_d -lPK-Discretizers_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lPK-ZLib_d -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_d.a
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
TARGET = $(TARGETDIR)/PK-Sample_11_ThreadPool_r
OBJDIR = ../intermediate/UnityStore/GM/android/Release/PK-Sample_11_ThreadPool
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_11_ThreadPool -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_r.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-Plugin_CodecImage_PKIM_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Plugin_CodecImage_HDR_r -lPK-Discretizers_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lPK-ZLib_r -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_r.a
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
TARGET = $(TARGETDIR)/PK-Sample_11_ThreadPool_r
OBJDIR = ../intermediate/UnityStore/GM/android64/Release/PK-Sample_11_ThreadPool
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_11_ThreadPool -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_r.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-Plugin_CodecImage_PKIM_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Plugin_CodecImage_HDR_r -lPK-Discretizers_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lPK-ZLib_r -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_r.a
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
TARGET = $(TARGETDIR)/PK-Sample_11_ThreadPool_s
OBJDIR = ../intermediate/UnityStore/GM/android/Retail/PK-Sample_11_ThreadPool
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_11_ThreadPool -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_s.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-Plugin_CodecImage_PKIM_s -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CodecImage_PKM_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_PVR_s -lPK-Plugin_CodecImage_TGA_s -lPK-Plugin_CodecImage_HDR_s -lPK-Discretizers_s -lPK-ParticlesToolbox_s -lPK-Runtime_s -lPK-ZLib_s -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-SampleLib_s.a
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
TARGET = $(TARGETDIR)/PK-Sample_11_ThreadPool_s
OBJDIR = ../intermediate/UnityStore/GM/android64/Retail/PK-Sample_11_ThreadPool
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_11_ThreadPool -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_s.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-Plugin_CodecImage_PKIM_s -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CodecImage_PKM_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_PVR_s -lPK-Plugin_CodecImage_TGA_s -lPK-Plugin_CodecImage_HDR_s -lPK-Discretizers_s -lPK-ParticlesToolbox_s -lPK-Runtime_s -lPK-ZLib_s -llog
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-SampleLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64 -s -shared

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################

PERFILE_FLAGS_0 = $(ALL_CXXFLAGS) -fvisibility-inlines-hidden

# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/GPUContext_D3D.o
GENERATED += $(OBJDIR)/Main.o
GENERATED += $(OBJDIR)/SampleScene.o
GENERATED += $(OBJDIR)/ThreadPool.o
GENERATED += $(OBJDIR)/ThreadPool_Default.o
GENERATED += $(OBJDIR)/ThreadPool_Mono.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/GPUContext_D3D.o
OBJECTS += $(OBJDIR)/Main.o
OBJECTS += $(OBJDIR)/SampleScene.o
OBJECTS += $(OBJDIR)/ThreadPool.o
OBJECTS += $(OBJDIR)/ThreadPool_Default.o
OBJECTS += $(OBJDIR)/ThreadPool_Mono.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Sample_11_ThreadPool
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
	@echo Cleaning PK-Sample_11_ThreadPool
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
	$(SILENT) $(CXX) -x c++-header $(PERFILE_FLAGS_0) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
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
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Main.o: ../../SDK/Samples/PK-Samples/PK-Sample_11_ThreadPool/Main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ThreadPool.o: ../../SDK/Samples/PK-Samples/PK-Sample_11_ThreadPool/ThreadPool.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ThreadPool_Default.o: ../../SDK/Samples/PK-Samples/PK-Sample_11_ThreadPool/ThreadPool_Default.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ThreadPool_Mono.o: ../../SDK/Samples/PK-Samples/PK-Sample_11_ThreadPool/ThreadPool_Mono.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GPUContext_D3D.o: ../../SDK/Samples/PK-Samples/PK-Sample_02_FullIntegration/GPUContext_D3D.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SampleScene.o: ../../SDK/Samples/PK-Samples/PK-Sample_02_FullIntegration/Scene/SampleScene.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
