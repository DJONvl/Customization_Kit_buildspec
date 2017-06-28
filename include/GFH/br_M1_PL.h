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
 *   br_M1_PL.h
 *
 * Project:
 * --------
 *   BOOTROM
 *
 * Description:
 * ------------
 *   MTK RSA public/private key format
 *
 * Author:
 * -------
 *   Clean Room
 *
 ****************************************************************************/
#ifndef __BR_M1_PL_H__
#define __BR_M1_PL_H__

#ifdef _WIN32
#include "SW_TYPES.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define RSAWORD unsigned short

#define CT_RSA_1024_LEN     128 // 1024 bits
#define CT_RSA_2048_LEN     256 // 2048 bits
#define CT_MAX_RSA_LEN      CT_RSA_2048_LEN

#define MAX_RSA_KEY_LEN         2048

#define M1_1024_CUT_LEN      (CT_RSA_1024_LEN-2)  // cut length must be <= (N_LEN - 1) 
#define M1_2048_CUT_LEN      (CT_RSA_2048_LEN-2)  // cut length must be <= (N_LEN - 1) 

//==============================================================================
// MTK RSA public/private key structure
//==============================================================================

typedef struct CT_M1_Key
{
    U32     nELen;
    U32     nNLen;
    U16     MCoeff;
    U16     nCutLength;
    U16     E_Key[CT_MAX_RSA_LEN>>1];
    U16     N_Key[CT_MAX_RSA_LEN>>1];   // 256, 512, 1024, 2048 bits
} CT_M1_Key;

typedef struct 
{
	int bEncrypt; //1: to encrypt, 0: to decrypt
	int nCutLength; //length of each clip of "plain text". This needs to be smaller than nNLen.
	int nTextLen; //length of pText (plain or encrypted text ) in RSAWORD, 
	RSAWORD MCoeff; //lease-significant WORD of Q: RS - NQ = 1 (R is (65536^N_LEN))
	//For encrypting, if (nTextLen % nCutLength != 0), then the last clip will be padded with enough 0s
	RSAWORD *pText;
	int nNLen; // N
	RSAWORD *pN;
	int nELen; // E or D
	RSAWORD *pE;
}
RsaEncodeRequest;

typedef struct
{
    int nLen;
    RSAWORD *pText;
}
RsaEncodeResult;

#ifdef __cplusplus
}
#endif

void RSAInit(void);
int RSAEncode(const RsaEncodeRequest *pReq, RsaEncodeResult *pRes);

#endif
