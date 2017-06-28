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

#ifndef __EMMC_HEADER_H__
#define __EMMC_HEADER_H__

//#include "sw_types.h"

#define	EMMC_HEADER_ID	"EMMC_BOOT"
#define	EMMC_HEADER_VER	1

typedef struct
{
   U8          m_identifier[12];	// "EMMC_BOOT"
   U32         m_ver;
   U32         m_record_region_size;
} EMMC_HEADER;


#endif //__EMMC_HEADER_H__
