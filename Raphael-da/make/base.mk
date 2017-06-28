# Executables and Pathes
GCCDIR          := c:/progra~1/GCC/arm-2015q1/bin
#GCCDIR          := c:/progra~1/GCC/arm-2012.09/bin
CC              := $(GCCDIR)/arm-none-eabi-gcc.exe
AS              := $(GCCDIR)/arm-none-eabi-as.exe
LD              := $(GCCDIR)/arm-none-eabi-ld.exe
AR              := $(GCCDIR)/arm-none-eabi-ar.exe
OBJDUMP         := $(GCCDIR)/arm-none-eabi-objdump.exe
OBJCOPY         := $(GCCDIR)/arm-none-eabi-objcopy.exe
READELF         := $(GCCDIR)/arm-none-eabi-readelf.exe
JOBS			:= -j32

TEE			:= "$(word 1,$(wildcard ../../../Tools/tee.exe ../../Tools/tee.exe ../Tools/tee.exe ) tee.exe)"
PWD			:= "$(word 1,$(wildcard ../../../Tools/pwd.exe ../../Tools/pwd.exe ../Tools/pwd.exe) pwd.exe)"
SED         := "$(word 1,$(wildcard ../../../Tools/sed.exe ../../Tools/sed.exe ../Tools/sed.exe) sed.exe)"
MKDIR       := "$(word 1,$(wildcard ../../../Tools/mkdir.exe ../../Tools/mkdir.exe ../Tools/mkdir.exe) mkdir.exe)"
RM          := "$(word 1,$(wildcard ../../../Tools/rm.exe  ../../Tools/rm.exe ../Tools/rm.exe) rm.exe)"
ZIP         := "$(word 1,$(wildcard ../../../Tools/zip.exe ../../Tools/zip.exe ../Tools/zip.exe) zip.exe)"
CAT         := "$(word 1,$(wildcard ../../../Tools/cat.exe ../../Tools/cat.exe ../Tools/cat.exe) cat.exe)"
MV          := "$(word 1,$(wildcard ../../../Tools/mv.exe ../../Tools/mv.exe ../Tools/mv.exe) mv.exe)"
TR          := "$(word 1,$(wildcard ../../../Tools/tr.exe ../../Tools/tr.exe ../Tools/tr.exe) tr.exe)"
CP          := "$(word 1,$(wildcard ../../../Tools/cp.exe ../../Tools/cp.exe ../Tools/cp.exe) cp.exe)"  
CHMOD       := "$(word 1,$(wildcard ../../../Tools/chmod.exe ../../Tools/chmod.exe ../Tools/chmod.exe) chmod.exe)"
FIND		:= "$(word 1,$(wildcard ../../../Tools/find.exe ../../Tools/find.exe ../Tools/find.exe) find.exe)"
XARGS		:= "$(word 1,$(wildcard ../../../Tools/xargs.exe ../../Tools/xargs.exe ../Tools/xargs.exe) xargs.exe)"
TOPDIR		:= $(shell $(PWD))

ADD_DA_INFO_EXE     :=  "$(word 1,$(wildcard ../build/tools/AddDAInfo_SV5.exe ./build/tools/AddDAInfo_SV5.exe) AddDAInfo_SV5.exe)"
SIGN_DA_EXE     	:=  "$(word 1,$(wildcard ../build/tools/SignDA_SV5.exe ./build/tools/SignDA_SV5.exe) SignDA_SV5.exe)"
HASH_GEN_EXE		:=  "$(word 1,$(wildcard ../build/tools/Hash_Gen.exe ./build/tools/Hash_Gen.exe) Hash_Gen.exe)"

KERNEL_LIB_INIT		:= kernel/kernel_lib_init.o
KERNEL_LIB_LOOP		:= kernel/kernel_lib_loop.o

BOOT_LIB_INIT		:= boot/boot_lib_init.o
BOOT_LIB_LOOP		:= boot/boot_lib_loop.o

DRIVER_LIB_INIT		:= driver/driver_lib_init.o
DRIVER_LIB_LOOP		:= driver/driver_lib_loop.o

LIB_LIB				:= lib/lib.o
ARCH_ARM_LIB_INIT	:= arch/arm/arch_arm_lib_init.o
ARCH_ARM_LIB_LOOP	:= arch/arm/arch_arm_lib_loop.o


FEATURES 	:= SF_SUPPORT NOR_SUPPORT NAND_SUPPORT SDMMC_SUPPORT EMMC_SUPPORT UFS_SUPPORT   
FEATURES	+= DISABLE_NFI_DMA  DUMP_SPEED ENABLE_MSDC_DMA ENABLE_USB_DMA ENABLE_LOG_FOR_INIT_STAGE
FEATURES	+= CFG_FPGA_PLATFORM DA_MODE_FPGA DA_MODE_BRINGUP DA_MODE_NORMAL DA_MODE DA_ENABLE_SECURITY 

FEATURE_SWITCHES	:= ENABLE_EMMC_DMA ENABLE_USB_DMA ENABLE_MSDC_DMA
FEATURE_SWITCHES_INIT	:= $(FEATURE_SWITCHES) 
FEATURE_SWITCHES_LOOP	:= $(FEATURE_SWITCHES) 

ADDRS		:=	DA_INIT_BASE_ADDRESS DA_INIT_SIZE DA_INIT_HEAP_BASE_ADDRESS DA_INIT_HEAP_BASE_SIZE DA_DRAM_BASE_ADDRESS DA_DRAM_SIZE
ARM_CORE	:= ARM_WITH_CP15 ARM_ISA_ARMV4 ARM_CPU_ARM7 ARM_WITH_MMU ARM_CPU_ARM9 ARM_CPU_ARM926 ARM_ISA_ARMV5E ARM_ISA_ARMV6 ARM_CPU_ARM1136 ARM_ISA_ARMV7 ARM_WITH_VFP ARM_WITH_NEON ARM_WITH_THUMB ARM_WITH_THUMB2 ARM_WITH_CACHE ARM_WITH_L2 ENABLE_L2_SHARING

SCATTER_INIT_DEMO	:= arch/arm/ld/system-bootstrap1-demo.ld
SCATTER_LOOP_DEMO	:= arch/arm/ld/system-bootstrap2-demo.ld




export  AS CC LD AR OBJCOPY OBJDUMP ASFLAGS LDFLAGS CFLAGS
export  TOPDIR SED MKDIR RM
export 	FEATURES SCATTER_INIT_DEMO SCATTER_LOOP_DEMO  

define dump-feature
$(if $(FEATURES.$1),
	@echo #define $1 $(FEATURES.$1) >> $2
,)
endef

define dump-drivers-init
	@echo #ifdef STAGE_DA_INIT >> $2
$(if $(FEATURE_SWITCHES_INIT.$1:0=) ,
	@echo #define $1 $(FEATURE_SWITCHES_INIT.$1) >> $2
,)
	@echo #endif >> $2
	
endef

define dump-drivers-loop
	@echo #ifdef STAGE_DA_LOOP >> $2

$(if $(FEATURE_SWITCHES_LOOP.$1:0=) ,
	@echo #define $1 $(FEATURE_SWITCHES_LOOP.$1) >> $2
,)
	@echo #endif >> $2
	
endef

define dump-addrs
$(if $(ADDRS.$1),
	@echo #define  $1 $(ADDRS.$1) >> $2
,)
endef

define dump-arm_core
$(if $(ARM_CORE.$1),
	@echo #define  $1 $(ARM_CORE.$1) >> $2
,)
endef


define clean_obj
$(FIND) $1 -name *.d | $(XARGS) rm -f
$(FIND) $1 -name *.o | $(XARGS) rm -f

endef

define clean_dep
$(FIND) $1 -name *.d | $(XARGS) rm -f

endef

define cat-file
	@echo dumping $1...
	echo [DA_$(if $(filter 0,$(words $(.cat-file-count))),Default,$(words $(.cat-file-count)))] >> $2
	$(CAT) $1 >> $2
$(eval .cat-file-count := $(.cat-file-count) 0)
endef

DA_CURRENT_DATE_ORIGINAL := $(shell date /T)
DA_CURRENT_DATE          := $(firstword $(DA_CURRENT_DATE_ORIGINAL))
DA_CURRENT_TIME_ORIGINAL := $(shell time /T)
DA_CURRENT_TIME          := $(firstword $(DA_CURRENT_TIME_ORIGINAL))
DA_VERSION               := 3.3001.$(DA_CURRENT_DATE).$(DA_CURRENT_TIME)
DA_SIG_SYMBOL   		 := g_Signature

