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
#include "flash_drv_SAMSUNG.h"
#include "hw_config.h"

#if defined(FEATURE_NOR_SUPPORT)
//------------------------------------------------------------------------------
// Callback Function Set                                                        
//------------------------------------------------------------------------------
const NOR_CMD_Callback_S	AMD_SAMSUNG_CB_UNLOCK_BYPASS_PGM = {
	ID_SAMSUNG_CMD_CB_UNLOCK_BYPASS_PGM,
	AMD_CheckDevID,
	AMD_CheckDevIdle,
	AMD_Erase_CMD,
	AMD_Erase_CheckDone_By_Polling,
	SAMSUNG_PreProcess,
	DUMMY_Program_PostProcess,
	AMD_UnlockBypass_Enter_CMD,
	AMD_UnlockBypass_Exit_CMD,
	AMD_UnlockBypass_Program_CMD,
	AMD_Program_CheckDone_By_Toggle,
	NULL,
	NULL
};

//
//OTP function set
//

//------------------------------------------------------------------------------
// Memory Sector Layout Set                                                     
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// TOSHIBA Related Callback Function                                         
//------------------------------------------------------------------------------
void SAMSUNG_PreProcess(void) {
	SAMSUNG_UnprotectChip();
}

void SAMSUNG_UnprotectChip(void) {
	uint32	die;
	uint32	die_baseaddr;
	uint32	addr;
	uint32	die_size;
	uint32	blk_size;

	for(die=0; die<FLASH_DIE_COUNT(); die++) {
		die_size = FLASH_DIE_SIZE(die);
		addr = die_baseaddr = FUTL_GetFlashDieBaseAddr(die);
		while( addr < (die_baseaddr+die_size) ) {
			if(!SAMSUNG_Protection_PPB_Program_CMD(addr)) {
				while(1);
			}
			// current sector size 
			FUTL_SearchBaseAddr(addr, NULL, &blk_size);
			// next sector address 
			addr += blk_size;
		}
	}

}

#pragma arm section code = "MT6223_CRITCAL_CODE"
bool SAMSUNG_Protection_PPB_Program_CMD(uint32 blockaddr)
{
	volatile uint16 *ba = (volatile uint16 *)blockaddr;
	volatile bool ok_flag = FALSE;
	volatile uint32	i;
	
	ba[SAMSUNG_UNPROTECTED_ADDR] = SAMSUNG_UNPROTECTED_DATA;     // BA + (0x42<<1) = 0x60;
	ba[SAMSUNG_UNPROTECTED_ADDR] = SAMSUNG_UNPROTECTED_DATA;
	ba[SAMSUNG_UNPROTECTED_ADDR] = SAMSUNG_UNPROTECTED_DATA;
	
	// delay 100us 
	for(i=0; i<10000; i++);
	
	// enter auto-select mode 
	ba[AMD_CMD_ADDR1] = AMD_UNLOCK_CYCLES_DATA1;
	ba[AMD_CMD_ADDR2] = AMD_UNLOCK_CYCLES_DATA2;
	ba[AMD_CMD_ADDR1] = AMD_ENTER_AUTOSELECT_MODE_CMD;

	if(!(ba[AMD_PROTECT_WP_ADDR]&AMD_PROTECT_PPB_SET )){
		ok_flag = TRUE;
	}
	else {
		ok_flag = FALSE;
	}
	// reset to read mode 
	ba[AMD_SA_ADDR] = AMD_RESET_TO_READ_MODE_DATA1;

	return ok_flag;
}
#pragma arm section

#endif
//------------------------------------------------------------------------------
// TOSHIBA OTP Drivers
//------------------------------------------------------------------------------
