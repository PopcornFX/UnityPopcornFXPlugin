# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_ios64
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
  CC = xcrun --sdk iphoneos clang -arch arm64
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch arm64
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
PCH = ../../ExternalLibs/pk_importerlib/src/precompiled/imp_precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../ExternalLibs -I../../ExternalLibs/pk_importerlib/src -I../../ExternalLibs/pk_importerlib/src/precompiled
FORCE_INCLUDE += -include pk_compiler_warnings.h
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
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

ifeq ($(config),debug_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64
TARGET = $(TARGETDIR)/libPK-ImporterLib_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Debug/PK-ImporterLib
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),release_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64
TARGET = $(TARGETDIR)/libPK-ImporterLib_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Release/PK-ImporterLib
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),retail_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64
TARGET = $(TARGETDIR)/libPK-ImporterLib_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Retail/PK-ImporterLib
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

GENERATED += $(OBJDIR)/Core.o
GENERATED += $(OBJDIR)/Events.o
GENERATED += $(OBJDIR)/Legacy.o
GENERATED += $(OBJDIR)/imp_ir_editor_effect.o
GENERATED += $(OBJDIR)/imp_ir_particle_effect.o
GENERATED += $(OBJDIR)/imp_ir_project_settings.o
GENERATED += $(OBJDIR)/imp_ir_renderers.o
GENERATED += $(OBJDIR)/imp_ir_samplers.o
GENERATED += $(OBJDIR)/imp_precompiled.o
GENERATED += $(OBJDIR)/imp_script_tokenizer.o
GENERATED += $(OBJDIR)/imp_v1_EditorParticle_Data.o
GENERATED += $(OBJDIR)/imp_v1_actionfactory.o
GENERATED += $(OBJDIR)/imp_v1_descriptor.o
GENERATED += $(OBJDIR)/imp_v1_dynamic_field_declarator.o
GENERATED += $(OBJDIR)/imp_v1_evolvers.o
GENERATED += $(OBJDIR)/imp_v1_particle_effect.o
GENERATED += $(OBJDIR)/imp_v1_projectsettings.o
GENERATED += $(OBJDIR)/imp_v1_renderers.o
GENERATED += $(OBJDIR)/imp_v1_samplers.o
GENERATED += $(OBJDIR)/imp_v1_scripts_utils.o
GENERATED += $(OBJDIR)/imp_v1_templates.o
GENERATED += $(OBJDIR)/tinyxml2.o
OBJECTS += $(OBJDIR)/Core.o
OBJECTS += $(OBJDIR)/Events.o
OBJECTS += $(OBJDIR)/Legacy.o
OBJECTS += $(OBJDIR)/imp_ir_editor_effect.o
OBJECTS += $(OBJDIR)/imp_ir_particle_effect.o
OBJECTS += $(OBJDIR)/imp_ir_project_settings.o
OBJECTS += $(OBJDIR)/imp_ir_renderers.o
OBJECTS += $(OBJDIR)/imp_ir_samplers.o
OBJECTS += $(OBJDIR)/imp_precompiled.o
OBJECTS += $(OBJDIR)/imp_script_tokenizer.o
OBJECTS += $(OBJDIR)/imp_v1_EditorParticle_Data.o
OBJECTS += $(OBJDIR)/imp_v1_actionfactory.o
OBJECTS += $(OBJDIR)/imp_v1_descriptor.o
OBJECTS += $(OBJDIR)/imp_v1_dynamic_field_declarator.o
OBJECTS += $(OBJDIR)/imp_v1_evolvers.o
OBJECTS += $(OBJDIR)/imp_v1_particle_effect.o
OBJECTS += $(OBJDIR)/imp_v1_projectsettings.o
OBJECTS += $(OBJDIR)/imp_v1_renderers.o
OBJECTS += $(OBJDIR)/imp_v1_samplers.o
OBJECTS += $(OBJDIR)/imp_v1_scripts_utils.o
OBJECTS += $(OBJDIR)/imp_v1_templates.o
OBJECTS += $(OBJDIR)/tinyxml2.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-ImporterLib
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
	@echo Cleaning PK-ImporterLib
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

$(OBJDIR)/imp_precompiled.o: ../../ExternalLibs/pk_importerlib/src/precompiled/imp_precompiled.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_ir_editor_effect.o: ../../ExternalLibs/pk_importerlib/src/IR/Editor/imp_ir_editor_effect.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Core.o: ../../ExternalLibs/pk_importerlib/src/IR/PopcornFXCore/Core.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Events.o: ../../ExternalLibs/pk_importerlib/src/IR/PopcornFXCore/Events.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Legacy.o: ../../ExternalLibs/pk_importerlib/src/IR/PopcornFXCore/Legacy.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_script_tokenizer.o: ../../ExternalLibs/pk_importerlib/src/IR/Utils/imp_script_tokenizer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_ir_particle_effect.o: ../../ExternalLibs/pk_importerlib/src/IR/imp_ir_particle_effect.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_ir_project_settings.o: ../../ExternalLibs/pk_importerlib/src/IR/imp_ir_project_settings.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_ir_renderers.o: ../../ExternalLibs/pk_importerlib/src/IR/imp_ir_renderers.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_ir_samplers.o: ../../ExternalLibs/pk_importerlib/src/IR/imp_ir_samplers.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_EditorParticle_Data.o: ../../ExternalLibs/pk_importerlib/src/v1/Editor/imp_v1_EditorParticle_Data.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_dynamic_field_declarator.o: ../../ExternalLibs/pk_importerlib/src/v1/Utils/imp_v1_dynamic_field_declarator.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_scripts_utils.o: ../../ExternalLibs/pk_importerlib/src/v1/Utils/imp_v1_scripts_utils.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_templates.o: ../../ExternalLibs/pk_importerlib/src/v1/Utils/imp_v1_templates.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_actionfactory.o: ../../ExternalLibs/pk_importerlib/src/v1/imp_v1_actionfactory.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_descriptor.o: ../../ExternalLibs/pk_importerlib/src/v1/imp_v1_descriptor.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_evolvers.o: ../../ExternalLibs/pk_importerlib/src/v1/imp_v1_evolvers.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_particle_effect.o: ../../ExternalLibs/pk_importerlib/src/v1/imp_v1_particle_effect.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_projectsettings.o: ../../ExternalLibs/pk_importerlib/src/v1/imp_v1_projectsettings.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_renderers.o: ../../ExternalLibs/pk_importerlib/src/v1/imp_v1_renderers.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imp_v1_samplers.o: ../../ExternalLibs/pk_importerlib/src/v1/imp_v1_samplers.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tinyxml2.o: ../../ExternalLibs/pk_importerlib/src/xmlparser/tinyxml2/tinyxml2.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
