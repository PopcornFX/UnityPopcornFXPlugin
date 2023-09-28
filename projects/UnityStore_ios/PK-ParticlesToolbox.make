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

PCH = ../../ExternalLibs/Runtime/pk_particles_toolbox/pt_precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../ExternalLibs/Runtime/pk_particles_toolbox -I../../ExternalLibs/Runtime/pk_particles_toolbox/src/include
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
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_d.a
OBJDIR = ../intermediate/UnityStore/GM/ios/Debug/PK-ParticlesToolbox
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
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_d.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Debug/PK-ParticlesToolbox
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
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_r.a
OBJDIR = ../intermediate/UnityStore/GM/ios/Release/PK-ParticlesToolbox
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
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_r.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Release/PK-ParticlesToolbox
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
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_s.a
OBJDIR = ../intermediate/UnityStore/GM/ios/Retail/PK-ParticlesToolbox
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
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-ParticlesToolbox_s.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Retail/PK-ParticlesToolbox
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

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/pt_compile.o
GENERATED += $(OBJDIR)/pt_debug_symbol_provider.o
GENERATED += $(OBJDIR)/pt_deformers_morph_morpher.o
GENERATED += $(OBJDIR)/pt_deformers_skin_core.o
GENERATED += $(OBJDIR)/pt_deformers_skin_helpers.o
GENERATED += $(OBJDIR)/pt_deformers_skin_skeletal_skinner.o
GENERATED += $(OBJDIR)/pt_deformers_skin_skeletal_skinner_jobs.o
GENERATED += $(OBJDIR)/pt_deformers_skin_stats.o
GENERATED += $(OBJDIR)/pt_helpers.o
GENERATED += $(OBJDIR)/pt_init.o
GENERATED += $(OBJDIR)/pt_legacy_timeline.o
GENERATED += $(OBJDIR)/pt_legacy_timeline_track.o
GENERATED += $(OBJDIR)/pt_legacy_timeline_track_domain.o
GENERATED += $(OBJDIR)/pt_overdraw.o
GENERATED += $(OBJDIR)/pt_precompiled.o
GENERATED += $(OBJDIR)/pt_profiler.o
GENERATED += $(OBJDIR)/pt_profiler_new.o
GENERATED += $(OBJDIR)/pt_seek_interface.o
GENERATED += $(OBJDIR)/pt_skeleton_animation.o
GENERATED += $(OBJDIR)/pt_skeleton_animation_instance.o
GENERATED += $(OBJDIR)/pt_skeleton_instance.o
GENERATED += $(OBJDIR)/pt_trace.o
GENERATED += $(OBJDIR)/pt_transforms.o
OBJECTS += $(OBJDIR)/pt_compile.o
OBJECTS += $(OBJDIR)/pt_debug_symbol_provider.o
OBJECTS += $(OBJDIR)/pt_deformers_morph_morpher.o
OBJECTS += $(OBJDIR)/pt_deformers_skin_core.o
OBJECTS += $(OBJDIR)/pt_deformers_skin_helpers.o
OBJECTS += $(OBJDIR)/pt_deformers_skin_skeletal_skinner.o
OBJECTS += $(OBJDIR)/pt_deformers_skin_skeletal_skinner_jobs.o
OBJECTS += $(OBJDIR)/pt_deformers_skin_stats.o
OBJECTS += $(OBJDIR)/pt_helpers.o
OBJECTS += $(OBJDIR)/pt_init.o
OBJECTS += $(OBJDIR)/pt_legacy_timeline.o
OBJECTS += $(OBJDIR)/pt_legacy_timeline_track.o
OBJECTS += $(OBJDIR)/pt_legacy_timeline_track_domain.o
OBJECTS += $(OBJDIR)/pt_overdraw.o
OBJECTS += $(OBJDIR)/pt_precompiled.o
OBJECTS += $(OBJDIR)/pt_profiler.o
OBJECTS += $(OBJDIR)/pt_profiler_new.o
OBJECTS += $(OBJDIR)/pt_seek_interface.o
OBJECTS += $(OBJDIR)/pt_skeleton_animation.o
OBJECTS += $(OBJDIR)/pt_skeleton_animation_instance.o
OBJECTS += $(OBJDIR)/pt_skeleton_instance.o
OBJECTS += $(OBJDIR)/pt_trace.o
OBJECTS += $(OBJDIR)/pt_transforms.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-ParticlesToolbox
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
	@echo Cleaning PK-ParticlesToolbox
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


# File Rules
# #############################################

$(OBJDIR)/pt_precompiled.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/pt_precompiled.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_legacy_timeline.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/LegacyTimeline/pt_legacy_timeline.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_legacy_timeline_track.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/LegacyTimeline/pt_legacy_timeline_track.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_legacy_timeline_track_domain.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/LegacyTimeline/pt_legacy_timeline_track_domain.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_deformers_morph_morpher.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/MeshDeformers/pt_deformers_morph_morpher.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_deformers_skin_core.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/MeshDeformers/pt_deformers_skin_core.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_deformers_skin_helpers.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/MeshDeformers/pt_deformers_skin_helpers.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_deformers_skin_skeletal_skinner.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/MeshDeformers/pt_deformers_skin_skeletal_skinner.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_deformers_skin_skeletal_skinner_jobs.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/MeshDeformers/pt_deformers_skin_skeletal_skinner_jobs.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_deformers_skin_stats.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/MeshDeformers/pt_deformers_skin_stats.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_skeleton_animation.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/Skeleton/pt_skeleton_animation.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_skeleton_animation_instance.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/Skeleton/pt_skeleton_animation_instance.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_skeleton_instance.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/Skeleton/pt_skeleton_instance.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_compile.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_compile.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_debug_symbol_provider.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_debug_symbol_provider.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_helpers.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_helpers.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_init.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_init.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_overdraw.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_overdraw.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_profiler.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_profiler.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_profiler_new.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_profiler_new.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_seek_interface.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_seek_interface.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_trace.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_trace.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pt_transforms.o: ../../ExternalLibs/Runtime/pk_particles_toolbox/src/pt_transforms.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
