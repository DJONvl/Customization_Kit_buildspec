#ifndef __TRANSFER_H_
#define __TRANSFER_H_
#include "boot/system_objects.h"

#define ROUND_TO_PAGE(x,y) (((x) + (y)) & (~(y)))
#define NAME_SIZE  32

struct subimage_auth_bypass_list {
        char img_name[NAME_SIZE];
};

typedef struct transfer_control {
   bool skip_sparse_check;	
   bool hash_binding;
   bool img_auth_required;
   bool dl_forbidden;
   unsigned char *hash;
   struct subimage_auth_bypass_list *subimg_auth_bypass_list;
}transfer_control_t;


typedef struct download_data_context {
   bool first_run;	
   uint64 bytes_written;
   uint64 length_to_write;
   uint16 checksum_of_image;
   uint16 checksum_of_flash;
   partition_info_struct_t *part_info;
   transfer_control_t *ctrl;
} download_data_context_t;


static inline void init_download_data_context(download_data_context_t* data_ctx, uint64 start_address, uint64 data_length, partition_info_struct_t* part_info, transfer_control_t* ctrl)
{       
   data_ctx->first_run = 1;
   data_ctx->bytes_written = 0;
   data_ctx->checksum_of_image = 0;
   data_ctx->checksum_of_flash = 0;
   data_ctx->length_to_write = data_length;
   data_ctx->part_info = part_info;
   data_ctx->ctrl = ctrl;
}


typedef struct upload_data_context {
   bool first_run;	
   uint64 bytes_read;
   uint64 length_to_read;
   partition_info_struct_t* part_info;
} upload_data_context_t;

static inline void  init_upload_data_context(upload_data_context_t* data_ctx, uint64 start_address, uint64 data_length, partition_info_struct_t* part_info) 
{       
   data_ctx->first_run = 1;
   data_ctx->bytes_read = 0;
   data_ctx->length_to_read = data_length;
   data_ctx->part_info = part_info;
}

status_t download_data(uint64 start_address, uint64 data_length, struct partition_info_struct* part_info, struct transfer_control* ctrl);

status_t upload_data(uint64 start_address, uint64 data_length, struct partition_info_struct* part_info);


#endif
