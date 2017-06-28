/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  History:
*  YYYYMMDD MTK12345 Description
*  --------------------
*  20131116 MTK81353 Initial version.
*
*
*******************************************************************************/


#ifndef BLKDEV_H
#define BLKDEV_H

#include "common_struct.h"
#include "sys/types.h"

/* blkdev type */
enum {
   BLKDEV_NAND,
   BLKDEV_SDMMC,
   BLKDEV_NOR
};

typedef struct blkdev blkdev_t;

struct blkdev {
   uint32 type;       /* block device type */
   uint32 blksz;      /* block size. (read/write unit) */
   uint32 erasesz;    /* erase size */
   uint32 blks;       /* number of blocks in the device */
   uint8 *blkbuf;     /* block size buffer */
   void *priv;     /* device private data */    
   blkdev_t *next; /* next block device */
   int (*bread)(blkdev_t *bdev, uint32 blknr, uint32 blks, uint8 *buf);
   int (*bwrite)(blkdev_t *bdev, uint32 blknr, uint32 blks, uint8 *buf);
   int (*bread_part)(blkdev_t *bdev, uint32 blknr, uint32 blks, uint8 *buf, emmc_section_e part);
   int (*bwrite_part)(blkdev_t *bdev, uint32 blknr, uint32 blks, uint8 *buf, emmc_section_e part);
};

extern int blkdev_register(blkdev_t *bdev);
extern int blkdev_read(blkdev_t *bdev, uint64 src, uint64 size, uint8 *dst);
extern int blkdev_write(blkdev_t *bdev, uint64 dst, uint64 size, uint8 *src);
extern int blkdev_bread(blkdev_t *bdev, uint32 blknr, uint32 blks, uint8 *buf);
extern int blkdev_bwrite(blkdev_t *bdev, uint32 blknr, uint32 blks, uint8 *buf);
extern blkdev_t *blkdev_get(uint32 type);

#endif /* BLKDEV_H */
