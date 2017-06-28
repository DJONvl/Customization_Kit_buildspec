ifeq ($(DA_PL),yes)
	BBCHIP        := MT6755 MT6797 MT6757 MT6759 MT6763 MT6799 MT6799_E1
	output_file    := .\BIN_NSLA\MTK_AllInOne_DA.bin
else
	BBCHIP         := MT6755 MT6797 MT6757 MT6759 MT6763 MT6799 MT6799_E1
	output_file    := .\BIN\MTK_AllInOne_DA.bin

endif
da_identifier  := MTK_DOWNLOAD_AGENT
da_description := MTK_AllInOne_DA
da_info_magic  := 0x22668899
sig_symbol     := Load$$$$MAIN_PROG_SIG$$$$Base
