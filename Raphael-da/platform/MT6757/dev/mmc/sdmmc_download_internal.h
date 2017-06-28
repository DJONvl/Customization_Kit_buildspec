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

#ifndef SDMMC_DOWNLOAD_INTERNAL_H
#define SDMMC_DOWNLOAD_INTERNAL_H

#include "autoconfig.h"
#include "header_block_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

   extern bool 	SDMMC_Download_BL_CheckHeaderBlock_Internal(GFH_FLASH_DEV flash_dev, BR_Layout *p_brlyt);
   extern STATUS_E SDMMC_Download_BL_CreateHeaderBlock_Internal(GFH_FLASH_DEV flash_dev, BR_Layout *p_phy_brlyt);
   extern STATUS_E SDMMC_Download_BL_CheckProfile_Internal(uint32 *p_bl_next_begin_dev_addr, BL_EXT_Descriptor *p_bl_ext_descriptor, BL_Descriptor *p_phy_bl_descriptor, bool bHeaderBlockExist);
   extern STATUS_E SDMMC_Download_BL_WritePacketToFlash_Internal(BR_EXT_Layout *p_brlyt_ext, uint32 current_bl_index, uint32 * p_bl_current_dev_addr, bool bLastPacket, bool bHeaderBlockExist);
   extern STATUS_E SDMMC_Download_BL_PostProcess_Internal(void);
   extern STATUS_E SDMMC_Download_BL_GetHdrBlkLocation_Internal(uint32 *hdrblk_blkaddr);

#if defined(FEATURE_EMMC_SUPPORT) || defined(FEATURE_SDMMC_SUPPORT)
#define SDMMC_Download_BL_CheckHeaderBlock(f,x)         SDMMC_Download_BL_CheckHeaderBlock_Internal(f,x)
#define SDMMC_Download_BL_CreateHeaderBlock(f,x)        SDMMC_Download_BL_CreateHeaderBlock_Internal(f,x)
#define SDMMC_Download_BL_CheckProfile(w,x,y,z)         SDMMC_Download_BL_CheckProfile_Internal(w,x,y,z)
#define SDMMC_Download_BL_WritePacketToFlash(v,w,x,y,z) SDMMC_Download_BL_WritePacketToFlash_Internal(v,w,x,y,z)
#define SDMMC_Download_BL_PostProcess()                 SDMMC_Download_BL_PostProcess_Internal()
#define SDMMC_Download_BL_GetHdrBlkLocation(x)          SDMMC_Download_BL_GetHdrBlkLocation_Internal(x)
#if defined(BB_MT6582)
#define SDMMC_Download_BL_EraseHeaderBlock(f)           SDMMC_Download_BL_EraseHeaderBlock_Internal(f)
#endif
#else
#define SDMMC_Download_BL_CheckHeaderBlock(f,x)         (FALSE)
#define SDMMC_Download_BL_CreateHeaderBlock(f,x)        (S_DA_UNSUPPORTED_BBCHIP)
#define SDMMC_Download_BL_CheckProfile(w,x,y,z)         (S_DA_UNSUPPORTED_BBCHIP)
#define SDMMC_Download_BL_WritePacketToFlash(v,w,x,y,z) (S_DA_UNSUPPORTED_BBCHIP)
#define SDMMC_Download_BL_PostProcess()                 (S_DA_UNSUPPORTED_BBCHIP)
#define SDMMC_Download_BL_GetHdrBlkLocation(x)          (S_DA_UNSUPPORTED_BBCHIP)
#define SDMMC_Download_BL_EraseHeaderBlock(f)           (S_DA_UNSUPPORTED_BBCHIP)
#endif

#ifdef __cplusplus
}
#endif


#endif  /* SDMMC_DOWNLOAD_INTERNAL_H */

