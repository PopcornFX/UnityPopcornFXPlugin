# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x64
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

ifeq ($(origin CC), default)
  CC = clang
endif
ifeq ($(origin CXX), default)
  CXX = clang++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
PCH = ../../SDK/Samples/PK-Samples/precompiled/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../SDK/Samples/PK-Samples/PK-Sample_01_BasicRendering -I../../SDK/Samples/PK-Samples/precompiled -I../../SDK/Samples/PK-Samples -I../../Native/Common -I../../Native/Common/PKFX -I../../ExternalLibs/GL/include
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
TARGET = $(TARGETDIR)/PK-Sample_01_BasicRendering_macosx_d
OBJDIR = ../intermediate/UnityStore/GM/x64/Debug/PK-Sample_01_BasicRendering
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_01_BasicRendering -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_d.a -framework quartzcore -framework cocoa -framework metal -framework iokit -lPK-RHI -lPK-RenderHelpers -lPK-MCPP -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-Plugin_CodecImage_PKIM_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Plugin_CodecImage_HDR_d -lPK-Discretizers_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lPK-ZLib_d -lpthread -lm -ldl -lglfw3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/GL/lib/macosx -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64 -m64 -target x86_64-apple-macos10.14 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-Sample_01_BasicRendering_macosx_d
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Debug/PK-Sample_01_BasicRendering
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_01_BasicRendering -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_d.a -framework quartzcore -framework cocoa -framework metal -framework iokit -lPK-RHI -lPK-RenderHelpers -lPK-MCPP -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-Plugin_CodecImage_PKIM_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Plugin_CodecImage_HDR_d -lPK-Discretizers_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lPK-ZLib_d -lpthread -lm -ldl -lglfw3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/GL/lib/macosx -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64 -target arm64-apple-macos11.0 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),release_x64)
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-Sample_01_BasicRendering_macosx_r
OBJDIR = ../intermediate/UnityStore/GM/x64/Release/PK-Sample_01_BasicRendering
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_01_BasicRendering -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_r.a -framework quartzcore -framework cocoa -framework metal -framework iokit -lPK-RHI -lPK-RenderHelpers -lPK-MCPP -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-Plugin_CodecImage_PKIM_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Plugin_CodecImage_HDR_r -lPK-Discretizers_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lPK-ZLib_r -lpthread -lm -ldl -lglfw3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/GL/lib/macosx -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64 -m64 -target x86_64-apple-macos10.14 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),release_arm64)
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-Sample_01_BasicRendering_macosx_r
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Release/PK-Sample_01_BasicRendering
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_01_BasicRendering -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_r.a -framework quartzcore -framework cocoa -framework metal -framework iokit -lPK-RHI -lPK-RenderHelpers -lPK-MCPP -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-Plugin_CodecImage_PKIM_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Plugin_CodecImage_HDR_r -lPK-Discretizers_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lPK-ZLib_r -lpthread -lm -ldl -lglfw3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/GL/lib/macosx -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64 -target arm64-apple-macos11.0 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),retail_x64)
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-Sample_01_BasicRendering_macosx_s
OBJDIR = ../intermediate/UnityStore/GM/x64/Retail/PK-Sample_01_BasicRendering
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_01_BasicRendering -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_s.a -framework quartzcore -framework cocoa -framework metal -framework iokit -lPK-RHI -lPK-RenderHelpers -lPK-MCPP -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-Plugin_CodecImage_PKIM_s -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CodecImage_PKM_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_PVR_s -lPK-Plugin_CodecImage_TGA_s -lPK-Plugin_CodecImage_HDR_s -lPK-Discretizers_s -lPK-ParticlesToolbox_s -lPK-Runtime_s -lPK-ZLib_s -lpthread -lm -ldl -lglfw3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-SampleLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/GL/lib/macosx -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64 -m64 -target x86_64-apple-macos10.14 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

else ifeq ($(config),retail_arm64)
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-Sample_01_BasicRendering_macosx_s
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Retail/PK-Sample_01_BasicRendering
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_01_BasicRendering -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_s.a -framework quartzcore -framework cocoa -framework metal -framework iokit -lPK-RHI -lPK-RenderHelpers -lPK-MCPP -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-Plugin_CodecImage_PKIM_s -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CodecImage_PKM_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_PVR_s -lPK-Plugin_CodecImage_TGA_s -lPK-Plugin_CodecImage_HDR_s -lPK-Discretizers_s -lPK-ParticlesToolbox_s -lPK-Runtime_s -lPK-ZLib_s -lpthread -lm -ldl -lglfw3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-SampleLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/GL/lib/macosx -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64 -target arm64-apple-macos11.0 -framework AppKit -framework OpenGL -framework CoreFoundation -framework CoreServices

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/BasicBatchDrawer_Billboard.o
GENERATED += $(OBJDIR)/BasicBatchDrawer_Mesh.o
GENERATED += $(OBJDIR)/BasicBatchDrawer_Ribbon.o
GENERATED += $(OBJDIR)/BasicRendererCache.o
GENERATED += $(OBJDIR)/PK-Sample_01_BasicRendering.o
GENERATED += $(OBJDIR)/ParticleSampleGLglfw3.o
GENERATED += $(OBJDIR)/gl_utils.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/BasicBatchDrawer_Billboard.o
OBJECTS += $(OBJDIR)/BasicBatchDrawer_Mesh.o
OBJECTS += $(OBJDIR)/BasicBatchDrawer_Ribbon.o
OBJECTS += $(OBJDIR)/BasicRendererCache.o
OBJECTS += $(OBJDIR)/PK-Sample_01_BasicRendering.o
OBJECTS += $(OBJDIR)/ParticleSampleGLglfw3.o
OBJECTS += $(OBJDIR)/gl_utils.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET) $(dir $(TARGETDIR))PkgInfo $(dir $(TARGETDIR))Info.plist
	@:

$(dir $(TARGETDIR))PkgInfo:
$(dir $(TARGETDIR))Info.plist:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Sample_01_BasicRendering
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
	@echo Cleaning PK-Sample_01_BasicRendering
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
$(OBJDIR)/BasicBatchDrawer_Billboard.o: ../../SDK/Samples/PK-Samples/PK-Sample_01_BasicRendering/BasicBatchDrawer_Billboard.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/BasicBatchDrawer_Mesh.o: ../../SDK/Samples/PK-Samples/PK-Sample_01_BasicRendering/BasicBatchDrawer_Mesh.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/BasicBatchDrawer_Ribbon.o: ../../SDK/Samples/PK-Samples/PK-Sample_01_BasicRendering/BasicBatchDrawer_Ribbon.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/BasicRendererCache.o: ../../SDK/Samples/PK-Samples/PK-Sample_01_BasicRendering/BasicRendererCache.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PK-Sample_01_BasicRendering.o: ../../SDK/Samples/PK-Samples/PK-Sample_01_BasicRendering/PK-Sample_01_BasicRendering.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ParticleSampleGLglfw3.o: ../../SDK/Samples/ParticleSampleGL/ParticleSampleGLglfw3.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/gl_utils.o: ../../Native/Common/Gl/gl_utils.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
