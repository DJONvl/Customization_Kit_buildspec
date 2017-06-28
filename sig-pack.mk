THIS_FILE := gen-sig.mk

include buildspec.mk

CUSTOM := MTK
-include build/core/configs/custom.mk
CUSTOM := $(strip $(CUSTOM))

MKDIR           := $(if $(LINUX),mkdir,"$(word 1,$(wildcard ../Tools/mkdir.exe Tools/mkdir.exe) mkdir.exe)")
RM              := $(if $(LINUX),rm,"$(word 1,$(wildcard ../Tools/rm.exe    Tools/rm.exe)    rm.exe)")
MV              := $(if $(LINUX),mv,"$(word 1,$(wildcard ../Tools/mv.exe    Tools/mv.exe)    mv.exe)")
CAT             := $(if $(LINUX),cat,"$(word 1,$(wildcard ../Tools/cat.exe   Tools/cat.exe)   cat.exe)")
CP              := $(if $(LINUX),cp,"$(word 1,$(wildcard ../Tools/cp.exe    Tools/cp.exe)    cp.exe)")

DA_VERSION      := 3.3001.00.00

OUTPUT_PREFIX   := MTK_$(if $(BBCHIP),$(BBCHIP),Default)$(if $(FLASH),_$(FLASH),)_DA
OUTPUT_BIN      := bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(OUTPUT_PREFIX).bin
OUTPUT_SIG      := lib/$(OUTPUT_PREFIX)$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,).sig


ADD_DA_INFO     := build/tools/AddDAInfo_SV5.exe
SIGN_DA_EXE     := build/tools/SignDA_SV5.exe 
SIGN_DA         := $(SIGN_DA_EXE) SHA256 $(OUTPUT_BIN) INT_SYSRAM,INT_TCM,D_TCM,EXT_RAM $(OUTPUT_SIG)


define cat-file
	@echo dumping $1...
	echo [DA_$(if $(filter 0,$(words $(.cat-file-count))),Default,$(words $(.cat-file-count)))] >> $2
	$(CAT) $1 >> $2
$(eval .cat-file-count := $(.cat-file-count) 0)
endef



.PHONY : FORCE

sign: FORCE	
	$(warning Exe: $(SIGN_DA))
	$(SIGN_DA)



pack: FORCE	
	-$(RM) -rf $(DAINFO)		
	@echo prepare DA info header...
	-$(MKDIR) $(dir $(DAINFO))
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

	$(ADD_DA_INFO) $(DA_VERSION) $(DAINFO)	
	$(MV) -f bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/MTK_AllInOne_DA.bin bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/T_AllInOne_DA.bin
	$(MV) -f bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/T_AllInOne_DA.bin bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(CUSTOM)_AllInOne_DA.bin
	-$(MKDIR) CustomDAbin
	$(CP) -f bin$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)/$(CUSTOM)_AllInOne_DA.bin CustomDAbin/$(if $(DA_PL),DA_PL,$(if $(SW_SEC_BOOT),DA_SWSEC,$(CUSTOM)_AllInOne_DA)).bin
	-$(RM) -rf obj$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)