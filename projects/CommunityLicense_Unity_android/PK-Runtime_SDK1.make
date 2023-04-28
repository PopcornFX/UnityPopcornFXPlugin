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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android
TARGET = $(TARGETDIR)/libPK-Runtime_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android/Debug/PK-Runtime_SDK1
DEFINES += -D_DEBUG
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../../ExternalLibs/Runtime/pk_maths/src -I../../../ExternalLibs/Runtime/pk_maths -I../../../ExternalLibs/Runtime/pk_toolkit -I../../../ExternalLibs/Runtime/pk_toolkit/src/include -I../../../ExternalLibs/Runtime/pk_kernel -I../../../ExternalLibs/Runtime/pk_imaging -I../../../ExternalLibs/Runtime/pk_base_object/src -I../../../ExternalLibs/Runtime/pk_base_object/src/precompiled -I../../../ExternalLibs/Runtime/pk_compiler/src -I../../../ExternalLibs/Runtime/pk_compiler/src/precompiled -I../../../ExternalLibs/Runtime/pk_engine_utils/src/precompiled -I../../../ExternalLibs/Runtime/pk_geometrics -I../../../ExternalLibs/Runtime/pk_particles/src -I../intermediate/CommunityLicense_Unity/GM/android/Debug/PK-Runtime_SDK1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon -pipe
ALL_LDFLAGS += $(LDFLAGS)

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android64
TARGET = $(TARGETDIR)/libPK-Runtime_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android64/Debug/PK-Runtime_SDK1
DEFINES += -D_DEBUG
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../../ExternalLibs/Runtime/pk_maths/src -I../../../ExternalLibs/Runtime/pk_maths -I../../../ExternalLibs/Runtime/pk_toolkit -I../../../ExternalLibs/Runtime/pk_toolkit/src/include -I../../../ExternalLibs/Runtime/pk_kernel -I../../../ExternalLibs/Runtime/pk_imaging -I../../../ExternalLibs/Runtime/pk_base_object/src -I../../../ExternalLibs/Runtime/pk_base_object/src/precompiled -I../../../ExternalLibs/Runtime/pk_compiler/src -I../../../ExternalLibs/Runtime/pk_compiler/src/precompiled -I../../../ExternalLibs/Runtime/pk_engine_utils/src/precompiled -I../../../ExternalLibs/Runtime/pk_geometrics -I../../../ExternalLibs/Runtime/pk_particles/src -I../intermediate/CommunityLicense_Unity/GM/android64/Debug/PK-Runtime_SDK1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe
ALL_LDFLAGS += $(LDFLAGS)

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android
TARGET = $(TARGETDIR)/libPK-Runtime_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android/Release/PK-Runtime_SDK1
DEFINES += -DNDEBUG
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../../ExternalLibs/Runtime/pk_maths/src -I../../../ExternalLibs/Runtime/pk_maths -I../../../ExternalLibs/Runtime/pk_toolkit -I../../../ExternalLibs/Runtime/pk_toolkit/src/include -I../../../ExternalLibs/Runtime/pk_kernel -I../../../ExternalLibs/Runtime/pk_imaging -I../../../ExternalLibs/Runtime/pk_base_object/src -I../../../ExternalLibs/Runtime/pk_base_object/src/precompiled -I../../../ExternalLibs/Runtime/pk_compiler/src -I../../../ExternalLibs/Runtime/pk_compiler/src/precompiled -I../../../ExternalLibs/Runtime/pk_engine_utils/src/precompiled -I../../../ExternalLibs/Runtime/pk_geometrics -I../../../ExternalLibs/Runtime/pk_particles/src -I../intermediate/CommunityLicense_Unity/GM/android/Release/PK-Runtime_SDK1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_LDFLAGS += $(LDFLAGS)

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android64
TARGET = $(TARGETDIR)/libPK-Runtime_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android64/Release/PK-Runtime_SDK1
DEFINES += -DNDEBUG
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../../ExternalLibs/Runtime/pk_maths/src -I../../../ExternalLibs/Runtime/pk_maths -I../../../ExternalLibs/Runtime/pk_toolkit -I../../../ExternalLibs/Runtime/pk_toolkit/src/include -I../../../ExternalLibs/Runtime/pk_kernel -I../../../ExternalLibs/Runtime/pk_imaging -I../../../ExternalLibs/Runtime/pk_base_object/src -I../../../ExternalLibs/Runtime/pk_base_object/src/precompiled -I../../../ExternalLibs/Runtime/pk_compiler/src -I../../../ExternalLibs/Runtime/pk_compiler/src/precompiled -I../../../ExternalLibs/Runtime/pk_engine_utils/src/precompiled -I../../../ExternalLibs/Runtime/pk_geometrics -I../../../ExternalLibs/Runtime/pk_particles/src -I../intermediate/CommunityLicense_Unity/GM/android64/Release/PK-Runtime_SDK1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_LDFLAGS += $(LDFLAGS)

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android
TARGET = $(TARGETDIR)/libPK-Runtime_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android/Retail/PK-Runtime_SDK1
DEFINES += -DNDEBUG -DPK_RETAIL
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../../ExternalLibs/Runtime/pk_maths/src -I../../../ExternalLibs/Runtime/pk_maths -I../../../ExternalLibs/Runtime/pk_toolkit -I../../../ExternalLibs/Runtime/pk_toolkit/src/include -I../../../ExternalLibs/Runtime/pk_kernel -I../../../ExternalLibs/Runtime/pk_imaging -I../../../ExternalLibs/Runtime/pk_base_object/src -I../../../ExternalLibs/Runtime/pk_base_object/src/precompiled -I../../../ExternalLibs/Runtime/pk_compiler/src -I../../../ExternalLibs/Runtime/pk_compiler/src/precompiled -I../../../ExternalLibs/Runtime/pk_engine_utils/src/precompiled -I../../../ExternalLibs/Runtime/pk_geometrics -I../../../ExternalLibs/Runtime/pk_particles/src -I../intermediate/CommunityLicense_Unity/GM/android/Retail/PK-Runtime_SDK1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_LDFLAGS += $(LDFLAGS) -s

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
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android64
TARGET = $(TARGETDIR)/libPK-Runtime_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android64/Retail/PK-Runtime_SDK1
DEFINES += -DNDEBUG -DPK_RETAIL
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../../ExternalLibs/Runtime/pk_maths/src -I../../../ExternalLibs/Runtime/pk_maths -I../../../ExternalLibs/Runtime/pk_toolkit -I../../../ExternalLibs/Runtime/pk_toolkit/src/include -I../../../ExternalLibs/Runtime/pk_kernel -I../../../ExternalLibs/Runtime/pk_imaging -I../../../ExternalLibs/Runtime/pk_base_object/src -I../../../ExternalLibs/Runtime/pk_base_object/src/precompiled -I../../../ExternalLibs/Runtime/pk_compiler/src -I../../../ExternalLibs/Runtime/pk_compiler/src/precompiled -I../../../ExternalLibs/Runtime/pk_engine_utils/src/precompiled -I../../../ExternalLibs/Runtime/pk_geometrics -I../../../ExternalLibs/Runtime/pk_particles/src -I../intermediate/CommunityLicense_Unity/GM/android64/Retail/PK-Runtime_SDK1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_LDFLAGS += $(LDFLAGS) -s

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
	@echo Linking PK-Runtime_SDK1
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
	@echo Cleaning PK-Runtime_SDK1
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
