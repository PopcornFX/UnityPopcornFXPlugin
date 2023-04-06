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
PCH = ../../../ExternalLibs/PK-AssetBaker/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../../ExternalLibs/PK-AssetBaker -I../../../ExternalLibs/PK-AssetBakerLib -I../../Native/Common/PKFX
FORCE_INCLUDE += -include ../../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
ALL_LDFLAGS += $(LDFLAGS) -L../../../ExternalLibs/Runtime/libs/freetype-2.5.5/lib/linux64 -L../../../ExternalLibs/CodecMesh_FBX/libs/linux64 -L../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64 -L/usr/lib64 -m64
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = ../../../release/application/BinariesGM_linux_x64_d
TARGET = $(TARGETDIR)/PK-AssetBaker
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Debug/PK-AssetBaker
DEFINES += -D_DEBUG -DPK_USE_RENDER_HELPERS=0 -DUSE_FBXIMPORTER -DUSE_IMAGE_PLUGIN_HDR -DUSE_IMAGE_PLUGIN_TIFF -DUSE_IMAGE_PLUGIN_PKM -DUSE_IMAGE_PLUGIN_PVR
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -pipe
LIBS += ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-AssetBakerLib_d.a -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_TGA_d -lPK-Plugin_CodecImage_PKM_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TIFF_d -lPK-Plugin_CodecImage_HDR_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-ZLib_d -lPK-Plugin_CodecMesh_FBX_d -lPK-Plugin_CodecImage_EXR_d -lfreetype -lfbxsdk_d -lxml2 -lPK-ParticlesToolbox_d -lPK-Runtime_d -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-AssetBakerLib_d.a

else ifeq ($(config),release_x64)
TARGETDIR = ../../../release/application/BinariesGM_linux_x64_r
TARGET = $(TARGETDIR)/PK-AssetBaker
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Release/PK-AssetBaker
DEFINES += -DNDEBUG -DPK_USE_RENDER_HELPERS=0 -DUSE_FBXIMPORTER -DUSE_IMAGE_PLUGIN_HDR -DUSE_IMAGE_PLUGIN_TIFF -DUSE_IMAGE_PLUGIN_PKM -DUSE_IMAGE_PLUGIN_PVR
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
LIBS += ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-AssetBakerLib_r.a -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_TGA_r -lPK-Plugin_CodecImage_PKM_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TIFF_r -lPK-Plugin_CodecImage_HDR_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-ZLib_r -lPK-Plugin_CodecMesh_FBX_r -lPK-Plugin_CodecImage_EXR_r -lfreetype -lfbxsdk_r -lxml2 -lPK-ParticlesToolbox_r -lPK-Runtime_r -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-AssetBakerLib_r.a

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/AssetBaker.o
GENERATED += $(OBJDIR)/AssetBaker_SimulationInterfaces.o
GENERATED += $(OBJDIR)/FxPlugins.o
GENERATED += $(OBJDIR)/FxStartup.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/AssetBaker.o
OBJECTS += $(OBJDIR)/AssetBaker_SimulationInterfaces.o
OBJECTS += $(OBJDIR)/FxPlugins.o
OBJECTS += $(OBJDIR)/FxStartup.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-AssetBaker
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
	@echo Cleaning PK-AssetBaker
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

$(OBJDIR)/AssetBaker.o: ../../../ExternalLibs/PK-AssetBaker/AssetBaker.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_SimulationInterfaces.o: ../../../ExternalLibs/PK-AssetBaker/AssetBaker_SimulationInterfaces.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FxPlugins.o: ../../Native/Common/PKFX/FxPlugins.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FxStartup.o: ../../Native/Common/PKFX/FxStartup.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/precompiled.o: ../../../ExternalLibs/PK-AssetBaker/precompiled.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
