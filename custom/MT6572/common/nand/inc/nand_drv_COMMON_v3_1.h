/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#ifndef _NAND_DRV_COMMON_V3_1_H_
#define _NAND_DRV_COMMON_V3_1_H_

#include "SW_TYPES.H"
#include "DOWNLOAD.H"
#include "nand_dev_tbl.h"

#define NFI_PSTA_TIMEOUT	0xFFFF

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// COMMON Callback Function Set                                                 
//------------------------------------------------------------------------------
extern const NAND_CMD_Callback_S	g_NAND_COMMON_CB_FUNC_SET;

//------------------------------------------------------------------------------
// Read Device ID Callback Function                                             
//------------------------------------------------------------------------------
#if defined (BB_MT6572)
STATUS_E  NAND_COMMON_ReadID_V3(
                const uint32  c_timeout
                ,uint32 *id1 //add by jiequn,2011.11.11
                ,uint32 *id2 
);
#else
STATUS_E  NAND_COMMON_ReadID_V3(
				const uint32  c_timeout
				,uint16	*p_maker_code
				,uint16	*p_device_code
				,uint16 *p_ext_code1
				,uint16 *p_ext_code2
				,uint16 *p_ext_code3 //add by jiequn,2011.11.11
);
#endif

//------------------------------------------------------------------------------
// Reset Device Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_Reset_V3(
				const uint32  c_timeout);

//------------------------------------------------------------------------------
// Read Status Callback Function                                                
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_ReadStatus_V3(
				const uint32  c_timeout);

//------------------------------------------------------------------------------
// Block Erase Related Callback Function                                        
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_BlockErase_V3(
				const uint32  row_addr);

//------------------------------------------------------------------------------
// Bad Block Symbol Identification Related Callback Function                    
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_BadBlockSymbol_Check_V3(
				const uint32  *p_spare32 /* MUST be 32bits alignment addr */);

STATUS_E  NAND_COMMON_BadBlockSymbol_Set_V3(
				uint32  *p_spare32 /* MUST be 32bits alignment addr */);

//------------------------------------------------------------------------------
// Page Read Callback Function                                                  
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_PageRead_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]);
//------------------------------------------------------------------------------
// Read From NFI FIFO                                                           
//------------------------------------------------------------------------------

STATUS_E  NUTL_FIFO_Read_V3(
				const uint32  c_timeout
				,const bool bUsingDMA
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,const uint32 data_len);
				
//------------------------------------------------------------------------------
// Page Program Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_PageProgram_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]);

//------------------------------------------------------------------------------
// Page+Spare Program Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E NAND_COMMON_PageSpareProgram_V3(uint32 row_addr,
                                         const uint32 *p_page_spare);

//------------------------------------------------------------------------------
// Write To NFI FIFO                                                            
//------------------------------------------------------------------------------

STATUS_E  NUTL_FIFO_Write_V3(const bool bUsingDMA,
                             const uint32 *p_data32, /* MUST be 32bits alignment addr */
                             NUTL_ProgramFlag_E flag);
//------------------------------------------------------------------------------
// Spare Read Callback Function                                                 
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_SpareRead_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_spare32 /* MUST be 32bits alignment addr */);

//------------------------------------------------------------------------------
// Spare Program Callback Function                                              
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_SpareProgram_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_spare32 /* MUST be 32bits alignment addr */);

//------------------------------------------------------------------------------
// CopyBack Callback Function                                                   
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_CopyBack_V3(
				const uint32  c_timeout
				,const uint32  src_row_addr
				,const uint32  dest_row_addr);

#ifdef __cplusplus
}
#endif

#endif