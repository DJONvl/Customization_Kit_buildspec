O_TARGET	:= boot_lib_$(DA_TYPE).o

ifeq ($(DA_TYPE),init)
obj-y 		:= bootstrap1.o
obj-y 		+= transfer_serial.o
obj-y 		+= loader_file/commands_preloader_boot1.o
obj-y 		+= debug_impl.o
obj-y     	+= storage_access_boot1.o
obj-y       += dl_info/dl_history_dummy.o
obj-y 		+= checksum/checksum_plain_st.o
obj-y 		+= sparse/sparse_state_machine_stub.o

else
obj-y 		+= security_export/sec_rom_info.o
obj-y 		+= security_export/sec_wrapper.o
obj-y 		+= bootstrap2.o
obj-y       += dl_info/dl_history.o
obj-y 		+= sparse/sparse_state_machine.o
ifeq ($(DA_MODE),DA_MODE_NORMAL)
obj-y 		+= transfer_parallel.o
obj-y 		+= loader_file/commands_preloader_boot2.o
else
obj-y 		+= transfer_serial.o

   ifeq ($(DA_MODE),DA_MODE_BRINGUP)
obj-y 		+= loader_file/commands_preloader_boot2.o
   else
obj-y 		+= loader_file/commands_preloader_boot1.o
   endif
   
endif


obj-y 		+= loader_file/preloader_nand.o
obj-y 		+= debug_impl_sync.o
obj-y       += storage_access_boot2.o
obj-y 		+= checksum/checksum_plain_mt.o
endif

ifeq ($(CREATE_SEC_LIB),yes)
obj-y     += security/security_lib_$(DA_TYPE).o
subdir-y  += security
endif


obj-y 		+= commands.o
obj-y 		+= hal.o
obj-y 		+= handler.o
obj-y 		+= protocol.o
obj-y 		+= pre_synchronize.o
obj-y 		+= system_objects.o
obj-y 		+= partition/partition_info_repertory.o
obj-y     += partition/gpt.o
obj-y     += partition/pmt.o
obj-y 		+= sparse/bulk_process.o
obj-y 		+= loader_file/preloader_mmc.o
obj-y 		+= loader_file/preloader_nand.o
obj-y 		+= loader_file/preloader_ufs.o
obj-y 		+= loader_file/preloader.o
obj-y 		+= checksum/checksum_plain.o
obj-y 		+= checksum/checksum.o
obj-y     += dl_info/dl_info.o
obj-y     += dl_info/dl_archive_proxy.o
obj-y     += overridable/overridable_interface.o
obj-y     += misc/meta_boot_mode.o
obj-y     += dump_speed.o
obj-y     += protocol_callbacks.o

include $(TOPDIR)/Rules.make
