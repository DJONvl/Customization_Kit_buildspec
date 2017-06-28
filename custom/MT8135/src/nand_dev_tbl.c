/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 *   nand_dev_tbl.c
 *
 * Project:
 * --------
 *   FlashTool Download Agent
 *
 * Description:
 * ------------
 *   NAND flash device table
 *
 * Author:
 * -------
 *   Amos Hsu
 *
 *==============================================================================
 *              HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:   Jan 03 2006 15:14:40  $
 * $Log:   //mtkvs01/vmdata/new_flash_tool/archives/DA/SRC/nand_dev_tbl.c-arc  $
 *
 * Feb 25 2009 MTK02172
 * [STP100001912] [FlashTool] v3.0908.00 release
 *
 *
 * Dec 1 2008 mtk01413
 * [STP100001857] [FlashTool] v3.08480 release
 *   2. Support New NOR Flash:
 *      [TOSHIBA] TC58FYM8T7D
 *      [TOSHIBA] TC58FYM8B7D
 *      [SAMSUNG] K5L2833ATA
 *      [ST] M58WR064KT
 *      [NUMONYX] M36C0W6050T0
 *   3. Support New NAND Flash:
 *      [MICRON] MT29F1G08ABB
 *      [MICRON] MT29F1G16ABB
 *
 * Nov 5 2008 mtk01413
 * [STP100001847] [FlashTool] v3.08440 release
 *
 *
 * Sep 8 2008 mtk01413
 * [STP100001809] [FlashTool] v3.0836.0 release
 * 1. Add NAND Flash:
 *  [HYNIX] HY27US16561M
 *  [HYNIX] HY27UF162G5A
 *  [HYNIX] HY27UT088G2A
 *  [TOSHIBA] TC58NVG4D1DTG
 *
 *
 * Jul 11 2008 mtk01413
 * [STP100001750] [FlashTool] v3.0828.0 release
 *
 *
 * Jan 31 2008 mtk01413
 * [STP100001630] [FlashTool] v3.0804.0 release
 * New features:
 *  1. [DA] Supports new NOR Flash device
 *      [SAMSUNG] KAL5563CAM
 *      [EON] EON_EN29PL032
 *      [EON] EON_EN29PL064
 *      [SHARP] SHARP_LRS18D5_D7
 *      [SHARP] SHARP_LRS18A7A
 *
 *  2. [DA] Supports new NAND Flash device
 *      [RENESAS] M6MGA157F2LCWG08
 *      [RENESAS] M6MGA157F2LCWG16
 *
 * Dec 3 2007 mtk01413
 * [STP100001568] [FlashTool] v3.0748.0 release
 * New features:
 *  1. [DA] Supports new NAND Flash device
 *      [HYNIX] HY27UF082G2M
 *
 * Nov 2 2006 mtk01413
 * [STP100001195] FlashTool v3.1.02 release
 *
 *
 * Aug 13 2006 mtk00539
 * [STP100001057] FlashTool v3.1.00 release
 *
 *
 * May 24 2006 mtk01413
 * [STP100000879] FlashTool v2.8.1001 release
 *  1. [DA] Supports new NAND flash device.
 *      [TOSHIBA] TC58NWM9S3B
 *
 *
 * Mar 8 2006 mtk00539
 * [STP100000625] FlashTool v2.7.1016
 *  1. [DA] Supports new NOR flash device.
 *      [INTEL] PF48F50xxM0x1xx
 *      [INTEL] PF48F40xxM0x0xx
 *      [INTEL] PF48F40xxM0x1xx
 *  2. [DA] Supports new NAND flash device.
 *      [MICRON] MT29F2G08AAC
 *      [MICRON] MT29F4G08BAB
 *      [RENESAS] superAND 512Mb/8bits
 *      [RENESAS] superAND 512Mb/16bits
 *      [Infineon] HYF33DS51280
 *      [Infineon] HYF33DS1G80
 *      [Infineon] HYF33DS1G16
 *
 *
 * Mar 8 2006 mtk00539
 * [STP100000669] [DA] Support RENESAS superAND flash read back and format operation.
 *
 *
 * Feb 26 2006 mtk00539
 * [STP100000633] [BROM_DLL] Using local DA_REPORT_T variable to avoid multithread unsafe problem.
 *
 *
 * Feb 23 2006 mtk00539
 * [STP100000625] FlashTool v2.7.1016
 *
 *
 *    Rev 1.3   Jan 04 2006 14:40:10   mtk00539
 * Bug fixes:
 *  1. [DA] Fix Spansion S71PLXXXN detection problem by reading CFI info.
 *  2. [DA] Fix TOSHIBA NAND flash callback function set, because TOSHIBA NAND flash doesn't support CopyBack command.
 *
 * New features:
 *  1. [DA] Supports Spansion MirrorBit Buffer-Program method.
 *  2. [DA] Supports new NOR flash device.
 *      [SPANSION] S71PL129N
 *
 * Enhancements:
 *  1. [DA] Halt program when external RAM is less than 128KB.
 * Resolution for 158: [FlashTool v2.7.1014][New] Support Spansion MirrorBit Buffer-Program method.
 *
 *    Rev 1.2   Dec 29 2005 10:51:18   mtk00539
 *  1. [DA] Supports new NOR flash device.
 *      [SPANSION] S71PL127N
 *      [SILICON7] SV6D2832UTA
 *      [SILICON7] SV6D2832UBA
 *      [SILICON7] SV6C2832UTA
 *      [SILICON7] SV6C2832UBA
 *      [SHARP] LH28F16
 *      [TOSHIBA] TV00578002DABD
 *      [TOSHIBA] TV00578003DABD
 *  2. [DA] Supports new NAND flash device.
 *      [TOSHIBA] TH58NVG1S8BFT
 *
 * Resolution for 156: [FlashTool v2.7.1013][BUG FIX] Fix BootROM start command failure while manually selecting NMT6226 or MT6227 baseband chip.
 *
 *    Rev 1.1   Nov 19 2005 00:46:12   mtk00539
 *  1. [BROM_DLL&DA][New] Add UID Secure Booting feature.
 *  2. [BROM_DLL&DA][BUG FIX] Fix RTC date-time value could not be initialized problem.
 *  3. [DA][New] Supports new NOR flash device.
 *              [INTEL] 38F1010C0ZBL0
 *              [INTEL] 28F1602C3BD70
 *              [TOSHIBA] TY0068B012APGG
 *              [TOSHIBA] TY0068B013APGG
 *  4. [DA][New] Supports new NAND flash device.
 *              [HYNIX] HY27XG082G2M
 *  5. [DA][BUF FIX] Refill BootLoader header for BootROM to identify NAND flash organization.
 * Resolution for 149: [BROM_DLL v2.7.1011][New] Add UID Secure Booting feature and Bug Fix.
 *
 *    Rev 1.0   Oct 19 2005 14:41:06   mtk00539
 * Initial revision.
 * Resolution for 140: [BROM_DLL v2.7.1008][New] Support NFB download and many new features.
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *==============================================================================
 *******************************************************************************/
#include "nand_dev_tbl.h"
#include "nand_drv_COMMON_v4.h"
#include "nand_drv_SAMSUNG.h"
#include "nand_drv_TOSHIBA.h"
#include "nand_drv_HYNIX.h"
#include "nand_drv_ST.h"
#include "nand_drv_superAND.h"


#pragma arm section rodata = "NAND_FLASH_TABLE"

const NAND_Device_S g_NandFlashDevTbl_Internal[] = {

    // NAND_FLASH_ID,       {{DEVICE_ID},   Density, Pages/B, Bytes/P, IO, Address Cycle, Spare size/Sector ,   N/A},       Command sets,               Driver sets}

    //------------------------------------------------------------------------------
    // SAMSUNG family flash
    //------------------------------------------------------------------------------

    // SAMSUNG: 512 page
     { NAND_TH58NVG7DDJTA20,	{ { 0x98, 0xDE, 0x94, 0x93, 0xFF, 0xFF},  2048/*MB*/, 256, 16384, NAND_IO_8BITS,  5, 80 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST08GW3C,		{ { 0x20, 0xD3, 0x14, 0xA5, 0x6C },          1024/*MB*/, 128, 2048,  NAND_IO_8BITS,   5, 32},  &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }    
    ,{ NAND_TC58NVG6D2GTA00, { {0x98, 0xDE, 0x94, 0x82, 0x76},	      8192/*MB*/, 256, 8192,  NAND_IO_8BITS,   5, 80},  &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL  }
    ,{ NAND_H27UBG8T2B,           { { 0xAD, 0xD7, 0x94, 0xDA, 0x74},           4096/*MB*/, 256, 8192,  NAND_IO_8BITS,   5, 80},   &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }         
    ,{ NAND_K9GBG08U0A,           { { 0xEC, 0xD7, 0x94, 0x7A, 0x54},           4096/*MB*/, 128, 8192, NAND_IO_8BITS,   5, 80},  &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL  }	
    ,{ NAND_H27UAG8T2A,           { { 0xAD, 0xD5, 0x94, 0x25, 0xFF},           2048/*MB*/, 128, 4096,  NAND_IO_8BITS,   5, 52},   &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }         
    ,{ NAND_MT29F8G08ABA,       { { 0x2C, 0x38, 0x00, 0x26, 0xFF},           1024/*MB*/, 128, 4096,  NAND_IO_8BITS,   5, 52},   &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }         
    ,{ NAND_H27U4G8F2DTR,       { { 0xAD, 0xDC, 0x90, 0x95, 0xFF},            512/*MB*/,     64, 2048,  NAND_IO_8BITS,   5, 32},   &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }         
    ,{ NAND_MT29F1G08ABA,       { { 0x2C, 0xF1, 0x80, 0x95, 0xFF},            128/*MB*/,     64, 2048,  NAND_IO_8BITS,   5, 32},   &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }         
    ,{ NAND_H27US08561A,         { { 0xAD, 0x75, 0xAD, 0x75, 0xFF},            32/*MB*/,     32, 512,  NAND_IO_8BITS,   3, 16},   &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }         

    ,{ NAND_H27UAG8T2BTR, { { 0xAD, 0xD5, 0x94, 0x9A, 0xFF, 0xFF},  2048/*MB*/, 256, 8192, NAND_IO_8BITS,  5, 52 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F2808U0C,     { { 0xEC, 0x73, 0xFF, 0xFF, 0xFF, 0xFF},    16/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F5608Q0C,     { { 0xEC, 0x35, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F5608X0C,     { { 0xEC, 0x75, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F5616Q0C,     { { 0xEC, 0x45, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F5616X0C,     { { 0xEC, 0x55, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9K1208X0C,     { { 0xEC, 0x76, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, &SAMSUNG_OTP, &g_NAND_SAMSUNG_OTP_CB_FUNC_SET }

    ,{ NAND_K5E1257ACM,     { { 0xEC, 0x36, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K5E1G12ACF,     { { 0xEC, 0x78, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }

    ,{ NAND_K9K1G08X0A,     { { 0xEC, 0x79, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    // SAMSUNG: 2048 page
    ,{ NAND_K9F1G08Q0M,     { { 0xEC, 0xA1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F1G08X0M,     { { 0xEC, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &SAMSUNG_OTP, &g_NAND_SAMSUNG_OTP_CB_FUNC_SET } //TEST
    ,{ NAND_K9F1G16Q0M,     { { 0xEC, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F1G16X0M,     { { 0xEC, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F2G08X0M,     { { 0xEC, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F2G16X0M,     { { 0xEC, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9F2G08R0A,     { { 0xEC, 0xAA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9K4G08Q0M,     { { 0xEC, 0xAC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9K4G08U0M,     { { 0xEC, 0xDC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_KA100O015E_BJTT,{ { 0xEC, 0xBC, 0x00, 0x66, 0x56, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL } 
    ,{ NAND_H27U8G8T2B,     { { 0xAD, 0xD3, 0x14, 0xB6, 0xFF, 0xFF},   1024/*MB*/, 128, 4096, NAND_IO_8BITS, 5, 32 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL } 
    ,{ NAND_KF94G16Q4X,     { { 0xEC, 0xBC, 0x00, 0x66, 0x62, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL } 
    ,{ NAND_K9K4G16Q0M,     { { 0xEC, 0xBC, 0x00, 0x55, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_KF98G16Q4X,     { { 0xEC, 0xB3, 0x00, 0x66, 0xFF, 0xFF},  1024/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL } 
    
    ,{ NAND_K9K4G16U0M,     { { 0xEC, 0xCC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9K8G08U0M,     { { 0xEC, 0xD3, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }


    //------------------------------------------------------------------------------
    // PowerFlash family flash
    //------------------------------------------------------------------------------
    ,{ NAND_ASU1GA30GT_G30CA,   { { 0x92, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }

    // end of the table
    ,{ NAND_UNKNOWN, { { 0, 0, 0, 0 }, 0, 0, 0,  0, 0, 0 }, NULL, NULL, NULL, NULL }
};

#pragma arm section rodata
