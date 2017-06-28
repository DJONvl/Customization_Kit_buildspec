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
#include "flash_drv_INTEL.h"
#include "hw_config.h"

#if defined(FEATURE_NOR_SUPPORT)
//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
const NOR_CMD_Callback_S	INTEL_CMD_CB_WORD_PGM = {
	ID_INTEL_CMD_CB_WORD_PGM,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	INTEL_Erase_Block_CMD,
	INTEL_Erase_CheckDone,
	DUMMY_Program_PreProcess,
	DUMMY_Program_PostProcess,
	INTEL_Block_Unlock,
	DUMMY_Program_Exit,
	INTEL_Word_Program,
	INTEL_Word_Program_CheckDone,
	NULL,
	NULL
};

const NOR_CMD_Callback_S	INTEL_CMD_CB_32WORD_BUF_PGM = {
	ID_INTEL_CMD_CB_32WORD_BUF_PGM,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	INTEL_Erase_Block_CMD,
	INTEL_Erase_CheckDone,
	INTEL_PreProcess_32WORD_BUFPGM,
	DUMMY_Program_PostProcess,
	INTEL_Block_Unlock,
	DUMMY_Program_Exit,
	INTEL_Word_Program,
	INTEL_Word_Program_CheckDone,
	INTEL_Buf_Program,
	INTEL_Word_Program_CheckDone
};

const NOR_CMD_Callback_S	INTEL_CMD_CB_SIBLEY_BUF_PGM = {
	ID_INTEL_CMD_CB_SIBLEY_BUF_PGM,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	INTEL_Erase_Block_CMD,
	INTEL_Erase_CheckDone,
	INTEL_Sibley_PreProcess_1KB_BUFPGM,
	DUMMY_Program_PostProcess,
	INTEL_Block_Unlock,
	DUMMY_Program_Exit,
	INTEL_Sibley_Word_Program,
	INTEL_Sibley_Program_CheckDone,
	INTEL_Sibley_Buf_Program,
	INTEL_Sibley_Program_CheckDone
};

//
//OTP function set
//
const NOR_OTP_CMD_Callback_S INTEL_OTP_CMD_CB = {
	DUMMY_Program_Enter,
	INTEL_OTP_Read,
	INTEL_OTP_Program,
	INTEL_OTP_Lock,
	INTEL_OTP_Lock_CheckStatus,
	DUMMY_Program_Exit
};

//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------

// L18/L30 series (pure 64KW sectors) 
const NOR_Die_Layout_S		INTEL_L_64 = {
	
	0x00800000,

	{
		 { 0x00000000,	64,		0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_L_128 = {
	
	0x01000000,

	{
		 { 0x00000000,	128,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_L_256 = {
	
	0x02000000,

	{
		 { 0x00000000,	256,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// L18/L30 series (Top Boot) 
const NOR_Die_Layout_S		INTEL_L_T_64 = {
	
	0x00800000,

	{
		 { 0x00000000,	63,		0x20000 }
		,{ 0x007E0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_L_T_128 = {
	
	0x01000000,

	{
		 { 0x00000000,	127,	0x20000 }
		,{ 0x00FE0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_L_T_256 = {
	
	0x02000000,

	{
		 { 0x00000000,	255,	0x20000 }
		,{ 0x01FE0000,	4,		0x8000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// L18/L30 series (Bottom Boot) 
const NOR_Die_Layout_S		INTEL_L_B_64 = {
	
	0x00800000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	63,		0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_L_B_128 = {
	
	0x01000000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	127,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_L_B_256 = {
	
	0x02000000,

	{
		 { 0x00000000,	4,		0x8000 }
		,{ 0x00020000,	255,	0x20000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// W18/W30 series (Pure 32KW sectors) 
const NOR_Die_Layout_S		INTEL_W_32 = {
	
	0x00400000,

	{
		 { 0x00000000,	64,		0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_64 = {
	
	0x00800000,

	{
		 { 0x00000000,	127,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_128 = {
	
	0x01000000,

	{
		 { 0x00000000,	255,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// W18/W30/C3 series (Top Boot) 
const NOR_Die_Layout_S		INTEL_W_T_16 = {
	
	0x00200000,

	{
		 { 0x00000000,	31,	    0x10000 }
		,{ 0x001F0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_T_32 = {
	
	0x00400000,

	{
		 { 0x00000000,	63,		0x10000 }
		,{ 0x003F0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_T_64 = {
	
	0x00800000,

	{
		 { 0x00000000,	127,	0x10000 }
		,{ 0x007F0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_T_128 = {
	
	0x01000000,

	{
		 { 0x00000000,	255,	0x10000 }
		,{ 0x00FF0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// W18/W30/C3 series (Bottom Boot) 
const NOR_Die_Layout_S		INTEL_W_B_16 = {
	
	0x00200000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	31,	    0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_B_32 = {
	
	0x00400000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	63,		0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_B_64 = {
	
	0x00800000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	127,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_B_128 = {
	
	0x01000000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	255,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// W18/W30 series (Dual Boot) 
const NOR_Die_Layout_S		INTEL_W_TB_32 = {
	
	0x00400000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	62,		0x10000 }
		,{ 0x003F0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_TB_64 = {
	
	0x00800000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	126,	0x10000 }
		,{ 0x007F0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_W_TB_128 = {
	
	0x01000000,

	{
		 { 0x00000000,	8,		0x2000 }
		,{ 0x00010000,	254,	0x10000 }
		,{ 0x00FF0000,	8,		0x2000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

// Sibley series (pure 128KW sectors) 
const NOR_Die_Layout_S		INTEL_M18_128 = {
	
	0x01000000,

	{
		 { 0x00000000,	64,		0x40000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_M18_256 = {
	
	0x02000000,

	{
		 { 0x00000000,	128,	0x40000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_M18_512 = {
	
	0x04000000,

	{
		 { 0x00000000,	256,	0x40000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_Die_Layout_S		INTEL_M18_1024 = {
	
	0x08000000,

	{
		 { 0x00000000,	512,	0x40000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};
//
//OTP layout
//
const NOR_OTP_Layout_S		INTEL_OTP_1PR ={
	0x008,
		
	{
		 { 0x10a,		1,	0x08}
		,{ 0, 0, 0}
	}
};

const NOR_OTP_Layout_S		INTEL_OTP_17PR ={
	0x108,
		
	{
		 { 0x10a,		1,	0x08}
		,{ 0x114,		16,	0x10}
	}
};

//------------------------------------------------------------------------------
// Check Device ID Callback Function                                            
//------------------------------------------------------------------------------
bool INTEL_CheckDevID(volatile uint16 *die1_addr, volatile uint16 *die2_addr) {

	uint16	die1_manufacture_code;
	uint16	die1_dev_code;
	uint16	die2_manufacture_code;
	uint16	die2_dev_code;
	uint16	die_count = 0;
	bool	ret;

	// clear status register first 
	die1_addr[0x0] = INTEL_CMD_CLR_SR;

	// issue device id command 
	die1_addr[0x0] = INTEL_CMD_READ_ID;

	// read manufacture id and device code 
	die1_manufacture_code = die1_addr[0x0];
	die1_dev_code = die1_addr[0x01];

	// compare id 
	if( g_FlashDev.m_hw_info.m_manufacture_code != die1_manufacture_code )
		goto not_match;

	// if manufacture code exist, temporarily record flash id to g_HW_DetectionResult 
	g_HW_DetectionResult.m_nor_flash_dev_code_1 = die1_manufacture_code;
	g_HW_DetectionResult.m_nor_flash_dev_code_2 = die1_dev_code;

	if( g_FlashDev.m_hw_info.m_dev_code != die1_dev_code )
		goto not_match;

	// die1 id matched, continue to check if this flash is dual stack 
	die2_manufacture_code = die2_addr[0x0];
	die2_dev_code = die2_addr[0x01];

	if( (die1_manufacture_code == die2_manufacture_code) && (die1_dev_code == die2_dev_code) ){
		die_count = 1;
	}
	else {
		die_count = 2;
	}

	// if die count matched, return true 
	if( die_count == FLASH_DIE_COUNT() ) {
		// compare die2 id 
		if( (2 == FLASH_DIE_COUNT()) && 
			(0 != g_FlashDev.m_hw_info.m_ext_dev_code1) && 
			(0 != g_FlashDev.m_hw_info.m_ext_dev_code2)
		) {
			// clear status register first 
			die2_addr[0x0] = INTEL_CMD_CLR_SR;

			// issue device id command 
			die2_addr[0x0] = INTEL_CMD_READ_ID;

			// read manufacture id and device code 
			die2_manufacture_code = die2_addr[0x0];
			die2_dev_code = die2_addr[0x01];

			// compare id 
			if( g_FlashDev.m_hw_info.m_ext_dev_code1 != die2_manufacture_code )
				goto not_match;

			// if manufacture code exist, temporarily record flash id to g_HW_DetectionResult 
			g_HW_DetectionResult.m_nor_flash_dev_code_3 = die2_manufacture_code;
			g_HW_DetectionResult.m_nor_flash_dev_code_4 = die2_dev_code;

			if( g_FlashDev.m_hw_info.m_ext_dev_code2 != die2_dev_code )
				goto not_match;
		}

		// found 
		goto match;
	}

not_match:
	ret = FALSE;
	goto end;

match:
	ret = TRUE;

end:
	// reset die1 to read mode 
	die1_addr[0x0] = INTEL_CMD_READ_ARRAY;
	// reset die2 to read mode 
	die2_addr[0x0] = INTEL_CMD_READ_ARRAY;

	return ret;
}


//------------------------------------------------------------------------------
// Check If Device Is Idle Callback Function                                    
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Erase Related Callback Function                                              
//------------------------------------------------------------------------------
void INTEL_Erase_Block_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;

	// clear status register first 
	ba[0x0] = INTEL_CMD_CLR_SR;

	// unlock block first 
	ba[0x0] = INTEL_CMD_SW_UNLOCK_STEP1;
	ba[0x0] = INTEL_CMD_SW_UNLOCK_STEP2;

	// erase 
	ba[0x0] = INTEL_CMD_ERASE_STEP1;
	ba[0x0] = INTEL_CMD_ERASE_STEP2;
}

STATUS_E INTEL_Erase_CheckDone(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 sr;

	// read SR 
	ba[0x0] = INTEL_CMD_READ_SR;
	sr = *ba;

	if( !(sr&INTEL_SR7_DWS_BIT) ) {
		return S_DA_IN_PROGRESS;
	}
	else if(sr&INTEL_SR3_VPPS_BIT) {
		return S_DA_NOR_VPP_RANGE_ERR;
	}
	else if( (sr&INTEL_SR4_PS_BIT) && (sr&INTEL_SR5_ES_BIT) ) {
		return S_DA_NOR_CMD_SEQUENCE_ERR;
	}
	else if(sr&INTEL_SR5_ES_BIT) {
		return S_DA_NOR_ERASE_FAILED;
	}
	else if(sr&INTEL_SR1_BLS_BIT) {
		return S_DA_NOR_BLOCK_IS_LOCKED;
	}

	// reset to read mode 
	ba[0x0] = INTEL_CMD_READ_ARRAY;
	return S_DONE;
}

//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
#pragma arm section code = "MT6223_CRITCAL_CODE"
void INTEL_PreProcess_32WORD_BUFPGM(void) {
	// set max buffered program size 
	g_FLASH_MAX_BUFPGM_SIZE_IN_BYTE = 64; // 32 WORDs 
}
#pragma arm section
void INTEL_Block_Unlock(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	// clear status register first 
	ba[0x0] = INTEL_CMD_CLR_SR;

	// unlock 
	ba[0x0] = INTEL_CMD_SW_UNLOCK_STEP1;
	ba[0x0] = INTEL_CMD_SW_UNLOCK_STEP2;

	// reset to read mode 
	ba[0x0] = INTEL_CMD_READ_ARRAY;
}

#pragma arm section code = "MT6223_CRITCAL_CODE"
void INTEL_Word_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	
	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// word program 
	*pa = INTEL_CMD_WORD_PGM;
	*pa = data;
}

STATUS_E INTEL_Word_Program_CheckDone(const uint32 prog_addr)
{
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	volatile uint16 sr;

	// read SR 
	*pa = INTEL_CMD_READ_SR;
	sr = *pa;

	if( !(sr&INTEL_SR7_DWS_BIT) ) {
		return S_DA_IN_PROGRESS;
	}
	else if(sr&INTEL_SR3_VPPS_BIT) {
		return S_DA_NOR_VPP_RANGE_ERR;
	}
	else if(sr&INTEL_SR4_PS_BIT) {
		return S_DA_NOR_PROGRAM_FAILED;
	}
	else if(sr&INTEL_SR1_BLS_BIT) {
		return S_DA_NOR_BLOCK_IS_LOCKED;
	}

	// reset to read mode 
	*pa = INTEL_CMD_READ_ARRAY;

	return S_DONE;
}

void INTEL_Buf_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	//                                                                                     
	// Be careful!! Before you invoke this function, you must calculate a reasonable       
	// length_in_word and make sure the program data won't exceed the sector boundary.     
	//                                                                                     
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	uint32		i;

	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// enter buffered programming 
	*pa = INTEL_CMD_BUF_PGM;
		
	// set data count 
	*pa = length_in_word-1;

	for(i=0; i<length_in_word; i++) {
		// fill data to device buffer 
		pa[i] = data[i];
	}

	// set confirm command to flush buffer to flash 
	*pa = INTEL_CMD_BUF_PGM_CNF;
}
#pragma arm section
//------------------------------------------------------------------------------
// Sibley Related Callback Function                                             
//------------------------------------------------------------------------------
void INTEL_Sibley_PreProcess_1KB_BUFPGM(void) {
	// set max buffered program size 
	g_FLASH_MAX_BUFPGM_SIZE_IN_BYTE = INTEL_SIBLEY_PGM_REGION_SIZE_IN_BYTE; // 1KB 
}
#pragma arm section code = "MT6223_CRITCAL_CODE"
void INTEL_Sibley_Word_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	
	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// word program 
	*pa = INTEL_CMD_SIBLEY_WORD_PGM;
	*pa = data;
}
#pragma arm section
STATUS_E INTEL_Sibley_Program_CheckDone(const uint32 prog_addr)
{
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	volatile uint16 sr;

	// read SR 
	*pa = INTEL_CMD_READ_SR;
	sr = *pa;

	if( !(sr&INTEL_SR7_DWS_BIT) ) {
		return S_DA_IN_PROGRESS;
	}
	else if(sr&INTEL_SR3_VPPS_BIT) {
		return S_DA_NOR_VPP_RANGE_ERR;
	}
	else if(sr&INTEL_SR4_PS_BIT) {
		switch(sr&INTEL_RPS_MASK) {
		case INTEL_RPS_REWRITE_OBJ_MODE_REGION:
			return S_DA_SIBLEY_REWRITE_OBJ_MODE_REGION;
		case INTEL_RPS_WRITE_B_HALF_IN_CTRL_MODE_REGION:
			return S_DA_SIBLEY_WRITE_B_HALF_IN_CTRL_MODE_REGION;
		case INTEL_RPS_ILLEGAL_CMD:
			return S_DA_SIBLEY_ILLEGAL_CMD;
		default:
			return S_DA_NOR_PROGRAM_FAILED;
		}
	}
	else if(sr&INTEL_SR1_BLS_BIT) {
		return S_DA_NOR_BLOCK_IS_LOCKED;
	}

	// reset to read mode 
	*pa = INTEL_CMD_READ_ARRAY;

	return S_DONE;
}
#pragma arm section code = "MT6223_CRITCAL_CODE"
void INTEL_Sibley_Buf_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	//                                                                                     
	// Be careful!! Before you invoke this function, you must calculate a reasonable       
	// length_in_word and make sure the program data won't exceed the sector boundary.     
	//                                                                                     
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	uint32		i;

	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// enter buffered programming 
	*pa = INTEL_CMD_SIBLEY_BUF_PGM;
		
	// set data count 
	*pa = length_in_word-1;

	for(i=0; i<length_in_word; i++) {
		// fill data to device buffer 
		pa[i] = data[i];
	}

	// set confirm command to flush buffer to flash 
	*pa = INTEL_CMD_BUF_PGM_CNF;
}
#pragma arm section
//------------------------------------------------------------------------------
// Protection Related Callback Function                                         
//------------------------------------------------------------------------------

//
//OTP_Drivers
//
void INTEL_OTP_Read(const uint32 blockaddr, const uint32 prog_addr, volatile uint16 * data)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	
	// clear status register first 
	ba[0x0] = INTEL_CMD_CLR_SR;

	// enter Device ID mode 
	ba[0x0] = INTEL_CMD_READ_ID;

	// OTP read
	* data = *pa;

	// reset to read mode 
	ba[0x0] = INTEL_CMD_READ_ARRAY;
}
#pragma arm section code = "MT6223_CRITCAL_CODE"
void INTEL_OTP_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	
	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// otp program protection register 
	*pa = INTEL_CMD_OTP_PROGRAM_PR;
	*pa = data;
	
	// reset to read mode 
	ba[0x0] = INTEL_CMD_READ_ARRAY;
}
#pragma arm section
void INTEL_OTP_Lock(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 *pa;
	uint16 lock;
	
	if( 0 == data ){							//Protection Register 0
		lock = 1 <<1;
		pa = (volatile uint16 *)INTEL_OTP_LOCK_REG_0_ADDR;
	}
	else if( (1 <= data) && (data <= 16) ) {	//Protection Register 1 ~ 16
		lock = 1 << (data -1);
		pa = (volatile uint16 *)INTEL_OTP_LOCK_REG_1_ADDR;
	}
	
	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// otp program protection register 
	*pa = INTEL_CMD_OTP_PROGRAM_PR;

	// otp program protection bit
	* pa = ~lock;	

	// reset to read mode 
	ba[0x0] = INTEL_CMD_READ_ARRAY;
}

STATUS_E INTEL_OTP_Lock_CheckStatus(const uint32 blockaddr, const uint32 prog_addr, const uint16 data){
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 *pba;
	volatile uint16 status;
	volatile uint16 lock;
	
	// clear status register first 
	ba[0x0] = INTEL_CMD_CLR_SR;

	// enter Device ID mode 
	ba[0x0] = INTEL_CMD_READ_ID;

	// get information for Lock Register status
	if( 0 == data ){							//Protection Register 0
		lock = 1 <<1;
		pba = (volatile uint16 *)INTEL_OTP_LOCK_REG_0_ADDR;
	}
	else if( (1 <= data) && (data <= 16) ) {	//Protection Register 1 ~ 16
		lock = 1 << (data -1);
		pba = (volatile uint16 *)INTEL_OTP_LOCK_REG_1_ADDR;
	}
	status = *pba;

	// reset to read mode 
	ba[0x0] = INTEL_CMD_READ_ARRAY;

	// check Lock Register lock status
	if( ~status & lock )
    {
		return S_DA_NOR_OTP_LOCKED;
    }
	else
    {
		return S_DA_NOR_OTP_UNLOCKED;
    }
}

#endif
