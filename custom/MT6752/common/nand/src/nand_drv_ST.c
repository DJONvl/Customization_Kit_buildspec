/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#include "nand_drv_ST.h"
#include "nand_drv_COMMON_v3_1.h"
#include "nand_util_func.h"
#include "nfi.h"

#if !defined (__MTK_NFI_V4__)
//------------------------------------------------------------------------------
// STMicroelectronics Command Set                                               
//------------------------------------------------------------------------------
const NAND_CommandSet_S		g_NAND_ST_512_CMD_SET={
	 { 1, 0x90 }	// read id 
	,{ 1, 0x70 }	// read status 
	,{ 1, 0xFF }	// reset device 
	,{ 1, 0x00 }	// read 
	,{ 1, 0x50 }	// read spare area 
	,{ 0, 0 }		// read confirm 
	,{ 1, 0x00 }	// program 1st half page 
	,{ 1, 0x80 }	// program 
	,{ 1, 0x10 }	// program confirm 
	,{ 1, 0x60 }	// erase 
	,{ 1, 0xD0 }	// erase confirm 
};

const NAND_CommandSet_S		g_NAND_ST_2048_CMD_SET={
	 { 1, 0x90 }	// read id 
	,{ 1, 0x70 }	// read status 
	,{ 1, 0xFF }	// reset device 
	,{ 1, 0x00 }	// read 
	,{ 0, 0 }		// read spare area 
	,{ 1, 0x30 }	// read confirm 
	,{ 0, 0 }		// program 1st half page 
	,{ 1, 0x80 }	// program 
	,{ 1, 0x10 }	// program confirm 
	,{ 1, 0x60 }	// erase 
	,{ 1, 0xD0 }	// erase confirm 
};

//------------------------------------------------------------------------------
// ST Callback Function Set                                                     
//------------------------------------------------------------------------------
const NAND_CMD_Callback_S	g_NAND_ST_CB_FUNC_SET={
	ID_ST_CB_FUNC_SET
	,NAND_COMMON_ReadID_V3
	,NAND_COMMON_Reset_V3
	,NAND_ST_WholeChipUnlock_V3
	,NAND_COMMON_ReadStatus_V3
	,NAND_COMMON_BlockErase_V3
	,NAND_COMMON_BadBlockSymbol_Check_V3
	,NAND_COMMON_BadBlockSymbol_Set_V3
	,NAND_COMMON_PageRead_V3
	,NAND_COMMON_PageProgram_V3
	,NAND_COMMON_SpareRead_V3
	,NAND_COMMON_SpareProgram_V3
	,NAND_COMMON_PageSpareProgram_V3
};

//------------------------------------------------------------------------------
// ST Callback Function Set Without H/W CopyBack Function                       
//------------------------------------------------------------------------------
const NAND_CMD_Callback_S	g_NAND_ST_CB_FUNC_SET_WITHOUT_COPYBACK={
	ID_ST_CB_FUNC_SET_WITHOUT_COPYBACK
	,NAND_COMMON_ReadID_V3
	,NAND_COMMON_Reset_V3
	,NAND_ST_WholeChipUnlock_V3
	,NAND_COMMON_ReadStatus_V3
	,NAND_COMMON_BlockErase_V3
	,NAND_COMMON_BadBlockSymbol_Check_V3
	,NAND_COMMON_BadBlockSymbol_Set_V3
	,NAND_COMMON_PageRead_V3
	,NAND_COMMON_PageProgram_V3
	,NAND_COMMON_SpareRead_V3
	,NAND_COMMON_SpareProgram_V3
	,NAND_COMMON_PageSpareProgram_V3
};

//------------------------------------------------------------------------------
// Whole Chip Unlock                                                            
//------------------------------------------------------------------------------
STATUS_E  NAND_ST_WholeChipUnlock_V3(const uint32  c_timeout)
{
	uint32	page_size;
	uint32	spare_size;
	uint32	addr_cycle;
	uint32	ra_last_blockaddr;
	uint32  timeout=c_timeout;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	addr_cycle = NUTL_ADDR_CYCLE();
	ra_last_blockaddr = NUTL_AbsAddrToBlockAddr((NUTL_TOTAL_SIZE()-1));

	//MT6516_AP/MT6516_MD: ray: todo!!!!

	return S_DONE;
}
#endif

