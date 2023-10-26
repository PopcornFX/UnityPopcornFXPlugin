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
INCLUDES += -I../../ExternalLibs/Runtime/libs/zlib-1.2.8
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
TARGET = $(TARGETDIR)/libPK-ZLib_d.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Debug/PK-ZLib
DEFINES += -D_DEBUG -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -Wno-unused-parameter -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -Wno-unused-parameter -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),release_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-ZLib_r.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Release/PK-ZLib
DEFINES += -DNDEBUG -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -Wno-unused-parameter -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -Wno-unused-parameter -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),retail_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-ZLib_s.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Retail/PK-ZLib
DEFINES += -DNDEBUG -DPK_RETAIL -DZ_PREFIX -DZ_PREFIX_CUSTOM=pk_z_
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -Wno-unused-parameter -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -Wno-unused-parameter -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################

PERFILE_FLAGS_0 = $(ALL_CXXFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path`
PERFILE_FLAGS_1 = $(ALL_CFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path`

# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/adler32.o
GENERATED += $(OBJDIR)/compress.o
GENERATED += $(OBJDIR)/crc32.o
GENERATED += $(OBJDIR)/deflate.o
GENERATED += $(OBJDIR)/gzclose.o
GENERATED += $(OBJDIR)/gzlib.o
GENERATED += $(OBJDIR)/gzread.o
GENERATED += $(OBJDIR)/gzwrite.o
GENERATED += $(OBJDIR)/infback.o
GENERATED += $(OBJDIR)/inffast.o
GENERATED += $(OBJDIR)/inflate.o
GENERATED += $(OBJDIR)/inftrees.o
GENERATED += $(OBJDIR)/trees.o
GENERATED += $(OBJDIR)/uncompr.o
GENERATED += $(OBJDIR)/zutil.o
OBJECTS += $(OBJDIR)/adler32.o
OBJECTS += $(OBJDIR)/compress.o
OBJECTS += $(OBJDIR)/crc32.o
OBJECTS += $(OBJDIR)/deflate.o
OBJECTS += $(OBJDIR)/gzclose.o
OBJECTS += $(OBJDIR)/gzlib.o
OBJECTS += $(OBJDIR)/gzread.o
OBJECTS += $(OBJDIR)/gzwrite.o
OBJECTS += $(OBJDIR)/infback.o
OBJECTS += $(OBJDIR)/inffast.o
OBJECTS += $(OBJDIR)/inflate.o
OBJECTS += $(OBJDIR)/inftrees.o
OBJECTS += $(OBJDIR)/trees.o
OBJECTS += $(OBJDIR)/uncompr.o
OBJECTS += $(OBJDIR)/zutil.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-ZLib
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
	@echo Cleaning PK-ZLib
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

$(OBJDIR)/adler32.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/adler32.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/compress.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/compress.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/crc32.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/crc32.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/deflate.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/deflate.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/gzclose.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/gzclose.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/gzlib.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/gzlib.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/gzread.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/gzread.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/gzwrite.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/gzwrite.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/infback.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/infback.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/inffast.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/inffast.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/inflate.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/inflate.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/inftrees.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/inftrees.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/trees.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/trees.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/uncompr.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/uncompr.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/zutil.o: ../../ExternalLibs/Runtime/libs/zlib-1.2.8/zutil.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
