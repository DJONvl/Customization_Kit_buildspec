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
#include "flash_drv_TOSHIBA.h"
#include "hw_config.h"

#if defined(FEATURE_NOR_SUPPORT)
//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
const NOR_CMD_Callback_S	TOSHIBA_CMD_CB_BUF_PGM = {
	ID_TOSHIBA_CMD_CB_BUF_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	TOSHIBA_PreProcess, 
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	AMD_Word_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	TOSHIBA_Buf_Program_CMD,
	AMD_Program_CheckDone_By_Toggle
};

const NOR_CMD_Callback_S	TOSHIBA_CMD_CB_PAGE_8WORDS_PGM = {
	ID_TOSHIBA_CMD_CB_PAGE_8WORDS_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	TOSHIBA_Page_8Words_PreProcess,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	AMD_Word_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	TOSHIBA_Page_8Words_Program_CMD,
	AMD_Program_CheckDone_By_Toggle
};

//
//OTP function set
//
const NOR_OTP_CMD_Callback_S TOSHIBA_HIDDEN_ROM_CMD_CB = {
	TOSHIBA_HIDDEN_ROM_ENTER,
	TOSHIBA_HIDDEN_ROM_Read,
	TOSHIBA_HIDDEN_ROM_Program,
	TOSHIBA_HIDDEN_ROM_Lock,
	TOSHIBA_HIDDEN_ROM_Lock_CheckStatus,
	TOSHIBA_HIDDEN_ROM_EXIT
};
//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------
const NOR_OTP_Layout_S		TOSHIBA_HIDDEN_ROM_T_32K ={
	0x8000,
		
	{
		 { 0x7F8000,		1,	0x8000}
		,{ 0, 0, 0}
	}
};

const NOR_OTP_Layout_S		TOSHIBA_HIDDEN_ROM_T_64K ={
	0x10000,
		
	{
		 { 0xFF0000,		1,	0x10000}
		,{ 0, 0, 0}
	}
};

//------------------------------------------------------------------------------
// TOSHIBA Related Callback Function                                         
//------------------------------------------------------------------------------
void TOSHIBA_PreProcess(void)
{
	g_FLASH_MAX_BUFPGM_SIZE_IN_BYTE = 64;
}

void TOSHIBA_Page_8Words_PreProcess(void)
{
	g_FLASH_MAX_BUFPGM_SIZE_IN_BYTE = 16;
}

#pragma arm section code = "MT6223_CRITCAL_CODE"
void TOSHIBA_Buf_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	volatile uint32 i;
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 *prog_word_addr = (volatile uint16*)prog_addr;
	volatile uint16 *data_word_addr = (volatile uint16*)data;

	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_CMD_ADDR1] = TOSHIBA_PAGE_PROGRAM_32WORD;
	
	for(i=0;i<length_in_word;i++)
	{
		*prog_word_addr++ = *data_word_addr++;
	}
}

void TOSHIBA_Page_8Words_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
	volatile uint32 i;
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile uint16 *prog_word_addr = (volatile uint16*)prog_addr;
	volatile uint16 *data_word_addr = (volatile uint16*)data;

	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_CMD_ADDR1] = TOSHIBA_PAGE_PROGRAM_8WORD;
	
	for(i=0;i<length_in_word;i++)
	{
		*prog_word_addr++ = *data_word_addr++;
	}
}
#pragma arm section
//------------------------------------------------------------------------------
// TOSHIBA OTP Drivers
//------------------------------------------------------------------------------
void TOSHIBA_HIDDEN_ROM_ENTER(const uint32 blockaddr)
{
	AMD_OTP_ENTER(blockaddr);
}

void TOSHIBA_HIDDEN_ROM_EXIT(const uint32 blockaddr)
{
	AMD_OTP_EXIT(blockaddr);
}

void TOSHIBA_HIDDEN_ROM_Read(const uint32 blockaddr, const uint32 prog_addr, volatile uint16 * data)
{
	AMD_OTP_Read(blockaddr,prog_addr,data);
}
#pragma arm section code = "MT6223_CRITCAL_CODE"
void TOSHIBA_HIDDEN_ROM_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	AMD_Word_Program_CMD( blockaddr, prog_addr, data);
}

#pragma arm section
void TOSHIBA_HIDDEN_ROM_Lock(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint32	i;
	volatile uint16 status;
	volatile uint16 *ba = (volatile uint16 *)blockaddr;

	// Enter Hidden ROM Area
	TOSHIBA_HIDDEN_ROM_ENTER(blockaddr);

	// Hidden ROM Protect cmd
	ba[AMD_CMD_ADDR1] = TOSHIBA_HIDDEN_ROM_RPOTECT_DATA1;
	ba[AMD_CMD_ADDR2] = TOSHIBA_HIDDEN_ROM_RPOTECT_DATA2;
	ba[AMD_CMD_ADDR1] = TOSHIBA_HIDDEN_ROM_RPOTECT_DATA3;
	ba[0x1A] 		  = TOSHIBA_HIDDEN_ROM_RPOTECT_DATA4;

	// a little delay
	for (i=0;i<500;i++);
	
	// Verify if Protect complete
	while(1){
		status = *(volatile uint16 *)blockaddr;

		if(status&AMD_DQ7_POLL_BIT){
			// Protect completes
			break;
		}
		else if(status&AMD_DQ5_TIMEOUT_ERR_BIT){
			// Protect fails
			break;
		}
		else{
				continue;				
		}
	}

	// a little delay
	for (i=0;i<500;i++);

	// Exit Hidden ROM Area
	TOSHIBA_HIDDEN_ROM_EXIT(blockaddr);
	// reset to read mode 
	ba[0x0] = AMD_RESET_TO_READ_MODE_DATA1;
}

STATUS_E TOSHIBA_HIDDEN_ROM_Lock_CheckStatus(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 status;
	volatile uint16 *ba = (volatile uint16 *)blockaddr;

	// Enter Hidden ROM Area
	TOSHIBA_HIDDEN_ROM_ENTER(blockaddr);
		
	// enter auto-select mode 
	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_ENTER_AUTOSELECT_MODE_CMD;

	//verify Hidden ROM lock status
	status = 	ba[0x02];
	
	// reset to read mode 
	ba[AMD_CMD_ADDR1] = 0xF0;

	// Exit Hidden ROM Area
	TOSHIBA_HIDDEN_ROM_EXIT(blockaddr);
	
	// 00h No-Protected, 01h Protected
	if( status )
		return S_DA_NOR_OTP_LOCKED;
	else
		return S_DA_NOR_OTP_UNLOCKED;
	
}

#endif
