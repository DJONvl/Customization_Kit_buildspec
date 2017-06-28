ADSDIR          :=

ifeq (MT6577,$(BBCHIP))
BBCHIP_BASE     := MT6575
else
BBCHIP_BASE     := $(word 1,$(subst _, ,$(BBCHIP)))
endif
#if we want to use environment variable to config ADS PATH, use following code
#ADSDIR          := $(if $(ARMHOME),$(ARMHOME),\
#                   c:/progra~1/arm/ADSv1_2)

ifeq ($(SPLIT_DA_METHOD), legacy)
LIBRARY_SUFFIX := 
endif
ifeq ($(SPLIT_DA_METHOD), init)
LIBRARY_SUFFIX := _INIT
endif
ifeq ($(SPLIT_DA_METHOD), loop)
LIBRARY_SUFFIX := _LOOP
endif

# Executables and Pathes
CC              :=
AS              :=
LD              :=
AR              :=
DISASSEMBLE     :=
BIN             :=
LIB             :=
MKDIR           := $(if $(LINUX),mkdir,"$(word 1,$(wildcard ../Tools/mkdir.exe Tools/mkdir.exe) mkdir.exe)")
RM              := $(if $(LINUX),rm,"$(word 1,$(wildcard ../Tools/rm.exe    Tools/rm.exe)    rm.exe)")
ZIP             := $(if $(LINUX),zip,"$(word 1,$(wildcard ../Tools/zip.exe   Tools/zip.exe)   zip.exe)")
CAT             := $(if $(LINUX),cat,"$(word 1,$(wildcard ../Tools/cat.exe   Tools/cat.exe)   cat.exe)")
MV              := $(if $(LINUX),mv,"$(word 1,$(wildcard ../Tools/mv.exe    Tools/mv.exe)    mv.exe)")
CP              := $(if $(LINUX),cp,"$(word 1,$(wildcard ../Tools/cp.exe    Tools/cp.exe)    cp.exe)")
CHMOD           := $(if $(LINUX),chmod,"$(word 1,$(wildcard ../Tools/chmod.exe    Tools/chmod.exe)    chmod.exe)")
SED           := $(if $(LINUX),sed,"$(word 1,$(wildcard ../Tools/sed.exe  Tools/sed.exe)    sed.exe)")
FIND			:= $(if $(LINUX),find,"$(word 1,$(wildcard ../Tools/find.exe Tools/find.exe) find.exe)")
XARGS			:= $(if $(LINUX),xargs,"$(word 1,$(wildcard ../Tools/xargs.exe Tools/xargs.exe) xargs.exe)")
 
ADD_DA_INFO     	:= build/tools/AddDAInfo_SV5.exe
SCATTER_INIT_DEMO	:=config/settings/generic/LINK.LD_demo.INIT
SCATTER_LOOP_DEMO	:=config/settings/generic/LINK.LD_demo.LOOP

TEXT_LEN			 := 1M
BSS_LEN 			 := 1M


ADDRS	 :=DA_DRAM_BASE_ADDRESS DA_INIT_STACK_ADDRESS COMPACT_DA_USB_BUFFER_BASE_CONFIG COMPACT_DA_USB_BUFFER_SIZE_CONFIG COMPACT_DA_FLASH_READ_BUFFER_BASE_CONFIG COMPACT_DA_FLASH_READ_BUFFER_SIZE_CONFIG


BUILD_LOG       := "buildlog.txt"

# Source Code and Include Directory 
D_ROOT          := .

# Default FLASH
FLASH = 

#Default non UT mode
UT =

DA_VERSION      :=
DA_SIG_SYMBOL   :=
DA_INFO_INI     :=

ADD_DA_INFO     :=
SIGN_DA         :=
SIGN_DA_EXE     :=

OUTPUT_PREFIX   :=
# Output Files
OUTPUT_BIN      :=
OUTPUT_BIN_DOS  :=
OUTPUT_LIB      :=
OUTPUT_LIB_INFO :=
OUTPUT_ELF      :=
OUTPUT_DIS      :=
OUTPUT_TXT      :=
OUTPUT_MAP      :=
OUTPUT_SIG      :=

# customization kit 
PLATFORM_KIT_ZIP := Platform_Kit_$(BBCHIP).zip
CUSTOM_KIT_ZIP   := Customization_Kit_$(BBCHIP).zip

INCLUDE_PATH    :=  

SOURCE_PATH     := 

LIBRARY         :=

CFLAGS          :=
SCATTER         :=
LDFLAGS         := 
ASFLAGS         :=
ARFLAGS         :=


# default source path list ..
COMMON_LIB_PATH   := 

PLATFORM_LIB_SYS_PATH := \
	platform/$(BBCHIP_BASE)/system/common \
    $(wildcard platform/$(BBCHIP_BASE)/system/modules/*) 


PLATFORM_LIB_PATH := \
    platform/$(BBCHIP_BASE) \
    platform/$(BBCHIP) \
    platform/$(BBCHIP_BASE)/common \
	platform/$(BBCHIP)/common \
	platform/$(BBCHIP)/emi \
	platform/$(BBCHIP)/main \
	platform/$(BBCHIP)/misc \
	platform/$(BBCHIP)/mmc \
	platform/$(BBCHIP)/ufs \
	platform/$(BBCHIP)/nand \
	platform/$(BBCHIP)/nor \
	platform/$(BBCHIP)/security \
	platform/$(BBCHIP)/test \
	platform/$(BBCHIP)/usb

CUSTOMER_PATH     := \
    custom/$(BBCHIP) \
    custom/$(BBCHIP_BASE) \
    custom/$(BBCHIP)/common/* \
	custom/$(BBCHIP)/common \
    custom/$(BBCHIP_BASE)/common/* \
	custom/$(BBCHIP_BASE)/common 

# if overwrite is needed, please overwrite following variables
SOURCE_COMMON     := 
SOURCE_PLATFORM   := $(call all-source-in,$(addsuffix /src/,$(PLATFORM_LIB_PATH))) \
                     $(call all-source-in,$(PLATFORM_LIB_PATH))
SOURCE_PLATFORM   += $(strip $(call all-source-in,$(addsuffix /src/,$(PLATFORM_LIB_SYS_PATH))) \
                     $(call all-source-in,platform/$(BBCHIP_BASE)/system/common/src/debug/))
					 
SOURCE_CUSTOMER   := $(call all-source-in,$(addsuffix /src/,$(CUSTOMER_PATH)))
INCLUDE_PATH      := $(strip $(wildcard $(addsuffix /inc/,$(PLATFORM_LIB_SYS_PATH))) \
                     $(wildcard $(addsuffix /inc/,$(PLATFORM_LIB_PATH))) \
					 $(wildcard $(PLATFORM_LIB_PATH)) \
                     $(wildcard $(addsuffix /inc/,$(CUSTOMER_PATH))) \
                     $(wildcard ../include include) \
                     custom/$(BBCHIP)/inc \
					 platform/$(BBCHIP_BASE)/system/common/inc \
                     platform/$(BBCHIP_BASE)/system/common/inc/debug \
                     platform/$(BBCHIP_BASE)/system/common/inc/security \
                     platform/$(BBCHIP_BASE)/system/common/inc/typedef \
                     platform/$(BBCHIP_BASE)/system/modules/gpt/inc \
                     platform/$(BBCHIP_BASE)/system/modules/nor/inc \
                     platform/$(BBCHIP_BASE)/system/modules/nand/inc \
                     platform/$(BBCHIP_BASE)/system/modules/emmc/inc \
                     platform/$(BBCHIP_BASE)/system/modules/sf/inc \
                     platform/$(BBCHIP_BASE)/system/modules/usb/inc \
	                 platform/$(BBCHIP_BASE)/emi \
	                 platform/$(BBCHIP_BASE)/main \
	                 platform/$(BBCHIP_BASE)/misc \
	                 platform/$(BBCHIP_BASE)/mmc \
					 platform/$(BBCHIP_BASE)/ufs \
	                 platform/$(BBCHIP_BASE)/nor \
	                 platform/$(BBCHIP_BASE)/nand \
	                 platform/$(BBCHIP_BASE)/security \
	                 platform/$(BBCHIP_BASE)/test \
	                 platform/$(BBCHIP_BASE)/usb \
                     )
# custom/$(BBCHIP)/inc can't be wildcarded, since it might not be existed when wildcarding

CUSTOM          := MTK
-include build/core/configs/custom.mk
CUSTOM          := $(strip $(CUSTOM))
OUTPUT_PREFIX   := MTK_$(if $(BBCHIP),$(BBCHIP),Default)$(if $(FLASH),_$(FLASH),)_DA
OUTPUT_BIN      := bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin
OUTPUT_BIN_DOS  := bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)\\$(OUTPUT_PREFIX).bin
OUTPUT_ELF      := $(OUTPUT_BIN)/$(OUTPUT_PREFIX).elf
OUTPUT_DIS      := $(OUTPUT_BIN)/$(OUTPUT_PREFIX).dis
OUTPUT_TXT      := $(OUTPUT_BIN)/$(OUTPUT_PREFIX)_list.txt
OUTPUT_MAP      := $(OUTPUT_BIN)/$(OUTPUT_PREFIX).map
OUTPUT_SIG      := lib/$(OUTPUT_PREFIX)$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,).sig
OUTPUT_COMMON_LIB        := 
OUTPUT_COMMON_LIB_INFO   := 
OUTPUT_PLATFORM_LIB      := bin/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform$(LIBRARY_SUFFIX).a
OUTPUT_PLATFORM_LIB_INFO := bin/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform$(LIBRARY_SUFFIX).a.txt
SEC_LIB			 := fw_ver_$(BBCHIP_BASE).a
SEC_LIB2			 := fw_ver_$(BBCHIP_BASE)_s.a

LIBRARY := $(OUTPUT_COMMON_LIB) $(OUTPUT_PLATFORM_LIB) lib/$(SEC_LIB) lib/$(SEC_LIB2) lib/fdm.a
NSLA_LIBRARY := 
NSLA_LIBRARY += bin_nsla/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform.a
NSLA_LIBRARY += bin_nsla/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform.a.txt
NSLA_LIBRARY += bin_nsla/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform_INIT.a
NSLA_LIBRARY += bin_nsla/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform_LOOP.a.txt

SWSLA_LIBRARY :=
SWSLA_LIBRARY += bin_swsla/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform.a
SWSLA_LIBRARY += bin_swsla/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform.a.txt
SWSLA_LIBRARY += bin_swsla/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform_INIT.a
SWSLA_LIBRARY += bin_swsla/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform_LOOP.a.txt

include build/core/configs/features.mk
$(call all-features-off)
-include build/core/configs/$(BBCHIP).mk
include build/core/autoconfig.mk
                  
custom-kits  := build/core/*.mk build/core/configs/*.mk \
				build/tools/* \
                config/settings/$(BBCHIP)/ \
				config/settings/generic/* \
                custom/$(BBCHIP)/* \
                custom/$(BBCHIP_BASE)/* \
                platform/$(BBCHIP_BASE)/system/common/inc/UART.H \
                platform/$(BBCHIP_BASE)/system/common/inc/hw_config.h \
                platform/$(BBCHIP_BASE)/system/common/inc/main_nand.h \
                make.exe \
                Makefile \
                signature.mk sig-pack.mk package.bat sig.bat \
                bin/MTK_$(BBCHIP)_DA.bin \
                bin/MTK_$(BBCHIP_BASE)_DA.bin \
                bin_nsla/MTK_$(BBCHIP)_DA.bin \
                bin_nsla/MTK_$(BBCHIP_BASE)_DA.bin \
                bin_swsla/MTK_$(BBCHIP)_DA.bin \
                bin_swsla/MTK_$(BBCHIP_BASE)_DA.bin \
                bin/MTK_$(BBCHIP)_EPP.bin \
                bin/MTK_$(BBCHIP_BASE)_EPP.bin \
                bin_nsla/MTK_$(BBCHIP)_EPP.bin \
                bin_nsla/MTK_$(BBCHIP_BASE)_EPP.bin \
                bin_swsla/MTK_$(BBCHIP)_EPP.bin \
                bin_swsla/MTK_$(BBCHIP_BASE)_EPP.bin \
                lib/MTK_$(BBCHIP)_DA.sig \
                lib/MTK_$(BBCHIP)_DA_nsla.sig \
                lib/MTK_$(BBCHIP)_DA_swsla.sig \
                lib/* \
                config/settings/$(BBCHIP)/* \
				$(LIBRARY) $(OUTPUT_COMMON_LIB_INFO) $(OUTPUT_PLATFORM_LIB_INFO)\
                ../Tools/* \
                ../include/* \
                custom/security\usbdl4enduser_dummy/* \
                $(NSLA_LIBRARY) \
                $(SWSLA_LIBRARY)
custom-kits		+= platform/$(BBCHIP_BASE)/misc/UART.H platform/$(BBCHIP_BASE)/misc/timer.h platform/$(BBCHIP_BASE)/misc/logging.h platform/$(BBCHIP_BASE)/emi/emi.h  platform/$(BBCHIP_BASE)/main/hw_config.h platform/$(BBCHIP_BASE)/main/kal_release.h platform/$(BBCHIP_BASE)/main/reg_base.h          
platform-kits  := build/ config/settings/$(BBCHIP)/ \
                  platform/cmm \
                  platform/common/* \
                  platform/$(BBCHIP)/* \
                  platform/$(BBCHIP_BASE)/* \
                  custom/$(BBCHIP)/* \
                  custom/$(BBCHIP_BASE)/* \
                  platform/$(BBCHIP_BASE)/system/common/inc/UART.H \
                  platform/$(BBCHIP_BASE)/system/common/inc/hw_config.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/main_nand.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/kal_release.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/UART_HW.H \
                  platform/$(BBCHIP_BASE)/system/common/inc/timer.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/drv_comm.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/usb_hw.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/security/security_export.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/security/sb_encode.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/external_memory_config.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/reg_base.H \
                  platform/$(BBCHIP_BASE)/system/common/inc/intrCtrl.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/BLOADER.H \
                  platform/$(BBCHIP_BASE)/system/common/inc/bmd.h \
                  platform/$(BBCHIP_BASE)/system/common/inc/test_main.h \
                  platform/$(BBCHIP_BASE)/system/modules/gpt/inc/gpt.h \
                  platform/$(BBCHIP_BASE)/system/modules/usb/inc/* \
                  make.exe \
                  doc \
                  Makefile \
                  buildspec.mk \
                  signature.mk sig-pack.mk package.bat sig.bat \
				  bin/MTK_$(BBCHIP)_DA.bin \
				  bin_nsla/MTK_$(BBCHIP)_DA.bin \
				  bin_swsla/MTK_$(BBCHIP)_DA.bin \
                  bin/MTK_$(BBCHIP)_EPP.bin \
                  bin_nsla/MTK_$(BBCHIP)_EPP.bin \
                  bin_swsla/MTK_$(BBCHIP)_EPP.bin \
                  lib/MTK_$(BBCHIP)_DA.sig \
                  lib/MTK_$(BBCHIP)_DA_nsla.sig \
                  lib/MTK_$(BBCHIP)_DA_swsla.sig \
                  $(LIBRARY) $(OUTPUT_COMMON_LIB_INFO) $(OUTPUT_PLATFORM_LIB_INFO)\
                  ../Tools/* \
                  ../include/* \
                  custom/security\usbdl4enduser_dummy/* \
                  $(NSLA_LIBRARY) \
                  $(SWSLA_LIBRARY)

#ifneq (,$(filter MT6516_AP MT6516_MD MT6516_NU,$(BBCHIP)))
#custom-kits += bin/MTK_MT6516_EPP.bin
#platform-kits += bin/MTK_MT6516_EPP.bin
#endif

#ifneq (,$(filter MT6516_NU,$(BBCHIP)))
#custom-kits += custom_nu.bat
#endif



custom-kits    += bin/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform_LOOP.a
platform-kits  += bin/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform_LOOP.a
custom-kits    += bin/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform_INIT.a
platform-kits  += bin/$(OUTPUT_PREFIX)$(DOWNLOAD_STYLE)_platform_INIT.a


custom-kits := $(wildcard $(custom-kits))
custom-kits := $(filter-out $(wildcard build/core/configs/MT*.mk), $(custom-kits))
custom-kits += $(wildcard build/core/configs/$(BBCHIP).mk)
platform-kits := $(wildcard $(platform-kits))
platform-kits := $(filter-out $(wildcard build/core/configs/MT*.mk), $(platform-kits))
platform-kits += $(wildcard build/core/configs/$(BBCHIP).mk)
