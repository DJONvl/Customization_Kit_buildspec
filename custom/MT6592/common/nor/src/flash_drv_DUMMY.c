/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#include "flash_drv_DUMMY.h"

#if defined(FEATURE_NOR_SUPPORT)

const NOR_Die_Layout_S		DUMMY_MAP = {
	
	0x00000000,

	{
		 { 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_OTP_Layout_S		DUMMY_OTP_MAP = {
	0x00000000,

	{
		 { 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

const NOR_CMD_Callback_S	DUMMY_CMD = {
	ID_DUMMY_CMD,
	DUMMY_CheckDevID,
	DUMMY_CheckDevIdle,
	DUMMY_Erase,
	DUMMY_Erase_CheckDone,
	DUMMY_Program_PreProcess,
	DUMMY_Program_PostProcess,
	DUMMY_Program_Enter,
	DUMMY_Program_Exit,
	DUMMY_Program,
	DUMMY_Program_CheckDone,
	NULL,
	NULL
};


#ifdef USE_GCC
__inline bool			DUMMY_CheckDevID(volatile uint16 *die1_addr, volatile uint16 *die2_addr) { return FALSE; }
__inline bool			DUMMY_CheckDevIdle(const uint32 blockaddr) { return TRUE; }
__inline void			DUMMY_Erase(const uint32 blockaddr) { }
__inline STATUS_E		DUMMY_Erase_CheckDone(const uint32 blockaddr) { return S_DONE; }
__inline void			DUMMY_Program_PreProcess(void) { }
__inline void			DUMMY_Program_PostProcess(void) { }
__inline void			DUMMY_Program_Enter(const uint32 blockaddr) { }
__inline void			DUMMY_Program_Exit(const uint32 blockaddr) { }
__inline void			DUMMY_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data) { }
__inline STATUS_E		DUMMY_Program_CheckDone(const uint32 prog_addr) { return S_DONE; }
#endif

#endif
