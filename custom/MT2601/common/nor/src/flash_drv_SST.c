/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#include "flash_drv_SST.h"
#include "hw_config.h"

#if defined(FEATURE_NOR_SUPPORT)
//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
const NOR_CMD_Callback_S	SST_CMD_CB_WORD_PGM = {
	ID_SST_CMD_CB_WORD_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	SST_Erase_CMD,
	SST_Erase_CheckDone_By_Polling,
	DUMMY_Program_PreProcess,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	AMD_Word_Program_CMD,
	SST_Program_CheckDone_By_Toggle,
	NULL,
	NULL
};

//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------
const NOR_Die_Layout_S		SST_16 = {

	0x00200000,

	{
		 { 0x00000000,	32, 0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}

};

const NOR_Die_Layout_S		SST_32 = {

	0x00400000,

	{
		 { 0x00000000,	64,	0x10000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};


//------------------------------------------------------------------------------
// Erase Related Callback Function                                              
//------------------------------------------------------------------------------
void SST_Erase_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;

	// In SST, there're 3 erase operation: sector/block/chip erase.
	// Chip erase operation is issued to erase all flash memory.
	// Sector-erase and block-erase operations are issued to erase
	// uniform sector size of 2 KWord and 32 KWord, respectively.
	//
	// We choose Block-Erase operation for an efficient erase time.
	
	ba[SST_CMD_ADDR1] = SST_BLOCK_ERASE_DATA1;
	ba[SST_CMD_ADDR2] = SST_BLOCK_ERASE_DATA2;
	ba[SST_CMD_ADDR1] = SST_BLOCK_ERASE_DATA3;
	ba[SST_CMD_ADDR1] = SST_BLOCK_ERASE_DATA4;
	ba[SST_CMD_ADDR2] = SST_BLOCK_ERASE_DATA5;
	ba[SST_SA_ADDR] = SST_BLOCK_ERASE_DATA6;
}

STATUS_E SST_Erase_CheckDone_By_Polling(const uint32 blockaddr)
{
	// use POLL_BIT instead of TOGGLE_BIT to wait for erase complete, 
	// because in some device using TOGGLE_BIT to wait for erase complete will fail. 
	volatile uint16 status;

	status = *(volatile uint16 *)blockaddr;
	if( (status&SST_DQ7_POLL_BIT) ) {
		return S_DONE;
	}
	else {
		return S_DA_IN_PROGRESS;
	}
}

//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------

STATUS_E SST_Program_CheckDone_By_Toggle(const uint32 prog_addr)
{
	volatile uint16 stat_data1;
	volatile uint16 stat_data2;
	
	stat_data1 = *(volatile uint16*)prog_addr;
	stat_data2 = *(volatile uint16*)prog_addr;

	if( (stat_data1&SST_DQ6_TOGGLE_BIT) == (stat_data2&SST_DQ6_TOGGLE_BIT) )
	{
		return S_DONE;
	}
	
	return S_DA_IN_PROGRESS;
}

#endif


