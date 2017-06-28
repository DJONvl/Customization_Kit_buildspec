#ifndef 	_BULK_DATA_PROCESS_H_
#define 	_BULK_DATA_PROCESS_H_

#include "boot/system_objects.h"

typedef struct bulk_cache_ctx
{
   uint8* c_base;             //cache buffer base
   uint32 c_offset;             //cache buffer offset
   uint32 c_size;             //cache buffer size
} bulk_cache_ctx_t;

typedef struct bulk_status
{
   status_t handle_status;   
   uint32 byte_to_process;    // byte to be processed in one packet.
   uint64 image_base_addr;   //partition base address.
   uint64 image_address_offset;
   uint8 *buf;                        // byte buffer of one packet.
   bool enable_cached_method;
   bulk_cache_ctx_t ctx;           //buffer download context
   uint32 checksum_level;
   uint16 checksum_of_flash;
   partition_info_struct_t* arg;
} bulk_status_t;


void init_bulk_process_status(bulk_status_t* status, uint64 image_base_addr, uint32 checksum_level, partition_info_struct_t* arg);
void write_bulk_data(bulk_status_t* status, uint8* data, uint32 length);

//ex functions
void end_write_bulk(bulk_status_t* status);
   


#endif

