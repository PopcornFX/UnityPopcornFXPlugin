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
TARGETDIR = ../../com.persistant-studios.popcornfx/Runtime/Plugins/x86_64
TARGET = $(TARGETDIR)/libPK-UnityPlugin.so
PCH = ../../Native/precompiled/precompiled.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../../../ExternalLibs/Runtime -I../../../ExternalLibs/Runtime/include -I../../../ExternalLibs/Runtime/include/license/CommunityLicense_Unity -I../../../ExternalLibs -I../../Native -I../../Native/precompiled -I../../Native/Common/PKFX -I../../Native/Common/Gl -I../../Native/Common/GLES -I../../../ExternalLibs/GL/include -I"$(VULKAN_SDK)/include"
FORCE_INCLUDE += -include ../../../ExternalLibs/Runtime/include/pk_linux_glibc2.19_symvers.h
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
ALL_LDFLAGS += $(LDFLAGS) -L../../../ExternalLibs/GL/lib/linux64 -L../../../ExternalLibs/Runtime/libs/freetype-2.5.5/lib/linux64 -L$$VULKAN_SDK/lib -L../../../ExternalLibs/CodecMesh_FBX/libs/linux64 -L../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64 -L/usr/lib64 -m64 -shared -Wl,-soname=libPK-UnityPlugin.so -Wl,--no-undefined
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Debug/PK-IntegrationUnity
DEFINES += -D_DEBUG -DPK_USE_RENDER_HELPERS=1 -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_D3D11_SUPPORT=0 -DPK_BUILD_WITH_D3D12_SUPPORT=0 -DPK_BUILD_WITH_GLES_SUPPORT=0 -DPK_BUILD_WITH_GLES3_SUPPORT=0 -DPK_BUILD_WITH_METAL_SUPPORT=0 -DPK_BUILD_WITH_GNM_SUPPORT=0 -DPK_BUILD_WITH_AGC_SUPPORT=0 -DPK_BUILD_WITH_GL_SUPPORT=1 -DPK_BUILD_WITH_NVN_SUPPORT=0 -DPK_PRINTF_TO_CLOG -DGLEW_STATIC -DGL_GLEXT_PROTOTYPES -DPK_UNITY_EDITOR=1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -ggdb -mfpmath=sse -pipe
LIBS += ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-AssetBakerLib_d.a -lPK-RenderHelpers_d -lPK-Plugin_CompilerBackend_CPU_VM_d -lPK-Plugin_CodecMesh_FBX_d -lPK-Plugin_CodecImage_DDS_d -lPK-Plugin_CodecImage_JPG_d -lPK-Plugin_CodecImage_PNG_d -lPK-Plugin_CodecImage_PVR_d -lPK-Plugin_CodecImage_TGA_d -lPK-Plugin_CodecImage_TIFF_d -lPK-Plugin_CodecImage_HDR_d -lPK-Plugin_CodecImage_EXR_d -lPK-ZLib_d -lPK-Plugin_CodecImage_PKM_d -lGL -lGLU -lvulkan -lfbxsdk_d -lxml2 -lfreetype -lPK-ParticlesToolbox_d -lPK-Runtime_d -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-AssetBakerLib_d.a

else ifeq ($(config),release_x64)
OBJDIR = ../intermediate/CommunityLicense_Unity/GM/x64/Release/PK-IntegrationUnity
DEFINES += -DNDEBUG -DPK_USE_RENDER_HELPERS=1 -DPK_BUILD_WITH_VULKAN_SUPPORT=1 -DPK_BUILD_WITH_D3D11_SUPPORT=0 -DPK_BUILD_WITH_D3D12_SUPPORT=0 -DPK_BUILD_WITH_GLES_SUPPORT=0 -DPK_BUILD_WITH_GLES3_SUPPORT=0 -DPK_BUILD_WITH_METAL_SUPPORT=0 -DPK_BUILD_WITH_GNM_SUPPORT=0 -DPK_BUILD_WITH_AGC_SUPPORT=0 -DPK_BUILD_WITH_GL_SUPPORT=1 -DPK_BUILD_WITH_NVN_SUPPORT=0 -DPK_PRINTF_TO_CLOG -DGLEW_STATIC -DGL_GLEXT_PROTOTYPES -DPK_UNITY_EDITOR=1
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -Wundef -fno-omit-frame-pointer -O3 -fPIC -fno-strict-aliasing -g -msse2 -Wall -Wextra -std=gnu++0x -fno-exceptions -fvisibility-inlines-hidden -fno-rtti -fvisibility=hidden -Winvalid-pch -fno-math-errno -fno-trapping-math -mfpmath=sse -pipe
LIBS += ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-AssetBakerLib_r.a -lPK-RenderHelpers_r -lPK-Plugin_CompilerBackend_CPU_VM_r -lPK-Plugin_CodecMesh_FBX_r -lPK-Plugin_CodecImage_DDS_r -lPK-Plugin_CodecImage_JPG_r -lPK-Plugin_CodecImage_PNG_r -lPK-Plugin_CodecImage_PVR_r -lPK-Plugin_CodecImage_TGA_r -lPK-Plugin_CodecImage_TIFF_r -lPK-Plugin_CodecImage_HDR_r -lPK-Plugin_CodecImage_EXR_r -lPK-ZLib_r -lPK-Plugin_CodecImage_PKM_r -lGL -lGLU -lvulkan -lfbxsdk_r -lxml2 -lfreetype -lPK-ParticlesToolbox_r -lPK-Runtime_r -lbfd -ldl -lpthread -lm -lz
LDDEPS += ../../../ExternalLibs/Runtime/bin/CommunityLicense_Unity/gmake_linux_x64/libPK-AssetBakerLib_r.a

#else
#  $(error "invalid configuration $(config)")
endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/BrowseEffect.o
GENERATED += $(OBJDIR)/EditorManagedToNative.o
GENERATED += $(OBJDIR)/EditorManager.o
GENERATED += $(OBJDIR)/EditorNativeToManaged.o
GENERATED += $(OBJDIR)/EffectBaking.o
GENERATED += $(OBJDIR)/FileWatcher.o
GENERATED += $(OBJDIR)/FrameCollectorUnityTypes.o
GENERATED += $(OBJDIR)/FxEffect.o
GENERATED += $(OBJDIR)/FxStartup.o
GENERATED += $(OBJDIR)/ILoopbackCapture.o
GENERATED += $(OBJDIR)/ManagedToNative.o
GENERATED += $(OBJDIR)/MeshBaking.o
GENERATED += $(OBJDIR)/NativeToManaged.o
GENERATED += $(OBJDIR)/PKUnity_CImageResourceManager.o
GENERATED += $(OBJDIR)/PKUnity_FileSystemController.o
GENERATED += $(OBJDIR)/PKUnity_Log.o
GENERATED += $(OBJDIR)/PKUnity_Plugins.o
GENERATED += $(OBJDIR)/PKUnity_Scene.o
GENERATED += $(OBJDIR)/PKUnity_SkinnedMesh.o
GENERATED += $(OBJDIR)/PKUnity_Tasks.o
GENERATED += $(OBJDIR)/PackFxExplorer.o
GENERATED += $(OBJDIR)/RenderAPI_DX11Data.o
GENERATED += $(OBJDIR)/RenderAPI_Data.o
GENERATED += $(OBJDIR)/RenderAPI_GLData.o
GENERATED += $(OBJDIR)/RenderAPI_VulkanData.o
GENERATED += $(OBJDIR)/RuntimeManager.o
GENERATED += $(OBJDIR)/UnityBillboardingBatchPolicy.o
GENERATED += $(OBJDIR)/UnityRenderDataFactory.o
GENERATED += $(OBJDIR)/UnityRendererCache.o
GENERATED += $(OBJDIR)/gl_utils.o
GENERATED += $(OBJDIR)/glew.o
GENERATED += $(OBJDIR)/precompiled.o
OBJECTS += $(OBJDIR)/BrowseEffect.o
OBJECTS += $(OBJDIR)/EditorManagedToNative.o
OBJECTS += $(OBJDIR)/EditorManager.o
OBJECTS += $(OBJDIR)/EditorNativeToManaged.o
OBJECTS += $(OBJDIR)/EffectBaking.o
OBJECTS += $(OBJDIR)/FileWatcher.o
OBJECTS += $(OBJDIR)/FrameCollectorUnityTypes.o
OBJECTS += $(OBJDIR)/FxEffect.o
OBJECTS += $(OBJDIR)/FxStartup.o
OBJECTS += $(OBJDIR)/ILoopbackCapture.o
OBJECTS += $(OBJDIR)/ManagedToNative.o
OBJECTS += $(OBJDIR)/MeshBaking.o
OBJECTS += $(OBJDIR)/NativeToManaged.o
OBJECTS += $(OBJDIR)/PKUnity_CImageResourceManager.o
OBJECTS += $(OBJDIR)/PKUnity_FileSystemController.o
OBJECTS += $(OBJDIR)/PKUnity_Log.o
OBJECTS += $(OBJDIR)/PKUnity_Plugins.o
OBJECTS += $(OBJDIR)/PKUnity_Scene.o
OBJECTS += $(OBJDIR)/PKUnity_SkinnedMesh.o
OBJECTS += $(OBJDIR)/PKUnity_Tasks.o
OBJECTS += $(OBJDIR)/PackFxExplorer.o
OBJECTS += $(OBJDIR)/RenderAPI_DX11Data.o
OBJECTS += $(OBJDIR)/RenderAPI_Data.o
OBJECTS += $(OBJDIR)/RenderAPI_GLData.o
OBJECTS += $(OBJDIR)/RenderAPI_VulkanData.o
OBJECTS += $(OBJDIR)/RuntimeManager.o
OBJECTS += $(OBJDIR)/UnityBillboardingBatchPolicy.o
OBJECTS += $(OBJDIR)/UnityRenderDataFactory.o
OBJECTS += $(OBJDIR)/UnityRendererCache.o
OBJECTS += $(OBJDIR)/gl_utils.o
OBJECTS += $(OBJDIR)/glew.o
OBJECTS += $(OBJDIR)/precompiled.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking PK-IntegrationUnity
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
	@echo Cleaning PK-IntegrationUnity
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) rmdir /s /q $(subst /,\\,$(GENERATED))
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

$(OBJDIR)/gl_utils.o: ../../Native/Common/Gl/gl_utils.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FxStartup.o: ../../Native/Common/PKFX/FxStartup.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/glew.o: ../../../ExternalLibs/GL/src/glew.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ILoopbackCapture.o: ../../Native/AudioApplicationLoopback/ILoopbackCapture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/BrowseEffect.o: ../../Native/EditorOnly/BrowseEffect.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/EditorManagedToNative.o: ../../Native/EditorOnly/EditorManagedToNative.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/EditorManager.o: ../../Native/EditorOnly/EditorManager.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/EditorNativeToManaged.o: ../../Native/EditorOnly/EditorNativeToManaged.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/EffectBaking.o: ../../Native/EditorOnly/EffectBaking.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FileWatcher.o: ../../Native/EditorOnly/FileWatcher.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/MeshBaking.o: ../../Native/EditorOnly/MeshBaking.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PackFxExplorer.o: ../../Native/EditorOnly/PackFxExplorer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FxEffect.o: ../../Native/FxEffect.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RenderAPI_DX11Data.o: ../../Native/ImplemGraphicsAPI/RenderAPI_DX11Data.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RenderAPI_Data.o: ../../Native/ImplemGraphicsAPI/RenderAPI_Data.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RenderAPI_GLData.o: ../../Native/ImplemGraphicsAPI/RenderAPI_GLData.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RenderAPI_VulkanData.o: ../../Native/ImplemGraphicsAPI/RenderAPI_VulkanData.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ManagedToNative.o: ../../Native/ManagedToNative.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/NativeToManaged.o: ../../Native/NativeToManaged.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PKUnity_CImageResourceManager.o: ../../Native/PKUnity_CImageResourceManager.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PKUnity_FileSystemController.o: ../../Native/PKUnity_FileSystemController.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PKUnity_Log.o: ../../Native/PKUnity_Log.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PKUnity_Plugins.o: ../../Native/PKUnity_Plugins.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PKUnity_Scene.o: ../../Native/PKUnity_Scene.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PKUnity_SkinnedMesh.o: ../../Native/PKUnity_SkinnedMesh.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/PKUnity_Tasks.o: ../../Native/PKUnity_Tasks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FrameCollectorUnityTypes.o: ../../Native/RenderingIntegration/FrameCollectorUnityTypes.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/UnityBillboardingBatchPolicy.o: ../../Native/RenderingIntegration/UnityBillboardingBatchPolicy.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/UnityRenderDataFactory.o: ../../Native/RenderingIntegration/UnityRenderDataFactory.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/UnityRendererCache.o: ../../Native/RenderingIntegration/UnityRendererCache.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/RuntimeManager.o: ../../Native/RuntimeManager.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/precompiled.o: ../../Native/precompiled/precompiled.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
