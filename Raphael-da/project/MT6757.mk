include make/base_bbchip.mk

#first da address
ADDRS.DA_INIT_BASE_ADDRESS 	   	:= 0x200000
ADDRS.DA_INIT_SIZE		 	   	:= 0x40000

#second da address
ADDRS.DA_INIT_HEAP_BASE_ADDRESS  	:= 0x100000
ADDRS.DA_INIT_HEAP_BASE_SIZE	   	:= 0x20000

ADDRS.DA_DRAM_BASE_ADDRESS		:= 0x40000000
ADDRS.DA_DRAM_SIZE		 		:= 0x10000000
#The DA_DRAM_SIZE is not the real value,it is the size that DA want to use
#we suggest 0x10000000(256M less than real size is OK)

#add_da_info
BBCHIP_NAME					:=MT6757
BBCHIP_HW_CODE				:=0x6757
BBCHIP_HW_SUB_CODE			:=0x0000
BBCHIP_HW_VER				:=0xCA00
BBCHIP_SW_VER				:=0x0000
DA_FEATURE_SET				:=0x1000

#CFLAGS 	:=
LDFLAGS :=
ASFLAGS :=

CFLAGS += -Os
CFLAGS += -D __FLASH_TOOL_DA__

CFLAGS += -mthumb -mthumb-interwork -mno-unaligned-access
CFLAGS += -mcpu=cortex-a53 -mfloat-abi=hard -mfpu=neon

ifneq ($(BUILD_TYPE),RELEASE)
CFLAGS += -g 
endif

CFLAGS += -c -Wall -ffunction-sections -fdata-sections

CFLAGS += $(addprefix -I,$(INCLUDE_PATH)) 

LD_GCC_LIBS := -L$(GCCDIR)/../lib/gcc/arm-none-eabi/4.9.3/fpu -lgcc 
  
LDFLAGS += --gc-sections 
#LDFLAGS += --cref 
LDFLAGS += --relax 
LDFLAGS += --verbose $(LD_GCC_LIBS)


ASFLAGS += -mcpu=cortex-a53
ASFLAGS += -mthumb-interwork -mfloat-abi=hard -mfpu=neon -g
ASFLAGS += $(addprefix -I,$(INCLUDE_PATH)) 




#ARM_CPU
ARM_CPU	 :=	CORTEX_A53
ARM_CORE += ARM_CPU_$(ARM_CPU)
ARM_CORE.ARM_CPU_$(ARM_CPU)	:= 1
ARM_CORE.ENABLE_L2_SHARING	:= 1

include	arch/arm/rules.mk


# Support Features
FEATURES.NAND_SUPPORT := n
FEATURES.EMMC_SUPPORT := y
FEATURES.SDMMC_SUPPORT := y

#different DA MODE, you can only set one mode to 1  at the same time
#1:normal mode 2:bringup mode 3:fpga mode

FEATURES.DA_MODE  := DA_MODE_NORMAL

FEATURES.DA_MODE_NORMAL  := 1
FEATURES.DA_MODE_BRINGUP := 2
FEATURES.DA_MODE_FPGA 	  := 3

FEATURES.ENABLE_MSDC_DMA 	:= 1
FEATURE_SWITCHES_INIT.ENABLE_USB_DMA	:=0
FEATURE_SWITCHES_LOOP.ENABLE_USB_DMA  :=1

FEATURES.DA_ENABLE_SECURITY := 1

FEATURES.ENABLE_LOG_FOR_INIT_STAGE	:= 1

FEATURES.DISABLE_NFI_DMA 	:= 1
FEATURES.DUMP_SPEED			:= 1

FEATURES += BB_$(.BBCHIP)
FEATURES.BB_$(.BBCHIP)			:= 1

FEATURES +=MTK_LM_MODE
FEATURES.MTK_LM_MODE		:=1


