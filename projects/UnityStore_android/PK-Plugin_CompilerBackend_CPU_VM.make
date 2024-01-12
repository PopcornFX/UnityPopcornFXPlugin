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

PCH = ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/UnityStore -I../../Plugins/CompilerBackend_CPU_VM -I../../Plugins/CompilerBackend_CPU_VM/include -I../../Plugins/CompilerBackend_CPU_VM/src
FORCE_INCLUDE += -include pk_compiler_warnings.h
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

ifeq ($(config),debug_android)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android
TARGET = $(TARGETDIR)/libPK-Plugin_CompilerBackend_CPU_VM_d.a
OBJDIR = ../intermediate/UnityStore/GM/android/Debug/PK-Plugin_CompilerBackend_CPU_VM
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon -pipe
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),debug_android64)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64
TARGET = $(TARGETDIR)/libPK-Plugin_CompilerBackend_CPU_VM_d.a
OBJDIR = ../intermediate/UnityStore/GM/android64/Debug/PK-Plugin_CompilerBackend_CPU_VM
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release_android)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android
TARGET = $(TARGETDIR)/libPK-Plugin_CompilerBackend_CPU_VM_r.a
OBJDIR = ../intermediate/UnityStore/GM/android/Release/PK-Plugin_CompilerBackend_CPU_VM
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release_android64)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64
TARGET = $(TARGETDIR)/libPK-Plugin_CompilerBackend_CPU_VM_r.a
OBJDIR = ../intermediate/UnityStore/GM/android64/Release/PK-Plugin_CompilerBackend_CPU_VM
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),retail_android)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/armv7a-linux-androideabi21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android
TARGET = $(TARGETDIR)/libPK-Plugin_CompilerBackend_CPU_VM_s.a
OBJDIR = ../intermediate/UnityStore/GM/android/Retail/PK-Plugin_CompilerBackend_CPU_VM
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_LDFLAGS += $(LDFLAGS) -s

else ifeq ($(config),retail_android64)
ifeq ($(origin CC), default)
  CC = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/aarch64-linux-android21-clang++
endif
ifeq ($(origin AR), default)
  AR = $(PK_ANDROID_TOOLCHAINS)/toolchain-r21e/bin/llvm-ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/UnityStore/gmake_android64
TARGET = $(TARGETDIR)/libPK-Plugin_CompilerBackend_CPU_VM_s.a
OBJDIR = ../intermediate/UnityStore/GM/android64/Retail/PK-Plugin_CompilerBackend_CPU_VM
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_LDFLAGS += $(LDFLAGS) -s

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/cbvm_compile.o
GENERATED += $(OBJDIR)/cbvm_compile_bytecode.o
GENERATED += $(OBJDIR)/cbvm_compile_instrcombiner.o
GENERATED += $(OBJDIR)/cbvm_compile_instrscheduler.o
GENERATED += $(OBJDIR)/cbvm_compile_registers.o
GENERATED += $(OBJDIR)/cbvm_debug.o
GENERATED += $(OBJDIR)/cbvm_exec.o
GENERATED += $(OBJDIR)/cbvm_main.o
GENERATED += $(OBJDIR)/cbvm_meta_op_broadcast.o
GENERATED += $(OBJDIR)/cbvm_meta_op_call.o
GENERATED += $(OBJDIR)/cbvm_meta_op_external_clear.o
GENERATED += $(OBJDIR)/cbvm_meta_op_external_load.o
GENERATED += $(OBJDIR)/cbvm_meta_op_external_store.o
GENERATED += $(OBJDIR)/cbvm_meta_op_math_function.o
GENERATED += $(OBJDIR)/cbvm_meta_op_math_function3.o
GENERATED += $(OBJDIR)/cbvm_meta_op_math_idiv_mulinv.o
GENERATED += $(OBJDIR)/cbvm_meta_op_math_madd.o
GENERATED += $(OBJDIR)/cbvm_meta_op_math_operator.o
GENERATED += $(OBJDIR)/cbvm_meta_op_math_permute.o
GENERATED += $(OBJDIR)/cbvm_meta_op_select.o
GENERATED += $(OBJDIR)/cbvm_meta_op_type_converter.o
GENERATED += $(OBJDIR)/cbvm_meta_op_vector_constructor.o
GENERATED += $(OBJDIR)/cbvm_meta_op_vector_swizzler.o
GENERATED += $(OBJDIR)/cbvm_meta_op_vector_swizzler_codegen.o
GENERATED += $(OBJDIR)/cbvm_precompiled.o
GENERATED += $(OBJDIR)/cbvm_program.o
GENERATED += $(OBJDIR)/cbvm_registers.o
GENERATED += $(OBJDIR)/cbvm_scratch_pool.o
OBJECTS += $(OBJDIR)/cbvm_compile.o
OBJECTS += $(OBJDIR)/cbvm_compile_bytecode.o
OBJECTS += $(OBJDIR)/cbvm_compile_instrcombiner.o
OBJECTS += $(OBJDIR)/cbvm_compile_instrscheduler.o
OBJECTS += $(OBJDIR)/cbvm_compile_registers.o
OBJECTS += $(OBJDIR)/cbvm_debug.o
OBJECTS += $(OBJDIR)/cbvm_exec.o
OBJECTS += $(OBJDIR)/cbvm_main.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_broadcast.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_call.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_external_clear.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_external_load.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_external_store.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_math_function.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_math_function3.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_math_idiv_mulinv.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_math_madd.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_math_operator.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_math_permute.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_select.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_type_converter.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_vector_constructor.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_vector_swizzler.o
OBJECTS += $(OBJDIR)/cbvm_meta_op_vector_swizzler_codegen.o
OBJECTS += $(OBJDIR)/cbvm_precompiled.o
OBJECTS += $(OBJDIR)/cbvm_program.o
OBJECTS += $(OBJDIR)/cbvm_registers.o
OBJECTS += $(OBJDIR)/cbvm_scratch_pool.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-Plugin_CompilerBackend_CPU_VM
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
	@echo Cleaning PK-Plugin_CompilerBackend_CPU_VM
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

$(OBJDIR)/cbvm_precompiled.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_precompiled.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_compile.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_compile.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_compile_bytecode.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_compile_bytecode.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_compile_instrcombiner.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_compile_instrcombiner.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_compile_instrscheduler.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_compile_instrscheduler.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_compile_registers.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_compile_registers.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_debug.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_debug.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_exec.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_exec.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_main.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_broadcast.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_broadcast.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_call.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_call.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_external_clear.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_external_clear.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_external_load.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_external_load.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_external_store.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_external_store.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_math_function.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_math_function.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_math_function3.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_math_function3.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_math_idiv_mulinv.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_math_idiv_mulinv.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_math_madd.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_math_madd.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_math_operator.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_math_operator.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_math_permute.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_math_permute.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_select.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_select.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_type_converter.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_type_converter.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_vector_constructor.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_vector_constructor.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_vector_swizzler.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_vector_swizzler.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_meta_op_vector_swizzler_codegen.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_meta_op_vector_swizzler_codegen.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_program.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_program.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_registers.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_registers.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cbvm_scratch_pool.o: ../../Plugins/CompilerBackend_CPU_VM/src/cbvm_scratch_pool.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
