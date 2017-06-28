#ifndef __ufs_INTERFACE__
#define __ufs_INTERFACE__

#include "sys/types.h"
#include "interface_struct.h"
#include "boot/dev_interface/storage_ctrl_interface.h"

//interface

extern status_t interface_ufs_init(uint32 boot_channel);
extern status_t interface_get_ufs_info(struct ufs_info_struct* info);
extern status_t interface_ufs_device_ctrl(uint32 ctrl_code, void* in, uint32 in_len, void* out, uint32 out_len, uint32* ret_len);
extern status_t interface_switch_ufs_section(uint32 section);
extern status_t interface_ufs_read(uint64 address,  uint8* buffer, uint64 length);
extern status_t interface_ufs_write(uint64 address, uint8* buffer, uint64 length);
extern status_t interface_ufs_erase(uint64 address, uint64 length);
extern status_t interface_set_ufs_boot_ext_csd(uint32 index);

#endif
