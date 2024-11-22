# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x32
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

PCH = ../../SDK/Samples/ParticleSample/src/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../SDK/Samples/ParticleSample/src -I../../SDK/Samples/ParticleSample/src/include -I../../ExternalLibs/pk_upgraderlib/include -I../../Native/Common/PKFX
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
ifeq ($(origin CC), default)
  CC = gcc
endif
ifeq ($(origin CXX), default)
  CXX = g++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
TARGETDIR = ../../../release/builds/x32_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Upgrader_linux_d
OBJDIR = ../intermediate/UnityStore/GM/x32/Debug/PK-Sample_05_Upgrader
DEFINES += -D_DEBUG -DPK_USE_RENDER_HELPERS=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-UpgraderLib_d.a -lPK-Plugin_CodecImage_PKIM_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-ZLib_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-UpgraderLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32 -L/usr/lib32 -m32

else ifeq ($(config),debug_x64)
ifeq ($(origin CC), default)
  CC = gcc
endif
ifeq ($(origin CXX), default)
  CXX = g++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Upgrader_linux_d
OBJDIR = ../intermediate/UnityStore/GM/x64/Debug/PK-Sample_05_Upgrader
DEFINES += -D_DEBUG -DPK_USE_RENDER_HELPERS=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-UpgraderLib_d.a -lPK-Plugin_CodecImage_PKIM_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-ZLib_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-UpgraderLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64 -L/usr/lib64 -m64

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
RESCOMP = windres
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Upgrader_linux_d
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Debug/PK-Sample_05_Upgrader
DEFINES += -D_DEBUG -DPK_USE_RENDER_HELPERS=0
FORCE_INCLUDE +=
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -target aarch64-linux-gnu
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe -target aarch64-linux-gnu
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-UpgraderLib_d.a -lPK-Plugin_CodecImage_PKIM_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-ZLib_d -lPK-ParticlesToolbox_d -lPK-Runtime_d -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-UpgraderLib_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64 -fuse-ld=lld -target aarch64-linux-gnu

else ifeq ($(config),release_x32)
ifeq ($(origin CC), default)
  CC = gcc
endif
ifeq ($(origin CXX), default)
  CXX = g++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
TARGETDIR = ../../../release/builds/x32_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Upgrader_linux_r
OBJDIR = ../intermediate/UnityStore/GM/x32/Release/PK-Sample_05_Upgrader
DEFINES += -DNDEBUG -DPK_USE_RENDER_HELPERS=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-UpgraderLib_r.a -lPK-Plugin_CodecImage_PKIM_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-ZLib_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-UpgraderLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32 -L/usr/lib32 -m32

else ifeq ($(config),release_x64)
ifeq ($(origin CC), default)
  CC = gcc
endif
ifeq ($(origin CXX), default)
  CXX = g++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Upgrader_linux_r
OBJDIR = ../intermediate/UnityStore/GM/x64/Release/PK-Sample_05_Upgrader
DEFINES += -DNDEBUG -DPK_USE_RENDER_HELPERS=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-UpgraderLib_r.a -lPK-Plugin_CodecImage_PKIM_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-ZLib_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-UpgraderLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64 -L/usr/lib64 -m64

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
RESCOMP = windres
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Upgrader_linux_r
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Release/PK-Sample_05_Upgrader
DEFINES += -DNDEBUG -DPK_USE_RENDER_HELPERS=0
FORCE_INCLUDE +=
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -target aarch64-linux-gnu
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -target aarch64-linux-gnu
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-UpgraderLib_r.a -lPK-Plugin_CodecImage_PKIM_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-ZLib_r -lPK-ParticlesToolbox_r -lPK-Runtime_r -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-UpgraderLib_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64 -fuse-ld=lld -target aarch64-linux-gnu

else ifeq ($(config),retail_x32)
ifeq ($(origin CC), default)
  CC = gcc
endif
ifeq ($(origin CXX), default)
  CXX = g++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
TARGETDIR = ../../../release/builds/x32_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Upgrader_linux_s
OBJDIR = ../intermediate/UnityStore/GM/x32/Retail/PK-Sample_05_Upgrader
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_USE_RENDER_HELPERS=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-UpgraderLib_s.a -lPK-Plugin_CodecImage_PKIM_s -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-ZLib_s -lPK-ParticlesToolbox_s -lPK-Runtime_s -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32/libPK-UpgraderLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32 -L/usr/lib32 -m32 -s

else ifeq ($(config),retail_x64)
ifeq ($(origin CC), default)
  CC = gcc
endif
ifeq ($(origin CXX), default)
  CXX = g++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
RESCOMP = windres
TARGETDIR = ../../../release/builds/x64_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Upgrader_linux_s
OBJDIR = ../intermediate/UnityStore/GM/x64/Retail/PK-Sample_05_Upgrader
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_USE_RENDER_HELPERS=0
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -fvisibility=hidden -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-UpgraderLib_s.a -lPK-Plugin_CodecImage_PKIM_s -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-ZLib_s -lPK-ParticlesToolbox_s -lPK-Runtime_s -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64/libPK-UpgraderLib_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64 -L/usr/lib64 -m64 -s

else ifeq ($(config),retail_arm64)
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
TARGETDIR = ../../../release/builds/ARM64_Samples
TARGET = $(TARGETDIR)/PK-Sample_05_Upgrader_linux_s
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Retail/PK-Sample_05_Upgrader
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_USE_RENDER_HELPERS=0
FORCE_INCLUDE +=
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -fvisibility=hidden -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -target aarch64-linux-gnu
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fvisibility-inlines-hidden -fomit-frame-pointer -O3 -fno-strict-aliasing -fvisibility=hidden -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe -target aarch64-linux-gnu
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-UpgraderLib_s.a -lPK-Plugin_CodecImage_PKIM_s -lPK-Plugin_CodecImage_DDS_s -lPK-Plugin_CodecImage_PNG_s -lPK-Plugin_CodecImage_JPG_s -lPK-Plugin_CompilerBackend_CPU_VM_s -lPK-ZLib_s -lPK-ParticlesToolbox_s -lPK-Runtime_s -ldl -lpthread -lm -lz
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_ARM64/libPK-UpgraderLib_s.a
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

GENERATED += $(OBJDIR)/FxStartup.o
GENERATED += $(OBJDIR)/ParticleSample_05_Upgrader.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/FxStartup.o
OBJECTS += $(OBJDIR)/ParticleSample_05_Upgrader.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Sample_05_Upgrader
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
	@echo Cleaning PK-Sample_05_Upgrader
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

$(OBJDIR)/FxStartup.o: ../../Native/Common/PKFX/FxStartup.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ParticleSample_05_Upgrader.o: ../../SDK/Samples/ParticleSample/src/ParticleSample_05_Upgrader.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/precompiled.o: ../../SDK/Samples/ParticleSample/src/precompiled.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
