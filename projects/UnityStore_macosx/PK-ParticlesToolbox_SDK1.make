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
PCH = ../../ExternalLibs/Runtime/pk_particles_toolbox/pt_precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../ExternalLibs/Runtime/pk_particles_toolbox -I../../ExternalLibs/Runtime/pk_particles_toolbox/src/include
FORCE_INCLUDE += -include pk_compiler_warnings.h
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS +=
LDDEPS +=
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

ifeq ($(config),debug_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_d.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Debug/PK-ParticlesToolbox_SDK1
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -m64 -target x86_64-apple-macos10.14

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_d.a
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Debug/PK-ParticlesToolbox_SDK1
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -target arm64-apple-macos11.0

else ifeq ($(config),release_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_r.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Release/PK-ParticlesToolbox_SDK1
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -m64 -target x86_64-apple-macos10.14

else ifeq ($(config),release_arm64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_r.a
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Release/PK-ParticlesToolbox_SDK1
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -target arm64-apple-macos11.0

else ifeq ($(config),retail_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_s.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Retail/PK-ParticlesToolbox_SDK1
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -m64 -target x86_64-apple-macos10.14

else ifeq ($(config),retail_arm64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_s.a
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Retail/PK-ParticlesToolbox_SDK1
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -fvisibility=hidden -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -target arm64-apple-macos11.0

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################


# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-ParticlesToolbox_SDK1
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
	@echo Cleaning PK-ParticlesToolbox_SDK1
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

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
