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

RESCOMP = windres
PCH = ../../ExternalLibs/pk_upgrader/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../ExternalLibs/pk_upgrader -I../../ExternalLibs
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = ../../../release/application/BinariesGM_linux_x64_d
TARGET = $(TARGETDIR)/PK-Upgrader
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Debug/PK-Upgrader
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -pipe
LIBS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-UpgraderLib_d.a ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-ImporterLib_d.a -lPK-ParticlesToolbox_d -lPK-Runtime_d -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-UpgraderLib_d.a ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-ImporterLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64 -L/usr/lib64 -m64

else ifeq ($(config),release_x64)
TARGETDIR = ../../../release/application/BinariesGM_linux_x64_r
TARGET = $(TARGETDIR)/PK-Upgrader
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Release/PK-Upgrader
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
LIBS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-UpgraderLib_r.a ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-ImporterLib_r.a -lPK-ParticlesToolbox_r -lPK-Runtime_r -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-UpgraderLib_r.a ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-ImporterLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64 -L/usr/lib64 -m64

else ifeq ($(config),retail_x64)
TARGETDIR = ../../../release/application/BinariesGM_linux_x64_r
TARGET = $(TARGETDIR)/PK-Upgrader_s
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Retail/PK-Upgrader
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
LIBS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-UpgraderLib_s.a ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-ImporterLib_s.a -lPK-ParticlesToolbox_s -lPK-Runtime_s -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-UpgraderLib_s.a ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-ImporterLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64 -L/usr/lib64 -m64 -s

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

OBJECTS :=

OBJECTS += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/tests.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
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
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
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
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/precompiled.o: ../../ExternalLibs/pk_upgrader/precompiled.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tests.o: ../../ExternalLibs/pk_upgrader/tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
