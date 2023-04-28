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

PCH = ../../SDK/Samples/PK-Samples/precompiled/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../SDK/Samples/PK-Samples -I../../SDK/Samples/PK-Samples/precompiled -I../../SDK/Samples/PK-Samples/PK-SampleLib -I../../../ExternalLibs/GL/include -I../../SDK/Samples/External/imgui
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
  CC = $(EASYPATH)/arm-linux-androideabi-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH)/arm-linux-androideabi-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH)/arm-linux-androideabi-ar
endif
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android
TARGET = $(TARGETDIR)/libPK-SampleLib_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android/Debug/PK-SampleLib
DEFINES += -D_DEBUG -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_SDL=0 -DPK_BUILD_WITH_FMODEX_SUPPORT=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpu=neon -pipe
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),debug_android64)
ifeq ($(origin CC), default)
  CC = $(EASYPATH64)/aarch64-linux-android-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH64)/aarch64-linux-android-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH64)/aarch64-linux-android-ar
endif
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android64
TARGET = $(TARGETDIR)/libPK-SampleLib_d.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android64/Debug/PK-SampleLib
DEFINES += -D_DEBUG -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_SDL=0 -DPK_BUILD_WITH_FMODEX_SUPPORT=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O2 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -pipe
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release_android)
ifeq ($(origin CC), default)
  CC = $(EASYPATH)/arm-linux-androideabi-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH)/arm-linux-androideabi-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH)/arm-linux-androideabi-ar
endif
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android
TARGET = $(TARGETDIR)/libPK-SampleLib_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android/Release/PK-SampleLib
DEFINES += -DNDEBUG -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_SDL=0 -DPK_BUILD_WITH_FMODEX_SUPPORT=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release_android64)
ifeq ($(origin CC), default)
  CC = $(EASYPATH64)/aarch64-linux-android-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH64)/aarch64-linux-android-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH64)/aarch64-linux-android-ar
endif
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android64
TARGET = $(TARGETDIR)/libPK-SampleLib_r.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android64/Release/PK-SampleLib
DEFINES += -DNDEBUG -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_SDL=0 -DPK_BUILD_WITH_FMODEX_SUPPORT=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS64) -march=armv8-a -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -pipe
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),retail_android)
ifeq ($(origin CC), default)
  CC = $(EASYPATH)/arm-linux-androideabi-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH)/arm-linux-androideabi-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH)/arm-linux-androideabi-ar
endif
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android
TARGET = $(TARGETDIR)/libPK-SampleLib_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android/Retail/PK-SampleLib
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_SDL=0 -DPK_BUILD_WITH_FMODEX_SUPPORT=0
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -MP -pipe -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) $(ADDITIONAL_FLAGS) -march=armv7-a -mfloat-abi=softfp -Wshadow -Wundef -fomit-frame-pointer -O3 -fPIC -fno-strict-aliasing -fno-unsigned-char -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpu=neon -pipe
ALL_LDFLAGS += $(LDFLAGS) -s

else ifeq ($(config),retail_android64)
ifeq ($(origin CC), default)
  CC = $(EASYPATH64)/aarch64-linux-android-clang
endif
ifeq ($(origin CXX), default)
  CXX = $(EASYPATH64)/aarch64-linux-android-clang++
endif
ifeq ($(origin AR), default)
  AR = $(EASYPATH64)/aarch64-linux-android-ar
endif
TARGETDIR = ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_android64
TARGET = $(TARGETDIR)/libPK-SampleLib_s.a
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/android64/Retail/PK-SampleLib
DEFINES += -DNDEBUG -DPK_RETAIL -DPK_BUILD_WITH_OGL_SUPPORT=1 -DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DGLEW_NO_GLU -DGLEW_EGL -DPK_BUILD_WITH_SDL=0 -DPK_BUILD_WITH_FMODEX_SUPPORT=0
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

GENERATED += $(OBJDIR)/AWindowContext.o
GENERATED += $(OBJDIR)/AbstractGraphicScene.o
GENERATED += $(OBJDIR)/BRDFLUT.o
GENERATED += $(OBJDIR)/BasicSceneShaderDefinitions.o
GENERATED += $(OBJDIR)/BlueNoise.o
GENERATED += $(OBJDIR)/Camera.o
GENERATED += $(OBJDIR)/D3D11Context.o
GENERATED += $(OBJDIR)/D3D12Context.o
GENERATED += $(OBJDIR)/DebugHelper.o
GENERATED += $(OBJDIR)/DeferredScene.o
GENERATED += $(OBJDIR)/DirectionalShadows.o
GENERATED += $(OBJDIR)/DownSampleTexture.o
GENERATED += $(OBJDIR)/EGLContext.o
GENERATED += $(OBJDIR)/EnvironmentMapEntity.o
GENERATED += $(OBJDIR)/FeatureRenderingSettings.o
GENERATED += $(OBJDIR)/FrameCollector.o
GENERATED += $(OBJDIR)/GBuffer.o
GENERATED += $(OBJDIR)/GLContext.o
GENERATED += $(OBJDIR)/GLSLShaderGenerator.o
GENERATED += $(OBJDIR)/GLXContext.o
GENERATED += $(OBJDIR)/Gizmo.o
GENERATED += $(OBJDIR)/HLSLShaderGenerator.o
GENERATED += $(OBJDIR)/ImguiRhiImplem.o
GENERATED += $(OBJDIR)/LightEntity.o
GENERATED += $(OBJDIR)/MaterialToRHI.o
GENERATED += $(OBJDIR)/MeshEntity.o
GENERATED += $(OBJDIR)/MetalShaderGenerator.o
GENERATED += $(OBJDIR)/OffscreenContext.o
GENERATED += $(OBJDIR)/PKPix.o
GENERATED += $(OBJDIR)/PKSampleInit.o
GENERATED += $(OBJDIR)/ParticleShaderGenerator.o
GENERATED += $(OBJDIR)/PipelineCacheHelper.o
GENERATED += $(OBJDIR)/PopEdShaderDefinitions.o
GENERATED += $(OBJDIR)/PopcornStartup.o
GENERATED += $(OBJDIR)/PostFxBloom.o
GENERATED += $(OBJDIR)/PostFxDistortion.o
GENERATED += $(OBJDIR)/PostFxFXAA.o
GENERATED += $(OBJDIR)/PostFxToneMapping.o
GENERATED += $(OBJDIR)/ProfilerRenderer.o
GENERATED += $(OBJDIR)/RHIBillboardingBatchPolicy.o
GENERATED += $(OBJDIR)/RHIBillboardingBatchPolicy_Vertex.o
GENERATED += $(OBJDIR)/RHICustomTasks.o
GENERATED += $(OBJDIR)/RHIGPUSorter.o
GENERATED += $(OBJDIR)/RHIGraphicResources.o
GENERATED += $(OBJDIR)/RHIParticleRenderDataFactory.o
GENERATED += $(OBJDIR)/RHIRenderParticleSceneHelpers.o
GENERATED += $(OBJDIR)/RendererCache.o
GENERATED += $(OBJDIR)/SampleLibShaderDefinitions.o
GENERATED += $(OBJDIR)/SampleUtils.o
GENERATED += $(OBJDIR)/SdlContext.o
GENERATED += $(OBJDIR)/ShaderDefinitions.o
GENERATED += $(OBJDIR)/ShaderGenerator.o
GENERATED += $(OBJDIR)/ShaderLoader.o
GENERATED += $(OBJDIR)/SimInterface_GBufferSampling.o
GENERATED += $(OBJDIR)/SoundPoolCache.o
GENERATED += $(OBJDIR)/UnitTestsShaderDefinitions.o
GENERATED += $(OBJDIR)/VulkanContext.o
GENERATED += $(OBJDIR)/VulkanShaderGenerator.o
GENERATED += $(OBJDIR)/WGLContext.o
GENERATED += $(OBJDIR)/imgui.o
GENERATED += $(OBJDIR)/imgui_demo.o
GENERATED += $(OBJDIR)/imgui_draw.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/AWindowContext.o
OBJECTS += $(OBJDIR)/AbstractGraphicScene.o
OBJECTS += $(OBJDIR)/BRDFLUT.o
OBJECTS += $(OBJDIR)/BasicSceneShaderDefinitions.o
OBJECTS += $(OBJDIR)/BlueNoise.o
OBJECTS += $(OBJDIR)/Camera.o
OBJECTS += $(OBJDIR)/D3D11Context.o
OBJECTS += $(OBJDIR)/D3D12Context.o
OBJECTS += $(OBJDIR)/DebugHelper.o
OBJECTS += $(OBJDIR)/DeferredScene.o
OBJECTS += $(OBJDIR)/DirectionalShadows.o
OBJECTS += $(OBJDIR)/DownSampleTexture.o
OBJECTS += $(OBJDIR)/EGLContext.o
OBJECTS += $(OBJDIR)/EnvironmentMapEntity.o
OBJECTS += $(OBJDIR)/FeatureRenderingSettings.o
OBJECTS += $(OBJDIR)/FrameCollector.o
OBJECTS += $(OBJDIR)/GBuffer.o
OBJECTS += $(OBJDIR)/GLContext.o
OBJECTS += $(OBJDIR)/GLSLShaderGenerator.o
OBJECTS += $(OBJDIR)/GLXContext.o
OBJECTS += $(OBJDIR)/Gizmo.o
OBJECTS += $(OBJDIR)/HLSLShaderGenerator.o
OBJECTS += $(OBJDIR)/ImguiRhiImplem.o
OBJECTS += $(OBJDIR)/LightEntity.o
OBJECTS += $(OBJDIR)/MaterialToRHI.o
OBJECTS += $(OBJDIR)/MeshEntity.o
OBJECTS += $(OBJDIR)/MetalShaderGenerator.o
OBJECTS += $(OBJDIR)/OffscreenContext.o
OBJECTS += $(OBJDIR)/PKPix.o
OBJECTS += $(OBJDIR)/PKSampleInit.o
OBJECTS += $(OBJDIR)/ParticleShaderGenerator.o
OBJECTS += $(OBJDIR)/PipelineCacheHelper.o
OBJECTS += $(OBJDIR)/PopEdShaderDefinitions.o
OBJECTS += $(OBJDIR)/PopcornStartup.o
OBJECTS += $(OBJDIR)/PostFxBloom.o
OBJECTS += $(OBJDIR)/PostFxDistortion.o
OBJECTS += $(OBJDIR)/PostFxFXAA.o
OBJECTS += $(OBJDIR)/PostFxToneMapping.o
OBJECTS += $(OBJDIR)/ProfilerRenderer.o
OBJECTS += $(OBJDIR)/RHIBillboardingBatchPolicy.o
OBJECTS += $(OBJDIR)/RHIBillboardingBatchPolicy_Vertex.o
OBJECTS += $(OBJDIR)/RHICustomTasks.o
OBJECTS += $(OBJDIR)/RHIGPUSorter.o
OBJECTS += $(OBJDIR)/RHIGraphicResources.o
OBJECTS += $(OBJDIR)/RHIParticleRenderDataFactory.o
OBJECTS += $(OBJDIR)/RHIRenderParticleSceneHelpers.o
OBJECTS += $(OBJDIR)/RendererCache.o
OBJECTS += $(OBJDIR)/SampleLibShaderDefinitions.o
OBJECTS += $(OBJDIR)/SampleUtils.o
OBJECTS += $(OBJDIR)/SdlContext.o
OBJECTS += $(OBJDIR)/ShaderDefinitions.o
OBJECTS += $(OBJDIR)/ShaderGenerator.o
OBJECTS += $(OBJDIR)/ShaderLoader.o
OBJECTS += $(OBJDIR)/SimInterface_GBufferSampling.o
OBJECTS += $(OBJDIR)/SoundPoolCache.o
OBJECTS += $(OBJDIR)/UnitTestsShaderDefinitions.o
OBJECTS += $(OBJDIR)/VulkanContext.o
OBJECTS += $(OBJDIR)/VulkanShaderGenerator.o
OBJECTS += $(OBJDIR)/WGLContext.o
OBJECTS += $(OBJDIR)/imgui.o
OBJECTS += $(OBJDIR)/imgui_demo.o
OBJECTS += $(OBJDIR)/imgui_draw.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-SampleLib
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
	@echo Cleaning PK-SampleLib
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

$(OBJDIR)/precompiled.o: ../../SDK/Samples/PK-Samples/precompiled/precompiled.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D11Context.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ApiContext/D3D/D3D11Context.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/D3D12Context.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ApiContext/D3D/D3D12Context.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/EGLContext.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ApiContext/OpenGL/EGLContext.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLContext.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ApiContext/OpenGL/GLContext.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLXContext.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ApiContext/OpenGL/GLXContext.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/WGLContext.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ApiContext/OpenGL/WGLContext.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanContext.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ApiContext/Vulkan/VulkanContext.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/BRDFLUT.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/BRDFLUT.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/BlueNoise.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/BlueNoise.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Camera.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/Camera.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/DebugHelper.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/DebugHelper.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Gizmo.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/Gizmo.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ImguiRhiImplem.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ImguiRhiImplem.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PKPix.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/PKPix.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PKSampleInit.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/PKSampleInit.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PipelineCacheHelper.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/PipelineCacheHelper.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PopcornStartup.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/PopcornStartup/PopcornStartup.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ProfilerRenderer.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ProfilerRenderer.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RHIRenderParticleSceneHelpers.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RHIRenderParticleSceneHelpers.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FeatureRenderingSettings.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/FeatureRenderingSettings.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FrameCollector.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/FrameCollector.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/MaterialToRHI.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/MaterialToRHI.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RHIBillboardingBatchPolicy.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/RHIBillboardingBatchPolicy.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RHIBillboardingBatchPolicy_Vertex.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/RHIBillboardingBatchPolicy_Vertex.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RHICustomTasks.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/RHICustomTasks.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RHIGPUSorter.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/RHIGPUSorter.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RHIGraphicResources.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/RHIGraphicResources.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RHIParticleRenderDataFactory.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/RHIParticleRenderDataFactory.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RendererCache.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/RendererCache.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SoundPoolCache.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderIntegrationRHI/SoundPoolCache.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/DirectionalShadows.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderPasses/DirectionalShadows.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/DownSampleTexture.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderPasses/DownSampleTexture.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GBuffer.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderPasses/GBuffer.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PostFxBloom.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderPasses/PostFxBloom.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PostFxDistortion.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderPasses/PostFxDistortion.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PostFxFXAA.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderPasses/PostFxFXAA.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PostFxToneMapping.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/RenderPasses/PostFxToneMapping.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AbstractGraphicScene.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/SampleScene/AbstractGraphicScene.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/DeferredScene.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/SampleScene/DeferredScene.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/EnvironmentMapEntity.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/SampleScene/Entities/EnvironmentMapEntity.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/LightEntity.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/SampleScene/Entities/LightEntity.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/MeshEntity.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/SampleScene/Entities/MeshEntity.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SampleUtils.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/SampleUtils.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/BasicSceneShaderDefinitions.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderDefinitions/BasicSceneShaderDefinitions.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PopEdShaderDefinitions.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderDefinitions/PopEdShaderDefinitions.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SampleLibShaderDefinitions.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderDefinitions/SampleLibShaderDefinitions.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ShaderDefinitions.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderDefinitions/ShaderDefinitions.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/UnitTestsShaderDefinitions.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderDefinitions/UnitTestsShaderDefinitions.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GLSLShaderGenerator.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderGenerator/GLSLShaderGenerator.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/HLSLShaderGenerator.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderGenerator/HLSLShaderGenerator.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/MetalShaderGenerator.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderGenerator/MetalShaderGenerator.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ParticleShaderGenerator.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderGenerator/ParticleShaderGenerator.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ShaderGenerator.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderGenerator/ShaderGenerator.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/VulkanShaderGenerator.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderGenerator/VulkanShaderGenerator.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ShaderLoader.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/ShaderLoader.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SimInterface_GBufferSampling.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/SimInterfaces/SimInterface_GBufferSampling.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/AWindowContext.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/WindowContext/AWindowContext.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OffscreenContext.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/WindowContext/OffscreenContext/OffscreenContext.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SdlContext.o: ../../SDK/Samples/PK-Samples/PK-SampleLib/WindowContext/SdlContext/SdlContext.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui.o: ../../SDK/Samples/External/imgui/imgui.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui_demo.o: ../../SDK/Samples/External/imgui/imgui_demo.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/imgui_draw.o: ../../SDK/Samples/External/imgui/imgui_draw.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
