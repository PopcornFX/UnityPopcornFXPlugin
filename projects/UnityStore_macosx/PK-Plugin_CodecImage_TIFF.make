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
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../Plugins/CodecImage_TIFF -I../../Plugins/CodecImage_TIFF/include -I../../ExternalLibs/Runtime/pk_kernel/include -I../../ExternalLibs/Runtime/pk_imaging/include -I../../Plugins/CodecImage_TIFF/tiff-4.0.7 -I../../ExternalLibs/Runtime/libs/zlib-1.2.8
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
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_TIFF_d.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Debug/PK-Plugin_CodecImage_TIFF
DEFINES += -D_DEBUG -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -m64 -target x86_64-apple-macos10.14

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_TIFF_d.a
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Debug/PK-Plugin_CodecImage_TIFF
DEFINES += -D_DEBUG -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -Wno-macro-redefined -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -Wno-macro-redefined -fno-math-errno -fno-trapping-math -ggdb -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -target arm64-apple-macos11.0

else ifeq ($(config),release_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_TIFF_r.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Release/PK-Plugin_CodecImage_TIFF
DEFINES += -DNDEBUG -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -m64 -target x86_64-apple-macos10.14

else ifeq ($(config),release_arm64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_TIFF_r.a
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Release/PK-Plugin_CodecImage_TIFF
DEFINES += -DNDEBUG -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -Wno-macro-redefined -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -g -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -Wno-macro-redefined -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -target arm64-apple-macos11.0

else ifeq ($(config),retail_x64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_x64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_TIFF_s.a
OBJDIR = ../intermediate/UnityStore/GM/x64/Retail/PK-Plugin_CodecImage_TIFF
DEFINES += -DNDEBUG -DPK_RETAIL -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -mfpmath=sse -target x86_64-apple-macos10.14 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -m64 -target x86_64-apple-macos10.14

else ifeq ($(config),retail_arm64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_macosx_ARM64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_TIFF_s.a
OBJDIR = ../intermediate/UnityStore/GM/ARM64/Retail/PK-Plugin_CodecImage_TIFF
DEFINES += -DNDEBUG -DPK_RETAIL -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -Wno-macro-redefined -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -Wno-pragma-pack -Wno-macro-redefined -fno-math-errno -fno-trapping-math -target arm64-apple-macos11.0 -iwithsysroot `xcrun --show-sdk-path`
ALL_LDFLAGS += $(LDFLAGS) -target arm64-apple-macos11.0

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################

PERFILE_FLAGS_0 = $(ALL_CXXFLAGS) -include pk_compiler_warnings.h
PERFILE_FLAGS_1 = $(ALL_CXXFLAGS) -Wno-unused-parameter -Wno-format
PERFILE_FLAGS_2 = $(ALL_CFLAGS) -Wno-unused-parameter -Wno-format

# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/tif_aux.o
GENERATED += $(OBJDIR)/tif_close.o
GENERATED += $(OBJDIR)/tif_codec.o
GENERATED += $(OBJDIR)/tif_color.o
GENERATED += $(OBJDIR)/tif_compress.o
GENERATED += $(OBJDIR)/tif_dir.o
GENERATED += $(OBJDIR)/tif_dirinfo.o
GENERATED += $(OBJDIR)/tif_dirread.o
GENERATED += $(OBJDIR)/tif_dirwrite.o
GENERATED += $(OBJDIR)/tif_dumpmode.o
GENERATED += $(OBJDIR)/tif_error.o
GENERATED += $(OBJDIR)/tif_extension.o
GENERATED += $(OBJDIR)/tif_fax3.o
GENERATED += $(OBJDIR)/tif_fax3sm.o
GENERATED += $(OBJDIR)/tif_flush.o
GENERATED += $(OBJDIR)/tif_getimage.o
GENERATED += $(OBJDIR)/tif_jbig.o
GENERATED += $(OBJDIR)/tif_jpeg.o
GENERATED += $(OBJDIR)/tif_jpeg_12.o
GENERATED += $(OBJDIR)/tif_luv.o
GENERATED += $(OBJDIR)/tif_lzma.o
GENERATED += $(OBJDIR)/tif_lzw.o
GENERATED += $(OBJDIR)/tif_next.o
GENERATED += $(OBJDIR)/tif_ojpeg.o
GENERATED += $(OBJDIR)/tif_open.o
GENERATED += $(OBJDIR)/tif_packbits.o
GENERATED += $(OBJDIR)/tif_pixarlog.o
GENERATED += $(OBJDIR)/tif_predict.o
GENERATED += $(OBJDIR)/tif_print.o
GENERATED += $(OBJDIR)/tif_read.o
GENERATED += $(OBJDIR)/tif_strip.o
GENERATED += $(OBJDIR)/tif_swab.o
GENERATED += $(OBJDIR)/tif_thunder.o
GENERATED += $(OBJDIR)/tif_tile.o
GENERATED += $(OBJDIR)/tif_unix.o
GENERATED += $(OBJDIR)/tif_version.o
GENERATED += $(OBJDIR)/tif_warning.o
GENERATED += $(OBJDIR)/tif_write.o
GENERATED += $(OBJDIR)/tif_zip.o
GENERATED += $(OBJDIR)/tiff_helpers.o
GENERATED += $(OBJDIR)/tiff_load.o
GENERATED += $(OBJDIR)/tiff_main.o
GENERATED += $(OBJDIR)/tiff_save.o
GENERATED += $(OBJDIR)/tiff_wrapper.o
OBJECTS += $(OBJDIR)/tif_aux.o
OBJECTS += $(OBJDIR)/tif_close.o
OBJECTS += $(OBJDIR)/tif_codec.o
OBJECTS += $(OBJDIR)/tif_color.o
OBJECTS += $(OBJDIR)/tif_compress.o
OBJECTS += $(OBJDIR)/tif_dir.o
OBJECTS += $(OBJDIR)/tif_dirinfo.o
OBJECTS += $(OBJDIR)/tif_dirread.o
OBJECTS += $(OBJDIR)/tif_dirwrite.o
OBJECTS += $(OBJDIR)/tif_dumpmode.o
OBJECTS += $(OBJDIR)/tif_error.o
OBJECTS += $(OBJDIR)/tif_extension.o
OBJECTS += $(OBJDIR)/tif_fax3.o
OBJECTS += $(OBJDIR)/tif_fax3sm.o
OBJECTS += $(OBJDIR)/tif_flush.o
OBJECTS += $(OBJDIR)/tif_getimage.o
OBJECTS += $(OBJDIR)/tif_jbig.o
OBJECTS += $(OBJDIR)/tif_jpeg.o
OBJECTS += $(OBJDIR)/tif_jpeg_12.o
OBJECTS += $(OBJDIR)/tif_luv.o
OBJECTS += $(OBJDIR)/tif_lzma.o
OBJECTS += $(OBJDIR)/tif_lzw.o
OBJECTS += $(OBJDIR)/tif_next.o
OBJECTS += $(OBJDIR)/tif_ojpeg.o
OBJECTS += $(OBJDIR)/tif_open.o
OBJECTS += $(OBJDIR)/tif_packbits.o
OBJECTS += $(OBJDIR)/tif_pixarlog.o
OBJECTS += $(OBJDIR)/tif_predict.o
OBJECTS += $(OBJDIR)/tif_print.o
OBJECTS += $(OBJDIR)/tif_read.o
OBJECTS += $(OBJDIR)/tif_strip.o
OBJECTS += $(OBJDIR)/tif_swab.o
OBJECTS += $(OBJDIR)/tif_thunder.o
OBJECTS += $(OBJDIR)/tif_tile.o
OBJECTS += $(OBJDIR)/tif_unix.o
OBJECTS += $(OBJDIR)/tif_version.o
OBJECTS += $(OBJDIR)/tif_warning.o
OBJECTS += $(OBJDIR)/tif_write.o
OBJECTS += $(OBJDIR)/tif_zip.o
OBJECTS += $(OBJDIR)/tiff_helpers.o
OBJECTS += $(OBJDIR)/tiff_load.o
OBJECTS += $(OBJDIR)/tiff_main.o
OBJECTS += $(OBJDIR)/tiff_save.o
OBJECTS += $(OBJDIR)/tiff_wrapper.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Plugin_CodecImage_TIFF
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
	@echo Cleaning PK-Plugin_CodecImage_TIFF
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

$(OBJDIR)/tif_aux.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_aux.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_close.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_close.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_codec.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_codec.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_color.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_color.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_compress.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_compress.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_dir.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_dir.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_dirinfo.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_dirinfo.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_dirread.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_dirread.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_dirwrite.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_dirwrite.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_dumpmode.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_dumpmode.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_error.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_error.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_extension.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_extension.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_fax3.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_fax3.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_fax3sm.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_fax3sm.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_flush.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_flush.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_getimage.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_getimage.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_jbig.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_jbig.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_jpeg.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_jpeg.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_jpeg_12.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_jpeg_12.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_luv.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_luv.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_lzma.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_lzma.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_lzw.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_lzw.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_next.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_next.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_ojpeg.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_ojpeg.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_open.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_open.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_packbits.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_packbits.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_pixarlog.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_pixarlog.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_predict.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_predict.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_print.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_print.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_read.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_read.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_strip.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_strip.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_swab.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_swab.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_thunder.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_thunder.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_tile.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_tile.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_unix.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_unix.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_version.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_version.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_warning.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_warning.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_write.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_write.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tif_zip.o: ../../Plugins/CodecImage_TIFF/tiff-4.0.7/tif_zip.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_2) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tiff_helpers.o: ../../Plugins/CodecImage_TIFF/tiff_helpers.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tiff_load.o: ../../Plugins/CodecImage_TIFF/tiff_load.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tiff_main.o: ../../Plugins/CodecImage_TIFF/tiff_main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tiff_save.o: ../../Plugins/CodecImage_TIFF/tiff_save.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tiff_wrapper.o: ../../Plugins/CodecImage_TIFF/tiff_wrapper.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
