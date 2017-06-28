#ifndef __GPT_H__
#define __GPT_H__

#include "boot/system_objects.h"
#include "interface_struct.h"

status_t read_info_from_gpt(uint8* partition, struct partition_info_struct* info);
status_t read_partitions_from_gpt(struct partition_info_struct* info, uint32 *partition_number);

#endif
