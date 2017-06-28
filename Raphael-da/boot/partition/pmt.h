#ifndef __PMT_H__
#define __PMT_H__

#include "boot/system_objects.h"

status_t read_partitions_from_pmt(struct partition_info_struct* info, uint32 *count);
status_t read_info_from_pmt(uint8* partition, struct partition_info_struct* part_info);

#endif