/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _FLASH_DRV_SHARP_H_
#define _FLASH_DRV_SHARP_H_

#include "flash_drv_INTEL.h"

// READ CMD 

// PROGRAM CMD 

// ERASE 

// LOCK/UNLOCK 

// Status Register Bit 
#define SHARP_SR7_WSMS_BIT				0x0080
#define SHARP_SR5_ES_BIT				0x0020
#define SHARP_SR4_PS_BIT				0x0010
#define SHARP_SR3_VPPS_BIT				0x0008
#define SHARP_SR1_DPS_BIT				0x0002

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
extern const NOR_CMD_Callback_S		SHARP_CMD_CB_WORD_PGM;
extern const NOR_CMD_Callback_S		SHARP_CMD_CB_16WORD_BUF_PGM;
extern const NOR_CMD_Callback_S		SHARP_CMD_CB_WORD_PGM_NO_UNLOCK;
//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------
extern const NOR_Die_Layout_S			SHARP_T_32;

//OTP layout
extern const NOR_OTP_Layout_S			SHARP_OTP_128_T;

//------------------------------------------------------------------------------
// Check Device ID Callback Function                                            
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Check If Device Is Idle Callback Function                                    
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Erase Related Callback Function                                              
//------------------------------------------------------------------------------
extern void		SHARP_Erase_Block_CMD(const uint32 blockaddr);
extern STATUS_E	SHARP_CheckDone(const uint32 blockaddr);
//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
extern void		SHARP_PreProcess_16WORD_BUFPGM(void);

#ifdef __cplusplus
}
#endif

#endif
