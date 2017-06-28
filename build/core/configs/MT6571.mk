GCCDIR          := c:/progra~1/GCC/arm-2015q1/bin

# Executables and Pathes
CC              := $(if $(LINUX),cc,"$(GCCDIR)/arm-none-eabi-gcc.exe")
AS              := $(if $(LINUX),as,"$(GCCDIR)/arm-none-eabi-as.exe")
LD              := $(if $(LINUX),ld,"$(GCCDIR)/arm-none-eabi-ld.exe")
AR              := $(if $(LINUX),ar,"$(GCCDIR)/arm-none-eabi-ar.exe")
OBJDUMP         := $(if $(LINUX),fromelf,"$(GCCDIR)/arm-none-eabi-objdump.exe")
OBJCOPY         := $(if $(LINUX),fromelf,"$(GCCDIR)/arm-none-eabi-objcopy.exe")

# Source Code and Include Directory 
D_ROOT          := .

#DA_CURRENT_DATE_DAY := $(shell date /T)
#DA_CURRENT_DATE     := $(firstword $(DA_CURRENT_DATE_DAY))
#DA_CURRENT_TIME     := $(shell time /T)
#DA_VERSION      := 3.3001.$(DA_CURRENT_DATE).$(DA_CURRENT_TIME)

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

# reset FLAGS
CFLAGS := 
LDFLAGS := 
ASFLAGS :=
DA_SRAM_BASE_ADDRESS := 0x2008000
DA_DRAM_BASE_ADDRESS := 0x80000000

# Default FLASH
FLASH = 
$(if $(FLASH),$(eval CFLAGS += -D$(FLASH)),)

#for gcc in makefile
USE_GCC := yes


ifeq ($(SPLIT_DA_METHOD), init)
	CFLAGS += -DDA_INIT -Os
	SCATTER := config/settings/$(BBCHIP)/LINK.LD.INIT
	LDFLAGS += -e Start
endif
ifeq ($(SPLIT_DA_METHOD), loop)
	CFLAGS += -O2
	SCATTER := config/settings/$(BBCHIP)/LINK.LD.LOOP
	LDFLAGS += -e StartDram
endif


CFLAGS += $(addprefix -I,$(INCLUDE_PATH)) -I c:/progra~1/GCC/arm-2015q1/arm-none-eabi/include


CFLAGS += -mcpu=cortex-a7 -mthumb -mthumb-interwork -mno-unaligned-access
CFLAGS += -g -c -Wall -ffunction-sections -fdata-sections
CFLAGS += -mfloat-abi=softfp -mfpu=neon
CFLAGS += -DDISABLE_NFI_DMA
CFLAGS += -DDISABLE_TOTAL_CHKSUM
CFLAGS += -DBROM_SUPPORT
CFLAGS += -D__MTK_TARGET__ -DFLASH_TOOL_SUPPORT
CFLAGS += -D__USB_ENABLE__ 
CFLAGS += -DDL_INFO_IN_NAND
CFLAGS += -DENABLE_UART4 -DCHIP_ID=\"MT6571\"

######## Use this macro for FPGA test
##CFLAGS += -DCFG_FPGA_PLATFORM
##CFLAGS += -DCFG_USE_FIXED_EMI
###CFLAGS += -DDISABLE_SECURITY_CHECK
CFLAGS += -DNAND_SPI 


$(if $(FLASH),$(eval CFLAGS += -D$(FLASH)),)
ifeq (yes,$(UT))
    CFLAGS += -DPLATFORM_TEST 
endif
CFLAGS += -DBB_$(BBCHIP) -DBB_MT6571

CLFAGS += -DDISABLE_NFI_DMA
CFLAGS += -DUSE_GCC
CFLAGS += -DDUMP_SPEED


LD_GCC_LIBS := $(GCCDIR)/../lib/gcc/arm-none-eabi/4.9.3/thumb/crti.o \
  $(GCCDIR)/../lib/gcc/arm-none-eabi/4.9.3/thumb/crtbegin.o \
  $(GCCDIR)/../lib/gcc/arm-none-eabi/4.9.3/thumb/crtend.o \
  $(GCCDIR)/../lib/gcc/arm-none-eabi/4.9.3/thumb/crtn.o \
  -L$(GCCDIR)/../lib/gcc/arm-none-eabi/4.9.3/thumb \
  -L$(GCCDIR)/../lib/gcc/arm-none-eabi/4.9.3/../../../../arm-none-eabi/lib/thumb \
  -L$(GCCDIR)/../arm-none-eabi/lib/thumb \
  -L$(GCCDIR)/../lib/gcc/arm-none-eabi/4.9.3 \
  -L$(GCCDIR)/../lib/gcc \
  -L$(GCCDIR)/../lib/gcc/arm-none-eabi/4.9.3/../../../../arm-none-eabi/lib \
  -L$(GCCDIR)/../arm-none-eabi/lib \
  -lgcc -lc
LDFLAGS += -T $(SCATTER) $(LD_GCC_LIBS)
LDFLAGS += --gc-sections --cref --print-map -Map bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)\MTK_$(BBCHIP)_DA.bin\MTK_$(BBCHIP)_DA_list.txt
LDFLAGS += --relax
LDFLAGS += --print-gc-sections --print-output-format
LDFLAGS += --verbose


ASFLAGS    := -mcpu=cortex-a7 -mthumb-interwork -mfloat-abi=softfp -mfpu=neon -g


# Support Features 

# FEATURES.SF_SUPPORT := y
# FEATURES.NOR_SUPPORT := y
FEATURES.NAND_SUPPORT := y
FEATURES.EMMC_SUPPORT := y
FEATURES.SDMMC_SUPPORT := y
FEATURES.COMBO_MCP := y
FEATURES.SPLIT_DA_SUPPORT := y
ifeq ($(DA_PL),yes)
FEATURES.SEC_USBDL_WO_SLA := y
else
ifeq ($(SW_SEC_BOOT),yes)
FEATURES.SW_SEC_BOOT := y
endif
endif

