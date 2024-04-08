# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_x64)
  PK_Runtime_SDK1_config = debug_x64
  PK_Discretizers_SDK1_config = debug_x64
  PK_ParticlesToolbox_SDK1_config = debug_x64
  PK_AssetBakerLib_config = debug_x64
  PK_AssetBaker_config = debug_x64
  PK_UpgraderLib_config = debug_x64
  PK_Upgrader_config = debug_x64
  PK_Sample_01_BasicStartup_config = debug_x64
  PK_Sample_02_BasicEvolve_config = debug_x64
  PK_Sample_03_EngineHooks_config = debug_x64
  PK_Sample_04_Baking_config = debug_x64
  PK_Sample_05_Upgrader_config = debug_x64
  PK_RenderHelpers_SDK1_config = debug_x64
  PK_RHI_SDK1_config = debug_x64
  PK_SampleLib_config = debug_x64
  PK_ShaderTool_config = debug_x64
  PK_MCPP_SDK1_config = debug_x64
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
  PK_Runtime_SDK1_config = debug_arm64
  PK_Discretizers_SDK1_config = debug_arm64
  PK_ParticlesToolbox_SDK1_config = debug_arm64
  PK_AssetBakerLib_config = debug_arm64
  PK_AssetBaker_config = debug_arm64
  PK_UpgraderLib_config = debug_arm64
  PK_Upgrader_config = debug_arm64
  PK_Sample_01_BasicStartup_config = debug_arm64
  PK_Sample_02_BasicEvolve_config = debug_arm64
  PK_Sample_03_EngineHooks_config = debug_arm64
  PK_Sample_04_Baking_config = debug_arm64
  PK_Sample_05_Upgrader_config = debug_arm64
  PK_RenderHelpers_SDK1_config = debug_arm64
  PK_RHI_SDK1_config = debug_arm64
  PK_SampleLib_config = debug_arm64
  PK_ShaderTool_config = debug_arm64
  PK_MCPP_SDK1_config = debug_arm64
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
  PK_Runtime_SDK1_config = release_x64
  PK_Discretizers_SDK1_config = release_x64
  PK_ParticlesToolbox_SDK1_config = release_x64
  PK_AssetBakerLib_config = release_x64
  PK_AssetBaker_config = release_x64
  PK_UpgraderLib_config = release_x64
  PK_Upgrader_config = release_x64
  PK_Sample_01_BasicStartup_config = release_x64
  PK_Sample_02_BasicEvolve_config = release_x64
  PK_Sample_03_EngineHooks_config = release_x64
  PK_Sample_04_Baking_config = release_x64
  PK_Sample_05_Upgrader_config = release_x64
  PK_RenderHelpers_SDK1_config = release_x64
  PK_RHI_SDK1_config = release_x64
  PK_SampleLib_config = release_x64
  PK_ShaderTool_config = release_x64
  PK_MCPP_SDK1_config = release_x64
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
  PK_Runtime_SDK1_config = release_arm64
  PK_Discretizers_SDK1_config = release_arm64
  PK_ParticlesToolbox_SDK1_config = release_arm64
  PK_AssetBakerLib_config = release_arm64
  PK_AssetBaker_config = release_arm64
  PK_UpgraderLib_config = release_arm64
  PK_Upgrader_config = release_arm64
  PK_Sample_01_BasicStartup_config = release_arm64
  PK_Sample_02_BasicEvolve_config = release_arm64
  PK_Sample_03_EngineHooks_config = release_arm64
  PK_Sample_04_Baking_config = release_arm64
  PK_Sample_05_Upgrader_config = release_arm64
  PK_RenderHelpers_SDK1_config = release_arm64
  PK_RHI_SDK1_config = release_arm64
  PK_SampleLib_config = release_arm64
  PK_ShaderTool_config = release_arm64
  PK_MCPP_SDK1_config = release_arm64
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
  PK_Runtime_SDK1_config = retail_x64
  PK_Discretizers_SDK1_config = retail_x64
  PK_ParticlesToolbox_SDK1_config = retail_x64
  PK_AssetBakerLib_config = retail_x64
  PK_UpgraderLib_config = retail_x64
  PK_Upgrader_config = retail_x64
  PK_Sample_01_BasicStartup_config = retail_x64
  PK_Sample_02_BasicEvolve_config = retail_x64
  PK_Sample_03_EngineHooks_config = retail_x64
  PK_Sample_04_Baking_config = retail_x64
  PK_Sample_05_Upgrader_config = retail_x64
  PK_RenderHelpers_SDK1_config = retail_x64
  PK_RHI_SDK1_config = retail_x64
  PK_SampleLib_config = retail_x64
  PK_MCPP_SDK1_config = retail_x64
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
  PK_Runtime_SDK1_config = retail_arm64
  PK_Discretizers_SDK1_config = retail_arm64
  PK_ParticlesToolbox_SDK1_config = retail_arm64
  PK_AssetBakerLib_config = retail_arm64
  PK_UpgraderLib_config = retail_arm64
  PK_Upgrader_config = retail_arm64
  PK_Sample_01_BasicStartup_config = retail_arm64
  PK_Sample_02_BasicEvolve_config = retail_arm64
  PK_Sample_03_EngineHooks_config = retail_arm64
  PK_Sample_04_Baking_config = retail_arm64
  PK_Sample_05_Upgrader_config = retail_arm64
  PK_RenderHelpers_SDK1_config = retail_arm64
  PK_RHI_SDK1_config = retail_arm64
  PK_SampleLib_config = retail_arm64
  PK_MCPP_SDK1_config = retail_arm64
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

PROJECTS := PK-Runtime_SDK1 PK-Discretizers_SDK1 PK-ParticlesToolbox_SDK1 PK-AssetBakerLib PK-AssetBaker PK-UpgraderLib PK-Upgrader PK-Sample_01_BasicStartup PK-Sample_02_BasicEvolve PK-Sample_03_EngineHooks PK-Sample_04_Baking PK-Sample_05_Upgrader PK-RenderHelpers_SDK1 PK-RHI_SDK1 PK-SampleLib PK-ShaderTool PK-MCPP_SDK1 PK-Sample_01_BasicRendering PK-Sample_02_FullIntegration PK-Sample_04_EffectInterface PK-Sample_05_Stats PK-Sample_06_SimInterface PK-Sample_06_SimInterfaceGPU PK-Sample_07_LOD PK-Sample_08_CustomCollision PK-Sample_09_AsyncLoading PK-Sample_10_AsyncRendering PK-Sample_11_ThreadPool PK-Sample_12_GBufferSampling

.PHONY: all clean help $(PROJECTS) Rendering Runtime Samples Samples/Basic Tools Tools/AssetBaker Tools/Upgrader

all: $(PROJECTS)

Rendering: PK-RHI_SDK1 PK-RenderHelpers_SDK1 PK-SampleLib

Runtime: PK-Discretizers_SDK1 PK-ParticlesToolbox_SDK1 PK-Runtime_SDK1

Samples: Samples/Basic PK-Sample_01_BasicRendering PK-Sample_02_FullIntegration PK-Sample_04_EffectInterface PK-Sample_05_Stats PK-Sample_06_SimInterface PK-Sample_06_SimInterfaceGPU PK-Sample_07_LOD PK-Sample_08_CustomCollision PK-Sample_09_AsyncLoading PK-Sample_10_AsyncRendering PK-Sample_11_ThreadPool PK-Sample_12_GBufferSampling

Samples/Basic: PK-Sample_01_BasicStartup PK-Sample_02_BasicEvolve PK-Sample_03_EngineHooks PK-Sample_04_Baking PK-Sample_05_Upgrader

Tools: Tools/AssetBaker Tools/Upgrader PK-MCPP_SDK1 PK-ShaderTool

Tools/AssetBaker: PK-AssetBaker PK-AssetBakerLib

Tools/Upgrader: PK-Upgrader PK-UpgraderLib

PK-Runtime_SDK1:
ifneq (,$(PK_Runtime_SDK1_config))
	@echo "==== Building PK-Runtime_SDK1 ($(PK_Runtime_SDK1_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Runtime_SDK1.make config=$(PK_Runtime_SDK1_config)
endif

PK-Discretizers_SDK1:
ifneq (,$(PK_Discretizers_SDK1_config))
	@echo "==== Building PK-Discretizers_SDK1 ($(PK_Discretizers_SDK1_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Discretizers_SDK1.make config=$(PK_Discretizers_SDK1_config)
endif

PK-ParticlesToolbox_SDK1:
ifneq (,$(PK_ParticlesToolbox_SDK1_config))
	@echo "==== Building PK-ParticlesToolbox_SDK1 ($(PK_ParticlesToolbox_SDK1_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-ParticlesToolbox_SDK1.make config=$(PK_ParticlesToolbox_SDK1_config)
endif

PK-AssetBakerLib:
ifneq (,$(PK_AssetBakerLib_config))
	@echo "==== Building PK-AssetBakerLib ($(PK_AssetBakerLib_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-AssetBakerLib.make config=$(PK_AssetBakerLib_config)
endif

PK-AssetBaker: PK-AssetBakerLib PK-UpgraderLib
ifneq (,$(PK_AssetBaker_config))
	@echo "==== Building PK-AssetBaker ($(PK_AssetBaker_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-AssetBaker.make config=$(PK_AssetBaker_config)
endif

PK-UpgraderLib:
ifneq (,$(PK_UpgraderLib_config))
	@echo "==== Building PK-UpgraderLib ($(PK_UpgraderLib_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-UpgraderLib.make config=$(PK_UpgraderLib_config)
endif

PK-Upgrader: PK-UpgraderLib
ifneq (,$(PK_Upgrader_config))
	@echo "==== Building PK-Upgrader ($(PK_Upgrader_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Upgrader.make config=$(PK_Upgrader_config)
endif

PK-Sample_01_BasicStartup:
ifneq (,$(PK_Sample_01_BasicStartup_config))
	@echo "==== Building PK-Sample_01_BasicStartup ($(PK_Sample_01_BasicStartup_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_01_BasicStartup.make config=$(PK_Sample_01_BasicStartup_config)
endif

PK-Sample_02_BasicEvolve:
ifneq (,$(PK_Sample_02_BasicEvolve_config))
	@echo "==== Building PK-Sample_02_BasicEvolve ($(PK_Sample_02_BasicEvolve_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_02_BasicEvolve.make config=$(PK_Sample_02_BasicEvolve_config)
endif

PK-Sample_03_EngineHooks:
ifneq (,$(PK_Sample_03_EngineHooks_config))
	@echo "==== Building PK-Sample_03_EngineHooks ($(PK_Sample_03_EngineHooks_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_03_EngineHooks.make config=$(PK_Sample_03_EngineHooks_config)
endif

PK-Sample_04_Baking: PK-AssetBakerLib
ifneq (,$(PK_Sample_04_Baking_config))
	@echo "==== Building PK-Sample_04_Baking ($(PK_Sample_04_Baking_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_04_Baking.make config=$(PK_Sample_04_Baking_config)
endif

PK-Sample_05_Upgrader: PK-UpgraderLib
ifneq (,$(PK_Sample_05_Upgrader_config))
	@echo "==== Building PK-Sample_05_Upgrader ($(PK_Sample_05_Upgrader_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_05_Upgrader.make config=$(PK_Sample_05_Upgrader_config)
endif

PK-RenderHelpers_SDK1:
ifneq (,$(PK_RenderHelpers_SDK1_config))
	@echo "==== Building PK-RenderHelpers_SDK1 ($(PK_RenderHelpers_SDK1_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-RenderHelpers_SDK1.make config=$(PK_RenderHelpers_SDK1_config)
endif

PK-RHI_SDK1:
ifneq (,$(PK_RHI_SDK1_config))
	@echo "==== Building PK-RHI_SDK1 ($(PK_RHI_SDK1_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-RHI_SDK1.make config=$(PK_RHI_SDK1_config)
endif

PK-SampleLib:
ifneq (,$(PK_SampleLib_config))
	@echo "==== Building PK-SampleLib ($(PK_SampleLib_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-SampleLib.make config=$(PK_SampleLib_config)
endif

PK-ShaderTool: PK-SampleLib
ifneq (,$(PK_ShaderTool_config))
	@echo "==== Building PK-ShaderTool ($(PK_ShaderTool_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-ShaderTool.make config=$(PK_ShaderTool_config)
endif

PK-MCPP_SDK1:
ifneq (,$(PK_MCPP_SDK1_config))
	@echo "==== Building PK-MCPP_SDK1 ($(PK_MCPP_SDK1_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-MCPP_SDK1.make config=$(PK_MCPP_SDK1_config)
endif

PK-Sample_01_BasicRendering: PK-SampleLib
ifneq (,$(PK_Sample_01_BasicRendering_config))
	@echo "==== Building PK-Sample_01_BasicRendering ($(PK_Sample_01_BasicRendering_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_01_BasicRendering.make config=$(PK_Sample_01_BasicRendering_config)
endif

PK-Sample_02_FullIntegration: PK-SampleLib
ifneq (,$(PK_Sample_02_FullIntegration_config))
	@echo "==== Building PK-Sample_02_FullIntegration ($(PK_Sample_02_FullIntegration_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_02_FullIntegration.make config=$(PK_Sample_02_FullIntegration_config)
endif

PK-Sample_04_EffectInterface: PK-SampleLib
ifneq (,$(PK_Sample_04_EffectInterface_config))
	@echo "==== Building PK-Sample_04_EffectInterface ($(PK_Sample_04_EffectInterface_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_04_EffectInterface.make config=$(PK_Sample_04_EffectInterface_config)
endif

PK-Sample_05_Stats: PK-SampleLib
ifneq (,$(PK_Sample_05_Stats_config))
	@echo "==== Building PK-Sample_05_Stats ($(PK_Sample_05_Stats_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_05_Stats.make config=$(PK_Sample_05_Stats_config)
endif

PK-Sample_06_SimInterface: PK-SampleLib
ifneq (,$(PK_Sample_06_SimInterface_config))
	@echo "==== Building PK-Sample_06_SimInterface ($(PK_Sample_06_SimInterface_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_06_SimInterface.make config=$(PK_Sample_06_SimInterface_config)
endif

PK-Sample_06_SimInterfaceGPU: PK-SampleLib
ifneq (,$(PK_Sample_06_SimInterfaceGPU_config))
	@echo "==== Building PK-Sample_06_SimInterfaceGPU ($(PK_Sample_06_SimInterfaceGPU_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_06_SimInterfaceGPU.make config=$(PK_Sample_06_SimInterfaceGPU_config)
endif

PK-Sample_07_LOD: PK-SampleLib
ifneq (,$(PK_Sample_07_LOD_config))
	@echo "==== Building PK-Sample_07_LOD ($(PK_Sample_07_LOD_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_07_LOD.make config=$(PK_Sample_07_LOD_config)
endif

PK-Sample_08_CustomCollision: PK-SampleLib
ifneq (,$(PK_Sample_08_CustomCollision_config))
	@echo "==== Building PK-Sample_08_CustomCollision ($(PK_Sample_08_CustomCollision_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_08_CustomCollision.make config=$(PK_Sample_08_CustomCollision_config)
endif

PK-Sample_09_AsyncLoading: PK-SampleLib
ifneq (,$(PK_Sample_09_AsyncLoading_config))
	@echo "==== Building PK-Sample_09_AsyncLoading ($(PK_Sample_09_AsyncLoading_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_09_AsyncLoading.make config=$(PK_Sample_09_AsyncLoading_config)
endif

PK-Sample_10_AsyncRendering: PK-SampleLib
ifneq (,$(PK_Sample_10_AsyncRendering_config))
	@echo "==== Building PK-Sample_10_AsyncRendering ($(PK_Sample_10_AsyncRendering_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_10_AsyncRendering.make config=$(PK_Sample_10_AsyncRendering_config)
endif

PK-Sample_11_ThreadPool: PK-SampleLib
ifneq (,$(PK_Sample_11_ThreadPool_config))
	@echo "==== Building PK-Sample_11_ThreadPool ($(PK_Sample_11_ThreadPool_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_11_ThreadPool.make config=$(PK_Sample_11_ThreadPool_config)
endif

PK-Sample_12_GBufferSampling: PK-SampleLib
ifneq (,$(PK_Sample_12_GBufferSampling_config))
	@echo "==== Building PK-Sample_12_GBufferSampling ($(PK_Sample_12_GBufferSampling_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-Sample_12_GBufferSampling.make config=$(PK_Sample_12_GBufferSampling_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f PK-Runtime_SDK1.make clean
	@${MAKE} --no-print-directory -C . -f PK-Discretizers_SDK1.make clean
	@${MAKE} --no-print-directory -C . -f PK-ParticlesToolbox_SDK1.make clean
	@${MAKE} --no-print-directory -C . -f PK-AssetBakerLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-AssetBaker.make clean
	@${MAKE} --no-print-directory -C . -f PK-UpgraderLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-Upgrader.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_01_BasicStartup.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_02_BasicEvolve.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_03_EngineHooks.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_04_Baking.make clean
	@${MAKE} --no-print-directory -C . -f PK-Sample_05_Upgrader.make clean
	@${MAKE} --no-print-directory -C . -f PK-RenderHelpers_SDK1.make clean
	@${MAKE} --no-print-directory -C . -f PK-RHI_SDK1.make clean
	@${MAKE} --no-print-directory -C . -f PK-SampleLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-ShaderTool.make clean
	@${MAKE} --no-print-directory -C . -f PK-MCPP_SDK1.make clean
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
	@echo "   PK-Runtime_SDK1"
	@echo "   PK-Discretizers_SDK1"
	@echo "   PK-ParticlesToolbox_SDK1"
	@echo "   PK-AssetBakerLib"
	@echo "   PK-AssetBaker"
	@echo "   PK-UpgraderLib"
	@echo "   PK-Upgrader"
	@echo "   PK-Sample_01_BasicStartup"
	@echo "   PK-Sample_02_BasicEvolve"
	@echo "   PK-Sample_03_EngineHooks"
	@echo "   PK-Sample_04_Baking"
	@echo "   PK-Sample_05_Upgrader"
	@echo "   PK-RenderHelpers_SDK1"
	@echo "   PK-RHI_SDK1"
	@echo "   PK-SampleLib"
	@echo "   PK-ShaderTool"
	@echo "   PK-MCPP_SDK1"
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
