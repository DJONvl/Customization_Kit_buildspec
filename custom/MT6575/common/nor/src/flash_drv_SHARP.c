/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#include "flash_drv_SHARP.h"

#if defined(FEATURE_NOR_SUPPORT)
//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
const NOR_CMD_Callback_S	SHARP_CMD_CB_WORD_PGM = {
	ID_SHARP_CMD_CB_WORD_PGM,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	INTEL_Erase_Block_CMD,
	SHARP_CheckDone,
	DUMMY_Program_PreProcess,
	DUMMY_Program_PostProcess,
	INTEL_Block_Unlock,
	DUMMY_Program_Exit,
	INTEL_Word_Program,
	SHARP_CheckDone,
	NULL,
	NULL
};

const NOR_CMD_Callback_S	SHARP_CMD_CB_16WORD_BUF_PGM = {
	ID_SHARP_CMD_CB_16WORD_BUF_PGM,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	INTEL_Erase_Block_CMD,
	SHARP_CheckDone,
	SHARP_PreProcess_16WORD_BUFPGM,
	DUMMY_Program_PostProcess,
	INTEL_Block_Unlock,
	DUMMY_Program_Exit,
	INTEL_Word_Program,
	SHARP_CheckDone,
	INTEL_Buf_Program,
	SHARP_CheckDone
};

const NOR_CMD_Callback_S	SHARP_CMD_CB_WORD_PGM_NO_UNLOCK = {
	ID_SHARP_CMD_CB_WORD_PGM_NO_UNLOCK,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	SHARP_Erase_Block_CMD,
	SHARP_CheckDone,
	DUMMY_Program_PreProcess,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	INTEL_Word_Program,
	SHARP_CheckDone,
	NULL,
	NULL
};

//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------

const NOR_Die_Layout_S SHARP_T_32 = {
	0x00400000,
	{
		 { 0x00000000,	31,		0x20000 } //main
		,{ 0x003E0000,	4,		0x8000 } //parameter
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};	

//
//OTP layout
//
const NOR_OTP_Layout_S		SHARP_OTP_128_T ={
	0x108,
		
	{
		 { 0xE0010a,		1,	0x08}
		,{ 0xE00114,		16,	0x10}
	}
};

//------------------------------------------------------------------------------
// Check Device ID Callback Function                                            
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Check If Device Is Idle Callback Function                                    
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Erase Related Callback Function                                              
//------------------------------------------------------------------------------
void SHARP_Erase_Block_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;

	// clear status register first 
	ba[0x0] = INTEL_CMD_CLR_SR;

	// erase 
	ba[0x0] = INTEL_CMD_ERASE_STEP1;
	ba[0x0] = INTEL_CMD_ERASE_STEP2;
}

STATUS_E SHARP_CheckDone(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 sr;

	// read SR 
	ba[0x0] = INTEL_CMD_READ_SR;
	sr = *ba;

	if( !(sr&SHARP_SR7_WSMS_BIT) ) {
		return S_DA_IN_PROGRESS;
	}
	else if(sr&SHARP_SR3_VPPS_BIT) {
		return S_DA_NOR_VPP_RANGE_ERR;
	}
	else if(sr&SHARP_SR1_DPS_BIT) {
		return S_DA_NOR_BLOCK_IS_LOCKED;
	}
	else if( (sr&SHARP_SR4_PS_BIT) && (sr&SHARP_SR5_ES_BIT) ) {
		return S_DA_NOR_CMD_SEQUENCE_ERR;
	}
	else if(sr&SHARP_SR4_PS_BIT) {
		return S_DA_NOR_PROGRAM_FAILED;
	}
	else if(sr&SHARP_SR5_ES_BIT) {
		return S_DA_NOR_ERASE_FAILED;
	}

	// reset to read mode 
	ba[0x0] = INTEL_CMD_READ_ARRAY;
	return S_DONE;
}

//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
void SHARP_PreProcess_16WORD_BUFPGM(void) {
	// set max buffered program size 
	g_FLASH_MAX_BUFPGM_SIZE_IN_BYTE = 32; // 16 WORDs 
}

#endif
//------------------------------------------------------------------------------
// Protection Related Callback Function                                         
//------------------------------------------------------------------------------
