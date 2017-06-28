/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001
*
*****************************************************************************/

/*
 *  Header format
 *  - Block 0
 *  |-- Page 0   => NFB (x2) or (x4)
 *  |-- Page 1   => BR_Layout
 *  |-- Page 2   => ttbl
 *  |-- Page 3-n => 0xFF
 *  - Block 1
 *  |-- Page 0-n => Preloader
 *  - Block 2
 *  |-- Page 0-n => Preloader ext     
 */

#ifndef _NFF_HEADER_H_
#define _NFF_HEADER_H_

#include "sys/types.h"

/*******************************************************************************
 * Macro definition
 *******************************************************************************/

#define ECC_PARITY_BYTES	(144) //80*14/8=140  140+4=144 sync with rom code
#define NFB_LOOP_STOP       (100)

#define	NFB_HEADER_ID1	"BOOTLOADER!"
#define	NFB_HEADER_ID2	"NFIINFO"

#define   SECTOR_1K 0x0100

#define __NFI_SUPPORT_TLC__

/*******************************************************************************
 * Type definition
 *******************************************************************************/
 
#if defined(__NFI_SUPPORT_TLC__)
typedef struct
{
	U8         slcopmodeEn;               //TRUE: slc mode   FALSE: tlc mode
	U8         pPlaneEn;                     //this chip has pseudo plane
	U8         needchangecolumn;    //read page with change column address command
	U8         normaltlc;        //whether need 09/0d 01/02/03 tlc command, whether need wl and page exchange.  micron no need
	U16         en_slc_mode_cmd;     //enable slc mode cmd
	U16         dis_slc_mode_cmd;    //disable slc mode cmd: 0xff is invalid
	U8         ecc_recalculate_en;  //for nfi config
	U8           ecc_required;
	U8           block_bit;                    //block address start bit;
	U8           pPlane_bit;                  //pesudo plane bit;
} NFI_TLC_CTRL;
#endif

typedef struct _NFIType
{
    U16         IOInterface;     /* IO_8BITS or IO_16BITS */
    U16         pageSize;        /* 512, 2048, 4096 bytes */
    U16         addressCycle;
    #if defined(__NFI_SUPPORT_TLC__)
    U16         NAND_FLASH_TYPE;
    NFI_TLC_CTRL tlcControl;
    #endif
} NFI_MENU;

typedef struct _NFIMLC
{
	U16         pPageAddr;
	U16			dqsDelayMux;
	U32         dqsDelayCtrl;
	U32         accconSetting;
	U32         acccon1Setting;
	U32         delayCtrl;
} NFI_MLC;

typedef struct __NFB_HEADER
{
    char        ID1[12];
    char        version[4];
    char        ID2[8];
    NFI_MENU    NFIinfo;
    U16         spareSize;
    U16         pagesPerBlock;
    U16         totalBlocks;
    U16         pageShift;
    U16         blockShift;
	NFI_MLC     mlcSetting;
    U32         padding[1];
} NFB_HEADER;

typedef struct {
    NFB_HEADER  nfb_header_body;
	char        nfb_dummy[256-sizeof(NFB_HEADER)-ECC_PARITY_BYTES];
    U8          nfb_ecc_parities[ECC_PARITY_BYTES];
} NFB_HDR_SECTOR;

#define REPLICATION_NUMBER         (512/sizeof(NFB_HDR_SECTOR))

typedef struct {
    NFB_HDR_SECTOR  nfb_hdr_sec[REPLICATION_NUMBER];
} NFB_HDR_REPLICATION;

//paired page table
#define PP_TBL_ID          "PPTBL"
#define PPTBL_MAX_ENTRY    128
#define ECC_PARITY_CODE    72
typedef struct {
	char            m_identifier[8];        // including '\0'
	U8            en_pp_tbl;
	U16							pagePerBlock;
	U8              padding_u8;
	U16             pp_tbl_entry[PPTBL_MAX_ENTRY];  //bootloader size may not exceed 2K*128=256KB if ppage enable
	U32             padding_u32[(511-12-(PPTBL_MAX_ENTRY*2))>>2];
} P_PAGE_TBL;

#endif /* _NFF_HEADER_H_ */
