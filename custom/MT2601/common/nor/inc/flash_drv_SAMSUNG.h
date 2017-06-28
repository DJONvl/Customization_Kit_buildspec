/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _FLASH_DRV_SAMSUNG_H_
#define _FLASH_DRV_SAMSUNG_H_

#include "flash_drv_AMD.h"

// SAMSUNG PBB Lock
#define SAMSUNG_UNPROTECTED_ADDR				0x42
#define	SAMSUNG_UNPROTECTED_DATA				0x60


#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
extern const NOR_CMD_Callback_S		AMD_SAMSUNG_CB_UNLOCK_BYPASS_PGM;

//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------
//
//OTP layout
//

//------------------------------------------------------------------------------
// TOSHIBA Related Callback Function                                         
//------------------------------------------------------------------------------
extern void SAMSUNG_PreProcess(void);

extern void		SAMSUNG_UnprotectChip(void);
extern bool		SAMSUNG_Protection_PPB_Program_CMD(uint32 blockaddr);

//------------------------------------------------------------------------------
// OTP Related Callback Function                                         
//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif


#endif
