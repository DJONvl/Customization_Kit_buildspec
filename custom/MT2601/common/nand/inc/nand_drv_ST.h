/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#ifndef _NAND_DRV_ST_H_
#define _NAND_DRV_ST_H_

#include "nand_dev_tbl.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// STMicroelectronics Command Set                                               
//------------------------------------------------------------------------------
extern const NAND_CommandSet_S		g_NAND_ST_512_CMD_SET;
extern const NAND_CommandSet_S		g_NAND_ST_2048_CMD_SET;

//------------------------------------------------------------------------------
// STMicroelectronics Callback Function Set                                     
//------------------------------------------------------------------------------
extern const NAND_CMD_Callback_S	g_NAND_ST_CB_FUNC_SET;
extern const NAND_CMD_Callback_S	g_NAND_ST_CB_FUNC_SET_WITHOUT_COPYBACK;

//------------------------------------------------------------------------------
// Whole Chip Unlock                                                            
//------------------------------------------------------------------------------
STATUS_E  NAND_ST_WholeChipUnlock(const uint32  c_timeout);
STATUS_E  NAND_ST_WholeChipUnlock_V3(const uint32  c_timeout);

#ifdef __cplusplus
}
#endif

#endif