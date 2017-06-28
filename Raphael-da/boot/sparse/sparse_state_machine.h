#ifndef 	_SPARSE_STATE_MACHINE_H_
#define 	_SPARSE_STATE_MACHINE_H_

#include "boot/system_objects.h"

#define STORAGE_BLOCK_SIZE 	4096

typedef enum unsparse_wait_phase
{
   UNSPARSE_WAIT_SPARSE_HEADER,
   UNSPARSE_WAIT_CHUNK_HEADER,
   UNSPARSE_WAIT_CHUNK_DATA,
   UNSPARSE_WAIT_CHUNK_NOT_CARE,
   UNSPARSE_WAIT_CHUNK_FILL,
   UNSPARSE_WAIT_CHUNK_CRC,
} unsparse_phase_t;

typedef struct cache_ctx
{   
   uint8* c_base;             //cache buffer base
   uint32 c_offset;             //cache buffer offset
   uint32 c_size;             //cache buffer size
} cache_ctx_t;

typedef struct unsparse_status
{
   status_t handle_status;
   unsparse_phase_t wait_phase;   
   uint64 image_base_addr;   //partition base address.
   uint32 byte_to_process;    // byte to be processed in one packet.
   uint8 *buf;                        // byte buffer of one packet.
   bool enable_cached_method;
   cache_ctx_t ctx;           //buffer download context
   uint32 checksum_level;
   uint16 checksum_of_flash;
   uint32 size;
   partition_info_struct_t* arg;
} unsparse_status_t;

typedef struct unsparse_data
{
   sparse_header_t sparse_hdr;
   chunk_header_t chunk_hdr;
   uint64 chunk_remain_data_size;      //data remained in current packet.
   uint64 image_address_offset;         //offset of current processed data in packet    |base---|---|---|-.....|offset.....|
   uint32 unhandle_buf_size;
   uint8  unhandle_buf[sizeof(chunk_header_t)];
} unsparse_data_t;

bool is_sparse_image(uint8* data, uint32 length);
uint64 unspared_size(uint8* data);
void init_unsparse_status(unsparse_status_t* status, uint64 image_base_addr, uint32 checksum_level, partition_info_struct_t* arg);
void write_sparse_data(unsparse_status_t* status, uint8* data, uint32 length);

//ex functions
void end_write_sparse_data(unsparse_status_t* status);


#endif

