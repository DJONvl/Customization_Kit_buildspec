/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef	_FLASH_DRV_DUMMY_H_
#define _FLASH_DRV_DUMMY_H_

#include "SW_TYPES.H"
#include "flash_dev_tbl.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const NOR_Die_Layout_S		DUMMY_MAP;
extern const NOR_OTP_Layout_S		DUMMY_OTP_MAP;
extern const NOR_CMD_Callback_S		DUMMY_CMD;

#define UNKNOWN_DEVICE_ENTRY	{ NOR_UNKNOWN, { { { &DUMMY_MAP }, { &DUMMY_MAP } }, &DUMMY_OTP_MAP, 0, 0, 0, 0, 0 }, &DUMMY_CMD }

#ifdef USE_GCC
extern __inline bool			DUMMY_CheckDevID(volatile uint16 *die1_addr, volatile uint16 *die2_addr);
extern __inline bool			DUMMY_CheckDevIdle(const uint32 blockaddr);
extern __inline void			DUMMY_Erase(const uint32 blockaddr);
extern __inline STATUS_E		DUMMY_Erase_CheckDone(const uint32 blockaddr);
extern __inline void			DUMMY_Program_PreProcess(void);
extern __inline void			DUMMY_Program_PostProcess(void);
extern __inline void			DUMMY_Program_Enter(const uint32 blockaddr);
extern __inline void			DUMMY_Program_Exit(const uint32 blockaddr);
extern __inline void			DUMMY_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 data);
extern __inline STATUS_E		DUMMY_Program_CheckDone(const uint32 prog_addr);

#else

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

#ifdef __cplusplus
}
#endif

#endif
