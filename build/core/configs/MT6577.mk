ADSDIR          := c:/progra~1/ARM/RVCT/Programs/4.1/462/win_32-pentium

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

# customization kit 
CUSTOM_KIT_ZIP = Customization_Kit_$(BBCHIP).zip

CFLAGS += $(addprefix -I,$(INCLUDE_PATH))

DEBUG_ON = --debug

CFLAGS += --thumb $(DEBUG_ON) -I$(D_ROOT)/INC --cpu CORTEX-A9 $(if $(LINUX),,--apcs /interwork) --littleend 
CFLAGS += -O2 -c --no_unaligned_access -Ono_memcpy --fpu=SoftVFP
CFLAGS += -DDISABLE_NFI_DMA
CFLAGS += -DDISABLE_TOTAL_CHKSUM
CFLAGS += -DBROM_SUPPORT
CFLAGS += -D__MTK_TARGET__ -DFLASH_TOOL_SUPPORT
CFLAGS += -D__USB_ENABLE__ 
CFLAGS += -DENABLE_UART4 -DCHIP_ID=\"MT6577E1\"
$(if $(FLASH),$(eval CFLAGS += -D$(FLASH)),)
ifeq (yes,$(UT))
    CFLAGS += -DPLATFORM_TEST 
endif
CFLAGS += -DBB_$(BBCHIP) -DBB_MT6575 -DBB_MT6575_E2

CLFAGS += -DDISABLE_NFI_DMA

ifeq ($(DA_PL),yes)
CFLAGS += --feedback ./custom/$(BBCHIP)/$(BBCHIP)_da_pl_fb.txt
else
ifeq ($(SW_SEC_BOOT),yes)
CFLAGS += --feedback ./custom/$(BBCHIP)/$(BBCHIP)_da_sw_fb.txt
endif
endif

SCATTER = config/settings/$(BBCHIP)/LINK.LD

LDFLAGS     = --scatter $(SCATTER) --libpath $(LIB) -symbols --xref --map --remove \
                --libpath c:/progra~1/ARM/RVCT/Data/4.1/462/lib \
				--keep g_Signature \
				--keep g_FreeIntSysRAMBeginAddr \
				--keep g_IntSysRAMEndAddr \
				--keep g_FreeIntTCMBeginAddr \
				--keep g_ITCMEndAddr \
				--keep g_DTCMEndAddr \
				--keep g_FreeExtRAMBeginAddr \
                --symdefs $(OUTPUT_MAP) --list $(OUTPUT_TXT)

ifeq ($(DA_PL),yes)
LDFLAGS += --feedback ./custom/$(BBCHIP)/$(BBCHIP)_da_pl_fb.txt
else
ifeq ($(SW_SEC_BOOT),yes)
LDFLAGS += --feedback ./custom/$(BBCHIP)/$(BBCHIP)_da_sw_fb.txt
endif
endif

ASFLAGS    := --debug --cpu CORTEX-A9 $(if $(LINUX),,--apcs /interwork) --no_unaligned_access --fpu=SoftVFP
ARFLAGS    :=

# Support Features 

# FEATURES.SF_SUPPORT := y
# FEATURES.NOR_SUPPORT := y
FEATURES.NAND_SUPPORT := y
FEATURES.EMMC_SUPPORT := y
FEATURES.EMMC_ADDR_TRANS := y
FEATURES.SDMMC_SUPPORT := y
FEATURES.COMBO_MCP := y
ifeq ($(DA_PL),yes)
FEATURES.SEC_USBDL_WO_SLA := y
else
ifeq ($(SW_SEC_BOOT),yes)
FEATURES.SW_SEC_BOOT := y
endif
endif