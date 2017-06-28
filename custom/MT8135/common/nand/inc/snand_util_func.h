/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#ifndef _SNAND_UTIL_FUNC_H_
#define _SNAND_UTIL_FUNC_H_

#include "SW_TYPES.H"
#include "DOWNLOAD.H"
#include "nand_dev_tbl.h"
#include "main_nand.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// NFI Initialization
//------------------------------------------------------------------------------
STATUS_E  SNUTL_NFI_Init(uint32  nfi_acccon, bool bEnableCS1);

//------------------------------------------------------------------------------
// NFI Enable Chip Select 1
//------------------------------------------------------------------------------
STATUS_E  SNUTL_NFI_SelectCS(bool bEnableCS1);

//------------------------------------------------------------------------------
// Set NFI Page Format
//------------------------------------------------------------------------------
void	SNUTL_NFI_SetPageFormat(void);

//------------------------------------------------------------------------------
// Read ID
//------------------------------------------------------------------------------
STATUS_E  SNUTL_ReadID(const uint32 c_timeout
                ,uint16 * p_maker_code
                ,uint16 * p_device_code
                ,uint16 * p_ext_code1
                ,uint16 * p_ext_code2
				,uint16 * p_ext_code3);

//------------------------------------------------------------------------------
// Bad Block Check
//------------------------------------------------------------------------------
STATUS_E  SNUTL_BadBlock_Check(
				const uint32  c_timeout
				,const uint32  row_addr);

//------------------------------------------------------------------------------
// Find Good Block
//------------------------------------------------------------------------------
STATUS_E  SNUTL_FindGoodBlock(
				const uint32  c_timeout
				,uint32  *p_row_addr);

//------------------------------------------------------------------------------
// Block Erase
//------------------------------------------------------------------------------
STATUS_E  SNUTL_BlockErase(
				const uint32  c_timeout
				,const uint32  row_addr
				,NUTL_EraseFlag_E	flag);

STATUS_E  SNUTL_BlockEraseEx(
				const uint32 start_block
				,const uint32 last_block
				,uint32 * final_block);

//------------------------------------------------------------------------------
// Spare Area Compose/Decompose Related Function
//------------------------------------------------------------------------------
STATUS_E  SNUTL_SpareCompose(
				const uint32  ecc_parity_from_reg[8]
				,uint32  *p_spare32 /* MUST be 32bits alignment addr */);

STATUS_E  SNUTL_SpareDecompose(
				const uint32  *p_spare32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_spare[8]);

//------------------------------------------------------------------------------
// ECC Correction
//------------------------------------------------------------------------------
STATUS_E  SNUTL_ECC_Correction(
				const uint32  ecc_block_size
				,const uint32  ecc_parity_from_reg[8]
				,const uint32  ecc_parity_from_spare[8]
				,uint32  *p_data32 /* MUST be 32bits alignment addr */);

//------------------------------------------------------------------------------
// Page Read
//------------------------------------------------------------------------------
STATUS_E  SNUTL_PageRead(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32 *p_spare32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
				,uint32  ecc_parity_from_spare[8]
				,NUTL_ReadFlag_E  flag);

//------------------------------------------------------------------------------
// Page Program Related Callback Function
//------------------------------------------------------------------------------
STATUS_E  SNUTL_PageProgram(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32 *p_spare32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
				,uint32  ecc_parity_from_spare[8]
				,NUTL_ProgramFlag_E flag);

//------------------------------------------------------------------------------
// CopyBack
//------------------------------------------------------------------------------
STATUS_E  SNUTL_CopyBack(
				const uint32  c_timeout
				,const uint32  src_row_addr
				,const uint32  dest_row_addr);

//------------------------------------------------------------------------------
// Block Replacement
//------------------------------------------------------------------------------
STATUS_E  SNUTL_BlockReplacement(
				const uint32  c_timeout
				,const uint32  src_row_addr /* any row_addr within source block */
				,const uint32  dest_row_addr /* any row_addr within destination block */
				,const uint32  err_page_index /* page index within block, 1st page index is 0 */
				,const uint32  *p_err_page_data32 /* MUST be 32bits alignment addr */);

STATUS_E  SNUTL_BlockCopy(
				const uint32  c_timeout
				,const uint32  src_row_addr /* any row_addr within source block */
				,const uint32  dest_row_addr /* any row_addr within destination block */);

//------------------------------------------------------------------------------
// Range Validation
//------------------------------------------------------------------------------
STATUS_E  SNUTL_CheckRange(
				const uint32  absolute_addr
				,const uint32  length
				,const NUTL_AddrTypeFlag_E addr_type);

//------------------------------------------------------------------------------
// Read/Erase/Write Blocks for SEC_RO, FLASHTOOL_CFG, CUST_PARA
//------------------------------------------------------------------------------
extern STATUS_E  SNUTL_ReadBlocks(
				const uint32 len
				,const uint32 base_block
				,uint8 *p_buf);

extern STATUS_E  SNUTL_EraseBlocks(
				const uint32 len
				,const uint32 base_block);

extern STATUS_E  SNUTL_WriteBlocks(
				const uint32 len
				,const uint32 base_block
				,uint8 *p_buf);

extern void SNUTL_SetPageBuf8 (NAND_PageBuffer_U * p_pagebuf, uint8  **pp_page, uint8  **pp_spare);
extern void SNUTL_SetPageBuf32(NAND_PageBuffer_U * p_pagebuf, uint32 **pp_page, uint32 **pp_spare);

extern STATUS_E  SNUTL_RandomRead(uint32 row_addr, uint32 col_addr, uint32 * output_buffer, uint32 read_size);
extern uint32	 SNUTL_PageCopy(uint32 row_addr, uint32 * p_buf, uint32 buf_size);
extern uint32	 SNUTL_PageCopy_Ex(uint32 row_addr, uint32 col_addr, uint32 * p_buf, uint32 buf_size);

/********************************************************************************/
//OTP_APIs
/********************************************************************************/

extern STATUS_E SNUTL_OTP_CheckDevice(void);
extern uint32   SNUTL_OTP_GetSize(void);
extern bool     SNUTL_OTP_SearchBaseAddr(const uint32 otp_offset, uint32 *otp_blk_addr, uint32 *otp_blk_size);
extern STATUS_E SNUTL_OTP_Page_Read(const uint32 otp_offset, uint32  * p_page);
extern STATUS_E SNUTL_OTP_Page_Program(const uint32 otp_offset, uint32 * p_page);
extern STATUS_E SNUTL_OTP_Program_CheckDone(void);
extern STATUS_E SNUTL_OTP_Lock(void);
extern STATUS_E SNUTL_OTP_Lock_CheckStatus(void);
extern int      mtk_snand_init(void);
extern void     mtk_snand_dev_ecc_control(uint8 enable);
extern void     mtk_snand_dev_unlock_all_blocks(void);


#ifdef __cplusplus
}
#endif

#endif

