# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_x64)
  PK_Runtime_config = debug_x64
  PK_Discretizers_config = debug_x64
  PK_ParticlesToolbox_config = debug_x64
  PK_Plugin_CompilerBackend_CPU_VM_config = debug_x64
  PK_Plugin_CodecImage_DDS_config = debug_x64
  PK_Plugin_CodecImage_JPG_config = debug_x64
  PK_Plugin_CodecImage_PNG_config = debug_x64
  PK_Plugin_CodecImage_TGA_config = debug_x64
  PK_Plugin_CodecImage_TIFF_config = debug_x64
  PK_Plugin_CodecImage_PKM_config = debug_x64
  PK_Plugin_CodecImage_PVR_config = debug_x64
  PK_Plugin_CodecImage_HDR_config = debug_x64
  PK_Plugin_CodecImage_EXR_config = debug_x64
  PK_Plugin_CodecMesh_FBX_config = debug_x64
  PK_ZLib_config = debug_x64
  PK_AssetBakerLib_config = debug_x64
  PK_AssetBaker_config = debug_x64
  PK_UpgraderLib_config = debug_x64
  PK_Upgrader_config = debug_x64
  PK_Sample_01_BasicStartup_config = debug_x64
  PK_Sample_02_BasicEvolve_config = debug_x64
  PK_Sample_03_EngineHooks_config = debug_x64
  PK_Sample_04_Baking_config = debug_x64
  PK_Sample_05_Upgrader_config = debug_x64
  PK_RenderHelpers_config = debug_x64
  PK_RHI_config = debug_x64
  PK_SampleLib_config = debug_x64
  PK_ShaderTool_config = debug_x64
  PK_MCPP_config = debug_x64
  PK_Sample_01_BasicRendering_config = debug_x64
  PK_Sample_02_FullIntegration_config = debug_x64
  PK_Sample_04_EffectInterface_config = debug_x64
  PK_Sample_05_Stats_config = debug_x64
  PK_Sample_06_SimInterface_config = debug_x64
  PK_Sample_06_SimInterfaceGPU_config = debug_x64
  PK_Sample_07_LOD_config = debug_x64
  PK_Sample_08_CustomCollision_config = debug_x64
  PK_Sample_09_AsyncLoading_config = debug_x64
  PK_Sample_10_AsyncRendering_config = debug_x64
  PK_Sample_11_ThreadPool_config = debug_x64
  PK_Sample_12_GBufferSampling_config = debug_x64

else ifeq ($(config),debug_arm64)
  PK_Runtime_config = debug_arm64
  PK_Discretizers_config = debug_arm64
  PK_ParticlesToolbox_config = debug_arm64
  PK_Plugin_CompilerBackend_CPU_VM_config = debug_arm64
  PK_Plugin_CodecImage_DDS_config = debug_arm64
  PK_Plugin_CodecImage_JPG_config = debug_arm64
  PK_Plugin_CodecImage_PNG_config = debug_arm64
  PK_Plugin_CodecImage_TGA_config = debug_arm64
  PK_Plugin_CodecImage_TIFF_config = debug_arm64
  PK_Plugin_CodecImage_PKM_config = debug_arm64
  PK_Plugin_CodecImage_PVR_config = debug_arm64
  PK_Plugin_CodecImage_HDR_config = debug_arm64
  PK_Plugin_CodecImage_EXR_config = debug_arm64
  PK_Plugin_CodecMesh_FBX_config = debug_arm64
  PK_ZLib_config = debug_arm64
  PK_AssetBakerLib_config = debug_arm64
  PK_AssetBaker_config = debug_arm64
  PK_UpgraderLib_config = debug_arm64
  PK_Upgrader_config = debug_arm64
  PK_Sample_01_BasicStartup_config = debug_arm64
  PK_Sample_02_BasicEvolve_config = debug_arm64
  PK_Sample_03_EngineHooks_config = debug_arm64
  PK_Sample_04_Baking_config = debug_arm64
  PK_Sample_05_Upgrader_config = debug_arm64
  PK_RenderHelpers_config = debug_arm64
  PK_RHI_config = debug_arm64
  PK_SampleLib_config = debug_arm64
  PK_ShaderTool_config = debug_arm64
  PK_MCPP_config = debug_arm64
  PK_Sample_01_BasicRendering_config = debug_arm64
  PK_Sample_02_FullIntegration_config = debug_arm64
  PK_Sample_04_EffectInterface_config = debug_arm64
  PK_Sample_05_Stats_config = debug_arm64
  PK_Sample_06_SimInterface_config = debug_arm64
  PK_Sample_06_SimInterfaceGPU_config = debug_arm64
  PK_Sample_07_LOD_config = debug_arm64
  PK_Sample_08_CustomCollision_config = debug_arm64
  PK_Sample_09_AsyncLoading_config = debug_arm64
  PK_Sample_10_AsyncRendering_config = debug_arm64
  PK_Sample_11_ThreadPool_config = debug_arm64
  PK_Sample_12_GBufferSampling_config = debug_arm64

else ifeq ($(config),release_x64)
  PK_Runtime_config = release_x64
  PK_Discretizers_config = release_x64
  PK_ParticlesToolbox_config = release_x64
  PK_Plugin_CompilerBackend_CPU_VM_config = release_x64
  PK_Plugin_CodecImage_DDS_config = release_x64
  PK_Plugin_CodecImage_JPG_config = release_x64
  PK_Plugin_CodecImage_PNG_config = release_x64
  PK_Plugin_CodecImage_TGA_config = release_x64
  PK_Plugin_CodecImage_TIFF_config = release_x64
  PK_Plugin_CodecImage_PKM_config = release_x64
  PK_Plugin_CodecImage_PVR_config = release_x64
  PK_Plugin_CodecImage_HDR_config = release_x64
  PK_Plugin_CodecImage_EXR_config = release_x64
  PK_Plugin_CodecMesh_FBX_config = release_x64
  PK_ZLib_config = release_x64
  PK_AssetBakerLib_config = release_x64
  PK_AssetBaker_config = release_x64
  PK_UpgraderLib_config = release_x64
  PK_Upgrader_config = release_x64
  PK_Sample_01_BasicStartup_config = release_x64
  PK_Sample_02_BasicEvolve_config = release_x64
  PK_Sample_03_EngineHooks_config = release_x64
  PK_Sample_04_Baking_config = release_x64
  PK_Sample_05_Upgrader_config = release_x64
  PK_RenderHelpers_config = release_x64
  PK_RHI_config = release_x64
  PK_SampleLib_config = release_x64
  PK_ShaderTool_config = release_x64
  PK_MCPP_config = release_x64
  PK_Sample_01_BasicRendering_config = release_x64
  PK_Sample_02_FullIntegration_config = release_x64
  PK_Sample_04_EffectInterface_config = release_x64
  PK_Sample_05_Stats_config = release_x64
  PK_Sample_06_SimInterface_config = release_x64
  PK_Sample_06_SimInterfaceGPU_config = release_x64
  PK_Sample_07_LOD_config = release_x64
  PK_Sample_08_CustomCollision_config = release_x64
  PK_Sample_09_AsyncLoading_config = release_x64
  PK_Sample_10_AsyncRendering_config = release_x64
  PK_Sample_11_ThreadPool_config = release_x64
  PK_Sample_12_GBufferSampling_config = release_x64

else ifeq ($(config),release_arm64)
  PK_Runtime_config = release_arm64
  PK_Discretizers_config = release_arm64
  PK_ParticlesToolbox_config = release_arm64
  PK_Plugin_CompilerBackend_CPU_VM_config = release_arm64
  PK_Plugin_CodecImage_DDS_config = release_arm64
  PK_Plugin_CodecImage_JPG_config = release_arm64
  PK_Plugin_CodecImage_PNG_config = release_arm64
  PK_Plugin_CodecImage_TGA_config = release_arm64
  PK_Plugin_CodecImage_TIFF_config = release_arm64
  PK_Plugin_CodecImage_PKM_config = release_arm64
  PK_Plugin_CodecImage_PVR_config = release_arm64
  PK_Plugin_CodecImage_HDR_config = release_arm64
  PK_Plugin_CodecImage_EXR_config = release_arm64
  PK_Plugin_CodecMesh_FBX_config = release_arm64
  PK_ZLib_config = release_arm64
  PK_AssetBakerLib_config = release_arm64
  PK_AssetBaker_config = release_arm64
  PK_UpgraderLib_config = release_arm64
  PK_Upgrader_config = release_arm64
  PK_Sample_01_BasicStartup_config = release_arm64
  PK_Sample_02_BasicEvolve_config = release_arm64
  PK_Sample_03_EngineHooks_config = release_arm64
  PK_Sample_04_Baking_config = release_arm64
  PK_Sample_05_Upgrader_config = release_arm64
  PK_RenderHelpers_config = release_arm64
  PK_RHI_config = release_arm64
  PK_SampleLib_config = release_arm64
  PK_ShaderTool_config = release_arm64
  PK_MCPP_config = release_arm64
  PK_Sample_01_BasicRendering_config = release_arm64
  PK_Sample_02_FullIntegration_config = release_arm64
  PK_Sample_04_EffectInterface_config = release_arm64
  PK_Sample_05_Stats_config = release_arm64
  PK_Sample_06_SimInterface_config = release_arm64
  PK_Sample_06_SimInterfaceGPU_config = release_arm64
  PK_Sample_07_LOD_config = release_arm64
  PK_Sample_08_CustomCollision_config = release_arm64
  PK_Sample_09_AsyncLoading_config = release_arm64
  PK_Sample_10_AsyncRendering_config = release_arm64
  PK_Sample_11_ThreadPool_config = release_arm64
  PK_Sample_12_GBufferSampling_config = release_arm64

else ifeq ($(config),retail_x64)
  PK_Runtime_config = retail_x64
  PK_Discretizers_config = retail_x64
  PK_ParticlesToolbox_config = retail_x64
  PK_Plugin_CompilerBackend_CPU_VM_config = retail_x64
  PK_Plugin_CodecImage_DDS_config = retail_x64
  PK_Plugin_CodecImage_JPG_config = retail_x64
  PK_Plugin_CodecImage_PNG_config = retail_x64
  PK_Plugin_CodecImage_TGA_config = retail_x64
  PK_Plugin_CodecImage_TIFF_config = retail_x64
  PK_Plugin_CodecImage_PKM_config = retail_x64
  PK_Plugin_CodecImage_PVR_config = retail_x64
  PK_Plugin_CodecImage_HDR_config = retail_x64
  PK_Plugin_CodecImage_EXR_config = retail_x64
  PK_Plugin_CodecMesh_FBX_config = retail_x64
  PK_ZLib_config = retail_x64
  PK_AssetBakerLib_config = retail_x64
  PK_UpgraderLib_config = retail_x64
  PK_Upgrader_config = retail_x64
  PK_Sample_01_BasicStartup_config = retail_x64
  PK_Sample_02_BasicEvolve_config = retail_x64
  PK_Sample_03_EngineHooks_config = retail_x64
  PK_Sample_04_Baking_config = retail_x64
  PK_Sample_05_Upgrader_config = retail_x64
  PK_RenderHelpers_config = retail_x64
  PK_RHI_config = retail_x64
  PK_SampleLib_config = retail_x64
  PK_MCPP_config = retail_x64
  PK_Sample_01_BasicRendering_config = retail_x64
  PK_Sample_02_FullIntegration_config = retail_x64
  PK_Sample_04_EffectInterface_config = retail_x64
  PK_Sample_05_Stats_config = retail_x64
  PK_Sample_06_SimInterface_config = retail_x64
  PK_Sample_06_SimInterfaceGPU_config = retail_x64
  PK_Sample_07_LOD_config = retail_x64
  PK_Sample_08_CustomCollision_config = retail_x64
  PK_Sample_09_AsyncLoading_config = retail_x64
  PK_Sample_10_AsyncRendering_config = retail_x64
  PK_Sample_11_ThreadPool_config = retail_x64
  PK_Sample_12_GBufferSampling_config = retail_x64

else ifeq ($(config),retail_arm64)
  PK_Runtime_config = retail_arm64
  PK_Discretizers_config = retail_arm64
  PK_ParticlesToolbox_config = retail_arm64
  PK_Plugin_CompilerBackend_CPU_VM_config = retail_arm64
  PK_Plugin_CodecImage_DDS_config = retail_arm64
  PK_Plugin_CodecImage_JPG_config = retail_arm64
  PK_Plugin_CodecImage_PNG_config = retail_arm64
  PK_Plugin_CodecImage_TGA_config = retail_arm64
  PK_Plugin_CodecImage_TIFF_config = retail_arm64
  PK_Plugin_CodecImage_PKM_config = retail_arm64
  PK_Plugin_CodecImage_PVR_config = retail_arm64
  PK_Plugin_CodecImage_HDR_config = retail_arm64
  PK_Plugin_CodecImage_EXR_config = retail_arm64
  PK_Plugin_CodecMesh_FBX_config = retail_arm64
  PK_ZLib_config = retail_arm64
  PK_AssetBakerLib_config = retail_arm64
  PK_UpgraderLib_config = retail_arm64
  PK_Upgrader_config = retail_arm64
  PK_Sample_01_BasicStartup_config = retail_arm64
  PK_Sample_02_BasicEvolve_config = retail_arm64
  PK_Sample_03_EngineHooks_config = retail_arm64
  PK_Sample_04_Baking_config = retail_arm64
  PK_Sample_05_Upgrader_config = retail_arm64
  PK_RenderHelpers_config = retail_arm64
  PK_RHI_config = retail_arm64
  PK_SampleLib_config = retail_arm64
  PK_MCPP_config = retail_arm64
  PK_Sample_01_BasicRendering_config = retail_arm64
  PK_Sample_02_FullIntegration_config = retail_arm64
  PK_Sample_04_EffectInterface_config = retail_arm64
  PK_Sample_05_Stats_config = retail_arm64
  PK_Sample_06_SimInterface_config = retail_arm64
  PK_Sample_06_SimInterfaceGPU_config = retail_arm64
  PK_Sample_07_LOD_config = retail_arm64
  PK_Sample_08_CustomCollision_config = retail_arm64
  PK_Sample_09_AsyncLoading_config = retail_arm64
  PK_Sample_10_AsyncRendering_config = retail_arm64
  PK_Sample_11_ThreadPool_config = retail_arm64
  PK_Sample_12_GBufferSampling_config = retail_arm64

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := PK-Runtime PK-Discretizers PK-ParticlesToolbox PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_TIFF PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_HDR PK-Plugin_CodecImage_EXR PK-Plugin_CodecMesh_FBX PK-ZLib PK-AssetBakerLib PK-AssetBaker PK-UpgraderLib PK-Upgrader PK-Sample_01_BasicStartup PK-Sample_02_BasicEvolve PK-Sample_03_EngineHooks PK-Sample_04_Baking PK-Sample_05_Upgrader PK-RenderHelpers PK-RHI PK-SampleLib PK-ShaderTool PK-MCPP PK-Sample_01_BasicRendering PK-Sample_02_FullIntegration PK-Sample_04_EffectInterface PK-Sample_05_Stats PK-Sample_06_SimInterface PK-Sample_06_SimInterfaceGPU PK-Sample_07_LOD PK-Sample_08_CustomCollision PK-Sample_09_AsyncLoading PK-Sample_10_AsyncRendering PK-Sample_11_ThreadPool PK-Sample_12_GBufferSampling

.PHONY: all clean help $(PROJECTS) Plugins Plugins/External Rendering Runtime Samples Samples/Basic Tools Tools/AssetBaker Tools/Upgrader

all: $(PROJECTS)

Plugins: Plugins/External PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_EXR PK-Plugin_CodecImage_HDR PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_TIFF PK-Plugin_CodecMesh_FBX PK-Plugin_CompilerBackend_CPU_VM

Plugins/External: PK-ZLib

Rendering: PK-RHI PK-RenderHelpers PK-SampleLib

Runtime: PK-Discretizers PK-ParticlesToolbox PK-Runtime

Samples: Samples/Basic PK-Sample_01_BasicRendering PK-Sample_02_FullIntegration PK-Sample_04_EffectInterface PK-Sample_05_Stats PK-Sample_06_SimInterface PK-Sample_06_SimInterfaceGPU PK-Sample_07_LOD PK-Sample_08_CustomCollision PK-Sample_09_AsyncLoading PK-Sample_10_AsyncRendering PK-Sample_11_ThreadPool PK-Sample_12_GBufferSampling

Samples/Basic: PK-Sample_01_BasicStartup PK-Sample_02_BasicEvolve PK-Sample_03_EngineHooks PK-Sample_04_Baking PK-Sample_05_Upgrader

Tools: Tools/AssetBaker Tools/Upgrader PK-MCPP PK-ShaderTool

Tools/AssetBaker: PK-AssetBaker PK-AssetBakerLib

Tools/Upgrader: PK-Upgrader PK-UpgraderLib

PK-Runtime:
ifneq (,$(PK_Runtime_config))
	@echo "==== Building PK-Runtime ($(PK_Runtime_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Runtime.make config=$(PK_Runtime_config)
endif

PK-Discretizers:
ifneq (,$(PK_Discretizers_config))
	@echo "==== Building PK-Discretizers ($(PK_Discretizers_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Discretizers.make config=$(PK_Discretizers_config)
endif

PK-ParticlesToolbox:
ifneq (,$(PK_ParticlesToolbox_config))
	@echo "==== Building PK-ParticlesToolbox ($(PK_ParticlesToolbox_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-ParticlesToolbox.make config=$(PK_ParticlesToolbox_config)
endif

PK-Plugin_CompilerBackend_CPU_VM:
ifneq (,$(PK_Plugin_CompilerBackend_CPU_VM_config))
	@echo "==== Building PK-Plugin_CompilerBackend_CPU_VM ($(PK_Plugin_CompilerBackend_CPU_VM_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CompilerBackend_CPU_VM.make config=$(PK_Plugin_CompilerBackend_CPU_VM_config)
endif

PK-Plugin_CodecImage_DDS:
ifneq (,$(PK_Plugin_CodecImage_DDS_config))
	@echo "==== Building PK-Plugin_CodecImage_DDS ($(PK_Plugin_CodecImage_DDS_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_DDS.make config=$(PK_Plugin_CodecImage_DDS_config)
endif

PK-Plugin_CodecImage_JPG:
ifneq (,$(PK_Plugin_CodecImage_JPG_config))
	@echo "==== Building PK-Plugin_CodecImage_JPG ($(PK_Plugin_CodecImage_JPG_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_JPG.make config=$(PK_Plugin_CodecImage_JPG_config)
endif

PK-Plugin_CodecImage_PNG:
ifneq (,$(PK_Plugin_CodecImage_PNG_config))
	@echo "==== Building PK-Plugin_CodecImage_PNG ($(PK_Plugin_CodecImage_PNG_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_PNG.make config=$(PK_Plugin_CodecImage_PNG_config)
endif

PK-Plugin_CodecImage_TGA:
ifneq (,$(PK_Plugin_CodecImage_TGA_config))
	@echo "==== Building PK-Plugin_CodecImage_TGA ($(PK_Plugin_CodecImage_TGA_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_TGA.make config=$(PK_Plugin_CodecImage_TGA_config)
endif

PK-Plugin_CodecImage_TIFF:
ifneq (,$(PK_Plugin_CodecImage_TIFF_config))
	@echo "==== Building PK-Plugin_CodecImage_TIFF ($(PK_Plugin_CodecImage_TIFF_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_TIFF.make config=$(PK_Plugin_CodecImage_TIFF_config)
endif

PK-Plugin_CodecImage_PKM:
ifneq (,$(PK_Plugin_CodecImage_PKM_config))
	@echo "==== Building PK-Plugin_CodecImage_PKM ($(PK_Plugin_CodecImage_PKM_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_PKM.make config=$(PK_Plugin_CodecImage_PKM_config)
endif

PK-Plugin_CodecImage_PVR:
ifneq (,$(PK_Plugin_CodecImage_PVR_config))
	@echo "==== Building PK-Plugin_CodecImage_PVR ($(PK_Plugin_CodecImage_PVR_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_PVR.make config=$(PK_Plugin_CodecImage_PVR_config)
endif

PK-Plugin_CodecImage_HDR:
ifneq (,$(PK_Plugin_CodecImage_HDR_config))
	@echo "==== Building PK-Plugin_CodecImage_HDR ($(PK_Plugin_CodecImage_HDR_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_HDR.make config=$(PK_Plugin_CodecImage_HDR_config)
endif

PK-Plugin_CodecImage_EXR:
ifneq (,$(PK_Plugin_CodecImage_EXR_config))
	@echo "==== Building PK-Plugin_CodecImage_EXR ($(PK_Plugin_CodecImage_EXR_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_EXR.make config=$(PK_Plugin_CodecImage_EXR_config)
endif

PK-Plugin_CodecMesh_FBX:
ifneq (,$(PK_Plugin_CodecMesh_FBX_config))
	@echo "==== Building PK-Plugin_CodecMesh_FBX ($(PK_Plugin_CodecMesh_FBX_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecMesh_FBX.make config=$(PK_Plugin_CodecMesh_FBX_config)
endif

PK-ZLib:
ifneq (,$(PK_ZLib_config))
	@echo "==== Building PK-ZLib ($(PK_ZLib_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-ZLib.make config=$(PK_ZLib_config)
endif

PK-AssetBakerLib:
ifneq (,$(PK_AssetBakerLib_config))
	@echo "==== Building PK-AssetBakerLib ($(PK_AssetBakerLib_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-AssetBakerLib.make config=$(PK_AssetBakerLib_config)
endif

PK-AssetBaker: PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TIFF PK-Plugin_CodecImage_HDR PK-Plugin_CompilerBackend_CPU_VM PK-ZLib PK-Plugin_CodecMesh_FBX PK-Plugin_CodecImage_EXR PK-AssetBakerLib PK-UpgraderLib PK-ParticlesToolbox PK-Runtime
ifneq (,$(PK_AssetBaker_config))
	@echo "==== Building PK-AssetBaker ($(PK_AssetBaker_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-AssetBaker.make config=$(PK_AssetBaker_config)
endif

PK-UpgraderLib:
ifneq (,$(PK_UpgraderLib_config))
	@echo "==== Building PK-UpgraderLib ($(PK_UpgraderLib_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-UpgraderLib.make config=$(PK_UpgraderLib_config)
endif

PK-Upgrader: PK-UpgraderLib PK-ParticlesToolbox PK-Runtime
ifneq (,$(PK_Upgrader_config))
	@echo "==== Building PK-Upgrader ($(PK_Upgrader_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Upgrader.make config=$(PK_Upgrader_config)
endif

PK-Sample_01_BasicStartup: PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_JPG PK-Plugin_CompilerBackend_CPU_VM PK-ZLib PK-ParticlesToolbox PK-Runtime
ifneq (,$(PK_Sample_01_BasicStartup_config))
	@echo "==== Building PK-Sample_01_BasicStartup ($(PK_Sample_01_BasicStartup_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_01_BasicStartup.make config=$(PK_Sample_01_BasicStartup_config)
endif

PK-Sample_02_BasicEvolve: PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_JPG PK-Plugin_CompilerBackend_CPU_VM PK-ZLib PK-ParticlesToolbox PK-Runtime
ifneq (,$(PK_Sample_02_BasicEvolve_config))
	@echo "==== Building PK-Sample_02_BasicEvolve ($(PK_Sample_02_BasicEvolve_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_02_BasicEvolve.make config=$(PK_Sample_02_BasicEvolve_config)
endif

PK-Sample_03_EngineHooks: PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_JPG PK-Plugin_CompilerBackend_CPU_VM PK-ZLib PK-ParticlesToolbox PK-Runtime
ifneq (,$(PK_Sample_03_EngineHooks_config))
	@echo "==== Building PK-Sample_03_EngineHooks ($(PK_Sample_03_EngineHooks_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_03_EngineHooks.make config=$(PK_Sample_03_EngineHooks_config)
endif

PK-Sample_04_Baking: PK-AssetBakerLib PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TIFF PK-Plugin_CodecImage_HDR PK-Plugin_CompilerBackend_CPU_VM PK-ZLib PK-Plugin_CodecMesh_FBX PK-Plugin_CodecImage_EXR PK-ParticlesToolbox PK-Runtime
ifneq (,$(PK_Sample_04_Baking_config))
	@echo "==== Building PK-Sample_04_Baking ($(PK_Sample_04_Baking_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_04_Baking.make config=$(PK_Sample_04_Baking_config)
endif

PK-Sample_05_Upgrader: PK-UpgraderLib PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_JPG PK-Plugin_CompilerBackend_CPU_VM PK-ZLib PK-ParticlesToolbox PK-Runtime
ifneq (,$(PK_Sample_05_Upgrader_config))
	@echo "==== Building PK-Sample_05_Upgrader ($(PK_Sample_05_Upgrader_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_05_Upgrader.make config=$(PK_Sample_05_Upgrader_config)
endif

PK-RenderHelpers:
ifneq (,$(PK_RenderHelpers_config))
	@echo "==== Building PK-RenderHelpers ($(PK_RenderHelpers_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-RenderHelpers.make config=$(PK_RenderHelpers_config)
endif

PK-RHI:
ifneq (,$(PK_RHI_config))
	@echo "==== Building PK-RHI ($(PK_RHI_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-RHI.make config=$(PK_RHI_config)
endif

PK-SampleLib:
ifneq (,$(PK_SampleLib_config))
	@echo "==== Building PK-SampleLib ($(PK_SampleLib_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-SampleLib.make config=$(PK_SampleLib_config)
endif

PK-ShaderTool: PK-SampleLib PK-MCPP PK-RHI PK-RenderHelpers PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Plugin_CompilerBackend_CPU_VM PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_ShaderTool_config))
	@echo "==== Building PK-ShaderTool ($(PK_ShaderTool_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-ShaderTool.make config=$(PK_ShaderTool_config)
endif

PK-MCPP:
ifneq (,$(PK_MCPP_config))
	@echo "==== Building PK-MCPP ($(PK_MCPP_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-MCPP.make config=$(PK_MCPP_config)
endif

PK-Sample_01_BasicRendering: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_01_BasicRendering_config))
	@echo "==== Building PK-Sample_01_BasicRendering ($(PK_Sample_01_BasicRendering_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_01_BasicRendering.make config=$(PK_Sample_01_BasicRendering_config)
endif

PK-Sample_02_FullIntegration: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_02_FullIntegration_config))
	@echo "==== Building PK-Sample_02_FullIntegration ($(PK_Sample_02_FullIntegration_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_02_FullIntegration.make config=$(PK_Sample_02_FullIntegration_config)
endif

PK-Sample_04_EffectInterface: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_04_EffectInterface_config))
	@echo "==== Building PK-Sample_04_EffectInterface ($(PK_Sample_04_EffectInterface_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_04_EffectInterface.make config=$(PK_Sample_04_EffectInterface_config)
endif

PK-Sample_05_Stats: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_05_Stats_config))
	@echo "==== Building PK-Sample_05_Stats ($(PK_Sample_05_Stats_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_05_Stats.make config=$(PK_Sample_05_Stats_config)
endif

PK-Sample_06_SimInterface: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_06_SimInterface_config))
	@echo "==== Building PK-Sample_06_SimInterface ($(PK_Sample_06_SimInterface_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_06_SimInterface.make config=$(PK_Sample_06_SimInterface_config)
endif

PK-Sample_06_SimInterfaceGPU: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_06_SimInterfaceGPU_config))
	@echo "==== Building PK-Sample_06_SimInterfaceGPU ($(PK_Sample_06_SimInterfaceGPU_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_06_SimInterfaceGPU.make config=$(PK_Sample_06_SimInterfaceGPU_config)
endif

PK-Sample_07_LOD: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_07_LOD_config))
	@echo "==== Building PK-Sample_07_LOD ($(PK_Sample_07_LOD_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_07_LOD.make config=$(PK_Sample_07_LOD_config)
endif

PK-Sample_08_CustomCollision: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_08_CustomCollision_config))
	@echo "==== Building PK-Sample_08_CustomCollision ($(PK_Sample_08_CustomCollision_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_08_CustomCollision.make config=$(PK_Sample_08_CustomCollision_config)
endif

PK-Sample_09_AsyncLoading: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_09_AsyncLoading_config))
	@echo "==== Building PK-Sample_09_AsyncLoading ($(PK_Sample_09_AsyncLoading_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_09_AsyncLoading.make config=$(PK_Sample_09_AsyncLoading_config)
endif

PK-Sample_10_AsyncRendering: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_10_AsyncRendering_config))
	@echo "==== Building PK-Sample_10_AsyncRendering ($(PK_Sample_10_AsyncRendering_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_10_AsyncRendering.make config=$(PK_Sample_10_AsyncRendering_config)
endif

PK-Sample_11_ThreadPool: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_11_ThreadPool_config))
	@echo "==== Building PK-Sample_11_ThreadPool ($(PK_Sample_11_ThreadPool_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_11_ThreadPool.make config=$(PK_Sample_11_ThreadPool_config)
endif

PK-Sample_12_GBufferSampling: PK-SampleLib PK-RHI PK-RenderHelpers PK-MCPP PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_HDR PK-Discretizers PK-ParticlesToolbox PK-Runtime PK-ZLib
ifneq (,$(PK_Sample_12_GBufferSampling_config))
	@echo "==== Building PK-Sample_12_GBufferSampling ($(PK_Sample_12_GBufferSampling_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_12_GBufferSampling.make config=$(PK_Sample_12_GBufferSampling_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f PK-Runtime.make clean
	@${MAKE} --no-print-directory -C . -f PK-Discretizers.make clean
	@${MAKE} --no-print-directory -C . -f PK-ParticlesToolbox.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CompilerBackend_CPU_VM.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_DDS.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_JPG.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_PNG.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_TGA.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_TIFF.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_PKM.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_PVR.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_HDR.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_EXR.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecMesh_FBX.make clean
	@${MAKE} --no-print-directory -C . -f PK-ZLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-AssetBakerLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-AssetBaker.make clean
	@${MAKE} --no-print-directory -C . -f PK-UpgraderLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-Upgrader.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_01_BasicStartup.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_02_BasicEvolve.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_03_EngineHooks.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_04_Baking.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_05_Upgrader.make clean
	@${MAKE} --no-print-directory -C . -f PK-RenderHelpers.make clean
	@${MAKE} --no-print-directory -C . -f PK-RHI.make clean
	@${MAKE} --no-print-directory -C . -f PK-SampleLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-ShaderTool.make clean
	@${MAKE} --no-print-directory -C . -f PK-MCPP.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_01_BasicRendering.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_02_FullIntegration.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_04_EffectInterface.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_05_Stats.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_06_SimInterface.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_06_SimInterfaceGPU.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_07_LOD.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_08_CustomCollision.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_09_AsyncLoading.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_10_AsyncRendering.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_11_ThreadPool.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_12_GBufferSampling.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_x64"
	@echo "  debug_arm64"
	@echo "  release_x64"
	@echo "  release_arm64"
	@echo "  retail_x64"
	@echo "  retail_arm64"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   PK-Runtime"
	@echo "   PK-Discretizers"
	@echo "   PK-ParticlesToolbox"
	@echo "   PK-Plugin_CompilerBackend_CPU_VM"
	@echo "   PK-Plugin_CodecImage_DDS"
	@echo "   PK-Plugin_CodecImage_JPG"
	@echo "   PK-Plugin_CodecImage_PNG"
	@echo "   PK-Plugin_CodecImage_TGA"
	@echo "   PK-Plugin_CodecImage_TIFF"
	@echo "   PK-Plugin_CodecImage_PKM"
	@echo "   PK-Plugin_CodecImage_PVR"
	@echo "   PK-Plugin_CodecImage_HDR"
	@echo "   PK-Plugin_CodecImage_EXR"
	@echo "   PK-Plugin_CodecMesh_FBX"
	@echo "   PK-ZLib"
	@echo "   PK-AssetBakerLib"
	@echo "   PK-AssetBaker"
	@echo "   PK-UpgraderLib"
	@echo "   PK-Upgrader"
	@echo "   PK-Sample_01_BasicStartup"
	@echo "   PK-Sample_02_BasicEvolve"
	@echo "   PK-Sample_03_EngineHooks"
	@echo "   PK-Sample_04_Baking"
	@echo "   PK-Sample_05_Upgrader"
	@echo "   PK-RenderHelpers"
	@echo "   PK-RHI"
	@echo "   PK-SampleLib"
	@echo "   PK-ShaderTool"
	@echo "   PK-MCPP"
	@echo "   PK-Sample_01_BasicRendering"
	@echo "   PK-Sample_02_FullIntegration"
	@echo "   PK-Sample_04_EffectInterface"
	@echo "   PK-Sample_05_Stats"
	@echo "   PK-Sample_06_SimInterface"
	@echo "   PK-Sample_06_SimInterfaceGPU"
	@echo "   PK-Sample_07_LOD"
	@echo "   PK-Sample_08_CustomCollision"
	@echo "   PK-Sample_09_AsyncLoading"
	@echo "   PK-Sample_10_AsyncRendering"
	@echo "   PK-Sample_11_ThreadPool"
	@echo "   PK-Sample_12_GBufferSampling"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"
