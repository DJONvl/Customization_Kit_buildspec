/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _FLASH_DRV_RENESAS_H_
#define _FLASH_DRV_RENESAS_H_

#include "flash_drv_INTEL.h"

// READ CMD 

// PROGRAM CMD 
#define RENESAS_CMD_PAGE_PGM				0x41
#define RENESAS_CMD_DATA_LOAD_TO_PAGE_BUF	0x74
#define RENESAS_CMD_PAGE_BUF_TO_FLASH		0x0E
#define RENESAS_CMD_PAGE_BUF_TO_FLASH_CNF	0xD0

// ERASE 

// LOCK/UNLOCK 
#define RENESAS_CMD_SWLOCK_RELEASE_CMD_1	0x60
#define RENESAS_CMD_SWLOCK_RELEASE_CMD_2	0xAC
#define RENESAS_CMD_SWLOCK_RELEASE_CMD_3	0x7B

// Status Register Bit 
#define RENESAS_SR7_DWS_BIT				0x0080
#define RENESAS_SR6_SS_BIT				0x0040
#define RENESAS_SR5_ES_BIT				0x0020
#define RENESAS_SR4_PS_BIT				0x0010
#define RENESAS_SR3_BSAE_BIT			0x0008

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
extern const NOR_CMD_Callback_S		RENESAS_CMD_CB_WORD_PGM;
extern const NOR_CMD_Callback_S		RENESAS_CMD_CB_128WORD_PAGE_PGM;
extern const NOR_CMD_Callback_S		RENESAS_CMD_CB_WORD_PGM_64Mb;
extern const NOR_CMD_Callback_S		RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb;
	
//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Check Device ID Callback Function                                            
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Check If Device Is Idle Callback Function                                    
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Erase Related Callback Function                                              
//------------------------------------------------------------------------------
extern void		RENESAS_Erase_Block_CMD(const uint32 blockaddr);
extern void		RENESAS_Erase_Block_CMD_64Mb(const uint32 blockaddr);
extern STATUS_E	RENESAS_CheckDone(const uint32 blockaddr);
//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
extern void		RENESAS_PreProcess_128WORD_BUFPGM(void);
extern void		RENESAS_Word_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern void 	RENESAS_Word_Program_64Mb(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern void		RENESAS_FixedPage_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
extern void 	RENESAS_FixedPage_Program_64Mb(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
#if 0
extern void		RENESAS_DynamicBuf_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
extern void 	RENESAS_DynamicBuf_Program_64Mb(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
#endif
//------------------------------------------------------------------------------
// Protection Related Callback Function                                         
//------------------------------------------------------------------------------
extern void		RENESAS_SoftwareLockRelease_Preprocess(const uint32 blockaddr);
extern void 	RENESAS_SoftwareLockRelease_Preprocess_64Mb(const uint32 blockaddr);

#ifdef __cplusplus
}
#endif

#endif
