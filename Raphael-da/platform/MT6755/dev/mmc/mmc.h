#ifndef __MMC_H__
#define __MMC_H__
#include "error_code.h"
#include "blkdev.h"

struct mmc_info_struct;

//definations
extern int g_boot_host_id; 
#define SDMMC_DEVID  g_boot_host_id  

#define SDMMC_BLK_BITS              (9)
#define SDMMC_BLK_SIZE              (1 << SDMMC_BLK_BITS)
#define SDMMC_BLK_NUM(size)         ((unsigned long long)(size) / SDMMC_BLK_SIZE)
#define SDMMC_SECTOR_SIZE           (SDMMC_BLK_SIZE)

extern unsigned long long g_sdmmc_size;
extern unsigned long long g_sdmmc_user_size;

status_t mmc_bwrite(blkdev_t *bdev, uint32 blknr, uint32 blks, uint8 *buf);
status_t mmc_bread(blkdev_t *bdev, uint32 blknr, uint32 blks, uint8 *buf);
status_t set_boot_channel(uint32 channel);
status_t get_mmc_type(uint32 *type);
status_t get_mmc_cid(uint32 *cid);
status_t get_mmc_fwver(uint8 *fwid);
status_t get_mmc_size(uint32 section, uint64 *size);
status_t sdmmc_addr_trans_tbl_init();
status_t err_trans(int error);
uint32 mmc_getwpgsize(void);
void dump_mmc_info(struct mmc_info_struct *info);
uint32 mmc_get_erase_size(void);


#endif
