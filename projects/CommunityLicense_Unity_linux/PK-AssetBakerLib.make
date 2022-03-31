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
PCH = ../../ExternalLibs/PK-AssetBakerLib/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../ExternalLibs/PK-AssetBakerLib -I../../ExternalLibs/Runtime/libs/freetype-2.5.5/include
FORCE_INCLUDE += -include ../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h -include pk_compiler_warnings.h
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
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

ifeq ($(config),debug_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64
TARGET = $(TARGETDIR)/libPK-AssetBakerLib_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Debug/PK-AssetBakerLib
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -pipe
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),release_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64
TARGET = $(TARGETDIR)/libPK-AssetBakerLib_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Release/PK-AssetBakerLib
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),retail_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64
TARGET = $(TARGETDIR)/libPK-AssetBakerLib_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Retail/PK-AssetBakerLib
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

OBJECTS :=

OBJECTS += $(OBJDIR)/AssetBaker_Cookery.o
OBJECTS += $(OBJDIR)/AssetBaker_Oven.o
OBJECTS += $(OBJDIR)/AssetBaker_Oven_Font.o
OBJECTS += $(OBJDIR)/AssetBaker_Oven_HBO.o
OBJECTS += $(OBJDIR)/AssetBaker_Oven_HBO_AttributeHotfix.o
OBJECTS += $(OBJDIR)/AssetBaker_Oven_Mesh.o
OBJECTS += $(OBJDIR)/AssetBaker_Oven_StraightCopy.o
OBJECTS += $(OBJDIR)/AssetBaker_Oven_Texture.o
OBJECTS += $(OBJDIR)/AssetBaker_Oven_VectorField.o
OBJECTS += $(OBJDIR)/AssetBaker_Startup.o
OBJECTS += $(OBJDIR)/ConvexHull.o
OBJECTS += $(OBJDIR)/TextureTrimmer.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-AssetBakerLib
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
	@echo Cleaning PK-AssetBakerLib
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

$(OBJDIR)/AssetBaker_Cookery.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Cookery.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_Oven.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Oven.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_Oven_Font.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Oven_Font.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_Oven_HBO.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Oven_HBO.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_Oven_HBO_AttributeHotfix.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Oven_HBO_AttributeHotfix.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_Oven_Mesh.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Oven_Mesh.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_Oven_StraightCopy.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Oven_StraightCopy.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_Oven_Texture.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Oven_Texture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_Oven_VectorField.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Oven_VectorField.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AssetBaker_Startup.o: ../../ExternalLibs/PK-AssetBakerLib/AssetBaker_Startup.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ConvexHull.o: ../../ExternalLibs/PK-AssetBakerLib/ConvexHull.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/TextureTrimmer.o: ../../ExternalLibs/PK-AssetBakerLib/TextureTrimmer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/precompiled.o: ../../ExternalLibs/PK-AssetBakerLib/precompiled.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
