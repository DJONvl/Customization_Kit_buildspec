#ifndef __DUMP_SPEED__
#define __DUMP_SPEED__

#include <type_define.h>
#include "system_objects.h"

#define SPEED(a,b) ((b==0)?0:(((a)*1000)/(b)/(1024*1024)))

struct speed_unit
{
   uint32 time;
   uint64 data_size;
};

struct speed_dump
{
    struct speed_unit average;
    struct speed_unit  usb_read;
    struct speed_unit  usb_write;
    struct speed_unit  storage_read;
    struct speed_unit  storage_write;
    struct speed_unit  mcopy;
};

struct speed_dump speed_info_record;

void dump_speed_info(void);
status_t read_storage_with_dump(uint64 at_address, uint8* data, uint64 length, struct partition_info_struct* part_info);
status_t write_storage_with_dump(uint64 at_address, uint8* data, uint64 length, struct partition_info_struct* part_info);
status_t read_usb_with_dump(uint8* buffer, uint32* length);
status_t write_usb_with_dump(uint8* buffer, uint32 length);

#endif
