AUTOCONFIG      := custom/$(BBCHIP)/inc/autoconfig.h
common-objs     := $(call prepare-object-list,$(SOURCE_COMMON))
platform-objs   := $(call prepare-object-list,$(SOURCE_PLATFORM))
custom-objs     := $(call prepare-object-list,$(SOURCE_CUSTOMER))

ifeq ($(SIGN),$(filter $(SIGN),no))
TARGET := lib/$(SEC_LIB) lib/$(SEC_LIB2) $(OUTPUT_COMMON_LIB) $(OUTPUT_PLATFORM_LIB)  $(custom-objs)
else
TARGET := 
endif

default-target: DA
DA: $(TARGET)
ifeq ($(SIGN),$(filter $(SIGN),no))
	$(MKDIR) -p $(OUTPUT_BIN)
ifeq ($(USE_GCC), yes)
ifeq ($(SPLIT_DA_METHOD), loop)
	$(SED) "s/LOOP_START/$(DA_DRAM_BASE_ADDRESS)/"  $(SCATTER_LOOP_DEMO)	> $(SCATTER)
	$(LD) -o $(OUTPUT_ELF) $(custom-objs) $(LIBRARY) $(LIBRARY) $(LDFLAGS)
	$(OBJCOPY) -O binary -S -g $(OUTPUT_ELF) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_$(BBCHIP)_DA.bin/EXT_RAM
	$(OBJDUMP) -D -S -t $(OUTPUT_ELF) > $(OUTPUT_DIS)
	$(CP) $(OUTPUT_ELF) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin/DA_LOOP.elf
	$(CP) $(OUTPUT_DIS) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin/DA_LOOP.elf.dis
endif
ifeq ($(SPLIT_DA_METHOD), init)
	$(SED) -e "s/INIT_START/$(DA_SRAM_BASE_ADDRESS)/"  -e "s/BSS_LEN/$(BSS_LEN)/" -e "s/TEXT_LEN/$(TEXT_LEN)/"  $(SCATTER_INIT_DEMO) > $(SCATTER)
	$(LD) -o $(OUTPUT_ELF) $(custom-objs) $(LIBRARY) $(LIBRARY) $(LDFLAGS)
	$(OBJCOPY) -O binary -S -g $(OUTPUT_ELF) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_$(BBCHIP)_DA.bin/INT_SYSRAM
	$(OBJDUMP) -D -S -t $(OUTPUT_ELF) > $(OUTPUT_DIS)
	$(CP) $(OUTPUT_ELF) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin/DA_INIT.elf
	$(CP) $(OUTPUT_DIS) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin/DA_INIT.elf.dis
	echo Memory Map of the image>> bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)\MTK_$(BBCHIP)_DA.bin\MTK_$(BBCHIP)_DA_list.txt
	echo   Load Region INT_SYSRAM (Base: $(DA_SRAM_BASE_ADDRESS), Size: 0x10000000, Max: 0x10000000, ABSOLUTE)>> bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)\MTK_$(BBCHIP)_DA.bin\MTK_$(BBCHIP)_DA_list.txt
	echo   Load Region EXT_RAM (Base: $(DA_DRAM_BASE_ADDRESS), Size: 0x010000000, Max: 0x010000000, ABSOLUTE)>> bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)\MTK_$(BBCHIP)_DA.bin\MTK_$(BBCHIP)_DA_list.txt
endif
else
	$(LD) -output $(OUTPUT_ELF) $(LDFLAGS) $(custom-objs) $(LIBRARY)
ifeq ($(SPLIT_DA_METHOD), loop)
	$(BIN) -bin -o $(OUTPUT_BIN)/EXT_RAM $(OUTPUT_ELF)
	$(CP) $(OUTPUT_ELF) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin/DA_LOOP.elf
	$(CP) $(OUTPUT_TXT) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin/DA_LOOP.out.txt
endif
ifeq ($(SPLIT_DA_METHOD), init)
	$(BIN) -bin -o $(OUTPUT_BIN)/INT_SYSRAM $(OUTPUT_ELF)
	$(CP) $(OUTPUT_ELF) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin/DA_INIT.elf
	$(CP) $(OUTPUT_TXT) bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin/DA_INIT.out.txt
endif
ifeq ($(SPLIT_DA_METHOD), legacy)
	$(BIN) -bin -o $(OUTPUT_BIN) $(OUTPUT_ELF)
endif
	$(DISASSEMBLE) -c $(OUTPUT_ELF) > $(OUTPUT_DIS)
endif

endif
ifeq ($(SIGN),$(filter $(SIGN),yes))
ifneq (,$(wildcard EPP))
	make -C EPP -f makefile.arm clean
ifeq (MT6577,$(BBCHIP))
	make -C EPP -f makefile.arm EPP
else
	make BBCHIP=$(BBCHIP_BASE) -C EPP -f makefile.arm EPP
endif
endif
#	$(SIGN_DA) $(OUTPUT_BIN) INT_SYSRAM,INT_TCM,D_TCM,EXT_RAM $(OUTPUT_SIG)
#	$(SIGN_DA) $(OUTPUT_BIN)/INT_SYSRAM $(OUTPUT_TXT) Load$$$$MAIN_PROG_SIG$$$$Base $(OUTPUT_SIG)
	$(SIGN_DA)
#	$(ADD_DA_INFO) $(DA_VERSION) $(DA_INFO_INI)	
#	$(MV) -f bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_AllInOne_DA.bin bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_AllInOne_DA_TEMP.bin
#	$(MV) -f bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_AllInOne_DA_TEMP.bin bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(CUSTOM)_AllInOne_DA.bin
endif	
	@echo Build Passed (SIGN = $(SIGN)) (DA_PL = $(DA_PL)) (SW_SEC_BOOT = $(SW_SEC_BOOT)) (BBCHIP = $(BBCHIP))

$(OUTPUT_COMMON_LIB): $(common-objs)
ifeq ($(USE_GCC), yes)
	$(MKDIR) -p bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)
	$(MKDIR) -p $(OUTPUT_BIN)
	$(AR) rcs $(OUTPUT_COMMON_LIB) $(ARFLAGS) $(common-objs)
	@echo ------ OBJ LIST --------- > $(OUTPUT_COMMON_LIB_INFO)
	@$(AR) t $(OUTPUT_COMMON_LIB) >> $(OUTPUT_COMMON_LIB_INFO)
else
	$(MKDIR) -p bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)
	$(MKDIR) -p $(OUTPUT_BIN)
	$(AR) -create $(OUTPUT_COMMON_LIB) $(ARFLAGS) $(common-objs)
	@echo ------ OBJ LIST --------- > $(OUTPUT_COMMON_LIB_INFO)
	@$(AR) -t $(OUTPUT_COMMON_LIB) >> $(OUTPUT_COMMON_LIB_INFO)
	@echo ------ SYMBOL LIST ------ >> $(OUTPUT_COMMON_LIB_INFO)
	@$(AR) -zs $(OUTPUT_COMMON_LIB) >> $(OUTPUT_COMMON_LIB_INFO)
endif

$(OUTPUT_PLATFORM_LIB): $(platform-objs)
ifeq ($(USE_GCC), yes)
	$(MKDIR) -p bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)
	$(MKDIR) -p $(OUTPUT_BIN)
	$(AR) rcs $(OUTPUT_PLATFORM_LIB) $(ARFLAGS) $(platform-objs)
	@echo ------ OBJ LIST --------- > $(OUTPUT_PLATFORM_LIB_INFO)
	@$(AR) t $(OUTPUT_PLATFORM_LIB) >> $(OUTPUT_PLATFORM_LIB_INFO)
else
	$(MKDIR) -p bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)
	$(MKDIR) -p $(OUTPUT_BIN)
	$(AR) -create $(OUTPUT_PLATFORM_LIB) $(ARFLAGS) $(platform-objs)
	@echo ------ OBJ LIST --------- > $(OUTPUT_PLATFORM_LIB_INFO)
	@$(AR) -t $(OUTPUT_PLATFORM_LIB) >> $(OUTPUT_PLATFORM_LIB_INFO)
	@echo ------ SYMBOL LIST ------ >> $(OUTPUT_PLATFORM_LIB_INFO)
	@$(AR) -zs $(OUTPUT_PLATFORM_LIB) >> $(OUTPUT_PLATFORM_LIB_INFO)
endif

lib/$(SEC_LIB): FORCE
ifneq (,$(wildcard lib/$(SEC_LIB) lib/$(call lc, $(SEC_LIB)) lib/$(call uc, $(SEC_LIB))))
	-$(CHMOD) 777 lib/$(SEC_LIB)
endif	
	-$(CP) -f ../SecurityLib/DA/$(SEC_LIB) lib/$(SEC_LIB)

lib/$(SEC_LIB2): FORCE
ifneq (,$(wildcard lib/$(SEC_LIB2) lib/$(call lc, $(SEC_LIB2)) lib/$(call uc, $(SEC_LIB2))))
	-$(CHMOD) 777 lib/$(SEC_LIB2)
endif	
	-$(CP) -f ../SecurityLib/DA/$(SEC_LIB2) lib/$(SEC_LIB2)

CUSTOM_KIT:
	-$(RM) $(CUSTOM_KIT_ZIP)
	echo CUSTOM := CUSTOM > build/core/configs/custom.mk
	$(ZIP) -r -9 $(CUSTOM_KIT_ZIP)   $(custom-kits) custom/common $(addprefix custom/,$(BBCHIPS))
	echo CUSTOM := PLATFORM > build/core/configs/custom.mk
	$(ZIP) -r -9 $(PLATFORM_KIT_ZIP) $(platform-kits) custom/common $(addprefix custom/,$(BBCHIPS)) platform/common $(addprefix platform/,$(BBCHIPS))
	-$(RM) build/core/configs/custom.mk
#	$(ZIP) -u -j -9 $(CUSTOM_KIT_ZIP) ../ReleaseNote.txt
.PHONY: FORCE
$(AUTOCONFIG): FORCE
ifeq (,$(INCREMENTAL))
	$(MKDIR) -p $(patsubst %/,%,$(dir $(AUTOCONFIG)))
	@echo /* autoconfig generated by building system */ > $(AUTOCONFIG)
	$(foreach f,$(FEATURES),$(call dump-feature,$f,$(AUTOCONFIG)))	
	$(foreach f,$(ADDRS),$(call dump-addrs,$f,$(AUTOCONFIG)))	
endif
