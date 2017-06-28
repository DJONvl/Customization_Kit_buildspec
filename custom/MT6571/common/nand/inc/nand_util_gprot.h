/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#include "sw_types.h"
#include "mtk_status.h"
#include "nand_dev_tbl.h"
#include "DOWNLOAD.H"

/*-------------------------------------------------------------------------
 * NAND Utility Block Callback APIs (for SEC_RO, FLASHTOOL_CFG, CUST_PARA)
 *-------------------------------------------------------------------------*/

#define __NAND_UTIL_BLOCK_API_CALLBACK

typedef STATUS_E (*F_NAND_UTIL_BLOCK_READ_BLOCK_T)(const uint32 len, const uint32 base_block, uint8 *p_buf);
typedef STATUS_E (*F_NAND_UTIL_BLOCK_ERASE_BLOCK_T)(const uint32 len, const uint32 base_block);
typedef STATUS_E (*F_NAND_UTIL_BLOCK_WRITE_BLOCK_T)(const uint32 len, const uint32 base_block, uint8 *p_buf);

typedef struct
{
    F_NAND_UTIL_BLOCK_READ_BLOCK_T       f_read_block;
    F_NAND_UTIL_BLOCK_ERASE_BLOCK_T      f_erase_block;
    F_NAND_UTIL_BLOCK_WRITE_BLOCK_T      f_write_block;
} NAND_UTIL_BLOCK_Adaption_S;

/*------------------------------------------------
 * NAND Utility Public Callback APIs
 *------------------------------------------------*/

#define __NAND_UTIL_GENERAL_API_CALLBACK

typedef STATUS_E (*F_NAND_UTIL_NFI_INIT_T)(uint32 nfi_acccon, bool bEnableCS1);
typedef STATUS_E (*F_NAND_UTIL_NFI_REINIT_AFTER_BL_T)(void);
typedef STATUS_E (*F_NAND_UTIL_NFI_REINIT_BL_T)(void);
typedef void     (*F_NAND_UTIL_NFI_SET_PAGEFORMAT_T)(void);
typedef STATUS_E (*F_NAND_UTIL_CHECK_BAD_BLOCK_T)(const uint32 c_timeout, const uint32 row_addr);
typedef STATUS_E (*F_NAND_UTIL_REMARK_BAD_BLOCK_T)(void);
typedef STATUS_E (*F_NAND_UTIL_FIND_GOOD_BLOCK_T)(const uint32 c_timeout, uint32 *p_row_addr);
typedef STATUS_E (*F_NAND_UTIL_ERASE_BLOCK_T)(const uint32 c_timeout, const uint32 row_addr, NUTL_EraseFlag_E flag);
typedef STATUS_E (*F_NAND_UTIL_ERASE_BLOCK_EX_T)(const uint32 start_block, const uint32 last_block, uint32 * final_block);
typedef STATUS_E (*F_NAND_UTIL_READ_PAGE_T)(const uint32 c_timeout, const uint32 row_addr, uint32 * p_data32, uint32 * p_spare32, uint32 ecc_parity_from_reg[8], uint32 ecc_parity_from_spare[8], NUTL_ReadFlag_E flag);
typedef uint32   (*F_NAND_UTIL_COPY_PAGE_T)(uint32 row_addr, uint32 * p_buf, uint32 buf_size);
typedef uint32   (*F_NAND_UTIL_COPY_PAGE_EX_T)(uint32 row_addr, uint32 col_addr, uint32 * p_buf, uint32 buf_size);
typedef STATUS_E (*F_NAND_UTIL_PROGRAM_PAGE_T)(const uint32 c_timeout ,const uint32 row_addr, const uint32 * p_data32, uint32 * p_spare32, uint32 ecc_parity_from_reg[8], uint32 ecc_parity_from_spare[8], NUTL_ProgramFlag_E flag);
typedef STATUS_E (*F_NAND_UTIL_REPLACE_BLOCK_T)(const uint32  c_timeout, const uint32 src_row_addr, const uint32 dest_row_addr, const uint32 err_page_index, const uint32 *p_err_page_data32);
typedef STATUS_E (*F_NAND_UTIL_CHECK_RANGE_T)(const uint64 absolute_addr, const uint64 length, const NUTL_AddrTypeFlag_E addr_type);
typedef void     (*F_NAND_UTIL_SET_PAGEBUF_8_T)(NAND_PageBuffer_U * p_pagebuf, uint8 **pp_page, uint8 **pp_spare);
typedef void     (*F_NAND_UTIL_SET_PAGEBUF_32_T)(NAND_PageBuffer_U * p_pagebuf, uint32 **pp_page, uint32 **pp_spare);
typedef STATUS_E (*F_NAND_UTIL_WRITE_DL_INFO)(DL_STATUS_V1* download_info);
typedef STATUS_E (*F_NAND_UTIL_READ_ID)(const uint32 c_timeout, uint32* id1, uint32* id2);

typedef struct
{
    // for NFI specific
    F_NAND_UTIL_NFI_INIT_T                 f_nfi_init;
    F_NAND_UTIL_NFI_REINIT_BL_T            f_nfi_reinit_bl;
    F_NAND_UTIL_NFI_REINIT_AFTER_BL_T      f_nfi_reinit_after_bl;
    F_NAND_UTIL_NFI_SET_PAGEFORMAT_T       f_nfi_set_page_format;

    // for block operations
    F_NAND_UTIL_FIND_GOOD_BLOCK_T          f_find_good_block;
    F_NAND_UTIL_CHECK_BAD_BLOCK_T          f_check_bad_block;
    F_NAND_UTIL_REMARK_BAD_BLOCK_T         f_remark_bad_block;
    F_NAND_UTIL_ERASE_BLOCK_T              f_erase_block;
    F_NAND_UTIL_ERASE_BLOCK_EX_T           f_erase_block_ex;
    F_NAND_UTIL_REPLACE_BLOCK_T            f_replace_block;

    // for page operations
    F_NAND_UTIL_COPY_PAGE_T                f_copy_page;
    F_NAND_UTIL_COPY_PAGE_EX_T             f_copy_page_ex;
    F_NAND_UTIL_READ_PAGE_T                f_read_page;
    F_NAND_UTIL_PROGRAM_PAGE_T             f_program_page;

    // for buffer configurations
    F_NAND_UTIL_SET_PAGEBUF_8_T            f_set_pagebuf_8;
    F_NAND_UTIL_SET_PAGEBUF_32_T           f_set_pagebuf_32;

    // other utilities
    F_NAND_UTIL_CHECK_RANGE_T              f_check_range;
    F_NAND_UTIL_READ_ID                    f_read_id;
    F_NAND_UTIL_WRITE_DL_INFO              f_write_dl_info;

} NAND_UTIL_Adaption_S;

/*------------------------------------------------
 * NAND Utility OTP Callback APIs
 *------------------------------------------------*/

#define __NAND_UTIL_OTP_API_CALLBACK

typedef STATUS_E (*F_NAND_UTIL_OTP_CHECK_DEVICE_T)(void);
typedef STATUS_E (*F_NAND_UTIL_OTP_GET_SIZE_T)(void);
typedef STATUS_E (*F_NAND_UTIL_OTP_LOCK_T)(void);
typedef STATUS_E (*F_NAND_UTIL_OTP_LOCK_CHECK_STATUS_T)(void);
typedef STATUS_E (*F_NAND_UTIL_OTP_READ_PAGE_T)(const uint32 otp_offset, uint32  * p_page);
typedef STATUS_E (*F_NAND_UTIL_OTP_PROGRAM_PAGE_T)(const uint32 otp_offset, uint32 * p_page);
typedef STATUS_E (*F_NAND_UTIL_OTP_PROGRAM_CHECK_STATUS_T)(void);
typedef STATUS_E (*F_NAND_UTIL_OTP_SEARCH_PHYSICAL_ADDR_T)(const uint32 otp_offset, volatile uint32 *phy_row_addr, volatile uint32 *phy_offset);

typedef struct
{
    F_NAND_UTIL_OTP_CHECK_DEVICE_T             f_otp_check_device;
    F_NAND_UTIL_OTP_GET_SIZE_T                 f_get_size;
    F_NAND_UTIL_OTP_LOCK_T                     f_lock;
    F_NAND_UTIL_OTP_LOCK_CHECK_STATUS_T        f_lock_check_status;
    F_NAND_UTIL_OTP_READ_PAGE_T                f_read_page;
    F_NAND_UTIL_OTP_PROGRAM_PAGE_T             f_program_page;
    F_NAND_UTIL_OTP_PROGRAM_CHECK_STATUS_T     f_program_check_status;
    F_NAND_UTIL_OTP_SEARCH_PHYSICAL_ADDR_T     f_search_physical_addr;
} NAND_UTIL_OTP_Adaption_S;


