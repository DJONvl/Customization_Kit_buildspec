.cat-file-count :=

CUSTOM := MTK
ADD_DA_INFO     := build/tools/AddDAInfo_SV5.exe
DA_CURRENT_DATE_ORIGINAL := $(shell date /T)
DA_CURRENT_DATE          := $(firstword $(DA_CURRENT_DATE_ORIGINAL))
DA_CURRENT_TIME_ORIGINAL := $(shell time /T)
DA_CURRENT_TIME          := $(firstword $(DA_CURRENT_TIME_ORIGINAL))
DA_VERSION               := 3.3001.$(DA_CURRENT_DATE).$(DA_CURRENT_TIME)
DA_SIG_SYMBOL   := g_Signature

-include build/core/configs/custom.mk
CUSTOM := $(strip $(CUSTOM))

SplitDA_ARMCC_CHIPS	:=	MT6582 MT6572  MT6592 MT8127 MT2601 MT8590
SplitDA_GCC_CHIPS	:=	MT6571 MT6595  MT6752  MT6580 MT8173 MT6795 MT6735 MT8163 MT0571

SplitDA_CHIPS 	:= $(SplitDA_ARMCC_CHIPS) $(SplitDA_GCC_CHIPS)
75Serial_CHIPS 	:= MT6575_E1 MT6575 MT6577 MT6589 MT8135
Refactor_CHIPS	:= MT6755 MT6797 MT0598 MT6757 MT6799 MT6799_E1 MT6570 MT8167 MT6759 MT6763 MT6758

define cat-file
	@echo dumping $1...
	echo [DA_$(if $(filter 0,$(words $(.cat-file-count))),Default,$(words $(.cat-file-count)))] >> $2
	$(CAT) $1 >> $2
$(eval .cat-file-count := $(.cat-file-count) 0)
endef

default-target: build-all-in-one-da
.PHONY: FORCE
build-all-in-one-da: build-chips prepare-dainfo-header add-da-info FORCE
ifeq ($(DA_PL),yes)
ifeq ($(CRYPTO20),yes)
	$(CP) -f bin_nsla/$(CUSTOM)_AllInOne_DA.bin bin_nsla/DA_PL_CRYPTO20.bin
else
	$(CP) -f bin_nsla/$(CUSTOM)_AllInOne_DA.bin bin_nsla/DA_PL.bin
endif
else
ifeq ($(SW_SEC_BOOT),yes)
ifeq ($(CRYPTO20),yes)
	$(CP) -f bin_swsla/$(CUSTOM)_AllInOne_DA.bin bin_swsla/DA_SWSEC_CRYPTO20.bin
else
	$(CP) -f bin_swsla/$(CUSTOM)_AllInOne_DA.bin bin_swsla/DA_SWSEC.bin
endif
endif
endif
	@echo build passed (SIGN = $(SIGN)) (DA_PL = $(DA_PL)) (SW_SEC_BOOT = $(SW_SEC_BOOT)).

add-da-info: FORCE
ifeq ($(SIGN),$(filter $(SIGN),yes))
	@echo "ADD_DA_INFO,$(ADD_DA_INFO)"
	$(ADD_DA_INFO) $(DA_VERSION) $(DAINFO)
	@echo all-in-one-da is generated.
	$(MV) -f bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_AllInOne_DA.bin bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_AllInOne_DA_TEMP.bin
	$(MV) -f bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_AllInOne_DA_TEMP.bin bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(CUSTOM)_AllInOne_DA.bin
endif

output_file := $(patsubst MTK_%,$(CUSTOM)_%,$(output_file))

prepare-dainfo-header: FORCE
ifeq ($(SIGN),$(filter $(SIGN),yes))
	-$(MKDIR) -p obj$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)
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
ifeq ($(CRYPTO20),yes)	
	echo custom_siggen_cb_exe=.\custom\security\usbdl4enduser_dummy\PL_DA_Auth.exe >> $(DAINFO)
	echo custom_siggen_asym_key=.\custom\security\usbdl4enduser_dummy\VERIFIED_BOOT_IMG_AUTH_KEY.ini >> $(DAINFO)
	echo custom_siggen_sym_key=.\custom\security\usbdl4enduser_dummy\Dummy_K3.bin >> $(DAINFO)
else
	echo custom_siggen_cb_exe=.\custom\security\usbdl4enduser_dummy\S_USBDL_Dummy.exe >> $(DAINFO)
	echo custom_siggen_asym_key=.\custom\security\usbdl4enduser_dummy\Dummy_K2.bin >> $(DAINFO)
	echo custom_siggen_sym_key=.\custom\security\usbdl4enduser_dummy\Dummy_K3.bin >> $(DAINFO)
endif
else
ifeq ($(SW_SEC_BOOT),yes)
ifeq ($(CRYPTO20),yes)
	echo custom_siggen_cb_exe=.\custom\security\usbdl4enduser_dummy\PL_DA_Auth.exe >> $(DAINFO)
	echo custom_siggen_asym_key=.\custom\security\usbdl4enduser_dummy\VERIFIED_BOOT_IMG_AUTH_KEY.ini >> $(DAINFO)
	echo custom_siggen_sym_key=.\custom\security\usbdl4enduser_dummy\Dummy_K3.bin >> $(DAINFO)
else
	echo custom_siggen_cb_exe=.\custom\security\usbdl4enduser_dummy\S_USBDL_Dummy.exe >> $(DAINFO)
	echo custom_siggen_asym_key=.\custom\security\usbdl4enduser_dummy\Dummy_K2.bin >> $(DAINFO)
	echo custom_siggen_sym_key=.\custom\security\usbdl4enduser_dummy\Dummy_K3.bin >> $(DAINFO)
endif
endif
endif	
	$(foreach b,$(BBCHIPS),$(call cat-file,config/settings/$(b)/da$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,).info,$(DAINFO)))
	@echo $(DAINFO) is generated.
endif

define build-bbchip
	make -j8 BBCHIP=$1 SIGN= DA_PL=$(DA_PL) SW_SEC_BOOT=$(SW_SEC_BOOT) SPEC= SPLIT_DA_METHOD=legacy

endef
define make -j8_init_feedback
	make -j8 BBCHIP=$1 SIGN=no DA_PL=$(DA_PL) SW_SEC_BOOT=$(SW_SEC_BOOT) SPEC= SPLIT_DA_METHOD=init
endef


define build-bbchip-split
	make -j8 BBCHIP=$1 SIGN=no DA_PL=$(DA_PL) SW_SEC_BOOT=$(SW_SEC_BOOT) SPEC= SPLIT_DA_METHOD=loop

	build/tools/Hash_Gen.exe bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_$1_DA.bin/EXT_RAM custom/$1/src/da_loop_hash.c

	$(if $(wildcard $(FEEDBACK_INIT_FILE) ),,$(call make -j8_init_feedback,$1))

	make -j8 BBCHIP=$1 SIGN= DA_PL=$(DA_PL) SW_SEC_BOOT=$(SW_SEC_BOOT) SPEC= SPLIT_DA_METHOD=init

	$(RM) -f custom/$1/src/da_loop_hash.c

endef

define build-bbchip-split-gcc
	make -j8 BBCHIP=$1 SIGN=no DA_PL=$(DA_PL) SW_SEC_BOOT=$(SW_SEC_BOOT) SPEC= SPLIT_DA_METHOD=loop

	build/tools/Hash_Gen.exe bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_$1_DA.bin/EXT_RAM custom/$1/src/da_loop_hash.c

	make -j8 BBCHIP=$1 SIGN= DA_PL=$(DA_PL) SW_SEC_BOOT=$(SW_SEC_BOOT) SPEC= SPLIT_DA_METHOD=init

	$(RM) -f custom/$1/src/da_loop_hash.c

endef

.PHONY: is_build_ok
is_build_ok : $(CURDIR)\Raphael-da\build_ok.txt
	@echo build OK
	
define build-bbchip-refactor
	make -C Raphael-da BBCHIP=$1 DA_PL=$(DA_PL) CALLED_BY=DA_OLD
	make is_build_ok
endef

define build-bbchip-da-old
	$(eval .BBCHIP := $1)
	$(eval USE_GCC := )
	$(eval include build/core/configs/$(.BBCHIP).mk)
	$(if $(USE_GCC),$(call build-bbchip-split-gcc,$1),$(if $(findstring $b,$(SplitDA_CHIPS)),$(call build-bbchip-split,$1),$(call build-bbchip,$1)))
endef

define  build-bbchip-da
	$(if $(findstring $1,$(Refactor_CHIPS)),$(call build-bbchip-refactor,$1),$(call build-bbchip-da-old,$1))

endef


define change-feedbackfile-mode
	-$(CHMOD) 777 custom/$1/*_fb.txt
	
endef

define change-feedbackfile-mode-splitda_armcc
	-$(CHMOD) 777 custom/$1/*_fb.txt
	
endef

define change-link-ld-mode
	-$(CHMOD) 777 config/settings/$1/LINK.LD.*
	
endef

build-chips:
	$(MKDIR) -p bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)
	$(foreach b,$(BBCHIPS),$(if $(findstring $b,$(75Serial_CHIPS)),$(call change-feedbackfile-mode,$b)))
	$(foreach b,$(BBCHIPS),$(if $(findstring $b,$(SplitDA_ARMCC_CHIPS)),$(call change-feedbackfile-mode-splitda_armcc,$b)))
	$(foreach b,$(BBCHIPS),$(if $(findstring $b,$(SplitDA_GCC_CHIPS)),$(call change-link-ld-mode,$b)))
	
	$(foreach b,$(BBCHIPS),$(call build-bbchip-da,$b))

#$(foreach b,$(BBCHIPS),$(eval include build/core/configs/$(b).mk))
CUSTOM_KIT_ZIP := Customization_Kit_$(basename $(notdir $(SPEC))).zip
PLATFORM_KIT_ZIP := Platform_Kit_$(basename $(notdir $(SPEC))).zip

#platform-kits += bin/ lib/ $(foreach b,$(BBCHIPS),config/settings/$(b))

define remove-elf-etc
	$(warning Remove all unnecessary files.)
	-$(RM) -f bin/MTK_$1_DA.bin/*.elf bin/MTK_$1_DA.bin/*.dis bin/MTK_$1_DA.bin/*.map
	-$(RM) -f bin_nsla/MTK_$1_DA.bin/*.elf bin_nsla/MTK_$1_DA.bin/*.dis bin_nsla/MTK_$1_DA.bin/*.map
	-$(RM) -f bin_swsla/MTK_$1_DA.bin/*.elf bin_swsla/MTK_$1_DA.bin/*.dis bin_swsla/MTK_$1_DA.bin/*.map	
endef

BUILD_CUSTOM_KIT:
	$(eval include build/core/configs/$(BBCHIP).mk)
	$(eval CUSTOM_KIT_ZIP := Customization_Kit_$(basename $(notdir $(SPEC))).zip)
	echo CUSTOM := CUSTOM > build/core/configs/custom.mk
	$(foreach b, $(sort $(BBCHIP) $(word 1,$(subst _, ,$(BBCHIP)))), $(call remove-elf-etc,$b))
	$(ZIP) -r -9 $(CUSTOM_KIT_ZIP)   $(custom-kits) 
	-$(RM) build/core/configs/custom.mk

define build-custom-kit
	make -j8 BBCHIP=$1 SPEC=$(SPEC) BUILD_CUSTOM_KIT
	
endef

define BUILD_FILE_modify
	$(MV)  buildspec.mk  buildspec_temp.mk	-f
	$(SED) -e "s/MT6589//" -e "s/MT8135//"  buildspec_temp.mk	> buildspec.mk
	
endef

define BUILD_FILE_resume
	-$(CHMOD)	777 buildspec_temp.mk
	-$(MV)	buildspec_temp.mk  buildspec.mk -f
	
endef
CUSTOM_KIT:
	-$(RM) $(CUSTOM_KIT_ZIP)
	$(foreach b, $(wildcard ./Raphael-da/custom/*), $(call clean_obj,$b) )
	$(call BUILD_FILE_modify)
	
	$(foreach b, $(BBCHIPS), $(call build-custom-kit,$b))
	$(ZIP) $(CUSTOM_KIT_ZIP) $(SPEC)
	
	$(call BUILD_FILE_resume)
	
	make -C  Raphael-da  BBCHIP="$(Refactor_CHIPS)" custom_kit CALLED_BY=DA_OLD
	

#	-$(RM) $(PLATFORM_KIT_ZIP)
#	$(foreach b, $(BBCHIPS), $(call build-platform-kit,$b))

BUILD_PLATFORM_KIT:
	$(eval include build/core/configs/$(BBCHIP).mk)
	$(eval PLATFORM_KIT_ZIP := Platform_Kit_$(basename $(notdir $(SPEC))).zip)
	echo CUSTOM := PLATFORM > build/core/configs/custom.mk
	$(foreach b, $(sort $(BBCHIP) $(word 1,$(subst _, ,$(BBCHIP)))), $(call remove-elf-etc,$b))
	$(ZIP) -r -9 $(PLATFORM_KIT_ZIP) $(platform-kits)
	-$(RM) build/core/configs/custom.mk

define build-platform-kit
	make -j8 BBCHIP=$1 SPEC=$(SPEC) BUILD_PLATFORM_KIT
	
endef	

#PLATFORM_KIT:
#	-$(RM) $(PLATFORM_KIT_ZIP)
#	$(foreach b, $(BBCHIPS), $(call build-platform-kit,$b))


clean:
	$(RM) -rf bin obj bin_nsla obj_nsla bin_swsla obj_swsla LIB/*.sig
	$(foreach b, $(wildcard ./platform/*), $(call clean_obj,$b) )
	$(foreach b, $(wildcard ./custom/*), $(call clean_obj,$b) )
	make clean -C Raphael-da




