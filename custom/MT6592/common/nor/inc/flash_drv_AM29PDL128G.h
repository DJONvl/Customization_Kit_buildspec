/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _FLASH_DRV_AM29PDL128G_H_
#define _FLASH_DRV_AM29PDL128G_H_

#include "flash_drv_AMD.h"

// AMD AM29PDL128G Command Address 
#define AMD_AM29PDL128G_CMD_ADDR1					0xAAA	
#define AMD_AM29PDL128G_CMD_ADDR2					0x555
#define AMD_AM29PDL128G_SA_ADDR						0x0

// AMD AM29PDL128G Device ID Read Address 
#define AMD_AM29PDL128G_MANUFACTURE_ID_ADDR			0x00	// 0x00 * 2 
#define AMD_AM29PDL128G_DEVICE_CODE_ADDR			0x02	// 0x01 * 2 
#define AMD_AM29PDL128G_EXT_DEVICE_CODE1_ADDR		0x1C	// 0x0E * 2 
#define AMD_AM29PDL128G_EXT_DEVICE_CODE2_ADDR		0x1E	// 0x0F * 2 

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
extern const NOR_CMD_Callback_S		AMD_AM29PDL128G_CMD_CB_UNLOCK_BYPASS_PGM;

//------------------------------------------------------------------------------
// Check Device ID Callback Function                                            
//------------------------------------------------------------------------------
//extern bool		AMD_AM29PDL128G_CheckDevID(const uint16 dev, volatile uint16 *die1_addr, volatile uint16 *die2_addr);

//------------------------------------------------------------------------------
// Erase Relative Callback Function                                             
//------------------------------------------------------------------------------
extern void		AMD_AM29PDL128G_Erase_CMD(const uint32 blockaddr);

//------------------------------------------------------------------------------
// Program Relative Callback Function                                           
//------------------------------------------------------------------------------
extern void		AMD_AM29PDL128G_UnlockBypass_Enter_CMD(const uint32 blockaddr);
extern void		AMD_AM29PDL128G_UnlockBypass_Exit_CMD(const uint32 blockaddr);
extern void		AMD_AM29PDL128G_UnlockBypass_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);

#ifdef __cplusplus
}
#endif

#endif
