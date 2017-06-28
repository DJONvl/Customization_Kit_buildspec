/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _FLASH_DRV_TOSHIBA_H_
#define _FLASH_DRV_TOSHIBA_H_

#include "flash_drv_AMD.h"

// TOSHIBA AUTO PAGE Program 
#define TOSHIBA_PAGE_PROGRAM_8WORD			0xE6 
#define TOSHIBA_PAGE_PROGRAM_32WORD			0xE8

//
//OTP_Drivers
//
//Toshiba Hidden ROM Protect cmd
#define TOSHIBA_HIDDEN_ROM_RPOTECT_DATA1		0xAA
#define TOSHIBA_HIDDEN_ROM_RPOTECT_DATA2		0x55
#define TOSHIBA_HIDDEN_ROM_RPOTECT_DATA3		0x60
#define TOSHIBA_HIDDEN_ROM_RPOTECT_DATA4		0x68

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
extern const NOR_CMD_Callback_S		TOSHIBA_CMD_CB_BUF_PGM;
extern const NOR_CMD_Callback_S		TOSHIBA_CMD_CB_PAGE_8WORDS_PGM;
extern const NOR_OTP_CMD_Callback_S 	TOSHIBA_HIDDEN_ROM_CMD_CB;

//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------
//
//OTP layout
//
extern const NOR_OTP_Layout_S		TOSHIBA_HIDDEN_ROM_T_32K;
extern const NOR_OTP_Layout_S		TOSHIBA_HIDDEN_ROM_T_64K;

//------------------------------------------------------------------------------
// TOSHIBA Related Callback Function                                         
//------------------------------------------------------------------------------
extern void TOSHIBA_PreProcess(void);
extern void TOSHIBA_Buf_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
extern void TOSHIBA_Page_8Words_PreProcess(void);
extern void TOSHIBA_Page_8Words_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);

//------------------------------------------------------------------------------
// OTP Related Callback Function                                         
//------------------------------------------------------------------------------
extern void 		TOSHIBA_HIDDEN_ROM_ENTER(const uint32 blockaddr);
extern void 		TOSHIBA_HIDDEN_ROM_EXIT(const uint32 blockaddr);
extern void 		TOSHIBA_HIDDEN_ROM_Read(const uint32 blockaddr, const uint32 prog_addr, volatile uint16 * data);
extern void 		TOSHIBA_HIDDEN_ROM_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern void 		TOSHIBA_HIDDEN_ROM_Lock(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern STATUS_E 	TOSHIBA_HIDDEN_ROM_Lock_CheckStatus(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);

#ifdef __cplusplus
}
#endif


#endif
