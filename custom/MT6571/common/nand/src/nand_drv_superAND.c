/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#include "nfi.h"
#if defined (__MTK_NFI_V5__)
#include "nand_drv_COMMON_v5.h"
#else
#include "nand_drv_COMMON_v3_1.h"
#endif
#include "nand_drv_superAND.h"
#include "nand_util_func.h"


#if !defined (__MTK_NFI_V4__)
//------------------------------------------------------------------------------
// superAND Command Set                                                         
//------------------------------------------------------------------------------
const NAND_CommandSet_S		g_NAND_superAND_512_CMD_SET={
	 { 1, 0x90 }	// read id 
	,{ 1, 0x70 }	// read status 
	,{ 1, 0xFF }	// reset device 
	,{ 1, 0x00 }	// read 
	,{ 0, 0 }		// read spare area 
	,{ 0, 0 }	// read confirm 
	,{ 0, 0 }		// program 1st half page 
	,{ 1, 0x10 }	// program 
	,{ 0, 0 }	// program confirm 
	,{ 1, 0x60 }	// erase 
	,{ 1, 0xD0 }	// erase confirm 
};

const NAND_CommandSet_S		g_NAND_superAND_2048_CMD_SET={
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
// superAND Callback Function Set                                               
//------------------------------------------------------------------------------
#if defined (__MTK_NFI_V5__)
const NAND_CMD_Callback_S	g_NAND_superAND_CB_FUNC_SET={
	ID_superAND_CB_FUNC_SET
	,NAND_COMMON_ReadID_V5
	,NAND_superAND_Reset
	,NULL
	,NAND_COMMON_ReadStatus_V5
	,NAND_COMMON_BlockErase_V5
	,NAND_superAND_BadBlockSymbol_Check
	,NAND_superAND_BadBlockSymbol_Set
	,NAND_superAND_PageRead_V3
	,NAND_superAND_PageProgram
	,NAND_superAND_SpareRead
	,NAND_superAND_SpareProgram
	,NAND_superAND_PageSpareProgram
};
#else
const NAND_CMD_Callback_S	g_NAND_superAND_CB_FUNC_SET={
	ID_superAND_CB_FUNC_SET
	,NAND_COMMON_ReadID_V3
	,NAND_superAND_Reset
	,NULL
	,NAND_COMMON_ReadStatus_V3
	,NAND_COMMON_BlockErase_V3
	,NAND_superAND_BadBlockSymbol_Check
	,NAND_superAND_BadBlockSymbol_Set
	,NAND_superAND_PageRead_V3
	,NAND_superAND_PageProgram
	,NAND_superAND_SpareRead
	,NAND_superAND_SpareProgram
	,NAND_superAND_PageSpareProgram
};
#endif
//------------------------------------------------------------------------------
// Read Device ID Callback Function                                             
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Reset Device Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_Reset(
				const uint32  c_timeout
) {
	return S_DONE;
}

//------------------------------------------------------------------------------
// Read Status Callback Function                                                
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Block Erase Related Callback Function                                        
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Bad Block Symbol Identification Related Callback Function                    
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_BadBlockSymbol_Check(
				const uint32  *p_spare32 /* MUST be 32bits alignment addr */
) {
	return S_DONE;
}

STATUS_E  NAND_superAND_BadBlockSymbol_Set(
				uint32  *p_spare32 /* MUST be 32bits alignment addr */
) {
	return S_DONE;
}

//------------------------------------------------------------------------------
// Page Read Callback Function                                                  
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_PageRead_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {
	uint32		page_size;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		timeout=c_timeout;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
	bool		bUsingDMA=TRUE;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();

	//MT6516_AP/MT6516_MD: ray: todo!!!!

	return ret;
}

//------------------------------------------------------------------------------
// Page Program Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_PageProgram(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {
	return S_DA_SUPERAND_PAGE_PRGRAM_NOT_SUPPORT;
}

//------------------------------------------------------------------------------
// Page+Spare Program Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E NAND_superAND_PageSpareProgram(uint32 row_addr,
                                        const uint32 *p_page_spare)
{
    return S_DA_SUPERAND_PAGE_PRGRAM_NOT_SUPPORT;
}

//------------------------------------------------------------------------------
// Spare Read Callback Function                                                 
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_SpareRead(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_spare32 /* MUST be 32bits alignment addr */
) {
	return S_DONE;
}

//------------------------------------------------------------------------------
// Spare Program Callback Function                                              
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_SpareProgram(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_spare32 /* MUST be 32bits alignment addr */
) {
	return S_DA_SUPERAND_SPARE_PRGRAM_NOT_SUPPORT;
}
#endif

