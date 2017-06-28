/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2011
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

#ifndef _ROMINFO_INTERNAL_H_
#define _ROMINFO_INTERNAL_H_

#include "stdint.h"

#define SEC_BUF_LEN (0x3000)

#define ROM_INFO_EMMC_SEARCH_OFFSET  		(0x800)
#define ROM_INFO_UFS_SEARCH_OFFSET   		(0x1000)
#define ROM_INFO_SEARCH_LENGTH       		(0xa0000)
#define ROM_INFO_NAND_SLC_SEARCH_OFFSET		(0x20000)

#define SECCFG_SEARCH_LENGTH         (0xa0000)

#define ANDROID_ROM_INFO_BEGIN       "AND_ROMINFO_v"
#define ANDROID_ROM_INFO_BEGIN_LEN   13

#define CUSTOMER_NAME_SIZE           32

#define GFH_MAGIC_MASK               (0x00ffffff)
#define GFH_MAGIC                    (0x004D4D4D)
#define GFH_BROM_FILE_INFO_TYPE      (0x0)
#define GFH_BROM_ANTI_CLONE_TYPE     (0x2)
#define GFH_BROM_SEC_CFG_TYPE        (0x8)
#define GFH_MAGIC_SIZE               (4)

#define FLASHTOOL_CFG_MAGIC                (0x544F4F4C)
#define FLASHTOOL_CFG_MAGIC_64             (0x544F4F5C)     /* support sign format v4 64bits 4G+ */

#define SDL_DISABLE               (0x00)
#define SDL_FORCE_ENABLE          (0x11)
#define SDL_ENABLE_ON_SCHIP       (0x22)

#define SBOOT_DISABLE                (0x00)
#define SBOOT_FORCE_ENABLE           (0x11)
#define SBOOT_ENABLE_ON_SCHIP        (0x22)

typedef struct {
    u32   m_magic_ver;
    u16   m_size;
    u16   m_type;
    char  m_identifier[12];       // including '\0'
    u32   m_file_ver;
    u16   m_file_type;
    u8    m_flash_dev;
    u8    m_sig_type;
    u32   m_load_addr;
    u32   m_file_len;
    u32   m_max_size;
    u32   m_content_offset;
    u32   m_sig_len;
    u32   m_jump_offset;
    u32   m_attr;
} GFH_BROM_FILE_INFO_v1;

typedef struct {
   u32   m_magic_ver;
   u16   m_size;
   u16   m_type;
   u8    m_ac_b2k;
   u8    m_ac_b2c;
   u32   m_ac_offset;
   u32   m_ac_length;
} GFH_BROM_ANTI_CLONE_v1;

typedef struct {
   u32   m_magic_ver;
   u16   m_size;
   u16   m_type;
   u32   m_attr;
   u8    m_cust_name[CUSTOMER_NAME_SIZE];
   u32   m_cmd_mode_permanent_dis;
} GFH_BROM_SEC_CFG_v1;

typedef struct {
   u8   m_img_name [16];
   u32  m_img_offset_high;                     /* change m_img_offset to m_img_offset_high to support 64 bits sign format v4*/
   u32  m_img_offset_low;                      /* change m_img_length to m_img_offset_low to support 64 bits sign format v4*/
} BYPASS_CHECK_IMAGE_T;

typedef struct {
   u32 m_magic_num;
   BYPASS_CHECK_IMAGE_T m_bypass_check_img [3];
} FLASHTOOL_SECCFG_T;

#define FLASHTOOL_NON_SLA_FORBID_MAGIC             (0x544F4F4D)
#define FLASHTOOL_NON_SLA_FORBID_MAGIC_64          (0x544F4F5D)   /* support sign format v4 64bits 4G+ */


typedef struct {
   u8  m_img_name [16];
   u32 m_img_offset_high;                    /* change m_img_offset to m_img_offset_high to support 64 bits sign format v4*/
   u32 m_img_offset_low;                     /* change m_img_length to m_img_offset_low to support 64 bits sign format v4*/
} FORBID_DOWNLOAD_IMAGE_T;

#define FLASHTOOL_NON_SLA_FORBID_CFG_SIZE              (52)
typedef struct {
   u32 m_forbid_magic_num;
   FORBID_DOWNLOAD_IMAGE_T m_forbid_dl_nsla_img [2];
} FLASHTOOL_FORBID_DOWNLOAD_NSLA_T;


typedef struct {
   u8   m_identifier[16];
   u32  m_sec_cfg_ver;
   u32  m_sec_usb_dl;
   u32  m_sec_boot;
   u32  m_sec_modem_auth;
   u32  m_sec_sds_en;
   u8   m_seccfg_ac_en;
   u8   m_sec_aes_legacy;
   u8   m_secro_ac_en;
   u8   m_sml_aes_key_ac_en;
   u32  reserve[3];
} AND_SECCTRL_T;

typedef struct {
   u8   name[9][10];
} AND_SECBOOT_CHECK_PART_T;

typedef struct {
   u8  m_identifier[16];
   u32 m_sec_key_ver;
   /* rsa key */
   u8  img_auth_rsa_n[256]; /* key length is 256/2 = 128 */
   u8  img_auth_rsa_e[5];
   /* aes key */
   u8  sml_aes_key[32];
   /* crypto seed */
   u8  crypto_seed[16];
} AND_SECKEY_T;

typedef struct {
   u8                   m_identifier[16];
   u32                  m_rom_info_ver;
   u8                   m_platform_id[16];
   u8                   m_project_id[16];
   u32                  m_sec_ro_exist;
   u32                  m_sec_ro_offset;
   u32                  m_sec_ro_length;
   u32                  m_ac_offset;
   u32                  m_ac_length;
   u32                  m_sec_cfg_offset;
   u32                  m_sec_cfg_length;
   FLASHTOOL_SECCFG_T   m_flashtool_cfg;
   FLASHTOOL_FORBID_DOWNLOAD_NSLA_T m_flashtool_forbid_dl_nsla_cfg;
   AND_SECCTRL_T        m_SEC_CTRL;
   u8                   m_reserve2[18];
   AND_SECBOOT_CHECK_PART_T  m_sec_boot_check_part;
   AND_SECKEY_T         m_sec_key;
} MTK_ROMInfo_ST;

#endif /* _ROMINFO_INTERNAL_H_ */

