/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#include "flash_drv_RENESAS.h"

#if defined(FEATURE_NOR_SUPPORT)
//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
const NOR_CMD_Callback_S	RENESAS_CMD_CB_WORD_PGM_64Mb = {
	ID_RENESAS_CMD_CB_WORD_PGM_64Mb,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	RENESAS_Erase_Block_CMD_64Mb,
	RENESAS_CheckDone,
	DUMMY_Program_PreProcess,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	RENESAS_Word_Program_64Mb,
	RENESAS_CheckDone,
	NULL,
	NULL
};

const NOR_CMD_Callback_S	RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb = {
	ID_RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	RENESAS_Erase_Block_CMD_64Mb,
	RENESAS_CheckDone,
	RENESAS_PreProcess_128WORD_BUFPGM,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	RENESAS_Word_Program_64Mb,
	RENESAS_CheckDone,
	RENESAS_FixedPage_Program_64Mb,
	RENESAS_CheckDone
};

const NOR_CMD_Callback_S	RENESAS_CMD_CB_WORD_PGM = {
	ID_RENESAS_CMD_CB_WORD_PGM,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	RENESAS_Erase_Block_CMD,
	RENESAS_CheckDone,
	DUMMY_Program_PreProcess,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	RENESAS_Word_Program,
	RENESAS_CheckDone,
	NULL,
	NULL
};

const NOR_CMD_Callback_S	RENESAS_CMD_CB_128WORD_PAGE_PGM = {
	ID_RENESAS_CMD_CB_128WORD_PAGE_PGM,
	INTEL_CheckDevID,
	DUMMY_CheckDevIdle,
	RENESAS_Erase_Block_CMD,
	RENESAS_CheckDone,
	RENESAS_PreProcess_128WORD_BUFPGM,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	RENESAS_Word_Program,
	RENESAS_CheckDone,
	RENESAS_FixedPage_Program,
	RENESAS_CheckDone
};

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
void RENESAS_Erase_Block_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	// clear status register first 
	ba[0x0] = INTEL_CMD_CLR_SR;

	// sw lock release preprocess 
	RENESAS_SoftwareLockRelease_Preprocess(blockaddr);

	// no need to unlock, just erase 
	ba[0x0] = INTEL_CMD_ERASE_STEP1;
	ba[0x0] = INTEL_CMD_ERASE_STEP2;
}

void RENESAS_Erase_Block_CMD_64Mb(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	// clear status register first 
	ba[0x0] = INTEL_CMD_CLR_SR;

	// sw lock release preprocess 
	RENESAS_SoftwareLockRelease_Preprocess_64Mb(blockaddr);

	// no need to unlock, just erase 
	ba[0x0] = INTEL_CMD_ERASE_STEP1;
	ba[0x0] = INTEL_CMD_ERASE_STEP2;
}

STATUS_E RENESAS_CheckDone(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 sr;

	// read SR 
	ba[0x0] = INTEL_CMD_READ_SR;
	sr = *ba;

	if( !(sr&RENESAS_SR7_DWS_BIT) ) {
		return S_DA_IN_PROGRESS;
	}
	else if( (sr&RENESAS_SR4_PS_BIT) && (sr&RENESAS_SR5_ES_BIT) ) {
		return S_DA_NOR_CMD_SEQUENCE_ERR;
	}
	else if(sr&RENESAS_SR5_ES_BIT) {
		return S_DA_NOR_ERASE_FAILED;
	}
	else if(sr&RENESAS_SR4_PS_BIT) {
		return S_DA_NOR_PROGRAM_FAILED;
	}
	else if(sr&RENESAS_SR3_BSAE_BIT) {
		return S_DA_NOR_BLOCK_DATA_UNSTABLE;
	}

	// rest to read mode 
	ba[0x0] = INTEL_CMD_READ_ARRAY;
	return S_DONE;
}

//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
void RENESAS_PreProcess_128WORD_BUFPGM(void) {
	// set max buffered program size 
	g_FLASH_MAX_BUFPGM_SIZE_IN_BYTE = 256; // 128 WORDs 
}
#pragma arm section code = "MT6223_CRITCAL_CODE"
void RENESAS_Word_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	
	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// sw lock release preprocess 
	RENESAS_SoftwareLockRelease_Preprocess(blockaddr);

	// word program 
	*pa = INTEL_CMD_WORD_PGM;
	*pa = data;
}

void RENESAS_Word_Program_64Mb(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	
	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// sw lock release preprocess 
	RENESAS_SoftwareLockRelease_Preprocess_64Mb(blockaddr);

	// word program 
	*pa = INTEL_CMD_WORD_PGM;
	*pa = data;
}

void RENESAS_FixedPage_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	//                                                                                
	// Be careful!! Before you invoke this function, please follow the rules          
	//                                                                                
	// 1. A reasonable length_in_word and make sure the program data won't exceed the 
	//    sector boundary.                                                            
	// 2. length_in_word MUST be equal to 128 WORDs.                                  
	//                                                                                
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	uint32		i;

	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// sw lock release preprocess 
	RENESAS_SoftwareLockRelease_Preprocess(blockaddr);

	// enter page programming 
	*pa = RENESAS_CMD_PAGE_PGM;
		
	for(i=0; i<length_in_word; i++) {
		// fill data to device buffer 
		pa[i] = data[i];
	}
}

void RENESAS_FixedPage_Program_64Mb(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	//                                                                                
	// Be careful!! Before you invoke this function, please follow the rules          
	//                                                                                
	// 1. A reasonable length_in_word and make sure the program data won't exceed the 
	//    sector boundary.                                                            
	// 2. length_in_word MUST be equal to 128 WORDs.                                  
	//                                                                                
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	uint32		i;

	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	// sw lock release preprocess 
	RENESAS_SoftwareLockRelease_Preprocess_64Mb(blockaddr);

	// enter page programming 
	*pa = RENESAS_CMD_PAGE_PGM;
		
	for(i=0; i<length_in_word; i++) {
		// fill data to device buffer 
		pa[i] = data[i];
	}
}
#pragma arm section
#if 0
void RENESAS_DynamicBuf_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	//                                                                                     
	// Be careful!! Before you invoke this function, you must calculate a reasonable       
	// length_in_word and make sure the program data won't exceed the sector boundary.     
	//                                                                                     
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	uint32		i;

	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	//                                                               
	// WARNING!!! DON'T use fixed 128 WORDs Page-Program method when 
	// length_in_word is not equal to 128 WORDS.                     
	//                                                               
	// Because when length_in_word is less than 128, Page-Program    
	// will fail! Therefore, we have to use another program method.  
	// First, load variant length data to Page-Buffer, then flush    
	// Page-Buffer to flash.                                         
	//                                                               

	// load data to Page-Buffer 
	for(i=0; i<length_in_word; i++) {
		// sw lock release preprocess 
		RENESAS_SoftwareLockRelease_Preprocess(blockaddr);
		// fill data to Page-Buffer 
		*pa = RENESAS_CMD_DATA_LOAD_TO_PAGE_BUF;
		pa[i] = data[i];
	}

	// sw lock release preprocess 
	RENESAS_SoftwareLockRelease_Preprocess(blockaddr);
	// flush Page-Buffer to flash 
	*pa = RENESAS_CMD_PAGE_BUF_TO_FLASH;
	*pa = RENESAS_CMD_PAGE_BUF_TO_FLASH_CNF;
}

void RENESAS_DynamicBuf_Program_64Mb(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	//                                                                                     
	// Be careful!! Before you invoke this function, you must calculate a reasonable       
	// length_in_word and make sure the program data won't exceed the sector boundary.     
	//                                                                                     
	volatile uint16 *pa = (volatile uint16 *)prog_addr;
	uint32		i;

	// clear status register first 
	*pa = INTEL_CMD_CLR_SR;

	//                                                               
	// WARNING!!! DON'T use fixed 128 WORDs Page-Program method when 
	// length_in_word is not equal to 128 WORDS.                     
	//                                                               
	// Because when length_in_word is less than 128, Page-Program    
	// will fail! Therefore, we have to use another program method.  
	// First, load variant length data to Page-Buffer, then flush    
	// Page-Buffer to flash.                                         
	//                                                               

	// load data to Page-Buffer 
	for(i=0; i<length_in_word; i++) {
		// sw lock release preprocess 
		RENESAS_SoftwareLockRelease_Preprocess_64Mb(blockaddr);
		// fill data to Page-Buffer 
		*pa = RENESAS_CMD_DATA_LOAD_TO_PAGE_BUF;
		pa[i] = data[i];
	}

	// sw lock release preprocess 
	RENESAS_SoftwareLockRelease_Preprocess_64Mb(blockaddr);
	// flush Page-Buffer to flash 
	*pa = RENESAS_CMD_PAGE_BUF_TO_FLASH;
	*pa = RENESAS_CMD_PAGE_BUF_TO_FLASH_CNF;
}
#endif

//------------------------------------------------------------------------------
// Protection Related Callback Function                                         
//------------------------------------------------------------------------------

void RENESAS_SoftwareLockRelease_Preprocess(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	uint16	block_num;

	//                     DQ7  DQ6  DQ5  DQ4  DQ3  DQ2  DQ1  DQ0        
	// ----------------------------------------------------------------- 
	//         Block Addr: A22  A21  A20  A19  A18  A17  A16  A15        
	// Block Addr Inverse: A22# A21# A20# A19# A18# A17# A16# A15#       
	//                                                                   
	// STEP 1: Right shift 1 bit to convert BYTE address to WORD address 
	// STEP 2: Right shift 15 bit to convert to block address            
	block_num = (blockaddr>>(1+15));

	ba[0x0] = RENESAS_CMD_SWLOCK_RELEASE_CMD_1;
	ba[0x0] = block_num;	// block address 
	ba[0x0] = RENESAS_CMD_SWLOCK_RELEASE_CMD_2;
	ba[0x0] = ~block_num;	// block address inverse 
	ba[0x0] = RENESAS_CMD_SWLOCK_RELEASE_CMD_3;
}

void RENESAS_SoftwareLockRelease_Preprocess_64Mb(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	uint16	block_num;

	//                     DQ7  DQ6  DQ5  DQ4  DQ3  DQ2  DQ1  DQ0        
	// ----------------------------------------------------------------- 
	//         Block Addr: A22  A21  A20  A19  A18  A17  A16  A15        
	// Block Addr Inverse: A22# A21# A20# A19# A18# A17# A16# A15#       
	//                                                                   
	// STEP 1: Right shift 1 bit to convert BYTE address to WORD address 
	// STEP 2: Right shift 15 bit to convert to block address            
	block_num = (blockaddr>>(1+15));

	ba[0x0] = RENESAS_CMD_SWLOCK_RELEASE_CMD_1;
	ba[0x0] = block_num & 0x7f;	// block address , DQ7 is fixed 0
	ba[0x0] = RENESAS_CMD_SWLOCK_RELEASE_CMD_2;
	ba[0x0] = ~block_num & 0x7f;	// block address inverse , DQ7 is fixed 0
	ba[0x0] = RENESAS_CMD_SWLOCK_RELEASE_CMD_3;
}

#endif