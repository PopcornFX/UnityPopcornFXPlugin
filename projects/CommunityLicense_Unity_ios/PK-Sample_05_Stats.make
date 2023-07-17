# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_ios
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
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../SDK/Samples/PK-Samples/PK-Sample_05_Stats -I../../SDK/Samples/PK-Samples/precompiled -I../../SDK/Samples/PK-Samples/PK-Sample_02_FullIntegration/Scene -I../../SDK/Samples/PK-Samples -I../../SDK/Samples/External/imgui
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

ifeq ($(config),debug_ios)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch armv7
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch armv7
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../../release/builds/iOS_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Stats_d.app
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Debug/PK-Sample_05_Stats
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_05_Stats -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0
LIBS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios/libPK-SampleLib_d.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Discretizers_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lPK-ZLib_d
LDDEPS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios/libPK-SampleLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

else ifeq ($(config),debug_ios64)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch arm64
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch arm64
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../../release/builds/iOS64_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Stats_d
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Debug/PK-Sample_05_Stats
DEFINES += -D_DEBUG -DPKSAMPLE_PROJNAME=PK-Sample_05_Stats -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0
LIBS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64/libPK-SampleLib_d.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Discretizers_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lPK-ZLib_d
LDDEPS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64/libPK-SampleLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64 -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

else ifeq ($(config),release_ios)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch armv7
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch armv7
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../../release/builds/iOS_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Stats_r.app
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Release/PK-Sample_05_Stats
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_05_Stats -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
LIBS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios/libPK-SampleLib_r.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Discretizers_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lPK-ZLib_r
LDDEPS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios/libPK-SampleLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

else ifeq ($(config),release_ios64)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch arm64
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch arm64
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../../release/builds/iOS64_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Stats_r
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Release/PK-Sample_05_Stats
DEFINES += -DNDEBUG -DPKSAMPLE_PROJNAME=PK-Sample_05_Stats -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
LIBS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64/libPK-SampleLib_r.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Discretizers_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lPK-ZLib_r
LDDEPS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64/libPK-SampleLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64 -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

else ifeq ($(config),retail_ios)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch armv7
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch armv7
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../../release/builds/iOS_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Stats_s.app
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Retail/PK-Sample_05_Stats
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_05_Stats -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
LIBS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios/libPK-SampleLib_s.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CodecImage_PKM_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_PVR_s -lPK-Plugin_CodecImage_TGA_s -lPK-Discretizers_s -lPK-ParticlesToolbox_s -lPK-Runtime_s -lPK-ZLib_s
LDDEPS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios/libPK-SampleLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios -Wl,-x -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

else ifeq ($(config),retail_ios64)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch arm64
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch arm64
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../../release/builds/iOS64_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Stats_s
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Retail/PK-Sample_05_Stats
DEFINES += -DNDEBUG -DPK_RETAIL -DPKSAMPLE_PROJNAME=PK-Sample_05_Stats -DPK_BUILD_WITH_FMODEX_SUPPORT=0 -DPK_BUILD_WITH_SDL=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
LIBS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64/libPK-SampleLib_s.a -lPK-RHI -lPK-RenderHelpers -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CodecImage_PKM_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_PVR_s -lPK-Plugin_CodecImage_TGA_s -lPK-Discretizers_s -lPK-ParticlesToolbox_s -lPK-Runtime_s -lPK-ZLib_s
LDDEPS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64/libPK-SampleLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64 -Wl,-x -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################

PERFILE_FLAGS_0 = $(ALL_CXXFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path`

# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/GPUContext_D3D.o
GENERATED += $(OBJDIR)/Main.o
GENERATED += $(OBJDIR)/SampleScene.o
GENERATED += $(OBJDIR)/Stats.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/GPUContext_D3D.o
OBJECTS += $(OBJDIR)/Main.o
OBJECTS += $(OBJDIR)/SampleScene.o
OBJECTS += $(OBJDIR)/Stats.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Sample_05_Stats
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
	@echo Cleaning PK-Sample_05_Stats
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
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) -x c++-header -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


ifeq ($(config),debug_ios)
all: $(TARGET)
	@:


else ifeq ($(config),debug_ios64)
all: $(TARGET) $(dir $(TARGETDIR))PkgInfo $(dir $(TARGETDIR))Info.plist
	@:

$(dir $(TARGETDIR))PkgInfo:
$(dir $(TARGETDIR))Info.plist:


else ifeq ($(config),release_ios)
all: $(TARGET)
	@:


else ifeq ($(config),release_ios64)
all: $(TARGET) $(dir $(TARGETDIR))PkgInfo $(dir $(TARGETDIR))Info.plist
	@:

$(dir $(TARGETDIR))PkgInfo:
$(dir $(TARGETDIR))Info.plist:


else ifeq ($(config),retail_ios)
all: $(TARGET)
	@:


else ifeq ($(config),retail_ios64)
all: $(TARGET) $(dir $(TARGETDIR))PkgInfo $(dir $(TARGETDIR))Info.plist
	@:

$(dir $(TARGETDIR))PkgInfo:
$(dir $(TARGETDIR))Info.plist:


#else
#  $(error "invalid configuration $(config)")
endif

# File Rules
# #############################################

$(OBJDIR)/precompiled.o: ../../SDK/Samples/PK-Samples/precompiled/precompiled.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Main.o: ../../SDK/Samples/PK-Samples/PK-Sample_05_Stats/Main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Stats.o: ../../SDK/Samples/PK-Samples/PK-Sample_05_Stats/Stats.cpp
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
