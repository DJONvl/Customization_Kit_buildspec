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

#ifndef SECLIB_SEJ_H
#define SECLIB_SEJ_H

#include "stdint.h"

/******************************************************************************
 * CONSTANT DEFINITIONS                                                       
 ******************************************************************************/
#define SEJ_AES_MAX_KEY_SZ          (32)            /* AES256 */
#define AES_IV_SZ                   (16)            /* 128bit IV for CBC mode */
#define AES_BLK_SZ                  (16)            /* 128bit block size */

/******************************************************************************
 * TYPE DEFINITIONS                                                           
 ******************************************************************************/
typedef enum
{   
    AES_ECB_MODE,
    AES_CBC_MODE
} AES_MODE;

typedef enum
{   
    AES_DEC,
    AES_ENC
} AES_OPS;

typedef enum
{
    AES_KEY_128 = 16,
    AES_KEY_192 = 24,
    AES_KEY_256 = 32
} AES_KEY;

typedef enum
{   
    AES_SW_KEY,
    AES_HW_KEY,
    AES_HW_WRAP_KEY,
    AES_RID_KEY,
    AES_CUSTOM_KEY
} AES_KEY_ID;

typedef struct {
    u8 vector[AES_IV_SZ];
} AES_IV;

typedef struct {
    u32 size;
    u8  seed[SEJ_AES_MAX_KEY_SZ];
} AES_KEY_SEED;

struct sej_context {
    AES_IV iv;
    AES_IV custom_iv;
    u32    blk_sz;
    u8     sw_key[SEJ_AES_MAX_KEY_SZ];
    u8     hw_key[SEJ_AES_MAX_KEY_SZ];
    u8     rid_key[SEJ_AES_MAX_KEY_SZ];
    u8     custom_key[SEJ_AES_MAX_KEY_SZ];
    u8     use_custom_iv;
};

/******************************************************************************
 * HW KEY SIZE                                                  
 ******************************************************************************/
#define SEJ_HW_KEY_SZ               (AES_KEY_128)   /* 128-bits */


/******************************************************************************
 * MACROS DEFINITIONS                                                         
 ******************************************************************************/
#define AES_BLK_SZ_ALIGN(size)      ((size) & ~((AES_BLK_SZ << 3) - 1))


/******************************************************************************
 * INTERNAL FUNCTIONs
 ******************************************************************************/
extern int sp_swotp_set (void);
extern int sp_swotp_clean (void);
 
extern void SEJ_V3_Init(u8 encode);
extern void SEJ_V3_Run(volatile u32 *p_src, u32 src_len, volatile u32 *p_dst);
extern void SEJ_V3_Terminate(void);
extern void SEJ_SWOTP_Set(u32 *p_cust_name);
extern void SEJ_SWOTP_Clear(void);
 
extern u32 sej_init(AES_KEY_SEED *keyseed);
extern u32 sej_deinit(void);
extern u32 sej_set_mode(AES_MODE mode);
extern u32 sej_set_iv(AES_IV *iv);
extern int sej_set_key(AES_KEY_ID id, AES_KEY key);
extern int sej_do_aes(AES_OPS ops, u8 *src, u8 *dst, u32 size);
extern int sp_sej_dec(u8 *buf, u32 size, u8 bAC);
extern int sp_sej_enc(u8 *buf, u32 size, u8 bAC);


#endif /* SECLIB_SEJ_H */

