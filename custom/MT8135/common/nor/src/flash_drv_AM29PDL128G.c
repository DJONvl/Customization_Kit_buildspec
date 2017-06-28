/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#include "flash_drv_AM29PDL128G.h"

#if defined(FEATURE_NOR_SUPPORT)

const NOR_CMD_Callback_S	AMD_AM29PDL128G_CMD_CB_UNLOCK_BYPASS_PGM = {
	ID_AMD_AM29PDL128G_CMD_CB_UNLOCK_BYPASS_PGM,
//	AMD_AM29PDL128G_CheckDevID,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_AM29PDL128G_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	DUMMY_Program_PreProcess,
	DUMMY_Program_PostProcess,
	AMD_AM29PDL128G_UnlockBypass_Enter_CMD,
	AMD_AM29PDL128G_UnlockBypass_Exit_CMD,
	AMD_AM29PDL128G_UnlockBypass_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	NULL,
	NULL
};

//------------------------------------------------------------------------------
// Erase Relative Callback Function                                             
//------------------------------------------------------------------------------
void AMD_AM29PDL128G_Erase_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	ba[AMD_AM29PDL128G_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_AM29PDL128G_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_AM29PDL128G_CMD_ADDR1] = AMD_SECTOR_ERASE_SETUP_CMD;
	ba[AMD_AM29PDL128G_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_AM29PDL128G_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_AM29PDL128G_SA_ADDR] = AMD_SECTOR_ERASE_CMD;
}

//------------------------------------------------------------------------------
// Program Relative Callback Function                                           
//------------------------------------------------------------------------------
void AMD_AM29PDL128G_UnlockBypass_Enter_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	ba[AMD_AM29PDL128G_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_AM29PDL128G_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_AM29PDL128G_CMD_ADDR1] = AMD_ENTER_UNLOCK_BYPASS_CMD;
}

void AMD_AM29PDL128G_UnlockBypass_Exit_CMD(const uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	ba[AMD_AM29PDL128G_CMD_ADDR1] = AMD_EXIT_UNLOCK_BYPASS_DATA1;
	ba[AMD_AM29PDL128G_CMD_ADDR2] = AMD_EXIT_UNLOCK_BYPASS_DATA2;
}

void AMD_AM29PDL128G_UnlockBypass_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	
	ba[AMD_AM29PDL128G_CMD_ADDR1] = AMD_UNLOCK_BYPASS_PGM_DATA1;
	*(volatile uint16*)prog_addr = data;
}

#endif
