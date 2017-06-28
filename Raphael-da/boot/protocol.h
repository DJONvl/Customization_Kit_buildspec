
#ifndef __PROTOCOL__
#define __PROTOCOL__

#include "sys/types.h"

/*synchonized*/ status_t read_packet(uint8* buf, uint32* length);

/*synchonized*/ status_t write_packet(uint8* buf, uint32 length);

/*synchonized*/ status_t log_packet_to_pc(const uint8* buf, uint32 length);

status_t log_to_uart(const uint8* buf, uint32 length);
#endif


