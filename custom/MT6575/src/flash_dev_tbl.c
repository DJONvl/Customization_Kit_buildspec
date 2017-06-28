/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 *	 flash_dev_tbl.c 
 *
 * Project:
 * --------
 *   FlashTool Download Agent 
 *
 * Description:
 * ------------
 *   Flash device table 
 *
 * Author:
 * -------
 *	 Amos Hsu 
 *
 *==============================================================================
 * 				HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: #3 $
 * $Modtime:   Jan 12 2006 17:27:58  $
 * $Log:   //mtkvs01/vmdata/flash_tool/archives/DA/SRC/flash_dev_tbl.c-arc  $
 *
 * Feb 25 2009 MTK02172
 * [STP100001912] [FlashTool] v3.0908.00 release
 * 
 *
 * Dec 1 2008 mtk01413
 * [STP100001857] [FlashTool] v3.08480 release
 *   2. Support New NOR Flash:
 * 		[TOSHIBA] TC58FYM8T7D
 * 		[TOSHIBA] TC58FYM8B7D
 * 		[SAMSUNG] K5L2833ATA
 * 		[ST] M58WR064KT
 * 		[NUMONYX] M36C0W6050T0  
 *   3. Support New NAND Flash:
 * 		[MICRON] MT29F1G08ABB
 * 		[MICRON] MT29F1G16ABB
 *
 * Nov 5 2008 mtk01413
 * [STP100001847] [FlashTool] v3.08440 release
 * 
 *
 * Feb 22 2008 mtk01413
 * [STP100001644] [FlashTool] v3.0808.0 release
 * 
 *
 * Feb 1 2008 mtk01413
 * [STP100001630] [FlashTool] v3.0804.0 release
 * 
 *
 * Jan 31 2008 mtk01413
 * [STP100001630] [FlashTool] v3.0804.0 release
 * New features:
 *  1. [DA] Supports new NOR Flash device
 * 		[SAMSUNG] KAL5563CAM
 * 		[EON] EON_EN29PL032
 * 		[EON] EON_EN29PL064
 * 		[SHARP] SHARP_LRS18D5_D7
 * 		[SHARP] SHARP_LRS18A7A
 * 
 *  2. [DA] Supports new NAND Flash device
 * 		[RENESAS] M6MGA157F2LCWG08
 * 		[RENESAS] M6MGA157F2LCWG16
 *
 * Dec 3 2007 mtk01413
 * [STP100001568] [FlashTool] v3.0748.0 release
 * New features:
 *  1. [DA] Supports new NOR Flash device
 * 		[ST] M58WR064HU
 * 		[ST] M58WR064HL
 * 		[TOSHIBA] TV00560002EDGB
 * 		[TOSHIBA] TV00560003EDGB
 * 		[TOSHIBA] TY00670002APGN
 * 		[INTEL] PF38F30xxM0x0xx
 * 		[INTEL] PF38F30xxM0x1xx
 * 		[SHARP]	LRS18CK
 * 		[AMIC] A82DL3228T
 * 		[AMIC] A82DL3228U
 * 		[AMIC] A82DL3238T
 * 		[AMIC] A82DL3238U
 * 		[AMIC] A82DL3248T
 * 		[AMIC] A82DL3248U
 * 		[SAMSUNG] K5L6331CAA
 *
 * Aug 30 2007 mtk01413
 * [STP100001497] [FlashTool] v3.2.0 release
 * New features:
 *  1. [DA] Supports new NOR Flash device
 * 		[ST] M58WR016QT
 * 		[ST] M58WR016QB
 * 		[ST] M58WR032QT
 * 		[ST] M58WR032QB
 * 		[INTEL] PF38F60xxM0x0xx
 * 		[INTEL] PF38F60xxM0x1xx
 * 		[SPANSION] S29NS016J
 * 		[SPANSION] S29NS032J
 * 		[SPANSION] S29NS064J
 * 		[SPANSION] S29NS128J
 *
 * May 18 2007 mtk01413
 * [STP100001402] FlashTool v3.1.05 release
 * New features:
 *  1. [DA] Supports new NOR Flash device
 * 		[SPANSION] S29WS128P
 * 		[SPANSION] S29WS256P
 * 		[TOSHIBA] TV00560002DDGB
 *
 * Mar 12 2007 mtk01413
 * [STP100001329] FlashTool v3.1.04 release
 * New features:
 *  1. [DA] Supports new NOR Flash device
 * 		[SAMSUNG] K5L2731CAM
 * 		[SAMSUNG] K5L2931CAM
 * 		[SPANSION] S71PL127J
 * 		[TOSHIBA] TY00689002APGN
 * 		[SPANSION] S29GL064A_T
 * 		[SPANSION] S29GL064A_B
 * 
 *  2. [DA] Supports OTP driver for below NOR Flash device
 * 		[SPANSION] NOR_S71WS256N
 * 		[SPANSION] S71WS128NB0BFWAN3
 * Enhancements:
 *  1. [DA] Rename old NOR Flash device
 * 		[SPANSION] S71WS128NB0BFWAN3	-->	S29WS128N
 * 		[SPANSION] S71WS256N		-->	S29WS256N
 * 		[SPANSION] S71WS512PD0HF3HR	-->	S29WS512P
 * 		[SPANSION] S29GL128NH		-->	S29GL128N_T
 * 		[SPANSION] S29GL128NL		-->	S29GL128N_B
 * 		[SPANSION] S29GL256NH		-->	S29GL256N_T
 * 		[SPANSION] S29GL256NL		-->	S29GL256N_B
 * 		[SPANSION] S29GL512NH		-->	S29GL512N_T
 * 		[SPANSION] S29GL512NL		-->	S29GL512N_B
 * 	
 *
 * Jan 2 2007 mtk01413
 * [STP100001195] FlashTool v3.1.02 release
 * New features:
 *  1. [BROM_DLL/DA] Support FOTA(Firmware update Over The Air) download.
 *  2. [BROM_DLL/DA] Support MT6223  download.
 *  3. [BROM_DLL/DA] Support MT6268T download.
 *  4. [DA] Supports new NOR Flash device
 * 		[SPANSION] S29GL128NH
 * 		[SPANSION] S29GL128NL
 * 		[SPANSION] S29GL256NH
 * 		[SPANSION] S29GL256NL
 * 		[SPANSION] S29GL512NH
 * 		[SPANSION] S29GL512NL
 * 
 *
 * Nov 2 2006 mtk01413
 * [STP100001195] FlashTool v3.1.02 release
 * New features:
 *  1. [DA] Supports new NOR Flash device (AD-Mux)
 * 		[SPANSION] S29NS064N
 * 		[SPANSION] S29NS128N
 * 		[SPANSION] S29NS256N
 * 
 *  2. [DA] Supports new NOR flash device.
 * 		[ST] M58PPR256J
 * 		[ST] M58PPR512J	
 * 		[SST] SST32HF3241C
 * 		[SPANSION] S71WS128NB0BFWAN3
 * 		[SPANSION] S71WS256N
 * 		[SPANSION] S71WS512PD0HF3HR
 *
 * Aug 13 2006 mtk01413
 * [STP100001057] FlashTool v3.1.00 release
 * New features:
 *  1. [FlashTool/BROM_DLL/DA] Provide OTP Read/Write/Lock Operations
 * 
 *
 * May 24 2006 mtk01413
 * [STP100000879] FlashTool v2.8.1001 release
 *  1. [DA][BUG_FIX] Fix a bug for wrong information about S71PL129N in Flash Device Table.
 *  2. [DA][BUG_FIX] Fix a bug for Software Unlock Command Sequence wrong block address in Renesas driver. Within 64Mb, DQ7 in block address is fixed 0.
 *  3. [DA] Supports new NOR flash device.
 * 	[SHARP] LRS18BK
 * 	[INTEL] 28F3204C3TD70, [MemoCom] KS03204
 *
 * Mar 18 2006 mtk01413
 * [STP100000694] [DA] 1. Support SST driver. 2. Bug Fix: WriteCmd  3. Bug Fix: DetectExtRAM
 * [SST]  SST34HF16x1
 * [SST]  SST34HF32x4
 *
 * Mar 8 2006 mtk00539
 * [STP100000625] FlashTool v2.7.1016
 *  1. [DA] Supports new NOR flash device.
 * 		[INTEL] PF48F50xxM0x1xx
 * 		[INTEL] PF48F40xxM0x0xx
 * 		[INTEL] PF48F40xxM0x1xx
 *  2. [DA] Supports new NAND flash device.
 * 		[MICRON] MT29F2G08AAC
 * 		[MICRON] MT29F4G08BAB
 * 		[RENESAS] superAND 512Mb/8bits
 * 		[RENESAS] superAND 512Mb/16bits
 * 		[Infineon] HYF33DS51280
 * 		[Infineon] HYF33DS1G80
 * 		[Infineon] HYF33DS1G16
 * 
 * 
 *    Rev 1.20   Jan 14 2006 00:26:26   mtk00539
 * Bug fixes:
 *  1. [DA] Fix SHARP LH28F16 flash download fail problem. 
 * 
 * New features:
 *  1. [DA] Supports new NOR flash device.
 * 		[INTEL] PF38F5050M0XXXX
 * Resolution for 159: [FlashTool v2.7.1015]
 * 
 *    Rev 1.19   Jan 04 2006 14:40:10   mtk00539
 * Bug fixes:
 *  1. [DA] Fix Spansion S71PLXXXN detection problem by reading CFI info.
 *  2. [DA] Fix TOSHIBA NAND flash callback function set, because TOSHIBA NAND flash doesn't support CopyBack command.
 *  
 * New features:
 *  1. [DA] Supports Spansion MirrorBit Buffer-Program method.
 *  2. [DA] Supports new NOR flash device.
 * 		[SPANSION] S71PL129N
 * 
 * Enhancements:
 *  1. [DA] Halt program when external RAM is less than 128KB.
 * Resolution for 158: [FlashTool v2.7.1014][New] Support Spansion MirrorBit Buffer-Program method.
 * 
 *    Rev 1.18   Jan 02 2006 13:16:32   mtk00539
 * Add new flash [SPANSION] S71PL256NC0HAW5B
 * Resolution for 156: [FlashTool v2.7.1013][BUG FIX] Fix BootROM start command failure while manually selecting NMT6226 or MT6227 baseband chip.
 * 
 *    Rev 1.17   Dec 29 2005 15:59:16   mtk00539
 * Add Spansion MirrorBit comment
 * Resolution for 156: [FlashTool v2.7.1013][BUG FIX] Fix BootROM start command failure while manually selecting NMT6226 or MT6227 baseband chip.
 * 
 *    Rev 1.16   Dec 29 2005 10:51:18   mtk00539
 *  1. [DA] Supports new NOR flash device.
 * 		[SPANSION] S71PL127N
 * 		[SILICON7] SV6D2832UTA
 * 		[SILICON7] SV6D2832UBA
 * 		[SILICON7] SV6C2832UTA
 * 		[SILICON7] SV6C2832UBA
 * 		[SHARP] LH28F16
 * 		[TOSHIBA] TV00578002DABD
 * 		[TOSHIBA] TV00578003DABD
 *  2. [DA] Supports new NAND flash device.
 * 		[TOSHIBA] TH58NVG1S8BFT
 * 
 * Resolution for 156: [FlashTool v2.7.1013][BUG FIX] Fix BootROM start command failure while manually selecting NMT6226 or MT6227 baseband chip.
 * 
 *    Rev 1.15   Nov 19 2005 00:46:10   mtk00539
 *  1. [BROM_DLL&DA][New] Add UID Secure Booting feature.
 *  2. [BROM_DLL&DA][BUG FIX] Fix RTC date-time value could not be initialized problem. 
 *  3. [DA][New] Supports new NOR flash device.
 * 				[INTEL] 38F1010C0ZBL0
 * 				[INTEL] 28F1602C3BD70
 * 				[TOSHIBA] TY0068B012APGG
 * 				[TOSHIBA] TY0068B013APGG
 *  4. [DA][New] Supports new NAND flash device.
 * 				[HYNIX] HY27XG082G2M
 *  5. [DA][BUF FIX] Refill BootLoader header for BootROM to identify NAND flash organization.
 * Resolution for 149: [BROM_DLL v2.7.1011][New] Add UID Secure Booting feature and Bug Fix.
 * 
 *    Rev 1.14   Oct 19 2005 14:45:16   mtk00539
 *  1. [BROM_DLL&DA][New] New H/W devices detection architecture including NOR, NAND, SRAM and DRAM detection. 
 *  2. [BROM_DLL&DA][New] DA partial download technic, that overcomes DA size exceeds MT6205B internal SRAM (32KB).
 *  3. [BROM_DLL&DA][New] Format verification option, each byte should be 0xFF after erasure. 
 *  4. [BROM_DLL&DA][New] DA validation to ensure invalid binary could not be loaded.
 *  5. [BROM_DLL&DA][New] Support NFB download. 
 *  6. [BROM_DLL&DA][New] Support NAND flash format with 3 method, NORMAL, FORCEDLY ERASE and MARK AS BAD BLOCK. 
 *  7. [BROM_DLL&DA][New] Support NAND flash read back with 4 methods, PAGE+ECC, PAGE ONLY, SPARE ONLY and PAGE+SPARE. 
 * Resolution for 140: [BROM_DLL v2.7.1008][New] Support NFB download and many new features.
 * 
 *    Rev 1.13   Oct 18 2005 13:37:10   mtk00539
 *  1. [DA][New] New NOR flash device support
 * 		[SILICON7] S7_SV7E160XT,
 * 		[SILICON7] S7_SV7E160XB,
 * 		[SILICON7] S7_SV7E320XT,
 * 		[SILICON7] S7_SV7E320XB,
 * 		[SPANSION] S71GL032R3_T,
 * 		[SPANSION] S71GL032R4_B,
 * 		[SPANSION] S71GL032R1R2,
 * 		[TOSHIBA] TV00569002BABD,
 * 		[TOSHIBA] TV00569003BABD,
 * 		[TOSHIBA] TV00569002AABD,
 * 		[TOSHIBA] TV00569003AABD,
 * Resolution for 137: [BROM_DLL v2.4.1017][New] Enable SID feature and add new flash device supoprt.
 * 
 *    Rev 1.12   Sep 13 2005 13:24:18   mtk00539
 *  1. [DA][New] New NOR flash device support [Winbond] W19B320ATB, W19B320ATT.
 *  2. [DA][BUG FIX] Fix MT6229 read DSP version failure problem.
 * Resolution for 130: [BROM_DLL v2.4.1016][Enhance] Workaround the 1st time power up failure issue when RTC is empty.
 * 
 *    Rev 1.11   Jun 15 2005 12:23:06   mtk00539
 *  1. [DA][New] Support new flash devices TV0057A002AABD and TV0057A002AABD.
 *  2. [DA][New] Support RENESAS flash with F-WP#(LOW).
 * Resolution for 118: [BROM_DLL v2.4.1013][New] Support RENESAS flash with F-WP#(LOW).
 * 
 *    Rev 1.10   May 24 2005 19:58:44   mtk00539
 *  1. [DA][New] Support new flash devices S71PL254JXX, S71WS256NXX.
 *  2. [DA][New] Add DA_ENABLE_WATCHDOG_CMD command to enable watchdog. 
 * Resolution for 115: [BROM_DLL v2.4.1012][New] Support new flash devices and bug fix.
 * 
 *    Rev 1.9   Mar 29 2005 11:14:12   mtk00539
 *  1. [BROM_DLL][BUG FIX] ROM_ID_Class::LoadID() should dump only the last 256 bytes data. 
 *  2. [BROM_DLL][New] Add detail H/W error debug log. 
 *  3. [DA][Enhance] Enhance bus test in FlashTest_DA.
 *  4. [DA][New] Add H/W error detail detection, thus we can distinguish INT_SRAM or EXT_SRAM error.
 *  5. [DA][New] Add all INTEL W18/W30, L18/L30 non-SCSP flashes support. 
 * Resolution for 107: [BROM_DLL v2.4.1010][New] Enhance bus test in FlashTest_DA, new INTEL flash support and bug fix.
 * 
 *    Rev 1.8   Feb 16 2005 17:24:26   mtk00539
 *  1. [DA][BUG FIX] Fix INTEL W18/W30 series flash program fail. this series don't support Buffered-Program method, we can only use WORD program.
 *  2. [DA][BUG FIX] Fix format %x bug in UART_Printf().
 *  3. [DA][BUG FIX] Fix small size EXT_SRAM detection error. Shrink the EXT_SRAM detection unit from 512KB to 128KB in Board_Schematic().
 *  4. [DA][New] Extend max sector region to 4 for SPANSION S71AL016D flash.
 *  5. [DA][New] Add SPANSION S71AL016D T/B and SHARP LRS18C8A flash support. 
 * Resolution for 105: [BROM_DLL v2.4.1009][New] Fix INTEL W18/W30 series bug and add SPANSION S71AL016D and SHARP LRS18C8A flash support.
 * 
 *    Rev 1.7   Nov 29 2004 15:50:14   mtk00539
 * [DA][New] Support new flashes [SHARP]LRS18B0 and LRS1863.
 * Resolution for 99: [BROM_DLL v2.4.1008][New] Support INTEL family flash Buffered-Program method.
 * 
 *    Rev 1.6   Nov 22 2004 12:25:36   mtk00539
 *  1. [DA][BUG FIX] Add data verification in status polling during erase or program operation.
 *  2. [DA][BUG FIX] Fix two dies flash detection fail in INTEL_CheckDevID(). 
 *  3. [DA][Change Behavior] When RX_BUFFER_FULL occurs, flush data queued in UART ring buffer til data is less than 512KB.
 *  4. [DA][New] Support Buffered-Program method for INTEL family flashes.
 *  5. [DA][New] Support new flashes [SHARP]LRS1828C and [RENESAS]M6MGB64BM34CDG.
 * Resolution for 99: [BROM_DLL v2.4.1008][New] Support INTEL family flash Buffered-Program method.
 * 
 *    Rev 1.5   Oct 29 2004 11:46:48   mtk00539
 *  1. [DA][BUG FIX] Read CFI boot sector flag to determine AM49DL3208GT and S71PL032J, since both flashes have the same id, but different layout.
 *  2. [DA][BUG FIX] Fix INTEL and RENESAS status checking flow.
 *  3. [DA][Enhance] Dump more logs in TEST_ExtSRAM().
 *  4. [DA][New] Support new flashes
 * 				[SAMSUNG] K5J6316CTM
 * 				[TOSHIBA] TH50VPF5682CDSB
 * 				[TOSHIBA] TH50VPF5683CDSB
 * 				[ISSI] IS75V16F128GS32
 * 				[RENESAS] M6MGT64BM34CDG
 * 				[ST] M30L0T7000T0, M36L0T7050T0
 * 				[ST] M30L0T7000B0, M36L0T7050B0
 * 				[SHARP] LRS1862
 * 				[SHARP] LRS1806A
 * 				[AMD] AM49DL3208GT
 * 				[SPANSION] S29PL032J, S71PL032J
 * Resolution for 92: [BROM_DLL v2.4.1006][BUG FIX] Fix AM49DL3208GT & S71PL032J download fail problem and some enhancement.
 * 
 *    Rev 1.4   Sep 14 2004 20:47:14   mtk00539
 * Add new flash [SAMSUNG] KAD060300B
 * Resolution for 90: [BROM_DLL v2.4.1005][BUG FIX] Fix MT6217 download fail problem and some enhancement.
 * 
 *    Rev 1.3   Sep 03 2004 19:48:46   mtk00539
 * [DA][Enhance] Modify ST flash to use unlock bypass program callback function set.
 * Resolution for 88: [BROM_DLL v2.4.1004][New] Add MT6217_BN, MT6219_CV and MT6219_EV support.
 * 
 *    Rev 1.2   Aug 23 2004 19:57:48   mtk00539
 * [DA][BUG FIX] Fix AM49DL3208G sector layout error.
 * Resolution for 86: [BROM_DLL v2.4.1003] Fix AM49DL3208G sector layout error.
 * 
 *    Rev 1.1   Aug 03 2004 10:33:06   mtk00539
 *  1. [DA][BUG FIX] Invoke Board_Schematic()(old name is HW_Init()) before FUTL_CheckDevice(). Because FUTL_CheckDevice() must know the absolute address for each bank.
 *  2. [DA][New] Add 12 new flash devices support 
 * 				[SAMSUNG]K5A3280YT,
 * 				[TOSHIBA]TH50VPF6782AASB,
 * 				[TOSHIBA]TH50VPF6783AASB,
 * 				[TOSHIBA]TV00578002AABD,
 * 				[TOSHIBA]TV00578003AABD,
 * 				[Fujitsu]MB84VP24581HK,
 * 				[INTEL]INTEL_28F640W30_B,
 * 				[SPANSION]AM49PDL127BH,
 * 				[SPANSION]AM49PDL129BH,
 * 				[RENESAS]M6MGD13BW66CDG,
 * 				[Winbond]W19B322TM,
 * 				[Winbond]W19B323TM,
 *  3. [DA][New] Readback check after WORD program is done, so that DA won't have to keep the chksum.
 *  4. [DA][New] Auto-detect baseband chip type.
 *  5. [DA][New] Auto-detect external SRAM size.
 *  6. [DA][New] Merge MT6205B, MT6218B and MT6219 to an all-in-one DA.
 *  7. [DA][New] Merge flash stress test module.
 *  8. [DA][New] Modify makefile to build ARM or THUMB code.
 *  9. [DA][New] Construct a customization kit for customers to add new flash or SOC checking algorithm by themselves.
 * 10. [DA][Enhance] Split out AM29PDL128G function to improve the performance of general AMD callback functions.
 * 11. [DA][Enhance] Enhance RX_BUFF_FULL behavior to prevent always retry two times.
 * Resolution for 83: [BROM_DLL v2.4.1002] Merge all the DAs into all-in-one DA and fix many bugs and enhancement.
 * 
 *    Rev 1.0   Jul 19 2004 01:43:50   mtk00539
 * Initial revision.
 * 
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *==============================================================================
 *******************************************************************************/
#include "flash_dev_tbl.h"
#include "flash_drv_DUMMY.h"
#include "flash_drv_AMD.h"
#include "flash_drv_AM29PDL128G.h"
#include "flash_drv_INTEL.h"
#include "flash_drv_RENESAS.h"
#include "flash_drv_SHARP.h"
#include "flash_drv_SST.h"
#include "flash_drv_TOSHIBA.h"
#include "flash_drv_SAMSUNG.h"

//#include "flash_drv_SF_SST.h"
//#include "flash_drv_SF_MXIC.h"

#pragma arm section rodata = "NOR_FLASH_TABLE"

#if defined(FEATURE_NOR_SUPPORT)	
// flash table 
const NOR_Device_S g_FlashDevTbl_Internal[] = {

	//------------------------------------------------------------------------------
	// Serial flash                                                             
	//------------------------------------------------------------------------------
#if defined(BB_MT6251)

	// SST
	{ SF_SST26VF016,			{ { { &SST_TB_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0001,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26VF032,			{ { { &SST_TB_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0002,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26VF064,			{ { { &SST_TB_64     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0003,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26WF080,			{ { { &SST_TB_8      }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0028,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26WF016,			{ { { &SST_TB_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0021,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26WF032,			{ { { &SST_TB_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0022,      0 }, &SST_CMD_CB_BUF_PGM, NULL },
	{ SF_SST26WF064,			{ { { &SST_TB_64     }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x0026, 0x0023,      0 }, &SST_CMD_CB_BUF_PGM, NULL },

	// MXIC
	{ SF_MX25U8035E,			{ { { &MXIC_8        }, { &DUMMY_MAP  } }, NULL, 1, 0x00C2, 0x0025, 0x0034,      0 }, &MXIC_CMD_CB_BUF_PGM, NULL },
	{ SF_MX25U1635E,			{ { { &MXIC_16       }, { &DUMMY_MAP  } }, NULL, 1, 0x00C2, 0x0025, 0x0035,      0 }, &MXIC_CMD_CB_BUF_PGM, NULL },
	{ SF_MX25U3235E,			{ { { &MXIC_32       }, { &DUMMY_MAP  } }, NULL, 1, 0x00C2, 0x0025, 0x0036,      0 }, &MXIC_CMD_CB_BUF_PGM, NULL },

#elif defined(FEATURE_NOR_SUPPORT)

	//------------------------------------------------------------------------------
	// AMD family flash                                                             
	//------------------------------------------------------------------------------
	
	// AMD & SPANSION 
	{ NOR_AM29DL323D,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x2250,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM29DL322GT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x2255,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM49DL3208GB,			{ { { &AMD_B_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x220A, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM49DL3208GT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29PL032J,			{ { { &AMD_TB_32    }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM29DL640D,			{ { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM50DL128CG,			{ { { &AMD_TB_64    }, { &AMD_TB_64  } }, NULL, 2, 0x0001, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM29PDL128G,			{ { { &AMD_TB_128   }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x220D, 0x2200 }, &AMD_AM29PDL128G_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
//	{ NOR_AM49PDL127BH,			{ { { &AMD_TB_128   }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },	--> expired to NOR_S71PL127J!!
	{ NOR_S71PL254J,			{ { { &AMD_TB_128   }, { &AMD_TB_128 } }, NULL, 2, 0x0001, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_AM49PDL129BH,			{ { { &AMD_B_64     }, { &AMD_T_64   } }, NULL, 2, 0x0001, 0x227E, 0x2221, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71AL016D_T,			{ { { &AMD_T_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x22C4,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71AL016D_B,			{ { { &AMD_B_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x2249,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71GL032R3_T,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x221A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71GL032R4_B,			{ { { &AMD_B_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x221A, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S71GL032R1R2,			{ { { &AMD_32       }, { &DUMMY_MAP  } }, NULL, 1, 0x0001, 0x227E, 0x221D, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// SPANSION (MirrorBit Family) 
	//- PL-N
	{ NOR_S71PL127N,			{ { { &AMD_MB_TB_128 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S71PL127J,			{ { { &AMD_TB_128    }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S71PL129N,			{ { { &AMD_MB_B_64   }, { &AMD_MB_T2_64  } }, &SPANSION_OTP_128W_SECSI, 2, 0x0001, 0x227E, 0x2221, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S71PL256N,			{ { { &AMD_MB_TB_256 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x223C, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	//- NS-N
	{ NOR_S29NS064N,			{ { { &AMD_MB_T_64	 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x2B7E, 0x2B33, 0x2B00 }, &AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29NS128N,			{ { { &AMD_MB_T2_128 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x2C7E, 0x2C35, 0x2C00 }, &AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29NS256N,			{ { { &AMD_MB_T2_256 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x2D7E, 0x2D2F, 0x2D00 }, &AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	//- NS-J
	{ NOR_S29NS016J,			{ { { &AMD_MB_T_16	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x297E, 0x2915, 0x2900 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29NS032J,			{ { { &AMD_MB_T_32	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x2A7E, 0x2A24, 0x2A00 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29NS064J,			{ { { &AMD_MB_T_64	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x277E, 0x2702, 0x2700 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29NS128J,			{ { { &AMD_MB_T_128	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x007E, 0x0016, 0x0000 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	//- GL-N
	{ NOR_S29GL128N_T,			{ { { &AMD_128 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2221, 0x2201 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL256N_T,			{ { { &AMD_256 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2222, 0x2201 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL512N_T,			{ { { &AMD_512 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2223, 0x2201 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL128N_B,			{ { { &AMD_128 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x0021, 0x0001 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL256N_B,			{ { { &AMD_256 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x0022, 0x0001 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	{ NOR_S29GL512N_B,			{ { { &AMD_512 	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x0023, 0x0001 }, &AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM, NULL },
	//- GL-A
	{ NOR_S29GL064A_T,			{ { { &AMD_T_64	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2210, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S29GL064A_B,			{ { { &AMD_B_64	 	 }, { &DUMMY_MAP     } }, 					  NULL, 1, 0x0001, 0x227E, 0x2210, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	//- WS-N
	{ NOR_S29WS128N,			{ { { &AMD_MB_TB2_128}, { &DUMMY_MAP  	 } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2231, 0x2200 }, &AMD_CMD_CB_S29WS_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29WS256N,			{ { { &AMD_MB_TB2_256}, { &DUMMY_MAP  	 } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2230, 0x2200 }, &AMD_CMD_CB_S29WS_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	//- WS-P
	{ NOR_S29WS128P,			{ { { &AMD_MB_TB2_128}, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2244, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29WS256P,			{ { { &AMD_MB_TB2_256}, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x2242, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },
	{ NOR_S29WS512P,			{ { { &AMD_MB_TB_512 }, { &DUMMY_MAP     } }, &SPANSION_OTP_128W_SECSI, 1, 0x0001, 0x227E, 0x223D, 0x2200 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, &AMD_OTP_CMD_CB },

	//- WS-R
	{ NOR_S29WS128R_T,			{ { { &AMD_MB_T2_128 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0027, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS128R_B,			{ { { &AMD_MB_B_128  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0027, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS128R,			{ { { &AMD_128       }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0027, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	{ NOR_S29WS256R_T,			{ { { &AMD_MB_T2_256 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0026, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS256R_B,			{ { { &AMD_MB_B_256  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0026, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS256R,			{ { { &AMD_256       }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0026, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	{ NOR_S29WS512R_T,			{ { { &AMD_MB_T_512  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0025, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS512R_B,			{ { { &AMD_MB_B_512  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0025, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS512R,			{ { { &AMD_512       }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0025, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	{ NOR_S29WS01GR_T,			{ { { &AMD_MB_T_1024 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0024, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS01GR_B,			{ { { &AMD_MB_B_1024 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0024, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29WS01GR,			{ { { &AMD_1024      }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0024, 0x0003 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	//- VS-R
	{ NOR_S29VS128R_T,			{ { { &AMD_MB_T2_128 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0063, 0x0001 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29VS128R_B,			{ { { &AMD_MB_B_128  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0065, 0x0001 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	{ NOR_S29VS256R_T,			{ { { &AMD_MB_T2_256 }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0064, 0x0001 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},
	{ NOR_S29VS256R_B,			{ { { &AMD_MB_B_256  }, { &DUMMY_MAP     } }, NULL, 1, 0x0001, 0x007E, 0x0066, 0x0001 }, &AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM, NULL},

	// Fujitsu 
	{ NOR_MB84VD23280FA,		{ { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x0004, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_MB84VD22280FE,		{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x0004, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_MB84VP24491HK,		{ { { &AMD_B_64     }, { &AMD_T_64   } }, NULL, 2, 0x0004, 0x227E, 0x2221, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_MB84VP24581HK,		{ { { &AMD_TB_128   }, { &DUMMY_MAP  } }, NULL, 1, 0x0004, 0x227E, 0x2220, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// ISSI 
	{ NOR_IS75V16F128GS32,		{ { { &AMD_TB_64    }, { &AMD_TB_64  } }, NULL, 2, 0x0004, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// ST 
	{ NOR_ST_M29DW640D,			{ { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x0020, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_ST_M74DW66500B,		{ { { &AMD_TB_64    }, { &AMD_TB_64  } }, NULL, 2, 0x0020, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// SAMSUNG 
	{ NOR_K5A3280YT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x22A0,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5J6316CTM,			{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x22E0,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_KAD060300B,			{ { { &AMD_T_64     }, { &AMD_T_64   } }, NULL, 2, 0x00EC, 0x22E0,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L3316CAM,           { { { &AMD_TB_32    }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x257E, 0x2503, 0x2501 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L6331CAA,           { { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x257E, 0x2506, 0x2501 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L2931CAM,           { { { &AMD_B_64     }, { &AMD_T_64   } }, NULL, 2, 0x00EC, 0x257E, 0x2508, 0x2501 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L2833ATA,           { { { &AMD_T_128    }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x2402,      0,      0 }, &AMD_SAMSUNG_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_K5L2731CAM,           { { { &AMD_TB_128   }, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x257E, 0x2508, 0x2501 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_KAL5563CAM,  			{ { { &AMD_MB_TB_256}, { &DUMMY_MAP  } }, NULL, 1, 0x00EC, 0x227E, 0x2263, 0x2260 }, &AMD_CMD_CB_MIRRORBIT_BUF_PGM, NULL },
	// Winbond 
	{ NOR_W19B322TM,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00DA, 0x2210,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_W19B323TM,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00DA, 0x2213,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_W19B320ATB,			{ { { &AMD_B_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00DA, 0x227E, 0x220A, 0x2200 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_W19B320ATT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x00DA, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// SILICON7 
	{ NOR_S7_SV7E160XT,			{ { { &AMD_T_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x22C4,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S7_SV7E160XB,			{ { { &AMD_B_16     }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x2249,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S7_SV7E320XT,			{ { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x22F6,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_S7_SV7E320XB,			{ { { &AMD_B_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x22F9,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	// TOSHIBA 
	{ NOR_TH50VPF6782AASB,		{ { { &AMD_64       }, { &AMD_T_64   } }, 						NULL, 2, 0x0098, 0x0074,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TH50VPF6783AASB,		{ { { &AMD_B_64     }, { &AMD_64     } }, 						NULL, 2, 0x0098, 0x0084,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TH50VPF5682CDSB,		{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0057,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TH50VPF5683CDSB,		{ { { &AMD_B_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0058,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00560002DDGB,		{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0049,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00560002EDGB,		{ { { &AMD_T_64   	}, { &DUMMY_MAP  } },  &TOSHIBA_HIDDEN_ROM_T_32K, 1, 0x0098, 0x0096,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, &TOSHIBA_HIDDEN_ROM_CMD_CB },
	{ NOR_TV00560003EDGB,		{ { { &AMD_B_64   	}, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0097,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00569002AABD,		{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x002D,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00569003AABD,		{ { { &AMD_B_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x002E,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00569002BABD,		{ { { &AMD_T_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0057,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00569003BABD,		{ { { &AMD_B_64     }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0058,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00578002AABD,		{ { { &AMD_T_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x007C,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00578003AABD,		{ { { &AMD_B_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0082,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV00578002DABD,		{ { { &AMD_T_128    }, { &DUMMY_MAP  } },  &TOSHIBA_HIDDEN_ROM_T_64K, 1, 0x0098, 0x0049,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, &TOSHIBA_HIDDEN_ROM_CMD_CB },
	{ NOR_TV00578003DABD,		{ { { &AMD_B_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x004A,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV0057A002AABD,		{ { { &AMD_T_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x001B,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TV0057A003AABD,		{ { { &AMD_B_128    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x001D,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TY00670002APGN,		{ { { &AMD_T2_128   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x00E7,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TC58FVM7TDD,			{ { { &AMD_T2_128   }, { &DUMMY_MAP  } },						NULL, 1, 0x0098, 0x0003, 	  0,	  0 }, &TOSHIBA_CMD_CB_PAGE_8WORDS_PGM, NULL},
	{ NOR_TC58FVM7BDD,    		{ { { &AMD_B2_128   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0014,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
	{ NOR_TY0068B012APGG,		{ { { &AMD_T_256    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x006B,      0,      0 }, &TOSHIBA_CMD_CB_BUF_PGM,	 	NULL },
	{ NOR_TY0068B013APGG,		{ { { &AMD_B_256    }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x006C,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TY00689002APGN,		{ { { &AMD_T2_256   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x0075,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_TC58FYM8T7D,			{ { { &AMD_T2_256   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x006F,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
    { NOR_TC58FYM8B7D,  	 	{ { { &AMD_B2_256   }, { &DUMMY_MAP  } },                       NULL, 1, 0x0098, 0x00EF,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
    { NOR_TC58FYM8T8D,		    { { { &AMD_T2_256   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x005F,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
    { NOR_TC58FYM8B8D,		    { { { &AMD_B2_256   }, { &DUMMY_MAP  } }, 						NULL, 1, 0x0098, 0x00CF,      0,      0 }, &AMD_CMD_CB_WORD_PGM, NULL },
	// SST
	{ NOR_SST34HF16x1,			{ { { &SST_16       }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x734B,      0,      0 }, &SST_CMD_CB_WORD_PGM, NULL },
	{ NOR_SST34HF32x4,			{ { { &SST_32       }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x7353,      0,      0 }, &SST_CMD_CB_WORD_PGM, NULL },
	{ NOR_SST32HF3241C,			{ { { &SST_32       }, { &DUMMY_MAP  } }, NULL, 1, 0x00BF, 0x235B,      0,      0 }, &SST_CMD_CB_WORD_PGM, NULL },
	// AMIC
	//- A82DL32x8T(U)
	{ NOR_A82DL3228T,       	{ { { &AMD_T_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x2255,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3228U,       	{ { { &AMD_B_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x2256,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3238T,       	{ { { &AMD_T_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x2250,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3238U,       	{ { { &AMD_B_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x2253,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3248T,       	{ { { &AMD_T_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x225C,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},
	{ NOR_A82DL3248U,       	{ { { &AMD_B_32  	}, { &DUMMY_MAP  } }, NULL, 1,  0x0037, 0x225F,     0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL},	
	// EON
	{ NOR_EON_EN29PL032,			{ { { &AMD_TB_32    }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x227E, 0x220A, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	{ NOR_EON_EN29PL064,			{ { { &AMD_TB_64    }, { &DUMMY_MAP  } }, NULL, 1, 0x007F, 0x227E, 0x2202, 0x2201 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },	
	// ESMT (Elite Semiconductor Memory Technology Inc)
	{ NOR_FM91L03208UA,         { { { &AMD_T_32     }, { &DUMMY_MAP  } }, NULL, 1, 0x008C, 0x22F6,      0,      0 }, &AMD_CMD_CB_UNLOCK_BYPASS_PGM, NULL },
	
	//------------------------------------------------------------------------------
	// INTEL family flash                                                           
	//------------------------------------------------------------------------------

	// INTEL 
	{ NOR_INTEL_28F640L18_T,	{ { { &INTEL_L_T_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880B,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128L18_T,	{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880C,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F256L18_T,	{ { { &INTEL_L_T_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880D,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640L18_B,	{ { { &INTEL_L_B_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880E,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128L18_B,	{ { { &INTEL_L_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x880F,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F256L18_B,	{ { { &INTEL_L_B_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8810,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640L30_T,	{ { { &INTEL_L_T_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8811,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128L30_T,	{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8812,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F256L30_T,	{ { { &INTEL_L_T_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8813,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640L30_B,	{ { { &INTEL_L_B_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8814,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128L30_B,	{ { { &INTEL_L_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8815,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F256L30_B,	{ { { &INTEL_L_B_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8816,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F320W30_T,	{ { { &INTEL_W_T_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8852,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F320W30_B,	{ { { &INTEL_W_B_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8853,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640W30_T,	{ { { &INTEL_W_T_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8854,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640W30_B,	{ { { &INTEL_W_B_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8855,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128W30_T,	{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8856,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128W30_B,	{ { { &INTEL_W_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8857,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F320W18_T,	{ { { &INTEL_W_T_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8862,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F320W18_B,	{ { { &INTEL_W_B_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8863,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640W18_T,	{ { { &INTEL_W_T_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8864,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F640W18_B,	{ { { &INTEL_W_B_64   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8865,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128W18_T,	{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8866,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F128W18_B,	{ { { &INTEL_W_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x8867,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_38F1010C0ZBL0,	{ { { &INTEL_W_B_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x88C5,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F1602C3BD70,	{ { { &INTEL_W_B_16   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x88C3,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_INTEL_28F3204C3TD70,	{ { { &INTEL_W_T_32   }, { &DUMMY_MAP     } }, &INTEL_OTP_1PR  , 1, 0x0089, 0x88C4,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, &INTEL_OTP_CMD_CB },
	// INTEL M18 Family (Sibley/Woodmere) 
	{ NOR_INTEL_PF38F30xxM0x0xx,	{ { { &INTEL_M18_128  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8900,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Non-Mux
	{ NOR_INTEL_PF38F30xxM0x1xx,	{ { { &INTEL_M18_128  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8903,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Ad-Mux	
	{ NOR_INTEL_PF48F40xxM0x0xx,	{ { { &INTEL_M18_256  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8901,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Non-Mux
	{ NOR_INTEL_PF48F40xxM0x1xx,	{ { { &INTEL_M18_256  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8904,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//AD-Mux
	{ NOR_INTEL_PF48F50xxM0x0xx,	{ { { &INTEL_M18_512  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x887E,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Non-Mux
	{ NOR_INTEL_PF48F50xxM0x1xx,	{ { { &INTEL_M18_512  }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x8881,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//AD-Mux
	{ NOR_INTEL_PF38F60xxM0x0xx,	{ { { &INTEL_M18_1024 }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x88B0,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//Non-Mux
	{ NOR_INTEL_PF38F60xxM0x1xx,	{ { { &INTEL_M18_1024 }, { &DUMMY_MAP } }, &INTEL_OTP_17PR , 1, 0x0089, 0x88B1,	   0,	   0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, &INTEL_OTP_CMD_CB },	//AD-Mux
	// RENESAS 
	{ NOR_M6MGD13BW66CDG,		{ { { &INTEL_W_B_64   }, { &INTEL_W_T_64  } }, NULL            , 2, 0x001C, 0x00B9, 0x001C, 0x00B8 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb, NULL },
	{ NOR_M6MGB64BM34CDG,		{ { { &INTEL_W_B_64   }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00B9,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb, NULL },
	{ NOR_M6MGT64BM34CDG,		{ { { &INTEL_W_T_64   }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00B8,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb, NULL },
	{ NOR_M6MGD15BM34CDG,		{ { { &INTEL_W_TB_128 }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00C8,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	// SILICON7 
	{ NOR_S7_SV6D2832UTA,		{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00C5,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	{ NOR_S7_SV6D2832UBA,		{ { { &INTEL_W_B_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00C6,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	{ NOR_S7_SV6C2832UTA,		{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00C9,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	{ NOR_S7_SV6C2832UBA,		{ { { &INTEL_W_B_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x001C, 0x00CA,      0,      0 }, &RENESAS_CMD_CB_128WORD_PAGE_PGM, NULL },
	// ST 
	{ NOR_ST_M30L0T7000T0,		{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0020, 0x88C4,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_ST_M30L0T7000B0,		{ { { &INTEL_L_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0020, 0x88C5,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_ST_M58PPR256J,		{ { { &INTEL_M18_256  }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8818,      0,      0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, NULL },
	{ NOR_ST_M58PPR512J,		{ { { &INTEL_M18_512  }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8819,      0,      0 }, &INTEL_CMD_CB_SIBLEY_BUF_PGM, NULL },
	{ NOR_ST_M58WR016QT,		{ { { &INTEL_W_T_16   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8812,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR016QB,		{ { { &INTEL_W_B_16   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8813,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR032QT,		{ { { &INTEL_W_T_32   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8814,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR032QB,		{ { { &INTEL_W_B_32   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x8815,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR064KT,        { { { &INTEL_W_T_64   }, { &DUMMY_MAP  	  } }, NULL			   , 1, 0x0020, 0x8810,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR064HU,		{ { { &INTEL_W_T_64   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x88C0,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_ST_M58WR064HL,		{ { { &INTEL_W_B_64   }, { &DUMMY_MAP  	  } }, NULL            , 1, 0x0020, 0x88C1,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	// NUMONYX (Intel+ST)
	{ NOR_M36C0W6050T0,         { { { &INTEL_W_T_64   }, { &DUMMY_MAP  	  } }, NULL			   , 1, 0x0020, 0x8848,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_M36C0W5030T0,         { { { &INTEL_W_T_32   }, { &DUMMY_MAP  	  } }, NULL			   , 1, 0x0020, 0x88BA,      0,      0 }, &INTEL_CMD_CB_WORD_PGM, NULL },
	{ NOR_M36L0T8060T3ZAQ,		{ { { &INTEL_L_T_256  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x0020, 0x880D,      0,      0 }, &INTEL_CMD_CB_32WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	// SHARP 
	{ NOR_SHARP_LH28F16,		{ { { &INTEL_W_B_16   }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x00E9,      0,      0 }, &SHARP_CMD_CB_WORD_PGM_NO_UNLOCK, NULL },
	{ NOR_SHARP_LRS18R,			{ { { &SHARP_T_32	  }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x00DC,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_SHARP_LRS1806A,		{ { { &INTEL_W_B_64   }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x00B1,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS1828C,		{ { { &INTEL_W_B_64   }, { &INTEL_W_T_64  } }, NULL            , 2, 0x00B0, 0x00B1, 0x00B0, 0x00B0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS18CK,		{ { { &INTEL_L_B_64   }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x008D,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS1862,		{ { { &INTEL_W_T_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x0010,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS1863,		{ { { &INTEL_W_B_128  }, { &INTEL_W_T_128 } }, NULL            , 2, 0x00B0, 0x0011, 0x00B0, 0x0010 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS18B0,		{ { { &INTEL_L_B_128  }, { &INTEL_L_128   } }, &INTEL_OTP_17PR , 2, 0x00B0, 0x000F,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_SHARP_LRS18BK,		{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, &SHARP_OTP_128_T, 1, 0x00B0, 0x001C,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_SHARP_LRS18C8A,		{ { { &INTEL_L_B_128  }, { &DUMMY_MAP     } }, &INTEL_OTP_17PR , 1, 0x00B0, 0x001D,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, &INTEL_OTP_CMD_CB },
	{ NOR_SHARP_LRS18D5_D7,	{ { { &INTEL_L_T_128  }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x001A,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },
	{ NOR_SHARP_LRS18A7A,	  { { { &INTEL_L_T_256  }, { &DUMMY_MAP     } }, NULL            , 1, 0x00B0, 0x000A,      0,      0 }, &SHARP_CMD_CB_16WORD_BUF_PGM, NULL },

#endif

	// <----- NOTICE!!! You can add your own flash device into whatever row before UNKNOWN_DEVICE_ENTRY. 
	
	UNKNOWN_DEVICE_ENTRY
};

#endif

#pragma arm section rodata
