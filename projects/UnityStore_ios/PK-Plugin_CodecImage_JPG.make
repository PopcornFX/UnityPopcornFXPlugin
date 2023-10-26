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
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../Plugins/CodecImage_JPG -I../../Plugins/CodecImage_JPG/include
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

ifeq ($(config),debug_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_JPG_d.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Debug/PK-Plugin_CodecImage_JPG
DEFINES += -D_DEBUG -DNO_GETENV
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),release_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_JPG_r.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Release/PK-Plugin_CodecImage_JPG
DEFINES += -DNDEBUG -DNO_GETENV
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),retail_ios64)
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_ios64
TARGET = $(TARGETDIR)/libPK-Plugin_CodecImage_JPG_s.a
OBJDIR = ../intermediate/UnityStore/GM/ios64/Retail/PK-Plugin_CodecImage_JPG
DEFINES += -DNDEBUG -DPK_RETAIL -DNO_GETENV
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################

PERFILE_FLAGS_0 = $(ALL_CXXFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path`
PERFILE_FLAGS_1 = $(ALL_CFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wno-unused-parameter
PERFILE_FLAGS_2 = $(ALL_CXXFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wno-unused-parameter

# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/jaricom.o
GENERATED += $(OBJDIR)/jcapimin.o
GENERATED += $(OBJDIR)/jcapistd.o
GENERATED += $(OBJDIR)/jcarith.o
GENERATED += $(OBJDIR)/jccoefct.o
GENERATED += $(OBJDIR)/jccolor.o
GENERATED += $(OBJDIR)/jcdctmgr.o
GENERATED += $(OBJDIR)/jchuff.o
GENERATED += $(OBJDIR)/jcinit.o
GENERATED += $(OBJDIR)/jcmainct.o
GENERATED += $(OBJDIR)/jcmarker.o
GENERATED += $(OBJDIR)/jcmaster.o
GENERATED += $(OBJDIR)/jcomapi.o
GENERATED += $(OBJDIR)/jcparam.o
GENERATED += $(OBJDIR)/jcprepct.o
GENERATED += $(OBJDIR)/jcsample.o
GENERATED += $(OBJDIR)/jctrans.o
GENERATED += $(OBJDIR)/jdapimin.o
GENERATED += $(OBJDIR)/jdapistd.o
GENERATED += $(OBJDIR)/jdarith.o
GENERATED += $(OBJDIR)/jdatadst.o
GENERATED += $(OBJDIR)/jdatasrc.o
GENERATED += $(OBJDIR)/jdcoefct.o
GENERATED += $(OBJDIR)/jdcolor.o
GENERATED += $(OBJDIR)/jddctmgr.o
GENERATED += $(OBJDIR)/jdhuff.o
GENERATED += $(OBJDIR)/jdinput.o
GENERATED += $(OBJDIR)/jdmainct.o
GENERATED += $(OBJDIR)/jdmarker.o
GENERATED += $(OBJDIR)/jdmaster.o
GENERATED += $(OBJDIR)/jdmerge.o
GENERATED += $(OBJDIR)/jdpostct.o
GENERATED += $(OBJDIR)/jdsample.o
GENERATED += $(OBJDIR)/jdtrans.o
GENERATED += $(OBJDIR)/jerror.o
GENERATED += $(OBJDIR)/jfdctflt.o
GENERATED += $(OBJDIR)/jfdctfst.o
GENERATED += $(OBJDIR)/jfdctint.o
GENERATED += $(OBJDIR)/jidctflt.o
GENERATED += $(OBJDIR)/jidctfst.o
GENERATED += $(OBJDIR)/jidctint.o
GENERATED += $(OBJDIR)/jmemansi.o
GENERATED += $(OBJDIR)/jmemmgr.o
GENERATED += $(OBJDIR)/jpeg_jlwrapper.o
GENERATED += $(OBJDIR)/jpeg_load.o
GENERATED += $(OBJDIR)/jpeg_main.o
GENERATED += $(OBJDIR)/jpeg_save.o
GENERATED += $(OBJDIR)/jquant1.o
GENERATED += $(OBJDIR)/jquant2.o
GENERATED += $(OBJDIR)/jutils.o
OBJECTS += $(OBJDIR)/jaricom.o
OBJECTS += $(OBJDIR)/jcapimin.o
OBJECTS += $(OBJDIR)/jcapistd.o
OBJECTS += $(OBJDIR)/jcarith.o
OBJECTS += $(OBJDIR)/jccoefct.o
OBJECTS += $(OBJDIR)/jccolor.o
OBJECTS += $(OBJDIR)/jcdctmgr.o
OBJECTS += $(OBJDIR)/jchuff.o
OBJECTS += $(OBJDIR)/jcinit.o
OBJECTS += $(OBJDIR)/jcmainct.o
OBJECTS += $(OBJDIR)/jcmarker.o
OBJECTS += $(OBJDIR)/jcmaster.o
OBJECTS += $(OBJDIR)/jcomapi.o
OBJECTS += $(OBJDIR)/jcparam.o
OBJECTS += $(OBJDIR)/jcprepct.o
OBJECTS += $(OBJDIR)/jcsample.o
OBJECTS += $(OBJDIR)/jctrans.o
OBJECTS += $(OBJDIR)/jdapimin.o
OBJECTS += $(OBJDIR)/jdapistd.o
OBJECTS += $(OBJDIR)/jdarith.o
OBJECTS += $(OBJDIR)/jdatadst.o
OBJECTS += $(OBJDIR)/jdatasrc.o
OBJECTS += $(OBJDIR)/jdcoefct.o
OBJECTS += $(OBJDIR)/jdcolor.o
OBJECTS += $(OBJDIR)/jddctmgr.o
OBJECTS += $(OBJDIR)/jdhuff.o
OBJECTS += $(OBJDIR)/jdinput.o
OBJECTS += $(OBJDIR)/jdmainct.o
OBJECTS += $(OBJDIR)/jdmarker.o
OBJECTS += $(OBJDIR)/jdmaster.o
OBJECTS += $(OBJDIR)/jdmerge.o
OBJECTS += $(OBJDIR)/jdpostct.o
OBJECTS += $(OBJDIR)/jdsample.o
OBJECTS += $(OBJDIR)/jdtrans.o
OBJECTS += $(OBJDIR)/jerror.o
OBJECTS += $(OBJDIR)/jfdctflt.o
OBJECTS += $(OBJDIR)/jfdctfst.o
OBJECTS += $(OBJDIR)/jfdctint.o
OBJECTS += $(OBJDIR)/jidctflt.o
OBJECTS += $(OBJDIR)/jidctfst.o
OBJECTS += $(OBJDIR)/jidctint.o
OBJECTS += $(OBJDIR)/jmemansi.o
OBJECTS += $(OBJDIR)/jmemmgr.o
OBJECTS += $(OBJDIR)/jpeg_jlwrapper.o
OBJECTS += $(OBJDIR)/jpeg_load.o
OBJECTS += $(OBJDIR)/jpeg_main.o
OBJECTS += $(OBJDIR)/jpeg_save.o
OBJECTS += $(OBJDIR)/jquant1.o
OBJECTS += $(OBJDIR)/jquant2.o
OBJECTS += $(OBJDIR)/jutils.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Plugin_CodecImage_JPG
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
	@echo Cleaning PK-Plugin_CodecImage_JPG
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

$(OBJDIR)/jaricom.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jaricom.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcapimin.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcapimin.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcapistd.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcapistd.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcarith.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcarith.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jccoefct.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jccoefct.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jccolor.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jccolor.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcdctmgr.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcdctmgr.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jchuff.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jchuff.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcinit.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcinit.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcmainct.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcmainct.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcmarker.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcmarker.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcmaster.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcmaster.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcomapi.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcomapi.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcparam.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcparam.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcprepct.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcprepct.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jcsample.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jcsample.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jctrans.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jctrans.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdapimin.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdapimin.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdapistd.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdapistd.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdarith.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdarith.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdatadst.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdatadst.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdatasrc.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdatasrc.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdcoefct.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdcoefct.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdcolor.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdcolor.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jddctmgr.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jddctmgr.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdhuff.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdhuff.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdinput.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdinput.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdmainct.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdmainct.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdmarker.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdmarker.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdmaster.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdmaster.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdmerge.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdmerge.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdpostct.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdpostct.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdsample.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdsample.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jdtrans.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jdtrans.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jerror.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jerror.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jfdctflt.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jfdctflt.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jfdctfst.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jfdctfst.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jfdctint.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jfdctint.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jidctflt.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jidctflt.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jidctfst.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jidctfst.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jidctint.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jidctint.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jmemansi.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jmemansi.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jmemmgr.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jmemmgr.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jquant1.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jquant1.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jquant2.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jquant2.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jutils.o: ../../Plugins/CodecImage_JPG/jpeg-9b/jutils.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(PERFILE_FLAGS_1) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jpeg_jlwrapper.o: ../../Plugins/CodecImage_JPG/jpeg_jlwrapper.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jpeg_load.o: ../../Plugins/CodecImage_JPG/jpeg_load.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jpeg_main.o: ../../Plugins/CodecImage_JPG/jpeg_main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jpeg_save.o: ../../Plugins/CodecImage_JPG/jpeg_save.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
