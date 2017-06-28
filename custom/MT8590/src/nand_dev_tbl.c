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
#include "nand_drv_KINGSTON.h"

#if 1

NAND_MLC_FEATURE g_MICRON_MLC_FEATURE =
{1,1,2,2,2, 
0xEF,0xEE,7,0,0x89,0xFF,0x01,0x05,0x14, RTYPE_MICRON,
{{{0,6},{1,7},{2,0xffff},{3,0xffff},{4,10},{5,11}, 
{8,14},{9,15},{12,18},{13,19},{16,22},{17,23}, 
{20,26},{21,27},{24,30},{25,31},{28,34},{29,35},
{32,38},{33,39},{36,42},{37,43},{40,46},{41,47}, 
{44,50},{45,51},{48,54},{49,55},{52,58},{53,59}, 
{56,62},{57,63},{60,66},{61,67},{64,70},{65,71}, 
{68,74},{69,75},{72,78},{73,79},{76,82},{77,83}, 
{80,86},{81,87},{84,90},{85,91},{88,94},{89,95}, 
{92,98},{93,99},{96,102},{97,103},{100,106},{101,107},
{104,110},{105,111},{108,114},{109,115},{112,118},{113,119}, 
{116,122},{117,123},{120,126},{121,127},{124,130},{125,131},  
{128,134},{129,135},{132,138},{133,139},{136,142},{137,143}, 
{140,146},{141,147},{144,150},{145,151},{148,154},{149,155}, 
{152,158},{153,159},{156,162},{157,163},{160,166},{161,167}, 
{164,170},{165,171},{168,174},{169,175},{172,178},{173,179}, 
{176,182},{177,183},{180,186},{181,187},{184,190},{185,191}, 
{188,194},{189,195},{192,198},{193,199},{196,202},{197,203}, 
{200,206},{201,207},{204,210},{205,211},{208,214},{209,215}, 
{212,218},{213,219},{216,222},{217,223},{220,226},{221,227}, 
{224,230},{225,231},{228,234},{229,235},{232,238},{233,239}, 
{236,242},{237,243},{240,246},{241,247},{244,250},{245,251}, 
{248,0xffff},{249,0xffff},{252,0xffff},{253,0xffff},{254,0xffff},{255,0xffff}}},
{1,2,3,4,5,6,7}};

NAND_MLC_FEATURE g_SANDISK_MLC_FEATURE =
{1,1,2,2,2,0xEF,0xEE,32,0,0x11,0x5D,0x80,0x01,0x00, RTYPE_SANDISK,\
{{{0 ,2},{1 ,4},{3 ,6},{5 ,8},{7 ,10},{9 ,12},{11 ,14},{13 ,16},\
{15 ,18},{17 ,20},{19 ,22},{21 ,24},{23 ,26},{25 ,28},{27 ,30},{29 ,32},\
{31 ,34},{33 ,36},{35 ,38},{37 ,40},{39 ,42},{41 ,44},{43 ,46},{45 ,48},\
{47 ,50},{49 ,52},{51 ,54},{53 ,56},{55 ,58},{57 ,60},{59 ,62},{61 ,64},\
{63 ,66},{65 ,68},{67 ,70},{69 ,72},{71 ,74},{73 ,76},{75 ,78},{77 ,80},\
{79 ,82},{81 ,84},{83 ,86},{85 ,88},{87 ,90},{89 ,92},{91 ,94},{93 ,96},\
{95 ,98},{97 ,100},{99 ,102},{101 ,104},{103 ,106},{105 ,108},{107 ,110},{109 ,112},\
{111 ,114},{113 ,116},{115 ,118},{117 ,120},{119 ,122},{121 ,124},{123 ,126},{125 ,128},\
{127 ,130},{129 ,132},{131 ,134},{133 ,136},{135 ,138},{137 ,140},{139 ,142},{141 ,144},\
{143 ,146},{145 ,148},{147 ,150},{149 ,152},{151 ,154},{153 ,156},{155 ,158},{157 ,160},\
{159 ,162},{161 ,164},{163 ,166},{165 ,168},{167 ,170},{169 ,172},{171 ,174},{173 ,176},\
{175 ,178},{177 ,180},{179 ,182},{181 ,184},{183 ,186},{185 ,188},{187 ,190},{189 ,192},\
{191 ,194},{193 ,196},{195 ,198},{197 ,200},{199 ,202},{201 ,204},{203 ,206},{205 ,208},\
{207 ,210},{209 ,212},{211 ,214},{213 ,216},{215 ,218},{217 ,220},{219 ,222},{221 ,224},\
{223 ,226},{225 ,228},{227 ,230},{229 ,232},{231 ,234},{233 ,236},{235 ,238},{237 ,240},\
{239 ,242},{241 ,244},{243 ,246},{245 ,248},{247 ,250},{249 ,252},{251 ,254},{253 ,255}}},
{0x00000000,0x7C00007C,0x04007C78,0x78007874,
0x087C007C,0x007C7C78,0x7C7C7874,0x007C7470,
0x0078007C,0x00787C78,0x00787874,0x00787470,
0x0078706C,0x00040400,0x0004007C,0x0C047C78,
0x0C047874,0x1008007C,0x10080400,0x78747874,
0x78747470,0x7874706C,0x78746C68,0x78707874,
0x78707470,0x78706C68,0x7870706C,0x786C706C,
0x786C6C68,0x786C6864,0x74686C68,0x74686864}};

NAND_MLC_FEATURE g_SANDISK_19NM_MLC_FEATURE =
{1,1,2,2,2,0xEF,0xEE,16,0,0x11,0xFF,0x80,0x01,0x00, RTYPE_SANDISK_19NM,\
{{{0 ,2},{1 ,4},{3 ,6},{5 ,8},{7 ,10},{9 ,12},{11 ,14},{13 ,16},\
{15 ,18},{17 ,20},{19 ,22},{21 ,24},{23 ,26},{25 ,28},{27 ,30},{29 ,32},\
{31 ,34},{33 ,36},{35 ,38},{37 ,40},{39 ,42},{41 ,44},{43 ,46},{45 ,48},\
{47 ,50},{49 ,52},{51 ,54},{53 ,56},{55 ,58},{57 ,60},{59 ,62},{61 ,64},\
{63 ,66},{65 ,68},{67 ,70},{69 ,72},{71 ,74},{73 ,76},{75 ,78},{77 ,80},\
{79 ,82},{81 ,84},{83 ,86},{85 ,88},{87 ,90},{89 ,92},{91 ,94},{93 ,96},\
{95 ,98},{97 ,100},{99 ,102},{101 ,104},{103 ,106},{105 ,108},{107 ,110},{109 ,112},\
{111 ,114},{113 ,116},{115 ,118},{117 ,120},{119 ,122},{121 ,124},{123 ,126},{125 ,128},\
{127 ,130},{129 ,132},{131 ,134},{133 ,136},{135 ,138},{137 ,140},{139 ,142},{141 ,144},\
{143 ,146},{145 ,148},{147 ,150},{149 ,152},{151 ,154},{153 ,156},{155 ,158},{157 ,160},\
{159 ,162},{161 ,164},{163 ,166},{165 ,168},{167 ,170},{169 ,172},{171 ,174},{173 ,176},\
{175 ,178},{177 ,180},{179 ,182},{181 ,184},{183 ,186},{185 ,188},{187 ,190},{189 ,192},\
{191 ,194},{193 ,196},{195 ,198},{197 ,200},{199 ,202},{201 ,204},{203 ,206},{205 ,208},\
{207 ,210},{209 ,212},{211 ,214},{213 ,216},{215 ,218},{217 ,220},{219 ,222},{221 ,224},\
{223 ,226},{225 ,228},{227 ,230},{229 ,232},{231 ,234},{233 ,236},{235 ,238},{237 ,240},\
{239 ,242},{241 ,244},{243 ,246},{245 ,248},{247 ,250},{249 ,252},{251 ,254},{253 ,255}}},
{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}};


NAND_MLC_FEATURE g_TOSHIBA_MLC_FEATURE =
{1,1,2,2,2,0xEF,0xEE,7,7,0xFF,0xFF,0x80,0x01,0x00, RTYPE_TOSHIBA,\
{{{0 ,2},{1 ,4},{3 ,6},{5 ,8},{7 ,10},{9 ,12},{11 ,14},{13 ,16},\
{15 ,18},{17 ,20},{19 ,22},{21 ,24},{23 ,26},{25 ,28},{27 ,30},{29 ,32},\
{31 ,34},{33 ,36},{35 ,38},{37 ,40},{39 ,42},{41 ,44},{43 ,46},{45 ,48},\
{47 ,50},{49 ,52},{51 ,54},{53 ,56},{55 ,58},{57 ,60},{59 ,62},{61 ,64},\
{63 ,66},{65 ,68},{67 ,70},{69 ,72},{71 ,74},{73 ,76},{75 ,78},{77 ,80},\
{79 ,82},{81 ,84},{83 ,86},{85 ,88},{87 ,90},{89 ,92},{91 ,94},{93 ,96},\
{95 ,98},{97 ,100},{99 ,102},{101 ,104},{103 ,106},{105 ,108},{107 ,110},{109 ,112},\
{111 ,114},{113 ,116},{115 ,118},{117 ,120},{119 ,122},{121 ,124},{123 ,126},{125 ,128},\
{127 ,130},{129 ,132},{131 ,134},{133 ,136},{135 ,138},{137 ,140},{139 ,142},{141 ,144},\
{143 ,146},{145 ,148},{147 ,150},{149 ,152},{151 ,154},{153 ,156},{155 ,158},{157 ,160},\
{159 ,162},{161 ,164},{163 ,166},{165 ,168},{167 ,170},{169 ,172},{171 ,174},{173 ,176},\
{175 ,178},{177 ,180},{179 ,182},{181 ,184},{183 ,186},{185 ,188},{187 ,190},{189 ,192},\
{191 ,194},{193 ,196},{195 ,198},{197 ,200},{199 ,202},{201 ,204},{203 ,206},{205 ,208},\
{207 ,210},{209 ,212},{211 ,214},{213 ,216},{215 ,218},{217 ,220},{219 ,222},{221 ,224},\
{223 ,226},{225 ,228},{227 ,230},{229 ,232},{231 ,234},{233 ,236},{235 ,238},{237 ,240},\
{239 ,242},{241 ,244},{243 ,246},{245 ,248},{247 ,250},{249 ,252},{251 ,254},{253 ,255}}},
{0,1,2,3,4,5,6}};

NAND_MLC_FEATURE g_HYNIX_MLC_FEATURE =
{1,1,2,2,2, 
0xEF,0xEE,7,0,0xFF,0xFF,0x01,0x05,0x14, RTYPE_HYNIX, //cmd to be double confirmed  xiaolei
{{{0x0,0x4},{0x1,0x5},
{0x2,0x8},{0x3,0x9},{0x6,0xC},{0x7,0xD}, {0xA,0x10},{0xB,0x11},{0xE,0x14},{0xF,0x15},
{0x12,0x18},{0x13,0x19}, {0x16,0x1C},{0x17,0x1D},{0x1A,0x20},{0x1B,0x21},{0x1E,0x24},{0x1F,0x25},
{0x22,0x28},{0x23,0x29}, {0x26,0x2C},{0x27,0x2D},{0x2A,0x30},{0x2B,0x31},{0x2E,0x34},{0x2F,0x35},
{0x32,0x38},{0x33,0x39}, {0x36,0x3C},{0x37,0x3D},{0x3A,0x40},{0x3B,0x41},{0x3E,0x44},{0x3F,0x45},
{0x42,0x48},{0x43,0x49}, {0x46,0x4C},{0x47,0x4D},{0x4A,0x50},{0x4B,0x51},{0x4E,0x54},{0x4F,0x55},
{0x52,0x58},{0x53,0x59}, {0x56,0x5C},{0x57,0x5D},{0x5A,0x60},{0x5B,0x61},{0x5E,0x64},{0x5F,0x65},
{0x62,0x68},{0x63,0x69}, {0x66,0x6C},{0x67,0x6D},{0x6A,0x70},{0x6B,0x71},{0x6E,0x74},{0x6F,0x75},
{0x72,0x78},{0x73,0x79}, {0x76,0x7C},{0x77,0x7D},{0x7A,0x80},{0x7B,0x81},{0x7E,0x84},{0x7F,0x85},
{0x82,0x88},{0x83,0x89}, {0x86,0x8C},{0x87,0x8D},{0x8A,0x90},{0x8B,0x91},{0x8E,0x94},{0x8F,0x95},
{0x92,0x98},{0x93,0x99}, {0x96,0x9C},{0x97,0x9D},{0x9A,0xA0},{0x9B,0xA1},{0x9E,0xA4},{0x9F,0xA5},
{0xA2,0xA8},{0xA3,0xA9}, {0xA6,0xAC},{0xA7,0xAD},{0xAA,0xB0},{0xAB,0xB1},{0xAE,0xB4},{0xAF,0xB5},
{0xB2,0xB8},{0xB3,0xB9}, {0xB6,0xBC},{0xB7,0xBD},{0xBA,0xC0},{0xBB,0xC1},{0xBE,0xC4},{0xBF,0xC5},
{0xC2,0xC8},{0xC3,0xC9}, {0xC6,0xCC},{0xC7,0xCD},{0xCA,0xD0},{0xCB,0xD1},{0xCE,0xD4},{0xCF,0xD5},
{0xD2,0xD8},{0xD3,0xD9}, {0xD6,0xDC},{0xD7,0xDD},{0xDA,0xE0},{0xDB,0xE1},{0xDE,0xE4},{0xDF,0xE5},
{0xE2,0xE8},{0xE3,0xE9}, {0xE6,0xEC},{0xE7,0xED},{0xEA,0xF0},{0xEB,0xF1},{0xEE,0xF4},{0xEF,0xF5},
{0xF2,0xF8},{0xF3,0xF9}, {0xF6,0xFC},{0xF7,0xFD},{0xFA,0xFE},{0xFB,0xFF}}},
{1,2,3,4,5,6,7}};

NAND_MLC_FEATURE g_HYNIX_16NM_MLC_FEATURE =
{1,1,2,2,2, 
0xEF,0xEE,7,0,0xFF,0xFF,0x01,0x05,0x14, RTYPE_HYNIX_16NM, //cmd to be double confirmed  xiaolei
{{{0 ,2},{1 ,4},{3 ,6},{5 ,8},{7 ,10},{9 ,12},{11 ,14},{13 ,16},\
{15 ,18},{17 ,20},{19 ,22},{21 ,24},{23 ,26},{25 ,28},{27 ,30},{29 ,32},\
{31 ,34},{33 ,36},{35 ,38},{37 ,40},{39 ,42},{41 ,44},{43 ,46},{45 ,48},\
{47 ,50},{49 ,52},{51 ,54},{53 ,56},{55 ,58},{57 ,60},{59 ,62},{61 ,64},\
{63 ,66},{65 ,68},{67 ,70},{69 ,72},{71 ,74},{73 ,76},{75 ,78},{77 ,80},\
{79 ,82},{81 ,84},{83 ,86},{85 ,88},{87 ,90},{89 ,92},{91 ,94},{93 ,96},\
{95 ,98},{97 ,100},{99 ,102},{101 ,104},{103 ,106},{105 ,108},{107 ,110},{109 ,112},\
{111 ,114},{113 ,116},{115 ,118},{117 ,120},{119 ,122},{121 ,124},{123 ,126},{125 ,128},\
{127 ,130},{129 ,132},{131 ,134},{133 ,136},{135 ,138},{137 ,140},{139 ,142},{141 ,144},\
{143 ,146},{145 ,148},{147 ,150},{149 ,152},{151 ,154},{153 ,156},{155 ,158},{157 ,160},\
{159 ,162},{161 ,164},{163 ,166},{165 ,168},{167 ,170},{169 ,172},{171 ,174},{173 ,176},\
{175 ,178},{177 ,180},{179 ,182},{181 ,184},{183 ,186},{185 ,188},{187 ,190},{189 ,192},\
{191 ,194},{193 ,196},{195 ,198},{197 ,200},{199 ,202},{201 ,204},{203 ,206},{205 ,208},\
{207 ,210},{209 ,212},{211 ,214},{213 ,216},{215 ,218},{217 ,220},{219 ,222},{221 ,224},\
{223 ,226},{225 ,228},{227 ,230},{229 ,232},{231 ,234},{233 ,236},{235 ,238},{237 ,240},\
{239 ,242},{241 ,244},{243 ,246},{245 ,248},{247 ,250},{249 ,252},{251 ,254},{253 ,255}}},
{1,2,3,4,5,6,7}};
#else
#define g_MICRON_MLC_FEATURE NULL
#define g_SANDISK_MLC_FEATURE NULL
#define g_SANDISK_19NM_MLC_FEATURE NULL
#define g_TOSHIBA_MLC_FEATURE NULL
#define g_HYNIX_MLC_FEATURE NULL
#define g_HYNIX_16NM_MLC_FEATURE NULL
#endif

#pragma arm section rodata = "NAND_FLASH_TABLE"

const NAND_Device_S g_NandFlashDevTbl_Internal[] = {

    // NAND_FLASH_ID,       {{DEVICE_ID},   Density, Pages/B, Bytes/P, IO, Address Cycle, Spare size/Sector ,   N/A},       Command sets,               Driver sets}

    //------------------------------------------------------------------------------
    // SAMSUNG family flash
    //------------------------------------------------------------------------------

    // SAMSUNG: 512 page
     { NAND_K9F2808U0C,     { { 0xEC, 0x73, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    16/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16,0 , 512}, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9F5608Q0C,     { { 0xEC, 0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16,0 , 512 },&g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9F5608X0C,     { { 0xEC, 0x75, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16,0 , 512}, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9F5616Q0C,     { { 0xEC, 0x45, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16,0 , 512}, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9F5616X0C,     { { 0xEC, 0x55, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16,0 , 512}, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9K1208X0C,     { { 0xEC, 0x76, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, &SAMSUNG_OTP, &g_NAND_SAMSUNG_OTP_CB_FUNC_SET }

    ,{ NAND_K5E1257ACM,     { { 0xEC, 0x36, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K5E1G12ACF,     { { 0xEC, 0x78, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }

    ,{ NAND_K9K1G08X0A,     { { 0xEC, 0x79, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    // SAMSUNG: 2048 page
    ,{ NAND_K9F1G08Q0M,     { { 0xEC, 0xA1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9F1G08X0M,     { { 0xEC, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, &SAMSUNG_OTP, &g_NAND_SAMSUNG_OTP_CB_FUNC_SET } //TEST
    ,{ NAND_K9F1G16Q0M,     { { 0xEC, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9F1G16X0M,     { { 0xEC, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9F2G08X0M,     { { 0xEC, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9F2G16X0M,     { { 0xEC, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9F2G08R0A,     { { 0xEC, 0xAA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9K4G08Q0M,     { { 0xEC, 0xAC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9K4G08U0M,     { { 0xEC, 0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_KA100O015B_BJTT,{ { 0xEC, 0xBC, 0x00, 0x55, 0x54, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  } 
    ,{ NAND_KA100O015E_BJTT,{ { 0xEC, 0xBC, 0x00, 0x66, 0x56, 0xFF, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  } 
    ,{ NAND_KF94G16Q4V,     { { 0xEC, 0xBC, 0x00, 0x66, 0x54, 0xFF, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  } 
    ,{ NAND_KF94G16Q4X,     { { 0xEC, 0xBC, 0x00, 0x66, 0x62, 0xFF, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  } 
    ,{ NAND_K9K4G16Q0M,     { { 0xEC, 0xBC, 0x00, 0x55, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_KF98G16Q4X,     { { 0xEC, 0xB3, 0x00, 0x66, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  } 
    
    ,{ NAND_K9K4G16U0M,     { { 0xEC, 0xCC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9K8G08U0M,     { { 0xEC, 0xD3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_K9K8G16U0K, 	{ { 0xEC, 0xBC, 0x00, 0x6A, 0x56, 0xFF, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS,  5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    // MICRON: 2048 page
    ,{ NAND_MT29F1G08ABB,   { { 0x2C, 0xA1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_MT29F1G16ABB,   { { 0x2C, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_MT29F2G08ABAFA, { { 0x2C, 0xDA, 0x90, 0x95, 0x04, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 112,0 , 1024}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
	,{ NAND_MT29F2G08AAC,   { { 0x2C, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_MT29F2G08ABD,   { { 0x2C, 0xAA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_MT29F2G16ABD,   { { 0x2C, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    //,{ NAND_MT29F4G08BAB,   { { 0x2C, 0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_MT29F4G08ABA,   { { 0x2C, 0xDC, 0x90, 0xa6, 0x54, 0xFF, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_8BITS,  5, 56,0 , 1024}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
	,{ NAND_MT29C4G96MAZAPCJA_5IT,  { { 0x2C, 0xBC, 0x90, 0x55, 0x56, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_MT29C4G48MAAHBAAKS_5WT,  { { 0x2C, 0xBC, 0x90, 0x66, 0x54, 0xFF, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 28,0 , 512 }, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_MT29F4G16ABD,   { { 0x2C, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
	,{ NAND_MT29F8G08ABACA, { { 0x2C, 0xD3, 0x90, 0xA6, 0x64, 0xFF, 0xFF},   1024/*MB*/, 64, 4096, NAND_IO_8BITS, 5, 56,0 , 1024}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_MT29F64G08CBABB,   { { 0x2C, 0x64, 0x44, 0x4B, 0xA9, 0xFF, 0xFF},   8192/*MB*/, 256, 8192, NAND_IO_8BITS, 5, 80,0 , 1024}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_MICRON_MLC_FEATURE, NULL, NULL  }
    ,{ NAND_MT29F32G08CBADB,   { { 0x2C, 0x44, 0x44, 0x4B, 0xA9, 0xFF, 0xFF},   4096/*MB*/, 256, 8192, NAND_IO_8BITS, 5, 80,0 , 1024}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_MICRON_MLC_FEATURE, NULL, NULL  }
    ,{ NAND_MT29F128G08CFAAA,   { { 0x2C, 0x88, 0x04, 0x4B, 0xA9, 0xFF, 0xFF},   16384/*MB*/, 256, 8192, NAND_IO_8BITS, 5, 52,  2, 1024}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_MICRON_MLC_FEATURE, NULL, NULL  }
    //sandisk   NAND_SDTNRGAMA_008GK: K stands "wakes in legacy mode only, and can be switched to toggle mode"
    ,{ NAND_SDTNRGAMA_008GK,   { { 0x45, 0xDE, 0x94, 0x93, 0x76, 0x50, 0xFF},   8192/*MB*/, 256, 16384, NAND_IO_8BITS, 5, 80,0 , 1024}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_SANDISK_MLC_FEATURE, NULL, NULL  }
    ,{ NAND_SDTNRFAMA_004GK,   { { 0x45, 0xD7, 0x84, 0x93, 0xFF, 0xFF, 0xFF},   4096/*MB*/, 256, 16384, NAND_IO_8BITS, 5, 80,0 , 1024}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_SANDISK_MLC_FEATURE, NULL, NULL  }
///    ,{ NAND_SDTNQGAMA_008G,   { { 0x45, 0xDE, 0x94, 0x93, 0x76, 0x57, 0xFF},   8192/*MB*/, 256, 16384, NAND_IO_8BITS, 5, 80,0 , 1024}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_SANDISK_19NM_MLC_FEATURE, NULL, NULL  }	
	// Infineon: 512 page
    ,{ NAND_HYF33DS51280,   { { 0xC1, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_SAMSUNG_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    // Infineon: 2048 page
    ,{ NAND_HYF33DS1G80,    { { 0xC1, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_HYF33DS1G16,    { { 0xC1, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }

	//------------------------------------------------------------------------------
	// KINGSTON family flash               
	//------------------------------------------------------------------------------                                       
	//KINGSTON 2048 page
	,{ NAND_KSLCBBL1FB4G3A,     { { 0x98, 0xBC, 0x90, 0x55, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS,  5, 16,0 , 512}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  

	//SPANSION family flash
	//SPANSION 2048 page
	///,{ NAND_S34ML04G200TFI00,     { { 0x01, 0xDC, 0x90, 0x95, 0x56, 0x1, 0xDC},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 26,0 , 512}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	,{ NAND_S34ML02G200TFI00,     { { 0x01, 0xDA, 0x90, 0x95, 0x46, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 64,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	,{ NAND_S34ML04G200TFI00,     { { 0x01, 0xDC, 0x90, 0x95, 0x56, 0x1, 0xDC},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 64,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	,{ NAND_S34ML08G201TFI00,     { { 0x01, 0xD3, 0xD1, 0x95, 0x5a, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 64,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	,{ NAND_S34ML08G101TFI00,     { { 0x01, 0xD3, 0xD1, 0x95, 0x58, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  

	,{ NAND_S34ML04G101TFI00,     { { 0x01, 0xDC, 0x90, 0x95, 0x54, 0xFF, 0xFF},  512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	,{ NAND_S34ML02G101TFI00,     { { 0x01, 0xDA, 0x90, 0x95, 0x44, 0xFF, 0xFF},  256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	,{ NAND_S34ML01G101TFI00,     { { 0x01, 0xF1, 0x00, 0x1D, 0xFF, 0xFF, 0xFF},  128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  } 
	,{ NAND_S34ML01G201TFI00,     { { 0x01, 0xF1, 0x80, 0x1D, 0xFF, 0xFF, 0xFF},  128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	//-----------------------------------------------------------------------------
    // TOSHIBA family flash
    //-----------------------------------------------------------------------------

    // TOSHIBA: 512 page
    ,{ NAND_TC58DVM82A1FT,  { { 0x98, 0x75, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16,0 , 512}, &g_NAND_TOSHIBA_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TC58DVM92A1FT,  { { 0x98, 0x76, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_TOSHIBA_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TC58DVG02A1FT,  { { 0x98, 0x79, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_TOSHIBA_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    // TOSHIBA: 2048 page
    ,{ NAND_TC58NWM9S3B,    { { 0x98, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TC58NVG0S3AFT,  { { 0x98, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TC58NYG0S8C,    { { 0x98, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TC58NYG1S8C,    { { 0x98, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TH58NVG1S3AFT,  { { 0x98, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
   // ,{ NAND_TH58NVG2D4BFT,  { { 0x98, 0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TH58NVG2S0HTAI0,  { { 0x98, 0xDC, 0x90, 0x26, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_8BITS,  5, 64,0 , 1024}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TH58NVG3S0HTAI0,  { { 0x98, 0xD3, 0x91, 0x26, 0x76, 0x16, 0xFF},   1024/*MB*/, 64, 4096, NAND_IO_8BITS,  5, 64,0 , 1024}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
	//- 16bit
    ,{ NAND_TC58NWM9S8C,    { { 0x98, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TC58NWG0S8C,    { { 0x98, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_TH58NVG1S8BFT,  { { 0x98, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    // TOSHIBA: 4096 page
#if 0
    ,{ NAND_TC58NVG4D1DTG,  { { 0x98, 0xD5, 0xFF, 0xFF },  2048/*MB*/, 128, 4096, NAND_IO_8BITS,  5, 16,0 ,512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
#else
    ,{ NAND_TC58NVG4D1DTG,  { { 0x98, 0xD5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS, 5, 27,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
#endif
    ,{ NAND_TY58NYG3S5F,    { { 0x98, 0xB3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  2048/*MB*/, 128, 4096, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }    
    ,{ NAND_TYBC0A111557KC, { { 0x98, 0xBC, 0x90, 0x66, 0xFF, 0xFF, 0xFF},  512/*MB*/,  64,  4096, NAND_IO_16BITS, 5, 16,0 , 512},  &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL } 
    ,{ NAND_TC58TEG5DCKTA00,   { { 0x98, 0xD7, 0x84, 0x93, 0xFF, 0xFF, 0xFF},   4096/*MB*/, 256, 16384, NAND_IO_8BITS, 5, 80,0 , 1024}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_TOSHIBA_MLC_FEATURE, NULL, NULL  }
    ,{ NAND_TC58TEG6DDKTA00,   { { 0x98, 0xDE, 0x94, 0x93, 0xFF, 0xFF, 0xFF},   8192/*MB*/, 256, 16384, NAND_IO_8BITS, 5, 80,0 , 1024}, &g_NAND_TOSHIBA_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_TOSHIBA_MLC_FEATURE, NULL, NULL  }	
    //------------------------------------------------------------------------------
    // HYNIX family flash
    //------------------------------------------------------------------------------

    // HYNIX: 512 page
    ,{ NAND_HY27XS08561M,   { { 0xAD, 0x75, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16,0 , 512}, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_HY27US16561M,   { { 0xAD, 0x55, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16,0 , 512}, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_HY27XS08121M,   { { 0xAD, 0x76, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_H8BCS0CG0MBR,   { { 0xAD, 0x36, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_HYD0SFG0MF1P,   { { 0xAD, 0x46, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }

    ,{ NAND_HY27XA081G1M,   { { 0xAD, 0x79, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_HY27XA161G1M,   { { 0xAD, 0x74, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_HYNIX_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    // HYNIX: 2048 page,0 
    ,{ NAND_HY27UF081G2M,   { { 0xAD, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_H8BCS0PG0MBP,   { { 0xAD, 0xA1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_HYD0SQG0MF1P,   { { 0xAD, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_HY27UF082G2M,   { { 0xAD, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_HY27UF162G5A,   { { 0xAD, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_HY27UT088G2A,   { { 0xAD, 0xD3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 , 512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_H9DA4GH4JJAMCR_4EM, { { 0xAD, 0xBC, 0x90, 0x55, 0x54, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 ,512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_H9DA4VH2GJAMCR_4EM, { { 0xAD, 0xBC, 0x80, 0x16, 0x20, 0xFF, 0xFF},   512/*MB*/, 64, 4096, NAND_IO_16BITS, 5, 16,0 ,512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL,NULL, NULL }    
    ,{ NAND_H9TA4GH2GDMCPR, { { 0xAD, 0xBC, 0x90, 0x55, 0x56, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS,  5, 16,0 , 512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_H8BCS0UN0MCR,   { { 0xAD, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS,  5, 16,0 , 512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }        
    ,{ NAND_H9DA4GH4JJAMCR, { { 0xAD, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 , 512}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
 ////   ,{ NAND_H27UBG8T2CTR, { { 0xAD, 0xD7, 0x94, 0x91, 0xFF, 0xFF, 0xFF},   4096/*MB*/, 256, 8192, NAND_IO_8BITS, 5, 80,0 , 1024}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_HYNIX_MLC_FEATURE, NULL, NULL  }
////    ,{ NAND_H27UCG8T2ETR, { { 0xAD, 0xDE, 0x14, 0xA7, 0xFF, 0xFF, 0xFF},   8192/*MB*/, 256, 16384, NAND_IO_8BITS, 5, 80,0 , 1024}, &g_NAND_HYNIX_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, &g_HYNIX_16NM_MLC_FEATURE, NULL, NULL  }	
    //------------------------------------------------------------------------------
    // STMicroelectronics family flash
    //------------------------------------------------------------------------------

    // ST: 512 page
    ,{ NAND_ST128W3A,       { { 0x20, 0x73, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    16/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST128W4A,       { { 0x20, 0x53, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    16/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST256W3A,       { { 0x20, 0x75, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_8BITS,  3, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST256W4A,       { { 0x20, 0x55, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/, 32,  512, NAND_IO_16BITS, 3, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST512W3A,       { { 0x20, 0x76, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST512R3A,       { { 0x20, 0x36, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST512W4A,       { { 0x20, 0x56, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST512R4A,       { { 0x20, 0x46, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST01GW3A,       { { 0x20, 0x79, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_8BITS,  4, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST01GW4A,       { { 0x20, 0x59, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 32,  512, NAND_IO_16BITS, 4, 16,0 , 512}, &g_NAND_ST_512_CMD_SET,  &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
    // ST: 2048 page
    ,{ NAND_ST512R3B,       { { 0x20, 0xA2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST512W3B,       { { 0x20, 0xF2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST512R4B,       { { 0x20, 0xB2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST512W4B,       { { 0x20, 0xC2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST01GR3B,       { { 0x20, 0xA1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST01GW3B,       { { 0x20, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  4, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST01GR4B,       { { 0x20, 0xB1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST01GW4B,       { { 0x20, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_16BITS, 4, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST02GR3B,       { { 0x20, 0xAA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST02GW3B,       { { 0x20, 0xDA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST02GR4B,       { { 0x20, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST02GW4B,       { { 0x20, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST04GR3B,       { { 0x20, 0xAC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST04GW3B,       { { 0x20, 0xDC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    //,{ NAND_ST04GR4B,     { { 0x20, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, &SAMSUNG_OTP, &g_NAND_SAMSUNG_OTP_CB_FUNC_SET }  //TEST
    ,{ NAND_ST04GR4B,       { { 0x20, 0xBC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST04GW4B,       { { 0x20, 0xCC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST08GR3B,       { { 0x20, 0xA3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST08GW3B,       { { 0x20, 0xD3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST08GR4B,       { { 0x20, 0xB3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_ST08GW4B,       { { 0x20, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  1024/*MB*/, 64, 2048, NAND_IO_16BITS, 5, 16,0 ,512}, &g_NAND_ST_2048_CMD_SET, &g_NAND_ST_CB_FUNC_SET, NULL, NULL, NULL  }

    //------------------------------------------------------------------------------
    // RENESAS superAND family flash
    //------------------------------------------------------------------------------

    // RENESAS superAND: 2048 page
    ,{ NAND_SUPERAND51208,      { { 0x07, 0x58, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/,  8, 2048, NAND_IO_8BITS,  4, 16,0 ,512}, &g_NAND_superAND_2048_CMD_SET, &g_NAND_superAND_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_SUPERAND51216,      { { 0x07, 0x5B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    64/*MB*/,  8, 2048, NAND_IO_16BITS, 4, 16,0 ,512}, &g_NAND_superAND_2048_CMD_SET, &g_NAND_superAND_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_M6MGA157F2LCWG08,   { { 0x07, 0x49, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/,  16, 512, NAND_IO_8BITS,  4, 16,0 ,512}, &g_NAND_superAND_512_CMD_SET, &g_NAND_superAND_CB_FUNC_SET, NULL, NULL, NULL  }
    ,{ NAND_M6MGA157F2LCWG16,   { { 0x07, 0x4A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},    32/*MB*/,  16, 512, NAND_IO_16BITS, 4, 16,0 ,512}, &g_NAND_superAND_512_CMD_SET, &g_NAND_superAND_CB_FUNC_SET, NULL, NULL, NULL  }

     //ESMT  family flash
     ,{ NAND_F592G81A,   { { 0xC8, 0xDA, 0x90, 0x95, 0x44, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 ,512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }
	 //Winbond  family flash
	 ,{ NAND_W29N04GV,   { { 0xEF, 0xDC, 0x90, 0x95, 0x54, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	 ,{ NAND_W29N02GV,   { { 0xEF, 0xDA, 0x90, 0x95, 0x04, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	 ,{ NAND_W29N01GV,   { { 0xEF, 0xF1, 0x80, 0x95, 0x00, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	 ,{ NAND_W29N01HV,   { { 0xEF, 0xF1, 0x00, 0x95, 0x00, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	 //MXIC  family flash
	  ,{ NAND_MX30LF1G18AC,    { { 0xC2, 0xF1, 0x80, 0x95, 0x02, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	 ,{ NAND_MX30LF1G08AA,     { { 0xC2, 0xF1, 0x80, 0x1D, 0xC2, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	 ,{ NAND_MX30LF2G18AC,     { { 0xC2, 0xDA, 0x90, 0x95, 0x06, 0xFF, 0xFF},   256/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	 ,{ NAND_MX30LF4G18AC,     { { 0xC2, 0xDC, 0x90, 0x95, 0x56, 0xFF, 0xFF},   512/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  
	 ,{ NAND_MX60LF8G18AC,     { { 0xC2, 0xD3, 0xD1, 0x95, 0x5A, 0xFF, 0xFF},   1024/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 32,0 , 1024}, &g_NAND_KINGSTON_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }  

    //------------------------------------------------------------------------------
    // PowerFlash family flash
    //------------------------------------------------------------------------------
    ,{ NAND_ASU1GA30GT_G30CA,   { { 0x92, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   128/*MB*/, 64, 2048, NAND_IO_8BITS,  5, 16,0 ,512}, &g_NAND_SAMSUNG_2048_CMD_SET, &g_NAND_COMMON_CB_FUNC_SET, NULL, NULL, NULL  }

    // end of the table
    ,{ NAND_UNKNOWN, { { 0, 0, 0, 0, 0, 0, 0 }, 0, 0, 0,  0, 0, 0 ,0,0}, NULL, NULL, NULL , NULL, NULL  }
};

#pragma arm section rodata
