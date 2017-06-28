#ifndef __NAND_INTERFACE__
#define __NAND_INTERFACE__

#include "sys/types.h"
#include "interface_struct.h"
#include "boot/protocol_callbacks.h"
#include "boot/dev_interface/storage_ctrl_interface.h"

/* We need this to distinguish MT8167 & MT8516
 * MT8516 use SLC Nand
 * 0x2144 => 8516
 * 0x1FE7 => 8167 */
#define NFI_IS_8516	((*((volatile uint32 *)0x10009300) & 0xffff) == 0x2144)

struct bmt_remark_cb
{
   void* user_arg;
   CB_OP_PROGRESS cb;
   uint64 ftl_start_address;
   uint32 ftl_version;
};

typedef enum {
   NAND_CRCODE_BMT_REMARK = 1,
   NAND_CRCODE_BRLYT_ADDR,
   NAND_CRCODE_FLAG_SUPPORT_CROSS_WRITE,
   NAND_CRCODE_DL_INFO_ADDR,
   NAND_CRCODE_GET_FTL_CHUNK_SIZE,
   NAND_CRCODE_GET_FTL_RECORD_SIZE,
}NAND_CRTL_CODE_E;

//interface
extern status_t interface_nand_init();
extern status_t interface_get_nand_info(struct nand_info_struct* info);
extern status_t interface_nand_read(uint64 address,  uint8* buffer, uint64 length,struct nand_extension* nand_extension);
extern status_t interface_nand_write(uint64 address, uint8* buffer, uint64 length,struct nand_extension* nand_extension);
extern status_t interface_nand_erase(uint64 address, uint64 length,struct nand_extension* nand_extension, const struct progress_cb* cb);
extern status_t interface_nand_device_ctrl(uint32 ctrl_code, void* in, uint32 in_len, void* out, uint32 out_len, uint32* ret_len);
extern status_t interface_nand_create_nfb_hdr(uint8* data, uint32 size);
#endif
