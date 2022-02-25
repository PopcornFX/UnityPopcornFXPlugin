# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_ios
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

PCH = ../../ExternalLibs/Runtime/pk_rhi/src/precompiled/rhi_precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../ExternalLibs/Runtime -I../../ExternalLibs/Runtime/include -I../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../ExternalLibs/Runtime/pk_rhi/include -I../../ExternalLibs/Runtime/pk_rhi/src -I../../ExternalLibs/Runtime/pk_rhi/src/precompiled -I../../SDK/Samples/PK-Samples/Common
FORCE_INCLUDE +=
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

ifeq ($(config),debug_ios)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch armv7
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch armv7
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios
TARGET = $(TARGETDIR)/libPK-RHI_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Debug/PK-RHI
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),debug_ios64)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch arm64
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch arm64
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64
TARGET = $(TARGETDIR)/libPK-RHI_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Debug/PK-RHI
DEFINES += -D_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -O2 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -ggdb -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),release_ios)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch armv7
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch armv7
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios
TARGET = $(TARGETDIR)/libPK-RHI_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Release/PK-RHI
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),release_ios64)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch arm64
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch arm64
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64
TARGET = $(TARGETDIR)/libPK-RHI_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Release/PK-RHI
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),retail_ios)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch armv7
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch armv7
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios
TARGET = $(TARGETDIR)/libPK-RHI_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios/Retail/PK-RHI
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -mcpu=cortex-a8 -marm -march=armv7-a -Wshadow -Wundef -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

else ifeq ($(config),retail_ios64)
ifeq ($(origin CC), default)
  CC = xcrun --sdk iphoneos clang -arch arm64
endif
ifeq ($(origin CXX), default)
  CXX = xcrun --sdk iphoneos clang++ -arch arm64
endif
ifeq ($(origin AR), default)
  AR = xcrun --sdk iphoneos ar
endif
TARGETDIR = ../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_ios64
TARGET = $(TARGETDIR)/libPK-RHI_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/ios64/Retail/PK-RHI
DEFINES += -DNDEBUG -DPK_RETAIL
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -arch arm64 -Wshadow -Wundef -fomit-frame-pointer -O3 -fno-strict-aliasing -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -Wall -Wextra -std=gnu++0x -fno-exceptions -fno-rtti -Winvalid-pch -Wno-pragma-pack -fno-math-errno -fno-trapping-math -miphoneos-version-min=8.0 -fembed-bitcode

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################

PERFILE_FLAGS_0 = $(ALL_CXXFLAGS) -pipe -isysroot `xcrun --sdk iphoneos --show-sdk-path` -include pk_compiler_warnings.h

# File sets
# #############################################

OBJECTS :=

OBJECTS += $(OBJDIR)/AbstractCommandBuffer.o
OBJECTS += $(OBJDIR)/AbstractConstantSet.o
OBJECTS += $(OBJDIR)/AbstractFrameBuffer.o
OBJECTS += $(OBJDIR)/AbstractGpuBuffer.o
OBJECTS += $(OBJDIR)/AbstractRenderPass.o
OBJECTS += $(OBJDIR)/AbstractRenderTarget.o
OBJECTS += $(OBJDIR)/AbstractTexture.o
OBJECTS += $(OBJDIR)/D3D11ApiManager.o
OBJECTS += $(OBJDIR)/D3D11CommandBuffer.o
OBJECTS += $(OBJDIR)/D3D11ComputeState.o
OBJECTS += $(OBJDIR)/D3D11ConstantSampler.o
OBJECTS += $(OBJDIR)/D3D11ConstantSet.o
OBJECTS += $(OBJDIR)/D3D11FrameBuffer.o
OBJECTS += $(OBJDIR)/D3D11GpuBuffer.o
OBJECTS += $(OBJDIR)/D3D11PopcornEnumConversion.o
OBJECTS += $(OBJDIR)/D3D11QueryPool.o
OBJECTS += $(OBJDIR)/D3D11RHI.o
OBJECTS += $(OBJDIR)/D3D11ReadBackTexture.o
OBJECTS += $(OBJDIR)/D3D11RenderPass.o
OBJECTS += $(OBJDIR)/D3D11RenderState.o
OBJECTS += $(OBJDIR)/D3D11RenderTarget.o
OBJECTS += $(OBJDIR)/D3D11ShaderModule.o
OBJECTS += $(OBJDIR)/D3D11ShaderProgram.o
OBJECTS += $(OBJDIR)/D3D11Texture.o
OBJECTS += $(OBJDIR)/D3D12ApiManager.o
OBJECTS += $(OBJDIR)/D3D12CommandBuffer.o
OBJECTS += $(OBJDIR)/D3D12ComputeState.o
OBJECTS += $(OBJDIR)/D3D12ConstantSampler.o
OBJECTS += $(OBJDIR)/D3D12ConstantSet.o
OBJECTS += $(OBJDIR)/D3D12DescriptorAllocator.o
OBJECTS += $(OBJDIR)/D3D12Fence.o
OBJECTS += $(OBJDIR)/D3D12FrameBuffer.o
OBJECTS += $(OBJDIR)/D3D12GpuBuffer.o
OBJECTS += $(OBJDIR)/D3D12PopcornEnumConversion.o
OBJECTS += $(OBJDIR)/D3D12QueryPool.o
OBJECTS += $(OBJDIR)/D3D12RHI.o
OBJECTS += $(OBJDIR)/D3D12ReadBackTexture.o
OBJECTS += $(OBJDIR)/D3D12RenderPass.o
OBJECTS += $(OBJDIR)/D3D12RenderState.o
OBJECTS += $(OBJDIR)/D3D12RenderTarget.o
OBJECTS += $(OBJDIR)/D3D12ShaderModule.o
OBJECTS += $(OBJDIR)/D3D12ShaderProgram.o
OBJECTS += $(OBJDIR)/D3D12Texture.o
OBJECTS += $(OBJDIR)/D3DCompileShader.o
OBJECTS += $(OBJDIR)/D3DPopcornEnumConversion.o
OBJECTS += $(OBJDIR)/EnumHelper.o
OBJECTS += $(OBJDIR)/IFrameBuffer.o
OBJECTS += $(OBJDIR)/IGpuBuffer.o
OBJECTS += $(OBJDIR)/IReadBackTexture.o
OBJECTS += $(OBJDIR)/IRenderPass.o
OBJECTS += $(OBJDIR)/IRenderTarget.o
OBJECTS += $(OBJDIR)/NullApiManager.o
OBJECTS += $(OBJDIR)/NullCommandBuffer.o
OBJECTS += $(OBJDIR)/NullConstantSampler.o
OBJECTS += $(OBJDIR)/NullConstantSet.o
OBJECTS += $(OBJDIR)/NullFrameBuffer.o
OBJECTS += $(OBJDIR)/NullGpuBuffer.o
OBJECTS += $(OBJDIR)/NullReadBackTexture.o
OBJECTS += $(OBJDIR)/NullRenderPass.o
OBJECTS += $(OBJDIR)/NullRenderState.o
OBJECTS += $(OBJDIR)/NullRenderTarget.o
OBJECTS += $(OBJDIR)/NullShaderModule.o
OBJECTS += $(OBJDIR)/NullShaderProgram.o
OBJECTS += $(OBJDIR)/NullTexture.o
OBJECTS += $(OBJDIR)/OpenGLApiManager.o
OBJECTS += $(OBJDIR)/OpenGLCommandBuffer.o
OBJECTS += $(OBJDIR)/OpenGLComputeState.o
OBJECTS += $(OBJDIR)/OpenGLConstantSampler.o
OBJECTS += $(OBJDIR)/OpenGLConstantSet.o
OBJECTS += $(OBJDIR)/OpenGLFrameBuffer.o
OBJECTS += $(OBJDIR)/OpenGLGpuBuffer.o
OBJECTS += $(OBJDIR)/OpenGLPopcornEnumConversion.o
OBJECTS += $(OBJDIR)/OpenGLQueryPool.o
OBJECTS += $(OBJDIR)/OpenGLRHI.o
OBJECTS += $(OBJDIR)/OpenGLReadBackTexture.o
OBJECTS += $(OBJDIR)/OpenGLRenderPass.o
OBJECTS += $(OBJDIR)/OpenGLRenderState.o
OBJECTS += $(OBJDIR)/OpenGLRenderTarget.o
OBJECTS += $(OBJDIR)/OpenGLShaderModule.o
OBJECTS += $(OBJDIR)/OpenGLShaderProgram.o
OBJECTS += $(OBJDIR)/OpenGLTexture.o
OBJECTS += $(OBJDIR)/PixelFormatFallbacks.o
OBJECTS += $(OBJDIR)/RHIInit.o
OBJECTS += $(OBJDIR)/SConstantSetLayout.o
OBJECTS += $(OBJDIR)/SShaderBindings.o
OBJECTS += $(OBJDIR)/ShaderConstantBindingGenerator.o
OBJECTS += $(OBJDIR)/Startup.o
OBJECTS += $(OBJDIR)/VulkanApiManager.o
OBJECTS += $(OBJDIR)/VulkanCommandBuffer.o
OBJECTS += $(OBJDIR)/VulkanComputeState.o
OBJECTS += $(OBJDIR)/VulkanConstantSampler.o
OBJECTS += $(OBJDIR)/VulkanConstantSet.o
OBJECTS += $(OBJDIR)/VulkanFrameBuffer.o
OBJECTS += $(OBJDIR)/VulkanGpuBuffer.o
OBJECTS += $(OBJDIR)/VulkanPopcornEnumConversion.o
OBJECTS += $(OBJDIR)/VulkanQueryPool.o
OBJECTS += $(OBJDIR)/VulkanRHI.o
OBJECTS += $(OBJDIR)/VulkanReadBackTexture.o
OBJECTS += $(OBJDIR)/VulkanRenderPass.o
OBJECTS += $(OBJDIR)/VulkanRenderState.o
OBJECTS += $(OBJDIR)/VulkanRenderTarget.o
OBJECTS += $(OBJDIR)/VulkanShaderModule.o
OBJECTS += $(OBJDIR)/VulkanShaderProgram.o
OBJECTS += $(OBJDIR)/VulkanTexture.o
OBJECTS += $(OBJDIR)/rhi_precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-RHI
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
	@echo Cleaning PK-RHI
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

$(OBJDIR)/rhi_precompiled.o: ../../ExternalLibs/Runtime/pk_rhi/src/precompiled/rhi_precompiled.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11ApiManager.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11ApiManager.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11CommandBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11CommandBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11ComputeState.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11ComputeState.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11ConstantSampler.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11ConstantSampler.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11ConstantSet.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11ConstantSet.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11FrameBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11FrameBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11GpuBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11GpuBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11PopcornEnumConversion.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11PopcornEnumConversion.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11QueryPool.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11QueryPool.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11RHI.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11RHI.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11ReadBackTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11ReadBackTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11RenderPass.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11RenderPass.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11RenderState.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11RenderState.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11RenderTarget.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11RenderTarget.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11ShaderModule.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11ShaderModule.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11ShaderProgram.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11ShaderProgram.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11Texture.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D11/D3D11Texture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12ApiManager.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12ApiManager.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12CommandBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12CommandBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12ComputeState.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12ComputeState.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12ConstantSampler.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12ConstantSampler.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12ConstantSet.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12ConstantSet.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12DescriptorAllocator.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12DescriptorAllocator.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12Fence.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12Fence.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12FrameBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12FrameBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12GpuBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12GpuBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12PopcornEnumConversion.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12PopcornEnumConversion.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12QueryPool.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12QueryPool.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12RHI.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12RHI.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12ReadBackTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12ReadBackTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12RenderPass.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12RenderPass.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12RenderState.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12RenderState.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12RenderTarget.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12RenderTarget.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12ShaderModule.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12ShaderModule.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12ShaderProgram.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12ShaderProgram.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12Texture.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3D12/D3D12Texture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3DCompileShader.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3DCommon/D3DCompileShader.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3DPopcornEnumConversion.o: ../../ExternalLibs/Runtime/pk_rhi/src/D3DCommon/D3DPopcornEnumConversion.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/EnumHelper.o: ../../ExternalLibs/Runtime/pk_rhi/src/EnumHelper.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PixelFormatFallbacks.o: ../../ExternalLibs/Runtime/pk_rhi/src/PixelFormatFallbacks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RHIInit.o: ../../ExternalLibs/Runtime/pk_rhi/src/RHIInit.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ShaderConstantBindingGenerator.o: ../../ExternalLibs/Runtime/pk_rhi/src/ShaderConstantBindingGenerator.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Startup.o: ../../ExternalLibs/Runtime/pk_rhi/src/Startup.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/IFrameBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/IFrameBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/IGpuBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/IGpuBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/IReadBackTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/IReadBackTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/IRenderPass.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/IRenderPass.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/IRenderTarget.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/IRenderTarget.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SConstantSetLayout.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/SConstantSetLayout.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SShaderBindings.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/SShaderBindings.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AbstractCommandBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/abstracts/AbstractCommandBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AbstractConstantSet.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/abstracts/AbstractConstantSet.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AbstractFrameBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/abstracts/AbstractFrameBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AbstractGpuBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/abstracts/AbstractGpuBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AbstractRenderPass.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/abstracts/AbstractRenderPass.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AbstractRenderTarget.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/abstracts/AbstractRenderTarget.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AbstractTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/interfaces/abstracts/AbstractTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullApiManager.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullApiManager.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullCommandBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullCommandBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullConstantSampler.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullConstantSampler.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullConstantSet.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullConstantSet.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullFrameBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullFrameBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullGpuBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullGpuBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullReadBackTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullReadBackTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullRenderPass.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullRenderPass.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullRenderState.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullRenderState.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullRenderTarget.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullRenderTarget.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullShaderModule.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullShaderModule.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullShaderProgram.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullShaderProgram.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NullTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/null/NullTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLApiManager.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLApiManager.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLCommandBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLCommandBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLComputeState.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLComputeState.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLConstantSampler.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLConstantSampler.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLConstantSet.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLConstantSet.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLFrameBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLFrameBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLGpuBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLGpuBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLPopcornEnumConversion.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLPopcornEnumConversion.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLQueryPool.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLQueryPool.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLRHI.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLRHI.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLReadBackTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLReadBackTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLRenderPass.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLRenderPass.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLRenderState.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLRenderState.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLRenderTarget.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLRenderTarget.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLShaderModule.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLShaderModule.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLShaderProgram.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLShaderProgram.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OpenGLTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/opengl/OpenGLTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanApiManager.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanApiManager.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanCommandBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanCommandBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanComputeState.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanComputeState.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanConstantSampler.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanConstantSampler.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanConstantSet.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanConstantSet.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanFrameBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanFrameBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanGpuBuffer.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanGpuBuffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanPopcornEnumConversion.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanPopcornEnumConversion.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanQueryPool.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanQueryPool.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanRHI.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanRHI.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanReadBackTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanReadBackTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanRenderPass.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanRenderPass.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanRenderState.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanRenderState.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanRenderTarget.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanRenderTarget.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanShaderModule.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanShaderModule.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanShaderProgram.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanShaderProgram.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanTexture.o: ../../ExternalLibs/Runtime/pk_rhi/src/vulkan/VulkanTexture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(PERFILE_FLAGS_0) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
