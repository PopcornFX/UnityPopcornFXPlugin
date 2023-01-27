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

PCH = ../../SDK/Samples/ParticleSample/src/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../SDK/Samples/ParticleSample/src -I../../SDK/Samples/ParticleSample/src/include -I../../Native/Common/PKFX
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LDDEPS +=
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
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_d
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Debug/PK-Sample_03_EngineHooks
DEFINES += -D_DEBUG -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0
LIBS += -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-ZLib_d -lPK-ParticlesToolbox_d -lPK-Runtime_d
ALL_LDFLAGS += $(LDFLAGS) -L../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

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
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_d
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Debug/PK-Sample_03_EngineHooks
DEFINES += -D_DEBUG -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0
LIBS += -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-ZLib_d -lPK-ParticlesToolbox_d -lPK-Runtime_d
ALL_LDFLAGS += $(LDFLAGS) -L../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64 -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

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
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_r
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Release/PK-Sample_03_EngineHooks
DEFINES += -DNDEBUG -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
LIBS += -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-ZLib_r -lPK-ParticlesToolbox_r -lPK-Runtime_r
ALL_LDFLAGS += $(LDFLAGS) -L../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

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
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_r
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Release/PK-Sample_03_EngineHooks
DEFINES += -DNDEBUG -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
LIBS += -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-ZLib_r -lPK-ParticlesToolbox_r -lPK-Runtime_r
ALL_LDFLAGS += $(LDFLAGS) -L../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64 -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

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
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_s
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Retail/PK-Sample_03_EngineHooks
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
LIBS += -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-ZLib_s -lPK-ParticlesToolbox_s -lPK-Runtime_s
ALL_LDFLAGS += $(LDFLAGS) -L../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios -Wl,-x -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

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
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_s
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Retail/PK-Sample_03_EngineHooks
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0
LIBS += -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-ZLib_s -lPK-ParticlesToolbox_s -lPK-Runtime_s
ALL_LDFLAGS += $(LDFLAGS) -L../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64 -Wl,-x -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0

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

GENERATED += $(OBJDIR)/FxPlatformGeneric.o
GENERATED += $(OBJDIR)/ParticleSample_03_EngineHooks.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/FxPlatformGeneric.o
OBJECTS += $(OBJDIR)/ParticleSample_03_EngineHooks.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Sample_03_EngineHooks
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
	@echo Cleaning PK-Sample_03_EngineHooks
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) rmdir /s /q $(subst /,\\,$(GENERATED))
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


# File Rules
# #############################################

$(OBJDIR)/FxPlatformGeneric.o: ../../Native/Common/PKFX/FxPlatformGeneric.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ParticleSample_03_EngineHooks.o: ../../SDK/Samples/ParticleSample/src/ParticleSample_03_EngineHooks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/precompiled.o: ../../SDK/Samples/ParticleSample/src/precompiled.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
