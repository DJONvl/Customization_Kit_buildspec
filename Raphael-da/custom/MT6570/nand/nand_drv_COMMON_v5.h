/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#ifndef _NAND_DRV_COMMON_V5_H_
#define _NAND_DRV_COMMON_V5_H_

#include "sys/types.h"
#include "nand_types.h"
#include "nand_dev_tbl.h"

#define NFI_PSTA_TIMEOUT	0xFFFF

#ifdef __cplusplus
extern "C" {
#endif


//------------------------------------------------------------------------------
// Read Device ID Callback Function                                             
//------------------------------------------------------------------------------
status_t  NAND_COMMON_ReadID_V5(
			const uint32	c_timeout,
			uint32* 		id1,
			uint32* 		id2
			);

//------------------------------------------------------------------------------
// Reset Device Callback Function                                               
//------------------------------------------------------------------------------
status_t  NAND_COMMON_Reset_V5(
			const uint32  c_timeout
			);

//------------------------------------------------------------------------------
// Read Status Callback Function                                                
//------------------------------------------------------------------------------
status_t  NAND_COMMON_ReadStatus_V5(
			const uint32  c_timeout
			);

//------------------------------------------------------------------------------
// Block Erase Related Callback Function                                        
//------------------------------------------------------------------------------
status_t  NAND_COMMON_BlockErase_V5(
			const uint32  row_addr
			);

//------------------------------------------------------------------------------
// Bad Block Symbol Identification Related Callback Function                    
//------------------------------------------------------------------------------
status_t  NAND_COMMON_BadBlockSymbol_Check_V5(
			const uint32  *p_spare32 /* MUST be 32bits alignment addr */
			);

status_t  NAND_COMMON_BadBlockSymbol_Set_V5(
			uint32  *p_spare32 /* MUST be 32bits alignment addr */
			);

//------------------------------------------------------------------------------
// Page Read Function                                                  
//------------------------------------------------------------------------------
status_t  NAND_COMMON_PageRead_V5(
			const uint32	c_timeout,
			const uint32	row_addr,
			uint32*			p_data32,		/* MUST be 32bits alignment addr */
			uint32*			p_spare32,		/* MUST be 32bits alignment addr */
			uint32			ecc_parity_from_reg[8]
			);

//------------------------------------------------------------------------------
// Page Program Function                                               
//------------------------------------------------------------------------------
status_t  NAND_COMMON_PageProgram_V5(
			const uint32	c_timeout,
			const uint32	row_addr,
			const uint32*	p_data32,		/* MUST be 32bits alignment addr */
			const uint32*	p_spare32,
			uint32			ecc_parity_from_reg[8]
			);

// For TLC NAND
status_t  NAND_COMMON_PageProgramBad_V5(
			const uint32	c_timeout,
			const uint32	row_addr,
			const uint32*	p_data32,		/* MUST be 32bits alignment addr */
			const uint32*	p_spare32,		/* MUST be 32bits alignment addr */
			uint32			ecc_parity_from_reg[8]
			);

// For TLC NAND
status_t NFI_TLC_PageWrite(
			const uint32	c_timeout, 
			const uint32	page_index, 
			const uint32*	sector_buff, 
			const uint32*	spare_buff,
			uint32			ecc_parity_from_reg[8]
			);

//------------------------------------------------------------------------------
// Block Charge for TLC NAND                                                           
//------------------------------------------------------------------------------
status_t  NAND_COMMON_Block_charge_V5(
			const uint32 row_addr
			);
//------------------------------------------------------------------------------
// Page+Spare Program Function                                               
//------------------------------------------------------------------------------
status_t NAND_COMMON_PageSpareProgram_V5(
			uint32			row_addr,
			const uint32	*p_page_spare
			);
//------------------------------------------------------------------------------
// Spare Read Function                                                 
//------------------------------------------------------------------------------
status_t  NAND_COMMON_SpareRead_V5(
			const uint32	c_timeout,
			const uint32	row_addr,
			uint32			*p_spare32	/* MUST be 32bits alignment addr */
			);
//------------------------------------------------------------------------------
// Spare Program Function                                              
//------------------------------------------------------------------------------
status_t  NAND_COMMON_SpareProgram_V5(
			const uint32	c_timeout,
			const uint32	row_addr,
			const uint32	*p_spare32	/* MUST be 32bits alignment addr */
			);
//------------------------------------------------------------------------------
// Read From NFI FIFO                                                           
//------------------------------------------------------------------------------
status_t  NUTL_FIFO_Read_V5(
			const uint32	c_timeout,
			const bool		bUsingDMA,
			uint32			*p_data32, /* MUST be 32bits alignment addr */
			const uint32	data_len
			);
//------------------------------------------------------------------------------
// Write To NFI FIFO                                                            
//------------------------------------------------------------------------------
status_t  NUTL_FIFO_Write_V5(
			const bool		bUsingDMA,
			const uint32	*p_data32,	/* MUST be 32bits alignment addr */
			const uint32	page_size,	/* MUST be 32bits alignment addr */
			const uint32	spare_size,	/* MUST be 32bits alignment addr */
			NUTL_ProgramFlag_E	flag
			);

void HYNIX_RR_TABLE_READ(void);

//------------------------------------------------------------------------------
// NAND COMMON Function
//------------------------------------------------------------------------------
status_t  NAND_COMMON_CopyBack_V5(
			const uint32  c_timeout,
			const uint32  src_row_addr,
			const uint32  dest_row_addr
			);

status_t  NAND_COMMON_Randomizer_Config(
			bool	use,
			uint32	pageNo
			);
status_t  NAND_COMMON_Randomizer_Drv_Config(
			bool	use,
			uint32	pageNo
			);

status_t  NAND_COMMON_TLC_BlockProgram(
			const uint32	c_timeout,
			U32			blk_index,
			P_U32		sector_buff,
			P_U32		spare_buff
			);

status_t  NAND_COMMON_Reset_For_DaInit(
			const uint32  c_timeout
			);

status_t  NAND_COMMON_ReadID_For_DaInit(
			const uint32  c_timeout,
			uint32* id1,
			uint32* id2
			);

status_t  NAND_COMMON_Switch_Sync(
			bool sync);

void NAND_COMMON_Set_Feature(
			uint8 cmd,
			uint8 addr,
			uint8 *value,
			uint8 bytes);

void NAND_COMMON_Get_Feature(
			uint8 cmd,
			uint8 addr,
			uint8 *value,
			uint8 bytes
			);

#ifdef __cplusplus
}
#endif

#endif
