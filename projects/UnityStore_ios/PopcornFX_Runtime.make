# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_ios
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_ios)
  PK_Runtime_config = debug_ios
  PK_Discretizers_config = debug_ios
  PK_ParticlesToolbox_config = debug_ios
  PK_Plugin_CompilerBackend_CPU_VM_config = debug_ios
  PK_Plugin_CodecImage_DDS_config = debug_ios
  PK_Plugin_CodecImage_JPG_config = debug_ios
  PK_Plugin_CodecImage_PNG_config = debug_ios
  PK_Plugin_CodecImage_TGA_config = debug_ios
  PK_Plugin_CodecImage_PKM_config = debug_ios
  PK_Plugin_CodecImage_PVR_config = debug_ios
  PK_Plugin_CodecImage_HDR_config = debug_ios
  PK_Plugin_CodecImage_EXR_config = debug_ios
  PK_ZLib_config = debug_ios
  PK_RenderHelpers_config = debug_ios
  PK_RHI_config = debug_ios

else ifeq ($(config),debug_ios64)
  PK_Runtime_config = debug_ios64
  PK_Discretizers_config = debug_ios64
  PK_ParticlesToolbox_config = debug_ios64
  PK_Plugin_CompilerBackend_CPU_VM_config = debug_ios64
  PK_Plugin_CodecImage_DDS_config = debug_ios64
  PK_Plugin_CodecImage_JPG_config = debug_ios64
  PK_Plugin_CodecImage_PNG_config = debug_ios64
  PK_Plugin_CodecImage_TGA_config = debug_ios64
  PK_Plugin_CodecImage_PKM_config = debug_ios64
  PK_Plugin_CodecImage_PVR_config = debug_ios64
  PK_Plugin_CodecImage_HDR_config = debug_ios64
  PK_Plugin_CodecImage_EXR_config = debug_ios64
  PK_ZLib_config = debug_ios64
  PK_RenderHelpers_config = debug_ios64
  PK_RHI_config = debug_ios64

else ifeq ($(config),release_ios)
  PK_Runtime_config = release_ios
  PK_Discretizers_config = release_ios
  PK_ParticlesToolbox_config = release_ios
  PK_Plugin_CompilerBackend_CPU_VM_config = release_ios
  PK_Plugin_CodecImage_DDS_config = release_ios
  PK_Plugin_CodecImage_JPG_config = release_ios
  PK_Plugin_CodecImage_PNG_config = release_ios
  PK_Plugin_CodecImage_TGA_config = release_ios
  PK_Plugin_CodecImage_PKM_config = release_ios
  PK_Plugin_CodecImage_PVR_config = release_ios
  PK_Plugin_CodecImage_HDR_config = release_ios
  PK_Plugin_CodecImage_EXR_config = release_ios
  PK_ZLib_config = release_ios
  PK_RenderHelpers_config = release_ios
  PK_RHI_config = release_ios

else ifeq ($(config),release_ios64)
  PK_Runtime_config = release_ios64
  PK_Discretizers_config = release_ios64
  PK_ParticlesToolbox_config = release_ios64
  PK_Plugin_CompilerBackend_CPU_VM_config = release_ios64
  PK_Plugin_CodecImage_DDS_config = release_ios64
  PK_Plugin_CodecImage_JPG_config = release_ios64
  PK_Plugin_CodecImage_PNG_config = release_ios64
  PK_Plugin_CodecImage_TGA_config = release_ios64
  PK_Plugin_CodecImage_PKM_config = release_ios64
  PK_Plugin_CodecImage_PVR_config = release_ios64
  PK_Plugin_CodecImage_HDR_config = release_ios64
  PK_Plugin_CodecImage_EXR_config = release_ios64
  PK_ZLib_config = release_ios64
  PK_RenderHelpers_config = release_ios64
  PK_RHI_config = release_ios64

else ifeq ($(config),retail_ios)
  PK_Runtime_config = retail_ios
  PK_Discretizers_config = retail_ios
  PK_ParticlesToolbox_config = retail_ios
  PK_Plugin_CompilerBackend_CPU_VM_config = retail_ios
  PK_Plugin_CodecImage_DDS_config = retail_ios
  PK_Plugin_CodecImage_JPG_config = retail_ios
  PK_Plugin_CodecImage_PNG_config = retail_ios
  PK_Plugin_CodecImage_TGA_config = retail_ios
  PK_Plugin_CodecImage_PKM_config = retail_ios
  PK_Plugin_CodecImage_PVR_config = retail_ios
  PK_Plugin_CodecImage_HDR_config = retail_ios
  PK_Plugin_CodecImage_EXR_config = retail_ios
  PK_ZLib_config = retail_ios
  PK_RenderHelpers_config = retail_ios
  PK_RHI_config = retail_ios

else ifeq ($(config),retail_ios64)
  PK_Runtime_config = retail_ios64
  PK_Discretizers_config = retail_ios64
  PK_ParticlesToolbox_config = retail_ios64
  PK_Plugin_CompilerBackend_CPU_VM_config = retail_ios64
  PK_Plugin_CodecImage_DDS_config = retail_ios64
  PK_Plugin_CodecImage_JPG_config = retail_ios64
  PK_Plugin_CodecImage_PNG_config = retail_ios64
  PK_Plugin_CodecImage_TGA_config = retail_ios64
  PK_Plugin_CodecImage_PKM_config = retail_ios64
  PK_Plugin_CodecImage_PVR_config = retail_ios64
  PK_Plugin_CodecImage_HDR_config = retail_ios64
  PK_Plugin_CodecImage_EXR_config = retail_ios64
  PK_ZLib_config = retail_ios64
  PK_RenderHelpers_config = retail_ios64
  PK_RHI_config = retail_ios64

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := PK-Runtime PK-Discretizers PK-ParticlesToolbox PK-Plugin_CompilerBackend_CPU_VM PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_TGA PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_HDR PK-Plugin_CodecImage_EXR PK-ZLib PK-RenderHelpers PK-RHI

.PHONY: all clean help $(PROJECTS) Plugins Plugins/External Rendering Runtime

all: $(PROJECTS)

Plugins: Plugins/External PK-Plugin_CodecImage_DDS PK-Plugin_CodecImage_EXR PK-Plugin_CodecImage_HDR PK-Plugin_CodecImage_JPG PK-Plugin_CodecImage_PKM PK-Plugin_CodecImage_PNG PK-Plugin_CodecImage_PVR PK-Plugin_CodecImage_TGA PK-Plugin_CompilerBackend_CPU_VM

Plugins/External: PK-ZLib

Rendering: PK-RHI PK-RenderHelpers

Runtime: PK-Discretizers PK-ParticlesToolbox PK-Runtime

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

PK-ZLib:
ifneq (,$(PK_ZLib_config))
	@echo "==== Building PK-ZLib ($(PK_ZLib_config)) ===="
	@${MAKE} --no-print-directory -C . -f PK-ZLib.make config=$(PK_ZLib_config)
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

clean:
	@${MAKE} --no-print-directory -C . -f PK-Runtime.make clean
	@${MAKE} --no-print-directory -C . -f PK-Discretizers.make clean
	@${MAKE} --no-print-directory -C . -f PK-ParticlesToolbox.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CompilerBackend_CPU_VM.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_DDS.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_JPG.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_PNG.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_TGA.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_PKM.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_PVR.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_HDR.make clean
	@${MAKE} --no-print-directory -C . -f PK-Plugin_CodecImage_EXR.make clean
	@${MAKE} --no-print-directory -C . -f PK-ZLib.make clean
	@${MAKE} --no-print-directory -C . -f PK-RenderHelpers.make clean
	@${MAKE} --no-print-directory -C . -f PK-RHI.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_ios"
	@echo "  debug_ios64"
	@echo "  release_ios"
	@echo "  release_ios64"
	@echo "  retail_ios"
	@echo "  retail_ios64"
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
	@echo "   PK-Plugin_CodecImage_PKM"
	@echo "   PK-Plugin_CodecImage_PVR"
	@echo "   PK-Plugin_CodecImage_HDR"
	@echo "   PK-Plugin_CodecImage_EXR"
	@echo "   PK-ZLib"
	@echo "   PK-RenderHelpers"
	@echo "   PK-RHI"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"
