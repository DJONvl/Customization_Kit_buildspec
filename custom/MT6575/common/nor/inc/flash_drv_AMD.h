/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _FLASH_DRV_AMD_H_
#define _FLASH_DRV_AMD_H_

#include "flash.h"

// AMD Command Address 
#define AMD_CMD_ADDR1						0x555
#define AMD_CMD_ADDR2						0x2AA
#define AMD_SA_ADDR							0x0

// AMD Unlock Cycles
#define AMD_UNLOCK_CYCLES_DATA1				0xAA
#define AMD_UNLOCK_CYCLES_DATA2				0x55

// AMD Enter Auto-Select Mode 
#define AMD_ENTER_AUTOSELECT_MODE_CMD		0x90

// AMD Device ID Read Address 
#define AMD_MANUFACTURE_ID_ADDR				0x00
#define AMD_DEVICE_CODE_ADDR				0x01
#define AMD_EXT_DEVICE_CODE1_ADDR			0x0E
#define AMD_EXT_DEVICE_CODE2_ADDR			0x0F

// AMD Enter CFI 
#define AMD_ENTER_CFI_ADDR					0x55
#define AMD_ENTER_CFI_DATA					0x98

// AMD CFI Boot Sector Flag 
#define AMD_CFI_BOOT_SECTOR_FLAG_ADDR		0x4F
#define AMD_CFI_UNIFORM_DEV					0x0000
#define AMD_CFI_TOP_BOTTOM_WP_DEV			0x0001
#define AMD_CFI_BOTTOM_DEV					0x0002
#define AMD_CFI_TOP_DEV						0x0003
#define AMD_CFI_TOP_BOTTOM_DEV				0x0004
#define AMD_CFI_BANK_A_REGION_INFO_ADDR		0x58
#define	AMD_CFI_DIE_SIZE_FLAG_ADDR			0x27

// AMD Reset 
#define AMD_RESET_TO_READ_MODE_DATA1		0xF0

// AMD Sector Erase 
#define AMD_SECTOR_ERASE_SETUP_CMD			0x80
#define AMD_SECTOR_ERASE_CMD				0x30

// AMD Enter Unlock Bypass Mode 
#define AMD_ENTER_UNLOCK_BYPASS_CMD			0x20

// AMD Unlock Bypass Mode Program 
#define AMD_UNLOCK_BYPASS_PGM_DATA1			0xA0

// AMD Exit Unlock Bypass Mode 
#define AMD_EXIT_UNLOCK_BYPASS_DATA1		0x90
#define AMD_EXIT_UNLOCK_BYPASS_DATA2		0x00

// AMD Word Program 
#define AMD_WORD_PGM_SETUP_CMD				0xA0

// AMD Buffer Program
#define AMD_BUF_PGM_LOAD					0x25
#define AMD_BUF_PGM_CONFIRM					0x29

// AMD Polling Status Bits 
#define AMD_DQ7_POLL_BIT					0x0080
#define AMD_DQ6_TOGGLE_BIT					0x0040
#define AMD_DQ5_TIMEOUT_ERR_BIT				0x0020

// AMD Sector Protection Command Address 
#define AMD_PROTECT_WP_ADDR					0x02		// 00000010 
#define AMD_PROTECT_EP_ADDR					AMD_PROTECT_WP_ADDR
#define AMD_PROTECT_PL_ADDR					0x0A		// 00001010 
#define AMD_PROTECT_SL_ADDR					0x12		// 00010010 

// AMD Sector PPB Program 
#define AMD_PROTECT_PPB_PGM_DATA1			0xAA
#define AMD_PROTECT_PPB_PGM_DATA2			0x55
#define AMD_PROTECT_PPB_PGM_DATA3			0x60
#define AMD_PROTECT_PPB_PGM_DATA4			0x68	// <-- 100us timeout is required between DATA4 and DATA5 
#define AMD_PROTECT_PPB_PGM_DATA5			0x48

// AMD Sector PPB Status 
#define AMD_PROTECT_PPB_STATUS_DATA1		0xAA
#define AMD_PROTECT_PPB_STATUS_DATA2		0x55
#define AMD_PROTECT_PPB_STATUS_DATA3		0x60
#define AMD_PROTECT_PPB_STATUS_DATA4		0x48

// AMD All Sector PPBs Erase 
#define AMD_PROTECT_PPB_ERASE_DATA1			0xAA
#define AMD_PROTECT_PPB_ERASE_DATA2			0x55
#define AMD_PROTECT_PPB_ERASE_DATA3			0x60
#define AMD_PROTECT_PPB_ERASE_DATA4			0x60	// <-- 1.2ms timeout is required between DATA4 and DATA5 
#define AMD_PROTECT_PPB_ERASE_DATA5			0x40

// AMD Sector Protection Status Bits 
#define AMD_PROTECT_PPB_LOCK_BIT_SET		0x02
#define AMD_PROTECT_PPB_SET					0x01

// AMD Sector DYB Entry
#define AMD_PROTECT_DYB_ENTRY_DATA1			0xAA
#define AMD_PROTECT_DYB_ENTRY_DATA2			0x55
#define AMD_PROTECT_DYB_ENTRY_DATA3			0xE0

// AMD Sector DYB Clear
#define AMD_PROTECT_DYB_CLEAR_DATA1			0xA0
#define AMD_PROTECT_DYB_CLEAR_DATA2			0x01

// AMD Sector DYB EXIT 
#define AMD_PROTECT_DYB_EXIT_DATA1			0xAA
#define AMD_PROTECT_DYB_EXIT_DATA2			0x55
#define AMD_PROTECT_DYB_EXIT_DATA3			0x90
#define AMD_PROTECT_DYB_EXIT_DATA4			0x00

// AMD Sector Unlock for NS-J
#define AMD_SECTOR_LOCK_MODE				0x00
#define AMD_SECTOR_UNLOCK_MODE				0x40

#define AMD_SECTOR_UNLOCK_DATA1				0x60
#define AMD_SECTOR_UNLOCK_DATA2				0x60
#define AMD_SECTOR_UNLOCK_DATA3				0x60

//
// AMD MirrorBit V2 specific
//
#define AMD_READ_SR_CMD						0x70
#define AMD_CLR_SR_CMD						0x71
#define AMD_ENTER_ID_CFI_MODE_CMD			AMD_ENTER_AUTOSELECT_MODE_CMD

// Status Register
#define AMD_SR_DEVICE_READY_BIT				0x80
#define AMD_SR_ERASE_STATUS_BIT				0x20
#define AMD_SR_PROGRAM_STATUS_BIT			0x10


//
//OTP_Drivers
//

//AMD Enter Secured Silicon sector
#define AMD_OTP_SECSI_ENTER_DATA1			0xAA
#define AMD_OTP_SECSI_ENTER_DATA2			0x55
#define AMD_OTP_SECSI_ENTER_DATA3			0x88

//AMD EXIT Secured Silicon sector
#define AMD_OTP_SECSI_EXIT_DATA1			0xAA
#define AMD_OTP_SECSI_EXIT_DATA2			0x55
#define AMD_OTP_SECSI_EXIT_DATA3			0x90
#define AMD_OTP_SECSI_EXIT_DATA4			0x00

//AMD Enter Customer Secured Silicon sector Lock mode
#define AMD_OTP_LOCK_REG_ENTER_DATA1		0xAA
#define AMD_OTP_LOCK_REG_ENTER_DATA2		0x55
#define AMD_OTP_LOCK_REG_ENTER_DATA3		0x40

//AMD Lock Customer Secured Silicon sector
#define AMD_OTP_LOCK_REG_PROGRAM_DATA1		0xA0

//AMD Exit Customer Secured Silicon sector Lock mode
#define AMD_OTP_LOCK_REG_EXIT_DATA1			0x90
#define AMD_OTP_LOCK_REG_EXIT_DATA2			0x00

//AMD Check Lock Register status
#define AMD_OTP_LOCK_STATUS_ADDR			0x0003
#define AMD_DQ7_FACTORY_LOCK_BIT			0x0080
#define AMD_DQ6_CUSTOMER_LOCK_BIT			0x0040


#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
extern const NOR_CMD_Callback_S		AMD_CMD_CB_UNLOCK_BYPASS_PGM;
extern const NOR_CMD_Callback_S		AMD_CMD_CB_MIRRORBIT_BUF_PGM;
extern const NOR_CMD_Callback_S		AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM;
extern const NOR_CMD_Callback_S		AMD_CMD_CB_S29WS_MIRRORBIT_BUF_PGM;
extern const NOR_CMD_Callback_S		AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM;
extern const NOR_CMD_Callback_S		AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM;
extern const NOR_CMD_Callback_S		AMD_CMD_CB_WORD_PGM;
extern const NOR_OTP_CMD_Callback_S 	AMD_OTP_CMD_CB;

//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------
extern const NOR_Die_Layout_S		AMD_32;
extern const NOR_Die_Layout_S		AMD_64;
extern const NOR_Die_Layout_S		AMD_128;
extern const NOR_Die_Layout_S		AMD_256;
extern const NOR_Die_Layout_S		AMD_512;
extern const NOR_Die_Layout_S		AMD_1024;
// Top Boot 
extern const NOR_Die_Layout_S		AMD_T_16;
extern const NOR_Die_Layout_S		AMD_T_32;
extern const NOR_Die_Layout_S		AMD_T_64;
extern const NOR_Die_Layout_S		AMD_T_128;
extern const NOR_Die_Layout_S		AMD_T2_128;
extern const NOR_Die_Layout_S		AMD_T_256;
extern const NOR_Die_Layout_S		AMD_T2_256;
// Bottom Boot 
extern const NOR_Die_Layout_S		AMD_B_16;
extern const NOR_Die_Layout_S		AMD_B_32;
extern const NOR_Die_Layout_S		AMD_B_64;
extern const NOR_Die_Layout_S		AMD_B_128;
extern const NOR_Die_Layout_S		AMD_B2_128;
extern const NOR_Die_Layout_S		AMD_B_256;
extern const NOR_Die_Layout_S		AMD_B2_256;
// Top/Bottom Boot 
extern const NOR_Die_Layout_S		AMD_TB_32;
extern const NOR_Die_Layout_S		AMD_TB_64;
extern const NOR_Die_Layout_S		AMD_TB_128;
//
//OTP layout
//
extern const NOR_OTP_Layout_S		SPANSION_OTP_8W_SECSI;
extern const NOR_OTP_Layout_S		SPANSION_OTP_16W_SECSI;
extern const NOR_OTP_Layout_S		SPANSION_OTP_128W_SECSI;

//------------------------------------------------------------------------------
// MirrorBit Memory Sector Layout Set                                           
//------------------------------------------------------------------------------

// MirrorBit Bottom Boot 
extern const NOR_Die_Layout_S		AMD_MB_B_64;
extern const NOR_Die_Layout_S		AMD_MB_B_128;
extern const NOR_Die_Layout_S		AMD_MB_B_256;
extern const NOR_Die_Layout_S		AMD_MB_B_512;
extern const NOR_Die_Layout_S		AMD_MB_B_1024;
// MirrorBit Top Boot 
extern const NOR_Die_Layout_S		AMD_MB_T_16;
extern const NOR_Die_Layout_S		AMD_MB_T_32;
extern const NOR_Die_Layout_S		AMD_MB_T_64;
extern const NOR_Die_Layout_S		AMD_MB_T_128;
extern const NOR_Die_Layout_S		AMD_MB_T2_64;
extern const NOR_Die_Layout_S		AMD_MB_T2_128;
extern const NOR_Die_Layout_S		AMD_MB_T2_256;
extern const NOR_Die_Layout_S		AMD_MB_T_512;
extern const NOR_Die_Layout_S		AMD_MB_T_1024;
// MirrorBit Top/Bottom Boot 
extern const NOR_Die_Layout_S		AMD_MB_TB_128;
extern const NOR_Die_Layout_S		AMD_MB_TB2_128;
extern const NOR_Die_Layout_S		AMD_MB_TB_256;
extern const NOR_Die_Layout_S		AMD_MB_TB2_256;
extern const NOR_Die_Layout_S		AMD_MB_TB_512;

//------------------------------------------------------------------------------
// Check Device ID Callback Function                                            
//------------------------------------------------------------------------------
extern bool		AMD_CheckDevID(volatile uint16 *die1_addr, volatile uint16 *die2_addr);
//------------------------------------------------------------------------------
// Check If Device Is Idle Callback Function                                    
//------------------------------------------------------------------------------
extern bool		AMD_CheckDevIdle(const uint32 addr);
//------------------------------------------------------------------------------
// Erase Related Callback Function                                              
//------------------------------------------------------------------------------
extern void		AMD_Erase_CMD(const uint32 blockaddr);
extern STATUS_E	AMD_Erase_CheckDone_By_Polling(const uint32 blockaddr);
//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
extern void		AMD_PreProcess(void);
extern void		AMD_S29GL_MirrorBit_PreProcess(void);
extern void		AMD_UnlockBypass_Enter_CMD(const uint32 blockaddr);
extern void		AMD_UnlockBypass_Exit_CMD(const uint32 blockaddr);
extern void		AMD_UnlockBypass_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern void		AMD_Word_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern STATUS_E	AMD_Program_CheckDone_By_Toggle(const uint32 prog_addr);
//------------------------------------------------------------------------------
// MirrorBit Related Callback Function                                          
//------------------------------------------------------------------------------
extern void 	AMD_MirrorBit_Erase_CMD(const uint32 blockaddr);
extern void 	AMD_S29N_MirrorBit_Erase_CMD(const uint32 blockaddr);
extern void 	AMD_MirrorBit_PreProcess(void);
extern void 	AMD_MirrorBit_Buf_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
extern void 	AMD_S71WS_MirrorBit_Buf_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
//------------------------------------------------------------------------------
// MirrorBit V2 Related Callback Function
//------------------------------------------------------------------------------
extern bool		AMD_MirrorBit_V2_CheckDevID(volatile uint16 *die1_addr, volatile uint16 *die2_addr);
extern void		AMD_MirrorBit_V2_Erase_CMD(const uint32 blockaddr);
extern STATUS_E	AMD_MirrorBit_V2_Erase_CheckDone(const uint32 blockaddr);
extern void		AMD_MirrorBit_V2_Word_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern void 	AMD_MirrorBit_V2_Buf_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word);
extern STATUS_E	AMD_MirrorBit_V2_Program_CheckDone(const uint32 prog_addr);
//------------------------------------------------------------------------------
// Protection Related Callback Function                                         
//------------------------------------------------------------------------------
#ifdef AMD_PROTECTION_ENABLED
extern void		AMD_UnprotectChip();
extern bool		AMD_Protection_PPB_Program_CMD(uint32 blockaddr);
extern bool		AMD_Protection_PPB_Status_CMD(uint32 blockaddr);
extern bool		AMD_Protection_Erase_All_PPBs_CMD(uint32 blockaddr);
#endif

extern void		AMD_Protection_DYB_Clear_CMD(uint32 blockaddr);

//------------------------------------------------------------------------------
// OTP Related Callback Function                                         
//------------------------------------------------------------------------------
extern void 		AMD_OTP_ENTER(const uint32 blockaddr);
extern void 		AMD_OTP_EXIT(const uint32 blockaddr);
extern void 		AMD_OTP_Read(const uint32 blockaddr, const uint32 prog_addr, volatile uint16 * data);
extern void 		AMD_OTP_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern void 		AMD_OTP_Lock(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern STATUS_E 	AMD_OTP_Lock_CheckStatus(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);

#ifdef __cplusplus
}
#endif

#endif
