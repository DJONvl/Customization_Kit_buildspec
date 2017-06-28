/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   GFH_file_info.h
 *
 * Project:
 * --------
 *   BOOTROM
 *
 * Description:
 * ------------
 *   FILE_INFO(the 1st GFH) GFH structure definition
 *
 * Author:
 * -------
 *   Clean Room
 *
 ****************************************************************************/
#ifndef __GFH_FILE_INFO_H__
#define __GFH_FILE_INFO_H__

#include "SW_TYPES.H"
#include "br_GFH.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// FILE_INFO definition (This is the 1st GFH header)
//==============================================================================

typedef enum {
     GFH_FILE_NONE          = 0             //Recognized by BootROM
    ,ARM_BL                 = 1
    ,ARM_EXT_BL             = 2
    ,FCORE_BL               = 3
    ,SCTRL_CERT             = 4
    ,TOOL_AUTH              = 5
    ,FILE_MTK_RESERVED1     = 6
    ,EPP                    = 7

    ,MAUI_BINRAY            = 100           //MAUI binary group
    ,PRIMARY_MAUI           = 100
    ,SECONDARY_MAUI         = 101
    ,ON_DEMAND_PAGING       = 102
    ,THIRD_ROM              = 103
    ,MAUI_BINARY_END

    ,RESOURCE_BINARY        = 200           //Resource binary group
    ,CUSTOM_PACK            = 200
    ,LANGUAGE_PACK          = 201
    ,RESOURCE_BINARY_END 
    
    ,SECURE_RO              = 300           //Secure structure group
    ,SECURE_RO_S            = 300
    ,SECURE_RO_ME           = 301
    ,SECURE_RO_END 
    
    ,GFH_FILE_END           = (U16)65535
} GFH_FILE_TYPE;

typedef enum {
     FLASH_DEV_NONE         = 0
    ,F_NOR                  = 1
    ,F_NAND_SEQUENTIAL      = 2
    ,F_NAND_TTBL            = 3
    ,F_NAND_FDM50           = 4
    ,F_EMMC_BOOT_REGION     = 5
    ,F_EMMC_DATA_REGION     = 6
    ,F_SF                   = 7
    ,F_SDMMC                = 8
    ,F_SPI_NAND_SEQUENTIAL  = 9
    ,FLASH_DEV_END          = (U8)255
} GFH_FLASH_DEV;

typedef enum {
     SIG_NONE               = 0
    ,SIG_PHASH              = 1
    ,SIG_SINGLE             = 2
    ,SIG_SINGLE_AND_PHASH   = 3
    ,SIG_MULTI              = 4
    ,SIG_TYPE_NUM
    ,SIG_TYPE_END           = (U8)255
} GFH_SIG_TYPE;

#define GFH_FILE_INFO_ID                "FILE_INFO"
#define GFH_MAX_CONTENT_OFFSET          0x100000    // 1MB
#define GFH_FILE_SIZE_NA                0xFFFFFFFF
#define GFH_FILE_MAX_SIZE_IGNORED       0xFFFFFFFF
#define GFH_FILE_MAX_SIZE_BY_POST_BUILD 0x00000000

#define FILE_INFO_ATTR_POST_BUILD_DONE  0x00000001

typedef struct {
    GFH_Header      m_gfh_hdr;
    char            m_identifier[12];       // including '\0'
    U32             m_file_ver;
    U16             m_file_type;
    U8              m_flash_dev;
    U8              m_sig_type;
    U32             m_load_addr;
    U32             m_file_len;
    U32             m_max_size;
    U32             m_content_offset;
    U32             m_sig_len;
    U32             m_jump_offset;
    U32             m_attr;
} GFH_FILE_INFO_v1;

extern U32  GFH_FILE_INFO_BasicCheck(const GFH_FILE_INFO_v1 *  p_gfh_file_info);
extern U32  GFH_FILE_INFO_IsValid(const GFH_FILE_INFO_v1 *  p_gfh_file_info);

#ifdef __cplusplus
}
#endif

#endif
