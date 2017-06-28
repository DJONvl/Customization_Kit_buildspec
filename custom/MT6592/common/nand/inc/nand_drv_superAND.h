/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#ifndef _NAND_DRV_SUPERAND_H_
#define _NAND_DRV_SUPERAND_H_

#include "SW_TYPES.H"
#include "DOWNLOAD.H"
#include "nand_dev_tbl.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// superAND Command Set                                                         
//------------------------------------------------------------------------------
extern const NAND_CommandSet_S		g_NAND_superAND_2048_CMD_SET;
extern const NAND_CommandSet_S		g_NAND_superAND_512_CMD_SET;
//------------------------------------------------------------------------------
// superAND Callback Function Set                                               
//------------------------------------------------------------------------------
extern const NAND_CMD_Callback_S	g_NAND_superAND_CB_FUNC_SET;

//------------------------------------------------------------------------------
// Read Device ID Callback Function                                             
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Reset Device Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_Reset(
				const uint32  c_timeout);

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
				const uint32  *p_spare32 /* MUST be 32bits alignment addr */);

STATUS_E  NAND_superAND_BadBlockSymbol_Set(
				uint32  *p_spare32 /* MUST be 32bits alignment addr */);

//------------------------------------------------------------------------------
// Page Read Callback Function                                                  
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_PageRead(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]);

STATUS_E  NAND_superAND_PageRead_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]);

//------------------------------------------------------------------------------
// Page Program Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_PageProgram(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]);

//------------------------------------------------------------------------------
// Page+Spare Program Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E NAND_superAND_PageSpareProgram(uint32 row_addr,
                                        const uint32 *p_page_spare);

//------------------------------------------------------------------------------
// Spare Read Callback Function                                                 
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_SpareRead(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_spare32 /* MUST be 32bits alignment addr */);

//------------------------------------------------------------------------------
// Spare Program Callback Function                                              
//------------------------------------------------------------------------------
STATUS_E  NAND_superAND_SpareProgram(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_spare32 /* MUST be 32bits alignment addr */);


#ifdef __cplusplus
}
#endif

#endif
