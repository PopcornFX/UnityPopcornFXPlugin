# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x32
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
PCH = ../../ExternalLibs/CodecMesh_FBX/include/fbx_precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../ExternalLibs/CodecMesh_FBX -I../../ExternalLibs/CodecMesh_FBX/include -I../../ExternalLibs/CodecMesh_FBX/NvTriStrip -I../../ExternalLibs/Runtime -I../../ExternalLibs/CodecMesh_FBX/include/FBX_2019.2
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h -include pk_compiler_warnings.h
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

ifeq ($(config),debug_x32)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32
TARGET = $(TARGETDIR)/libPK-Plugin_CodecMesh_FBX_d.a
OBJDIR = ../intermediate/UnityStore/GM/x32/Debug/PK-Plugin_CodecMesh_FBX
DEFINES += -D_DEBUG -DFBXSDK_STATIC_LINK=1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

else ifeq ($(config),debug_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecMesh_FBX_d.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Debug/PK-Plugin_CodecMesh_FBX
DEFINES += -D_DEBUG -DFBXSDK_STATIC_LINK=1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -ggdb -pipe -mfpmath=sse
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),release_x32)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32
TARGET = $(TARGETDIR)/libPK-Plugin_CodecMesh_FBX_r.a
OBJDIR = ../intermediate/UnityStore/GM/x32/Release/PK-Plugin_CodecMesh_FBX
DEFINES += -DNDEBUG -DFBXSDK_STATIC_LINK=1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

else ifeq ($(config),release_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecMesh_FBX_r.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Release/PK-Plugin_CodecMesh_FBX
DEFINES += -DNDEBUG -DFBXSDK_STATIC_LINK=1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),retail_x32)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x32
TARGET = $(TARGETDIR)/libPK-Plugin_CodecMesh_FBX_s.a
OBJDIR = ../intermediate/UnityStore/GM/x32/Retail/PK-Plugin_CodecMesh_FBX
DEFINES += -DNDEBUG -DPK_RETAIL -DFBXSDK_STATIC_LINK=1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -msse2 -Wall -Wextra -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -s

else ifeq ($(config),retail_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_linux_x64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecMesh_FBX_s.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Retail/PK-Plugin_CodecMesh_FBX
DEFINES += -DNDEBUG -DPK_RETAIL -DFBXSDK_STATIC_LINK=1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -msse2 -Wall -Wextra -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-class-memaccess -Wno-unused-parameter -fno-math-errno -fno-trapping-math -pipe -mfpmath=sse
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/Common.o
GENERATED += $(OBJDIR)/NvTriStrip.o
GENERATED += $(OBJDIR)/NvTriStripObjects.o
GENERATED += $(OBJDIR)/fbx_export.o
GENERATED += $(OBJDIR)/fbx_import.o
GENERATED += $(OBJDIR)/fbx_import_animation.o
GENERATED += $(OBJDIR)/fbx_import_mesh.o
GENERATED += $(OBJDIR)/fbx_import_nodes.o
GENERATED += $(OBJDIR)/fbx_import_skeletal_animation.o
GENERATED += $(OBJDIR)/fbx_import_skeleton.o
GENERATED += $(OBJDIR)/fbx_main.o
GENERATED += $(OBJDIR)/fbx_mesh_codec.o
GENERATED += $(OBJDIR)/fbx_precompiled.o
GENERATED += $(OBJDIR)/fbx_stream.o
GENERATED += $(OBJDIR)/fbx_utils.o
OBJECTS += $(OBJDIR)/Common.o
OBJECTS += $(OBJDIR)/NvTriStrip.o
OBJECTS += $(OBJDIR)/NvTriStripObjects.o
OBJECTS += $(OBJDIR)/fbx_export.o
OBJECTS += $(OBJDIR)/fbx_import.o
OBJECTS += $(OBJDIR)/fbx_import_animation.o
OBJECTS += $(OBJDIR)/fbx_import_mesh.o
OBJECTS += $(OBJDIR)/fbx_import_nodes.o
OBJECTS += $(OBJDIR)/fbx_import_skeletal_animation.o
OBJECTS += $(OBJDIR)/fbx_import_skeleton.o
OBJECTS += $(OBJDIR)/fbx_main.o
OBJECTS += $(OBJDIR)/fbx_mesh_codec.o
OBJECTS += $(OBJDIR)/fbx_precompiled.o
OBJECTS += $(OBJDIR)/fbx_stream.o
OBJECTS += $(OBJDIR)/fbx_utils.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Plugin_CodecMesh_FBX
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
	@echo Cleaning PK-Plugin_CodecMesh_FBX
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

$(OBJDIR)/Common.o: ../../ExternalLibs/CodecMesh_FBX/src/Common/Common.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NvTriStrip.o: ../../ExternalLibs/CodecMesh_FBX/NvTriStrip/NvTriStrip.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NvTriStripObjects.o: ../../ExternalLibs/CodecMesh_FBX/NvTriStrip/NvTriStripObjects.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_export.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_export.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_import.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_import.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_import_animation.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_import_animation.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_import_mesh.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_import_mesh.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_import_nodes.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_import_nodes.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_import_skeletal_animation.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_import_skeletal_animation.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_import_skeleton.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_import_skeleton.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_main.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_mesh_codec.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_mesh_codec.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_precompiled.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_precompiled.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_stream.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_stream.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/fbx_utils.o: ../../ExternalLibs/CodecMesh_FBX/src/fbx_utils.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif