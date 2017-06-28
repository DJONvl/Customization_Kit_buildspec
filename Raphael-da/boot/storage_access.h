#ifndef __STORAGE_ACCESS_H_
#define __STORAGE_ACCESS_H_

#include "sys/types.h"
#include "boot/protocol_callbacks.h"

extern status_t write_storage(uint64 at_address, uint8* data, uint64 length, struct partition_info_struct* part_info);
extern status_t read_storage(uint64 at_address, uint8* data, uint64 length, struct partition_info_struct* part_info);
extern status_t storage_ctrl(enum storage_type storage, uint32 ctrl_code, void* in, uint32 in_len, void* out, uint32 out_len, uint32* ret_len);
extern status_t erase_storage(struct partition_info_struct* part_info, const struct progress_cb* cb);
extern status_t diagnose_storage(struct partition_info_struct* part_info);

#endif
