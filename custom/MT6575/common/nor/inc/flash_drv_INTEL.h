/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _FLASH_DRV_INTEL_H_
#define _FLASH_DRV_INTEL_H_

#include "flash.h"

// READ CMD 
#define INTEL_CMD_READ_ARRAY			0xFF
#define INTEL_CMD_READ_ID				0x90
#define INTEL_CMD_READ_SR				0x70
#define INTEL_CMD_CLR_SR				0x50

// PROGRAM CMD 
#define INTEL_CMD_WORD_PGM				0x40
#define INTEL_CMD_BUF_PGM				0xE8
#define INTEL_CMD_SIBLEY_WORD_PGM		0x41
#define INTEL_CMD_SIBLEY_BUF_PGM		0xE9
#define INTEL_CMD_BUF_PGM_CNF			0xD0
#define INTEL_CMD_BUF_EFPGM				0x80
#define INTEL_CMD_BUF_EFPGM_CNF			0xD0

// ERASE 
#define INTEL_CMD_ERASE_STEP1			0x20
#define INTEL_CMD_ERASE_STEP2			0xD0

// LOCK/UNLOCK 
#define INTEL_CMD_SW_UNLOCK_STEP1		0x60
#define INTEL_CMD_SW_UNLOCK_STEP2		0xD0

// Status Register Bit 
#define INTEL_SR7_DWS_BIT				0x0080
#define INTEL_SR6_ESS_BIT				0x0040
#define INTEL_SR5_ES_BIT				0x0020
#define INTEL_SR4_PS_BIT				0x0010
#define INTEL_SR3_VPPS_BIT				0x0008
#define INTEL_SR2_PSS_BIT				0x0004
#define INTEL_SR1_BLS_BIT				0x0002
#define INTEL_SR0_PWS_BIT				0x0001

// Extended Status (For Sibley) 
#define INTEL_RPS_MASK								0x0300
#define INTEL_RPS_REWRITE_OBJ_MODE_REGION			0x0100
#define INTEL_RPS_WRITE_B_HALF_IN_CTRL_MODE_REGION	0x0200
#define INTEL_RPS_ILLEGAL_CMD						0x0300

//
//OTP CMDs
//
#define INTEL_CMD_OTP_PROGRAM_PR		0xC0

#define INTEL_OTP_LOCK_REG_0_ADDR		0x100
#define INTEL_OTP_LOCK_REG_1_ADDR		0x112

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
extern const NOR_CMD_Callback_S		INTEL_CMD_CB_WORD_PGM;
extern const NOR_CMD_Callback_S		INTEL_CMD_CB_32WORD_BUF_PGM;
extern const NOR_CMD_Callback_S		INTEL_CMD_CB_SIBLEY_BUF_PGM;
extern const NOR_OTP_CMD_Callback_S 	INTEL_OTP_CMD_CB;
	
//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------

// L18/L30 series (pure 64KW sectors) 
extern const NOR_Die_Layout_S		INTEL_L_64;
extern const NOR_Die_Layout_S		INTEL_L_128;
extern const NOR_Die_Layout_S		INTEL_L_256;
// L18/L30 series (Top Boot) 
extern const NOR_Die_Layout_S		INTEL_L_T_64;
extern const NOR_Die_Layout_S		INTEL_L_T_128;
extern const NOR_Die_Layout_S		INTEL_L_T_256;
// L18/L30 series (Bottom Boot) 
extern const NOR_Die_Layout_S		INTEL_L_B_64;
extern const NOR_Die_Layout_S		INTEL_L_B_128;
extern const NOR_Die_Layout_S		INTEL_L_B_256;


// W18/W30 series (pure 32KW sectors) 
extern const NOR_Die_Layout_S		INTEL_W_32;
extern const NOR_Die_Layout_S		INTEL_W_64;
extern const NOR_Die_Layout_S		INTEL_W_128;
// W18/W30 series (Top Boot) 
extern const NOR_Die_Layout_S		INTEL_W_T_16;
extern const NOR_Die_Layout_S		INTEL_W_T_32;
extern const NOR_Die_Layout_S		INTEL_W_T_64;
extern const NOR_Die_Layout_S		INTEL_W_T_128;
// W18/W30 series (Bottom Boot) 
extern const NOR_Die_Layout_S		INTEL_W_B_16;
extern const NOR_Die_Layout_S		INTEL_W_B_32;
extern const NOR_Die_Layout_S		INTEL_W_B_64;
extern const NOR_Die_Layout_S		INTEL_W_B_128;
// W18/W30 series (Dual Boot) 
extern const NOR_Die_Layout_S		INTEL_W_TB_32;
extern const NOR_Die_Layout_S		INTEL_W_TB_64;
extern const NOR_Die_Layout_S		INTEL_W_TB_128;

// M18 series (pure 128KW sectors) 
extern const NOR_Die_Layout_S		INTEL_M18_128;
extern const NOR_Die_Layout_S		INTEL_M18_256;
extern const NOR_Die_Layout_S		INTEL_M18_512;
extern const NOR_Die_Layout_S		INTEL_M18_1024;

//
//OTP layout
//
extern const NOR_OTP_Layout_S		INTEL_OTP_1PR;
extern const NOR_OTP_Layout_S		INTEL_OTP_17PR;

//------------------------------------------------------------------------------
// Check Device ID Callback Function                                            
//------------------------------------------------------------------------------
extern bool		INTEL_CheckDevID(volatile uint16 *die1_addr, volatile uint16 *die2_addr);
//------------------------------------------------------------------------------
// Check If Device Is Idle Callback Function                                    
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Erase Related Callback Function                                              
//------------------------------------------------------------------------------
extern void		INTEL_Erase_Block_CMD(const uint32 blockaddr);
extern STATUS_E	INTEL_Erase_CheckDone(const uint32 blockaddr);
//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
extern void		INTEL_PreProcess_32WORD_BUFPGM(void);
extern void		INTEL_Block_Unlock(const uint32 blockaddr);
extern void		INTEL_Word_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern STATUS_E	INTEL_Word_Program_CheckDone(const uint32 prog_addr);
extern void		INTEL_Buf_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
//------------------------------------------------------------------------------
// Sibley Related Callback Function                                             
//------------------------------------------------------------------------------
extern void		INTEL_Sibley_PreProcess_1KB_BUFPGM(void);
extern void		INTEL_Sibley_Word_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern STATUS_E INTEL_Sibley_Program_CheckDone(const uint32 prog_addr);
extern void		INTEL_Sibley_Buf_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
//------------------------------------------------------------------------------
// Protection Related Callback Function                                         
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// OTP Related Callback Function                                         
//------------------------------------------------------------------------------
extern void 		INTEL_OTP_ENTER(const uint32 blockaddr);
extern void 		INTEL_OTP_EXIT(const uint32 blockaddr);
extern void 		INTEL_OTP_Read(const uint32 blockaddr, const uint32 prog_addr, volatile uint16 * data);
extern void 		INTEL_OTP_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern void 		INTEL_OTP_Lock(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern STATUS_E 	INTEL_OTP_Lock_CheckStatus(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);

#ifdef __cplusplus
}
#endif

#endif
