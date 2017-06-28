/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#ifndef _NAND_DRV_SAMSUNG_H_
#define _NAND_DRV_SAMSUNG_H_

#include "nand_dev_tbl.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// SAMSUNG Command Set                                                          
//------------------------------------------------------------------------------
extern const NAND_CommandSet_S		g_NAND_SAMSUNG_512_CMD_SET;
extern const NAND_CommandSet_S		g_NAND_SAMSUNG_2048_CMD_SET;

//------------------------------------------------------------------------------
// SAMSUNG OTP
//------------------------------------------------------------------------------
extern const NAND_OTP_Layout_S SAMSUNG_OTP;
extern const NAND_OTP_CMD_Callback_S	g_NAND_SAMSUNG_OTP_CB_FUNC_SET;

extern STATUS_E  NAND_SAMSUNG_OTP_Enter(const uint32  c_timeout);
extern STATUS_E  NAND_SAMSUNG_OTP_PageRead(const uint32  c_timeout, const uint32 row_addr, uint32 *p_data32, uint32  ecc_parity_from_reg[8]);
extern STATUS_E  NAND_SAMSUNG_OTP_PageProgram(const uint32  c_timeout, const uint32 row_addr, const uint32 *p_data32, uint32  ecc_parity_from_reg[8]);
extern STATUS_E  NAND_SAMSUNG_OTP_Lock(const uint32  c_timeout);
extern STATUS_E  NAND_SAMSUNG_OTP_Lock_CheckStatus(const uint32  c_timeout);
extern STATUS_E  NAND_SAMSUNG_OTP_Exit(const uint32  c_timeout);


#ifdef __cplusplus
}
#endif

#endif
