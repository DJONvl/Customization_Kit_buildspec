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
#include "nand_drv_COMMON_v3_1.h"
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
     { NAND_K9F2808U0C,     { { 0xEC, 0x73, 0xFF, 0xFF, 0xFF, 0xFF},    16/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
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
    ,{ NAND_KF94G16Q4V,     { { 0xEC, 0xBC, 0x00, 0x66, 0x54, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL } 
    ,{ NAND_KF94G16Q4X,     { { 0xEC, 0xBC, 0x00, 0x66, 0x62, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL } 
    ,{ NAND_K9K4G16Q0M,     { { 0xEC, 0xBC, 0x00, 0x55, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_KF98G16Q4X,     { { 0xEC, 0xB3, 0x00, 0x66, 0xFF, 0xFF},  1024/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL } 
    
    ,{ NAND_K9K4G16U0M,     { { 0xEC, 0xCC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_K9K8G08U0M,     { { 0xEC, 0xD3, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    
    // MICRON: 2048 page
    ,{ NAND_MT29F1G08ABB,   { { 0x2C, 0xA1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_MT29F1G16ABB,   { { 0x2C, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_MT29F2G08AAC,   { { 0x2C, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_MT29F2G08ABD,   { { 0x2C, 0xAA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_MT29F2G16ABD,   { { 0x2C, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_MT29F4G08BAB,   { { 0x2C, 0xDC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_MT29C4G96MAZAPCJA_5IT,  { { 0x2C, 0xBC, 0x90, 0x55, 0x56, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_MT29F4G16ABD,   { { 0x2C, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
        
    // Infineon: 512 page
    ,{ NAND_HYF33DS51280,   { { 0xC1, 0x66, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    // Infineon: 2048 page
    ,{ NAND_HYF33DS1G80,    { { 0xC1, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_HYF33DS1G16,    { { 0xC1, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }

    //-----------------------------------------------------------------------------
    // TOSHIBA family flash
    //-----------------------------------------------------------------------------

    // TOSHIBA: 512 page
    ,{ NAND_TC58DVM82A1FT,  { { 0x98, 0x75, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16 }, &g_NAND_TOSHIBA_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_TC58DVM92A1FT,  { { 0x98, 0x76, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_TOSHIBA_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_TC58DVG02A1FT,  { { 0x98, 0x79, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_TOSHIBA_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    // TOSHIBA: 2048 page
    ,{ NAND_TC58NWM9S3B,    { { 0x98, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_TC58NVG0S3AFT,  { { 0x98, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_TC58NYG0S8C,    { { 0x98, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_TC58NYG1S8C,    { { 0x98, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_TH58NVG1S3AFT,  { { 0x98, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_TH58NVG2D4BFT,  { { 0x98, 0xDC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    //- 16bit
    ,{ NAND_TC58NWM9S8C,    { { 0x98, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_TC58NWG0S8C,    { { 0x98, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_TH58NVG1S8BFT,  { { 0x98, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    // TOSHIBA: 4096 page
#if 0
    ,{ NAND_TC58NVG4D1DTG,  { { 0x98, 0xD5, 0xFF, 0xFF },  2048/*MB*/, 128, 4096, NAND_IO_8BITS,  5, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
#else
    ,{ NAND_TC58NVG4D1DTG,  { { 0x98, 0xD5, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS, 5, 27 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
#endif
    ,{ NAND_TY58NYG3S5F,    { { 0x98, 0xB3, 0xFF, 0xFF, 0xFF, 0xFF},  2048/*MB*/, 128, 4096, NAND_IO_16BITS, 5, 16 }, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }    

    //------------------------------------------------------------------------------
    // HYNIX family flash
    //------------------------------------------------------------------------------

    // HYNIX: 512 page
    ,{ NAND_HY27XS08561M,   { { 0xAD, 0x75, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16 }, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_HY27US16561M,   { { 0xAD, 0x55, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16 }, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_HY27XS08121M,   { { 0xAD, 0x76, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_H8BCS0CG0MBR,   { { 0xAD, 0x36, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_HYD0SFG0MF1P,   { { 0xAD, 0x46, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16 }, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }

    ,{ NAND_HY27XA081G1M,   { { 0xAD, 0x79, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_HY27XA161G1M,   { { 0xAD, 0x74, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16 }, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    // HYNIX: 2048 page
    ,{ NAND_HY27UF081G2M,   { { 0xAD, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_H8BCS0PG0MBP,   { { 0xAD, 0xA1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_HYD0SQG0MF1P,   { { 0xAD, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_HY27UF082G2M,   { { 0xAD, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_HY27UF162G5A,   { { 0xAD, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_HY27UT088G2A,   { { 0xAD, 0xD3, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_H9DA4GH4JJAMCR_4EM, { { 0xAD, 0xBC, 0x90, 0x55, 0x54, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS,  5, 16}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &SAMSUNG_OTP, &g_NAND_SAMSUNG_OTP_CB_FUNC_SET }
    ,{ NAND_H8BCS0UN0MCR,   { { 0xAD, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS,  5, 16}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }        
    ,{ NAND_H9DA4GH4JJAMCR, { { 0xAD, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }  

    //------------------------------------------------------------------------------
    // STMicroelectronics family flash
    //------------------------------------------------------------------------------

    // ST: 512 page
    ,{ NAND_ST128W3A,       { { 0x20, 0x73, 0xFF, 0xFF, 0xFF, 0xFF},    16/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST128W4A,       { { 0x20, 0x53, 0xFF, 0xFF, 0xFF, 0xFF},    16/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST256W3A,       { { 0x20, 0x75, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST256W4A,       { { 0x20, 0x55, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST512W3A,       { { 0x20, 0x76, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST512R3A,       { { 0x20, 0x36, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST512W4A,       { { 0x20, 0x56, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST512R4A,       { { 0x20, 0x46, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST01GW3A,       { { 0x20, 0x79, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST01GW4A,       { { 0x20, 0x59, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16 }, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }
    // ST: 2048 page
    ,{ NAND_ST512R3B,       { { 0x20, 0xA2, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST512W3B,       { { 0x20, 0xF2, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST512R4B,       { { 0x20, 0xB2, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST512W4B,       { { 0x20, 0xC2, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST01GR3B,       { { 0x20, 0xA1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST01GW3B,       { { 0x20, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST01GR4B,       { { 0x20, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST01GW4B,       { { 0x20, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST02GR3B,       { { 0x20, 0xAA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST02GW3B,       { { 0x20, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST02GR4B,       { { 0x20, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST02GW4B,       { { 0x20, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST04GR3B,       { { 0x20, 0xAC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST04GW3B,       { { 0x20, 0xDC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    //,{ NAND_ST04GR4B,     { { 0x20, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, &SAMSUNG_OTP, &g_NAND_SAMSUNG_OTP_CB_FUNC_SET }  //TEST
    ,{ NAND_ST04GR4B,       { { 0x20, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST04GW4B,       { { 0x20, 0xCC, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST08GR3B,       { { 0x20, 0xA3, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST08GW3B,       { { 0x20, 0xD3, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST08GR4B,       { { 0x20, 0xB3, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_ST08GW4B,       { { 0x20, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16 }, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL }

    //------------------------------------------------------------------------------
    // RENESAS superAND family flash
    //------------------------------------------------------------------------------

    // RENESAS superAND: 2048 page
    ,{ NAND_SUPERAND51208,      { { 0x07, 0x58, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/,  8, 2048, NAND_IO_8BITS,  4, 16 }, &g_NAND_superAND_2048_CMD_SET, &g_NAND_superAND_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_SUPERAND51216,      { { 0x07, 0x5B, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/,  8, 2048, NAND_IO_16BITS, 4, 16 }, &g_NAND_superAND_2048_CMD_SET, &g_NAND_superAND_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_M6MGA157F2LCWG08,   { { 0x07, 0x49, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/,  16, 512, NAND_IO_8BITS,  4, 16 }, &g_NAND_superAND_512_CMD_SET, &g_NAND_superAND_CB_FUNC_SET, NULL, NULL }
    ,{ NAND_M6MGA157F2LCWG16,   { { 0x07, 0x4A, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/,  16, 512, NAND_IO_16BITS, 4, 16 }, &g_NAND_superAND_512_CMD_SET, &g_NAND_superAND_CB_FUNC_SET, NULL, NULL }

    //------------------------------------------------------------------------------
    // PowerFlash family flash
    //------------------------------------------------------------------------------
    ,{ NAND_ASU1GA30GT_G30CA,   { { 0x92, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL }

    // end of the table
    ,{ NAND_UNKNOWN, { { 0, 0, 0, 0 }, 0, 0, 0,  0, 0, 0 }, NULL, NULL, NULL, NULL }
};

#pragma arm section rodata
