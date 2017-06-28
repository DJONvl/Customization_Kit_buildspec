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
DA_INFO_INI     := config/settings/$(BBCHIP)/AddDAInfo_MTK_AllInOne_DA_SV5_$(BBCHIP).ini
endif

ADD_DA_INFO     := build/tools/AddDAInfo_SV5.exe
SIGN_DA_EXE     := build/tools/SignDA_SV5.exe 
SIGN_DA         := $(SIGN_DA_EXE) SHA256 $(OUTPUT_BIN) INT_SYSRAM,INT_TCM,D_TCM,EXT_RAM $(OUTPUT_SIG)

# customization kit 
CUSTOM_KIT_ZIP = Customization_Kit_$(BBCHIP).zip

#INCLUDE_PATH    :=  \
#    platform/$(BBCHIP)/inc \
#	$(wildcard ../include include) \
#	platform/common/inc \
#    source/common/inc/debug \
#    source/common/inc/security \
#    source/common/inc/typedef \
#    source/common/inc \
#    source/modules/gpt/inc \
#    source/modules/nor/inc \
#    source/modules/nand/inc \
#    source/modules/sf/inc \
#    source/modules/comm/uart/inc \
#    source/modules/comm/usb/inc \
#    source/modules/test/inc \
#    custom/inc \
#    $(wildcard custom/modules/*/inc)

CFLAGS += $(addprefix -I,$(INCLUDE_PATH))

CFLAGS += --thumb $(DEBUG_ON) -I$(D_ROOT)/INC --cpu ARM1176JZ-S $(if $(LINUX),,--apcs /interwork) --littleend 
CFLAGS += -O2 -c --no_unaligned_access -Ono_memcpy --fpu=SoftVFP
CFLAGS += -DDISABLE_NFI_DMA
CFLAGS += -DDISABLE_TOTAL_CHKSUM
CFLAGS += -DBROM_SUPPORT
CFLAGS += -D__MTK_TARGET__ -DFLASH_TOOL_SUPPORT
CFLAGS += -D__USB_ENABLE__ 
CFLAGS += -DENABLE_UART4 -DCHIP_ID=\"MT6573\"
$(if $(FLASH),$(eval CFLAGS += -D$(FLASH)),)
ifeq (yes,$(UT))
    CFLAGS += -DPLATFORM_TEST 
endif
CFLAGS += -DBB_$(BBCHIP)

CLFAGS += -DDISABLE_NFI_DMA

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

ASFLAGS    := $(if $(LINUX),,--apcs /interwork) --cpu ARM1176JZ-S --no_unaligned_access --fpu=SoftVFP
ARFLAGS    :=

custom-files += $(OUTPUT_LIB)

# Support Features 

# FEATURES.SF_SUPPORT := y
# FEATURES.NOR_SUPPORT := y
FEATURES.NAND_SUPPORT := y
#FEATURES.EMMC_SUPPORT := y
#FEATURES.EMMC_ADDR_TRANS := y
FEATURES.COMBO_MCP := y
ifeq ($(DA_PL),yes)
FEATURES.SEC_USBDL_WO_SLA := y
endif