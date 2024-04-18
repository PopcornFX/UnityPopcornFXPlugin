# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x32
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
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../SDK/Samples/PK-Samples -I../../SDK/Samples/PK-Samples/precompiled -I../../SDK/Samples/PK-Samples/PK-ShaderTool -I../../ExternalLibs/GL/include -I"$(VULKAN_SDK)/include"
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x32)
RESCOMP = windres
TARGETDIR = ../../../release/builds/x32_Samples
TARGET = $(TARGETDIR)/PK-ShaderTool_linux_d
OBJDIR = ../intermediate/UnityStore/GM/x32/Debug/PK-ShaderTool
DEFINES += -D_DEBUG -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_SDL=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-SampleLib_d.a -lPK-MCPP -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Plugin_CodecImage_HDR_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-RHI_d -lPK-RenderHelpers_d -lPK-Discretizers_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lPK-ZLib_d -ldl -lpthread -lbfd -lGL -lGLX -lX11 -lvulkan
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-SampleLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(VULKAN_SDK)/lib32" -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32 -L/usr/lib32 -m32

else ifeq ($(config),debug_x64)
RESCOMP = windres
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-ShaderTool_linux_d
OBJDIR = ../intermediate/UnityStore/GM/x64/Debug/PK-ShaderTool
DEFINES += -D_DEBUG -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_SDL=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-SampleLib_d.a -lPK-MCPP -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Plugin_CodecImage_HDR_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-RHI_d -lPK-RenderHelpers_d -lPK-Discretizers_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lPK-ZLib_d -ldl -lpthread -lbfd -lGL -lGLX -lX11 -lvulkan
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-SampleLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(VULKAN_SDK)/lib" -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64 -L/usr/lib64 -m64

else ifeq ($(config),debug_arm64)
ifeq ($(origin CC), default)
  CC = clang
endif
ifeq ($(origin CXX), default)
  CXX = clang++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-ShaderTool_linux_d
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Debug/PK-ShaderTool
DEFINES += -D_DEBUG -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_SDL=0
FORCE_INCLUDE +=
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -target aarch64-linux-gnu
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -target aarch64-linux-gnu
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-SampleLib_d.a -lPK-MCPP -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Plugin_CodecImage_HDR_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-RHI_d -lPK-RenderHelpers_d -lPK-Discretizers_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lPK-ZLib_d -ldl -lpthread -lbfd -lGL -lGLX -lX11 -lvulkan
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-SampleLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64 -fuse-ld=lld -target aarch64-linux-gnu

else ifeq ($(config),release_x32)
RESCOMP = windres
TARGETDIR = ../../../release/builds/x32_Samples
TARGET = $(TARGETDIR)/PK-ShaderTool_linux_r
OBJDIR = ../intermediate/UnityStore/GM/x32/Release/PK-ShaderTool
DEFINES += -DNDEBUG -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_SDL=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-SampleLib_r.a -lPK-MCPP -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Plugin_CodecImage_HDR_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-RHI_r -lPK-RenderHelpers_r -lPK-Discretizers_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lPK-ZLib_r -ldl -lpthread -lbfd -lGL -lGLX -lX11 -lvulkan
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-SampleLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(VULKAN_SDK)/lib32" -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32 -L/usr/lib32 -m32

else ifeq ($(config),release_x64)
RESCOMP = windres
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-ShaderTool_linux_r
OBJDIR = ../intermediate/UnityStore/GM/x64/Release/PK-ShaderTool
DEFINES += -DNDEBUG -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_SDL=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-SampleLib_r.a -lPK-MCPP -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Plugin_CodecImage_HDR_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-RHI_r -lPK-RenderHelpers_r -lPK-Discretizers_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lPK-ZLib_r -ldl -lpthread -lbfd -lGL -lGLX -lX11 -lvulkan
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-SampleLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(VULKAN_SDK)/lib" -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64 -L/usr/lib64 -m64

else ifeq ($(config),release_arm64)
ifeq ($(origin CC), default)
  CC = clang
endif
ifeq ($(origin CXX), default)
  CXX = clang++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-ShaderTool_linux_r
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Release/PK-ShaderTool
DEFINES += -DNDEBUG -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_SDL=0
FORCE_INCLUDE +=
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -target aarch64-linux-gnu
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -target aarch64-linux-gnu
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-SampleLib_r.a -lPK-MCPP -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Plugin_CodecImage_HDR_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-RHI_r -lPK-RenderHelpers_r -lPK-Discretizers_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lPK-ZLib_r -ldl -lpthread -lbfd -lGL -lGLX -lX11 -lvulkan
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-SampleLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64 -fuse-ld=lld -target aarch64-linux-gnu

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/Main.o
GENERATED += $(OBJDIR)/ShaderTool.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/Main.o
OBJECTS += $(OBJDIR)/ShaderTool.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-ShaderTool
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
	@echo Cleaning PK-ShaderTool
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
$(OBJDIR)/Main.o: ../../SDK/Samples/PK-Samples/PK-ShaderTool/Main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ShaderTool.o: ../../SDK/Samples/PK-Samples/PK-ShaderTool/ShaderTool.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
