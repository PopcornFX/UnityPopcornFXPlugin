# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_android
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
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../SDK/Samples/ParticleSample/src -I../../SDK/Samples/ParticleSample/src/include -I../../Native/Common/PKFX
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
  CC = $(EASYPATH)/arm-linux-androideabi-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH)/arm-linux-androideabi-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH)/arm-linux-androideabi-ar
endif
TARGETDIR = ../../../release/builds/Android_Samples
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_d
OBJDIR = ../intermediate/UnityStore/GM/android/Debug/PK-Sample_03_EngineHooks
DEFINES += -D_DEBUG -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_d.a -llog -lEGL -lGLESv3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android -shared

else ifeq ($(config),debug_android64)
ifeq ($(origin CC), default)
  CC = $(EASYPATH64)/aarch64-linux-android-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH64)/aarch64-linux-android-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH64)/aarch64-linux-android-ar
endif
TARGETDIR = ../../../release/builds/Android64_Samples
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_d
OBJDIR = ../intermediate/UnityStore/GM/android64/Debug/PK-Sample_03_EngineHooks
DEFINES += -D_DEBUG -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_d.a -llog -lEGL -lGLESv3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_d.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_d.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64 -shared

else ifeq ($(config),release_android)
ifeq ($(origin CC), default)
  CC = $(EASYPATH)/arm-linux-androideabi-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH)/arm-linux-androideabi-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH)/arm-linux-androideabi-ar
endif
TARGETDIR = ../../../release/builds/Android_Samples
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_r
OBJDIR = ../intermediate/UnityStore/GM/android/Release/PK-Sample_03_EngineHooks
DEFINES += -DNDEBUG -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_r.a -llog -lEGL -lGLESv3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android -shared

else ifeq ($(config),release_android64)
ifeq ($(origin CC), default)
  CC = $(EASYPATH64)/aarch64-linux-android-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH64)/aarch64-linux-android-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH64)/aarch64-linux-android-ar
endif
TARGETDIR = ../../../release/builds/Android64_Samples
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_r
OBJDIR = ../intermediate/UnityStore/GM/android64/Release/PK-Sample_03_EngineHooks
DEFINES += -DNDEBUG -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_r.a -llog -lEGL -lGLESv3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_r.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_r.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64 -shared

else ifeq ($(config),retail_android)
ifeq ($(origin CC), default)
  CC = $(EASYPATH)/arm-linux-androideabi-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH)/arm-linux-androideabi-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH)/arm-linux-androideabi-ar
endif
TARGETDIR = ../../../release/builds/Android_Samples
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_s
OBJDIR = ../intermediate/UnityStore/GM/android/Retail/PK-Sample_03_EngineHooks
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_s.a -llog -lEGL -lGLESv3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ZLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android/libPK-Runtime_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android -s -shared

else ifeq ($(config),retail_android64)
ifeq ($(origin CC), default)
  CC = $(EASYPATH64)/aarch64-linux-android-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH64)/aarch64-linux-android-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH64)/aarch64-linux-android-ar
endif
TARGETDIR = ../../../release/builds/Android64_Samples
TARGET = $(TARGETDIR)/PK-Sample_03_EngineHooks_s
OBJDIR = ../intermediate/UnityStore/GM/android64/Retail/PK-Sample_03_EngineHooks
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_USE_RENDER_HELPERS=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math
LIBS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_s.a -llog -lEGL -lGLESv3
LDDEPS += ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_DDS_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_PNG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CodecImage_JPG_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Plugin_CompilerBackend_CPU_VM_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ZLib_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-ParticlesToolbox_s.a ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64/libPK-Runtime_s.a
ALL_LDFLAGS += $(LDFLAGS) -L../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64 -s -shared

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


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

$(OBJDIR)/FxPlatformGeneric.o: ../../Native/Common/PKFX/FxPlatformGeneric.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ParticleSample_03_EngineHooks.o: ../../SDK/Samples/ParticleSample/src/ParticleSample_03_EngineHooks.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/precompiled.o: ../../SDK/Samples/ParticleSample/src/precompiled.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif