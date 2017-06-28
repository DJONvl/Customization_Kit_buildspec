OUTPUT_BIN		:= bin$(if $(DA_PL),_nsla,)/MTK_$(.BBCHIP)_DA.bin
OUTPUT_EPP_BIN	:= bin$(if $(DA_PL),_nsla,)/MTK_$(.BBCHIP)_EPP.bin
DAINFO_INFO_DEMO:= platform/dainfo/da$(if $(DA_PL),_nsla,).info
DAINFO			:= temp/da$(if $(DA_PL),_nsla,).info
DAINFO_OLD		:= ../config/settings/$(.BBCHIP)/da$(if $(DA_PL),_nsla,).info

DA_INFO			:= temp/$(.BBCHIP)/da$(if $(DA_PL),_nsla,).info

OUTPUT_TEMP		:= temp/$(.BBCHIP)
AUTOCONFIG      := temp/$(.BBCHIP)/autoconfig.h
LINK_LD_LOOP 	:= temp/$(.BBCHIP)/system-bootstrap2.ld
LINK_LD_INIT 	:= temp/$(.BBCHIP)/system-bootstrap1.ld

SECURTY_LIB_OBJ_INIT := boot/security/security_lib_init.o
SECURTY_LIB_OBJ_LOOP := boot/security/security_lib_loop.o platform/$(CODE_BASE)/security/sec_lib.o

SECURTY_LIB_INIT	:= platform/$(CODE_BASE)/sec_lib/libsec_init.a
SECURTY_LIB_LOOP	:= platform/$(CODE_BASE)/sec_lib/libsec_loop.a

CUSTOM_LIB_BR		:= custom/$(CODE_BASE)/$(.BBCHIP)_custom_BR.o
CUSTOM_LIB_PL		:= custom/$(CODE_BASE)/$(.BBCHIP)_custom_PL.o

PLATFORM_LIB_INIT	:= platform/$(CODE_BASE)/$(.BBCHIP)_platform_init.o
PLATFORM_LIB_LOOP	:= platform/$(CODE_BASE)/$(.BBCHIP)_platform_loop.o

DA_INIT_LIB 	:= $(ARCH_ARM_LIB_INIT) $(KERNEL_LIB_INIT) $(BOOT_LIB_INIT) $(DRIVER_LIB_INIT) $(LIB_LIB) $(PLATFORM_LIB_INIT)
DA_LOOP_LIB		:= $(ARCH_ARM_LIB_LOOP) $(KERNEL_LIB_LOOP) $(BOOT_LIB_LOOP) $(DRIVER_LIB_LOOP) $(LIB_LIB) $(PLATFORM_LIB_LOOP)
ifneq ($(CREATE_SEC_LIB),yes)	
DA_LOOP_LIB		+= $(SECURTY_LIB_LOOP)
DA_INIT_LIB		+= $(SECURTY_LIB_INIT)
endif

ifeq ($(DA_PL),yes)	
DA_LOOP_LIB		+= $(CUSTOM_LIB_PL)
DA_INIT_LIB		+= $(CUSTOM_LIB_PL)
else
DA_LOOP_LIB		+= $(CUSTOM_LIB_BR)
DA_INIT_LIB		+= $(CUSTOM_LIB_BR)
endif

OUTPUT_SIG      := $(OUTPUT_TEMP)/MTK_$(.BBCHIP)_DA$(if $(DA_PL),_nsla,).sig
SIGN_DA         := $(SIGN_DA_EXE) SHA256 $(OUTPUT_BIN) INT_SYSRAM,INT_TCM,D_TCM,EXT_RAM $(OUTPUT_SIG)


CFLAGS := 
CFLAGS += -D__FLASH_TOOL_DA__

INCLUDE_PATH := 

INCLUDE_PATH   := $(strip 	\
		$(TOPDIR)/		\
		$(TOPDIR)/include 	\
		$(TOPDIR)/arch/arm/include 	\
		$(TOPDIR)/../../Raphael-inc 	\
		$(TOPDIR)/../Raphael-inc 	\
		$(TOPDIR)/driver/ufs \
		)		

INCLUDE_PATH    += $(strip 	\
				$(TOPDIR)/platform/$(CODE_BASE)/ \
				$(TOPDIR)/custom/$(CODE_BASE)/nand \
				$(TOPDIR)/$(OUTPUT_TEMP) \
				$(TOPDIR)/platform/$(CODE_BASE)/dev/ufs \
				)
				
DA_CURRENT_DATE_ORIGINAL := $(shell date /T)
DA_CURRENT_DATE          := $(firstword $(DA_CURRENT_DATE_ORIGINAL))
DA_CURRENT_TIME_ORIGINAL := $(shell time /T)
DA_CURRENT_TIME          := $(firstword $(DA_CURRENT_TIME_ORIGINAL))
DA_VERSION               := 3.3001.$(DA_CURRENT_DATE).$(DA_CURRENT_TIME)
DA_SIG_SYMBOL   		 := g_Signature
