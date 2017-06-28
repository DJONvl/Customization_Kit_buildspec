ADSDIR          := c:/progra~1/DS-5/bin

#if we want to use environment variable to config ADS PATH, use following code
#ADSDIR          := $(if $(ARMHOME),$(ARMHOME),\
#                   c:/progra~1/arm/ADSv1_2)

# Executables and Pathes
CC              := $(if $(LINUX),cc,"$(ADSDIR)/armcc.exe")
AS              := $(if $(LINUX),as,"$(ADSDIR)/armasm.exe")
LD              := $(if $(LINUX),ld,"$(ADSDIR)/armlink.exe")
AR              := $(if $(LINUX),ar,"$(ADSDIR)/armar.exe")
DISASSEMBLE     := $(if $(LINUX),fromelf,"$(ADSDIR)/fromelf.exe")
BIN             := $(if $(LINUX),fromelf,"$(ADSDIR)/fromelf.exe")
LIB             := $(if $(LINUX),lib,$(ADSDIR)/LIB)

# Source Code and Include Directory 
D_ROOT          := .

# Default FLASH
FLASH = 

DA_VERSION      := 3.3001.00.00
DA_SIG_SYMBOL   := g_Signature

ifeq ($(DA_PL),yes)
DA_INFO_INI     := config/settings/$(BBCHIP)/AddDAInfo_MTK_AllInOne_DA_SV5_$(BBCHIP)_4EndUser.ini
else
ifeq ($(SW_SEC_BOOT),yes)
DA_INFO_INI     := config/settings/$(BBCHIP)/AddDAInfo_MTK_AllInOne_DA_SV5_$(BBCHIP)_SwSecBoot.ini
else
DA_INFO_INI     := config/settings/$(BBCHIP)/AddDAInfo_MTK_AllInOne_DA_SV5_$(BBCHIP).ini
endif

endif

ADD_DA_INFO     := build/tools/AddDAInfo_SV5.exe
SIGN_DA_EXE     := build/tools/SignDA_SV5.exe 
SIGN_DA         := $(SIGN_DA_EXE) SHA256 $(OUTPUT_BIN) INT_SYSRAM,INT_TCM,D_TCM,EXT_RAM $(OUTPUT_SIG)

DA_RUN_IN_DRAM  := no

# customization kit 
CUSTOM_KIT_ZIP = Customization_Kit_$(BBCHIP).zip

CFLAGS += $(addprefix -I,$(INCLUDE_PATH))

DEBUG_ON = --debug

CFLAGS += --thumb $(DEBUG_ON) -I$(D_ROOT)/INC --cpu CORTEX-A7 $(if $(LINUX),,--apcs /interwork) --littleend 
CFLAGS += -O2 -c --no_unaligned_access -Ono_memcpy
CFLAGS += -DDISABLE_NFI_DMA
CFLAGS += -DDISABLE_TOTAL_CHKSUM
CFLAGS += -DBROM_SUPPORT
CFLAGS += -D__MTK_TARGET__ -DFLASH_TOOL_SUPPORT
CFLAGS += -D__USB_ENABLE__ 
CFLAGS += -DCHIP_ID=\"MT8135\"
CFLAGS += -DTEST_RAM_COMPLEX
CFLAGS += -DENABLE_UART4
#CFLAGS += -DMT8135_FPGA_FEATURE_ONLY
CFLAGS += -DPMIC_WRAP_SUPPORT
ifeq (yes,$(DA_RUN_IN_DRAM))
CFLAGS += -DMT8135_DOWNLOAD_DA_TO_DRAM
endif
$(if $(FLASH),$(eval CFLAGS += -D$(FLASH)),)
ifeq (yes,$(UT))
    CFLAGS += -DPLATFORM_TEST 
endif
CFLAGS += -DBB_$(BBCHIP)

CLFAGS += -DDISABLE_NFI_DMA
CFLAGS += -IC:\Progra~1\DS-5\include

CFLAGS += --feedback ./custom/$(BBCHIP)/$(BBCHIP)$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)_fb.txt

ifeq (yes,$(DA_RUN_IN_DRAM))
SCATTER = config/settings/$(BBCHIP)/LINK_DAINDRAM.LD
else
SCATTER = config/settings/$(BBCHIP)/LINK.LD
endif

LDFLAGS     = --scatter $(SCATTER) -symbols --xref --map --remove \
                --libpath c:/progra~1/DS-5/lib \
				--keep g_Signature \
				--keep g_FreeIntSysRAMBeginAddr \
				--keep g_IntSysRAMEndAddr \
				--keep g_FreeIntTCMBeginAddr \
				--keep g_ITCMEndAddr \
				--keep g_DTCMEndAddr \
				--keep g_FreeExtRAMBeginAddr \
                --symdefs $(OUTPUT_MAP) --list $(OUTPUT_TXT)

LDFLAGS += --feedback ./custom/$(BBCHIP)/$(BBCHIP)$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)_fb.txt

ASFLAGS    := --debug --cpu CORTEX-A7 $(if $(LINUX),,--apcs /interwork) --no_unaligned_access --fpu=SoftVFP
ifeq ($(UT),yes)
#ASFLAGS	   +=  --pd "__JUMP_DA_WORKAROUND__ SETL {TRUE}"
endif
ARFLAGS    :=

# Support Features 

# FEATURES.SF_SUPPORT := y
# FEATURES.NOR_SUPPORT := y
FEATURES.NAND_SUPPORT := y
FEATURES.EMMC_SUPPORT := y
FEATURES.EMMC_ADDR_TRANS := y
FEATURES.SDMMC_SUPPORT := y
#FEATURES.COMBO_MCP := y
ifeq ($(DA_PL),yes)
FEATURES.SEC_USBDL_WO_SLA := y
else
ifeq ($(SW_SEC_BOOT),yes)
FEATURES.SW_SEC_BOOT := y
endif
endif
