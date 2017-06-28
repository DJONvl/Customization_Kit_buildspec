/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _NAND_DEV_TBL_H_
#define _NAND_DEV_TBL_H_

#include "SW_TYPES.H"
#include "DOWNLOAD.H"

#ifdef __cplusplus
extern "C" {
#endif

#define __NFI_SUPPORT_TLC__

#define NFI_MEM_OPTIMIZE //reduce memcopy operation

#define NAND_MAX_SPARE_SIZE_BYTE		2048
#define NAND_MAX_OTP_REGION_PER_DIE		4

// DO NOT modify the IO BITS value, because it must be the same as BootROM definition 
#define NAND_IO_8BITS				0
#define NAND_IO_16BITS				1
#define NAND_IO_TOGGLE				2
#define NAND_IO_ONFI				4
#define NAND_IO_BITS_MASK                1

//nand type
#define NAND_FLASH_SLC          (0x0000)
#define NAND_FLASH_MLC         (0x0001)
#define NAND_FLASH_TLC          (0x0002)
#define NAND_FLASH_AMLC          (0x0003)
#define NAND_FLASH_MASK       (0x00FF)

typedef struct {
	uint16		m_maker_code;
	uint16		m_device_code;
    /* Koshi, 2011.05.20,  { */
    uint8		m_identify_code;
    uint8		m_feature_code;
    /* Koshi, 2011.05.20,  } */	
    uint8       m_plane_code;
#if defined(BB_MT6580)|| defined(BB_MT6572)
    uint8       ext_code4;
    uint8       ext_code5;
#else
    uint8       _align;
#endif
} NAND_ID_S;

typedef struct {
	uint16		m_enable;
	uint16		m_cmd;
} NAND_Command_S;

typedef struct {
	NAND_Command_S			m_read_id;
	NAND_Command_S			m_status;
	NAND_Command_S			m_reset;
	NAND_Command_S			m_read;
	NAND_Command_S			m_read_spare;
	NAND_Command_S			m_read_cnf;
	NAND_Command_S			m_program_1st_half_page;
	NAND_Command_S			m_program;
	NAND_Command_S			m_program_cnf;
	NAND_Command_S			m_erase;
	NAND_Command_S			m_erase_cnf;
	// for SPI-NAND specific
	NAND_Command_S			m_get_feature;
	NAND_Command_S			m_set_feature;
	NAND_Command_S			m_write_enable;
} NAND_CommandSet_S;

struct _NAND_DeviceInfo_S;
typedef struct _NAND_DeviceInfo_S  NAND_DeviceInfo_S;
typedef STATUS_E  (*FP_CB_NAND_READ_ID_T)(const uint32  c_timeout, uint32* id1, uint32* id2);
typedef STATUS_E  (*FP_CB_NAND_RESET_T)(const uint32  c_timeout);
typedef STATUS_E  (*FP_CB_NAND_PRE_PROCESS_T)(const uint32  c_timeout);
typedef STATUS_E  (*FP_CB_NAND_READ_STATUS_T)(const uint32  c_timeout);
typedef STATUS_E  (*FP_CB_NAND_BLOCK_ERASE_T)(const uint32  row_addr);
typedef STATUS_E  (*FP_CB_NAND_BAD_BLOCK_SYMBOL_CHECK_T)(const uint32  *p_spare32);
typedef STATUS_E  (*FP_CB_NAND_BAD_BLOCK_SYMBOL_SET_T)(uint32  *p_spare32);
typedef STATUS_E  (*FP_CB_NAND_PAGE_READ_T)(const uint32  c_timeout, const uint32 row_addr, uint32 *p_data32, uint32* p_spare32, uint32  ecc_parity_from_reg[8]);
typedef STATUS_E  (*FP_CB_NAND_PAGE_PROGRAM_T)(const uint32  c_timeout, const uint32 row_addr, const uint32 *p_data32, const uint32 *p_spare32, uint32  ecc_parity_from_reg[8]);
typedef STATUS_E  (*FP_CB_NAND_SPARE_READ_T)(const uint32  c_timeout, const uint32 row_addr, uint32 *p_spare32);
typedef STATUS_E  (*FP_CB_NAND_SPARE_PROGRAM_T)(const uint32  c_timeout, const uint32 row_addr, const uint32 *p_spare32);
typedef STATUS_E  (*FP_CB_NAND_PAGE_SPARE_PROGRAM_T)(uint32 row_addr, const uint32 *p_page_spare);
typedef STATUS_E  (*FP_CB_NAND_RANDOMIZER_CONFIG)(bool use,uint32 page);
typedef STATUS_E  (*FP_CB_NAND_BLOCK_PROGRAM_T)(const uint32  c_timeout, const uint32 blk_addr, const uint32 *p_data32, const uint32 *p_spare32);
typedef STATUS_E  (*FP_CB_BAD_NAND_PAGE_PROGRAM_T)(const uint32  c_timeout, const uint32 row_addr, const uint32 *p_data32, const uint32 *p_spare32, uint32  ecc_parity_from_reg[8]);
typedef STATUS_E  (*FP_CB_NAND_BLOCK_CHARGE)(const uint32  row_addr);

typedef struct {
	NAND_CMD_Callback_ID_E					m_cb_gid;
	FP_CB_NAND_READ_ID_T					m_cb_read_id;
	FP_CB_NAND_RESET_T						m_cb_reset;
	FP_CB_NAND_PRE_PROCESS_T				m_cb_pre_process;
	FP_CB_NAND_READ_STATUS_T				m_cb_read_status;
	FP_CB_NAND_BLOCK_ERASE_T				m_cb_block_erase;
	FP_CB_NAND_BAD_BLOCK_SYMBOL_CHECK_T		m_cb_bad_block_symbol_check;
	FP_CB_NAND_BAD_BLOCK_SYMBOL_SET_T		m_cb_bad_block_symbol_set;
	FP_CB_NAND_PAGE_READ_T					m_cb_page_read;
	FP_CB_NAND_PAGE_PROGRAM_T				m_cb_page_program;
	FP_CB_NAND_SPARE_READ_T					m_cb_spare_read;
	FP_CB_NAND_SPARE_PROGRAM_T				m_cb_spare_program;
	FP_CB_NAND_PAGE_SPARE_PROGRAM_T			m_cb_page_spare_program;
	FP_CB_NAND_RANDOMIZER_CONFIG			m_cb_page_randomizer_config;
	FP_CB_NAND_BLOCK_PROGRAM_T                   m_cb_block_program;      
	FP_CB_BAD_NAND_PAGE_PROGRAM_T                   m_cb_bad_program;
	FP_CB_NAND_BLOCK_CHARGE                   m_cb_block_charge;
} NAND_CMD_Callback_S;

// OTP

typedef STATUS_E  (*FP_CB_NAND_OTP_ENTER_T)(const uint32  c_timeout);
typedef STATUS_E  (*FP_CB_NAND_OTP_PAGE_READ_T)(const uint32  c_timeout, const uint32 row_addr, uint32 *p_data32, uint32  ecc_parity_from_reg[8]);
typedef STATUS_E  (*FP_CB_NAND_OTP_PAGE_PROGRAM_T)(const uint32  c_timeout, const uint32 row_addr, const uint32 *p_data32, uint32  ecc_parity_from_reg[8]);
typedef STATUS_E  (*FP_CB_NAND_OTP_LOCK_T)(const uint32  c_timeout);
typedef STATUS_E  (*FP_CB_NAND_OTP_LOCK_CHECKSTATUS_T)(const uint32  c_timeout);
typedef STATUS_E  (*FP_CB_NAND_OTP_EXIT_T)(const uint32  c_timeout);

typedef struct {
	FP_CB_NAND_OTP_ENTER_T					m_cb_otp_enter;
	FP_CB_NAND_OTP_PAGE_READ_T				m_cb_otp_page_read;
	FP_CB_NAND_OTP_PAGE_PROGRAM_T			m_cb_otp_page_program;
	FP_CB_NAND_OTP_LOCK_T					m_cb_otp_lock;
	FP_CB_NAND_OTP_LOCK_CHECKSTATUS_T		m_cb_otp_lock_checkstatus;
	FP_CB_NAND_OTP_EXIT_T					m_cb_otp_exit;
} NAND_OTP_CMD_Callback_S;

typedef struct {
	// sector layout 
	uint32		m_region_base;
	uint32		m_page_count;
	uint32		m_region_offset;
} NAND_SectorRegion_S;

typedef struct {
	// total size (in pages) on this die 
	uint32		m_total_pages;

	// grouped sector region map 
	NAND_SectorRegion_S	m_region_map[NAND_MAX_OTP_REGION_PER_DIE];

} NAND_OTP_Layout_S;

typedef struct {

	// nand flash H/W manufacture id and device code 
	NAND_ID_S	m_id;

	// total size in MB 
	uint64		m_total_size;
	// pages per block 
	uint32		m_pages_per_block;
	// page size in Byte 
	uint32		m_page_size;
	// I/O interface: 8bits or 16bits 
	uint32		m_io_interface;
	// address cycle 
	uint32		m_addr_cycle;
	// spare size per sector
	uint32		m_spare_size_per_sector;
	// chip number
	uint32      m_chip_num;

	uint32      m_sector_size;
} NAND_HW_Info_S;

typedef struct {
	uint16 low_page;
	uint16 high_page;
}PPAGE_ELEMENT;

typedef struct {
	PPAGE_ELEMENT table[256];
}NAND_PPAGE_TBL;

typedef struct{
	const uint32                         acccon;
	const uint32                         acccon1;
	const uint32                         dqs_mux;
	const uint32                         dqs_ctrl;
	const uint32                         pre_dqs;
	const uint32                         set_feature;
	const uint32                         get_feature;
	const uint32                         read_retry_num;
	const uint32                         read_retry_default;
	const uint32                         read_retry_address;
	const uint32                         interface_address;
	const uint32                         interface_async;
	const uint32                         interface_sync;
	const NAND_PPAGE_TBL        m_ppage_tbl;
}NAND_MLC_FEATURE;;

typedef struct
{
	bool         slcopmodeEn;               //bool,TRUE: slc mode   FALSE: tlc mode
	bool		 normalslcopen;
	bool         pPlaneEn;                     //bool, this chip has pseudo plane
	bool         needchangecolumn;    //bool, read page with change column address command
	bool         normaltlc;        //bool,whether need 09/0d 01/02/03 tlc command, whether need wl and page exchange.  micron no need
	uint32         en_slc_mode_cmd;     //enable slc mode cmd
	uint32         dis_slc_mode_cmd;    //disable slc mode cmd: 0xff is invalid
	uint32		normal_slc_mode_cmd;
       uint32           ecc_required;
	bool         ecc_recalculate_en;  //for nfi config;bool
	uint32           block_bit;                    //block address start bit;
	uint32           pPlane_bit;                  //pesudo plane bit;
	uint8             fdm_size;
} NFI_TLC_FEATURE;

typedef struct {
	// nand flash id 
	NAND_DeviceID_E			m_id;
	// NAND flash device H/W info 
	NAND_HW_Info_S				m_hw_info;

       //nand type
       const uint16                         m_nand_type;

	//tlc control
	NFI_TLC_FEATURE *m_tlc_setting;
	
	// command set 
	const NAND_CommandSet_S		*m_cmd_set;
	// callback function set 
	const NAND_CMD_Callback_S	*m_cb_func_set;
	
	const NAND_MLC_FEATURE        *m_mlc_setting;
	
	//OTP layout
	const NAND_OTP_Layout_S			*m_otp_layout;
	// OTP callback function set 
	const NAND_OTP_CMD_Callback_S	*m_otp_cb_func_set;
} NAND_Device_S;

struct _NAND_DeviceInfo_S {

	// total page count 
	uint32				m_total_pages;
	// total block count 
	uint32				m_total_blocks;
	// block size in Byte 
	uint32				m_block_size;
	// spare size in Byte 
	uint32				m_spare_size;
	// page addr shift bits 
	uint32				m_page_addr_shift_bits;
	// block addr shift bits 
	uint32				m_block_addr_shift_bits;
#if defined(__NFI_SUPPORT_TLC__)
	// for TLC erase level purpose
	bool                             m_eraselevel_support;
#endif
	// NAND flash H/W info 
	const NAND_Device_S		*m_dev;
};

//For DA_INIT, g_NUTL_PageRead_Var and g_NUTL_PageProgram_Var are NOT used.
//So define below fake structure to reduce their size in ZI section.
#ifdef DA_INIT  
typedef struct {
	uint8	m_data[1];
	uint8	m_spare[1];
} NAND_Page512_8_S;

typedef struct {
	uint16	m_data[1];
	uint16	m_spare[1];
} NAND_Page512_16_S;

typedef struct {
	uint32	m_data[1];
	uint32	m_spare[1];
} NAND_Page512_32_S;
//used by  MT8135 sector size = 1024
typedef struct {
	uint8	m_data[1];
	uint8	m_spare[1];
} NAND_Page1024_8_S;

typedef struct {
	uint16	m_data[1];
	uint16	m_spare[1];
} NAND_Page1024_16_S;

typedef struct {
	uint32	m_data[1];
	uint32	m_spare[1];
} NAND_Page1024_32_S;
//
typedef struct {
	uint8	m_data[1];
	uint8	m_spare[1];
} NAND_Page2048_8_S;

typedef struct {
	uint16	m_data[1];
	uint16	m_spare[1];
} NAND_Page2048_16_S;

typedef struct {
	uint32	m_data[1];
	uint32	m_spare[1];
} NAND_Page2048_32_S;

//MT6516 FPGA
typedef struct {
	uint8	m_data[1];
	uint8	m_spare[1];
} NAND_Page4096_8_S;

typedef struct {
	uint16	m_data[1];
	uint16	m_spare[1];
} NAND_Page4096_16_S;

typedef struct {
	uint32	m_data[1];
	uint32	m_spare[1];
} NAND_Page4096_32_S;
//
//MT8135 FPGA -- page size = 8192
typedef struct {
	uint8	m_data[1];
	uint8	m_spare[1];
} NAND_Page8192_8_S;
typedef struct {
	uint16	m_data[1];
	uint16	m_spare[1];
} NAND_Page8192_16_S;

typedef struct {
	uint32	m_data[1];
	uint32	m_spare[1];
} NAND_Page8192_32_S;
//MT8135 FPGA -- page size = 16384
typedef struct {
	uint8	m_data[1];
	uint8	m_spare[1];
} NAND_Page16384_8_S;
typedef struct {
	uint16	m_data[1];
	uint16	m_spare[1];
} NAND_Page16384_16_S;

typedef struct {
	uint32	m_data[1];
	uint32	m_spare[1];
} NAND_Page16384_32_S;


typedef union {
	uint8				m_raw8[1];
	uint16				m_raw16[1];
	uint32				m_raw32[1];

	// NFIv1, v2, v3, v4
	NAND_Page512_8_S	m_pagespare8;
	NAND_Page512_16_S	m_pagespare16;
	NAND_Page512_32_S	m_pagespare32;
} NAND_Page512_U;

typedef union {
	uint8				m_raw8[1];
	uint16				m_raw16[1];
	uint32				m_raw32[1];

	// PAGE: NFIv1, v2
	NAND_Page2048_8_S	m_pagespare8;
	NAND_Page2048_16_S	m_pagespare16;
	NAND_Page2048_32_S	m_pagespare32;

	// SECTOR: NFIv3, v4
	NAND_Page512_8_S	m_sectorspare8[1];
	NAND_Page512_16_S	m_sectorspare16[1];
	NAND_Page512_32_S	m_sectorspare32[1];	
} NAND_Page2048_U;

//MT6516 FPGA
typedef union {
	uint8				m_raw8[1];
	uint16				m_raw16[1];
	uint32				m_raw32[1];

	NAND_Page4096_8_S	m_pagespare8;
	NAND_Page4096_16_S	m_pagespare16;
	NAND_Page4096_32_S	m_pagespare32;

       NAND_Page512_8_S	m_sectorspare8[1];
	NAND_Page512_16_S	m_sectorspare16[1];
	NAND_Page512_32_S	m_sectorspare32[1];
} NAND_Page4096_U;

//MT8135 FPGA -page size 8192
typedef union {
	uint8				m_raw8[1];
	uint16				m_raw16[1];
	uint32				m_raw32[1];

	NAND_Page8192_8_S	m_pagespare8;
	NAND_Page8192_16_S	m_pagespare16;
	NAND_Page8192_32_S	m_pagespare32;

       NAND_Page1024_8_S	m_sectorspare8[1];
	NAND_Page1024_16_S	m_sectorspare16[1];
	NAND_Page1024_32_S	m_sectorspare32[1];
} NAND_Page8192_U;

//MT8135 FPGA -page size 16K
typedef union {
	uint8				m_raw8[1];
	uint16				m_raw16[1];
	uint32				m_raw32[1];

	NAND_Page16384_8_S	m_pagespare8;
	NAND_Page16384_16_S	m_pagespare16;
	NAND_Page16384_32_S	m_pagespare32;

       NAND_Page1024_8_S	m_sectorspare8[1];
	NAND_Page1024_16_S	m_sectorspare16[1];
	NAND_Page1024_32_S	m_sectorspare32[1];
} NAND_Page16384_U;

typedef union {
	NAND_Page512_U		m_512;
	NAND_Page2048_U		m_2048;
	NAND_Page4096_U		m_4096;
	NAND_Page8192_U		m_8192;
	NAND_Page16384_U      m_16384;
} NAND_PageBuffer_U;
#else
typedef struct {
	uint8	m_data[512];
	uint8	m_spare[16*2];
} NAND_Page512_8_S;

typedef struct {
	uint16	m_data[256];
	uint16	m_spare[8*2];
} NAND_Page512_16_S;

typedef struct {
	uint32	m_data[128];
	uint32	m_spare[4*2];
} NAND_Page512_32_S;
//used by  MT8135 sector size = 1024
typedef struct {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint8	m_data[1024];
	uint8	m_spare[128];
#else
	uint8	m_data[1];
	uint8	m_spare[1];
#endif
} NAND_Page1024_8_S;

typedef struct {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint16	m_data[512];
	uint16	m_spare[64];
#else
	uint16	m_data[1];
	uint16	m_spare[1];
#endif
} NAND_Page1024_16_S;

typedef struct {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint32	m_data[256];
	uint32	m_spare[32];
#else
	uint32	m_data[1];
	uint32	m_spare[1];
#endif
} NAND_Page1024_32_S;
//
typedef struct {
	uint8	m_data[2048];
	uint8	m_spare[64*2];
} NAND_Page2048_8_S;

typedef struct {
	uint16	m_data[1024];
	uint16	m_spare[32*2];
} NAND_Page2048_16_S;

typedef struct {
	uint32	m_data[512];
	uint32	m_spare[16*2];
} NAND_Page2048_32_S;

//MT6516 FPGA
typedef struct {
	uint8	m_data[4096];
	uint8	m_spare[128*2];
} NAND_Page4096_8_S;

typedef struct {
	uint16	m_data[2048];
	uint16	m_spare[64*2];
} NAND_Page4096_16_S;

typedef struct {
	uint32	m_data[1024];
	uint32	m_spare[32*2];
} NAND_Page4096_32_S;
//
//MT8135 FPGA -- page size = 8192
typedef struct {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint8	m_data[8192];
	uint8	m_spare[128*8];
#else
	uint8	m_data[1];
	uint8	m_spare[1];
#endif
} NAND_Page8192_8_S;
typedef struct {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint16	m_data[4096];
	uint16	m_spare[128*4];
#else
	uint16	m_data[1];
	uint16	m_spare[1];
#endif
} NAND_Page8192_16_S;

typedef struct {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint32	m_data[2048];
	uint32	m_spare[128*2];
#else
	uint32	m_data[1];
	uint32	m_spare[1];
#endif
} NAND_Page8192_32_S;
//MT8135 FPGA -- page size = 16384
typedef struct {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint8	m_data[16384];
	uint8	m_spare[128*16];
#else
	uint8	m_data[1];
	uint8	m_spare[1];
#endif
} NAND_Page16384_8_S;
typedef struct {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint16	m_data[8192];
	uint16	m_spare[128*8];
#else
	uint16	m_data[1];
	uint16	m_spare[1];
#endif
} NAND_Page16384_16_S;

typedef struct {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint32	m_data[4096];
	uint32	m_spare[128*4];
#else
	uint32	m_data[1];
	uint32	m_spare[1];
#endif
} NAND_Page16384_32_S;


typedef union {
	uint8				m_raw8[512+16*2];
	uint16				m_raw16[256+8*2];
	uint32				m_raw32[128+4*2];

	// NFIv1, v2, v3, v4
	NAND_Page512_8_S	m_pagespare8;
	NAND_Page512_16_S	m_pagespare16;
	NAND_Page512_32_S	m_pagespare32;
} NAND_Page512_U;

typedef union {
	uint8				m_raw8[2048+64*2];
	uint16				m_raw16[1024+32*2];
	uint32				m_raw32[512+16*2];

	// PAGE: NFIv1, v2
	NAND_Page2048_8_S	m_pagespare8;
	NAND_Page2048_16_S	m_pagespare16;
	NAND_Page2048_32_S	m_pagespare32;

	// SECTOR: NFIv3, v4
	NAND_Page512_8_S	m_sectorspare8[4];
	NAND_Page512_16_S	m_sectorspare16[4];
	NAND_Page512_32_S	m_sectorspare32[4];	
} NAND_Page2048_U;

//MT6516 FPGA
typedef union {
	uint8				m_raw8[4096+128*2];
	uint16				m_raw16[2048+64*2];
	uint32				m_raw32[1024+32*2];

	NAND_Page4096_8_S	m_pagespare8;
	NAND_Page4096_16_S	m_pagespare16;
	NAND_Page4096_32_S	m_pagespare32;

       NAND_Page512_8_S	m_sectorspare8[8];
	NAND_Page512_16_S	m_sectorspare16[8];
	NAND_Page512_32_S	m_sectorspare32[8];
} NAND_Page4096_U;

//MT8135 FPGA -page size 8192
typedef union {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint8				m_raw8[8192+128*8];
	uint16				m_raw16[4096+64*8];
	uint32				m_raw32[2048+32*8];
#else
	uint8				m_raw8[1];
	uint16				m_raw16[1];
	uint32				m_raw32[1];
#endif

	NAND_Page8192_8_S	m_pagespare8;
	NAND_Page8192_16_S	m_pagespare16;
	NAND_Page8192_32_S	m_pagespare32;

       NAND_Page1024_8_S	m_sectorspare8[8];
	NAND_Page1024_16_S	m_sectorspare16[8];
	NAND_Page1024_32_S	m_sectorspare32[8];
} NAND_Page8192_U;

//MT8135 FPGA -page size 16K
typedef union {
#if defined(BB_MT8135)|| defined(BB_MT6580)
	uint8				m_raw8[16384+128*16];
	uint16				m_raw16[8192+64*16];
	uint32				m_raw32[4096+32*16];
#else
	uint8				m_raw8[1];
	uint16				m_raw16[1];
	uint32				m_raw32[1];
#endif

	NAND_Page16384_8_S	m_pagespare8;
	NAND_Page16384_16_S	m_pagespare16;
	NAND_Page16384_32_S	m_pagespare32;

       NAND_Page1024_8_S	m_sectorspare8[16];
	NAND_Page1024_16_S	m_sectorspare16[16];
	NAND_Page1024_32_S	m_sectorspare32[16];
} NAND_Page16384_U;

typedef union {
	NAND_Page512_U		m_512;
	NAND_Page2048_U		m_2048;
	NAND_Page4096_U		m_4096;
	NAND_Page8192_U		m_8192;
	NAND_Page16384_U      m_16384;
} NAND_PageBuffer_U;
#endif
typedef struct {
	NAND_PageBuffer_U	m_page[128];
	bool				m_page_dirty[128];
} NAND_BlockBuffer_S;

typedef union {
	uint8	d8[4];
	uint16	d16[2];
	uint32	d32;
} UnionData_U;

extern		 NAND_Device_S  g_NandFlashDev;
extern const NAND_Device_S 	g_NandFlashDevTbl_Internal[];
extern 		 NAND_Device_S 	*g_NandFlashDevTbl;
extern const NAND_Device_S 	g_SNandFlashDevTbl_Internal[];
extern 		 NAND_Device_S 	*g_SNandFlashDevTbl;
extern NAND_DeviceInfo_S	g_NandFlashInfo;
extern NAND_DeviceInfo_S	*g_pNandInfo;

#ifdef __cplusplus
}
#endif

#endif

