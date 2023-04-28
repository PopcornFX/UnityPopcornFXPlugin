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

PCH = ../../../ExternalLibs/Runtime/pk_render_helpers/src/precompiled/rh_precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../../ExternalLibs/Runtime/pk_render_helpers/src -I../../../ExternalLibs/Runtime/pk_render_helpers/src/precompiled
FORCE_INCLUDE += -include pk_compiler_warnings.h
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS +=
LDDEPS +=
ALL_LDFLAGS += $(LDFLAGS) -Wl,-x -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wl,-syslibroot `xcrun --sdk iphoneos --show-sdk-path` -miphoneos-version-min=8.0
define LINKCMD
  $(SILENT) $(RM) -f $@
  $(SILENT) $(AR) -rcs $@ $(OBJECTS)
endef
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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios
TARGET = $(TARGETDIR)/libPK-RenderHelpers_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Debug/PK-RenderHelpers_SDK1
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64
TARGET = $(TARGETDIR)/libPK-RenderHelpers_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Debug/PK-RenderHelpers_SDK1
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios
TARGET = $(TARGETDIR)/libPK-RenderHelpers_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Release/PK-RenderHelpers_SDK1
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64
TARGET = $(TARGETDIR)/libPK-RenderHelpers_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Release/PK-RenderHelpers_SDK1
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios
TARGET = $(TARGETDIR)/libPK-RenderHelpers_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Retail/PK-RenderHelpers_SDK1
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64
TARGET = $(TARGETDIR)/libPK-RenderHelpers_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Retail/PK-RenderHelpers_SDK1
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################

PERFILE_FLAGS_0 = $(ALL_CXXFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path`

# File sets
# #############################################


# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-RenderHelpers_SDK1
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
	@echo Cleaning PK-RenderHelpers_SDK1
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
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) -x c++-header -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
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

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
