/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#ifndef _SNAND_DRV_VENDOR_1_H_
#define _SNAND_DRV_VENDOR_1_H_

#include "nand_dev_tbl.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// SAMSUNG Command Set
//------------------------------------------------------------------------------
extern const NAND_CommandSet_S		g_SNAND_VENDOR_1_2048_CMD_SET;

//------------------------------------------------------------------------------
// SAMSUNG OTP
//------------------------------------------------------------------------------
extern const NAND_OTP_Layout_S          SNAND_VENDOR_1_OTP;
extern const NAND_OTP_CMD_Callback_S	g_SNAND_VENDOR_1_OTP_CB_FUNC_SET;


#ifdef __cplusplus
}
#endif

#endif
