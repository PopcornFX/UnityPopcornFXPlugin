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
PCH = ../../ExternalLibs/pk_upgrader/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../ExternalLibs/pk_upgrader -I../../ExternalLibs
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
TARGETDIR = ../../../release/application/BinariesGM_macosx_x64_d
TARGET = $(TARGETDIR)/PK-Upgrader
OBJDIR = ../intermediate/UnityStore/GM/x64/Debug/PK-Upgrader
DEFINES += -D_DEBUG -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-UpgraderLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_d.a
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-UpgraderLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64 -m64 -target x86_64-apple-macos10.14 -liconv

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../../release/application/BinariesGM_macosx_ARM64_d
TARGET = $(TARGETDIR)/PK-Upgrader
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Debug/PK-Upgrader
DEFINES += -D_DEBUG -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-UpgraderLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_d.a
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-UpgraderLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64 -target arm64-apple-macos11.0 -liconv

else ifeq ($(config),release_x64)
TARGETDIR = ../../../release/application/BinariesGM_macosx_x64_r
TARGET = $(TARGETDIR)/PK-Upgrader
OBJDIR = ../intermediate/UnityStore/GM/x64/Release/PK-Upgrader
DEFINES += -DNDEBUG -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-UpgraderLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_r.a
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-UpgraderLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64 -m64 -target x86_64-apple-macos10.14 -liconv

else ifeq ($(config),release_arm64)
TARGETDIR = ../../../release/application/BinariesGM_macosx_ARM64_r
TARGET = $(TARGETDIR)/PK-Upgrader
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Release/PK-Upgrader
DEFINES += -DNDEBUG -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-UpgraderLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_r.a
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-UpgraderLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64 -target arm64-apple-macos11.0 -liconv

else ifeq ($(config),retail_x64)
TARGETDIR = ../../../release/application/BinariesGM_macosx_x64_r
TARGET = $(TARGETDIR)/PK-Upgrader_s
OBJDIR = ../intermediate/UnityStore/GM/x64/Retail/PK-Upgrader
DEFINES += -DNDEBUG -DPK_RETAIL -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-UpgraderLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_s.a
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-UpgraderLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64/libPK-Runtime_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64 -m64 -target x86_64-apple-macos10.14 -liconv

else ifeq ($(config),retail_arm64)
TARGETDIR = ../../../release/application/BinariesGM_macosx_ARM64_r
TARGET = $(TARGETDIR)/PK-Upgrader_s
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Retail/PK-Upgrader
DEFINES += -DNDEBUG -DPK_RETAIL -DMACOSX
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-UpgraderLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_s.a
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-UpgraderLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64/libPK-Runtime_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64 -target arm64-apple-macos11.0 -liconv

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/main.o
GENERATED += $(OBJDIR)/precompiled.o
GENERATED += $(OBJDIR)/tests.o
OBJECTS += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/tests.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Upgrader
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
	@echo Cleaning PK-Upgrader
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

$(OBJDIR)/main.o: ../../ExternalLibs/pk_upgrader/main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/precompiled.o: ../../ExternalLibs/pk_upgrader/precompiled.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tests.o: ../../ExternalLibs/pk_upgrader/tests.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif