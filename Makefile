MAKEFLAGS += --no-builtin-rules --no-print-directory

# Ensure the build fails if a piped command fails
SHELL = /bin/bash
.SHELLFLAGS = -o pipefail -c

# Build options can either be changed by modifying the makefile, or by building with 'make SETTING=value'

# currently, GCC is the only supported compiler
COMPILER := gcc

# Target game version. Currently only the following version is supported:
#   gc-eu-mq       GameCube Europe/PAL Master Quest
#   gc-eu-mq-dbg   GameCube Europe/PAL Master Quest Debug
#   hackeroot-mq   HackerOoT, based on gc-eu-mq-dbg (default)
#
# The following versions are work-in-progress and not yet matching:
#   gc-eu          GameCube Europe/PAL
#
# Note: choosing hackeroot-mq will enable HackerOoT features,
#       if another version is chosen, this repo will be like
#       zeldaret/main decomp but without the disassembly, decompilation
#       and matching tools, including the IDO compiler
VERSION := hackeroot-mq

# Enable optimization flags to use GDB on Ares
ARES_GDB := 1

# Toggle release or debug mode. 1=Release, 0=Debug
# Note: currently only used for HackerOoT
RELEASE := 0

# Valid compression algorithms are 'yaz', 'lzo' and 'aplib'
COMPRESSION ?= yaz
COMPRESSION_TYPE ?= $(shell echo $(COMPRESSION) | tr '[:lower:]' '[:upper:]')

# Number of threads to extract and compress with
N_THREADS := $(shell nproc)
# Music Macro Language Version
MML_VERSION := MML_VERSION_OOT
# Check code syntax with host compiler
RUN_CC_CHECK := 1

CFLAGS ?=
CPPFLAGS ?=
CFLAGS_IDO ?=

TARGET ?=

ifeq ($(TARGET),wad)
CFLAGS := -DCONSOLE_WIIVC -fno-reorder-blocks -fno-optimize-sibling-calls
CPPFLAGS := -DCONSOLE_WIIVC
else ifeq ($(TARGET),iso)
CFLAGS := -DCONSOLE_GC -fno-reorder-blocks -fno-optimize-sibling-calls
CPPFLAGS := -DCONSOLE_GC
endif

ifeq ($(COMPILER),gcc)
  CFLAGS += -DCOMPILER_GCC -DNON_MATCHING -DAVOID_UB -std=gnu11
  CPPFLAGS += -DCOMPILER_GCC -DNON_MATCHING -DAVOID_UB -std=gnu11
else
  $(error Unsupported compiler. Please use gcc as the COMPILER variable.)
endif

# Set prefix to mips binutils binaries (mips-linux-gnu-ld => 'mips-linux-gnu-') - Change at your own risk!
# In nearly all cases, not having 'mips-linux-gnu-*' binaries on the PATH is indicative of missing dependencies

# Returns the path to the command $(1) if exists. Otherwise returns an empty string.
find-command = $(shell which $(1) 2>/dev/null)

ifneq ($(call find-command,mips-n64-ld),)
  MIPS_BINUTILS_PREFIX := mips-n64-
else ifneq ($(call find-command,mips64-ld),)
  MIPS_BINUTILS_PREFIX := mips64-
else ifneq ($(call find-command,mips-linux-gnu-ld),)
  MIPS_BINUTILS_PREFIX := mips-linux-gnu-
else ifneq ($(call find-command,mips64-linux-gnu-ld),)
  MIPS_BINUTILS_PREFIX := mips64-linux-gnu-
else ifneq ($(call find-command,mips-ld),)
  MIPS_BINUTILS_PREFIX := mips-
else ifneq ($(call find-command,mips64-elf-ld),)
  MIPS_BINUTILS_PREFIX := mips64-elf-
else
  $(error Unable to detect a suitable MIPS toolchain installed)
endif

# Version-specific settings
ifeq ($(VERSION),gc-eu)
  DEBUG := 0
  HACKEROOT := 0
else ifeq ($(VERSION),gc-eu-mq)
  DEBUG := 0
  CFLAGS += -DOOT_MQ
  CPPFLAGS += -DOOT_MQ
  HACKEROOT := 0
else ifeq ($(VERSION),gc-eu-mq-dbg)
  DEBUG := 1
  CFLAGS += -DOOT_MQ
  CPPFLAGS += -DOOT_MQ
  HACKEROOT := 0
else ifeq ($(VERSION),hackeroot-mq)
  CFLAGS += -DOOT_MQ
  CPPFLAGS += -DOOT_MQ
  HACKEROOT := 1
else
$(error Unsupported version $(VERSION))
endif

ifeq ($(VERSION),hackeroot-mq)
  AUDIO_GAME_VERSION := $(VERSION)
else
  AUDIO_GAME_VERSION := oot-$(VERSION)
endif

PROJECT_DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR := build/$(VERSION)
EXPECTED_DIR := expected/$(BUILD_DIR)
BASEROM_DIR := baseroms/$(VERSION)
EXTRACTED_DIR := extracted/$(VERSION)
VENV := .venv

MAKE = make
CPPFLAGS += -P -xc -fno-dollars-in-identifiers

# Set PACKAGE_VERSION define for printing commit hash
ifeq ($(origin PACKAGE_VERSION), undefined)
  PACKAGE_VERSION := $(shell git log -1 --pretty=%h | tr -d '\n')
  ifeq ('$(PACKAGE_VERSION)', '')
    PACKAGE_VERSION = Unknown version
  endif
endif

ifeq ($(VERSION),hackeroot-mq)
  CFLAGS += -DENABLE_HACKEROOT=1
  CPPFLAGS += -DENABLE_HACKEROOT=1
  CFLAGS_IDO += -DENABLE_HACKEROOT=1
  OPTFLAGS := -Os

  ifeq ($(RELEASE),1)
    CFLAGS += -DRELEASE_ROM=1 -DOOT_DEBUG=0
    CPPFLAGS += -DRELEASE_ROM=1 -DOOT_DEBUG=0
    CFLAGS_IDO += -DOOT_DEBUG=0
  else
    CFLAGS += -DRELEASE_ROM=0 -DOOT_DEBUG=1
    CPPFLAGS += -DRELEASE_ROM=0 -DOOT_DEBUG=1
    CFLAGS_IDO += -DRELEASE_ROM=0 -DOOT_DEBUG=1
  endif
else
  ifeq ($(DEBUG),1)
    CFLAGS += -DOOT_DEBUG=1
    CPPFLAGS += -DOOT_DEBUG=1
    CFLAGS_IDO += -DOOT_DEBUG=1
    OPTFLAGS := -O2
  else
    CFLAGS += -DNDEBUG -DOOT_DEBUG=0
    CPPFLAGS += -DNDEBUG -DOOT_DEBUG=0
    CFLAGS_IDO += -DNDEBUG -DOOT_DEBUG=0
    OPTFLAGS := -O2 -g3
  endif

  CFLAGS += -DENABLE_HACKEROOT=0
  CPPFLAGS += -DENABLE_HACKEROOT=0
  CFLAGS_IDO += -DENABLE_HACKEROOT=0
endif

# Override optimization flags if using GDB
ifeq ($(ARES_GDB),1)
  OPTFLAGS := -Og -ggdb3
endif

# Define author and package version for every OoT version
# Note: this won't be used if not using HackerOoT
CFLAGS += -DPACKAGE_VERSION='$(PACKAGE_VERSION)' -DCOMPRESS_$(COMPRESSION_TYPE)=1
CPPFLAGS += -DPACKAGE_VERSION='$(PACKAGE_VERSION)' -DCOMPRESS_$(COMPRESSION_TYPE)=1
CFLAGS_IDO += -DPACKAGE_VERSION='$(PACKAGE_VERSION)' -DCOMPRESS_$(COMPRESSION_TYPE)=1
OPTFLAGS += -ffast-math -fno-unsafe-math-optimizations

ifeq ($(OS),Windows_NT)
    DETECTED_OS=windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS=linux
    endif
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS=macos
        MAKE=gmake
    endif
endif

# Verbose toggle
V := @
ifeq (VERBOSE, 1)
    V=
endif

# Colors
NO_COL  := \033[0m
GREEN   := \033[0;32m
BLUE    := \033[0;36m
YELLOW  := \033[0;33m
BLINK   := \033[32;5m

PRINT := printf

# Generic print function for make rules
define print
  $(V)echo -e "$(GREEN)$(1) $(YELLOW)$(2)$(GREEN) -> $(BLUE)$(3)$(NO_COL)"
endef

#### Tools ####
ifneq ($(shell type $(MIPS_BINUTILS_PREFIX)ld >/dev/null 2>/dev/null; echo $$?), 0)
  $(error Unable to find $(MIPS_BINUTILS_PREFIX)ld. Please install or build MIPS binutils, commonly mips-linux-gnu. (or set MIPS_BINUTILS_PREFIX if your MIPS binutils install uses another prefix))
endif

# Detect compiler and set variables appropriately.
ifeq ($(COMPILER),gcc)
  CC       := $(MIPS_BINUTILS_PREFIX)gcc
else
$(error Unsupported compiler. Please use gcc as the COMPILER variable.)
endif

AS      := $(MIPS_BINUTILS_PREFIX)as
LD      := $(MIPS_BINUTILS_PREFIX)ld
OBJCOPY := $(MIPS_BINUTILS_PREFIX)objcopy
OBJDUMP := $(MIPS_BINUTILS_PREFIX)objdump
NM      := $(MIPS_BINUTILS_PREFIX)nm

N64_EMULATOR ?= 

INC := -Iinclude -Iinclude/libc -Isrc -I$(BUILD_DIR) -I. -I$(EXTRACTED_DIR)

# Check code syntax with host compiler
CHECK_WARNINGS := -Wall -Wextra -Wno-format-security -Wno-unknown-pragmas -Wno-unused-parameter -Wno-unused-variable -Wno-missing-braces

# The `cpp` command behaves differently on macOS (it behaves as if
# `-traditional-cpp` was passed) so we use `gcc -E` instead.
CPP        := gcc -E
MKLDSCRIPT := tools/mkldscript
MKDMADATA  := tools/mkdmadata
ELF2ROM    := tools/elf2rom
ZAPD       := tools/ZAPD/ZAPD.out
FADO       := tools/fado/fado.elf
PYTHON     ?= $(VENV)/bin/python3
FLIPS      := tools/Flips/flips
GZINJECT   := tools/gzinject/gzinject
CC_IDO     := tools/ido_recomp/linux/5.3/cc

# Command to replace $(BUILD_DIR) in some files with the build path.
# We can't use the C preprocessor for this because it won't substitute inside string literals.
BUILD_DIR_REPLACE := sed -e 's|$$(BUILD_DIR)|$(BUILD_DIR)|g'

# Audio tools
AUDIO_EXTRACT := $(PYTHON) tools/audio/extraction/audio_extract.py
SAMPLECONV    := tools/audio/sampleconv/sampleconv
SBC           := tools/audio/sbc --matching
SFC           := tools/audio/sfc --matching
ELFPATCH      := tools/audio/elfpatch
ATBLGEN       := tools/audio/atblgen
AFILE_SIZES   := tools/audio/afile_sizes

# We want linemarkers in sequence assembly files for better assembler error messages
SEQ_CPP := $(CPP) -x assembler-with-cpp -fno-dollars-in-identifiers
SEQ_CPPFLAGS := -D_LANGUAGE_ASEQ -DMML_VERSION=$(MML_VERSION) -I include/audio -I include/tables/sfx -I $(BUILD_DIR)/assets/audio/soundfonts

ASFLAGS := -march=vr4300 -32 -no-pad-sections -Iinclude

ifeq ($(COMPILER),gcc)
  CFLAGS += -G 0 -nostdinc $(INC) -march=vr4300 -mfix4300 -mabi=32 -mno-abicalls -mdivide-breaks -fno-PIC -fno-common -ffreestanding -fbuiltin -fno-builtin-sinf -fno-builtin-cosf $(CHECK_WARNINGS) -funsigned-char
  MIPS_VERSION := -mips3
  RUN_CC_CHECK := 0
endif

OBJDUMP_FLAGS := -d -r -z -Mreg-names=32

#### Files ####

# ROM image
ifeq ($(VERSION),hackeroot-mq)
  ROM      := $(BUILD_DIR)/$(VERSION).z64
else
  ROM      := $(BUILD_DIR)/oot-$(VERSION).z64
endif
ROMC     := $(ROM:.z64=-compressed-$(COMPRESSION).z64)
WAD      := $(ROM:.z64=.wad)
ISO      := $(ROM:.z64=.iso)
BPS      := $(ROM:.z64=.bps)
ELF      := $(ROM:.z64=.elf)
MAP      := $(ROM:.z64=.map)
LDSCRIPT := $(ROM:.z64=.ld)
DMA_CONFIG_FILE := dma_config.txt
# description of ROM segments
SPEC := spec

# Baserom to use when creating BPS patches
BASEROM_PATCH ?= baseroms/$(VERSION)/baserom.z64

ifeq ($(COMPILER),gcc)
SRC_DIRS := $(shell find src -type d)
endif

ifneq ($(wildcard assets/audio/samples),)
  AIFF_DIRS := $(shell find assets/audio/samples -type d)
  SAMPLEBANK_DIRS := $(shell find assets/audio/samplebanks -type d)
  SOUNDFONT_DIRS := $(shell find assets/audio/soundfonts -type d)
  SEQUENCE_DIRS := $(shell find assets/audio/sequences -type d)
else
  AIFF_DIRS :=
  SAMPLEBANK_DIRS :=
  SOUNDFONT_DIRS :=
  SEQUENCE_DIRS :=
endif

AIFF_FILES := $(foreach dir,$(AIFF_DIRS),$(wildcard $(dir)/*.wav))
AIFC_FILES := $(foreach f,$(AIFF_FILES),$(BUILD_DIR)/$(f:.wav=.aifc))

SAMPLEBANK_XMLS := $(foreach dir,$(SAMPLEBANK_DIRS),$(wildcard $(dir)/*.xml))
SAMPLEBANK_BUILD_XMLS := $(foreach f,$(SAMPLEBANK_XMLS),$(BUILD_DIR)/$f)
SAMPLEBANK_O_FILES := $(foreach f,$(SAMPLEBANK_XMLS),$(BUILD_DIR)/$(f:.xml=.o))
SAMPLEBANK_DEP_FILES := $(foreach f,$(SAMPLEBANK_O_FILES),$(f:.o=.d))

SOUNDFONT_XMLS := $(foreach dir,$(SOUNDFONT_DIRS),$(wildcard $(dir)/*.xml))
SOUNDFONT_BUILD_XMLS := $(foreach f,$(SOUNDFONT_XMLS),$(BUILD_DIR)/$f)
SOUNDFONT_O_FILES := $(foreach f,$(SOUNDFONT_XMLS),$(BUILD_DIR)/$(f:.xml=.o))
SOUNDFONT_HEADERS := $(foreach f,$(SOUNDFONT_XMLS),$(BUILD_DIR)/$(f:.xml=.h))
SOUNDFONT_DEP_FILES := $(foreach f,$(SOUNDFONT_O_FILES),$(f:.o=.d))

SEQUENCE_FILES := $(foreach dir,$(SEQUENCE_DIRS),$(wildcard $(dir)/*.seq))
SEQUENCE_O_FILES := $(foreach f,$(SEQUENCE_FILES:.seq=.o),$(BUILD_DIR)/$f)
SEQUENCE_DEP_FILES := $(foreach f,$(SEQUENCE_O_FILES),$(f:.o=.d))

SEQUENCE_TABLE := include/tables/sequence_table.h

ASSET_BIN_DIRS := $(shell find assets/* -type d -not -path "assets/audio*" -not -path "assets/xml*" -not -path "assets/text")
ASSET_FILES_XML := $(foreach dir,$(ASSET_BIN_DIRS),$(wildcard $(dir)/*.xml))
ASSET_FILES_BIN := $(foreach dir,$(ASSET_BIN_DIRS),$(wildcard $(dir)/*.bin))
ASSET_FILES_OUT := $(foreach f,$(ASSET_FILES_XML:.xml=.c),$f) \
				   $(foreach f,$(ASSET_FILES_BIN:.bin=.bin.inc.c),$(BUILD_DIR)/$f) \
				   $(foreach f,$(wildcard assets/text/*.c),$(BUILD_DIR)/$(f:.c=.o))

UNDECOMPILED_DATA_DIRS := $(shell find data -type d)

BASEROM_BIN_FILES := $(wildcard $(EXTRACTED_DIR)/baserom/*)

# source files
C_FILES       := $(filter-out %.inc.c,$(foreach dir,$(SRC_DIRS) $(ASSET_BIN_DIRS),$(wildcard $(dir)/*.c)))
S_FILES       := $(foreach dir,$(SRC_DIRS) $(UNDECOMPILED_DATA_DIRS),$(wildcard $(dir)/*.s))
O_FILES       := $(foreach f,$(S_FILES:.s=.o),$(BUILD_DIR)/$f) \
                 $(foreach f,$(C_FILES:.c=.o),$(BUILD_DIR)/$f) \
                 $(foreach f,$(BASEROM_BIN_FILES),$(BUILD_DIR)/baserom/$(notdir $f).o)
UCODE_PATCHES := $(wildcard F3DEX3/*.bps)
UCODE_FILES   := $(foreach f,$(UCODE_PATCHES:.bps=),$f)
UCODE_O_FILES := $(foreach f,$(UCODE_FILES),$(BUILD_DIR)/$f.o)

OVL_RELOC_FILES := $(shell $(CPP) $(CPPFLAGS) $(SPEC) | $(BUILD_DIR_REPLACE) | grep -o '[^"]*_reloc.o' )

# Automatic dependency files
# (Only asm_processor dependencies and reloc dependencies are handled for now)
DEP_FILES := $(O_FILES:.o=.asmproc.d) $(OVL_RELOC_FILES:.o=.d)


TEXTURE_FILES_PNG := $(foreach dir,$(ASSET_BIN_DIRS),$(wildcard $(dir)/*.png))
TEXTURE_FILES_JPG := $(foreach dir,$(ASSET_BIN_DIRS),$(wildcard $(dir)/*.jpg))
TEXTURE_FILES_OUT := $(foreach f,$(TEXTURE_FILES_PNG:.png=.inc.c),$(BUILD_DIR)/$f) \
					 $(foreach f,$(TEXTURE_FILES_JPG:.jpg=.jpg.inc.c),$(BUILD_DIR)/$f) \

# create build directories
$(shell mkdir -p $(BUILD_DIR)/baserom $(BUILD_DIR)/include/tables $(EXTRACTED_DIR)/text $(BUILD_DIR)/assets/text $(foreach dir,$(SRC_DIRS) $(UNDECOMPILED_DATA_DIRS) $(ASSET_BIN_DIRS) $(AIFF_DIRS) $(SAMPLEBANK_DIRS) $(SOUNDFONT_DIRS) $(SEQUENCE_DIRS),$(BUILD_DIR)/$(dir)))

ifeq ($(COMPILER),gcc)
# Note that if adding additional assets directories for modding reasons these flags must also be used there
  $(BUILD_DIR)/assets/%.o: CFLAGS += -fno-zero-initialized-in-bss -fno-toplevel-reorder
  $(BUILD_DIR)/src/%.o: CFLAGS += -fexec-charset=utf-8
  $(BUILD_DIR)/src/libultra/libc/ll.o: OPTFLAGS := -Ofast
  $(BUILD_DIR)/src/overlays/%.o: CFLAGS += -fno-merge-constants -mno-explicit-relocs -mno-split-addresses

  $(BUILD_DIR)/src/overlays/actors/ovl_Item_Shield/%.o: OPTFLAGS := -O2
  $(BUILD_DIR)/src/overlays/actors/ovl_En_Part/%.o: OPTFLAGS := -O2
  $(BUILD_DIR)/src/overlays/actors/ovl_Item_B_Heart/%.o: OPTFLAGS := -O0
  $(BUILD_DIR)/src/overlays/actors/ovl_Bg_Mori_Hineri/%.o: OPTFLAGS := -O0

# library overrides for Gamecube
ifeq ($(TARGET),iso)
  MIPS_VERSION_IDO := -mips2
  CFLAGS_IDO += -G 0 -non_shared -fullwarn -verbose -Xcpluscomm $(INC) -Wab,-r4300_mul -woff 516,609,649,838,712
  $(BUILD_DIR)/src/libultra/io/viswapbuf.o: OPTFLAGS := -O2
  $(BUILD_DIR)/src/libultra/io/viswapbuf.o: MIPS_VERSION := $(MIPS_VERSION_IDO)
  $(BUILD_DIR)/src/libultra/io/viswapbuf.o: CFLAGS := $(CFLAGS_IDO)
  $(BUILD_DIR)/src/libultra/io/viswapbuf.o: CC := $(CC_IDO)
  $(BUILD_DIR)/src/libultra/gu/sinf.o: OPTFLAGS := -O2
  $(BUILD_DIR)/src/libultra/gu/sinf.o: MIPS_VERSION := $(MIPS_VERSION_IDO)
  $(BUILD_DIR)/src/libultra/gu/sinf.o: CFLAGS := $(CFLAGS_IDO)
  $(BUILD_DIR)/src/libultra/gu/sinf.o: CC := $(CC_IDO)
  $(BUILD_DIR)/src/libultra/gu/cosf.o: OPTFLAGS := -O2
  $(BUILD_DIR)/src/libultra/gu/cosf.o: MIPS_VERSION := $(MIPS_VERSION_IDO)
  $(BUILD_DIR)/src/libultra/gu/cosf.o: CFLAGS := $(CFLAGS_IDO)
  $(BUILD_DIR)/src/libultra/gu/cosf.o: CC := $(CC_IDO)
  $(BUILD_DIR)/src/libultra/gu/perspective.o: OPTFLAGS := -O2
  $(BUILD_DIR)/src/libultra/gu/perspective.o: MIPS_VERSION := $(MIPS_VERSION_IDO)
  $(BUILD_DIR)/src/libultra/gu/perspective.o: CFLAGS := $(CFLAGS_IDO)
  $(BUILD_DIR)/src/libultra/gu/perspective.o: CC := $(CC_IDO)
  $(BUILD_DIR)/src/libultra/os/getmemsize.o: OPTFLAGS := -O1
  $(BUILD_DIR)/src/libultra/os/getmemsize.o: MIPS_VERSION := $(MIPS_VERSION_IDO)
  $(BUILD_DIR)/src/libultra/os/getmemsize.o: CFLAGS := $(CFLAGS_IDO)
  $(BUILD_DIR)/src/libultra/os/getmemsize.o: CC := $(CC_IDO)
  $(BUILD_DIR)/src/libultra/os/aisetnextbuf.o: OPTFLAGS := -O1
  $(BUILD_DIR)/src/libultra/os/aisetnextbuf.o: MIPS_VERSION := $(MIPS_VERSION_IDO)
  $(BUILD_DIR)/src/libultra/os/aisetnextbuf.o: CFLAGS := $(CFLAGS_IDO)
  $(BUILD_DIR)/src/libultra/os/aisetnextbuf.o: CC := $(CC_IDO)
endif
endif

#### Main Targets ###

all: rom

rom:
	$(call print,Building the rom...)
	$(V)python3 tools/mod_assets.py
	$(V)$(MAKE) $(ROM)

compress:
	$(call print,Compressing the rom...)
# make sure z_std_dma.c and spec are up-to-date
	$(V)$(shell touch spec)
	$(V)$(shell touch src/boot/z_std_dma.c)
	$(V)$(MAKE) $(ROMC)
	$(call print,Success!)

wad:
	$(call print,Patching WAD...)
ifeq ("$(wildcard baseroms/$(VERSION)/common-key.bin)", "")
	$(error Please provide the common-key.bin file.)
endif
	$(V)$(MAKE) compress TARGET=wad
	$(V)$(GZINJECT) -a inject -r 1 -k baseroms/$(VERSION)/common-key.bin -w baseroms/$(VERSION)/basewad.wad -m $(ROMC) -o $(WAD) -t "HackerOoT" -i NHOE -p tools/gzinject/patches/NACE.gzi -p tools/gzinject/patches/gz_default_remap.gzi
	$(V)$(RM) -r wadextract/
	$(call print,Success!)

iso:
	$(V)$(MAKE) compress TARGET=iso
	$(call print,Patching ISO...)
	$(V)$(PYTHON) tools/gc_utility.py -v $(VERSION) -c $(COMPRESSION)
	$(V)$(GZINJECT) -a extract -s baseroms/$(VERSION)/baseiso.iso
	$(V)cp $(BUILD_DIR)/$(DMA_CONFIG_FILE) isoextract/zlj_f.tgc/$(DMA_CONFIG_FILE)
	$(V)cp $(ROMC) isoextract/zlj_f.tgc/zlj_f.n64
	$(V)$(RM) -r isoextract/S_*.tgc/ isoextract/zlj_f.tgc/*.thp
	$(V)$(FLIPS) --apply tools/gamecube.bps isoextract/zlj_f.tgc/main.dol isoextract/zlj_f.tgc/main.dol
	$(V)$(GZINJECT) -a pack -s $(ISO)
	$(V)$(RM) -r isoextract/
	$(call print,Success!)

clean:
	$(V)$(RM) -r $(BUILD_DIR)
	$(call print,Success!)

assetclean:
	$(V)$(RM) -r $(ASSET_BIN_DIRS)
	$(V)$(RM) -r $(EXTRACTED_DIR)
	$(V)$(RM) -r $(BUILD_DIR)/assets
	$(V)$(RM) -r .extracted-assets.json
	$(V)$(RM) -r assets/audio/samplebanks
	$(V)$(RM) -r assets/audio/samples
	$(V)$(RM) -r assets/audio/soundfonts
	$(V)find assets/audio/sequences -type f -name "*.seq" -not -name "*.prg.seq" -delete
	$(call print,Success!)

distclean: assetclean
	$(V)$(RM) -r extracted/
	$(V)$(RM) -r build/
	$(V)$(MAKE) -C tools distclean
	$(V)$(RM) -r F3DEX3/*.code F3DEX3/*.data
	$(call print,Success!)

venv:
# Create the virtual environment if it doesn't exist.
# Delete the virtual environment directory if creation fails.
	$(call print,Preparing the virtual environment...)
	$(V)test -d $(VENV) || python3 -m venv $(VENV) || { rm -rf $(VENV); false; }
	$(V)$(PYTHON) -m pip install -U pip
	$(V)$(PYTHON) -m pip install -U -r requirements.txt
	$(call print,Success!)

setup-audio:
	$(call print,Audio Setup in progress...)
	$(V)$(AUDIO_EXTRACT) -r $(BASEROM_DIR)/baserom-decompressed.z64 -v $(AUDIO_GAME_VERSION) --read-xml

setup: venv
	$(call print,Setup in progress...)
	$(V)$(MAKE) -C tools
	$(call print,Tools: Done!)
	$(V)$(PYTHON) tools/decompress_baserom.py $(VERSION)
	$(call print,Decompressing baserom: Done!)
	$(V)$(PYTHON) tools/extract_baserom.py $(BASEROM_DIR)/baserom-decompressed.z64 -o $(EXTRACTED_DIR)/baserom --dmadata-start `cat $(BASEROM_DIR)/dmadata_start.txt` --dmadata-names $(BASEROM_DIR)/dmadata_names.txt
	$(V)$(PYTHON) tools/msgdis.py --oot-version $(VERSION) --text-out $(EXTRACTED_DIR)/text/message_data.h --staff-text-out $(EXTRACTED_DIR)/text/message_data_staff.h
# TODO: for now, we only extract assets from the Debug ROM
ifneq ($(VERSION),gc-eu-mq)
	$(V)$(PYTHON) extract_assets.py -j$(N_THREADS) -v $(VERSION)
endif
	$(call print,Extracting files: Done!)
ifeq ($(VERSION),hackeroot-mq)
# TODO: proper fix (for .s files)
	cp baseroms/hackeroot-mq/baserom-decompressed.z64 baseroms/gc-eu-mq-dbg/baserom-decompressed.z64
endif
	$(V)$(MAKE) setup-audio
	$(call print,Success!)

run: rom
ifeq ($(N64_EMULATOR),)
	$(error Emulator path not set. Set N64_EMULATOR in the Makefile or define it as an environment variable)
endif
	$(N64_EMULATOR) $(ROM)

patch:
	$(call print,Creating BPS patch...)
	$(V)$(FLIPS) --create --bps $(BASEROM_PATCH) $(ROM) $(BPS)
	$(call print,Success!)

create_f3dex3_patches: F3DEX3/f3dzex2.code F3DEX3/f3dzex2.data
	$(call print,Creating F3DEX3 patches...)
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.code F3DEX3/F3DEX3_BrW.code F3DEX3/F3DEX3_BrW.code.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.data F3DEX3/F3DEX3_BrW.data F3DEX3/F3DEX3_BrW.data.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.code F3DEX3/F3DEX3_BrW_PA.code F3DEX3/F3DEX3_BrW_PA.code.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.data F3DEX3/F3DEX3_BrW_PA.data F3DEX3/F3DEX3_BrW_PA.data.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.code F3DEX3/F3DEX3_BrW_PB.code F3DEX3/F3DEX3_BrW_PB.code.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.data F3DEX3/F3DEX3_BrW_PB.data F3DEX3/F3DEX3_BrW_PB.data.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.code F3DEX3/F3DEX3_BrW_PC.code F3DEX3/F3DEX3_BrW_PC.code.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.data F3DEX3/F3DEX3_BrW_PC.data F3DEX3/F3DEX3_BrW_PC.data.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.code F3DEX3/F3DEX3_BrW_NOC.code F3DEX3/F3DEX3_BrW_NOC.code.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.data F3DEX3/F3DEX3_BrW_NOC.data F3DEX3/F3DEX3_BrW_NOC.data.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.code F3DEX3/F3DEX3_BrW_NOC_PA.code F3DEX3/F3DEX3_BrW_NOC_PA.code.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.data F3DEX3/F3DEX3_BrW_NOC_PA.data F3DEX3/F3DEX3_BrW_NOC_PA.data.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.code F3DEX3/F3DEX3_BrW_NOC_PB.code F3DEX3/F3DEX3_BrW_NOC_PB.code.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.data F3DEX3/F3DEX3_BrW_NOC_PB.data F3DEX3/F3DEX3_BrW_NOC_PB.data.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.code F3DEX3/F3DEX3_BrW_NOC_PC.code F3DEX3/F3DEX3_BrW_NOC_PC.code.bps
	$(V)$(FLIPS) --create --bps F3DEX3/f3dzex2.data F3DEX3/F3DEX3_BrW_NOC_PC.data F3DEX3/F3DEX3_BrW_NOC_PC.data.bps
	$(call print,Success!)

verify:
	$(V)$(MAKE) clean
	$(V)$(MAKE) rom
	@md5sum $(ROM)

.PHONY: all rom compress clean assetclean distclean venv setup run wad iso patch create_f3dex3_patches verify

.DEFAULT_GOAL := rom

#### Various Recipes ####

$(ROM): $(ELF)
	$(V)$(ELF2ROM) -cic 6105 $< $@
	@$(PRINT) "${BLINK}Build succeeded.\n$(NO_COL)"
	@$(PRINT) "==== Build Options ====$(NO_COL)\n"
	@$(PRINT) "${GREEN}OoT Version: $(BLUE)$(VERSION)$(NO_COL)\n"
	@$(PRINT) "${GREEN}Code Version: $(BLUE)$(PACKAGE_VERSION)$(NO_COL)\n"

$(ROMC): $(ROM) $(ELF) $(BUILD_DIR)/compress_ranges.txt
ifeq ($(COMPRESSION),yaz)
	$(V)$(PYTHON) tools/compress.py --in $(ROM) --out $@ --dmadata-start `./tools/dmadata_start.sh $(NM) $(ELF)` --compress `cat $(BUILD_DIR)/compress_ranges.txt` --threads $(N_THREADS)
else
	$(V)$(PYTHON) tools/z64compress_wrapper.py --codec $(COMPRESSION) --cache $(BUILD_DIR)/cache --threads $(N_THREADS) $< $@ $(ELF) $(BUILD_DIR)/$(SPEC)
endif
	$(V)$(PYTHON) -m ipl3checksum sum --cic 6105 --update $@

$(ELF): $(TEXTURE_FILES_OUT) $(ASSET_FILES_OUT) $(O_FILES) $(OVL_RELOC_FILES) $(UCODE_O_FILES) $(LDSCRIPT) $(BUILD_DIR)/undefined_syms.txt \
        $(AIFC_FILES) \
        $(SAMPLEBANK_O_FILES) \
        $(SOUNDFONT_O_FILES) \
        $(SEQUENCE_O_FILES) \
        $(BUILD_DIR)/src/audio/tables/samplebank_table.o \
        $(BUILD_DIR)/src/audio/tables/soundfont_table.o \
        $(BUILD_DIR)/src/audio/tables/sequence_table.o \
        $(BUILD_DIR)/assets/audio/sequence_font_table.o \
        $(BUILD_DIR)/assets/audio/sequence_sizes.h \
        $(BUILD_DIR)/assets/audio/soundfont_sizes.h \
        $(BUILD_DIR)/assets/audio/audiobank_padding.o
	$(call print,Linking:,,$@)
	$(V)$(LD) -T $(LDSCRIPT) -T $(BUILD_DIR)/undefined_syms.txt --no-check-sections --accept-unknown-input-arch --emit-relocs -Map $(MAP) -o $@

## Order-only prerequisites
# These ensure e.g. the O_FILES are built before the OVL_RELOC_FILES.
# The intermediate phony targets avoid quadratically-many dependencies between the targets and prerequisites.

o_files: $(O_FILES)
$(OVL_RELOC_FILES): | o_files

asset_files: $(TEXTURE_FILES_OUT) $(ASSET_FILES_OUT)
$(O_FILES): | asset_files

.PHONY: o_files asset_files

$(BUILD_DIR)/$(SPEC): $(SPEC)
	$(call print,Preprocessing:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) $< | $(BUILD_DIR_REPLACE) > $@

$(LDSCRIPT): $(BUILD_DIR)/$(SPEC)
	$(call print,Creating linker script:,$<,$@)
	$(V)$(MKLDSCRIPT) $< $@

$(BUILD_DIR)/undefined_syms.txt: undefined_syms.txt
	$(call print,Preprocessing:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) $< > $@

$(BUILD_DIR)/baserom/%.o: $(EXTRACTED_DIR)/baserom/%
	$(call print,Wrapping binary to ELF:,$<,$@)
	$(V)$(OBJCOPY) -I binary -O elf32-big $< $@
	
$(BUILD_DIR)/F3DEX3/%.o: F3DEX3/%
	$(call print,Wrapping binary to ELF:,$<,$@)
	$(V)mkdir -p $(BUILD_DIR)/F3DEX3
	$(V)$(OBJCOPY) -I binary -O elf32-big $< $@

$(BUILD_DIR)/data/%.o: data/%.s
	$(call print,Assembling:,$<,$@)
	$(V)$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/assets/text/%.enc.h: assets/text/%.h $(EXTRACTED_DIR)/text/%.h assets/text/charmap.txt
	$(call print,Encoding:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) -I$(EXTRACTED_DIR) $< | $(PYTHON) tools/msgenc.py - --output $@ --charmap assets/text/charmap.txt

# Dependencies for files including message data headers
# TODO remove when full header dependencies are used.
$(BUILD_DIR)/assets/text/fra_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.h
$(BUILD_DIR)/assets/text/ger_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.h
$(BUILD_DIR)/assets/text/nes_message_data_static.o: $(BUILD_DIR)/assets/text/message_data.enc.h
$(BUILD_DIR)/assets/text/staff_message_data_static.o: $(BUILD_DIR)/assets/text/message_data_staff.enc.h
$(BUILD_DIR)/src/code/z_message_PAL.o: $(BUILD_DIR)/assets/text/message_data.enc.h $(BUILD_DIR)/assets/text/message_data_staff.enc.h

$(BUILD_DIR)/assets/%.o: assets/%.c
	$(call print,Compiling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $@ $<
	$(V)$(OBJCOPY) -O binary $@ $@.bin

$(BUILD_DIR)/src/%.o: src/%.s
	$(call print,Compiling:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) -Iinclude $< | $(AS) $(ASFLAGS) -o $@

$(BUILD_DIR)/dmadata_table_spec.h $(BUILD_DIR)/compress_ranges.txt: $(BUILD_DIR)/$(SPEC)
	$(V)$(MKDMADATA) $< $(BUILD_DIR)/dmadata_table_spec.h $(BUILD_DIR)/compress_ranges.txt

# Dependencies for files that may include the dmadata header automatically generated from the spec file
$(BUILD_DIR)/src/boot/z_std_dma.o: $(BUILD_DIR)/dmadata_table_spec.h
$(BUILD_DIR)/src/dmadata/dmadata.o: $(BUILD_DIR)/dmadata_table_spec.h

# Dependencies for files including from include/tables/
# TODO remove when full header dependencies are used.
$(BUILD_DIR)/src/code/graph.o: include/tables/gamestate_table.h
$(BUILD_DIR)/src/code/object_table.o: include/tables/object_table.h
$(BUILD_DIR)/src/code/z_actor.o: include/tables/actor_table.h # so uses of ACTOR_ID_MAX update when the table length changes
$(BUILD_DIR)/src/code/z_actor_dlftbls.o: include/tables/actor_table.h
$(BUILD_DIR)/src/code/z_effect_soft_sprite_dlftbls.o: include/tables/effect_ss_table.h
$(BUILD_DIR)/src/code/z_game_dlftbls.o: include/tables/gamestate_table.h
$(BUILD_DIR)/src/code/z_scene_table.o: include/tables/scene_table.h include/tables/entrance_table.h
$(BUILD_DIR)/src/audio/general.o: $(SEQUENCE_TABLE)
$(BUILD_DIR)/src/audio/sfx_params.o: include/tables/sfx/*.h

$(BUILD_DIR)/src/%.o: src/%.c
ifneq ($(RUN_CC_CHECK),0)
	$(V)$(CC_CHECK) $<
endif
	$(call print,Compiling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $@ $<
	$(V)@$(OBJDUMP) $(OBJDUMP_FLAGS) $@ > $(@:.o=.s)

$(BUILD_DIR)/src/libultra/libc/ll.o: src/libultra/libc/ll.c
ifneq ($(RUN_CC_CHECK),0)
	$(V)$(CC_CHECK) $<
endif
	$(call print,Compiling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $@ $<
	$(V)$(PYTHON) tools/set_o32abi_bit.py $@
	$(V)@$(OBJDUMP) $(OBJDUMP_FLAGS) $@ > $(@:.o=.s)

$(BUILD_DIR)/src/libultra/libc/llcvt.o: src/libultra/libc/llcvt.c
ifneq ($(RUN_CC_CHECK),0)
	$(V)$(CC_CHECK) $<
endif
	$(call print,Compiling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $@ $<
	$(V)$(PYTHON) tools/set_o32abi_bit.py $@
	$(V)@$(OBJDUMP) $(OBJDUMP_FLAGS) $@ > $(@:.o=.s)

$(BUILD_DIR)/src/overlays/%_reloc.o: $(BUILD_DIR)/$(SPEC)
	$(call print,Generating Relocation:,$<,$@)
	$(V)$(FADO) $$(tools/reloc_prereq $< $(notdir $*)) -n $(notdir $*) -o $(@:.o=.s) -M $(@:.o=.d)
	$(V)$(AS) $(ASFLAGS) $(@:.o=.s) -o $@

$(BUILD_DIR)/%.inc.c: %.png
	$(V)$(ZAPD) btex -eh -tt $(subst .,,$(suffix $*)) -i $< -o $@

$(BUILD_DIR)/assets/%.bin.inc.c: assets/%.bin
	$(V)$(ZAPD) bblb -eh -i $< -o $@

$(BUILD_DIR)/assets/%.jpg.inc.c: assets/%.jpg
	$(V)$(ZAPD) bren -eh -i $< -o $@

F3DEX3/f3dzex2.code:
	$(V)$(PYTHON) tools/data_extractor.py --start 0xBCD0F0 --size 0x1630 --input $(BASEROM_DIR)/baserom-decompressed.z64 --output $@

F3DEX3/f3dzex2.data:
	$(V)$(PYTHON) tools/data_extractor.py --start 0xBCE720 --size 0x420 --input $(BASEROM_DIR)/baserom-decompressed.z64 --output $@

F3DEX3/F3DEX3%.code: F3DEX3/F3DEX3%.code.bps F3DEX3/f3dzex2.code
	$(V)$(FLIPS) --apply F3DEX3/F3DEX3$*.code.bps F3DEX3/f3dzex2.code $@
	
F3DEX3/F3DEX3%.data: F3DEX3/F3DEX3%.data.bps F3DEX3/f3dzex2.data
	$(V)$(FLIPS) --apply F3DEX3/F3DEX3$*.data.bps F3DEX3/f3dzex2.data $@

.PRECIOUS: $(UCODE_FILES)

# Audio

# first build samples...

$(BUILD_DIR)/assets/audio/samples/%.half.aifc: assets/audio/samples/%.half.wav
	$(SAMPLECONV) vadpcm-half $< $@
# TESTING:
#	@(cmp $(<D)/aifc/$(<F:.half.wav=.half.aifc) $@ && echo "$(<F) OK") || (mkdir -p NONMATCHINGS/$(<D) && cp $(<D)/aifc/$(<F:.half.wav=.half.aifc) NONMATCHINGS/$(<D)/$(<F:.half.wav=.half.aifc))

$(BUILD_DIR)/assets/audio/samples/%.aifc: assets/audio/samples/%.wav
	$(SAMPLECONV) vadpcm $< $@
# TESTING:
#	@(cmp $(<D)/aifc/$(<F:.wav=.aifc) $@ && echo "$(<F) OK") || (mkdir -p NONMATCHINGS/$(<D) && cp $(<D)/aifc/$(<F:.wav=.aifc) NONMATCHINGS/$(<D)/$(<F:.wav=.aifc))

# then assemble the samplebanks...

$(BUILD_DIR)/assets/audio/samplebanks/%.xml: assets/audio/samplebanks/%.xml
	cat $< | $(BUILD_DIR_REPLACE) > $@

.PRECIOUS: $(BUILD_DIR)/assets/audio/samplebanks/%.s
$(BUILD_DIR)/assets/audio/samplebanks/%.s: $(BUILD_DIR)/assets/audio/samplebanks/%.xml | $(AIFC_FILES)
	$(SBC) --makedepend $(@:.s=.d) $< $@

-include $(SAMPLEBANK_DEP_FILES)

$(BUILD_DIR)/assets/audio/samplebanks/%.o: $(BUILD_DIR)/assets/audio/samplebanks/%.s
	$(AS) $(ASFLAGS) $< -o $@
# TESTING:
#	$(OBJCOPY) -O binary -j.rodata $@ $(@:.o=.bin)
#	@cmp $(@:.o=.bin) $(patsubst $(BUILD_DIR)/assets/audio/samplebanks/%,$(EXTRACTED_DIR)/baserom_audiotest/audiotable_files/%,$(@:.o=.bin)) && echo "$(<F) OK"

# also assemble the soundfonts and generate the associated headers...

$(BUILD_DIR)/assets/audio/soundfonts/%.xml: assets/audio/soundfonts/%.xml
	cat $< | $(BUILD_DIR_REPLACE) > $@

.PRECIOUS: $(BUILD_DIR)/assets/audio/soundfonts/%.c $(BUILD_DIR)/assets/audio/soundfonts/%.h $(BUILD_DIR)/assets/audio/soundfonts/%.name
$(BUILD_DIR)/assets/audio/soundfonts/%.c $(BUILD_DIR)/assets/audio/soundfonts/%.h $(BUILD_DIR)/assets/audio/soundfonts/%.name: $(BUILD_DIR)/assets/audio/soundfonts/%.xml | $(SAMPLEBANK_BUILD_XMLS) $(AIFC_FILES)
# This rule can be triggered for either the .c or .h file, so $@ may refer to either the .c or .h file. A simple
# substitution $(@:.c=.h) will fail ~50% of the time with -j. Instead, don't assume anything about the suffix of $@.
	$(SFC) --makedepend $(basename $@).d $< $(basename $@).c $(basename $@).h $(basename $@).name

-include $(SOUNDFONT_DEP_FILES)

$(BUILD_DIR)/assets/audio/soundfonts/%.o: $(BUILD_DIR)/assets/audio/soundfonts/%.c $(BUILD_DIR)/assets/audio/soundfonts/%.name #$(SAMPLEBANK_O_FILES) # (for debugging only)
# compile c to unlinked object
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -I include/audio -o $(@:.o=.tmp) $<
# partial link
	@$(LD) -r -T include/audio/sf.ld $(@:.o=.tmp) -o $(@:.o=.tmp2)
# patch defined symbols to be ABS symbols so that they remain file-relative offsets forever
	@$(ELFPATCH) $(@:.o=.tmp2) $(@:.o=.tmp2)
# write start and size symbols afterwards, filename != symbolic name so source symbolic name from the .name file written by sfc
	@$(OBJCOPY) --add-symbol $$(cat $(<:.c=.name))_Start=.rodata:0,global --redefine-sym __LEN__=$$(cat $(<:.c=.name))_Size $(@:.o=.tmp2) $@
# cleanup temp files
	@$(RM) $(@:.o=.tmp) $(@:.o=.tmp2)
# TESTING: link with samplebanks and dump binary
#	$(LD) $(foreach f,$(SAMPLEBANK_O_FILES),-R $f) -T include/audio/sf.ld $@ -o $(@:.o=.elf)
#	$(OBJCOPY) -O binary -j.rodata $(@:.o=.elf) $(@:.o=.bin)
#	@(cmp $(@:.o=.bin) $(patsubst $(BUILD_DIR)/assets/audio/soundfonts/%,$(EXTRACTED_DIR)/baserom_audiotest/audiobank_files/%,$(@:.o=.bin)) && echo "$(<F) OK" || (mkdir -p NONMATCHINGS/soundfonts && cp $(@:.o=.bin) NONMATCHINGS/soundfonts/$(@F:.o=.bin)))

# then assemble the sequences...

$(BUILD_DIR)/assets/audio/sequences/%.o: assets/audio/sequences/%.seq include/audio/aseq.h | include/audio/sequence_ids.h $(SEQUENCE_TABLE) $(SOUNDFONT_HEADERS)
	$(SEQ_CPP) $(SEQ_CPPFLAGS) $< -o $(@:.o=.S) -MMD -MT $@
	$(AS) $(ASFLAGS) -I $(BUILD_DIR)/assets/audio/soundfonts -I include/audio $(@:.o=.S) -o $@
# TESTING:
#	$(OBJCOPY) -O binary -j.data $@ $(@:.o=.aseq)
#	@(cmp $(@:.o=.aseq) $(patsubst $(BUILD_DIR)/assets/audio/sequences/%,$(EXTRACTED_DIR)/baserom_audiotest/audioseq_files/%,$(@:.o=.aseq)) && echo "$(<F) OK" || (mkdir -p NONMATCHINGS/sequences && cp $(@:.o=.aseq) NONMATCHINGS/sequences/$(@F:.o=.aseq)))

-include $(SEQUENCE_DEP_FILES)

# put together the tables

$(BUILD_DIR)/assets/audio/samplebank_table.h: $(SAMPLEBANK_BUILD_XMLS)
	$(ATBLGEN) -banks $@ $^

$(BUILD_DIR)/assets/audio/soundfont_table.h: $(SOUNDFONT_BUILD_XMLS)
	$(ATBLGEN) -fonts $@ $^

SEQ_ORDER_DEFS := -DDEFINE_SEQUENCE_PTR\(name,seqId,_2,_3,_4,_5\)=*\(name,seqId\) \
                  -DDEFINE_SEQUENCE\(name,seqId,_2,_3,_4,_5\)=\(name,seqId\)
$(BUILD_DIR)/include/tables/sequence_order.in: $(SEQUENCE_TABLE)
	$(CPP) $(CPPFLAGS) $< $(SEQ_ORDER_DEFS) -o $@

$(BUILD_DIR)/assets/audio/sequence_font_table.s: $(BUILD_DIR)/include/tables/sequence_order.in $(SEQUENCE_O_FILES)
	$(ATBLGEN) -sequences $@ $^

# build the tables into objects, move data -> rodata

$(BUILD_DIR)/src/audio/tables/samplebank_table.o: src/audio/tables/samplebank_table.c $(BUILD_DIR)/assets/audio/samplebank_table.h
ifneq ($(RUN_CC_CHECK),0)
	$(CC_CHECK) $<
endif
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $(@:.o=.tmp) $<
	@$(LD) -r -T include/audio/atbl_rdata.ld $(@:.o=.tmp) -o $@
	@$(RM) $(@:.o=.tmp)
# TESTING:
#	$(OBJCOPY) -O binary -j.rodata $@ $(@:.o=.bin)
# Can't compare this due to pointers

$(BUILD_DIR)/src/audio/tables/soundfont_table.o: src/audio/tables/soundfont_table.c $(BUILD_DIR)/assets/audio/soundfont_table.h $(SOUNDFONT_HEADERS)
ifneq ($(RUN_CC_CHECK),0)
	$(CC_CHECK) $<
endif
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -o $(@:.o=.tmp) $<
	@$(LD) -r -T include/audio/atbl_rdata.ld $(@:.o=.tmp) -o $@
	@$(RM) $(@:.o=.tmp)
# TESTING:
#	$(OBJCOPY) -O binary -j.rodata $@ $(@:.o=.bin)
# Can't compare this due to pointers

$(BUILD_DIR)/src/audio/tables/sequence_table.o: src/audio/tables/sequence_table.c $(SEQUENCE_TABLE)
ifneq ($(RUN_CC_CHECK),0)
	$(CC_CHECK) $<
endif
	$(CC) -c $(CFLAGS) $(MIPS_VERSION) $(OPTFLAGS) -I include/tables -o $(@:.o=.tmp) $<
	@$(LD) -r -T include/audio/atbl_rdata.ld $(@:.o=.tmp) -o $@
	@$(RM) $(@:.o=.tmp)
# TESTING:
#	$(OBJCOPY) -O binary -j.rodata $@ $(@:.o=.bin)
# Can't compare this due to pointers

$(BUILD_DIR)/assets/audio/sequence_font_table.o: $(BUILD_DIR)/assets/audio/sequence_font_table.s
	$(AS) $(ASFLAGS) $< -o $@
# TESTING:
#	$(OBJCOPY) -O binary -j.rodata $@ $(@:.o=.bin)
#	@cmp $(@:.o=.bin) $(EXTRACTED_DIR)/baserom_audiotest/audio_code_tables/sequence_font_table.bin && echo "$(@F:.o=) OK"

# make headers with file sizes and amounts

$(BUILD_DIR)/src/audio/session_config.o: $(BUILD_DIR)/assets/audio/soundfont_sizes.h $(BUILD_DIR)/assets/audio/sequence_sizes.h

$(BUILD_DIR)/assets/audio/soundfont_sizes.h: $(SOUNDFONT_O_FILES)
	$(AFILE_SIZES) $@ NUM_SOUNDFONTS SOUNDFONT_SIZES $^

$(BUILD_DIR)/assets/audio/sequence_sizes.h: $(SEQUENCE_O_FILES)
	$(AFILE_SIZES) $@ NUM_SEQUENCES SEQUENCE_SIZES $^

# Extra audiobank padding that doesn't belong to any soundfont file
$(BUILD_DIR)/assets/audio/audiobank_padding.o:
	echo ".rdata; .fill 0x20" | $(AS) $(ASFLAGS) -o $@

-include $(DEP_FILES)

# Print target for debugging
print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
