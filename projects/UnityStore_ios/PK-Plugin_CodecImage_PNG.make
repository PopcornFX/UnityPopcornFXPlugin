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
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../Plugins/CodecImage_PNG -I../../Plugins/CodecImage_PNG/include -I../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed -I../../ExternalLibs/Runtime/libs/zlib-1.2.8
FORCE_INCLUDE +=
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

ifeq ($(config),debug_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_PNG_d.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Debug/PK-Plugin_CodecImage_PNG
DEFINES += -D_DEBUG -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),release_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_PNG_r.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Release/PK-Plugin_CodecImage_PNG
DEFINES += -DNDEBUG -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),retail_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_PNG_s.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Retail/PK-Plugin_CodecImage_PNG
DEFINES += -DNDEBUG -DPK_RETAIL -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################

PERFILE_FLAGS_0 = $(ALL_CXXFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -include pk_compiler_warnings.h
PERFILE_FLAGS_1 = $(ALL_CXXFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wno-unused-parameter
PERFILE_FLAGS_2 = $(ALL_CFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wno-unused-parameter

# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/arm_init.o
GENERATED += $(OBJDIR)/filter_neon.o
GENERATED += $(OBJDIR)/filter_neon_intrinsics.o
GENERATED += $(OBJDIR)/palette_neon_intrinsics.o
GENERATED += $(OBJDIR)/png.o
GENERATED += $(OBJDIR)/png_load.o
GENERATED += $(OBJDIR)/png_main.o
GENERATED += $(OBJDIR)/png_save.o
GENERATED += $(OBJDIR)/pngerror.o
GENERATED += $(OBJDIR)/pngget.o
GENERATED += $(OBJDIR)/pngmem.o
GENERATED += $(OBJDIR)/pngpread.o
GENERATED += $(OBJDIR)/pngread.o
GENERATED += $(OBJDIR)/pngrio.o
GENERATED += $(OBJDIR)/pngrtran.o
GENERATED += $(OBJDIR)/pngrutil.o
GENERATED += $(OBJDIR)/pngset.o
GENERATED += $(OBJDIR)/pngtrans.o
GENERATED += $(OBJDIR)/pngwio.o
GENERATED += $(OBJDIR)/pngwrite.o
GENERATED += $(OBJDIR)/pngwtran.o
GENERATED += $(OBJDIR)/pngwutil.o
OBJECTS += $(OBJDIR)/arm_init.o
OBJECTS += $(OBJDIR)/filter_neon.o
OBJECTS += $(OBJDIR)/filter_neon_intrinsics.o
OBJECTS += $(OBJDIR)/palette_neon_intrinsics.o
OBJECTS += $(OBJDIR)/png.o
OBJECTS += $(OBJDIR)/png_load.o
OBJECTS += $(OBJDIR)/png_main.o
OBJECTS += $(OBJDIR)/png_save.o
OBJECTS += $(OBJDIR)/pngerror.o
OBJECTS += $(OBJDIR)/pngget.o
OBJECTS += $(OBJDIR)/pngmem.o
OBJECTS += $(OBJDIR)/pngpread.o
OBJECTS += $(OBJDIR)/pngread.o
OBJECTS += $(OBJDIR)/pngrio.o
OBJECTS += $(OBJDIR)/pngrtran.o
OBJECTS += $(OBJDIR)/pngrutil.o
OBJECTS += $(OBJDIR)/pngset.o
OBJECTS += $(OBJDIR)/pngtrans.o
OBJECTS += $(OBJDIR)/pngwio.o
OBJECTS += $(OBJDIR)/pngwrite.o
OBJECTS += $(OBJDIR)/pngwtran.o
OBJECTS += $(OBJDIR)/pngwutil.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Plugin_CodecImage_PNG
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
	@echo Cleaning PK-Plugin_CodecImage_PNG
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
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) -x c++-header -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
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

$(OBJDIR)/arm_init.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/arm/arm_init.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/filter_neon.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/arm/filter_neon.S
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/filter_neon_intrinsics.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/arm/filter_neon_intrinsics.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/palette_neon_intrinsics.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/arm/palette_neon_intrinsics.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/png.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/png.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngerror.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngerror.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngget.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngget.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngmem.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngmem.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngpread.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngpread.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngread.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngread.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngrio.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngrio.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngrtran.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngrtran.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngrutil.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngrutil.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngset.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngset.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngtrans.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngtrans.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngwio.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngwio.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngwrite.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngwrite.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngwtran.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngwtran.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pngwutil.o: ../../Plugins/CodecImage_PNG/libpng-1.6.37-prefixed/pngwutil.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/png_load.o: ../../Plugins/CodecImage_PNG/png_load.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/png_main.o: ../../Plugins/CodecImage_PNG/png_main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/png_save.o: ../../Plugins/CodecImage_PNG/png_save.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
