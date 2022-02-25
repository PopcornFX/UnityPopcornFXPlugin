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

PCH = ../../ExternalLibs/Runtime/pk_render_helpers/src/precompiled/rh_precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../ExternalLibs/Runtime/pk_render_helpers/src -I../../ExternalLibs/Runtime/pk_render_helpers/src/precompiled
FORCE_INCLUDE += -include pk_compiler_warnings.h
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS +=
LDDEPS +=
ALL_LDFLAGS += $(LDFLAGS) -s
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
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android
TARGET = $(TARGETDIR)/libPK-RenderHelpers_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android/Debug/PK-RenderHelpers
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -O2 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MMD -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -O2 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon -pipe

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
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android64
TARGET = $(TARGETDIR)/libPK-RenderHelpers_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android64/Debug/PK-RenderHelpers
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -O2 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MMD -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -O2 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe

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
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android
TARGET = $(TARGETDIR)/libPK-RenderHelpers_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android/Release/PK-RenderHelpers
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MMD -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe

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
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android64
TARGET = $(TARGETDIR)/libPK-RenderHelpers_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android64/Release/PK-RenderHelpers
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MMD -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe

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
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android
TARGET = $(TARGETDIR)/libPK-RenderHelpers_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android/Retail/PK-RenderHelpers
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MMD -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe

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
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android64
TARGET = $(TARGETDIR)/libPK-RenderHelpers_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android64/Retail/PK-RenderHelpers
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MMD -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

OBJECTS :=

OBJECTS += $(OBJDIR)/rh_basic_renderer_properties.o
OBJECTS += $(OBJDIR)/rh_billboard.o
OBJECTS += $(OBJDIR)/rh_billboard_cpu.o
OBJECTS += $(OBJDIR)/rh_billboard_cpu_tasks.o
OBJECTS += $(OBJDIR)/rh_billboard_ribbon_batch_helper.o
OBJECTS += $(OBJDIR)/rh_bufferpool.o
OBJECTS += $(OBJDIR)/rh_common.o
OBJECTS += $(OBJDIR)/rh_copystream_cpu.o
OBJECTS += $(OBJDIR)/rh_copystream_cpu_tasks.o
OBJECTS += $(OBJDIR)/rh_decal.o
OBJECTS += $(OBJDIR)/rh_decal_cpu.o
OBJECTS += $(OBJDIR)/rh_decal_cpu_tasks.o
OBJECTS += $(OBJDIR)/rh_draw_requests.o
OBJECTS += $(OBJDIR)/rh_init.o
OBJECTS += $(OBJDIR)/rh_light.o
OBJECTS += $(OBJDIR)/rh_main.o
OBJECTS += $(OBJDIR)/rh_mesh.o
OBJECTS += $(OBJDIR)/rh_mesh_cpu.o
OBJECTS += $(OBJDIR)/rh_mesh_cpu_tasks.o
OBJECTS += $(OBJDIR)/rh_particle_render_data_factory.o
OBJECTS += $(OBJDIR)/rh_precompiled.o
OBJECTS += $(OBJDIR)/rh_renderers_walker.o
OBJECTS += $(OBJDIR)/rh_ribbon.o
OBJECTS += $(OBJDIR)/rh_ribbon_cpu.o
OBJECTS += $(OBJDIR)/rh_ribbon_cpu_tasks.o
OBJECTS += $(OBJDIR)/rh_sound.o
OBJECTS += $(OBJDIR)/rh_triangle.o
OBJECTS += $(OBJDIR)/rh_triangle_cpu.o
OBJECTS += $(OBJDIR)/rh_triangle_cpu_tasks.o
OBJECTS += $(OBJDIR)/rh_vertex_animation_renderer_properties.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-RenderHelpers
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
	@echo Cleaning PK-RenderHelpers
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

$(OBJDIR)/rh_precompiled.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/precompiled/rh_precompiled.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_basic_renderer_properties.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/basic_renderer_properties/rh_basic_renderer_properties.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_vertex_animation_renderer_properties.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/basic_renderer_properties/rh_vertex_animation_renderer_properties.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_billboard_ribbon_batch_helper.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/batches/rh_billboard_ribbon_batch_helper.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_bufferpool.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/buffer_pool/rh_bufferpool.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_billboard.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_billboard.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_billboard_cpu.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_billboard_cpu.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_billboard_cpu_tasks.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_billboard_cpu_tasks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_common.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_common.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_copystream_cpu.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_copystream_cpu.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_copystream_cpu_tasks.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_copystream_cpu_tasks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_decal.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_decal.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_decal_cpu.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_decal_cpu.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_decal_cpu_tasks.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_decal_cpu_tasks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_draw_requests.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_draw_requests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_light.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_light.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_mesh.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_mesh.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_mesh_cpu.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_mesh_cpu.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_mesh_cpu_tasks.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_mesh_cpu_tasks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_ribbon.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_ribbon.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_ribbon_cpu.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_ribbon_cpu.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_ribbon_cpu_tasks.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_ribbon_cpu_tasks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_sound.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_sound.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_triangle.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_triangle.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_triangle_cpu.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_triangle_cpu.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_triangle_cpu_tasks.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/draw_requests/rh_triangle_cpu_tasks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_particle_render_data_factory.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/frame_collector/rh_particle_render_data_factory.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_renderers_walker.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/frame_collector/rh_renderers_walker.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_init.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/rh_init.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rh_main.o: ../../ExternalLibs/Runtime/pk_render_helpers/src/rh_main.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
