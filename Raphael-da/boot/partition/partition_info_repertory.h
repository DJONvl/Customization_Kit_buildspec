
#ifndef __PROTOCOL__
#define __PROTOCOL__

#include "boot/system_objects.h"
#include "interface_struct.h"
#include "partition_struct.h"

struct partition
{
   uint8 name[PARTITION_NAME_LEN];
};

typedef struct sys_partition_tbl
{
   BOOL valid;
   int count;
   int reserved;
   struct partition_info_struct* info;
}sys_partition_tbl_t;

extern struct sys_partition_tbl sysob_partition_tbl;

void invalidate_sys_partition_tbl_lookup(uint8* partition_name);
//interface
status_t pir_get_partition_info(uint8* partition, struct partition_info_struct* info);

/*
* function:tell user which partitions are in the operation range.
* parameters:
*    para:input, provide operation information such as range, section and storage etc.
*    partitions:output, partition list in the given range
* return:
*    STATUS_OK if success.
*    STATUS_READ_PT_FAIL shows that read partition table failed, could not finish the convert.
*    Other status need to refer to error_code.h
*/
status_t range_to_partition(const struct partition_info_struct *para, struct partition **partitions, uint32 *count);
void free_partition_buffer(struct partition * partitions);
status_t get_extention_info_from_addr(struct partition_info_struct* para, uint64 addr);

#endif


