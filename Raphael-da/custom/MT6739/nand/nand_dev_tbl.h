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

#include "sys/types.h"
#include "nand_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __NFI_SUPPORT_TLC__

#define NFI_MEM_OPTIMIZE //reduce memcopy operation

#define NAND_MAX_SPARE_SIZE_BYTE		2048
#define NAND_MAX_OTP_REGION_PER_DIE		4

// DO NOT modify the IO BITS value, because it must be the same as BootROM definition 
#define NAND_IO_8BITS					0
#define NAND_IO_16BITS					1
#define NAND_IO_TOGGLE					2
#define NAND_IO_ONFI					8
#define NAND_IO_BITS_MASK 				0xF

//nand type
#define NAND_FLASH_SLC          		(0x0000)
#define NAND_FLASH_MLC         			(0x0001)
#define NAND_FLASH_TLC          		(0x0002)
#define NAND_FLASH_AMLC          		(0x0003)
#define NAND_FLASH_MASK       			(0x00FF)

typedef struct {
	uint16		m_maker_code;
	uint16		m_device_code;

    uint8		m_identify_code;
    uint8		m_feature_code;
    uint8       m_plane_code;

    uint8       ext_code4;
    uint8       ext_code5;
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
	NAND_ID_S	m_id;						// nand flash H/W manufacture id and device code
	uint64		m_total_size;				// total size in MB 
	uint32		m_pages_per_block;			// pages per block 
	uint32		m_page_size;				// page size in Byte 
	uint32		m_io_interface;				// I/O interface: 8bits or 16bits 
	uint32		m_addr_cycle;				// address cycle 
	uint32		m_spare_size_per_sector;	// spare size per sector
	uint32      m_chip_num;					// chip number
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
	const NAND_PPAGE_TBL				 m_ppage_tbl;
}NAND_MLC_FEATURE;;

typedef struct
{
	bool		slcopmodeEn;			//bool,TRUE: slc mode   FALSE: tlc mode
	bool		normalslcopen;
	bool		pPlaneEn;				//bool, this chip has pseudo plane
	bool		needchangecolumn;		//bool, read page with change column address command
	bool		normaltlc;				//bool,whether need 09/0d 01/02/03 tlc command, whether need wl and page exchange.  micron no need
	uint32		en_slc_mode_cmd;		//enable slc mode cmd
	uint32		dis_slc_mode_cmd;		//disable slc mode cmd: 0xff is invalid
	uint32		normal_slc_mode_cmd;
	uint32		ecc_required;
	bool		ecc_recalculate_en;		//for nfi config;bool
	uint32		block_bit;				//block address start bit;
	uint32		pPlane_bit;				//pesudo plane bit;
	uint8		fdm_size;
	uint32		dqs_mux;
	uint32		dqs_ctrl;
	uint32		set_feature;
	uint32		get_feature;
	uint32		interface_address;
	uint32		interface_async;
	uint32 		interface_sync;
	uint32		acccon;
	uint32		acccon1;
} NFI_TLC_FEATURE;

typedef struct {
	NAND_DeviceID_E				m_id;				// nand flash id 
	NAND_HW_Info_S				m_hw_info;			// NAND flash device H/W info
	const uint16				m_nand_type;		//nand type
	NFI_TLC_FEATURE 			*m_tlc_setting;		//tlc control
	const NAND_CommandSet_S		*m_cmd_set;			// command set 
	const NAND_MLC_FEATURE		*m_mlc_setting;

	//OTP layout
	//const NAND_OTP_Layout_S			*m_otp_layout;
	//OTP callback function set 
	//const NAND_OTP_CMD_Callback_S	*m_otp_cb_func_set;
} NAND_Device_S;

struct _NAND_DeviceInfo_S {
	uint32				m_total_pages;				// total page count 
	uint32				m_total_blocks;				// total block count 
	uint32				m_block_size;				// block size in Byte 
	uint32				m_spare_size;				// spare size in Byte
	uint32				m_page_addr_shift_bits;		// page addr shift bits
	uint32				m_block_addr_shift_bits;	// block addr shift bits 
#if defined(__NFI_SUPPORT_TLC__)
	bool				m_eraselevel_support;		// for TLC erase level purpose
#endif
	const NAND_Device_S		*m_dev;					// NAND flash H/W info 
};

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
	uint8	m_data[1024];
	uint8	m_spare[128];
} NAND_Page1024_8_S;

typedef struct {
	uint16	m_data[512];
	uint16	m_spare[64];
} NAND_Page1024_16_S;

typedef struct {
	uint32	m_data[256];
	uint32	m_spare[32];
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
	uint8	m_data[8192];
	uint8	m_spare[128*8];
} NAND_Page8192_8_S;
typedef struct {
	uint16	m_data[4096];
	uint16	m_spare[128*4];
} NAND_Page8192_16_S;

typedef struct {
	uint32	m_data[2048];
	uint32	m_spare[128*2];
} NAND_Page8192_32_S;
//MT8135 FPGA -- page size = 16384
typedef struct {
	uint8	m_data[16384];
	uint8	m_spare[128*16];
} NAND_Page16384_8_S;
typedef struct {
	uint16	m_data[8192];
	uint16	m_spare[128*8];
} NAND_Page16384_16_S;

typedef struct {
	uint32	m_data[4096];
	uint32	m_spare[128*4];
} NAND_Page16384_32_S;


typedef union {
	uint8					m_raw8[512+16*2];
	uint16					m_raw16[256+8*2];
	uint32					m_raw32[128+4*2];

	// NFIv1, v2, v3, v4
	NAND_Page512_8_S		m_pagespare8;
	NAND_Page512_16_S		m_pagespare16;
	NAND_Page512_32_S		m_pagespare32;
} NAND_Page512_U;

typedef union {
	uint8					m_raw8[2048+64*2];
	uint16					m_raw16[1024+32*2];
	uint32					m_raw32[512+16*2];

	// PAGE: NFIv1, v2
	NAND_Page2048_8_S		m_pagespare8;
	NAND_Page2048_16_S		m_pagespare16;
	NAND_Page2048_32_S		m_pagespare32;

	// SECTOR: NFIv3, v4
	NAND_Page512_8_S		m_sectorspare8[4];
	NAND_Page512_16_S		m_sectorspare16[4];
	NAND_Page512_32_S		m_sectorspare32[4];	
} NAND_Page2048_U;

//MT6516 FPGA
typedef union {
	uint8					m_raw8[4096+128*2];
	uint16					m_raw16[2048+64*2];
	uint32					m_raw32[1024+32*2];

	NAND_Page4096_8_S		m_pagespare8;
	NAND_Page4096_16_S		m_pagespare16;
	NAND_Page4096_32_S		m_pagespare32;

	NAND_Page512_8_S		m_sectorspare8[8];
	NAND_Page512_16_S		m_sectorspare16[8];
	NAND_Page512_32_S		m_sectorspare32[8];
} NAND_Page4096_U;

//MT8135 FPGA -page size 8192
typedef union {
	uint8					m_raw8[8192+128*8];
	uint16					m_raw16[4096+64*8];
	uint32					m_raw32[2048+32*8];

	NAND_Page8192_8_S		m_pagespare8;
	NAND_Page8192_16_S		m_pagespare16;
	NAND_Page8192_32_S		m_pagespare32;

	NAND_Page1024_8_S		m_sectorspare8[8];
	NAND_Page1024_16_S		m_sectorspare16[8];
	NAND_Page1024_32_S		m_sectorspare32[8];
} NAND_Page8192_U;

//MT8135 FPGA -page size 16K
typedef union {
	uint8					m_raw8[16384+128*16*2];
	uint16					m_raw16[8192+64*16*2];
	uint32					m_raw32[4096+32*16*2];

	NAND_Page16384_8_S		m_pagespare8;
	NAND_Page16384_16_S		m_pagespare16;
	NAND_Page16384_32_S		m_pagespare32;

	NAND_Page1024_8_S		m_sectorspare8[16];
	NAND_Page1024_16_S		m_sectorspare16[16];
	NAND_Page1024_32_S		m_sectorspare32[16];
} NAND_Page16384_U;

typedef union {
	NAND_Page512_U			m_512;
	NAND_Page2048_U			m_2048;
	NAND_Page4096_U			m_4096;
	NAND_Page8192_U			m_8192;
	NAND_Page16384_U		m_16384;
} NAND_PageBuffer_U;

typedef struct {
	NAND_PageBuffer_U	m_page[128];
	bool				m_page_dirty[128];
} NAND_BlockBuffer_S;

typedef union {
	uint8		d8[4];
	uint16		d16[2];
	uint32		d32;
} UnionData_U;

//extern		 NAND_Device_S  g_NandFlashDev;
extern const NAND_Device_S 	g_NandFlashDevTbl_Internal[];
//extern 		 NAND_Device_S 	*g_NandFlashDevTbl;
//extern const NAND_Device_S 	g_SNandFlashDevTbl_Internal[];
//extern 		 NAND_Device_S 	*g_SNandFlashDevTbl;
extern NAND_DeviceInfo_S	g_NandFlashInfo;
extern NAND_DeviceInfo_S	*g_pNandInfo;

#ifdef __cplusplus
}
#endif

#endif

