/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2015
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
#ifndef SDMMC_HANDLER_H
#define SDMMC_HANDLER_H

//#include "sw_types.h"
#include "sdmmc.h"
#include "DOWNLOAD.H"

#ifdef __cplusplus
extern "C" {
#endif

   extern STATUS_E SDMMC_CheckRange(EMMC_Part_E part, const uint64 addr,const uint64 length);
   extern void SDMMC_Format(uint64 addr, uint64 length, bool bValidation);
   extern void SDMMC_SwitchPartition(void);
   extern void SDMMC_WriteImage(void);
   extern void SDMMC_WriteData(void);
   extern void SDMMC_Write_DL_info(char *dl_status,int len_s,bool clear);
   extern void SDMMC_Write_Cust_Info(uint8 *p_cust_info);
   extern void SDMMC_Read_DL_info(void);


   typedef struct{
      uint16 IsSparse;
      uint16 checksum;
   }IMAGE_CHECKSUM;

   //FIXME: Count of g_Image_Checksum array could be reduced to reduce ZI region size of DA
   extern IMAGE_CHECKSUM g_Image_Checksum[MAX_LOAD_SECTIONS];
#if 0
   extern DL_STATUS_EMMC g_download_info;
#else
   extern DL_STATUS_V1 g_download_info;
#endif
#define DL_INFO_SIZE 2048
#ifdef __cplusplus
}
#endif


#endif  /* SDMMC_HANDLER_H */

