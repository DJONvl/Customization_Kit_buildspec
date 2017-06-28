default-target:build-bbchips 
build-bbchips: compiler_check $(BBCHIPS) add_da_info
ifeq ($(DA_PL),yes)
	-$(CP) -f bin_nsla/MTK_AllInOne_DA.bin bin_nsla/DA_PL_CRYPTO20.bin
endif
	@echo build passed  (DA_PL = $(DA_PL)).
	
compiler_check:
ifeq (build_ok.txt, $(wildcard build_ok.txt))
	-$(RM) build_ok.txt
endif

ifneq ($(GCCDIR), $(wildcard $(GCCDIR)))
	echo "please install c:/progra~1/GCC/arm-2015q1/bin"
	$(error compiler version error, please install c:/progra~1/GCC/arm-2015q1/bin) 
	
endif
	@echo "compiler check pass"	
	
$(BBCHIPS):
	@$(eval	.BBCHIP = $@)
	@$(eval    CODE_BASE=$(.BBCHIP))
	@$(eval -include  project/$(.BBCHIP).mk)
	@echo "start build $(.BBCHIP)"
	@echo "code base is $(CODE_BASE)"
	
ifneq ($(BUILD_TYPE),CUSTOM)	
	@$(MKDIR) -p $(OUTPUT_BIN) $(OUTPUT_EPP_BIN) $(OUTPUT_TEMP)
	
	$(call CREATE_AUTOCONFIG )
	
	$(call BUILD_LIB)
	$(call BUILD_DRIVER)
	$(call BUILD_ARCH_ARM)
	$(call BUILD_PLATFORM_LIB)
	$(call BUILD_KERNEL)
	$(call BUILD_BOOT,$(FEATURES.DA_MODE))

ifeq ($(CREATE_SEC_LIB),yes)	
	$(call UPDATE_SECURITY_LIB )
endif	

ifneq ($(CALLED_BY),DA_OLD)	
	$(call CREATE_DA_INFO_REFACTOR)
else
	$(call CREATE_DA_INFO_OLD)
endif
	
endif	
	$(call BUILD_CUSTOM_LIB)

	$(SED) -e "s/SRAM_START/$(ADDRS.DA_INIT_BASE_ADDRESS)/"  -e "s/SRAM_LEN/$(ADDRS.DA_INIT_SIZE)/" -e "s/HEAP_BASE_INIT/$(ADDRS.DA_INIT_HEAP_BASE_ADDRESS)/" -e "s/HEAP_SIZE_INIT/$(ADDRS.DA_INIT_HEAP_BASE_SIZE)/"  $(SCATTER_INIT_DEMO) > $(LINK_LD_INIT)
	$(SED) -e "s/DA_DRAM_BASE_ADDRESS/$(ADDRS.DA_DRAM_BASE_ADDRESS)/" -e "s/DA_DRAM_SIZE/$(ADDRS.DA_DRAM_SIZE)/" $(SCATTER_LOOP_DEMO)	> $(LINK_LD_LOOP)
	
	$(LD) -o $(OUTPUT_BIN)/DA_LOOP.elf $(DA_LOOP_LIB) $(LDFLAGS) -T $(LINK_LD_LOOP)
	$(OBJCOPY) -O binary -S -g $(OUTPUT_BIN)/DA_LOOP.elf 	$(OUTPUT_BIN)/EXT_RAM
	
ifneq ($(BUILD_TYPE),RELEASE)
	$(READELF) -s  $(OUTPUT_BIN)/DA_LOOP.elf > $(OUTPUT_BIN)/DA_LOOP.dis
	$(OBJDUMP) -D -S  $(OUTPUT_BIN)/DA_LOOP.elf >> $(OUTPUT_BIN)/DA_LOOP.dis
	
endif

	$(HASH_GEN_EXE) bin$(if $(DA_PL),_nsla,)/MTK_$(.BBCHIP)_DA.bin/EXT_RAM  $(OUTPUT_TEMP)/da_loop_hash$(if $(DA_PL),_nsla,).c
	$(CC) -c $(CFLAGS)	$(OUTPUT_TEMP)/da_loop_hash$(if $(DA_PL),_nsla,).c -o $(OUTPUT_TEMP)/da_loop_hash$(if $(DA_PL),_nsla,).o
	
	$(LD) -o $(OUTPUT_BIN)/DA_INIT.elf $(DA_INIT_LIB) $(LDFLAGS) $(OUTPUT_TEMP)/da_loop_hash$(if $(DA_PL),_nsla,).o -T $(LINK_LD_INIT)
	$(OBJCOPY) -O binary -S -g $(OUTPUT_BIN)/DA_INIT.elf 	$(OUTPUT_BIN)/INT_SYSRAM
	
ifneq ($(BUILD_TYPE),RELEASE)	
	$(READELF) -s  $(OUTPUT_BIN)/DA_INIT.elf > $(OUTPUT_BIN)/DA_INIT.dis
	$(OBJDUMP) -D -S  $(OUTPUT_BIN)/DA_INIT.elf >> $(OUTPUT_BIN)/DA_INIT.dis
endif	
	
	echo Memory Map of the image>  $(OUTPUT_BIN)\MTK_$(.BBCHIP)_DA_list.txt
	echo   Load Region INT_SYSRAM (Base: $(ADDRS.DA_INIT_BASE_ADDRESS), Size: 0x10000000, Max: 0x10000000, ABSOLUTE)>> $(OUTPUT_BIN)\MTK_$(.BBCHIP)_DA_list.txt
	echo   Load Region EXT_RAM (Base: $(ADDRS.DA_DRAM_BASE_ADDRESS), Size: 0x010000000, Max: 0x010000000, ABSOLUTE)>> $(OUTPUT_BIN)\MTK_$(.BBCHIP)_DA_list.txt

	$(SIGN_DA)
	$(call  PREPARE-EPP)
	@echo "1" > build_ok.txt
	@echo "build @(.BBCHIP) end"

define BUILD_KERNEL
	$(MAKE)	$(JOBS) DA_TYPE="init"  CFLAGS="$(CFLAGS)" -C kernel/	
	$(MAKE)	$(JOBS) DA_TYPE="loop"  CFLAGS="$(CFLAGS)" -C kernel/	
endef	

define BUILD_BOOT
	$(MAKE)	DA_TYPE="init" CFLAGS="$(CFLAGS) -DSTAGE_DA_INIT" -C boot/
	-$(call clean_obj,./boot/sparse)
	$(MAKE)	DA_MODE=$1 DA_TYPE="loop" CREATE_SEC_LIB=$(CREATE_SEC_LIB) CFLAGS="$(CFLAGS) -DSTAGE_DA_LOOP" -C boot/
endef

define BUILD_PLATFORM_LIB
	$(MAKE)	.BBCHIP=$(.BBCHIP) DA_TYPE="init" CFLAGS="$(CFLAGS) -DSTAGE_DA_INIT"  -C platform/$(CODE_BASE)
	-$(foreach b, $(wildcard ./platform/$(CODE_BASE)/*),$(call clean_obj,$b) )
	$(MAKE)	.BBCHIP=$(.BBCHIP) DA_TYPE="loop" CREATE_SEC_LIB=$(CREATE_SEC_LIB) CFLAGS="$(CFLAGS) -DSTAGE_DA_LOOP"  -C platform/$(CODE_BASE)
endef
define BUILD_DRIVER
	$(MAKE)	DA_TYPE="init" CFLAGS="$(CFLAGS) -DSTAGE_DA_INIT"  -C driver/
	-$(foreach b, $(wildcard ./driver/*),$(call clean_obj,$b) )
	$(MAKE)	DA_TYPE="loop" CFLAGS="$(CFLAGS) -DSTAGE_DA_LOOP"  -C driver/
endef

define CREATE_DA_INFO_REFACTOR
	$(SED) -e "s/BBCHIP_NAME/$(BBCHIP_NAME)/" -e "s/BBCHIP_NAME/$(BBCHIP_NAME)/"  -e "s/BBCHIP_HW_CODE/$(BBCHIP_HW_CODE)/"  -e "s/BBCHIP_HW_SUB_CODE/$(BBCHIP_HW_SUB_CODE)/" 	-e "s/BBCHIP_HW_VER/$(BBCHIP_HW_VER)/" -e "s/BBCHIP_SW_VER/$(BBCHIP_SW_VER)/" -e "s/DA_FEATURE_SET/$(DA_FEATURE_SET)/"   $(DAINFO_INFO_DEMO)	> $(DA_INFO)

endef
	
define CREATE_DA_INFO_OLD
	$(SED) -e "s/BBCHIP_NAME/$(BBCHIP_NAME)/" -e "s/BBCHIP_NAME/$(BBCHIP_NAME)/"  -e "s/BBCHIP_HW_CODE/$(BBCHIP_HW_CODE)/"  -e "s/BBCHIP_HW_SUB_CODE/$(BBCHIP_HW_SUB_CODE)/" 	-e "s/BBCHIP_HW_VER/$(BBCHIP_HW_VER)/" -e "s/BBCHIP_SW_VER/$(BBCHIP_SW_VER)/" -e "s/DA_FEATURE_SET/$(DA_FEATURE_SET)/" -e "s,BIN,Raphael-da/BIN," -e "s,temp,Raphael-da/temp,"  -e "s,custom\\security_export,Raphael-da\\custom\\security_export,"    $(DAINFO_INFO_DEMO)	> $(DA_INFO)

endef

define BUILD_CUSTOM_LIB
	$(MAKE)	 .BBCHIP=$(.BBCHIP)  DA_PL=$(DA_PL) CFLAGS="$(CFLAGS)" -C custom/$(CODE_BASE)
	
endef

define BUILD_ARCH_ARM
	$(MAKE)	$(JOBS) DA_TYPE="init" CFLAGS="$(CFLAGS)" ASFLAGS="$(ASFLAGS)" -C arch/arm/	
	$(MAKE)	$(JOBS) DA_TYPE="loop" CFLAGS="$(CFLAGS)" ASFLAGS="$(ASFLAGS)" -C arch/arm/	
endef	

define BUILD_LIB
	$(MAKE)	$(JOBS) CFLAGS="$(CFLAGS)" ASFLAGS="$(ASFLAGS)" -C lib/	
endef	




define CREATE_AUTOCONFIG
	@echo /* autoconfig generated by building system */ > $(AUTOCONFIG)
	$(foreach f,$(FEATURES),$(call dump-feature,$f,$(AUTOCONFIG)))	
	
	$(foreach f,$(FEATURE_SWITCHES_INIT),$(call dump-drivers-init,$f,$(AUTOCONFIG)))	
	$(foreach f,$(FEATURE_SWITCHES_LOOP),$(call dump-drivers-loop,$f,$(AUTOCONFIG)))	
	
	$(foreach f,$(ADDRS),$(call dump-addrs,$f,$(AUTOCONFIG)))	
	$(foreach f,$(ARM_CORE),$(call dump-arm_core,$f,$(AUTOCONFIG)))	
endef

define UPDATE_SECURITY_LIB
	$(LD) -r -o $(SECURTY_LIB_INIT) $(SECURTY_LIB_OBJ_INIT)
	$(LD) -r -o $(SECURTY_LIB_LOOP) $(SECURTY_LIB_OBJ_LOOP)
endef

add_da_info:PREPARE-DAINFO-HEADER
ifneq ($(CALLED_BY),DA_OLD)
	$(ADD_DA_INFO_EXE) $(DA_VERSION) $(DAINFO)
else
	@echo "called by old da"
endif
	-$(foreach b, $(wildcard ./custom/*),$(call clean_obj,$b) )
	-$(foreach b, $(wildcard ./arch/*),$(call clean_dep,$b) )
	-$(foreach b, $(wildcard ./boot/*),$(call clean_obj,$b) )
	-$(foreach b, $(wildcard ./driver/*),$(call clean_obj,$b) )
	-$(foreach b, $(wildcard ./lib/*),$(call clean_obj,$b) )
	-$(call clean_dep,"./boot")
	-$(call clean_dep,"./kernel")
	
PREPARE-DAINFO-HEADER:
ifneq ($(CALLED_BY),DA_OLD)
	-$(RM) -rf $(DAINFO)		
	@echo prepare ini file header...
	echo [MultipleDA] > $(DAINFO)
	echo da_identifier=$(da_identifier) >> $(DAINFO)
	echo da_description=$(da_description) >> $(DAINFO)
	echo da_info_magic=$(da_info_magic) >> $(DAINFO)
	echo sig_symbol=$(sig_symbol) >> $(DAINFO)
	echo output_file=$(output_file) >> $(DAINFO)
	echo da_customized_version= >> $(DAINFO)
	
ifeq ($(DA_PL),yes)
	echo custom_siggen_cb_exe=.\custom\security_export\usbdl4enduser_dummy\PL_DA_Auth.exe >> $(DAINFO)
	echo custom_siggen_asym_key=.\custom\security_export\usbdl4enduser_dummy\VERIFIED_BOOT_IMG_AUTH_KEY.ini >> $(DAINFO)
	echo custom_siggen_sym_key=.\custom\security_export\usbdl4enduser_dummy\Dummy_K3.bin >> $(DAINFO)
endif

	$(foreach b,$(BBCHIPS),$(call cat-file,temp/$(b)/da$(if $(DA_PL),_nsla,).info,$(DAINFO)))
	@echo $(DAINFO) is generated.
else
	-$(CP) temp/$(BBCHIPS)/da$(if $(DA_PL),_nsla,).info	$(DAINFO_OLD)
endif
	
define PREPARE-EPP
	-$(CHMOD) 777  platform/epp/*
	-$(CP) -f platform/epp/EPP 					$(OUTPUT_EPP_BIN)
	-$(CP) -f platform/epp/MTK_EPP_list.txt	 	$(OUTPUT_EPP_BIN)/MTK_$(.BBCHIP)_EPP_list.txt
endef	
