/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _FLASH_UTIL_FUNC_H_
#define _FLASH_UTIL_FUNC_H_

#include "hw_config.h"
#include "flash_dev_tbl.h"
#include "autoconfig.h"


#define CB_NOR_CALLBACK_GID()							g_FlashDev.m_cmd->m_cb_gid
#define CB_CHECK_DEV_ID(addr1,addr2)				       g_FlashDev.m_cmd->m_cb_chk_dev_id(addr1,addr2)
#define CB_CHECK_DEV_IDLE(addr)					       g_FlashDev.m_cmd->m_cb_chk_dev_idle(addr)
#define CB_SECTOR_ERASE(blockaddr)						g_FlashDev.m_cmd->m_cb_sec_erase(blockaddr)
#define CB_SECTOR_ERASE_CHECK_DONE(blockaddr)			g_FlashDev.m_cmd->m_cb_sec_erase_chk_done(blockaddr)
#define CB_PROGRAM_PRE_PROCESS()						g_FlashDev.m_cmd->m_cb_pgm_pre_process()
#define CB_PROGRAM_POST_PROCESS()						g_FlashDev.m_cmd->m_cb_pgm_post_process()
#define CB_PROGRAM_ENTER(blockaddr)						g_FlashDev.m_cmd->m_cb_pgm_enter(blockaddr)
#define CB_PROGRAM_EXIT(blockaddr)						g_FlashDev.m_cmd->m_cb_pgm_exit(blockaddr)
#define CB_PROGRAM(ba, pa, data)						g_FlashDev.m_cmd->m_cb_pgm(ba, pa, data)
#define CB_PROGRAM_CHECK_DONE(prog_addr)				g_FlashDev.m_cmd->m_cb_pgm_check_done(prog_addr)
#define CB_BUF_PROGRAM(ba, pa, data, len)				g_FlashDev.m_cmd->m_cb_buf_pgm(ba, pa, data, len)
#define CB_BUF_PROGRAM_CHECK_DONE(prog_addr)			g_FlashDev.m_cmd->m_cb_buf_pgm_check_done(prog_addr)
#define BUFPGM_SUPPORT()								((NULL!=g_FlashDev.m_cmd->m_cb_buf_pgm) && (NULL!=g_FlashDev.m_cmd->m_cb_buf_pgm_check_done))

#define FLASH_DIE(die_num)								(g_FlashDev.m_hw_info.m_die[die_num])
#define FLASH_DIE_LAYOUT(die_num)						(g_FlashDev.m_hw_info.m_die[die_num].m_die_layout)
#define FLASH_DIE_SIZE(die_num)							(g_FlashDev.m_hw_info.m_die[die_num].m_die_layout.m_size)
#define FLASH_DIE_COUNT()								(g_FlashDev.m_hw_info.m_die_count)

#define REGION_BASE_OFFSET(die_num, region)				(g_FlashDev.m_hw_info.m_die[die_num].m_die_layout.m_region_map[region].m_base_offset)
#define REGION_SECTOR_COUNT(die_num, region)			(g_FlashDev.m_hw_info.m_die[die_num].m_die_layout.m_region_map[region].m_sector_count)
#define REGION_SECTOR_SIZE(die_num, region)				(g_FlashDev.m_hw_info.m_die[die_num].m_die_layout.m_region_map[region].m_sector_size)
#define REGION_SIZE(die_num, region)					(REGION_SECTOR_COUNT(die_num, region)*REGION_SECTOR_SIZE(die_num, region))
#define IF_EMPTY_REGION(die_num, region)				((0==(g_FlashDev.m_hw_info.m_die[die_num].m_die_layout.m_region_map[region].m_sector_count))?TRUE:FALSE)

//
//OTP CB
//
#define CB_OTP_ENTER(ba)								g_FlashDev.m_otp_cmd->m_cb_otp_enter(ba)
#define CB_OTP_READ(ba, pa, data)						g_FlashDev.m_otp_cmd->m_cb_otp_read(ba, pa, data)
#define CB_OTP_PROGRAM(ba, pa, data)					g_FlashDev.m_otp_cmd->m_cb_otp_pgm(ba, pa, data)
#define CB_OTP_LOCK(ba, pa, data)						g_FlashDev.m_otp_cmd->m_cb_otp_lock(ba, pa, data)
#define CB_OTP_LOCK_CHECKSTATUS(ba, pa, data)			g_FlashDev.m_otp_cmd->m_cb_otp_lock_checkstatus(ba, pa, data)
#define CB_OTP_EXIT(ba)									g_FlashDev.m_otp_cmd->m_cb_otp_exit(ba)

#define FLASH_OTP_LAYOUT()								(g_FlashDev.m_hw_info.m_otp_layout)
#define FLASH_OTP_SIZE()								(g_FlashDev.m_hw_info.m_otp_layout->m_size)

#define OTP_REGION_BASE_OFFSET(region)					(g_FlashDev.m_hw_info.m_otp_layout->m_region_map[region].m_base_offset)
#define OTP_REGION_SECTOR_COUNT(region)					(g_FlashDev.m_hw_info.m_otp_layout->m_region_map[region].m_sector_count)
#define OTP_REGION_SECTOR_SIZE(region)					(g_FlashDev.m_hw_info.m_otp_layout->m_region_map[region].m_sector_size)
#define OTP_REGION_SIZE(region)							(REGION_SECTOR_COUNT(region)*REGION_SECTOR_SIZE(region))

#ifdef __cplusplus
extern "C" {
#endif

extern bool			FUTL_CheckRange(const uint32 start_addr, const uint32 length);
extern bool			FUTL_SearchBaseAddr(const uint32 addr, uint32 *blk_addr, uint32 *blk_size);
extern uint32		FUTL_GetFlashDieBaseAddr(const uint16 die_num);
extern uint32		FUTL_GetFlashSize(void);
extern bool         FUTL_ConvertBlockIndexToAddress(uint32 blockIndex,
                                                    uint32 *address);
extern bool         FUTL_ConvertAddressToBlockIndex(uint32 address,
                                                    uint32 *blockIndex);
extern STATUS_E		FUTL_EraseSector(const uint32  blk_addr, const uint32 blk_size, bool bValidation);
extern STATUS_E		FUTL_ReadSector(const uint32  blk_addr, const uint32  blk_size, uint8 *p_buf);
extern STATUS_E		FUTL_WriteSector(const uint32  blk_addr, const uint32  blk_size, const uint8 *p_buf);
extern STATUS_E     FUTL_WriteSectorWithOffset(uint32 blk_addr,
                                               uint32 offset,
                                               uint32 buf_len,
                                               const uint8 *p_buf);
extern STATUS_E		FUTL_Erase_CheckDone(const uint32 blockaddr, const uint32 blocksize, bool bValidation);
extern STATUS_E		FUTL_Program_CheckDone(const uint32 prog_addr, const uint16 verify_data);
extern STATUS_E		FUTL_Buf_Program_CheckDone(const uint32 prog_addr, const uint16 *verify_data, const uint32 length_in_word);
extern STATUS_E		FUTL_EraseSectors(const uint32 addr, const uint32 len, bool bValidation);
extern STATUS_E		FUTL_ReadSectors(const uint32 addr, const uint32 len, uint8 *p_buf, uint32  *p_base_sector_addr, uint32  *p_total_sector_size);
extern STATUS_E		FUTL_WriteSectors(const uint32 sector_addr, const uint32 total_sector_size, const uint8 *p_buf);

/********************************************************************************/
//OTP_APIs
/********************************************************************************/

extern STATUS_E FUTL_OTP_CheckDevice(void);
extern uint32 FUTL_OTP_GetSize(void);
extern bool FUTL_OTP_SearchBaseAddr(const uint32 otp_offset, uint32 *otp_blk_addr, uint32 *otp_blk_size);
extern STATUS_E FUTL_OTP_Read(const uint32 otp_offset, volatile uint16  * read_data);
extern STATUS_E FUTL_OTP_Program(const uint32 otp_offset, const uint16 verify_data);
extern STATUS_E FUTL_OTP_Program_CheckDone(const uint32 otp_offset, const uint16 verify_data);
extern STATUS_E FUTL_OTP_Lock(void);
extern STATUS_E FUTL_OTP_Lock_CheckStatus(void);

#define	INTEL_SIBLEY_PGM_REGION_SIZE_IN_BYTE	1024

#define	MAX_BUFPGM_SIZE_IN_WORD		(INTEL_SIBLEY_PGM_REGION_SIZE_IN_BYTE/2)	// For INTEL Sibley, the max buffer is up to 1KB 

extern uint16	g_FLASH_PGM_BUF[MAX_BUFPGM_SIZE_IN_WORD];
extern uint32	g_FLASH_MAX_BUFPGM_SIZE_IN_BYTE;
extern uint32	g_FLASH_CUR_PGM_COUNT;
extern uint32	g_FLASH_CUR_PGM_ADDR;
extern uint32	g_FLASH_LAST_PGM_COUNT;
extern uint32	g_FLASH_LAST_PGM_ADDR;

#ifdef __cplusplus
}
#endif


#endif
