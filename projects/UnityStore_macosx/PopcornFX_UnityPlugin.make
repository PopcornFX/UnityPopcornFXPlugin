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
  PK_UpgraderLib_config = debug_x64
  PK_RenderHelpers_SDK1_config = debug_x64
  PK_IntegrationUnity_config = debug_x64

else ifeq ($(config),debug_arm64)
  PK_Runtime_SDK1_config = debug_arm64
  PK_Discretizers_SDK1_config = debug_arm64
  PK_ParticlesToolbox_SDK1_config = debug_arm64
  PK_AssetBakerLib_config = debug_arm64
  PK_UpgraderLib_config = debug_arm64
  PK_RenderHelpers_SDK1_config = debug_arm64
  PK_IntegrationUnity_config = debug_arm64

else ifeq ($(config),release_x64)
  PK_Runtime_SDK1_config = release_x64
  PK_Discretizers_SDK1_config = release_x64
  PK_ParticlesToolbox_SDK1_config = release_x64
  PK_AssetBakerLib_config = release_x64
  PK_UpgraderLib_config = release_x64
  PK_RenderHelpers_SDK1_config = release_x64
  PK_IntegrationUnity_config = release_x64

else ifeq ($(config),release_arm64)
  PK_Runtime_SDK1_config = release_arm64
  PK_Discretizers_SDK1_config = release_arm64
  PK_ParticlesToolbox_SDK1_config = release_arm64
  PK_AssetBakerLib_config = release_arm64
  PK_UpgraderLib_config = release_arm64
  PK_RenderHelpers_SDK1_config = release_arm64
  PK_IntegrationUnity_config = release_arm64

else ifeq ($(config),retail_x64)
  PK_Runtime_SDK1_config = retail_x64
  PK_Discretizers_SDK1_config = retail_x64
  PK_ParticlesToolbox_SDK1_config = retail_x64
  PK_AssetBakerLib_config = retail_x64
  PK_UpgraderLib_config = retail_x64
  PK_RenderHelpers_SDK1_config = retail_x64
  PK_IntegrationUnity_config = retail_x64

else ifeq ($(config),retail_arm64)
  PK_Runtime_SDK1_config = retail_arm64
  PK_Discretizers_SDK1_config = retail_arm64
  PK_ParticlesToolbox_SDK1_config = retail_arm64
  PK_AssetBakerLib_config = retail_arm64
  PK_UpgraderLib_config = retail_arm64
  PK_RenderHelpers_SDK1_config = retail_arm64
  PK_IntegrationUnity_config = retail_arm64

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := PK-Runtime_SDK1 PK-Discretizers_SDK1 PK-ParticlesToolbox_SDK1 PK-AssetBakerLib PK-UpgraderLib PK-RenderHelpers_SDK1 PK-IntegrationUnity

.PHONY: all clean help $(PROJECTS) Rendering Runtime Tools Tools/AssetBaker Tools/Upgrader Unity

all: $(PROJECTS)

Rendering: PK-RenderHelpers_SDK1

Runtime: PK-Discretizers_SDK1 PK-ParticlesToolbox_SDK1 PK-Runtime_SDK1

Tools: Tools/AssetBaker Tools/Upgrader

Tools/AssetBaker: PK-AssetBakerLib

Tools/Upgrader: PK-UpgraderLib

Unity: PK-IntegrationUnity

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

PK-UpgraderLib:
ifneq (,$(PK_UpgraderLib_config))
	@echo "==== Building PK-UpgraderLib ($(PK_UpgraderLib_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-UpgraderLib.make config=$(PK_UpgraderLib_config)
endif

PK-RenderHelpers_SDK1:
ifneq (,$(PK_RenderHelpers_SDK1_config))
	@echo "==== Building PK-RenderHelpers_SDK1 ($(PK_RenderHelpers_SDK1_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-RenderHelpers_SDK1.make config=$(PK_RenderHelpers_SDK1_config)
endif

PK-IntegrationUnity: PK-UpgraderLib PK-AssetBakerLib
ifneq (,$(PK_IntegrationUnity_config))
	@echo "==== Building PK-IntegrationUnity ($(PK_IntegrationUnity_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-IntegrationUnity.make config=$(PK_IntegrationUnity_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f PK-Runtime_SDK1.make clean
	@${MAKE} --no-print-directory -C . -f PK-Discretizers_SDK1.make clean
	@${MAKE} --no-print-directory -C . -f PK-ParticlesToolbox_SDK1.make clean
	@${MAKE} --no-print-directory -C . -f PK-AssetBakerLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-UpgraderLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-RenderHelpers_SDK1.make clean
	@${MAKE} --no-print-directory -C . -f PK-IntegrationUnity.make clean

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
	@echo "   PK-UpgraderLib"
	@echo "   PK-RenderHelpers_SDK1"
	@echo "   PK-IntegrationUnity"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"
