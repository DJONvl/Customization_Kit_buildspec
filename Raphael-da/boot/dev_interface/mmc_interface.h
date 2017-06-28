#ifndef __MMC_INTERFACE__
#define __MMC_INTERFACE__

#include "sys/types.h"
#include "interface_struct.h"
#include "boot/dev_interface/storage_ctrl_interface.h"

//interface

extern status_t interface_mmc_init(uint32 boot_channel);
extern status_t interface_get_mmc_info(struct mmc_info_struct* info);
extern status_t interface_mmc_device_ctrl(uint32 ctrl_code, void* in, uint32 in_len, void* out, uint32 out_len, uint32* ret_len);
extern status_t interface_switch_mmc_section(uint32 section);
extern status_t interface_mmc_read(uint64 address,  uint8* buffer, uint64 length);
extern status_t interface_mmc_write(uint64 address, uint8* buffer, uint64 length);
extern status_t interface_mmc_erase(uint64 address, uint64 length);
extern status_t interface_set_emmc_boot_ext_csd(uint32 index);
extern status_t interface_check_RPMB_key_status();


#endif
