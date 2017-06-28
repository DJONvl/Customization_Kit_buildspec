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
#include "flash_drv_8590.h"
#include "hw_config.h"
#include "logging.h"
#include "Timer.h"
#include "kal_release.h"
#include "mtk_status.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SFLASH_POLLINGREG_COUNT     500000
#define SFLASH_WRITECMD_TIMEOUT     100000
#define SFLASH_WRITEBUSY_TIMEOUT    500000
#define SFLASH_ERASESECTOR_TIMEOUT  200000
#define SFLASH_CHIPERASE_TIMEOUT    500000

#define SFLASH_HW_ALIGNMENT         4
#ifndef SFLASH_WRITE_PROTECTION_VAL
  #define SFLASH_WRITE_PROTECTION_VAL 0x00
#endif
#define DUAL_READ 0x01
#define DUAL_IO   0x02
#define QUAD_READ 0x04
#define QUAD_IO   0x08

const NOR_CMD_Callback_S	COMMON_NOR_CMD_CB_PGM = {
	ID_COMMON_NOR_CMD_CB_PGM,
	COMMON_CheckDevID,
	Common_CheckDevIdle,
	Common_Erase_CMD,
	Common_Erase_CheckDone_By_Polling,
	Common_PreProcess,
	Common_Program_PostProcess,
	Common_UnlockBypass_Enter_CMD,
	Common_UnlockBypass_Exit_CMD,
	Common_UnlockBypass_Program_CMD,
	Common_Program_CheckDone_By_Toggle,
	Common_Nor_Buf_Program,
	Common_Nor_Buf_Program_Check_Done
};
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
static SFLASHHW_VENDOR_T _aVendorFlash[] =
{
    { 0x01, 0x02, 0x12, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL004A)" },
    { 0x01, 0x02, 0x13, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL008A)" },
    { 0x01, 0x02, 0x14, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL016A)" },
    { 0x01, 0x02, 0x15, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL032A)" },
    { 0x01, 0x02, 0x16, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL064A)" },
    { 0x01, 0x20, 0x18, 0x0,0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL128P)" },
    { 0x01, 0x02, 0x19, 0x0,0x2000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL256S)" },
	{ 0x01, 0x40, 0x15, 0x0, 0x200000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL016A)" },

    { 0xC2, 0x20, 0x13, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L400)" },
    { 0xC2, 0x20, 0x14, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L80)" },
	{ 0xC2, 0x20, 0x15, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L160)" },
    { 0xC2, 0x24, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L1635D)" },
    { 0xC2, 0x20, 0x16, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L320)" },
    { 0xC2, 0x20, 0x17, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L640)" },
    { 0xC2, 0x20, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L1280)" },
    { 0xC2, 0x5E, 0x16, 0x0, 0x400000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L3235D)" },
	{ 0xC2, 0x20, 0x19, 0x0, 0x2000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3b, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L256)" },
    { 0xC2, 0x20, 0x1A, 0x0, 0x2000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3b, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L512)" },
    { 0xC2, 0x25, 0x39, 0x0, 0x2000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3b, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25U256)" },

    { 0x20, 0x20, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P80)" },
    { 0x20, 0x20, 0x15, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P16)" },
    { 0x20, 0x20, 0x16, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P32)" },
    { 0x20, 0x20, 0x17, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P64)" },
    { 0x20, 0x20, 0x18, 0x0, 0x1000000, 0x40000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P128)" },
    { 0x20, 0x71, 0x16, 0x0, 0x400000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25PX32)" },
    { 0x20, 0x71, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25PX64)" },
    { 0x20, 0xBA, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P64)" },

    { 0xBF, 0x25, 0x41, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00, SFLASH_WRITE_PROTECTION_VAL, "SST(25VF016B)" },
	
    { 0xEF, 0x30, 0x13, 0x0,  0x80000,   0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X40)" },
    { 0xEF, 0x30, 0x14, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X80)" },
    { 0xEF, 0x30, 0x15, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X16)" },
    { 0xEF, 0x30, 0x16, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X32)" },
    { 0xEF, 0x30, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X64)" },
	{ 0xEF, 0x40, 0x19, 0x0, 0x2000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q256FV)" },
    { 0xEF, 0x40, 0x15, 0x0, 0x200000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q16CV)" },
	{ 0xEF, 0x40, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q32BV)" },
    { 0xEF, 0x40, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q64BV)" },
    { 0xEF, 0x40, 0x18, 0x0, 0x1000000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q128BV)" },

    { 0xC8, 0x40, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "GD(GD25Q16BSIG)" },
	{ 0xC8, 0x40, 0x16, 0x0, 0x400000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "GD(GD25Q32BSIG)" },
	{ 0xC8, 0x40, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "GD(GD25Q64BSIG)" },
	{ 0xC8, 0x40, 0x18, 0x0, 0x1000000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "GD(GD25Q128BSIG)" },
#if (defined(CC_MT5360B) || defined(CC_MT5387) ||defined(CC_MT5363) ||defined(ENABLE_AAIWRITE))
    { 0xBF, 0x43, 0x10, 0x0,  0x40000,  0x8000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0x52, 0x60, 0x02, 0xAF, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF020)" },
    { 0xBF, 0x25, 0x8D, 0x0,  0x80000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF040B)" },
    { 0xBF, 0x25, 0x8E, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF080B)" },
    { 0xBF, 0x25, 0x41, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF016B)" },
    { 0xBF, 0x25, 0x4A, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF032B)" },
    { 0xBF, 0x25, 0x4B, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF064C)" },
#else
    { 0xBF, 0x43, 0x10, 0x1,  0x40000,  0x8000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0x52, 0x60, 0x02, 0xAF, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF020)" },
    { 0xBF, 0x25, 0x8D, 0x1,  0x80000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF040B)" },
    { 0xBF, 0x25, 0x8E, 0x1, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF080B)" },
    { 0xBF, 0x25, 0x41, 0x1, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF016B)" },
    { 0xBF, 0x25, 0x4A, 0x1, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF032B)" },
    { 0xBF, 0x25, 0x4B, 0x1, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF064C)" },
#endif

    { 0x1F, 0x47, 0x00, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ATMEL(AT25DF321)" },
    { 0x1F, 0x48, 0x00, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ATMEL(AT25DF641)" },
	{ 0x1F, 0x45, 0x01, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ATMEL(AT26DF081A)" },
    { 0x1C, 0x20, 0x13, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25B40)" },
    { 0x1C, 0x31, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25F80)" },
    { 0x1C, 0x20, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P16)" },
    { 0x1C, 0x20, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P32)" },
    { 0x1C, 0x20, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P64)" },
    { 0x1C, 0x30, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25Q64)" },
    { 0x1C, 0x20, 0x14, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P80/EN25B80)" },
    { 0x7F, 0x37, 0x20, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L40P)" },
    { 0x37, 0x30, 0x13, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L040)" },
    { 0x37, 0x30, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L032)" },

    { 0xFF, 0xFF, 0x10, 0x0,  0x10000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xC7, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "512Kb" },
//    { 0xFF, 0xFF, 0x11, 0x0,  0x20000,  0x8000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "1Mb" },
    { 0xFF, 0xFF, 0x12, 0x0,  0x40000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "2Mb" },
    { 0xFF, 0xFF, 0x13, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "4Mb" },
    { 0xFF, 0xFF, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "8Mb" },
    { 0xFF, 0xFF, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "16Mb" },
    { 0xFF, 0xFF, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "32Mb" },
    { 0xFF, 0xFF, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "64Mb" },
    { 0xFF, 0xFF, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "128Mb" },

    { 0x00, 0x00, 0x00, 0x0, 0x000000, 0x00000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "NULL Device" },
};

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


#define LoWord(d)     ((uint16)(d & 0x0000ffffL))
#define HiWord(d)     ((uint16)((d >> 16) & 0x0000ffffL))
#define LoByte(w)     ((U8)(w & 0x00ff))
#define HiByte(w)     ((U8)((w >> 8) & 0x00ff))
#define MidWord(d)    ((uint16)((d >>8) & 0x00ffff))

#define BYTE0(arg)    (*(U8 *)&arg)
#define BYTE1(arg)    (*((U8 *)&arg + 1))
#define BYTE2(arg)    (*((U8 *)&arg + 2))
#define BYTE3(arg)    (*((U8 *)&arg + 3))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
SFLASHHW_VENDOR_T _arFlashChip[MAX_FLASHCOUNT];

static uint32 _u4ChipCount;


static uint32 _u4SFIsrStatus;
static bool _fgSFIsrEn = TRUE;
#ifdef REPLACE_EEPROM_WITH_NOR
bool _fgDoProtect = FALSE;
#else
static bool _fgDoProtect = FALSE;
bool _fgWriteProtect = TRUE;
#endif
#if (defined(ENABLE_DUALREAD))
static U8 _u1DualReadCmd = 0;
#endif


#if (defined(ENABLE_AAIWRITE))
static bool _fgAAIWrite = TRUE;
#else
static bool _fgAAIWrite = FALSE;
#endif



uint32 _fgNorPartialProtection = 0;

uint32 SZ_OPT_UNIT_SIZE = 0x10000;
static U8 u1MenuID, u1DevID1, u1DevID2;
bool fgProgramPass = FALSE;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _SendFlashCommand(U8 u1Val)
{
    SFLASH_WREG8(SFLASH_CMD_REG, u1Val);
}

//-----------------------------------------------------------------------------
static int32 _PollingReg(U8 u1RegOffset, U8 u8Compare)
{
    uint32 u4Polling;
    U8 u1Reg;

    u4Polling = 0;
    while(1)
    {
        u1Reg = SFLASH_RREG8(u1RegOffset);
        if (0x00 == (u1Reg & u8Compare))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_POLLINGREG_COUNT)
        {
            return 1;
        }
    }

    return 0;
}


#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
//-----------------------------------------------------------------------------
/** _SFLASHHW_Isr()
 */
//-----------------------------------------------------------------------------
static void _SFLASHHW_Isr(uint16 u2VctId)
{
    uint32 u4Val;

 //   UNUSED(u2VctId);
//    VERIFY(BIM_ClearIrq(VECTOR_FLASH));

    u4Val = SFLASH_RREG32(SFLASH_SF_INTRSTUS_REG);
    if (u4Val & 0x7F)
    {
        // Clear interrupt
        SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, u4Val);
        _u4SFIsrStatus = u4Val;
    }
}
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
static void _SetFlashCountReg(uint32 u4Count)
{
    U8 u1Reg;

    u1Reg = SFLASH_RREG8(SFLASH_CFG1_REG);
    u1Reg &= (~0xC);

    if(u4Count == 1)
    {
        SFLASH_WREG8(SFLASH_CFG1_REG, u1Reg);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Count == 2)
    {
        switch(_arFlashChip[0].u4ChipSize)
        {
        case 0x200000:
            u1Reg |= 0xC;
            break;
        case 0x400000:
            u1Reg |= 0x8;
            break;
        case 0x800000:
        default:
            u1Reg |= 0x4;
            break;
        }

        SFLASH_WREG8(SFLASH_CFG1_REG, u1Reg);
    }
#endif
    else
    {
        ASSERT(0);
    }
}

//-----------------------------------------------------------------------------
static int32 _DoIdentify(uint32 u4Index, uint32 *pu4Vendor)
{
    uint32 i;

    if(pu4Vendor == NULL)
    {
        return 1;
    }

    *pu4Vendor = 0xFFFFFFFF;

    if(SFLASHHW_GetID(u4Index, &u1MenuID, &u1DevID1, &u1DevID2) != 0)
    {
        return 1;
    }
	DBG_MSG( "Flash index :%d,MenuID: 0x%X, DeviceID1: 0x%X, DeviceID2: 0x%X\n",
            u4Index,u1MenuID, u1DevID1, u1DevID2);
    i = 0;
	
    while(_aVendorFlash[i].u1MenuID != (U8)0x0)
    {
        if( (_aVendorFlash[i].u1MenuID == u1MenuID) &&
            (_aVendorFlash[i].u1DevID1 == u1DevID1) &&
            (_aVendorFlash[i].u1DevID2 == u1DevID2))
        {
            *pu4Vendor = i;
			
#if(defined(ENABLE_DUALREAD))
            if (_aVendorFlash[i].u1DualREADCmd)
            {      
                _u1DualReadCmd = _aVendorFlash[i].u1DualREADCmd;
                DBG_MSG( "dualread flash _u1DualReadCmd = 0x%x\n",_u1DualReadCmd);
            }
#endif
		#if (defined(ENABLE_AAIWRITE))
            if ( _aVendorFlash[i].u1AAIPROGRAMCmd)
            {
                _fgAAIWrite = TRUE;
                DBG_MSG( "AAIwrite flash\n");
            }
#endif
            return 0;
        }
        i++;
    }

    return 1;
}

//-----------------------------------------------------------------------------
static int32 _WBEnable(void)
{
    uint32 u4Polling;
    U8 u1Reg;
    U8 u1Tmp;
    u1Tmp = 0x01;
    if (_fgAAIWrite == TRUE)
    {
        if(_arFlashChip[0].u1AAIPROGRAMCmd != 0xAF)
        {
          u1Tmp |= 0x40;
        }
    }
    SFLASH_WREG8(SFLASH_CFG2_REG, u1Tmp);

    u4Polling = 0;
    while(1)
    {
        u1Reg = SFLASH_RREG8(SFLASH_CFG2_REG);
        if (0x01 == (u1Reg & 0x1))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_POLLINGREG_COUNT)
        {
            return 1;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
static int32 _WBDisable(void)
{
    uint32 u4Polling;
    U8 u1Reg;
	U8 u1Tmp;
    u1Tmp = 0x00;

    SFLASH_WREG8(SFLASH_CFG2_REG, u1Tmp);
    u4Polling = 0;
    while(1)    // Wait for finish write buffer
    {
        u1Reg = SFLASH_RREG8(SFLASH_CFG2_REG);
        if (u1Tmp == (u1Reg & 0xF))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_POLLINGREG_COUNT)
        {
            return 1;
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
static int32 _WaitForWriteBusy(uint32 u4Index, uint32 u4Timeout)
{
    uint32 u4Count;
    U8 u1Reg;

    u4Count = 0;
    while(1)
    {
        if(SFLASHHW_ReadFlashStatus(u4Index, &u1Reg) != 0)
        {
            return 1;
        }

        if(0 == (u1Reg & 0x1))
        {
            break;
        }

        u4Count++;
        if(u4Count > u4Timeout)
        {
            return 1;
        }

        //x_thread_delay(5);
    }

    return 0;
}

//-----------------------------------------------------------------------------
static int32 _SetFlashWriteEnable(uint32 u4Index)
{

    SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1WRENCmd);
    SFLASH_WREG8(SFLASH_CNT_REG,8); // Write SF Bit Count

	if(u4Index==0)
    {
        _SendFlashCommand(0x4);	// Write PRG

    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);	// Write PRG
    }
#endif
    else
    {
        ASSERT(0);
    }

    if( _PollingReg(SFLASH_CMD_REG, 0x04) !=0) // timeout
    {
        return 1;
    }
	
    return 0;
}



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_Init.
 *  Details of SFLASHHW_Init (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------

int32 SFLASHHW_Init(void)
{
    uint32 u4Loop, u4VendorIdx;
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
    void (* pfnOldIsr)(uint16);
#endif /* CC_MTK_LOADER */

    memset((void*)_arFlashChip, 0x0, MAX_FLASHCOUNT*sizeof(SFLASHHW_VENDOR_T));

    _u4ChipCount = 0;
    for(u4Loop = 0; u4Loop < MAX_FLASHCOUNT; u4Loop++)
    {
        if(_DoIdentify(u4Loop, &u4VendorIdx) != 0)
        {
            continue;
        }

        memcpy((void*)&(_arFlashChip[_u4ChipCount]),
                 (void*)&(_aVendorFlash[u4VendorIdx]),
                 sizeof(SFLASHHW_VENDOR_T));

        //VERIFY(NULL != x_strncpy(_arFlashChip[_u4ChipCount].pcFlashStr, _aVendorFlash[u4VendorIdx].pcFlashStr, x_strlen(_aVendorFlash[u4VendorIdx].pcFlashStr)));
        _u4ChipCount++;
        DBG_MSG( "Detect flash #%d: %s\n", u4Loop, _arFlashChip[u4Loop].pcFlashStr);
    }
   
    if(_u4ChipCount == 0)
    {
        DBG_MSG( "There is no flash!\n");
        return 1;
    }

    //--------------------------------------------------
    // Set flash number register
    //--------------------------------------------------
    _SetFlashCountReg(_u4ChipCount);

	//set clock
//    CKGEN_WRITE32(REG_RW_CLK_CFG0, CKGEN_READ32(REG_RW_CLK_CFG0) | 0x3000);//54mhz
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
    //--------------------------------------------------
    // Register ISR
    //--------------------------------------------------
    SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x50); //enable pp Interrupt & aai
    //VERIFY(BIM_ClearIrq(VECTOR_FLASH));

    //VERIFY(x_reg_isr(VECTOR_FLASH, _SFLASHHW_Isr, &pfnOldIsr) == OSR_OK);
#endif /* CC_MTK_LOADER */

    _fgSFIsrEn = FALSE;
    _u4SFIsrStatus = 0;

	//Guarantee every time nor init can WriteProtect
#ifdef REPLACE_EEPROM_WITH_NOR
    _fgDoProtect = TRUE;
    if(_fgDoProtect)
    {
    	for(u4Loop=0; u4Loop < _u4ChipCount; u4Loop++)
    	{
    		if(SFLASHHW_WriteProtect(u4Loop, TRUE) != 0)
    		{
    			return -1;
    		}
    	}
    }
#else
	_fgWriteProtect = TRUE;
	
	if(_fgWriteProtect)
	{
        _fgDoProtect = TRUE;
        if(_fgDoProtect)
        {
            for(u4Loop=0; u4Loop < _u4ChipCount; u4Loop++)
            {
                if(SFLASHHW_WriteProtect(u4Loop, TRUE) != 0)
                {
                    return -1;
                }
            }
        }
	}
#endif

       g_FlashDev.m_device_id = u4VendorIdx;
       g_FlashDev.m_hw_info.m_dev_code = _arFlashChip[0].u1DevID1;
       g_FlashDev.m_hw_info.m_ext_dev_code1 = _arFlashChip[0].u1DevID1;
       g_FlashDev.m_hw_info.m_ext_dev_code2 = _arFlashChip[0].u1DevID2;
       g_FlashDev.m_hw_info.m_manufacture_code =  _arFlashChip[0].u1MenuID;
       g_FlashDev.m_hw_info.m_die[0].m_die_layout.m_region_map[0].m_base_offset    =  0x30000000;
       g_FlashDev.m_hw_info.m_die[0].m_die_layout.m_region_map[0].m_sector_count  = _arFlashChip[0].u4ChipSize/_arFlashChip[0].u4SecSize;
       g_FlashDev.m_hw_info.m_die[0].m_die_layout.m_region_map[0].m_sector_size    = _arFlashChip[0].u4SecSize;     
       g_FlashDev.m_hw_info.m_die[0].m_die_layout.m_size = _arFlashChip[0].u4ChipSize;

       g_HW_DetectionResult.m_nor_chip_select[0] = CS_0;
       g_HW_DetectionResult.m_nor_chip_select[1] = CS_WITH_DECODER;
       g_HW_DetectionResult.m_nor_flash_id    =    u4VendorIdx;
       g_HW_DetectionResult.m_nor_flash_size = _arFlashChip[0].u4ChipSize;
       g_HW_DetectionResult.m_nor_flash_dev_code_1 = _arFlashChip[0].u1MenuID;
       g_HW_DetectionResult.m_nor_flash_dev_code_2 = _arFlashChip[0].u1DevID1;
       g_HW_DetectionResult.m_nor_flash_dev_code_3 = _arFlashChip[0].u1DevID2;
       g_HW_DetectionResult.m_nor_flash_dev_code_4 = 0;
       g_HW_DetectionResult.m_nor_flash_otp_status = 0;
       g_HW_DetectionResult.m_nor_flash_otp_size = 0;
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_ReadFlashStatus.
 *  Details of SFLASHHW_ReadFlashStatus (optional).
 *  @retval 1    Success
 *  @retval 0   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_ReadFlashStatus(uint32 u4Index, U8 *pu1Val)
{
    if(pu1Val == NULL)
    {
        return 1;
    }
#ifdef REPLACE_EEPROM_WITH_NOR
	if((0xEF ==_arFlashChip[u4Index].u1MenuID) && (0x40 ==_arFlashChip[u4Index].u1DevID1) && (0x17 ==_arFlashChip[u4Index].u1DevID2))
	{
		//cmd 05 read nor first status register
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	// Write
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	// Write
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	// Write
		SFLASH_WREG8(SFLASH_CNT_REG, 24);			// Write SF Bit Count

		if(u4Index == 0)
		{
			_SendFlashCommand(0x4);
		}
		if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
		{
			return -1;
		}
		*pu1Val = SFLASH_RREG8(SFLASH_SHREG0_REG);
	}
	else
	{
		if(u4Index==0)
		{
			_SendFlashCommand(0x2);
		}
		else
		{
			ASSERT(0);
		}
		
		if(_PollingReg(SFLASH_CMD_REG, 0x2) != 0)
		{
			return -1;
		}
		
		*pu1Val = SFLASH_RREG8(SFLASH_RDSR_REG);
	}
#else

    if(u4Index==0)
    {
        _SendFlashCommand(0x2);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x42);
    }
#endif
    else
    {
        ASSERT(0);
    }

    if(_PollingReg(SFLASH_CMD_REG, 0x2) != 0)
    {
        return 1;
    }

    *pu1Val = SFLASH_RREG8(SFLASH_RDSR_REG);
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetIsr.
 *  Details of SFLASHHW_SetIsr (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetIsr(bool fgSet)
{
    if (fgSet)
    {
        /* enable irq. */
        SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x3F);
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x3F);
        //VERIFY(BIM_ClearIrq(VECTOR_FLASH));
        //VERIFY(BIM_EnableIrq(VECTOR_FLASH));
        _fgSFIsrEn = TRUE;
    }
    else
    {
        /* disable irq. */
        //VERIFY(BIM_DisableIrq(VECTOR_FLASH));
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0);
        SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x3F);
        //VERIFY(BIM_ClearIrq(VECTOR_FLASH));
        _fgSFIsrEn = FALSE;
    }
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SwitchChip.
 *  Details of SFLASHHW_SwitchChip ().
 */
//-----------------------------------------------------------------------------
uint32 SFLASHHW_SwitchChip(uint32 uAddr)
{
uint32 uDualReg = SFLASH_RREG32(SFLASH_READ_DUAL_REG);
    if(uAddr == 0)
    {
       uDualReg &= 0xFFFFFF7F;
       SFLASH_WREG32(SFLASH_READ_DUAL_REG, uDualReg);
    }
    else if(uAddr == 1)
    {
       uDualReg |= 0x80;
       SFLASH_WREG32(SFLASH_READ_DUAL_REG, uDualReg);
    }
    else
    {
       return SFLASH_RREG32(uAddr);
    }
    return 0;
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_Ewsr.
 *  Details
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_Send2Byte(uint32 u4Index, U8 u1Data1, U8 u1Data2)
{
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, u1Data1);
    SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1Data2);
    SFLASH_WREG8(SFLASH_CNT_REG,0x10); // Write SF Bit Count
    
    SFLASHHW_SwitchChip(u4Index);
    if(u4Index==0)
    {
        _SendFlashCommand(0x4); // Write PRG

    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);    // Write PRG
    }
#endif
    else
    {
        ASSERT(0);
    }

    if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
    {
			SFLASHHW_SwitchChip(0);

        return -1;
    }
		SFLASHHW_SwitchChip(0);
    return 0;

}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SendByte.
 *  Details
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_SendByte(uint32 u4Index, U8 u1Data)
{
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, u1Data);
    SFLASH_WREG8(SFLASH_CNT_REG,8); // Write SF Bit Count

		SFLASHHW_SwitchChip(u4Index);

    if(u4Index==0)
    {
        _SendFlashCommand(0x4); // Write PRG

    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);    // Write PRG
    }
#endif
    else
    {
        ASSERT(0);
    }

    if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
    {
			SFLASHHW_SwitchChip(0);

        return -1;
    }
		SFLASHHW_SwitchChip(0);

    return 0;

}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetSstID.
 *  Details of SFLASHHW_Identify (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_GetSstID(uint32 u4Index, U8 *pu1MenuID, U8 *pu1DevID1, U8 *pu1DevID2)
{
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x90);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA2_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA1_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA0_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_CNT_REG, 0x30);           // Write SF Bit Count

    if(u4Index == 0)
    {
        _SendFlashCommand(0x4);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);
    }
#else
    else
    {
        ASSERT(0);
    }
#endif

    if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
    {
        return -1;
    }

    if(pu1DevID2 != NULL)
    {
        *pu1DevID2 = 0x10;
    }

    if(pu1DevID1 != NULL)
    {
        *pu1DevID1 = SFLASH_RREG8(SFLASH_SHREG0_REG);
    }

    if(pu1MenuID != NULL)
    {
        *pu1MenuID = SFLASH_RREG8(SFLASH_SHREG1_REG);
    }

    _SendFlashCommand(0x0);
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetID.
 *  Details of SFLASHHW_Identify (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_GetID(uint32 u4Index, U8 *pu1MenuID, U8 *pu1DevID1, U8 *pu1DevID2)
{

	DBG_MSG( "Try SFLASHHW_GetID\n");
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x9F);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA2_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_CNT_REG, 32);           // Write SF Bit Count

    if(u4Index == 0)
    {
        _SendFlashCommand(0x4);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);
    }
#else
    else
    {
        ASSERT(0);
    }
#endif

    if( _PollingReg(SFLASH_CMD_REG, 0x04) !=0) // timeout
    {
        return 1;
    }
    if(pu1DevID2 != NULL)
    {
        *pu1DevID2 = SFLASH_RREG8(SFLASH_SHREG0_REG);
    }

    if(pu1DevID1 != NULL)
    {
        *pu1DevID1 = SFLASH_RREG8(SFLASH_SHREG1_REG);
    }

    if(pu1MenuID != NULL)
    {
        *pu1MenuID = SFLASH_RREG8(SFLASH_SHREG2_REG);
    }

    _SendFlashCommand(0x0);
    if((pu1MenuID != NULL) && (pu1DevID1 != NULL))
    {
    if((*pu1MenuID == 0xFF) || (*pu1DevID1 == 0x00))
    {
       DBG_MSG( "Try not JEDEC ID\n");
       if(SFLASHHW_GetSstID(u4Index, pu1MenuID, pu1DevID1, pu1DevID2) != 0)
      {
          return -1;
      }
    }
    }
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetFlashInfo.
 *  Details of SFLASHHW_GetInfo (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
void SFLASHHW_GetFlashInfo(SFLASH_INFO_T *prInfo)
{
    uint32 i;

    if(prInfo == NULL)
    {
        return;
    }

    prInfo->u1FlashCount = (U8)(_u4ChipCount & 0xFF);
    for(i=0; i<_u4ChipCount; i++)
    {
        prInfo->arFlashInfo[i].u1MenuID = _arFlashChip[i].u1MenuID;
        prInfo->arFlashInfo[i].u1DevID1 = _arFlashChip[i].u1DevID1;
        prInfo->arFlashInfo[i].u1DevID2 = _arFlashChip[i].u1DevID2;
        prInfo->arFlashInfo[i].u4ChipSize = _arFlashChip[i].u4ChipSize;
        prInfo->arFlashInfo[i].u4SecSize = _arFlashChip[i].u4SecSize;
        prInfo->arFlashInfo[i].u4SecCount =
                                _arFlashChip[i].u4ChipSize / _arFlashChip[i].u4SecSize;

        //VERIFY(NULL != x_strcpy(prInfo->arFlashInfo[i].pcFlashStr,
                            //_arFlashChip[i].pcFlashStr));
        memcpy(prInfo->arFlashInfo[i].pcFlashStr, _arFlashChip[i].pcFlashStr, SFLASHHWNAME_LEN);
    }

}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_Read.
 *  Details of SFLASHHW_Read (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_Read(uint32 u4Addr, uint32 u4Len, U8* pu1buf)
{
    uint32 i;
	U8 _u1FlagVaule;
//	HAL_TIME_T time1={0}, time2={0}, result_time={0};
//	HAL_GetTime(&time1);
    if(pu1buf == NULL)
    {
        return 1;
    }
    SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);    // Disable pretch write buffer

    SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting
    _u1FlagVaule = IO_READ8(0xCA000000, 0);
    if(_u1FlagVaule == 1)
    {
        DBG_MSG( "...cdd SFLASHHW_Read _u1DualReadCmd is 0x3B u4Len = 0x%x!\n",u4Len);
        SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x3b);
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x1);
    }
    else if(_u1FlagVaule == 2)
    {
        DBG_MSG( "...cdd SFLASHHW_Read _u1DualReadCmd is 0xBB u4Len = 0x%x!\n",u4Len);
        SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0xbb);
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x3);
    }
    else if(_u1FlagVaule == 3)
    {
        _SetFlashEnter4BitRead();
        DBG_MSG( "...cdd SFLASHHW_Read _u1QuadReadCmd is 0x6B u4Len = 0x%x!\n",u4Len);
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x6B);
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x4);
    }
    else if(_u1FlagVaule == 4)
    {
        _SetFlashEnter4BitRead();
        DBG_MSG( "...cdd SFLASHHW_Read _u1QuadReadCmd is 0xEB u4Len = 0x%x!\n",u4Len);
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0xEB);	
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0xc);
    }
    else
    {
        //DBG_MSG( "...cdd SFLASHHW_Read NULL _u1DualReadCmd!\n");
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x0);
    }
   
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Set Addr
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));
    
    for (i=0; i<u4Len; i++)
    {
#ifdef USE_SECOND_FLASH
		if(u4Addr >= _arFlashChip[0].u4ChipSize)
		{
        	_SendFlashCommand(0xC1);    // Read and autoinc address, use second flash
		}
		else
		{
			_SendFlashCommand(0x81);  // Read and autoinc address
		}
#else
        _SendFlashCommand(0x81);  // Read and autoinc address
#endif

        if( _PollingReg(SFLASH_CMD_REG, 0x01) !=0) // timeout
        {
            DBG_MSG( "...cdd SFLASHHW_Read timeout !\n");
            return 1;
        }

		#ifdef ENABLE_CONTROLLER_STATUS
		DBG_MSG("Serial flash control starus:0x87b4=0x%08x!\n",SFLASH_RREG32(SFLASH_SAMPLE_EDGE_REG));
		#endif
        pu1buf[i] = SFLASH_RREG8(SFLASH_RDATA_REG); // Get data
    }

	
	SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_4BitRead.
 *  Details of SFLASHHW_4BitRead (optional) -----support 4 bit mode.
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_4BitRead(uint32 u4Addr, uint32 u4Len, U8* pu1buf)
{
    uint32 i;
	U8 _u1FlagVaule;
//	HAL_TIME_T time1={0}, time2={0}, result_time={0};
//	HAL_GetTime(&time1);

    if(pu1buf == NULL)
    {
        return 1;
    }
    SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);    // Disable pretch write buffer
#if 0
#if (defined(ENABLE_DUALREAD))
			if (_u1DualReadCmd == 0x3B)
			{
				DBG_MSG( "SFLASHHW_Read _u1DualReadCmd is 0x3B!\n");
				SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x1);
			}
			else if (_u1DualReadCmd == 0xBB)
			{
				DBG_MSG( "SFLASHHW_Read _u1DualReadCmd is 0xBB!\n");
				SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x3);
			}
			else
			{
				DBG_MSG( "SFLASHHW_Read NULL _u1DualReadCmd!\n");
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x0);
			}
#endif	
#else 
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting
		_u1FlagVaule = IO_READ8(0x83C00000, 0);
		if(_u1FlagVaule == 1)
		{
			DBG_MSG( "...cdd SFLASHHW_Read _u1DualReadCmd is 0x3B u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x3b);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x1);
		}
		else if(_u1FlagVaule == 2)
		{
			DBG_MSG( "...cdd SFLASHHW_Read _u1DualReadCmd is 0xBB u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0xbb);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x3);
		}
		else if(_u1FlagVaule == 3)
		{
			_SetFlashEnter4BitRead();
			DBG_MSG( "...cdd SFLASHHW_Read _u1QuadReadCmd is 0x6B u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x6B);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x4);
		}
		else if(_u1FlagVaule == 4)
		{
			_SetFlashEnter4BitRead();
			DBG_MSG( "...cdd SFLASHHW_Read _u1QuadReadCmd is 0xEB u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0xEB);	
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0xc);
		}
		else
		{
			//DBG_MSG( "...cdd SFLASHHW_Read NULL _u1DualReadCmd!\n");
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x0);
		}
#endif

    SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Set Addr
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));
	DBG_MSG( "...cdd SFLASHHW_4BitRead u4Addr = 0x%x!\n", u4Addr);

    for (i=0; i<u4Len; i++)
    {
        _SendFlashCommand(0x81);  // Read and auto inc address

        if( _PollingReg(SFLASH_CMD_REG, 0x01) == 1) // timeout
        {
            return 1;
        }

        pu1buf[i] = SFLASH_RREG8(SFLASH_RDATA_REG); // Get data
		if(( i==0 )||(i == 5))
		DBG_MSG( "...cdd SFLASHHW_4BitRead pu1buf[%d] = 0x%x\n",i,pu1buf[i]);
    }
	//HAL_GetTime(&time2);
    //HAL_GetDeltaTime(&result_time, &time1, &time2);
//DBG_MSG( "read 0x%XBytes elapse time: %d sec, %d usec\n", u4Len,
   //                                 result_time.u4Seconds, result_time.u4Micros);
	
    SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_4BBRead.
 *  Details of SFLASHHW_4BBRead (optional) -----support 4 byte mode and 4 bit mode.
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_4Byte4BitRead(uint32 u4Addr, uint32 u4Len, U8* pu1buf)
{
    uint32 i;
	U8 _u1FlagVaule;
	//HAL_TIME_T time1={0}, time2={0}, result_time={0};
	//HAL_GetTime(&time1);

    if(pu1buf == NULL)
    {
        return 1;
    }
    SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);    // Disable pretch write buffer
#if 0
#if (defined(ENABLE_DUALREAD))
			if (_u1DualReadCmd == 0x3B)
			{
				DBG_MSG( "SFLASHHW_Read _u1DualReadCmd is 0x3B!\n");
				SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x1);
			}
			else if (_u1DualReadCmd == 0xBB)
			{
				DBG_MSG( "SFLASHHW_Read _u1DualReadCmd is 0xBB!\n");
				SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x3);
			}
			else
			{
				DBG_MSG( "SFLASHHW_Read NULL _u1DualReadCmd!\n");
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x0);
			}
#endif	
#else 
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting
		_u1FlagVaule = IO_READ8(0x83C00000, 0);
		if(_u1FlagVaule == 1)
		{
			DBG_MSG( "...cdd SFLASHHW_Read _u1DualReadCmd is 0x3B(dual data read) u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x3b);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x1);
		}
		else if(_u1FlagVaule == 2)
		{
			DBG_MSG( "...cdd SFLASHHW_Read _u1DualReadCmd is 0xBB(2 IO) u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0xbb);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x3);
		}
		else if(_u1FlagVaule == 3)
		{
			_SetFlashEnter4BitRead();
			DBG_MSG( "...cdd SFLASHHW_Read _u1QuadReadCmd is 0x6B(quad data read) u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x6B);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x4);
		}
		else if(_u1FlagVaule == 4)
		{
			_SetFlashEnter4BitRead();
			DBG_MSG( "...cdd SFLASHHW_Read _u1QuadReadCmd is 0xEB(4 IO) u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0xEB);	
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0xc);
		}
		else
		{
			//DBG_MSG( "...cdd SFLASHHW_Read NULL _u1DualReadCmd!\n");
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x0);
		}
#endif

	//if(_SetFlashEnter4Byte()!=0)
	//	return -1;

    SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Set Addr
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));
	DBG_MSG( "...cdd SFLASHHW_4Byte4BitRead u4Addr = 0x%x!\n", u4Addr);

    for (i=0; i<u4Len; i++)
    {
        _SendFlashCommand(0x81);  // Read and auto inc address

        if( _PollingReg(SFLASH_CMD_REG, 0x01) == 1) // timeout
        {
            return 1;
        }

        pu1buf[i] = SFLASH_RREG8(SFLASH_RDATA_REG); // Get data
		if(( i==0 )||(i == 5))
		DBG_MSG( "...cdd SFLASHHW_4Byte4BitRead pu1buf[%d] = 0x%x\n",i,pu1buf[i]);
    }
	//HAL_GetTime(&time2);
    //HAL_GetDeltaTime(&result_time, &time1, &time2);
    //DBG_MSG( "read 0x%XBytes elapse time: %d sec, %d usec\n", u4Len,
     //                               result_time.u4Seconds, result_time.u4Micros);
    SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//exit read mode setting
    return 0;
}


//-----------------------------------------------------------------------------
static int32 _ExecuteWriteCmd(uint32 u4Index)
{
    //HAL_TIME_T rOrgTime = {0}, rNewTime = {0}, rDeltaTime = {0};
    U8 u1Reg;

    if(_fgSFIsrEn)
    {
        _u4SFIsrStatus = 0;
		if (_fgAAIWrite == FALSE)
        {
          SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x10);    // Clear interrupt
          SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x10);
		}
		else
	    {
          SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x40);    // Clear interrupt
          SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x40);
		}
    }
		SFLASHHW_SwitchChip(u4Index);
    if(u4Index == 0)
    {
        if (_fgAAIWrite == FALSE)
        {
           _SendFlashCommand(0x10);
        }
        else
        {
            SFLASH_WREG8(SFLASH_CMD2_REG, 0x01);
        }
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        if (_fgAAIWrite == FALSE)
        {
            _SendFlashCommand(0x50);
        }
        else
        {
            SFLASH_WREG8(SFLASH_CMD2_REG, 0x01);
        }
    }
#endif
    else
    {
        ASSERT(0);
    }

    //-------------------------
    if(_fgSFIsrEn)
    {
        while(((_fgAAIWrite == FALSE)&&(!(_u4SFIsrStatus & 0x10))) 
			  || ((_fgAAIWrite == TRUE)&&(!(_u4SFIsrStatus & 0x40))))
        {
#ifndef CC_5381_LOADER
            //if(x_sema_lock_timeout(_hSFLASHSema, SFLASH_WRITECMD_TIMEOUT) == OSR_TIMEOUT)
            {
               // SFLASHHW_SwitchChip(0);
//return 1;
            }
#endif /* CC_5381_LOADER */
        }
    }
    else
    {
        //HAL_GetTime(&rOrgTime);
        while(1)
        {
            if (_fgAAIWrite == FALSE)
            {
               u1Reg = SFLASH_RREG8(SFLASH_CMD_REG);
               if (0x0 == (u1Reg & 0x10))
               {
                  break;
               }
            }
            else
            {
                u1Reg = SFLASH_RREG8(SFLASH_CMD2_REG);
                if (0x0 == (u1Reg & 0x01))
                {
                    break;
                }
			
            }

           // HAL_GetTime(&rNewTime);
           // HAL_GetDeltaTime(&rDeltaTime, &rOrgTime, &rNewTime);
           // if (rDeltaTime.u4Seconds > SFLASH_WRITECMD_TIMEOUT)
            {
             //   return 1;
            }
            //x_thread_delay(10);
        }
    }

    if(_fgSFIsrEn)
    {
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x0);   // Disable interrupt
    }

    _SendFlashCommand(0x0);
		SFLASHHW_SwitchChip(0);
    return 0;
}

//-----------------------------------------------------------------------------
static int32 _WriteBuffer(uint32 u4Index, uint32 u4Addr, uint32 u4Len, const U8* pu1Buf)
{
    uint32 i, j, u4BufIdx, u4Data;

    if(pu1Buf == NULL)
    {
        return 1;
    }

    ASSERT(u4Len <= SFLASH_WRBUF_SIZE);
    ASSERT((u4Addr%SFLASH_HW_ALIGNMENT) == 0);
    ASSERT((u4Len%SFLASH_HW_ALIGNMENT) == 0);

   
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr))); // Write
    
    u4BufIdx = 0;
    for(i=0; i<u4Len; i+=4)
    {
        for(j=0; j<4; j++)
        {
            (*((U8 *)&u4Data + j)) = pu1Buf[u4BufIdx];
            u4BufIdx++;
        }
        SFLASH_WREG32(SFLASH_PP_DATA_REG, u4Data);
     }

    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
	if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
	{
	    return 1;
	}


    return 0;
}


//-----------------------------------------------------------------------------
static int32 _WriteSingleByte(uint32 u4Index, uint32 u4Addr, U8 u1Data)
{
   
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));
   
    SFLASH_WREG8(SFLASH_WDATA_REG, u1Data);

    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteSector.
 *  Details of SFLASHHW_WriteSector (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_WriteSector(uint32 u4Index, uint32 u4Addr, uint32 u4Len,
                           const U8* pu1Buf)
{
    uint32 i, u4Count, u4PgAlign;
	  if(u4Index >= MAX_FLASHCOUNT)
	  {
       DBG_MSG( "Nor Flash Index is out fo Max Flash Count\n");
       return -1;	  	
	  }
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);
    ASSERT(u4Len <= _arFlashChip[u4Index].u4SecSize);

    if(u4Len == 0)
    {
        return 0;
    }

    if(pu1Buf == NULL)
    {
        return 1;
    }

    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

    // Handle cross page
    //sst don't use AAI write singlebyte,so if handle cross page,disable AAIwrite,
    
    u4PgAlign = u4Addr % SFLASH_WRBUF_SIZE;
    if(u4PgAlign != 0)
    {
    	_fgAAIWrite = FALSE;
        for(i=0; i<(SFLASH_WRBUF_SIZE - u4PgAlign); i++)
        {
            if(_WriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                return 1;
            }
            u4Addr++;
            pu1Buf++;
            u4Len--;

            if(u4Len == 0)
            {
                return 0;
            }
        }
    }

	//After handle cross page,we must reenble AAIwrite
	#if (defined(ENABLE_AAIWRITE))
	_fgAAIWrite = TRUE;
    #endif
    if(_WBEnable() != 0)
    {
        return 1;
    }
    if(_arFlashChip[u4Index].u1PPType ==0)
    {
    while((int32)u4Len > 0)
    {
        if(u4Len >= SFLASH_WRBUF_SIZE)
        {
            u4Count = SFLASH_WRBUF_SIZE;
        }
        else
        {
            // Not write-buffer alignment
            break;
        }

        if(_WriteBuffer(u4Index, u4Addr, u4Count, pu1Buf) != 0)
        {
            DBG_MSG( "Write flash error!! faddr = 0x%x, len = 0x%x\n", u4Addr, u4Count);
            if(_WBDisable() != 0)
            {
                return 1;
            }
            return 1;
        }
        u4Len -= u4Count;
        u4Addr += u4Count;
        pu1Buf += u4Count;

    }

    if(_WBDisable() != 0)
    {
        return 1;
    }
    }
    if((int32)u4Len > 0)
    {
    	_fgAAIWrite = FALSE;
        for(i=0; i<u4Len; i++)
        {
            if(_WriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                if(_WBDisable() != 0)
                {
                    return 1;
                }
                return 1;
            }
            u4Addr++;
            pu1Buf++;
        }
    }
	//After handle cross page,we must reenble AAIwrite
	#if (defined(ENABLE_AAIWRITE))
	_fgAAIWrite = TRUE;
    #endif

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_EraseSector.
 *  Details of SFLASHHW_EraseSector (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_EraseSector(uint32 u4Index, uint32 u4Addr)
{
    uint32 u4Polling;
    U8 u1Reg;

	  if(u4Index >= MAX_FLASHCOUNT)
	  {
       DBG_MSG( "Nor Flash Index is out fo Max Flash Count\n");
       return -1;	  	
	  }
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

    // Execute sector erase command
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1SECERASECmd);
    SFLASH_WREG8(SFLASH_PRGDATA4_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA2_REG, LoByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_CNT_REG, 32);       // Write SF Bit Count
  
#ifdef USE_FLASH_ERASE_ISR	//use program interrupt for erase
	//DBG_MSG("SFLASHHW_EraseSector USE_FLASH_ERASE_ISR......... ");
	u1Reg = SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG);
	SFLASH_WREG8(SFLASH_SF_INTRSTUS_REG, u1Reg);	// Clear interrupt
	SFLASH_WREG8(SFLASH_SF_INTREN_REG, 0x04);				
	if(u4Index==0)
	{
		_SendFlashCommand(0xc);
	}
#ifdef USE_SECOND_FLASH
	else if(u4Index == 1)
	{
		_SendFlashCommand(0x4c);
	}
#endif
	else
	{
		ASSERT(0);
	}
		u4Polling = 0;	
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
	//if(x_sema_lock_timeout(_hSFLASHSema, SFLASH_WRITECMD_TIMEOUT) == OSR_TIMEOUT)
	{
	//	return -1;
	}
	
#else
		
	while(1)
	{
		u1Reg = SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG);
		if(4 == (u1Reg & 0x4))
		{
			break;
		}
	
		u4Polling++;
		if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
		{
			return -1;
		}
	
		x_thread_delay(100);
	}
#endif

#else					//use cpu polling							
    if(u4Index==0)
    {
        _SendFlashCommand(0x4);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);
    }
#endif
    else
    {
        ASSERT(0);
    }

    // Can not use ISR mode, because there is not erase sector interrupt
    u4Polling = 0;
    while(1)
    {
        if(SFLASHHW_ReadFlashStatus(u4Index, &u1Reg) != 0)
        {
				SFLASHHW_SwitchChip(0);
            return 1;
        }

        if(0 == (u1Reg & 0x1))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
        {
				SFLASHHW_SwitchChip(0);

            return 1;
        }

        mdelay(10);
    }

#endif
    _SendFlashCommand(0x0);
		SFLASHHW_SwitchChip(0);
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_EraseChip.
 *  Details of SFLASHHW_EraseChip (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_EraseChip(uint32 u4Index)
{
   // HAL_TIME_T rOrgTime, rNewTime, rDeltaTime;
    U8 u1Reg;

    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);

    if(u4Index >= _u4ChipCount)
    {
        DBG_MSG( "Flash chip not exist for %d\n", u4Index);
        return 1;
    }

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

    if(_fgSFIsrEn)
    {
        _u4SFIsrStatus = 0;
        SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x8);    // Clear interrupt
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x8);
    }

    // Execute bulk erase command
    if(u4Index == 0)
    {
        _SendFlashCommand(0x8);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x48);
    }
#endif
    else
    {
        ASSERT(0);
    }

    if(_fgSFIsrEn)
    {
        while(!(_u4SFIsrStatus & 0x8))
        {
#ifndef CC_5381_LOADER
           // if(x_sema_lock_timeout(_hSFLASHSema, SFLASH_CHIPERASE_TIMEOUT) == OSR_TIMEOUT)
           // {
            //    return 1;
           // }
#endif /* CC_5381_LOADER */
        }
    }
    else
    {
        //HAL_GetTime(&rOrgTime);
        while(1)
        {
            u1Reg = SFLASH_RREG8(SFLASH_CMD_REG);
            if (0x0 == (u1Reg & 0x8))
            {
                break;
            }

            //HAL_GetTime(&rNewTime);
            //HAL_GetDeltaTime(&rDeltaTime, &rOrgTime, &rNewTime);
            //if (rDeltaTime.u4Seconds > (SFLASH_CHIPERASE_TIMEOUT/1000))
            //{
            //    return 1;
            //}

            //x_thread_delay(1000);
        }
    }

    if(_fgSFIsrEn)
    {
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x0);   // Disable interrupt
    }

    _SendFlashCommand(0x0);


    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_CalCRC.
 *  Details of SFLASHHW_CalCRC (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
static U8 _u1ReadData = 0;
int32 SFLASHHW_CalCRC(uint32 u4Addr, uint32 u4Len, uint32* pu4CheckSum)
{
	uint32 u4Loop;
    U8 *pu1Buf;
	//if _u1ReadData defined here will be fail. becaust the variable will not real use, the system will optimizer the code, and then will not do the operation read data from nor
	//U8  _u1ReadData;
	//UNUSED(_u1ReadData);
    if(pu4CheckSum == NULL)
    {
        return 1;
    }

	SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);
    SFLASH_WREG8(SFLASH_CHKSUM_CTL_REG, 0x00);
	SFLASH_WREG32(SFLASH_CHECKSUM_REG, 0x0);

    SFLASH_WREG8(SFLASH_CHKSUM_CTL_REG, 0x1);  // start caculate CRC

	pu1Buf = (U8*)(u4Addr + SFLASH_MEM_BASE);
	for(u4Loop=0; u4Loop<u4Len; u4Loop++)
    {
		_u1ReadData = *pu1Buf;
        pu1Buf++;
    }

    SFLASH_WREG8(SFLASH_CHKSUM_CTL_REG, 0x0);   // stop calculate CRC

    *pu4CheckSum = SFLASH_RREG32(SFLASH_CHECKSUM_REG);
	

    return 0;
}


//-----------------------------------------------------------------------------
/*
*Enter 4 bytes address mode
*return 0 success, return others fial
*/
int32 _SetFlashEnter4Byte(void)
{
	uint32 u4Index, u4DualReg, u4Polling;
	u4Index = 0;
	if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }	
	
    if( (u1MenuID == 0x01) &&(u1DevID1 == 0x02) &&(u1DevID2 == 0x19) )	//for spansion S25FL256s flash
    {
        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x17);	//Enter EN4B cmd
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x80);	
        SFLASH_WREG8(SFLASH_CNT_REG,16); 			// Write SF Bit Count
    }
    else
    {
        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0xb7);	//Enter EN4B cmd
        SFLASH_WREG8(SFLASH_CNT_REG,8); 			// Write SF Bit Count
    }
    _SendFlashCommand(0x4);	// Write PRG
    if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
    {
        return 1;
    }	
	
	u4DualReg = SFLASH_RREG32(SFLASH_READ_DUAL_REG); 
	u4DualReg |= 0x10;
	SFLASH_WREG32(SFLASH_READ_DUAL_REG, u4DualReg);

    u4Polling = 0;
    while(1)
    {
        u4DualReg = SFLASH_RREG32(SFLASH_READ_DUAL_REG); 
        if (0x10 == (u4DualReg & 0x10))
        {
            break;
        }
        u4Polling++;
        if(u4Polling > SFLASH_POLLINGREG_COUNT)
        {
            return 1;
        }
    }	
	DBG_MSG( "...cdd Enter 4 bytes address!\n");
	return 0;
}


//-----------------------------------------------------------------------------
/*
*Enter 4 bit read mode
*return 0 success, return others fial
*/
int32 _SetFlashEnter4BitRead(void)
{
	uint32 u4Index;
	U8  u1status, u1flag, u1count = 0;
	u4Index = 0;
	if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }		
	DBG_MSG( "... u1MenuID = 0x%x u1DevID1 = 0x%x u1DevID2 = 0x%x!\n",u1MenuID,u1DevID1,u1DevID2);
	//if( (u1MenuID == 0x01) &&(u1DevID1 == 0x20) &&(u1DevID2 == 0x18))
	if( (u1MenuID == 0x01) &&(u1DevID1 == 0x20))	//for spansion S25FL128s flash
	{	
		do
		{
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);	//write status cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x1c);	
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x0e);	
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
		
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}	
		
			//read config status
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	//read config cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}
			
			u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x2;	//make sure the quad read flag is enable
			//DBG_MSG( "...cdd flash config register status   = 0x%x!\n",u1flag);
			u1count++ ;
			if( u1count >= 200)
			{
				DBG_MSG( "...setting config register fail!\n");
				return 1;
			}
		}while(!u1flag);

	}
	else if((u1MenuID == 0xEF) &&(u1DevID1 == 0x40))//winbond nor, enable QE bit in status register2(bit1) and then send enter qpi cmd(38h)
    {
        U8 u1Status1 = 0;
        if(_SetFlashWriteEnable(u4Index) != 0)
        {
            return 1;
        }
		if(u1DevID2 == 0x15)
        {
            //read config status
            SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
            SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
            SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
            SFLASH_WREG8(SFLASH_CNT_REG,16);			// Write SF Bit Count
            _SendFlashCommand(0x4); // Write PRG
            if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
            {
                return 1;
            }			
            u1Status1 = SFLASH_RREG8(SFLASH_SHREG0_REG);	//make sure the quad read flag is enable	
            DBG_MSG( "...cdd flash config register u1status1   = 0x%x!\n",u1Status1);
            
        }
        //read config status
        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	//read status cmd
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
        SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
        SFLASH_WREG8(SFLASH_CNT_REG,16);			// Write SF Bit Count
        _SendFlashCommand(0x4); // Write PRG
        if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
        {
            return 1;
        }			
        u1status = SFLASH_RREG8(SFLASH_SHREG0_REG);	//make sure the quad read flag is enable	
        DBG_MSG( "...cdd flash config register u1status   = 0x%x!\n",u1status);
        u1status = u1status | 0x2;					////quad read enable bit
        do
        {
            if(_SetFlashWriteEnable(u4Index) != 0)
            {
                return 1;
            }
            if(u1DevID2 == 0x15)
            {
                SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);   //write status cmd
                SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1Status1);    
                SFLASH_WREG8(SFLASH_PRGDATA3_REG, u1status);    
                SFLASH_WREG8(SFLASH_CNT_REG,24); 		   // Write SF Bit Count
            }
            else
            {
                SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x31);	//write status cmd
                SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1status);	
                SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
                SFLASH_WREG8(SFLASH_CNT_REG,16); 			// Write SF Bit Count
            }
            _SendFlashCommand(0x4);	// Write PRG
            if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
            {
                return 1;
            }
            
            //read config status
            SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	//read status cmd
            SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
            SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
            SFLASH_WREG8(SFLASH_CNT_REG,16);			// Write SF Bit Count
            _SendFlashCommand(0x4); // Write PRG
            if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
            {
               return 1;
            }			
            u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x2;	//make sure the quad read flag is enable
            DBG_MSG( " flash config register    = 0x%x!\n",(SFLASH_RREG8(SFLASH_SHREG0_REG)));
            u1count++ ;
            if( u1count >= 200)
            {
               DBG_MSG( "...setting status register fail!\n");
               return 1;
            }
        
        }while(!u1flag);
		//
        //SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x38);	//enter QPI cmd
        //SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
        //SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
        //SFLASH_WREG8(SFLASH_CNT_REG,8); 			// Write SF Bit Count
        
        //_SendFlashCommand(0x4);	// Write PRG
        //if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
        //{
        //    DBG_MSG( "WINBOND enter QPI cmd fail!\n");
        //    return 1;
        //}
        DBG_MSG( "WINBOND enter QUAD SPI successfully !\n");
        mdelay(1000);
    
    }
	else		////for mxic  flash
	{
		if(_SetFlashWriteEnable(u4Index) != 0)
		{
		    return 1;
		}
		//read config status
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
		SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
		_SendFlashCommand(0x4); // Write PRG
		if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
		{
			return 1;
		}			
		u1status = SFLASH_RREG8(SFLASH_SHREG0_REG);	//make sure the quad read flag is enable	
		u1status = u1status | 0x40;					////quad read enable bit
		DBG_MSG( "...cdd flash config register u1status   = 0x%x!\n",u1status);
		do
		{
		    SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);	//write status cmd
		    SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1status);	
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
		    SFLASH_WREG8(SFLASH_CNT_REG,24); 			// Write SF Bit Count

		    _SendFlashCommand(0x4);	// Write PRG
		    if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
		    {
		        return 1;
		    }
			if(_SetFlashWriteEnable(u4Index) != 0)
		    {
		        return 1;
		    }
			//read config status
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}			
			u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x40;	//make sure the quad read flag is enable
			DBG_MSG( "...cdd flash config register    = 0x%x!\n",(SFLASH_RREG8(SFLASH_SHREG0_REG)));
			u1count++ ;
			if( u1count >= 200)
			{
				DBG_MSG( "...setting status register fail!\n");
				return 1;
			}

		}while(!u1flag);
		 	//SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	
		    //SFLASH_WREG8(SFLASH_CNT_REG,08); 			// Write SF Bit Count
		    //_SendFlashCommand(0x4);	// Write PRG
	}
	//DBG_MSG( "...cdd Enter 4 bit read mode!\n");
	return 0;
}

//-----------------------------------------------------------------------------
/*
*Exit 4 bit read mode
*return 0 success, return others fial
*/
int32 _SetFlashExit4BitRead(void)
{
	uint32 u4Index;
	U8  u1status, u1flag, u1count = 0;
	u4Index = 0;
	if(_SetFlashWriteEnable(u4Index) != 0)
       {
              return 1;
       }		
	DBG_MSG( "... u1MenuID = 0x%x u1DevID1 = 0x%x u1DevID2 = 0x%x!\n",u1MenuID,u1DevID1,u1DevID2);
	//if( (u1MenuID == 0x01) &&(u1DevID1 == 0x20) &&(u1DevID2 == 0x18))
	if( (u1MenuID == 0x01) &&(u1DevID1 == 0x20))	//for spansion S25FL128s flash
	{	
		do
		{
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);	//write status cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x1c);	
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
		
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}	
		
			//read config status
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	//read config cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}
			
			u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x2;	//make sure the quad read flag is enable
			//DBG_MSG( "...cdd flash config register status   = 0x%x!\n",u1flag);
			u1count++ ;
			if( u1count >= 200)
			{
				DBG_MSG( "...setting config register fail!\n");
				return 1;
			}
		}while(!u1flag);

	}
	else if((u1MenuID == 0xEF) &&(u1DevID1 == 0x40))//winbond nor, enable QE bit in status register2(bit1) and then send enter qpi cmd(38h)
    {
        U8 u1Status1 = 0;
        //if(_SetFlashWriteEnable(u4Index) != 0)
        //{
        //    return 1;
        //}
        //SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0xFF);	//EXIT QUAD MODE cmd
        //SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
        //SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
        //SFLASH_WREG8(SFLASH_CNT_REG,16);			// Write SF Bit Count
        
        //_SendFlashCommand(0x4); // Write PRG
        //if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
        //{
        //    return 1;
        //}	
        //read config status
        if(u1DevID2 == 0x15)
        {
            //read config status
            SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
            SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
            SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
            SFLASH_WREG8(SFLASH_CNT_REG,16);			// Write SF Bit Count
            _SendFlashCommand(0x4); // Write PRG
            if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
            {
                return 1;
            }			
            u1Status1 = SFLASH_RREG8(SFLASH_SHREG0_REG);	//make sure the quad read flag is enable	
            DBG_MSG( "...cdd flash config register u1status1   = 0x%x!\n",u1Status1);
            
        }
        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	//read status cmd
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
        SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
        SFLASH_WREG8(SFLASH_CNT_REG,16);			// Write SF Bit Count
        _SendFlashCommand(0x4); // Write PRG
        if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
        {
            return 1;
        }			
        u1status = SFLASH_RREG8(SFLASH_SHREG0_REG);	//make sure the quad read flag is enable	
        u1status = u1status & 0xFD;					////quad read enable bit
        DBG_MSG( "...cdd flash config register u1status   = 0x%x!\n",u1status);
        do
        {
            if(_SetFlashWriteEnable(u4Index) != 0)  //write enable cmd need befor write status cmd
            {
               return 1;
            }
            if(u1DevID2 == 0x15)
            {
                SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);   //write status cmd
                SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1Status1);    
                SFLASH_WREG8(SFLASH_PRGDATA3_REG, u1status);    
                SFLASH_WREG8(SFLASH_CNT_REG,24); 		   // Write SF Bit Count
            }
            else
            {
                SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x31);	//write status cmd
                SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1status);	
                SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
                SFLASH_WREG8(SFLASH_CNT_REG,16); 			// Write SF Bit Count
            }
            
            _SendFlashCommand(0x4);	// Write PRG
            if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
            {
               return 1;
            }
            if(_SetFlashWriteEnable(u4Index) != 0)
            {
               return 1;
            }
            //read config status
            SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	//read status cmd
            SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
            SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
            SFLASH_WREG8(SFLASH_CNT_REG,16);			// Write SF Bit Count
            _SendFlashCommand(0x4); // Write PRG
            if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
            {
               return 1;
            }			
            u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x2;	//make sure the quad read flag is enable
            DBG_MSG( "...cdd flash config register    = 0x%x!\n",(SFLASH_RREG8(SFLASH_SHREG0_REG)));
            u1count++ ;
            if( u1count >= 200)
            {
               DBG_MSG( "...setting status register fail!\n");
               return 1;
            }
        
        }while(u1flag);
		//
        mdelay(1000);
    
    }
	else		////for mxic  flash
	{
		if(_SetFlashWriteEnable(u4Index) != 0)
		{
		    return 1;
		}
		//read config status
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
		SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
		_SendFlashCommand(0x4); // Write PRG
		if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
		{
			return 1;
		}			
		u1status = SFLASH_RREG8(SFLASH_SHREG0_REG);	//make sure the quad read flag is enable	
		u1status = u1status & 0xBF;					////quad read disable bit
		DBG_MSG( "...cdd flash config register u1status   = 0x%x!\n",u1status);
		do
		{
		    SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);	//write status cmd
		    SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1status);	
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
		    SFLASH_WREG8(SFLASH_CNT_REG,24); 			// Write SF Bit Count

		    _SendFlashCommand(0x4);	// Write PRG
		    if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
		    {
		        return 1;
		    }
			if(_SetFlashWriteEnable(u4Index) != 0)
		    {
		        return 1;
		    }
			//read config status
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
                            DBG_MSG( "polling set status reg fail	SFLASH_CMD_REG= 0x%x!\n",(SFLASH_RREG8(SFLASH_CMD_REG)));
				return 1;
			}			
			u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x40;	//make sure the quad read flag is disable
			DBG_MSG( "...cdd flash config register    = 0x%x!\n",(SFLASH_RREG8(SFLASH_SHREG0_REG)));
			u1count++ ;
			if( u1count >= 200)
			{
				DBG_MSG( "...setting status register fail!\n");
				return 1;
			}

		}while(u1flag);
		 	//SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	
		    //SFLASH_WREG8(SFLASH_CNT_REG,08); 			// Write SF Bit Count
		    //_SendFlashCommand(0x4);	// Write PRG
	}
	//DBG_MSG( "...cdd Enter 4 bit read mode!\n");
	return 0;
}

//-----------------------------------------------------------------------------
/*
*Exit 4 bytes address mode
*return 0 success, return others fial
*/
int32 _SetFlashExit4Byte(void)
{
	uint32 u4Index, u4DualReg;
	u4Index = 0;
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

	u4DualReg = SFLASH_RREG32(SFLASH_READ_DUAL_REG); 
	u4DualReg &= 0xffffffef;
	SFLASH_WREG32(SFLASH_READ_DUAL_REG, u4DualReg);
	if( _PollingReg(SFLASH_READ_DUAL_REG, 0x10) == 1) // timeout
	{
		return 1;
	}
    if( (u1MenuID == 0x01) &&(u1DevID1 == 0x02) &&(u1DevID2 == 0x19) )	//for spansion S25FL256s flash
    {
        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x17);	//Enter EN4B cmd
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	
        SFLASH_WREG8(SFLASH_CNT_REG,16);			// Write SF Bit Count
    }
    else
    {
    
        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0xe9);	//EXIT EN4B cmd
        SFLASH_WREG8(SFLASH_CNT_REG,8); 			// Write SF Bit Count
    }
	_SendFlashCommand(0x4); // Write PRG
	if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
	{
		return 1;
	}	
    DBG_MSG( "...cdd Exit 4 bytes address!\n");
	return 0;
}

//-----------------------------------------------------------------------------
static int32 _4BWriteBuffer(uint32 u4Index, uint32 u4Addr, uint32 u4Len, const U8* pu1Buf)
{
    uint32 i, j, u4BufIdx, u4Data;

    if(pu1Buf == NULL)
    {
        return 1;
    }

    ASSERT(u4Len <= SFLASH_WRBUF_SIZE);
    ASSERT((u4Addr%SFLASH_HW_ALIGNMENT) == 0);
    ASSERT((u4Len%SFLASH_HW_ALIGNMENT) == 0);
	//DBG_MSG( "...cdd _4BWriteBuffer u4Addr = 0x%x!\n", u4Addr);

    SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr))); // Write

    u4BufIdx = 0;
	//DBG_MSG( "...cdd u4Data = \n");
    for(i=0; i<u4Len; i+=4)
    {
        for(j=0; j<4; j++)
        {
            (*((U8 *)&u4Data + j)) = pu1Buf[u4BufIdx];
            u4BufIdx++;
        }
		//DBG_MSG( "0x%x  ", u4Data);
		//if(i % 8 == 0)
		//DBG_MSG( "\n");
        SFLASH_WREG32(SFLASH_PP_DATA_REG, u4Data);
     }

    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return 1;
    }
#if 0 //the following code can't be used for Atmel flash.
    //--------------------------------------------------------------
    if(!_fgSFIsrEn)
    {
	    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
	    {
	        return 1;
	    }
    }
#else
	if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
	{
	    return 1;
	}
#endif


    return 0;
}


//-----------------------------------------------------------------------------
static int32 _4BWriteSingleByte(uint32 u4Index, uint32 u4Addr, U8 u1Data)
{
    //DBG_MSG( "...cdd _4BWriteSingleByte u4Addr = 0x%x!\n", u4Addr);
    SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));

    //DBG_MSG( "...cdd _4BWriteSingleByte u1Data = 0x%x!\n", u1Data);
    SFLASH_WREG8(SFLASH_WDATA_REG, u1Data);

    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_4BWriteSector.
 *  Details of SFLASHHW_4BWriteSector (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_4BWriteSector(uint32 u4Index, uint32 u4Addr, uint32 u4Len,
                           const U8* pu1Buf)
{
    uint32 i, u4Count, u4PgAlign;
    ASSERT(u4Index < MAX_FLASHCOUNT);
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);
    ASSERT(u4Len <= _arFlashChip[u4Index].u4SecSize);

    if(u4Len == 0)
    {
        return 0;
    }

    if(pu1Buf == NULL)
    {
        return 1;
    }

    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

    // Handle cross page
    u4PgAlign = u4Addr % SFLASH_WRBUF_SIZE;
    if(u4PgAlign != 0)
    {
        for(i=0; i<(SFLASH_WRBUF_SIZE - u4PgAlign); i++)
        {
            if(_4BWriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                return 1;
            }
            u4Addr++;
            pu1Buf++;
            u4Len--;

            if(u4Len == 0)
            {
                return 0;
            }
        }
    }

    if(_WBEnable() != 0)
    {
        return 1;
    }

    while((int32)u4Len > 0)
    {
        if(u4Len >= SFLASH_WRBUF_SIZE)
        {
            u4Count = SFLASH_WRBUF_SIZE;
        }
        else
        {
            // Not write-buffer alignment
            break;
        }

        if(_4BWriteBuffer(u4Index, u4Addr, u4Count, pu1Buf) != 0)
        {
            DBG_MSG("Write flash error!! faddr = 0x%x, len = 0x%x\n", u4Addr, u4Count);
            if(_WBDisable() != 0)
            {
                return 1;
            }
            return 1;
        }
        u4Len -= u4Count;
        u4Addr += u4Count;
        pu1Buf += u4Count;

    }

    if(_WBDisable() != 0)
    {
        return 1;
    }

    if((int32)u4Len > 0)
    {
        for(i=0; i<u4Len; i++)
        {
            if(_4BWriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                if(_WBDisable() != 0)
                {
                    return 1;
                }
                return 1;
            }
            u4Addr++;
            pu1Buf++;
        }
    }

    return 0;
}



int32 SFLASHHW_4BEraseSector(uint32 u4Index, uint32 u4Addr)
{
    uint32 u4Polling;
    U8 u1Reg;

    ASSERT(u4Index < MAX_FLASHCOUNT);
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);

	DBG_MSG( "...cdd SFLASHHW_4BEraseSector u4Addr = 0x%x!\n", u4Addr);

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }


    // Execute sector erase command
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1SECERASECmd);
	SFLASH_WREG8(SFLASH_PRGDATA4_REG, HiByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA2_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA1_REG, LoByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_CNT_REG, 40);       // Write SF Bit Count
    SFLASHHW_SwitchChip(u4Index);
    if(u4Index==0)
    {
        _SendFlashCommand(0x4);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);
    }
#endif
    else
    {
        ASSERT(0);
    }

    // Can not use ISR mode, because there is not erase sector interrupt
    u4Polling = 0;
    while(1)
    {
        if(SFLASHHW_ReadFlashStatus(u4Index, &u1Reg) != 0)
        {
            SFLASHHW_SwitchChip(0);
            return 1;
        }

        if(0 == (u1Reg & 0x1))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
        {
            SFLASHHW_SwitchChip(0);
            return 1;
        }

        mdelay(10);
    }

    _SendFlashCommand(0x0);
    SFLASHHW_SwitchChip(0);
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_Init.
 *  Details of SFLASHHW_Init (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_DeInit(void)
{
#ifndef CC_5381_LOADER
    void (* pfnOldIsr)(uint16);
    //--------------------------------------------------
    // Un-Register ISR
    //--------------------------------------------------

    SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x0);
    //VERIFY(BIM_ClearIrq(VECTOR_FLASH));

   // VERIFY(x_reg_isr(VECTOR_FLASH, NULL, &pfnOldIsr) == OSR_OK);
#endif /* CC_5381_LOADER */

  return 0;
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteSfProtect.
 *  Details of SFLASHHW_WriteSfProtect (u4Val).
 */
//-----------------------------------------------------------------------------
void SFLASHHW_WriteSfProtect(uint32 u4Val)
{
	SFLASH_WREG32(SFLASH_WRPROT_REG, u4Val);
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_ReadSfProtect.
 *  Details of SFLASHHW_ReadSfProtect ().
 */
//-----------------------------------------------------------------------------
uint32 SFLASHHW_ReadSfProtect(void)
{
    return SFLASH_RREG32(SFLASH_WRPROT_REG);
}



uint32 SFLASHHW_PinMux(uint32 u4Val)
{
    //BIM_WRITE32(0x3B0, u4Val);
    //return (uint32)(BIM_READ32(0x3B0));
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetClk.
 *  Details of SFLASHHW_SetClk (optional).
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetClk(uint32 u4Val)
{
    //SFLASH_WREG32(SFLASH_CKGEN_REG, u4Val);
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteProtect.
 *  Details of SFLASHHW_WriteProtect (optional).
 */
//-----------------------------------------------------------------------------
int32 SFLASHHW_WriteProtect(uint32 u4Index, bool fgEnable)
{
	
	  if(u4Index >= MAX_FLASHCOUNT)
	  {
       DBG_MSG( "Nor Flash Index is out fo Max Flash Count\n");
       return -1;	  	
	  }
#ifdef REPLACE_EEPROM_WITH_NOR	
    if(!_fgDoProtect)
    {
        return 0;
    }

    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return -1;
    }

    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return -1;
    }

	if((0xEF ==_arFlashChip[u4Index].u1MenuID) || (0xC8 ==_arFlashChip[u4Index].u1MenuID) )
	{
		if(fgEnable)
		{
			//Write status cmd 0x01 + status register 1 +  status register 2
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1WRSRCmd);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, _arFlashChip[u4Index].u1Protection_Eeprom);
		
			//SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x08);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x40);
		
			SFLASH_WREG8(SFLASH_CNT_REG,24); // Write SF Bit Count
			if(u4Index==0)
			{
				_SendFlashCommand(0x04);
			}
			else
			{
				ASSERT(0);
			}
			if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
			{
				return -1;
			}			
			if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
			{
				return -1;
			}
			_SendFlashCommand(0x0);
			return 0;
		
		}
		else
		{
			//DBG_MSG("....fgdisable\n");
			//Write status cmd 0x01 + status register 1 +  status register 2
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1WRSRCmd);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, _arFlashChip[u4Index].u1Protection);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
			SFLASH_WREG8(SFLASH_CNT_REG,24); // Write SF Bit Count
			if(u4Index==0)
			{
				_SendFlashCommand(0x04);
			}
			else
			{
				ASSERT(0);
			}
			if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
			{
				return -1;
			}	
			
			if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
			{
				return -1;
			}
			_SendFlashCommand(0x0);	
			return 0;
		}
	}	
    if(fgEnable)
    {
	    SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1Protection_Eeprom);
    }
    else
    {
        if(_fgNorPartialProtection)
        {
            SFLASH_WREG32(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1Protection);
        }
        else
        {
            SFLASH_WREG32(SFLASH_PRGDATA5_REG, 0x0);
        }
    }

    SFLASH_WREG8(SFLASH_CNT_REG,8); // Write SF Bit Count

    if(u4Index==0)
    {
        _SendFlashCommand(0x20);
    }
    else
    {
        ASSERT(0);
    }
    if(_PollingReg(SFLASH_CMD_REG, 0x20) != 0)
    {
        return -1;
    }
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return -1;
    }

#else		//not  REPLACE_EEPROM_WITH_NOR
	if(_fgWriteProtect)
	{

        if(!_fgDoProtect)
        {
            return 0;
        }
    
        if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
        {
            return -1;
        }
    
        if(_SetFlashWriteEnable(u4Index) != 0)
        {
            return -1;
        }
        if(_fgAAIWrite)
        {
            DBG_MSG( "AAI Write Protect Enable flag %d\n", fgEnable);
            SFLASHHW_SendByte(u4Index, 0x50); // EWSR
            if(fgEnable)
            {
                SFLASHHW_Send2Byte(u4Index, _arFlashChip[u4Index].u1WRSRCmd, 0x3C); // WRSR
            }
            else
            {
                if(_fgNorPartialProtection)
                {
                  SFLASHHW_Send2Byte(u4Index, _arFlashChip[u4Index].u1WRSRCmd, _arFlashChip[u4Index].u1Protection); // WRSR
                }
                else
                {
                SFLASHHW_Send2Byte(u4Index, _arFlashChip[u4Index].u1WRSRCmd, 0x00); // WRSR
            	}
            }
            if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
            {
                return -1;
            }
        }
        if(fgEnable)
        {
            SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x3C);
        }
        else
        {
            if(_fgNorPartialProtection)//_fgNorPartialProtection is TV used only
            {
                SFLASH_WREG32(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1Protection);
            }
            else
            {
                SFLASH_WREG32(SFLASH_PRGDATA5_REG, 0x0);
            }
        }
    
        SFLASH_WREG8(SFLASH_CNT_REG,8); // Write SF Bit Count
    
        if(u4Index==0)
        {
            _SendFlashCommand(0x20);
        }
    		
    #ifdef USE_SECOND_FLASH
        else if(u4Index == 1)
        {
            _SendFlashCommand(0x60);
        }
    #endif
    		
        else
        {
            ASSERT(0);
        }
    
        if(_PollingReg(SFLASH_CMD_REG, 0x20) != 0)
        {
            return -1;
        }
    
        if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
        {
            return -1;
        }
	}
else
{
   // UNUSED(u4Index);
    //UNUSED(fgEnable);
}
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetFlashName.
 *  Details of SFLASHHW_GetFlashName (optional).
 */
//-----------------------------------------------------------------------------
U8* SFLASHHW_GetFlashName(U8 u1MenuID, U8 u1DevID1, U8 u1DevID2)
{
    U8 *pStr;
    uint32 i;

    i = 0;
    while(_aVendorFlash[i].u1MenuID != (U8)0x0)
    {
        if( (_aVendorFlash[i].u1MenuID == u1MenuID) &&
            (_aVendorFlash[i].u1DevID1 == u1DevID1) &&
            (_aVendorFlash[i].u1DevID2 == u1DevID2))
        {
            pStr = _aVendorFlash[i].pcFlashStr;
            return pStr;
        }
        i++;
    }

    return NULL;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetFlashSize.
 *  Details of SFLASHHW_GetFlashSize (optional).
 */
//-----------------------------------------------------------------------------
uint32 SFLASHHW_GetFlashSize(U8 u1MenuID, U8 u1DevID1, U8 u1DevID2)
{
    uint32 u4Size;
    uint32 i;

    i = 0;
    while(_aVendorFlash[i].u1MenuID != (U8)0x0)
    {
        if( (_aVendorFlash[i].u1MenuID == u1MenuID) &&
            (_aVendorFlash[i].u1DevID1 == u1DevID1) &&
            (_aVendorFlash[i].u1DevID2 == u1DevID2))
        {
            u4Size = _aVendorFlash[i].u4ChipSize;
            return u4Size;
        }
        i++;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_DMA.
 *  Details of SFLASHHW_DMA (optional).
 */
//-----------------------------------------------------------------------------

int32 SFLASHHW_EnableDMA(uint32 u4SrcAddr, uint32 u4DestAddr, uint32 u4Size)
{
    uint32 u4DRAMStart, u4DRAMEnd;
    uint32 u4Reg, u4BinReg, u4ResetVal;
	//HAL_TIME_T time1={0}, time2={0}, result_time={0};
	//HAL_GetTime(&time1);
    u4BinReg   = 0x718;
	u4ResetVal = 0x4;

    if(((u4DestAddr%16)!=0) || ((u4Size%16)!=0))
    {
        DBG_MSG( "Address or size is not 16-byte alignment!\n");
        return 1;
    }
	DBG_MSG( "...cdd SFLASHHW_EnableDMA u4SrcAddr = 0x%x u4DestAddr = 0x%x\n", u4SrcAddr, u4DestAddr);

    //if(u4SrcAddr > PBI_A_BASE)
    {
     //   u4SrcAddr -= PBI_A_BASE;
    }
#if 1
	// Do reset
	SFLASH_WREG32(u4BinReg, 0);
	SFLASH_WREG32(u4BinReg, 0x2);
	SFLASH_WREG32(u4BinReg, u4ResetVal);

	// Flash source address
	SFLASH_WREG32(u4BinReg + 4, u4SrcAddr);

	// DRAM destination address
	u4DRAMStart = u4DestAddr & (~(0xF));
	u4DRAMEnd = (u4DestAddr + u4Size) & (~(0xF));
	SFLASH_WREG32(u4BinReg + 8, u4DRAMStart);
	SFLASH_WREG32(u4BinReg + 12, u4DRAMEnd);

	// Enable
    SFLASH_WREG32(u4BinReg, 0x5);	// MT5391

	while(1)
	{
		u4Reg = SFLASH_RREG32(u4BinReg);
        if((u4Reg&0x1) == 0)
        {
            break;
        }
	}
    DBG_MSG( "dma read done\n");
#endif
	return 0;
}


//------------------------------------------------------------------------------
// Check Device ID Callback Function                                            
//------------------------------------------------------------------------------
bool COMMON_CheckDevID(volatile uint16 *die1_addr, volatile uint16 *die2_addr) {
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
	//  For 8590 not support two die nor flash now.
	
       SFLASHHW_Init();
	return TRUE;
}

//------------------------------------------------------------------------------
// Check If Device Is Idle Callback Function                                    
//------------------------------------------------------------------------------
bool Common_CheckDevIdle(const uint32 addr)
{	
	if(0 ==  SFLASH_RREG32(SFLASH_CMD_REG) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void Common_Erase_CMD(const uint32 blockaddr)
{
	SFLASHHW_EraseSector(0, blockaddr);
}

STATUS_E Common_Erase_CheckDone_By_Polling(const uint32 blockaddr)
{

        uint32 u4Polling;
        U8 u1Reg;  
        
        // Can not use ISR mode, because there is not erase sector interrupt
        u4Polling = 0;
        while(1)
        {
              if(SFLASHHW_ReadFlashStatus(0, &u1Reg) != 0)
              {
                  SFLASHHW_SwitchChip(0);
                  return S_DA_NOR_ERASE_FAILED;
              }
              
              if(0 == (u1Reg & 0x1))
              {
                  break;
              }
              
              u4Polling++;
              if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
              {
                  SFLASHHW_SwitchChip(0);
                  
                  return S_DA_NOR_ERASE_FAILED;
              }
              
              mdelay(10);
        }
        
        _SendFlashCommand(0x0);
        SFLASHHW_SwitchChip(0);
        return S_DONE;
}

//------------------------------------------------------------------------------
// Program Related Callback Function                                            
//------------------------------------------------------------------------------
void Common_PreProcess(void) {
	SFLASHHW_WriteSfProtect(0x30);
}
void Common_Program_PostProcess(void){}

void Common_UnlockBypass_Enter_CMD(const uint32 blockaddr)
{
	
}

void Common_UnlockBypass_Exit_CMD(const uint32 blockaddr)
{
	
}
void Common_UnlockBypass_Program_CMD(const uint32 blockaddr, const uint32 prog_addr, const uint16 data)
{
       _WriteSingleByte(0, blockaddr + prog_addr, (uint8)(data));
       _WriteSingleByte(0, blockaddr + prog_addr, (uint8)(data>>8));
}


STATUS_E Common_Program_CheckDone_By_Toggle(const uint32 prog_addr)
{
      return S_DONE;
}

void Common_Nor_Buf_Program(const uint32 blockaddr, const uint32 prog_addr, const uint16 *data, const uint32 length_in_word)
{
    uint8 *pu1Buf;
    uint32 i, j, u4WriteAddr, u4Len, u4WriteByte, u4ChipOffset, u4SectStart, u4SectEnd;
    pu1Buf = (uint8*)data;
    u4WriteAddr = blockaddr + prog_addr;
    u4WriteByte = length_in_word*2;
    u4ChipOffset = 0;
    u4SectStart = 0;
    u4SectEnd = 0;

        if(SFLASHHW_WriteProtect(i, FALSE) != 0)
        {
            DBG_MSG( "Disable Flash write protect fail!\n");
            fgProgramPass = FALSE;
        }
        for(j=0; j<_arFlashChip[0].u4ChipSize / _arFlashChip[0].u4SecSize; j++)
        {
            u4SectEnd = u4SectStart + _arFlashChip[0].u4SecSize;
            if((u4SectStart<=u4WriteAddr)&&(u4WriteAddr<u4SectEnd))
            {
                u4Len = _arFlashChip[0].u4SecSize - (u4WriteAddr - u4SectStart);
                if(u4Len >= u4WriteByte)
                {
                    u4Len = u4WriteByte;
                }
				//printf("....u4WriteAddr = 0x%x\n",u4WriteAddr);
                if(SFLASHHW_WriteSector(i, u4WriteAddr-u4ChipOffset, u4Len, pu1Buf) != 0)
                {
                    DBG_MSG( "Write flash error !\n");
                    if(SFLASHHW_WriteProtect(i, TRUE) != 0)
                    {
                        DBG_MSG( "Enable flash write protect fail!\n");
                        fgProgramPass = FALSE;
                    }
                    fgProgramPass = FALSE;
                }

                u4WriteAddr += u4Len;
                u4WriteByte -= u4Len;
                pu1Buf += u4Len;

                if(u4WriteByte == 0)
                {
                    //if(SFLASH_WriteProtect(i, TRUE) != 0)
                    //{
                    //    must_print( "Enable flash write protect fail!\n");
                    //    return -1;
                    //}
                    //return 0;
                    //must_print( "Write flash index: %d is done!\n",i);
                    break;
                }
            }
            u4SectStart += _arFlashChip[0].u4SecSize;
        }
        u4ChipOffset += _arFlashChip[0].u4SecSize;

        if(SFLASHHW_WriteProtect(i, TRUE) != 0)
        {
            DBG_MSG( "Enable flash write protect fail!\n");
            fgProgramPass = FALSE;
        }
	
       fgProgramPass = TRUE;
}


STATUS_E Common_Nor_Buf_Program_Check_Done(const uint32 prog_addr)
{
       if(fgProgramPass)
           return S_DONE;
       else
           return S_DA_NOR_PROGRAM_FAILED;
}


