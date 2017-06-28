/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#ifndef _NAND_DRV_KINGSTON_H_
#define _NAND_DRV_KINGSTON_H_

#include "nand_dev_tbl.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// KINGSTON Command Set                                                          
//------------------------------------------------------------------------------
extern const NAND_CommandSet_S		g_NAND_KINGSTON_512_CMD_SET;
extern const NAND_CommandSet_S		g_NAND_KINGSTON_2048_CMD_SET;

//------------------------------------------------------------------------------
// KINGSTON Callback Function Set                                                
//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif
