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

ifeq ($(origin CC), default)
  CC = clang
endif
ifeq ($(origin CXX), default)
  CXX = clang++
endif
ifeq ($(origin AR), default)
  AR = ar
endif
PCH = ../../ExternalLibs/Runtime/pk_rhi/src/precompiled/rhi_precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../ExternalLibs/Runtime/pk_rhi/include -I../../ExternalLibs/Runtime/pk_rhi/src -I../../ExternalLibs/Runtime/pk_rhi/src/precompiled -I../../SDK/Samples/PK-Samples/Common -I../../ExternalLibs/GL/include
FORCE_INCLUDE +=
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

ifeq ($(config),debug_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64
TARGET = $(TARGETDIR)/libPK-RHI_d.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Debug/PK-RHI_SDK1
DEFINES += -D_DEBUG -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -m64 -target x86_64-apple-macos10.14

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64
TARGET = $(TARGETDIR)/libPK-RHI_d.a
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Debug/PK-RHI_SDK1
DEFINES += -D_DEBUG -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -target arm64-apple-macos11.0

else ifeq ($(config),release_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64
TARGET = $(TARGETDIR)/libPK-RHI_r.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Release/PK-RHI_SDK1
DEFINES += -DNDEBUG -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -m64 -target x86_64-apple-macos10.14

else ifeq ($(config),release_arm64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64
TARGET = $(TARGETDIR)/libPK-RHI_r.a
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Release/PK-RHI_SDK1
DEFINES += -DNDEBUG -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -target arm64-apple-macos11.0

else ifeq ($(config),retail_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64
TARGET = $(TARGETDIR)/libPK-RHI_s.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Retail/PK-RHI_SDK1
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -m64 -target x86_64-apple-macos10.14

else ifeq ($(config),retail_arm64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64
TARGET = $(TARGETDIR)/libPK-RHI_s.a
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Retail/PK-RHI_SDK1
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_BUILD_WITH_METAL_SUPPORT=1 -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -target arm64-apple-macos11.0

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################

PERFILE_FLAGS_0 = $(ALL_CXXFLAGS) -include pk_compiler_warnings.h

# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/glew.o
OBJECTS += $(OBJDIR)/glew.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-RHI_SDK1
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
	@echo Cleaning PK-RHI_SDK1
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

$(OBJDIR)/glew.o: ../../ExternalLibs/GL/src/glew.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
