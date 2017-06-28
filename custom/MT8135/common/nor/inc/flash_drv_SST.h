/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _FLASH_DRV_SST_H_
#define _FLASH_DRV_SST_H_

#include "flash.h"

#include "flash_drv_AMD.h"

// SST Command Address 
#define SST_CMD_ADDR1							0x555
#define SST_CMD_ADDR2							0x2AA
#define SST_SA_ADDR							    0x0

// SST Block Erase 
#define SST_BLOCK_ERASE_DATA1					0xAA
#define SST_BLOCK_ERASE_DATA2					0x55
#define SST_BLOCK_ERASE_DATA3					0x80
#define SST_BLOCK_ERASE_DATA4					0xAA
#define SST_BLOCK_ERASE_DATA5					0x55
#define SST_BLOCK_ERASE_DATA6					0x30

// SST Polling Status Bits 
#define SST_DQ7_POLL_BIT						0x0080
#define SST_DQ6_TOGGLE_BIT						0x0040


#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
extern const NOR_CMD_Callback_S		SST_CMD_CB_WORD_PGM;

//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------
extern const NOR_Die_Layout_S		SST_16;
extern const NOR_Die_Layout_S		SST_32;

//------------------------------------------------------------------------------
// Erase Related Callback Function                                              
//------------------------------------------------------------------------------
extern void		SST_Erase_CMD(const uint32 blockaddr);
extern STATUS_E	SST_Erase_CheckDone_By_Polling(const uint32 blockaddr);
//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
extern STATUS_E	SST_Program_CheckDone_By_Toggle(const uint32 prog_addr);


#ifdef __cplusplus
}
#endif

#endif
