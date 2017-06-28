/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#include <string.h>
#include "flash_drv_AMD.h"
#include "hw_config.h"

#if defined(FEATURE_NOR_SUPPORT)
//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
const NOR_CMD_Callback_S	AMD_CMD_CB_UNLOCK_BYPASS_PGM = {
	ID_AMD_CMD_CB_UNLOCK_BYPASS_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	AMD_PreProcess,
	DUMMY_Program_PostProcess,
	AMD_UnlockBypass_Enter_CMD,
	AMD_UnlockBypass_Exit_CMD,
	AMD_UnlockBypass_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	NULL,
	NULL
};

const NOR_CMD_Callback_S	AMD_CMD_CB_MIRRORBIT_BUF_PGM = {
	ID_AMD_CMD_CB_MIRRORBIT_BUF_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_MirrorBit_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	AMD_MirrorBit_PreProcess, 
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	AMD_Word_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	AMD_MirrorBit_Buf_Program_CMD,
	AMD_Program_CheckDone_By_Toggle
};

const NOR_CMD_Callback_S AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM = {
	ID_AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM,
	AMD_MirrorBit_V2_CheckDevID,
	DUMMY_CheckDevIdle,
	AMD_MirrorBit_V2_Erase_CMD,
	AMD_MirrorBit_V2_Erase_CheckDone,
	AMD_MirrorBit_PreProcess, 
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	AMD_MirrorBit_V2_Word_Program_CMD,
	AMD_MirrorBit_V2_Program_CheckDone,
	AMD_MirrorBit_V2_Buf_Program_CMD,
	AMD_MirrorBit_V2_Program_CheckDone
};

const NOR_CMD_Callback_S	AMD_CMD_CB_S29WS_MIRRORBIT_BUF_PGM = {
	ID_AMD_CMD_CB_S29WS_MIRRORBIT_BUF_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_MirrorBit_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	AMD_MirrorBit_PreProcess, 
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	AMD_Word_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	AMD_S71WS_MirrorBit_Buf_Program_CMD,
	AMD_Program_CheckDone_By_Toggle
};

const NOR_CMD_Callback_S	AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM = {
	ID_AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_S29N_MirrorBit_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	AMD_MirrorBit_PreProcess, 
	DUMMY_Program_PostProcess,
	AMD_Protection_DYB_Clear_CMD,
	DUMMY_Program_Exit,
	AMD_Word_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	AMD_MirrorBit_Buf_Program_CMD,
	AMD_Program_CheckDone_By_Toggle
};

const NOR_CMD_Callback_S	AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM = {
	ID_AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_MirrorBit_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	AMD_S29GL_MirrorBit_PreProcess,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	AMD_Word_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	AMD_MirrorBit_Buf_Program_CMD,
	AMD_Program_CheckDone_By_Toggle
};

const NOR_CMD_Callback_S	AMD_CMD_CB_WORD_PGM = {
	ID_AMD_CMD_CB_WORD_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	AMD_PreProcess,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	AMD_Word_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	NULL,
	NULL
};

//
//OTP function set
//
const NOR_OTP_CMD_Callback_S AMD_OTP_CMD_CB = {
	AMD_OTP_ENTER,
	AMD_OTP_Read,
	AMD_OTP_Program,
	AMD_OTP_Lock,
	AMD_OTP_Lock_CheckStatus,
	AMD_OTP_EXIT
};

//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------
const NOR_Die_Layout_S		AMD_32 = {

	0x00400000,

	{
		 { 0x00000000,	64,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_64 = {

	0x00800000,

	{
		 { 0x00000000,	128,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_128 = {

	0x01000000,

	{
		 { 0x00000000,	128,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_256 = {

	0x02000000,

	{
		 { 0x00000000,	256,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_512 = {

	0x04000000,

	{
		 { 0x00000000,	512,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_1024 = {

	0x08000000,

	{
		 { 0x00000000,	1024,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// Top Boot 
const NOR_Die_Layout_S		AMD_T_16 = {
	
	0x00200000,

	{
		 { 0x00000000,	31,		0x10000 }
		,{ 0x001F0000,	1,		0x8000 }
		,{ 0x001F8000,	2,		0x2000 }
		,{ 0x001FC000,	1,		0x4000 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_T_32 = {
	
	0x00400000,

	{
		 { 0x00000000,	63,		0x10000 }
		,{ 0x003F0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_T_64 = {

	0x00800000,

	{
		 { 0x00000000,	127,	0x10000 }
		,{ 0x007F0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_T_128 = {

	0x01000000,

	{
		 { 0x00000000,	255,	0x10000 }
		,{ 0x00FF0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_T2_128 = {

	0x01000000,

	{
		 { 0x00000000,	127,	0x20000 }
		,{ 0x00FE0000,	8,		0x4000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_T_256 = {

	0x02000000,

	{
		 { 0x00000000,	511,	0x10000 }
		,{ 0x01FF0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_T2_256 = {

	0x02000000,

	{
		 { 0x00000000,	255,	0x20000 }
		,{ 0x01FE0000,	8,		0x4000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// Bottom Boot 
const NOR_Die_Layout_S		AMD_B_16 = {
	
	0x00200000,

	{
		 { 0x00000000,	1,		0x4000 }
		,{ 0x00004000,	2,		0x2000 }
		,{ 0x00008000,	1,		0x8000 }
		,{ 0x00010000,	31,		0x10000 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_B_32 = {

	0x00400000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	63,		0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_B_64 = {

	0x00800000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	127,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_B_128 = {

	0x01000000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	255,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_B2_128 = {

	0x01000000,

	{
		 { 0x00000000, 8,	0x4000 }
		,{ 0x00020000, 127, 0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_B_256 = {

	0x02000000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	511,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_B2_256 = {

	0x02000000,

	{
		 { 0x00000000,	8,	0x4000 }
		,{ 0x00020000,	255,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// Top/Bottom Boot 
const NOR_Die_Layout_S		AMD_TB_32 = {

	0x00400000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	62,		0x10000 }
		,{ 0x003F0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_TB_64 = {

	0x00800000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	126,	0x10000 }
		,{ 0x007F0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_TB_128 = {

	0x01000000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	254,	0x10000 }
		,{ 0x00FF0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

//------------------------------------------------------------------------------
// MirrorBit Memory Sector Layout Set                                           
//------------------------------------------------------------------------------

// MirrorBit Bottom Boot 
const NOR_Die_Layout_S		AMD_MB_B_64 = {

	0x00800000,

	{
		 { 0x00000000,	4,		0x10000 }
		,{ 0x00040000,	31,		0x40000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_B_128 = {

	0x01000000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	127,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_B_256 = {

	0x02000000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	255,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_B_512 = {

	0x04000000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	511,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_B_1024 = {

	0x08000000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	1023,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// MirrorBit Top Boot 
const NOR_Die_Layout_S		AMD_MB_T_16 = {
	
	0x00200000,

	{
		 { 0x00000000,	31,		0x10000 }
		,{ 0x001F0000,	4,		0x4000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_T_32 = {
	
	0x00400000,

	{
		 { 0x00000000,	63,		0x10000 }
		,{ 0x003F0000,	4,		0x4000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_T_64 = {

	0x00800000,

	{
		 { 0x00000000,	127,	0x10000 }
		,{ 0x007F0000,	4,		0x4000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_T2_64 = {

	0x00800000,

	{
		 { 0x00000000,	31,		0x40000 }
		,{ 0x007C0000,	4,		0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_T_128 = {

	0x01000000,

	{
		 { 0x00000000,	255,	0x10000 }
		,{ 0x00FF0000,	4,		0x4000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_T2_128 = {

	0x01000000,

	{
		 { 0x00000000,	127,	0x20000 }
		,{ 0x00FE0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_T2_256 = {

	0x02000000,

	{
		 { 0x00000000,	255,	0x20000 }
		,{ 0x01FE0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_T_512 = {

	0x04000000,

	{
		 { 0x00000000,	511,	0x20000 }
		,{ 0x03FE0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_T_1024 = {

	0x08000000,

	{
		 { 0x00000000,	1023,	0x20000 }
		,{ 0x07FE0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// MirrorBit Top/Bottom Boot 
const NOR_Die_Layout_S		AMD_MB_TB_128 = {

	0x01000000,

	{
		 { 0x00000000,	4,		0x10000 }
		,{ 0x00040000,	62,		0x40000 }
		,{ 0x00FC0000,	4,		0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_TB2_128 = {

	0x01000000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	126,	0x20000}
		,{ 0x00FE0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_TB_256 = {

	0x02000000,

	{
		 { 0x00000000,	4,		0x10000 }
		,{ 0x00040000,	126,    0x40000 }
		,{ 0x01FC0000,	4,		0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_TB2_256 = {

	0x02000000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	254,	0x20000}
		,{ 0x01FE0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		AMD_MB_TB_512 = {

	0x04000000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	510, 	0x20000}
		,{ 0x03FE0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

//
//OTP layout
//
const NOR_OTP_Layout_S		SPANSION_OTP_8W_SECSI ={
	0x010,
		
	{
		 { 0x010,		1,	0x10}
		,{ 0, 0, 0}
	}
};

const NOR_OTP_Layout_S		SPANSION_OTP_64W_SECSI ={
	0x080,
		
	{
		 { 0x080,		1,	0x080}
		 ,{ 0, 0, 0}
	}
};

const NOR_OTP_Layout_S		SPANSION_OTP_128W_SECSI ={
	0x100,
		
	{
		 { 0x100,		1,	0x100}
		,{ 0, 0, 0}
	}
};

//------------------------------------------------------------------------------
// Check Device ID Callback Function                                            
//------------------------------------------------------------------------------
bool AMD_CheckDevID(volatile uint16 *die1_addr, volatile uint16 *die2_addr) {
	//                                                                                     
	// Some 128Mb flash devices were assembled by two the same 64Mb flash devices.         
	// The worse thing is that the 128Mb flash share the same manufacture id and device    
	// code with 64Mb flash !!!                                                            
	//                                                                                     
	// For example:                                                                        
	//    AM50DL128CG(128Mb) was assembled by two AM29DL640D(64Mb) devices.                
	//    S71JL128HXX(128Mb) was assembled by two S29JL064HXX(64Mb) devices.               
	//    ST_M74DW66500B(128Mb) was assembled by two ST_M29DW640D(64Mb) devices.           
	//                                                                                     
	// This function has ability to tell from those flashes that share the same id.        
	//                                                                                     
	uint16	die1_manufacture_code;
	uint16	die1_dev_code;
	uint16	die1_ext_dev_code1;
	uint16	die1_ext_dev_code2;
	uint16	die2_manufacture_code;
	uint16	die2_dev_code;
	uint16	die2_ext_dev_code1;
	uint16	die2_ext_dev_code2;	
	uint16	die_count = 0;
	uint16	ADDR_LEFT_SHIFT_BIT = 0;
	uint16	boot_sector_flag;
	uint32	die_size;
	uint16	cfi_bank_a_region_info;
	NOR_DeviceID_E	flash_enum_id=g_FlashDev.m_device_id;

	// check if AM29PDL128G 
	if( NOR_AM29PDL128G == flash_enum_id ) {
		ADDR_LEFT_SHIFT_BIT = 1;
	}

	// reset die1 to read mode 
	die1_addr[AMD_SA_ADDR<<ADDR_LEFT_SHIFT_BIT] = AMD_RESET_TO_READ_MODE_DATA1;

	// identify AM49DL3208GT or S29PL032J by reading CFI boot sector flag, because these two flashes has the same ID 
	if(	(NOR_AM49DL3208GT == flash_enum_id) ||
		(NOR_S29PL032J == flash_enum_id) 
	) {
		// enter CFI mode 
		die1_addr[AMD_ENTER_CFI_ADDR<<ADDR_LEFT_SHIFT_BIT] = AMD_ENTER_CFI_DATA;
		// read Top/Bottom Boot Sector Flag 
		boot_sector_flag = die1_addr[AMD_CFI_BOOT_SECTOR_FLAG_ADDR<<ADDR_LEFT_SHIFT_BIT];
		// reset die1 to read mode 
		die1_addr[AMD_SA_ADDR<<ADDR_LEFT_SHIFT_BIT] = AMD_RESET_TO_READ_MODE_DATA1;
		// check if boot sector is matched 
		if( ((&AMD_T_32)==FLASH_DIE_LAYOUT(0) && AMD_CFI_TOP_DEV!=boot_sector_flag) ||
			((&AMD_B_32)==FLASH_DIE_LAYOUT(0) && AMD_CFI_BOTTOM_DEV!=boot_sector_flag) 
		) {
			return FALSE;
		}
	}

	// identify NOR_TV00578002DABD or NOR_TV00560002DDGB by reading CFI size flag, because these two flashes has the same ID 
	if(	(NOR_TV00578002DABD == flash_enum_id) ||
		(NOR_TV00560002DDGB == flash_enum_id) 
	) {
		// enter CFI mode 
		die1_addr[AMD_ENTER_CFI_ADDR<<ADDR_LEFT_SHIFT_BIT] = AMD_ENTER_CFI_DATA;
		// read die_size Flag 
		die_size = die1_addr[AMD_CFI_DIE_SIZE_FLAG_ADDR];// from spec. 0x27 mean size
		// reset die1 to read mode 
		die1_addr[AMD_SA_ADDR<<ADDR_LEFT_SHIFT_BIT] = AMD_RESET_TO_READ_MODE_DATA1;
		// check if size is matched , 0x0017 means 64Mbits==2**23(0x0017)
		if( ((&AMD_T_64)==FLASH_DIE_LAYOUT(0) && 0x0017!=die_size) ||	// for NOR_TV00560002DDGB
			((&AMD_T_128)==FLASH_DIE_LAYOUT(0) && 0x0018!=die_size) 	// for NOR_TV00578002DABD
		) {
			return FALSE;
		}
	}

	// identify S71PLXXXJ and S71PLXXXN(MirrorBit) by reading CFI Bank A region info, because those devices have the same ID 
	switch(flash_enum_id) {
	case NOR_S71PL127J:
	case NOR_S71PL254J:
	case NOR_AM49PDL129BH:
	case NOR_S71PL127N:
	case NOR_S71PL129N:
		// enter CFI mode 
		die1_addr[AMD_ENTER_CFI_ADDR<<ADDR_LEFT_SHIFT_BIT] = AMD_ENTER_CFI_DATA;
		// read bank A region info 
		cfi_bank_a_region_info = die1_addr[AMD_CFI_BANK_A_REGION_INFO_ADDR<<ADDR_LEFT_SHIFT_BIT];
		// reset die1 to read mode 
		die1_addr[AMD_SA_ADDR<<ADDR_LEFT_SHIFT_BIT] = AMD_RESET_TO_READ_MODE_DATA1;
		// check if region info match 
		if( ( (NOR_S71PL127N==flash_enum_id || NOR_S71PL129N==flash_enum_id) && 0x000B!=cfi_bank_a_region_info) ||
			(!(NOR_S71PL127N==flash_enum_id || NOR_S71PL129N==flash_enum_id) && 0x000B==cfi_bank_a_region_info)
		) {
			return FALSE;
		}
		break;
	default:
		break;
	}

	// enter auto-select mode on die1 
	die1_addr[AMD_CMD_ADDR1<<ADDR_LEFT_SHIFT_BIT] = AMD_UNLOCK_CYCLES_DATA1;
	die1_addr[AMD_CMD_ADDR2<<ADDR_LEFT_SHIFT_BIT] = AMD_UNLOCK_CYCLES_DATA2;
	die1_addr[AMD_CMD_ADDR1<<ADDR_LEFT_SHIFT_BIT] = AMD_ENTER_AUTOSELECT_MODE_CMD;

	// read manufacture id and device code from die1 
	die1_manufacture_code = (die1_addr[AMD_MANUFACTURE_ID_ADDR<<ADDR_LEFT_SHIFT_BIT]&0x00FF);
	die1_dev_code = die1_addr[AMD_DEVICE_CODE_ADDR<<ADDR_LEFT_SHIFT_BIT];
	die1_ext_dev_code1 = die1_addr[AMD_EXT_DEVICE_CODE1_ADDR<<ADDR_LEFT_SHIFT_BIT];
	die1_ext_dev_code2 = die1_addr[AMD_EXT_DEVICE_CODE2_ADDR<<ADDR_LEFT_SHIFT_BIT];

	// compare id 
	if( g_FlashDev.m_hw_info.m_manufacture_code != die1_manufacture_code )
		goto not_match;

	// if manufacture code exist, temporarily record flash id to g_HW_DetectionResult 
	g_HW_DetectionResult.m_nor_flash_dev_code_1 = die1_manufacture_code;
	g_HW_DetectionResult.m_nor_flash_dev_code_2 = die1_dev_code;
	g_HW_DetectionResult.m_nor_flash_dev_code_3 = die1_ext_dev_code1;
	g_HW_DetectionResult.m_nor_flash_dev_code_4 = die1_ext_dev_code2;

	if( g_FlashDev.m_hw_info.m_dev_code != die1_dev_code )
		goto not_match;
	if( 0!=g_FlashDev.m_hw_info.m_ext_dev_code1 && (g_FlashDev.m_hw_info.m_ext_dev_code1!=die1_ext_dev_code1) )
		goto not_match;
	if( 0!=g_FlashDev.m_hw_info.m_ext_dev_code2 && (g_FlashDev.m_hw_info.m_ext_dev_code2!=die1_ext_dev_code2) )
		goto not_match;

	// die1 id matched, continue to check if this flash is dual stack 
	die2_manufacture_code = (die2_addr[AMD_MANUFACTURE_ID_ADDR<<ADDR_LEFT_SHIFT_BIT]&0x00FF);
	die2_dev_code = die2_addr[AMD_DEVICE_CODE_ADDR<<ADDR_LEFT_SHIFT_BIT];
	die2_ext_dev_code1 = die2_addr[AMD_EXT_DEVICE_CODE1_ADDR<<ADDR_LEFT_SHIFT_BIT];
	die2_ext_dev_code2 = die2_addr[AMD_EXT_DEVICE_CODE2_ADDR<<ADDR_LEFT_SHIFT_BIT];	
	
	if( (die1_manufacture_code == die2_manufacture_code)
	&& (die1_dev_code == die2_dev_code)
	&& (die1_ext_dev_code1 == die2_ext_dev_code1)
	&& (die1_ext_dev_code2 == die2_ext_dev_code2)
	) {
		die_count = 1;
	}
	else {
		die_count = 2;
	}

	// reset die1 to read mode 
	die1_addr[AMD_SA_ADDR<<ADDR_LEFT_SHIFT_BIT] = AMD_RESET_TO_READ_MODE_DATA1;

	// if die count matched, return true 
	if( die_count == FLASH_DIE_COUNT() ) {
		return TRUE;
	}

not_match:

	// reset die1 to read mode 
	die1_addr[AMD_SA_ADDR<<ADDR_LEFT_SHIFT_BIT] = AMD_RESET_TO_READ_MODE_DATA1;

	return FALSE;
}

//------------------------------------------------------------------------------
// Check If Device Is Idle Callback Function                                    
//------------------------------------------------------------------------------
bool AMD_CheckDevIdle(const uint32 addr)
{
	volatile uint16 stat_data1;
	volatile uint16 stat_data2;
	
	stat_data1 = *(volatile uint16*)addr;
	stat_data2 = *(volatile uint16*)addr;
	
	if( (stat_data1&AMD_DQ6_TOGGLE_BIT) == (stat_data2&AMD_DQ6_TOGGLE_BIT) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//------------------------------------------------------------------------------
// Erase Related Callback Function                                              
//------------------------------------------------------------------------------

void AMD_Erase_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;

	if( NOR_S29NS016J == g_FlashDev.m_device_id ||
		NOR_S29NS032J == g_FlashDev.m_device_id ||
		NOR_S29NS064J == g_FlashDev.m_device_id ||
		NOR_S29NS128J == g_FlashDev.m_device_id 
	){
		ba[AMD_SECTOR_UNLOCK_MODE] = AMD_SECTOR_UNLOCK_DATA1;
		ba[AMD_SECTOR_UNLOCK_MODE] = AMD_SECTOR_UNLOCK_DATA2;
		ba[AMD_SECTOR_UNLOCK_MODE] = AMD_SECTOR_UNLOCK_DATA3;
		ba[AMD_SECTOR_UNLOCK_MODE] = AMD_RESET_TO_READ_MODE_DATA1;
	}
	
	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_SECTOR_ERASE_SETUP_CMD;
	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_SA_ADDR] = AMD_SECTOR_ERASE_CMD;
}

STATUS_E AMD_Erase_CheckDone_By_Polling(const uint32 blockaddr)
{
	// use POLL_BIT instead of TOGGLE_BIT to wait for erase complete, 
	// because in some device using TOGGLE_BIT to wait for erase complete will fail. 
	volatile uint16 status;

	status = *(volatile uint16 *)blockaddr;
	if( (status&AMD_DQ7_POLL_BIT) ) {
		return S_DONE;
	}
	else if( (status&AMD_DQ5_TIMEOUT_ERR_BIT) ) {
		status = *(volatile uint16 *)blockaddr;
		if( (status&AMD_DQ7_POLL_BIT) ) {
			return S_DONE;
		}
		else {
			return S_DA_TIMEOUT;
		}
	}
	else {
		return S_DA_IN_PROGRESS;
	}
}

//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
void AMD_PreProcess(void) {
#ifdef AMD_PROTECTION_ENABLED
	AMD_UnprotectChip();
#endif
}

void AMD_UnlockBypass_Enter_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;

	if( NOR_S29NS016J == g_FlashDev.m_device_id ||
		NOR_S29NS032J == g_FlashDev.m_device_id ||
		NOR_S29NS064J == g_FlashDev.m_device_id ||
		NOR_S29NS128J == g_FlashDev.m_device_id 
	){
		ba[AMD_SECTOR_UNLOCK_MODE] = AMD_SECTOR_UNLOCK_DATA1;
		ba[AMD_SECTOR_UNLOCK_MODE] = AMD_SECTOR_UNLOCK_DATA2;
		ba[AMD_SECTOR_UNLOCK_MODE] = AMD_SECTOR_UNLOCK_DATA3;
		ba[AMD_SECTOR_UNLOCK_MODE] = AMD_RESET_TO_READ_MODE_DATA1;
	}
		
	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_ENTER_UNLOCK_BYPASS_CMD;
}

void AMD_UnlockBypass_Exit_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	ba[AMD_CMD_ADDR1] = AMD_EXIT_UNLOCK_BYPASS_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_EXIT_UNLOCK_BYPASS_DATA2;
}
#pragma arm section code = "MT6223_CRITCAL_CODE"
void AMD_UnlockBypass_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_BYPASS_PGM_DATA1;
	*(volatile uint16*)prog_addr = data;
}

void AMD_Word_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_WORD_PGM_SETUP_CMD;
	*(volatile uint16*)prog_addr = data;
}
#pragma arm section
STATUS_E AMD_Program_CheckDone_By_Toggle(const uint32 prog_addr)
{
	volatile uint16 stat_data1;
	volatile uint16 stat_data2;
	
	stat_data1 = *(volatile uint16*)prog_addr;
	stat_data2 = *(volatile uint16*)prog_addr;

	if( (stat_data1&AMD_DQ6_TOGGLE_BIT) == (stat_data2&AMD_DQ6_TOGGLE_BIT) )
	{
		return S_DONE;
	}
	
	if( (stat_data1&AMD_DQ5_TIMEOUT_ERR_BIT) )
	{
		stat_data1 = *(volatile uint16*)prog_addr;
		stat_data2 = *(volatile uint16*)prog_addr;
		if( (stat_data1&AMD_DQ6_TOGGLE_BIT) == (stat_data2&AMD_DQ6_TOGGLE_BIT) )
		{
			return S_DONE;
		}
		else
		{
			return S_DA_NOR_PROGRAM_FAILED;
		}
	}

	return S_DA_IN_PROGRESS;
}

//------------------------------------------------------------------------------
// MirrorBit Related Callback Function                                          
//------------------------------------------------------------------------------
void AMD_MirrorBit_Erase_CMD(const uint32 blockaddr)
{
	volatile uint32 i;

	AMD_Erase_CMD(blockaddr);

	//final step, wait for 10us 
	for (i=0;i<1000;i++);
}

void AMD_S29N_MirrorBit_Erase_CMD(const uint32 blockaddr)
{
	AMD_Protection_DYB_Clear_CMD(blockaddr);
	AMD_MirrorBit_Erase_CMD(blockaddr);
}

void AMD_MirrorBit_PreProcess(void)
{
	g_FLASH_MAX_BUFPGM_SIZE_IN_BYTE = 64;
}

void AMD_S29GL_MirrorBit_PreProcess(void)
{
	g_FLASH_MAX_BUFPGM_SIZE_IN_BYTE = 32;
}
#pragma arm section code = "MT6223_CRITCAL_CODE"
void AMD_MirrorBit_Buf_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	volatile uint32 i;
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 *prog_word_addr = (volatile uint16*)prog_addr;
	volatile uint16 *data_word_addr = (volatile uint16*)data;

	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[0] = AMD_BUF_PGM_LOAD;
	ba[0] = (uint16)(length_in_word - 1); 

	for(i=0;i<length_in_word;i++)
	{
		*prog_word_addr++ = *data_word_addr++;
	}

	ba[0] = AMD_BUF_PGM_CONFIRM;

	//final step, wait for 10us 
	for (i=0;i<1000;i++);
}

void AMD_S71WS_MirrorBit_Buf_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	volatile uint32 i;
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 *prog_word_addr = (volatile uint16*)prog_addr;
	volatile uint16 *data_word_addr = (volatile uint16*)data;

	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	
	*prog_word_addr=AMD_BUF_PGM_LOAD;
	*prog_word_addr=(uint16)(length_in_word - 1);

	for(i=0;i<length_in_word;i++)
	{
		*prog_word_addr++ = *data_word_addr++;
	}

	ba[0] = AMD_BUF_PGM_CONFIRM;

	//final step, wait for 10us 
	for (i=0;i<1000;i++);
}
#pragma arm section

//------------------------------------------------------------------------------
// MirrorBit V2 Related Callback Function
//------------------------------------------------------------------------------
static bool CheckBootSectorFlag(NOR_DeviceID_E devID, uint16 boot_sector_flag)
{
    switch (devID)
    {
        case NOR_S29WS128R_T:
        case NOR_S29WS256R_T:
        case NOR_S29WS512R_T:
        case NOR_S29WS01GR_T:
            return (boot_sector_flag == AMD_CFI_TOP_DEV);

        case NOR_S29WS128R_B:
        case NOR_S29WS256R_B:
        case NOR_S29WS512R_B:
        case NOR_S29WS01GR_B:
            return (boot_sector_flag == AMD_CFI_BOTTOM_DEV);

        case NOR_S29WS128R:
        case NOR_S29WS256R:
        case NOR_S29WS512R:
        case NOR_S29WS01GR:
            return (boot_sector_flag == AMD_CFI_UNIFORM_DEV);

        default:
            return TRUE;
    }

    return TRUE;
}

bool AMD_MirrorBit_V2_CheckDevID(volatile uint16 *die1_addr,
                                 volatile uint16 *die2_addr)
{
    const NOR_HW_Info_S *flashInfo = &g_FlashDev.m_hw_info;
    uint16 die1_manufacture_code;
    uint16 die1_dev_code;
    uint16 die1_ext_dev_code1;
    uint16 die1_ext_dev_code2;
    uint16 die2_manufacture_code;
    uint16 die2_dev_code;
    uint16 die2_ext_dev_code1;
    uint16 die2_ext_dev_code2;
    uint16 die_count = 0;

    // Enter ID/CFI mode on die1
    die1_addr[0x55] = AMD_ENTER_ID_CFI_MODE_CMD;

    // Read manufacture ID, device ID, and extended device IDs
    die1_manufacture_code = die1_addr[AMD_MANUFACTURE_ID_ADDR];
    die1_dev_code = die1_addr[AMD_DEVICE_CODE_ADDR];
    die1_ext_dev_code1 = die1_addr[AMD_EXT_DEVICE_CODE1_ADDR];
    die1_ext_dev_code2 = die1_addr[AMD_EXT_DEVICE_CODE2_ADDR];

    if (flashInfo->m_manufacture_code != die1_manufacture_code)
    {
        // Reset to read mode
        die1_addr[0x0] = AMD_RESET_TO_READ_MODE_DATA1;
        return FALSE;
    }

    // If manufacture codes match, temporarily record flash ID.
    g_HW_DetectionResult.m_nor_flash_dev_code_1 = die1_manufacture_code;
    g_HW_DetectionResult.m_nor_flash_dev_code_2 = die1_dev_code;
    g_HW_DetectionResult.m_nor_flash_dev_code_3 = die1_ext_dev_code1;
    g_HW_DetectionResult.m_nor_flash_dev_code_4 = die1_ext_dev_code2;

    if ((flashInfo->m_dev_code != die1_dev_code) ||
        ((flashInfo->m_ext_dev_code1 != 0) &&
         (flashInfo->m_ext_dev_code1 != die1_ext_dev_code1)) ||
        ((flashInfo->m_ext_dev_code2 != 0) &&
         (flashInfo->m_ext_dev_code2 != die1_ext_dev_code2)))
    {
        // Reset to read mode
        die1_addr[0x0] = AMD_RESET_TO_READ_MODE_DATA1;
        return FALSE;
    }

    if (!CheckBootSectorFlag(g_FlashDev.m_device_id,
                             die1_addr[AMD_CFI_BOOT_SECTOR_FLAG_ADDR]))
    {
        // Reset to read mode
        die1_addr[0x0] = AMD_RESET_TO_READ_MODE_DATA1;
        return FALSE;
    }

    // Die1 ID matched. Continue to check if this flash is dual stack. 
    die2_manufacture_code = die2_addr[AMD_MANUFACTURE_ID_ADDR];
    die2_dev_code = die2_addr[AMD_DEVICE_CODE_ADDR];
    die2_ext_dev_code1 = die2_addr[AMD_EXT_DEVICE_CODE1_ADDR];
    die2_ext_dev_code2 = die2_addr[AMD_EXT_DEVICE_CODE2_ADDR];

    if ((die1_manufacture_code == die2_manufacture_code) &&
        (die1_dev_code == die2_dev_code) &&
        (die1_ext_dev_code1 == die2_ext_dev_code1) &&
        (die1_ext_dev_code2 == die2_ext_dev_code2))
    {
        die_count = 1;
    }
    else
    {
        die_count = 2;
    }

    // Reset die1 to read mode
    die1_addr[0x0] = AMD_RESET_TO_READ_MODE_DATA1;

    return (die_count == FLASH_DIE_COUNT()) ? TRUE : FALSE;
}

void AMD_MirrorBit_V2_Erase_CMD(const uint32 blockaddr)
{
    volatile uint16 *ba = (volatile uint16 *) blockaddr;

    // Clear status register
    ba[AMD_CMD_ADDR1] = AMD_CLR_SR_CMD;

    ba[AMD_CMD_ADDR1] = AMD_SECTOR_ERASE_SETUP_CMD;
    ba[AMD_CMD_ADDR2] = AMD_SECTOR_ERASE_CMD;
}

STATUS_E AMD_MirrorBit_V2_Erase_CheckDone(const uint32 blockaddr)
{
    volatile uint16 *ba = (volatile uint16 *) blockaddr;
    volatile uint16 sr;

    // Read status register
    ba[AMD_CMD_ADDR1] = AMD_READ_SR_CMD;
    sr = *ba; 

    if (!(sr & AMD_SR_DEVICE_READY_BIT))
    {
	    return S_DA_IN_PROGRESS;
    }
    else if (sr & AMD_SR_ERASE_STATUS_BIT)
    {
        return S_DA_NOR_ERASE_FAILED;
    }

    // Reset to read mode
    *ba = AMD_RESET_TO_READ_MODE_DATA1;

    return S_DONE;
}

#pragma arm section code = "MT6223_CRITCAL_CODE"
void AMD_MirrorBit_V2_Word_Program_CMD(const uint32 blockaddr,
                                       const uint32 prog_addr,
                                       const uint16 data)
{
    AMD_MirrorBit_V2_Buf_Program_CMD(blockaddr, prog_addr, &data, 1);
}

void AMD_MirrorBit_V2_Buf_Program_CMD(const uint32 blockaddr,
                                      const uint32 prog_addr,
                                      const uint16 *data,
                                      const uint32 length_in_word)
{
    volatile uint16 *ba = (volatile uint16 *) blockaddr;
    volatile uint16 *prog_word_addr = (volatile uint16 *) prog_addr;
    volatile uint16 *data_word_addr = (volatile uint16 *) data;
    volatile uint32 i;

    // Clear status register
    ba[AMD_CMD_ADDR1] = AMD_CLR_SR_CMD;

    //
    // Write buffer load
    //
    ba[AMD_CMD_ADDR1] = AMD_BUF_PGM_LOAD;
    ba[AMD_CMD_ADDR2] = (uint16) (length_in_word - 1);

    for (i=0; i<length_in_word; i++)
    {
        *prog_word_addr++ = *data_word_addr++;
    }

    // Buffer to flash
    ba[AMD_CMD_ADDR1] = AMD_BUF_PGM_CONFIRM;
}
#pragma arm section

STATUS_E AMD_MirrorBit_V2_Program_CheckDone(const uint32 prog_addr)
{
    volatile uint16 *pa = (volatile uint16 *) prog_addr;
    volatile uint16 sr;

    // Read status register
    pa[AMD_CMD_ADDR1] = AMD_READ_SR_CMD;
    sr = *pa; 

    if (!(sr & AMD_SR_DEVICE_READY_BIT))
    {
        return S_DA_IN_PROGRESS;
    }
    else if (sr & AMD_SR_PROGRAM_STATUS_BIT)
    {
        return S_DA_NOR_PROGRAM_FAILED;
    }

    // Reset to read mode
    *pa = AMD_RESET_TO_READ_MODE_DATA1;

    return S_DONE;
}

//------------------------------------------------------------------------------
// Protection Related Callback Function                                         
//------------------------------------------------------------------------------
#ifdef AMD_PROTECTION_ENABLED
void AMD_UnprotectChip(void) {
	uint32	die;
	uint32	die_baseaddr;
	uint32	addr;
	uint32	die_size;
	uint32	blk_size;

	// charge all PPBs to 1 
	for(die=0; die<FLASH_DIE_COUNT(); die++) {
		die_size = FLASH_DIE_SIZE(die);
		addr = die_baseaddr = FUTL_GetFlashDieBaseAddr(die);
		while( addr < (die_baseaddr+die_size) ) {
			// set PPB to 1 
			if(!AMD_Protection_PPB_Program_CMD(addr)) {
				while(1);
			}
			// current sector size 
			FUTL_SearchBaseAddr(addr, NULL, &blk_size);
			// next sector address 
			addr += blk_size;
		}
	}

	// issue erase all PPBs command 
	if(!AMD_Protection_Erase_All_PPBs_CMD(FUTL_GetFlashDieBaseAddr(0))) {
		while(1);
	}
}
#pragma arm section code = "MT6223_CRITCAL_CODE"
bool AMD_Protection_PPB_Program_CMD(uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile bool ok_flag = FALSE;
	volatile uint32	i;
	
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_PPB_PGM_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_PROTECT_PPB_PGM_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_PPB_PGM_DATA3;
	ba[AMD_PROTECT_WP_ADDR] = AMD_PROTECT_PPB_PGM_DATA4;
	// delay 100us 
	for(i=0; i<10000; i++);
	ba[AMD_PROTECT_WP_ADDR] = AMD_PROTECT_PPB_PGM_DATA5;
	
	if(!(ba[AMD_PROTECT_WP_ADDR]&AMD_PROTECT_PPB_SET)) {
		ok_flag = FALSE;
	}
	else {
		ok_flag = TRUE;
	}

	// enter auto-select mode 
	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_ENTER_AUTOSELECT_MODE_CMD;

	// reset to read mode 
	ba[AMD_SA_ADDR] = AMD_RESET_TO_READ_MODE_DATA1;

	return ok_flag;
}
#pragma arm section
bool AMD_Protection_PPB_Status_CMD(uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile bool ok_flag = FALSE;
	
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_PPB_STATUS_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_PROTECT_PPB_STATUS_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_PPB_STATUS_DATA3;
	ba[AMD_PROTECT_WP_ADDR] = AMD_PROTECT_PPB_STATUS_DATA4;
	if(!(ba[AMD_PROTECT_WP_ADDR]&AMD_PROTECT_PPB_SET)) {
		ok_flag = FALSE;
	}
	else {
		ok_flag = TRUE;
	}

	// reset to read mode 
	ba[AMD_SA_ADDR] = AMD_RESET_TO_READ_MODE_DATA1;

	return ok_flag;
}

bool AMD_Protection_Erase_All_PPBs_CMD(uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint32	i;
	volatile bool ok_flag=FALSE;
	
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_PPB_ERASE_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_PROTECT_PPB_ERASE_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_PPB_ERASE_DATA3;
	ba[AMD_PROTECT_WP_ADDR] = AMD_PROTECT_PPB_ERASE_DATA4;
	// delay 1.2ms 
	for(i=0; i<0x30000; i++);
	ba[AMD_SA_ADDR] = AMD_PROTECT_PPB_ERASE_DATA5;
	if(ba[AMD_PROTECT_WP_ADDR]&AMD_PROTECT_PPB_SET) {
		ok_flag = TRUE;
	}
	else {
		ok_flag = FALSE;
	}
	
	// reset to read mode 
	ba[AMD_SA_ADDR] = AMD_RESET_TO_READ_MODE_DATA1;
	
	return ok_flag;
}
#endif

void AMD_Protection_DYB_Clear_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	// AMD Sector DYB Entry
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_DYB_ENTRY_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_PROTECT_DYB_ENTRY_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_DYB_ENTRY_DATA3;
	
	// AMD Sector DYB Clear
	ba[0x0] = AMD_PROTECT_DYB_CLEAR_DATA1;
	ba[0x0] = AMD_PROTECT_DYB_CLEAR_DATA2;
	
	while(1)
    {
        if( ba[0x0] == 0x0001){
          break; 
        }
    }
	
	// AMD Sector DYB Exit
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_DYB_EXIT_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_PROTECT_DYB_EXIT_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_PROTECT_DYB_EXIT_DATA3;
	ba[0x0] 		  = AMD_PROTECT_DYB_EXIT_DATA4;
}


//------------------------------------------------------------------------------
// AMD OTP Drivers
//------------------------------------------------------------------------------

void AMD_OTP_ENTER(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;

	// enter SecSi sector
	ba[AMD_CMD_ADDR1] = AMD_OTP_SECSI_ENTER_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_OTP_SECSI_ENTER_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_OTP_SECSI_ENTER_DATA3;
}

void AMD_OTP_EXIT(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	// exit SecSi sector
	ba[AMD_CMD_ADDR1] = AMD_OTP_SECSI_EXIT_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_OTP_SECSI_EXIT_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_OTP_SECSI_EXIT_DATA3;
	ba[AMD_CMD_ADDR1] = AMD_OTP_SECSI_EXIT_DATA4;

	// reset to read mode 
	ba[0x0] = AMD_RESET_TO_READ_MODE_DATA1;
}

void AMD_OTP_Read(const uint32 blockaddr, const uint32 prog_addr, volatile uint16 * data)
{
	volatile uint16 *pa = (volatile uint16 *)prog_addr;

	*data = *pa;
}
#pragma arm section code = "MT6223_CRITCAL_CODE"
void AMD_OTP_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	AMD_Word_Program_CMD( blockaddr, prog_addr, data);
}
#pragma arm section

void AMD_OTP_Lock(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 i;
	volatile uint16 *ba = (volatile uint16 *)blockaddr;

	// enter Lock Register mode
	ba[AMD_CMD_ADDR1] = AMD_OTP_LOCK_REG_ENTER_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_OTP_LOCK_REG_ENTER_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_OTP_LOCK_REG_ENTER_DATA3;

	// program Lock Register DQ0(bit0)
	ba[0x0] = AMD_OTP_LOCK_REG_PROGRAM_DATA1;
	if( NOR_S29WS256N == g_FlashDev.m_device_id ) {
		// for WS-N series
		ba[0x77] &= 0xFE;	// set Secured Silicon Sector Protection Bit (DQ0) = 0,
	}
	else {
		// for PL-N series
		ba[0x0] &= 0xFE;	// set Secured Silicon Sector Protection Bit (DQ0) = 0,
	}
	
	//if MirrorBits, delay 4us
	for (i=0;i<400;i++);

	//perform Toggling algorithm
	while ( S_DA_IN_PROGRESS == AMD_Program_CheckDone_By_Toggle(blockaddr) );

	//fo WS-N series, delay 1000us
	for (i=0;i<1000;i++);

	// exit Lock Register mode
	ba[0x0] = AMD_OTP_LOCK_REG_EXIT_DATA1;
	ba[0x0] = AMD_OTP_LOCK_REG_EXIT_DATA2;
	
	// reset to read mode 
	ba[0x0] = AMD_RESET_TO_READ_MODE_DATA1;
}

STATUS_E AMD_OTP_Lock_CheckStatus(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 status;
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	// enter auto-select mode 
	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_ENTER_AUTOSELECT_MODE_CMD;

	//get information of Indicator Bit
	status = 	ba[AMD_OTP_LOCK_STATUS_ADDR];
	
	// reset to read mode 
	ba[AMD_SA_ADDR] = AMD_RESET_TO_READ_MODE_DATA1;


	//Check DQ6 for LOCK status of Customer Secured Silicon sector
	if( status & AMD_DQ6_CUSTOMER_LOCK_BIT )
		return S_DA_NOR_OTP_LOCKED;
	else
		return S_DA_NOR_OTP_UNLOCKED;
	
}

#endif

