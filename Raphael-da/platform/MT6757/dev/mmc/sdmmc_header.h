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
*  20131116 MTK81353 Delete unused code.
*
*
*******************************************************************************/

#ifndef __SDMMC_HEADER_H__
#define __SDMMC_HEADER_H__

//#include "sw_types.h"

#define SDMMC_HEADER_ID      "SDMMC_BOOT"
#define SDMMC_HEADER_VER     1

typedef struct {
   char            m_identifier[12];   // including '\0'
   U32             m_ver;
   U32             m_record_region_size; // NAND: in page
   // NOR/eMMC/SD/MMC/SFlash: in byte
} SDMMC_HEADER;

#endif

