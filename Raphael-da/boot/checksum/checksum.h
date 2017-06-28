#ifndef __CHECKSUM_H_
#define __CHECKSUM_H_
#include "sys/types.h"
#include "interface_struct.h"

extern unsigned int has_initialed;

extern uint16 checksum_plain(uint8* data, uint32 length);
extern uint32 checksum_crc32(uint8* data, uint32 length);

void init_crc32_table();
status_t calc_checksum(uint8* data, uint32 length, /*OUT*/uint8* checksum, checksum_algorithm_e algo);

#endif

