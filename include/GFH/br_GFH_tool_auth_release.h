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
 *   GFH_tool_auth.h
 *
 * Project:
 * --------
 *   BOOTROM
 *
 * Description:
 * ------------
 *   TOOL_AUTH(Tool Authentication) GFH structure definition 
 *
 * Author:
 * -------
 *   Clean Room
 *
 ****************************************************************************/
#ifndef __GFH_TOOL_AUTH_H__
#define __GFH_TOOL_AUTH_H__

#include "SW_TYPES.H"
#include "br_GFH.h"
#include "mtk_struct_sync.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// MTK RSA public/private key structure
//==============================================================================

//this structure and define value are aligned to tool, should not change at will!

#define M1_1024_KEY_LEN     128 // 1024 bits
#define M1_2048_KEY_LEN     256 // 2048 bits
#if defined(M1_MAX_KEY_LEN)
#undef M1_MAX_KEY_LEN
#endif
#define M1_MAX_KEY_LEN      M1_2048_KEY_LEN
#define M1_CUT_LEN          (M1_MAX_KEY_LEN-2)  // cut length must be <= (N_LEN - 1) 

typedef struct {
    GFH_Header      m_gfh_hdr;
    char            m_cust_name[SECURE_CUST_NAME_LEN];
    U8              dummy[GFH_TOOL_AUTH_v1_padding];
} GFH_TOOL_AUTH_v1;

extern U32  GFH_TOOL_AUTH_IsValid(const GFH_FILE_INFO_v1  *p_gfh_file_info,
                                  const GFH_TOOL_AUTH_v1  *p_gfh_tool_auth);

#ifdef __cplusplus
}
#endif

#endif
