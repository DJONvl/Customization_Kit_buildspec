/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/

#include "NFI.h"

#include "logging.h"

#include "nand_drv_COMMON_v3_1.h"
#include "nand_util_func.h"


#define DRV_WriteReg(addr,data)     ((*(volatile uint16 *)(addr)) = (uint16)data)
#define DRV_Reg(addr)               (*(volatile uint16 *)(addr))
#define DRV_WriteReg32(addr,data)     ((*(volatile uint32 *)(addr)) = (uint32)data)
#define DRV_Reg32(addr)               (*(volatile uint32 *)(addr))
#define DRV_WriteReg8(addr,data)     ((*(volatile uint8 *)(addr)) = (uint8)data)
#define DRV_Reg8(addr)               (*(volatile uint8 *)(addr))


#ifdef DUMP_SPEED
extern U32 gpt4_get_current_tick (void);
extern uint32 g_ds_nand_block_erase_number,  g_ds_nand_block_erase_time;
extern uint32 g_ds_nand_page_program_number, g_ds_nand_page_program_time;
extern uint32 g_ds_nand_page_read_number,    g_ds_nand_page_read_time;
#endif

#if	CFG_2CS_NAND
extern bool g_bTricky_CS;
extern bool g_bTryrun_CS;
//extern bool g_b2Die_CS; 
extern uint32 g_nanddie_pages;
#endif

bool use_randomizer = FALSE;

bool use_HWECC = TRUE;

extern bool bad_block_remark_done;

bool bl_region_ing = FALSE;

//to reset fdm size and ecc level
//2015.6.24 for l85a
bool reset_fdm_and_ecc = FALSE;
uint32 re_fdm_size = 2;
uint32 re_ecc_bit_cfg = ECC_CNFG_ECC40;
uint32 re_ecc_level = 40;
uint32 org_ecc_bit_cfg = ECC_CNFG_ECC40;
uint32 org_ecc_level = 40;
//this is a fixed value now. if pl partition size is changed, this value should be changed the same time
uint32 second_part_start_page = 3584; //take l85a as example. 3584 = 7 * 512
//

#define MAX_SECTOR_NUM 16
struct debug_info_MLC
{
	char magic[7];
	uint8 sector_number;
	uint16 sector_ecc_bitmap;
	uint8 sector_bit_flip_num[MAX_SECTOR_NUM];
};
struct debug_info_SLC
{
	char magic[7];
	uint8 sector_number;
	uint16 sector_ecc_bitmap;
	uint8 sector_bit_flip_num[6];
};

struct debug_info_MLC dbg_MLC;
struct debug_info_SLC dbg_SLC;
uint8 debug_spare[1024];
char magic[7] = {'E','C','C','_','D','B','G'};

#if defined(__NFI_SUPPORT_TLC__)
uint32 _u4TLC2SLCRatio = 6;
uint32 _u4UBISlcPgS     = 0xFFFFFFFF;
uint32 _u4UBISlcPgE     = 0xFFFFFFFF;
uint32 _u4LogicPdIdx = 0;
bool _fgMcuRead = FALSE;
bool _fgUbiPart    = FALSE;
#endif
//------------------------------------------------------------------------------
// COMMON Callback Function Set                                                 
//------------------------------------------------------------------------------
const NAND_CMD_Callback_S	g_NAND_COMMON_CB_FUNC_SET={
	ID_COMMON_CB_FUNC_SET
	,NAND_COMMON_ReadID_V3
	,NAND_COMMON_Reset_V3
	,NULL
	,NAND_COMMON_ReadStatus_V3
	,NAND_COMMON_BlockErase_V3
	,NAND_COMMON_BadBlockSymbol_Check_V3
	,NAND_COMMON_BadBlockSymbol_Set_V3
	,NAND_COMMON_PageRead_V3
#if defined(__NFI_SUPPORT_TLC__)
       ,NFI_TLC_PageWrite	
#else
       ,NAND_COMMON_PageProgram_V3
#endif
	,NAND_COMMON_SpareRead_V3
	,NAND_COMMON_SpareProgram_V3
	,NAND_COMMON_PageSpareProgram_V3
	,NAND_COMMON_Randomizer_Config
       ,NAND_COMMON_TLC_BlockProgram
       ,NAND_COMMON_PageProgramBad_V3
};

#define NFI_ISSUE_COMMAND(cmd, col_addr, row_addr, col_num, row_num) \
   do { \
      DRV_WriteReg(NFI_CMD,cmd);\
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);\
      DRV_WriteReg32(NFI_COLADDR, col_addr);\
      DRV_WriteReg32(NFI_ROWADDR, row_addr);\
      DRV_WriteReg(NFI_ADDRNOB, col_num | (row_num<<ADDR_ROW_NOB_SHIFT));\
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);\
   }while(0);

void NFI_Reset(void)
{
   *NFI_CON = CON_FIFO_FLUSH|CON_NFI_RST;
   while( *NFI_STA & (STA_NFI_FSM_MASK|STA_NAND_FSM_MASK));
   while( FIFO_RD_REMAIN(*NFI_FIFOSTA) || FIFO_WR_REMAIN(*NFI_FIFOSTA) );
}   

#if 1 //xiaolei add for randomizer
#define SS_RANDOM_EN
//#define TSB_RANDOM_EN
#define SS_SEED_NUM 128 //256
bool fgRandomEn = FALSE;
bool fgSSRandomizer = TRUE;
bool fgPageReload = FALSE;

#ifdef SS_RANDOM_EN
static U16 SS_RANDOM_SEED[SS_SEED_NUM] =
{
    //for page 0~127
        0x576A, 0x05E8, 0x629D, 0x45A3, 0x649C, 0x4BF0, 0x2342, 0x272E,
        0x7358, 0x4FF3, 0x73EC, 0x5F70, 0x7A60, 0x1AD8, 0x3472, 0x3612,
        0x224F, 0x0454, 0x030E, 0x70A5, 0x7809, 0x2521, 0x484F, 0x5A2D,
        0x492A, 0x043D, 0x7F61, 0x3969, 0x517A, 0x3B42, 0x769D, 0x0647,
        0x7E2A, 0x1383, 0x49D9, 0x07B8, 0x2578, 0x4EEC, 0x4423, 0x352F,
        0x5B22, 0x72B9, 0x367B, 0x24B6, 0x7E8E, 0x2318, 0x6BD0, 0x5519,
        0x1783, 0x18A7, 0x7B6E, 0x7602, 0x4B7F, 0x3648, 0x2C53, 0x6B99,
        0x0C23, 0x67CF, 0x7E0E, 0x4D8C, 0x5079, 0x209D, 0x244A, 0x747B,
        0x350B, 0x0E4D, 0x7004, 0x6AC3, 0x7F3E, 0x21F5, 0x7A15, 0x2379,
        0x1517, 0x1ABA, 0x4E77, 0x15A1, 0x04FA, 0x2D61, 0x253A, 0x1302,
        0x1F63, 0x5AB3, 0x049A, 0x5AE8, 0x1CD7, 0x4A00, 0x30C8, 0x3247,
        0x729C, 0x5034, 0x2B0E, 0x57F2, 0x00E4, 0x575B, 0x6192, 0x38F8,
        0x2F6A, 0x0C14, 0x45FC, 0x41DF, 0x38DA, 0x7AE1, 0x7322, 0x62DF,
        0x5E39, 0x0E64, 0x6D85, 0x5951, 0x5937, 0x6281, 0x33A1, 0x6A32,
        0x3A5A, 0x2BAC, 0x743A, 0x5E74, 0x3B2E, 0x7EC7, 0x4FD2, 0x5D28,
        0x751F, 0x3EF8, 0x39B1, 0x4E49, 0x746B, 0x6EF6, 0x44BE, 0x6DB7/*,
	
	//for page 128~255
        0x576A, 0x05E8, 0x629D, 0x45A3, 0x649C, 0x4BF0, 0x2342, 0x272E,
        0x7358, 0x4FF3, 0x73EC, 0x5F70, 0x7A60, 0x1AD8, 0x3472, 0x3612,
        0x224F, 0x0454, 0x030E, 0x70A5, 0x7809, 0x2521, 0x484F, 0x5A2D,
        0x492A, 0x043D, 0x7F61, 0x3969, 0x517A, 0x3B42, 0x769D, 0x0647,
        0x7E2A, 0x1383, 0x49D9, 0x07B8, 0x2578, 0x4EEC, 0x4423, 0x352F,
        0x5B22, 0x72B9, 0x367B, 0x24B6, 0x7E8E, 0x2318, 0x6BD0, 0x5519,
        0x1783, 0x18A7, 0x7B6E, 0x7602, 0x4B7F, 0x3648, 0x2C53, 0x6B99,
        0x0C23, 0x67CF, 0x7E0E, 0x4D8C, 0x5079, 0x209D, 0x244A, 0x747B,
        0x350B, 0x0E4D, 0x7004, 0x6AC3, 0x7F3E, 0x21F5, 0x7A15, 0x2379,
        0x1517, 0x1ABA, 0x4E77, 0x15A1, 0x04FA, 0x2D61, 0x253A, 0x1302,
        0x1F63, 0x5AB3, 0x049A, 0x5AE8, 0x1CD7, 0x4A00, 0x30C8, 0x3247,
        0x729C, 0x5034, 0x2B0E, 0x57F2, 0x00E4, 0x575B, 0x6192, 0x38F8,
        0x2F6A, 0x0C14, 0x45FC, 0x41DF, 0x38DA, 0x7AE1, 0x7322, 0x62DF,
        0x5E39, 0x0E64, 0x6D85, 0x5951, 0x5937, 0x6281, 0x33A1, 0x6A32,
        0x3A5A, 0x2BAC, 0x743A, 0x5E74, 0x3B2E, 0x7EC7, 0x4FD2, 0x5D28,
        0x751F, 0x3EF8, 0x39B1, 0x4E49, 0x746B, 0x6EF6, 0x44BE, 0x6DB7
        */
};
#endif

#ifdef TSB_RANDOM_EN
static U16 TSB_RANDOM_SEED0[16] =
{
	0x056C, 0x0FBC, 0x1717, 0x0A52, 0x07CF, 0x064E, 0x098C, 0x1399, 
	0x1246, 0x0150, 0x11F1, 0x1026, 0x1C45, 0x012A, 0x0849, 0x1D4C
};
static U32 TSB_RANDOM_SEED1[16] = 
{
	0x0001BC77, 0x0000090C, 0x0001E1AD, 0x0000D564, 
	0x0001CB3B, 0x000167A7, 0x0001BD54, 0x00011690,
	0x00014794, 0x00000787, 0x00017BAD, 0x000173AB,
	0x00008B63, 0x00003A3A, 0x0001CB9B, 0x0000AF6E
};
static U32 TSB_RANDOM_SEED2[16] =
{
	0x0005D356, 0x0007F880, 0x0006DB67, 0x0006FBAC,
	0x00037CD1, 0x0000F1D2, 0x0002C2AF, 0x0001D310,
	0x0000F34F, 0x00073450, 0x00046EAA, 0x00079634,
	0x00042924, 0x000435D5, 0x00028350, 0x0000949E
};
static U32 TSB_RANDOM_SEED3[16] =
{
	0x001F645D, 0x003D9E86, 0x007D7EA0, 0x006823DD,
	0x005C91F0, 0x00506BE8, 0x004B5FB7, 0x0027E53B,
	0x00347BC4, 0x003D8927, 0x005403F5, 0x0001B987,
	0x004BF708, 0x001A7BAA, 0x001E8309, 0x0000193A
};
#endif
void NFI_RANDOMIZER_ENCODE(U32 u4PgIdx, bool fgRandom, bool fgSS, bool fgPage, U32 u4SectorIdx)
{
	U32 u4NFI_CFG = 0;
	U32 u4NFI_RAN_CFG = 0;
       U32 u4PgIdPre = 1;
	U32 u4OrgPgidx = u4PgIdx;
#if defined(__NFI_SUPPORT_TLC__)    
if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
{
        u4PgIdPre = 3;
       
        if(     (g_pNandInfo->m_dev->m_tlc_setting->normaltlc) 
            &&(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) )
        {
             if( (0xFFFFFFFF != _u4UBISlcPgS) && (_u4UBISlcPgS < _u4LogicPdIdx && _u4LogicPdIdx <_u4UBISlcPgE && _fgUbiPart))
                 u4PgIdx =  _u4LogicPdIdx - _u4UBISlcPgS - 1;
             else
                 u4PgIdx = u4PgIdx*3;
        }
        
        u4PgIdx = u4PgIdx % (g_pNandInfo->m_dev->m_hw_info.m_pages_per_block * 3);  //to fit flash which page per block is not 32 aligned
}
#endif
	u4NFI_CFG = DRV_Reg32(NFI_CNFG);

	if(fgRandom)
	{
		if(fgSS) //samsung randomizer
		{
			#ifdef SS_RANDOM_EN
			DRV_WriteReg32(NFI_RANDOM_ENSEED01_TS, 0); //clear TSB seed  to avoid S/TSB change side effect
			DRV_WriteReg32(NFI_RANDOM_ENSEED02_TS, 0);
			DRV_WriteReg32(NFI_RANDOM_ENSEED03_TS, 0);
			DRV_WriteReg32(NFI_RANDOM_ENSEED04_TS, 0);
			DRV_WriteReg32(NFI_RANDOM_ENSEED05_TS, 0);
			DRV_WriteReg32(NFI_RANDOM_ENSEED06_TS, 0);
				
			u4NFI_CFG |= CNFG_HW_RAN_SEL;
			if((g_pNandInfo->m_dev->m_hw_info.m_pages_per_block * u4PgIdPre) <= SS_SEED_NUM)
				u4NFI_RAN_CFG = RAN_CNFG_ENCODE_SEED(SS_RANDOM_SEED[u4PgIdx % ((g_pNandInfo->m_dev->m_hw_info.m_pages_per_block)-1)]) | RAN_CNFG_ENCODE_EN;
			else
				u4NFI_RAN_CFG = RAN_CNFG_ENCODE_SEED(SS_RANDOM_SEED[u4PgIdx & (SS_SEED_NUM-1)]) | RAN_CNFG_ENCODE_EN;
			#endif

               // MSG(ERR," qq   ,randmoizer,row_addr=%d, seed=%d, data=%d \n ",  u4OrgPgidx,  (u4PgIdx & (SS_SEED_NUM-1)), (SS_RANDOM_SEED[u4PgIdx & (SS_SEED_NUM-1)]));

		}
		else //toshiba randomizer
		{
			#ifdef TSB_RANDOM_EN
			DRV_WriteReg32(NFI_RANDOM_CNFG, 0); //clear SS seed to avoid S/TSB change side effect
			
			u4NFI_CFG &= ~CNFG_HW_RAN_SEL;
			u4NFI_RAN_CFG = RAN_CNFG_ENCODE_EN;
			
			if(fgPage) //reload seed for each page
			{
				DRV_WriteReg32(NFI_RANDOM_ENSEED01_TS, (TSB_RANDOM_SEED0[u4PgIdx % 16] & 0x1FFF) | ((TSB_RANDOM_SEED1[u4PgIdx % 16] & 0x1FFFF) << 15));
				DRV_WriteReg32(NFI_RANDOM_ENSEED02_TS, TSB_RANDOM_SEED2[u4PgIdx % 16] & 0x7FFFF);
				DRV_WriteReg32(NFI_RANDOM_ENSEED03_TS, TSB_RANDOM_SEED3[u4PgIdx % 16] & 0x7FFFFF);
				DRV_WriteReg32(NFI_RANDOM_ENSEED04_TS, (TSB_RANDOM_SEED0[(u4PgIdx + 1)% 16] & 0x1FFF) | ((TSB_RANDOM_SEED1[(u4PgIdx + 1) % 16] & 0x1FFFF) << 15));
				DRV_WriteReg32(NFI_RANDOM_ENSEED05_TS, TSB_RANDOM_SEED2[(u4PgIdx + 1) % 16] & 0x7FFFF);
				DRV_WriteReg32(NFI_RANDOM_ENSEED06_TS, TSB_RANDOM_SEED3[(u4PgIdx + 1) % 16] & 0x7FFFFF);
			}
			else //reload seed for each sector
			{
				DRV_WriteReg32(NFI_RANDOM_ENSEED01_TS, (TSB_RANDOM_SEED0[(u4PgIdx + u4SectorIdx) % 16] & 0x1FFF) | ((TSB_RANDOM_SEED1[(u4PgIdx + u4SectorIdx) % 16] & 0x1FFFF) << 15));
				DRV_WriteReg32(NFI_RANDOM_ENSEED02_TS, TSB_RANDOM_SEED2[(u4PgIdx + u4SectorIdx) % 16] & 0x7FFFF);
				DRV_WriteReg32(NFI_RANDOM_ENSEED03_TS, TSB_RANDOM_SEED3[(u4PgIdx + u4SectorIdx) % 16] & 0x7FFFFF);
				DRV_WriteReg32(NFI_RANDOM_ENSEED04_TS, (TSB_RANDOM_SEED0[((u4PgIdx + u4SectorIdx) + 1)% 16] & 0x1FFF) | ((TSB_RANDOM_SEED1[((u4PgIdx + u4SectorIdx) + 1) % 16] & 0x1FFFF) << 15));
				DRV_WriteReg32(NFI_RANDOM_ENSEED05_TS, TSB_RANDOM_SEED2[((u4PgIdx + u4SectorIdx) + 1) % 16] & 0x7FFFF);
				DRV_WriteReg32(NFI_RANDOM_ENSEED06_TS, TSB_RANDOM_SEED3[((u4PgIdx + u4SectorIdx) + 1) % 16] & 0x7FFFFF);
			}
			#endif
			
		}

		if(fgPage) //reload seed for each page
			u4NFI_CFG &= ~CNFG_RAN_SEC_EN;
		else //reload seed for each sector
			u4NFI_CFG |= CNFG_RAN_SEC_EN;

		DRV_WriteReg32(NFI_CNFG, u4NFI_CFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4NFI_RAN_CFG);
	}
	else
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, 0);
	}
}

void NFI_RANDOMIZER_DECODE(U32 u4PgIdx, bool fgRandom, bool fgSS, bool fgPage, U32 u4SectorIdx)
{
	U32 u4NFI_CFG = 0;
	U32 u4NFI_RAN_CFG = 0;
	U32 u4PgIdPre = 1;
	U32 u4OrgPgidx= u4PgIdx;
	u4NFI_CFG = DRV_Reg32(NFI_CNFG);
#if defined(__NFI_SUPPORT_TLC__)    
if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
{
        u4PgIdPre = 3;
        if(    (g_pNandInfo->m_dev->m_tlc_setting->normaltlc) 
            &&(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) )
        {
             if( (0xFFFFFFFF != _u4UBISlcPgS) && (_fgUbiPart && _u4UBISlcPgS < _u4LogicPdIdx && _u4LogicPdIdx <_u4UBISlcPgE))
                 u4PgIdx =  _u4LogicPdIdx - _u4UBISlcPgS - 1;
             else
                 u4PgIdx = u4PgIdx*3;
        }
//MSG(ERR," qq   ,randmoizer deocode,row_addr=%d, seed=%d, data=%d, 0x%x  ,0x%x ,0x%x \n ",  
//	u4OrgPgidx,  (u4PgIdx & (SS_SEED_NUM-1)), (SS_RANDOM_SEED[u4PgIdx & (SS_SEED_NUM-1)])
//	,_u4UBISlcPgS, _u4LogicPdIdx, _u4UBISlcPgE);

        u4PgIdx = u4PgIdx % (g_pNandInfo->m_dev->m_hw_info.m_pages_per_block * 3);  //to fit flash which page per block is not 32 aligned
}
#endif

	if(fgRandom)
	{
		DRV_WriteReg32(NFI_EMPTY_THRESH, 40); // empty threshold 40
		if(fgSS) //samsung randomizer
		{
			#ifdef SS_RANDOM_EN
			DRV_WriteReg32(NFI_RANDOM_DESEED01_TS, 0); //clear TSB seed to avoid S/TSB change side effect
			DRV_WriteReg32(NFI_RANDOM_DESEED02_TS, 0);
			DRV_WriteReg32(NFI_RANDOM_DESEED03_TS, 0);
			DRV_WriteReg32(NFI_RANDOM_DESEED04_TS, 0);
			DRV_WriteReg32(NFI_RANDOM_DESEED05_TS, 0);
			DRV_WriteReg32(NFI_RANDOM_DESEED06_TS, 0);
			
			u4NFI_CFG |= CNFG_HW_RAN_SEL;
			if((g_pNandInfo->m_dev->m_hw_info.m_pages_per_block * u4PgIdPre) <= SS_SEED_NUM)
				u4NFI_RAN_CFG = RAN_CNFG_DECODE_SEED(SS_RANDOM_SEED[u4PgIdx % ((g_pNandInfo->m_dev->m_hw_info.m_pages_per_block)-1)]) | RAN_CNFG_DECODE_EN;
			else
				u4NFI_RAN_CFG = RAN_CNFG_DECODE_SEED(SS_RANDOM_SEED[u4PgIdx & (SS_SEED_NUM-1)]) | RAN_CNFG_DECODE_EN;
			#endif
		}
		else //toshiba randomizer
		{
			#ifdef TSB_RANDOM_EN
			DRV_WriteReg32(NFI_RANDOM_CNFG, 0); //clear SS seed to avoid S/TSB change side effect
			
			u4NFI_CFG &= ~CNFG_HW_RAN_SEL;
			u4NFI_RAN_CFG = RAN_CNFG_DECODE_EN;
			
			if(fgPage) //reload seed for each page
			{
				DRV_WriteReg32(NFI_RANDOM_DESEED01_TS, (TSB_RANDOM_SEED0[u4PgIdx % 16] & 0x1FFF) | ((TSB_RANDOM_SEED1[u4PgIdx % 16] & 0x1FFFF) << 15));
				DRV_WriteReg32(NFI_RANDOM_DESEED02_TS, TSB_RANDOM_SEED2[u4PgIdx % 16] & 0x7FFFF);
				DRV_WriteReg32(NFI_RANDOM_DESEED03_TS, TSB_RANDOM_SEED3[u4PgIdx % 16] & 0x7FFFFF);
				DRV_WriteReg32(NFI_RANDOM_DESEED04_TS, (TSB_RANDOM_SEED0[(u4PgIdx + 1)% 16] & 0x1FFF) | ((TSB_RANDOM_SEED1[(u4PgIdx + 1) % 16] & 0x1FFFF) << 15));
				DRV_WriteReg32(NFI_RANDOM_DESEED05_TS, TSB_RANDOM_SEED2[(u4PgIdx + 1) % 16] & 0x7FFFF);
				DRV_WriteReg32(NFI_RANDOM_DESEED06_TS, TSB_RANDOM_SEED3[(u4PgIdx + 1) % 16] & 0x7FFFFF);
			}
			else //reload seed for each sector
			{
				DRV_WriteReg32(NFI_RANDOM_DESEED01_TS, (TSB_RANDOM_SEED0[(u4PgIdx + u4SectorIdx) % 16] & 0x1FFF) | ((TSB_RANDOM_SEED1[(u4PgIdx + u4SectorIdx) % 16] & 0x1FFFF) << 15));
				DRV_WriteReg32(NFI_RANDOM_DESEED02_TS, TSB_RANDOM_SEED2[(u4PgIdx + u4SectorIdx) % 16] & 0x7FFFF);
				DRV_WriteReg32(NFI_RANDOM_DESEED03_TS, TSB_RANDOM_SEED3[(u4PgIdx + u4SectorIdx) % 16] & 0x7FFFFF);
				DRV_WriteReg32(NFI_RANDOM_DESEED04_TS, (TSB_RANDOM_SEED0[((u4PgIdx + u4SectorIdx) + 1)% 16] & 0x1FFF) | ((TSB_RANDOM_SEED1[((u4PgIdx + u4SectorIdx) + 1) % 16] & 0x1FFFF) << 15));
				DRV_WriteReg32(NFI_RANDOM_DESEED05_TS, TSB_RANDOM_SEED2[((u4PgIdx + u4SectorIdx) + 1) % 16] & 0x7FFFF);
				DRV_WriteReg32(NFI_RANDOM_DESEED06_TS, TSB_RANDOM_SEED3[((u4PgIdx + u4SectorIdx) + 1) % 16] & 0x7FFFFF);
			}
			#endif
		}

		if(fgPage) //reload seed for each page
			u4NFI_CFG &= ~CNFG_RAN_SEC_EN;
		else //reload seed for each sector
			u4NFI_CFG |= CNFG_RAN_SEC_EN;

		DRV_WriteReg32(NFI_CNFG, u4NFI_CFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4NFI_RAN_CFG);
	}
	else
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, 0);
	}
}

#endif

#if 1 //hynix read retry table read
#define HYNIX_RR_TABLE_SIZE (1026+6)

typedef struct{
	U8 rr_table[64];
	U8 inversed_rr_table[64];
}SINGLE_RR_TABLE;

typedef struct{
	U8 total_rr_conut;
	U8 reserved[3];
	U8 total_rr_reg_count;
	U8 reserved1[3];
	SINGLE_RR_TABLE rr_table[8];
}HYNIX_RR_TABLE;

HYNIX_RR_TABLE hynix_rr_table;
U8 real_hynix_rr_table_idx = 0;

static bool hynix_rr_table_select(U8 table_index)
{
	uint32 i;
	
	for(i = 0; i < 64; i++)
	{
		if(0xFF != (hynix_rr_table.rr_table[table_index].rr_table[i] ^ hynix_rr_table.rr_table[table_index].inversed_rr_table[i]))
			return FALSE; // error table
	}

	return TRUE; // correct table
}

void HYNIX_RR_TABLE_READ(void)
{
	uint32           reg_val     	 = 0;
	uint32            read_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;
	uint32 u4RandomSetting;
	U8*  rr_table = (U8*)(&hynix_rr_table);
	U8 table_index = 0;

	NAND_COMMON_Reset_V3(timeout);  // take care under sync mode. need change nand device inferface xiaolei
		
	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

	DRV_WriteReg(NFI_CMD,0x36);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
      DRV_WriteReg32(NFI_COLADDR, 0xAE);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	*NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, 0x00);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0xB0);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, 0x4D);

	NFI_Reset();
	
	DRV_WriteReg(NFI_CMD,0x16);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg(NFI_CMD,0x17);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg(NFI_CMD,0x04);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	  
	DRV_WriteReg(NFI_CMD,0x19);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);  

	DRV_WriteReg(NFI_CMD,0x00);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x02);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg(NFI_CMD,0x30);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	*NFI_CNRNB = NFI_PSTA_TIMEOUT << 4;
	*NFI_CNRNB |=1;
	timeout=NFI_PSTA_TIMEOUT;
	NFI_Wait_Return_Ready(timeout);

    reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW | CNFG_READ_EN);
    DRV_WriteReg(NFI_CNFG, reg_val);	  	
    DRV_WriteReg(NFI_CON, (CON_NFI_BRD | (2<< CON_NFI_SEC_SHIFT)));
    DRV_WriteReg(NFI_STRDATA, 0x1);
	timeout=NFI_PSTA_TIMEOUT;
	while ( (read_count < HYNIX_RR_TABLE_SIZE) && timeout )
    {
    	if(((read_count > 0) && (read_count < 4)) || ((read_count > 4) && (read_count < 8)))
    	{
    		*rr_table++ = 0;//align
    	}
	else
	{
		WAIT_NFI_PIO_READY(timeout);        
		*rr_table++ = (U8)DRV_Reg32(NFI_DATAR);
	}
    	 read_count++;
        timeout = NFI_PSTA_TIMEOUT;
    }
	DBG_MSG("\n");
	timeout=NFI_PSTA_TIMEOUT;
	NAND_COMMON_Reset_V3(timeout);
	timeout=NFI_PSTA_TIMEOUT;
	NFI_Wait_Return_Ready(timeout);

	reg_val |= (CNFG_OP_CUST | CNFG_BYTE_RW | CNFG_READ_EN);
	DRV_WriteReg(NFI_CNFG, reg_val);
	DRV_WriteReg(NFI_CMD,0x38);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	timeout=NFI_PSTA_TIMEOUT;
	  NFI_Wait_Return_Ready(timeout);

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}

	
	rr_table = (U8*)(&hynix_rr_table);
	for(read_count=0;read_count<HYNIX_RR_TABLE_SIZE;read_count++)
	{
		DBG_MSG("0x%x ",*rr_table++);
		if((read_count+1)%10 == 0)
			DBG_MSG("\n");
	}
	DBG_MSG("\n");
	
	if((hynix_rr_table.total_rr_conut != 8) || (hynix_rr_table.total_rr_reg_count != 8))
	{
		DBG_MSG("Warning! Hynix RR table error! rr_count = %d, rr_reg_count = %d\n", hynix_rr_table.total_rr_conut, hynix_rr_table.total_rr_reg_count);
	}
	else
	{
		for(table_index = 0 ;table_index < 8; table_index++)
		{
			if(hynix_rr_table_select(table_index))
			{
				real_hynix_rr_table_idx = table_index;
				break;
			}
		}
		if(table_index == 8)
			DBG_MSG("Warning! Hynix RR table index error!\n");
	}
}

void HYNIX_Set_RR_Para(uint32 rr_index)
{
	uint32           reg_val     	 = 0;
	uint32            read_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;
	uint32 u4RandomSetting;
		
	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

	DRV_WriteReg(NFI_CMD,0x36);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
      DRV_WriteReg32(NFI_COLADDR, 0xB0);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	*NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, hynix_rr_table.rr_table[real_hynix_rr_table_idx].rr_table[rr_index*8]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0xB1);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, hynix_rr_table.rr_table[real_hynix_rr_table_idx].rr_table[rr_index*8 + 1]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0xB2);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW,  hynix_rr_table.rr_table[real_hynix_rr_table_idx].rr_table[rr_index*8 + 2]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0xB3);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW,  hynix_rr_table.rr_table[real_hynix_rr_table_idx].rr_table[rr_index*8 + 3]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0xB4);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW,  hynix_rr_table.rr_table[real_hynix_rr_table_idx].rr_table[rr_index*8 + 4]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0xB5);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW,  hynix_rr_table.rr_table[real_hynix_rr_table_idx].rr_table[rr_index*8 + 5]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0xB6);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW,  hynix_rr_table.rr_table[real_hynix_rr_table_idx].rr_table[rr_index*8 + 6]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0xB7);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW,  hynix_rr_table.rr_table[real_hynix_rr_table_idx].rr_table[rr_index*8 + 7]);

	NFI_Reset();
	DRV_WriteReg(NFI_CMD,0x16);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}
}
	
#endif

bool inverse_table = FALSE;

#if 1 //hynix 16nm read retry table read
#define HYNIX_16NM_RR_TABLE_SIZE 528

typedef struct{
	U8 rr_table[32];
	U8 inversed_rr_table[32];
}SINGLE_RR_TABLE_16NM;

typedef struct{
	U8 total_rr_conut[8];
	U8 total_rr_reg_count[8];
	SINGLE_RR_TABLE_16NM rr_table[8];
}HYNIX_RR_TABLE_16NM;

HYNIX_RR_TABLE_16NM hynix_rr_table_16nm;
U8 real_hynix_rr_table_16nm[32];

static bool hynix_rr_table_select_16nm(void)
{
	uint32 i, j, k;
	uint32 zero_num = 0;
	uint32 one_num = 0;

	inverse_table = FALSE;
	for(j = 0; j < 32; j++) //byte of each set
	{
		real_hynix_rr_table_16nm[j] = 0; //initial
		
		for(k = 0; k < 8; k++) // bit of each byte
		{
			zero_num = 0;
			one_num = 0;
			
			for(i = 0; i < 8; i++) // set number
			{
				if((hynix_rr_table_16nm.rr_table[i].rr_table[j] >> k) & 0x1)
					one_num ++;
				else
					zero_num ++;
			}
			if(one_num == zero_num)
			{
				zero_num = 0;
				one_num = 0;
				for(i = 0; i < 8; i++) // set number
				{
					if((hynix_rr_table_16nm.rr_table[i].inversed_rr_table[j] >> k) & 0x1)
						one_num ++;
					else
						zero_num ++;
				}
				if(one_num == zero_num) //no valuable table existed
				{
					DBG_MSG("Byte %d wrong\n", j);
					return FALSE;
				}
				else // repeated time > 4
				{
					inverse_table = TRUE;
					real_hynix_rr_table_16nm[j] |= (((zero_num > one_num) ? 1 : 0) << k);
				}
			}
			else // repeated time > 4
			{
				real_hynix_rr_table_16nm[j] |= (((zero_num > one_num) ? 0 : 1) << k);
			}
		}
	}

	return TRUE; // correct table
}

void HYNIX_16NM_RR_TABLE_READ(void)
{
	uint32           reg_val     	 = 0;
	uint32            read_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;
	uint32 u4RandomSetting;
	U8*  rr_table = (U8*)(&hynix_rr_table_16nm);
	U8 table_index = 0;
	bool RR_TABLE_EXIST = TRUE;

	NAND_COMMON_Reset_V3(timeout);  // take care under sync mode. need change nand device inferface xiaolei
		
	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

	DRV_WriteReg(NFI_CMD,0x36);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
      DRV_WriteReg32(NFI_COLADDR, 0x38);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	*NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, 0x52);

	NFI_Reset();
	
	DRV_WriteReg(NFI_CMD,0x16);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg(NFI_CMD,0x17);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg(NFI_CMD,0x04);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	  
	DRV_WriteReg(NFI_CMD,0x19);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);  

	DRV_WriteReg(NFI_CMD,0x00);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x02);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg(NFI_CMD,0x30);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	*NFI_CNRNB = NFI_PSTA_TIMEOUT << 4;
	*NFI_CNRNB |=1;
	timeout=NFI_PSTA_TIMEOUT;
	NFI_Wait_Return_Ready(timeout);

    reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW | CNFG_READ_EN);
    DRV_WriteReg(NFI_CNFG, reg_val);	  	
    DRV_WriteReg(NFI_CON, (CON_NFI_BRD | (2<< CON_NFI_SEC_SHIFT)));
    DRV_WriteReg(NFI_STRDATA, 0x1);
	timeout=NFI_PSTA_TIMEOUT;
	while ( (read_count < HYNIX_16NM_RR_TABLE_SIZE) && timeout )
    {
    	WAIT_NFI_PIO_READY(timeout);        
	*rr_table++ = (U8)DRV_Reg32(NFI_DATAR);
        read_count++;
        timeout = NFI_PSTA_TIMEOUT;
    }
	DBG_MSG("\n");
	timeout=NFI_PSTA_TIMEOUT;
	NAND_COMMON_Reset_V3(timeout);
	timeout=NFI_PSTA_TIMEOUT;
	NFI_Wait_Return_Ready(timeout);

	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);
	DRV_WriteReg(NFI_CMD,0x36);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
      DRV_WriteReg32(NFI_COLADDR, 0x38);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	*NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, 0x00);

	NFI_Reset();
	
	DRV_WriteReg(NFI_CMD,0x16);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg(NFI_CMD,0x00);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x00); //dummy read, add don't care
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	  DRV_WriteReg(NFI_CMD,0x30);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	  
	timeout=NFI_PSTA_TIMEOUT;
	  NFI_Wait_Return_Ready(timeout);

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}

	
	rr_table = (U8*)(&hynix_rr_table_16nm);
	for(read_count=0;read_count<528;read_count++)
	{
		DBG_MSG("0x%x ",*rr_table++);
		if((read_count+1)%8 == 0)
			DBG_MSG("\n");
	}
	DBG_MSG("\n");

	for(read_count = 0; read_count<8; read_count++)
	{
		if((hynix_rr_table_16nm.total_rr_conut[read_count] != 8) || (hynix_rr_table_16nm.total_rr_reg_count[read_count] != 4))
		{
			DBG_MSG("Warning! Hynix 16 NM RR table error! rr_count = %d, rr_reg_count = %d\n", hynix_rr_table_16nm.total_rr_conut[read_count], hynix_rr_table_16nm.total_rr_reg_count[read_count]);
			RR_TABLE_EXIST = FALSE;
			break;
		}
	}
	
	if(RR_TABLE_EXIST)
	{
		#if 1
		if(hynix_rr_table_select_16nm())
		{
			DBG_MSG("Hynix 16nm RR Table Got\n");
			if(inverse_table)
				DBG_MSG("Some Bit Got From Inverse\n");
			DBG_MSG("Hynix 16nm RR Table in Dram: \n");
			for(read_count=0;read_count<32;read_count++)
			{
				DBG_MSG("0x%x ",real_hynix_rr_table_16nm[read_count]);
				if((read_count+1)%4 == 0)
					DBG_MSG("\n");
			}
			DBG_MSG("\n");
		}
		else
		{
			DBG_MSG("Warning! Hynix 16nm RR Table Not Got\n");
		}
		#else
		for(table_index = 0 ;table_index < 8; table_index++)
		{
			if(hynix_rr_table_select_16nm(table_index))
			{
				real_hynix_rr_table_idx_16nm = table_index;
				DBG_MSG("Hynix 16NM RR table index = %d \n",real_hynix_rr_table_idx_16nm);
				break;
			}
		}
		if(table_index == 8)
			DBG_MSG("Warning! Hynix 16NM RR table index error!\n");
		#endif
	}
}

#if 1
void HYNIX_16NM_Set_RR_Para(uint32 rr_index, uint32 def)
{
	uint32           reg_val     	 = 0;
	uint32            read_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;
	uint32 u4RandomSetting, reg_cunt;

    //if(hynix_16nm_rr_cnt == 8)
     //   hynix_16nm_rr_cnt = 0;
    
    //rr_index = hynix_16nm_rr_cnt;

    //DBG_MSG("rr_index:%d def:%d\n", rr_index, def);

    if(def){
        rr_index = 0;
    }
    if(rr_index > 7)
        return;

	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

    //DRV_WriteReg(NFI_CMD,0x36);        
    //while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	NFI_Reset();    

    DRV_WriteReg(NFI_CMD,0x36);        
    while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
    //udelay(1);
    
    for(reg_cunt=0; reg_cunt<4; reg_cunt++){
        //DRV_WriteReg(NFI_CMD,0x36);        
        //while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
        udelay(1);
        reg_val = (0x38+reg_cunt);

        DRV_WriteReg32(NFI_COLADDR, reg_val);        
        DRV_WriteReg32(NFI_ROWADDR, 0);        
        DRV_WriteReg(NFI_ADDRNOB, 0x1);        
        while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
        //udelay(2);
        
        reg_val = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));

        DRV_WriteReg(NFI_CON, reg_val);          
        DRV_WriteReg(NFI_STRDATA, 0x1);

        timeout=NFI_PSTA_TIMEOUT;
    	WAIT_NFI_PIO_READY(timeout);
        if(timeout == 0)
        {  
            DBG_MSG("HYNIX_Get_RR_Para timeout\n");
        }
	 reg_val = real_hynix_rr_table_16nm[rr_index*4+reg_cunt];
        //reg_val = hynix_rr_table_16nm.rr_table[real_hynix_rr_table_idx_16nm].rr_table[rr_index*4+reg_cunt];
        udelay(1);

        DRV_WriteReg32(NFI_DATAW, reg_val);
        
        //DBG_MSG("W[%x]0x%x ",(0x38+reg_cunt),reg_val);

    	//NFI_Reset();
    }
    
    NFI_Reset();
	DRV_WriteReg(NFI_CMD,0x16);
    while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
    udelay(10);
#if 1
    if(def){
        DRV_WriteReg(NFI_CMD,0x0);
        while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
        
        //dummy address 00
        DRV_WriteReg32(NFI_COLADDR, 0);        
        DRV_WriteReg32(NFI_ROWADDR, 254);        
        DRV_WriteReg(NFI_ADDRNOB, ((3<<ADDR_ROW_NOB_SHIFT) | (2<<ADDR_COL_NOB_SHIFT)));        
        while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);        

        DRV_WriteReg(NFI_CMD,0x30);
        while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
        
        // 5. Wait for Read Buffer is ready
        // wait til device is from busy to ready
        timeout = 0xffffff;
        udelay(20);

        NAND_COMMON_Reset_V3(timeout);
        //NFI_Wait_Return_Ready(timeout);
        //if(!timeout){
        //    DBG_MSG("HYNIX_set_RR_Para for RB timeout\n");
        //}  
    }
 #endif   
    //DBG_MSG("\n");

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	} 
    
}
#else
void HYNIX_16NM_Set_RR_Para(uint32 rr_index)
{
	uint32           reg_val     	 = 0;
	uint32            read_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;
	uint32 u4RandomSetting;
		
	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

	DRV_WriteReg(NFI_CMD,0x36);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
      DRV_WriteReg32(NFI_COLADDR, 0x38);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	*NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, hynix_rr_table_16nm.rr_table[real_hynix_rr_table_idx_16nm].rr_table[rr_index*4]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0x39);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, hynix_rr_table_16nm.rr_table[real_hynix_rr_table_idx_16nm].rr_table[rr_index*4 + 1]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0x3A);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW,  hynix_rr_table_16nm.rr_table[real_hynix_rr_table_idx_16nm].rr_table[rr_index*4 + 2]);

	NFI_Reset();
	
	DRV_WriteReg32(NFI_COLADDR, 0x3B);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

       *NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW,  hynix_rr_table_16nm.rr_table[real_hynix_rr_table_idx_16nm].rr_table[rr_index*4 + 3]);

	NFI_Reset();
	DRV_WriteReg(NFI_CMD,0x16);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	  
	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}
}
#endif
	
#endif

#if 1 //hynix fdie read retry table read
#define HYNIX_FDIE_RR_TABLE_SIZE 784

typedef struct{
	U8 rr_table[48];
	U8 inversed_rr_table[48];
}SINGLE_RR_TABLE_FDIE;

typedef struct{
	U8 total_rr_conut[8];
	U8 total_rr_reg_count[8];
	SINGLE_RR_TABLE_FDIE rr_table[8];
}HYNIX_RR_TABLE_FDIE;

HYNIX_RR_TABLE_FDIE hynix_rr_table_fdie;
U8 real_hynix_rr_table_fdie[48];

static bool hynix_rr_table_select_fdie(void)
{
	uint32 i, j, k;
	uint32 zero_num = 0;
	uint32 one_num = 0;
	
	inverse_table = FALSE;
	for(j = 0; j < 48; j++) //byte of each set
	{
		real_hynix_rr_table_fdie[j] = 0; //initial
		
		for(k = 0; k < 8; k++) // bit of each byte
		{
			zero_num = 0;
			one_num = 0;
			
			for(i = 0; i < 8; i++) // set number
			{
				if((hynix_rr_table_fdie.rr_table[i].rr_table[j] >> k) & 0x01)
					one_num ++;
				else
					zero_num ++;
			}
			if(one_num == zero_num)
			{
				zero_num = 0;
				one_num = 0;
				for(i = 0; i < 8; i++) // set number
				{
					if((hynix_rr_table_fdie.rr_table[i].inversed_rr_table[j] >> k) & 0x01)
						one_num ++;
					else
						zero_num ++;
				}
				if(one_num == zero_num) //no valuable table existed
				{
					DBG_MSG("Byte %d wrong\n", j);
					return FALSE;
				}
				else // repeated time > 4
				{
					inverse_table = TRUE;
					real_hynix_rr_table_fdie[j] |= (((zero_num > one_num) ? 0x01 : 0x00) << k);
				}
			}
			else // repeated time > 4
			{
				real_hynix_rr_table_fdie[j] |= (((zero_num > one_num) ? 0x00 : 0x01) << k);
			}
		}
	}

	return TRUE; // correct table
}

void HYNIX_FDIE_RR_TABLE_READ(void)
{
	uint32           reg_val     	 = 0;
	uint32            read_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;
	uint32 u4RandomSetting;
	U8*  rr_table = (U8*)(&hynix_rr_table_fdie);
	U8 table_index = 0;
	bool RR_TABLE_EXIST = TRUE;

	NAND_COMMON_Reset_V3(timeout);  // take care under sync mode. need change nand device inferface xiaolei
		
	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

	DRV_WriteReg(NFI_CMD,0x36);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
      DRV_WriteReg32(NFI_COLADDR, 0x0E);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	*NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, 0x52);

	NFI_Reset();
	
	DRV_WriteReg(NFI_CMD,0x16);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg(NFI_CMD,0x17);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg(NFI_CMD,0x04);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	  
	DRV_WriteReg(NFI_CMD,0x19);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);  

	DRV_WriteReg(NFI_CMD,0x00);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x1F);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x02);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x00);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	DRV_WriteReg(NFI_CMD,0x30);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	*NFI_CNRNB = NFI_PSTA_TIMEOUT << 4;
	*NFI_CNRNB |=1;
	timeout=NFI_PSTA_TIMEOUT;
	NFI_Wait_Return_Ready(timeout);

    reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW | CNFG_READ_EN);
    DRV_WriteReg(NFI_CNFG, reg_val);	  	
    DRV_WriteReg(NFI_CON, (CON_NFI_BRD | (2<< CON_NFI_SEC_SHIFT)));
    DRV_WriteReg(NFI_STRDATA, 0x1);
	timeout=NFI_PSTA_TIMEOUT;
	while ( (read_count < HYNIX_FDIE_RR_TABLE_SIZE) && timeout )
    {
    	WAIT_NFI_PIO_READY(timeout);        
	*rr_table++ = (U8)DRV_Reg32(NFI_DATAR);
        read_count++;
        timeout = NFI_PSTA_TIMEOUT;
    }
	DBG_MSG("\n");
	timeout=NFI_PSTA_TIMEOUT;
	NAND_COMMON_Reset_V3(timeout);
	timeout=NFI_PSTA_TIMEOUT;
	NFI_Wait_Return_Ready(timeout);

	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);
	DRV_WriteReg(NFI_CMD,0x36);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
      DRV_WriteReg32(NFI_COLADDR, 0x0E);
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	*NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	timeout=NFI_PSTA_TIMEOUT;
	WAIT_NFI_PIO_READY(timeout);
        DRV_WriteReg32(NFI_DATAW, 0x00);

	NFI_Reset();
	
	DRV_WriteReg(NFI_CMD,0x16);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg(NFI_CMD,0x00);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg32(NFI_COLADDR, 0x00); //dummy read, add don't care
      DRV_WriteReg32(NFI_ROWADDR, 0);
      DRV_WriteReg(NFI_ADDRNOB, 0x1);
      while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

	  DRV_WriteReg(NFI_CMD,0x30);
      while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	  
	timeout=NFI_PSTA_TIMEOUT;
	  NFI_Wait_Return_Ready(timeout);

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}

	
	rr_table = (U8*)(&hynix_rr_table_fdie);
	for(read_count=0;read_count<HYNIX_FDIE_RR_TABLE_SIZE;read_count++)
	{
		DBG_MSG("0x%x ",*rr_table++);
		if((read_count+1)%8 == 0)
			DBG_MSG("\n");
	}
	DBG_MSG("\n");

	for(read_count = 0; read_count<8; read_count++)
	{
		if((hynix_rr_table_fdie.total_rr_conut[read_count] != 12) || (hynix_rr_table_fdie.total_rr_reg_count[read_count] != 4))
		{
			DBG_MSG("Warning! Hynix FDIE RR table error! rr_count = %d, rr_reg_count = %d\n", hynix_rr_table_fdie.total_rr_conut[read_count], hynix_rr_table_fdie.total_rr_reg_count[read_count]);
			RR_TABLE_EXIST = FALSE;
			break;
		}
	}
	
	if(RR_TABLE_EXIST)
	{
		#if 1
		if(hynix_rr_table_select_fdie())
		{
			DBG_MSG("Hynix FDIE RR table Got\n");
			if(inverse_table)
				DBG_MSG("Some Got From Inverse\n");
			
			DBG_MSG("Hynix FDIE RR Table In DRAM: \n");
			rr_table = (U8*)(&real_hynix_rr_table_fdie);
			for(read_count=0;read_count<48;read_count++)
			{
				DBG_MSG("0x%x ",*rr_table++);
				if((read_count+1)%4 == 0)
					DBG_MSG("\n");
			}
			DBG_MSG("\n");
		}
		else
		{
			DBG_MSG("Warning! Hynix FDIE RR table index error!\n");
		}
		#else
		for(table_index = 0 ;table_index < 8; table_index++)
		{
			if(hynix_rr_table_select_fdie(table_index))
			{
				real_hynix_rr_table_idx_fdie= table_index;
				DBG_MSG("Hynix FDIE RR table index = %d \n",real_hynix_rr_table_idx_fdie);
				break;
			}
		}
		if(table_index == 8)
			DBG_MSG("Warning! Hynix FDIE RR table index error!\n");
		#endif
	}
}

void HYNIX_FDIE_Set_RR_Para(uint32 rr_index, uint32 def)
{
	uint32           reg_val     	 = 0;
	uint32            read_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;
	uint32 u4RandomSetting, reg_cunt;

    if(def){
        rr_index = 0;
    }
    if(rr_index > 11)
        return;

	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

    //DRV_WriteReg(NFI_CMD,0x36);        
    //while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	NFI_Reset();    

    DRV_WriteReg(NFI_CMD,0x36);        
    while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
    //udelay(1);
    
    for(reg_cunt=0; reg_cunt<4; reg_cunt++){
        //DRV_WriteReg(NFI_CMD,0x36);        
        //while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
        udelay(1);
        reg_val = (0x0E + reg_cunt);

        DRV_WriteReg32(NFI_COLADDR, reg_val);        
        DRV_WriteReg32(NFI_ROWADDR, 0);        
        DRV_WriteReg(NFI_ADDRNOB, 0x1);        
        while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
        //udelay(2);
        
        reg_val = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));

        DRV_WriteReg(NFI_CON, reg_val);          
        DRV_WriteReg(NFI_STRDATA, 0x1);

        timeout=NFI_PSTA_TIMEOUT;
    	WAIT_NFI_PIO_READY(timeout);
        if(timeout == 0)
        {  
            DBG_MSG("HYNIX_Get_RR_Para timeout\n");
        }
        reg_val = real_hynix_rr_table_fdie[rr_index*4+reg_cunt];
	 //reg_val = hynix_rr_table_fdie.rr_table[real_hynix_rr_table_idx_fdie].rr_table[rr_index*4+reg_cunt];
        udelay(1);

        DRV_WriteReg32(NFI_DATAW, reg_val);
        
        //DBG_MSG("W[%x]0x%x ",(0x38+reg_cunt),reg_val);

    	//NFI_Reset();
    }
    
    NFI_Reset();
	DRV_WriteReg(NFI_CMD,0x16);
    while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
    udelay(10);
#if 1
    if(def){
        DRV_WriteReg(NFI_CMD,0x0);
        while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
        
        //dummy address 00
        DRV_WriteReg32(NFI_COLADDR, 0);        
        DRV_WriteReg32(NFI_ROWADDR, 254);        
        DRV_WriteReg(NFI_ADDRNOB, ((3<<ADDR_ROW_NOB_SHIFT) | (2<<ADDR_COL_NOB_SHIFT)));        
        while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);        

        DRV_WriteReg(NFI_CMD,0x30);
        while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
        
        // 5. Wait for Read Buffer is ready
        // wait til device is from busy to ready
        timeout = 0xffffff;
        udelay(20);

        NAND_COMMON_Reset_V3(timeout);
        //NFI_Wait_Return_Ready(timeout);
        //if(!timeout){
        //    DBG_MSG("HYNIX_set_RR_Para for RB timeout\n");
        //}  
    }
 #endif   
    //DBG_MSG("\n");

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	} 
    
}
	
#endif


//#ifdef TSB_RR_SUPPORT
#if 1
const U8 addr_tbl[8][5] =
{
	{0x04, 0x05, 0x06, 0x07, 0x0D},
	{0x04, 0x05, 0x06, 0x07, 0x0D},
	{0x04, 0x05, 0x06, 0x07, 0x0D},
	{0x04, 0x05, 0x06, 0x07, 0x0D},
	{0x04, 0x05, 0x06, 0x07, 0x0D},
	{0x04, 0x05, 0x06, 0x07, 0x0D},
	{0x04, 0x05, 0x06, 0x07, 0x0D},
	{0x04, 0x05, 0x06, 0x07, 0x0D}
};

const U8 data_tbl[8][5] =
{
	{0x04, 0x04, 0x7C, 0x7E, 0x00},
	{0x00, 0x7C, 0x78, 0x78, 0x00},
	{0x7C, 0x76, 0x74, 0x72, 0x00},
	{0x08, 0x08, 0x00, 0x00, 0x00},
	{0x0B, 0x7E, 0x76, 0x74, 0x00},
	{0x10, 0x76, 0x72, 0x70, 0x00},
	{0x02, 0x7C, 0x7E, 0x70, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00}
};	

const U8 data_tbl_15nm[11][5] =
{
	{0x00, 0x00, 0x00, 0x00, 0x00},	
	{0x02, 0x04, 0x02, 0x00, 0x00},	
	{0x7C, 0x00, 0x7C, 0x7C, 0x00},	
	{0x7A, 0x00, 0x7A, 0x7A, 0x00},	
	{0x78, 0x02, 0x78, 0x7A, 0x00},	
	{0x7E, 0x04, 0x7E, 0x7A, 0x00},	
	{0x76, 0x04, 0x76, 0x78, 0x00},	
	{0x04, 0x04, 0x04, 0x76, 0x00},	
	{0x06, 0x0A, 0x06, 0x02, 0x00},	
	{0x74, 0x7C, 0x74, 0x76, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00}	
};	

static void mtk_nand_set_mode(U32 opmode)
{
	U32 reg_val;

	reg_val = DRV_Reg(NFI_CNFG);

	reg_val &= ~CNFG_OP_MODE_MASK;
	
	reg_val |= opmode;
	DRV_WriteReg(NFI_CNFG, reg_val);
}

static void mtk_nand_set_command(U32 cmd)
{
	DRV_WriteReg(NFI_CMD, cmd);
    /* wait for the command status ready */
    while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
}

static bool mtk_nand_status_ready(U32 status)
{
	U32 timeout = 0xFFFF;

	while((DRV_Reg(NFI_STA) & status) != 0)
	{
		timeout--;
		if(0 == timeout)
			return FALSE;
	}
	return TRUE;
}
	
static void mtk_nand_modeentry_rrtry(void)
{
	NFI_Reset();

	mtk_nand_set_mode(CNFG_OP_CUST);
	
	mtk_nand_set_command(0x5C);
	mtk_nand_set_command(0xC5);

	mtk_nand_status_ready(STA_NFI_OP_MASK);
}

static void mtk_nand_rren_rrtry(bool needB3)
{
	NFI_Reset();

	mtk_nand_set_mode(CNFG_OP_CUST);

	if(needB3)
		mtk_nand_set_command(0xB3);	
	mtk_nand_set_command(0x26);
	mtk_nand_set_command(0x5D);

	mtk_nand_status_ready(STA_NFI_OP_MASK);
}

static void mtk_nand_rren_15nm_rrtry(bool flag)
{
	NFI_Reset();

	mtk_nand_set_mode(CNFG_OP_CUST);

	if(flag)
		mtk_nand_set_command(0xCD);
	else
		mtk_nand_set_command(0x26);

	mtk_nand_set_command(0x5D);

	mtk_nand_status_ready(STA_NFI_OP_MASK);
}

static void mtk_nand_sprmset_rrtry(U32 addr, U32 data) //single parameter setting
{
	U16           reg_val     	 = 0;
	U8            write_count     = 0;
	U32           reg = 0;
	U32           timeout=0xffff;

	NFI_Reset();

	reg_val |= (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

	NFI_ISSUE_COMMAND(0x55, addr, 0, 1, 0);

	mtk_nand_status_ready(STA_NFI_OP_MASK);

	DRV_WriteReg32(NFI_CON, 1 << CON_NFI_SEC_SHIFT);

	reg_val = DRV_Reg(NFI_CON);
	reg_val |= CON_NFI_BWR;
	DRV_WriteReg(NFI_CON, reg_val);
	DRV_WriteReg(NFI_STRDATA, 0x1);
	
	
	WAIT_NFI_PIO_READY(timeout);
    
    DRV_WriteReg8(NFI_DATAW, data);
    
	NFI_Wait_Return_Ready(timeout);
    	
}

static void mtk_nand_parapage_rrtry(U32 retryCount)
{
	U32 acccon;
	U32 u4RandomSetting;
	
	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}

	acccon = DRV_Reg32(NFI_ACCCON);
	DRV_WriteReg32(NFI_ACCCON, 0x31C083F9); //to fit read retry timing
	
	if(0 == retryCount)
		mtk_nand_modeentry_rrtry();

	mtk_nand_sprmset_rrtry(addr_tbl[retryCount][0], data_tbl[retryCount][0]);
	mtk_nand_sprmset_rrtry(addr_tbl[retryCount][1], data_tbl[retryCount][1]);
	mtk_nand_sprmset_rrtry(addr_tbl[retryCount][2], data_tbl[retryCount][2]);
	mtk_nand_sprmset_rrtry(addr_tbl[retryCount][3], data_tbl[retryCount][3]);
	mtk_nand_sprmset_rrtry(addr_tbl[retryCount][4], data_tbl[retryCount][4]);

	if(3 == retryCount)
		mtk_nand_rren_rrtry(TRUE);
	else if(6 > retryCount)
		mtk_nand_rren_rrtry(FALSE);

	if(7 == retryCount) // to exit
	{
		mtk_nand_set_mode(CNFG_OP_RESET);
		NFI_ISSUE_COMMAND (RESET_CMD, 0, 0, 0, 0);
		NFI_Reset();
	}

	DRV_WriteReg32(NFI_ACCCON, acccon);

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}
}

static void mtk_nand_parapage_15nm_rrtry(U32 retryCount, U32 def)
{
	U32 acccon;
	U32 u4RandomSetting;
	
	//DBG_MSG("mtk_nand_parapage_15nm_rrtry  at count:%d def:%d\n", retryCount, def);
	
	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}

	acccon = DRV_Reg32(NFI_ACCCON);
	DRV_WriteReg32(NFI_ACCCON, 0x31C083F9); //to fit read retry timing
	
	if(0 == retryCount)
		mtk_nand_modeentry_rrtry();

	mtk_nand_sprmset_rrtry(0x04, data_tbl_15nm[retryCount][0]);
	mtk_nand_sprmset_rrtry(0x05, data_tbl_15nm[retryCount][1]);
	mtk_nand_sprmset_rrtry(0x06, data_tbl_15nm[retryCount][2]);
	mtk_nand_sprmset_rrtry(0x07, data_tbl_15nm[retryCount][3]);
	mtk_nand_sprmset_rrtry(0x0D, data_tbl_15nm[retryCount][4]);

	if(0 == retryCount)
		mtk_nand_rren_15nm_rrtry(FALSE);

	if(def) // to exit
	{
		mtk_nand_set_mode(CNFG_OP_RESET);
		NFI_ISSUE_COMMAND (RESET_CMD, 0, 0, 0, 0);
		NFI_Reset();
	}
	else
		mtk_nand_rren_15nm_rrtry(TRUE);

	DRV_WriteReg32(NFI_ACCCON, acccon);

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}
}
#endif

#if 1 //sandisk 19nm read retry
uint16 sandisk_19nm_rr_table[17] =
{
	0x0000, 
	0xFF0F, 0xEEFE, 0xDDFD, 0x11EE, //04h[7:4] | 07h[7:4] | 04h[3:0] | 05h[7:4]
	0x22ED, 0x33DF, 0xCDDE, 0x01DD,
	0x0211, 0x1222, 0xBD21, 0xAD32,
	0x9DF0, 0xBCEF, 0xACDC, 0x9CFF
};

static void sandisk_19nm_rr_init(void)
{
	uint32 		  reg_val		 = 0;
	uint32 		   count	  = 0;
	uint32 		  timeout = 0xffff;
	uint32 u4RandomSetting;
	uint32 acccon;

	acccon = DRV_Reg32(NFI_ACCCON);
	DRV_WriteReg32(NFI_ACCCON, 0x31C083F9); //to fit read retry timing
	
	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

	DRV_WriteReg(NFI_CMD,0x3B);
	  while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	DRV_WriteReg(NFI_CMD,0xB9);
	  while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	for(count = 0; count < 9; count++)
	{
		DRV_WriteReg(NFI_CMD,0x53);
		  while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);	  
		  DRV_WriteReg32(NFI_COLADDR, (0x04 + count));
		  DRV_WriteReg32(NFI_ROWADDR, 0);
		  DRV_WriteReg(NFI_ADDRNOB, 0x1);
		  while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
		DRV_WriteReg(NFI_CON, (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT)));
		DRV_WriteReg(NFI_STRDATA, 1);
		WAIT_NFI_PIO_READY(timeout);
			DRV_WriteReg32(NFI_DATAW, 0x00);

		NFI_Reset();
	}

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}
	
	DRV_WriteReg32(NFI_ACCCON, acccon);	
}

static void sandisk_19nm_rr_loading(uint32 retryCount, bool defValue)
{
	uint32 		  reg_val		 = 0;
	uint32 		  timeout = 0xffff;
	uint32 u4RandomSetting;
	uint32 acccon;

	acccon = DRV_Reg32(NFI_ACCCON);
	DRV_WriteReg32(NFI_ACCCON, 0x31C083F9); //to fit read retry timing
	
	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

	if((0 != retryCount) || defValue)
	{
		DRV_WriteReg(NFI_CMD,0xD6); //disable rr
	 	while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	}

	DRV_WriteReg(NFI_CMD,0x3B);
	  while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	DRV_WriteReg(NFI_CMD,0xB9);
	  while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	DRV_WriteReg(NFI_CMD,0x53);
	  while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);	  
	  DRV_WriteReg32(NFI_COLADDR, 0x04);
	  DRV_WriteReg32(NFI_ROWADDR, 0);
	  DRV_WriteReg(NFI_ADDRNOB, 0x1);
	  while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	DRV_WriteReg(NFI_CON, (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT)));
	DRV_WriteReg(NFI_STRDATA, 1);
	WAIT_NFI_PIO_READY(timeout);
		DRV_WriteReg32(NFI_DATAW, (((sandisk_19nm_rr_table[retryCount] & 0xF000) >> 8) | ((sandisk_19nm_rr_table[retryCount] & 0x00F0) >> 4)));

	NFI_Reset();

	DRV_WriteReg(NFI_CMD,0x53);
	  while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);	  
	  DRV_WriteReg32(NFI_COLADDR, 0x05);
	  DRV_WriteReg32(NFI_ROWADDR, 0);
	  DRV_WriteReg(NFI_ADDRNOB, 0x1);
	  while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	DRV_WriteReg(NFI_CON, (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT)));
	DRV_WriteReg(NFI_STRDATA, 1);
	WAIT_NFI_PIO_READY(timeout);
		DRV_WriteReg32(NFI_DATAW, ((sandisk_19nm_rr_table[retryCount] & 0x000F) << 4));

	NFI_Reset();

	DRV_WriteReg(NFI_CMD,0x53);
	  while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);	  
	  DRV_WriteReg32(NFI_COLADDR, 0x07);
	  DRV_WriteReg32(NFI_ROWADDR, 0);
	  DRV_WriteReg(NFI_ADDRNOB, 0x1);
	  while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);
	DRV_WriteReg(NFI_CON, (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT)));
	DRV_WriteReg(NFI_STRDATA, 1);
	WAIT_NFI_PIO_READY(timeout);
		DRV_WriteReg32(NFI_DATAW, ((sandisk_19nm_rr_table[retryCount] & 0x0F00) >> 4));

	if(!defValue)
	{
		DRV_WriteReg(NFI_CMD,0xB6); //enable rr
		while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);	  
	}

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}
	
	DRV_WriteReg32(NFI_ACCCON, acccon);	
}

static void sandisk_19nm_rr(uint32 retrycount, bool defvalue)
{
	if((retrycount == 0) && (!defvalue))
		sandisk_19nm_rr_init();
	sandisk_19nm_rr_loading(retrycount, defvalue);
}
#endif

#if 1
void NAND_COMMON_Set_Feature(uint8 cmd,uint8 addr, uint8 *value,  uint8 bytes)
{
	uint16           reg_val     	 = 0;
	uint8            write_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;
       uint32  u4RandomSetting;

	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val |= (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

	NFI_ISSUE_COMMAND(cmd, addr, 0, 1, 0);

	*NFI_CON = (CON_NFI_BWR | (1 << CON_NFI_SEC_SHIFT));
	*NFI_STRDATA = 1;
	while ( (write_count < bytes) && timeout )
    {
    	WAIT_NFI_PIO_READY(timeout)
        if(timeout == 0)
        {
            break;
        }
        DRV_WriteReg32(NFI_DATAW, *value++);
        write_count++;
        timeout = NFI_PSTA_TIMEOUT;
    }
	*NFI_CNRNB = NFI_PSTA_TIMEOUT << 4;
	*NFI_CNRNB |=1;
	NFI_Wait_Return_Ready(timeout);

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}
	
}

void SAL_NFI_GetFeature(uint8 cmd,uint8 addr, uint8 *value,  uint8 bytes)
{
	uint16           reg_val     	 = 0;
	uint8            read_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;
	uint32 u4RandomSetting;

	NFI_Wait_Return_Ready(timeout);
	
	NFI_Reset();

	if(fgRandomEn)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
		DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
	}
	
	reg_val |= (CNFG_OP_CUST | CNFG_BYTE_RW | CNFG_READ_EN);
	DRV_WriteReg(NFI_CNFG, reg_val);

	NFI_ISSUE_COMMAND(cmd, addr, 0, 1, 0)

       NFI_Wait_Return_Ready(timeout); //bug here if no wait busy   xiaolei
	
	//SAL_NFI_Config_Sector_Number(0);
	//*NFI_CON = (CON_NFI_BWR | (0<< CON_NFI_SEC_SHIFT));
	reg_val = DRV_Reg(NFI_CON);
    reg_val &= ~CON_NFI_NOB_MASK;
    reg_val |= ((4 << CON_NFI_NOB_SHIFT)|CON_NFI_SRD);
    DRV_WriteReg(NFI_CON, reg_val);
	DRV_WriteReg(NFI_STRDATA, 0x1);
//	SAL_NFI_Start_Data_Transfer(KAL_TRUE, KAL_TRUE);

	while ( (read_count < bytes) && timeout )
    {
    	WAIT_NFI_PIO_READY(timeout)
        if(timeout == 0)
        {
            break;
        }
		*value++ = DRV_Reg32(NFI_DATAR);
        read_count++;
        timeout = NFI_PSTA_TIMEOUT;
    }
	*NFI_CNRNB = NFI_PSTA_TIMEOUT << 4;
	*NFI_CNRNB |=1;
	NFI_Wait_Return_Ready(timeout);

	if(fgRandomEn)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	}
}

static void mtk_nand_reset_fdm_ecc(bool nonplpart)
{
	uint32 u4ENCODESize;
	uint32 u4DECODESize;
	uint32 reg_value;

	if(nonplpart)//non preloader partition
	{
		//set ECC_DECCNFG_REG32
		u4DECODESize = ((NUTL_SECTOR_SIZE() + re_fdm_size) << 3) + re_ecc_level * ECC_PARITY_BITS;
		reg_value = *ECC_DECCNFG;
		reg_value &= 0xFFFFFFE0; //clear ENC_TNUM
		reg_value &= 0xC007FFFF; //clear ENC_MS
		reg_value |= re_ecc_bit_cfg;
		reg_value |= (u4DECODESize << DEC_CNFG_CODE_SHIFT);
		*ECC_DECCNFG = reg_value;
		//set ECC_ENCCNFG_REG32
		u4ENCODESize = (NUTL_SECTOR_SIZE() + re_fdm_size) << 3;
		reg_value = *ECC_ENCCNFG;
		reg_value &= 0xFFFFFFE0; //clear DEC_TNUM
		reg_value &= 0xC007FFFF; //clear DEC_CS
		reg_value |= re_ecc_bit_cfg;
		reg_value |= (u4ENCODESize << ENC_CNFG_MSG_SHIFT);
		*ECC_ENCCNFG = reg_value;
		//set NFI_PAGEFMT_REG16
		reg_value = *NFI_PAGEFMT;
		reg_value &= 0xFFFFF0FF; //clear FDM_NUM
		reg_value &= 0xFFFF0FFF; //clear FDM_ECC_NUM
		reg_value |= (re_fdm_size << PAGEFMT_FDM_SHIFT);
		reg_value |= (re_fdm_size << PAGEFMT_FDM_ECC_SHIFT);
		*NFI_PAGEFMT = reg_value;
	}
	else
	{
		//set ECC_DECCNFG_REG32
		u4DECODESize = ((NUTL_SECTOR_SIZE() + 8) << 3) + org_ecc_level * ECC_PARITY_BITS;
		reg_value = *ECC_DECCNFG;
		reg_value &= 0xFFFFFFE0; //clear ENC_TNUM
		reg_value &= 0xC007FFFF; //clear ENC_MS
		reg_value |= org_ecc_bit_cfg;
		reg_value |= (u4DECODESize << DEC_CNFG_CODE_SHIFT);
		*ECC_DECCNFG = reg_value;
		//set ECC_ENCCNFG_REG32
		u4ENCODESize = (NUTL_SECTOR_SIZE() + 8) << 3;
		reg_value = *ECC_ENCCNFG;
		reg_value &= 0xFFFFFFE0; //clear DEC_TNUM
		reg_value &= 0xC007FFFF; //clear DEC_CS
		reg_value |= org_ecc_bit_cfg;
		reg_value |= (u4ENCODESize << ENC_CNFG_MSG_SHIFT);
		*ECC_ENCCNFG = reg_value;
		//set NFI_PAGEFMT_REG16
		reg_value = *NFI_PAGEFMT;
		reg_value &= 0xFFFFF0FF; //clear FDM_NUM
		reg_value &= 0xFFFF0FFF; //clear FDM_ECC_NUM
		reg_value |= (8 << PAGEFMT_FDM_SHIFT);
		reg_value |= (8 << PAGEFMT_FDM_ECC_SHIFT);
		*NFI_PAGEFMT = reg_value;
	}
}

STATUS_E NAND_COMMON_ChangeInterface(uint32 u4NandType, uint32 u4AccCon, uint32 u4AccCon1)
{
	uint32 u4Val = 0;
	uint32 u4reg_val, u4reg_val1, u4reg_val2, u4reg_val3; 
	// reset NFI
	NFI_Reset();
	
	u4Val = u4NandType; //NFI_ASYNC, NFI_ONFI, NFI_TOGGLE

	if((u4NandType == NFI_TOGGLE) || (u4NandType == NFI_ONFI))
	{
		u4reg_val1 = *NFI_DEBUG_CON3;
		u4reg_val2 = *ECC_BYPASS;
		u4reg_val3 = *NFI_1X_INFRA_SEL;

		u4reg_val1 &= ~0x8000;
		*NFI_DEBUG_CON3 = u4reg_val1;

		u4reg_val2 &= ~0x1;
		*ECC_BYPASS = u4reg_val2;

		#if defined(BB_MT8163)
		u4reg_val3 |= (0x1 << 10);
		*NFI_1X_INFRA_SEL = u4reg_val3;
		#else
		u4reg_val3 |= 0x1;
		*NFI_1X_INFRA_SEL = u4reg_val3;
		#endif
		
		u4reg_val = *NFI_CKGEN_SEL;
		*NFI_CKGEN_SEL = u4reg_val & (~(7 << 8));
		u4reg_val = *NFI_CKGEN_SEL;
		*NFI_CKGEN_SEL = u4reg_val | (2 << 8);
		#if defined(BB_MT8163)
		*NFI_DELAY_CTRL = 0x64011;
		#else
		*NFI_DELAY_CTRL = 0x4001;
		*NFI_MAC_CTRL = 0x10006;
		#endif
	}
	else
	{
		u4reg_val1 = *NFI_DEBUG_CON3;
		u4reg_val2 = *ECC_BYPASS;
		u4reg_val3 = *NFI_1X_INFRA_SEL;

		u4reg_val1 |= 0x8000;
		*NFI_DEBUG_CON3 = u4reg_val1;

		u4reg_val2 |= 0x1;
		*ECC_BYPASS = u4reg_val2;

		#if defined(BB_MT8163)
		u4reg_val3 &= (~(0x1 << 10));
		*NFI_1X_INFRA_SEL = u4reg_val3;
		#else
		u4reg_val3 &= ~0x1;
		*NFI_1X_INFRA_SEL = u4reg_val3;
		#endif
	}

	while(0 == (*NFI_STA && STA_FLASH_MACRO_IDLE));

	*NFI_NAND_TYPE_CNFG = u4Val;

	*NFI_ACCCON = u4AccCon;
	*NFI_ACCCON1 = u4AccCon1;

	return S_DONE;
}

#endif

#if 1
STATUS_E NAND_COMMON_Randomizer_Config(bool use)
{
	#if 0
    uint16   nfi_cnfg = 0;
	uint32   nfi_ran_cnfg = 0;
	uint8 i;

    /* set up NFI_CNFG */
    nfi_cnfg = DRV_Reg(NFI_CNFG);
	nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
	if(use)
	{
		nfi_ran_cnfg &= ~SEED_MASK << EN_SEED_SHIFT;
		nfi_ran_cnfg &= ~SEED_MASK << DE_SEED_SHIFT;
		nfi_ran_cnfg |= 0x2D2D << EN_SEED_SHIFT;
		nfi_ran_cnfg |= 0x2D2D << DE_SEED_SHIFT;
		nfi_cnfg &= ~CNFG_RAN_SEC;
		nfi_cnfg &= ~CNFG_RAN_SEL;
		
		nfi_ran_cnfg |= 0x00010001;
	}else
	{
		nfi_ran_cnfg &= ~0x00010001;
		//use_randomizer = FALSE;
		//return 0;
	}
	use_randomizer = use;

       DRV_WriteReg(NFI_CNFG, nfi_cnfg);
	DRV_WriteReg32(NFI_RANDOM_CNFG_REG32, nfi_ran_cnfg);

	#endif
	return S_DONE;
}	
static bool NAND_COMMON_Randomizer_Status()
{
	#if 0
	uint32   nfi_ran_cnfg = 0;
	nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
	if(nfi_ran_cnfg&0x00010001)
		return TRUE;

	return FALSE;
	#endif
}
static void NAND_COMMON_Turn_On_Randomizer()
{
	#if 0
	uint32   nfi_ran_cnfg = 0;
	nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
	nfi_ran_cnfg |= 0x00010001;
	DRV_WriteReg32(NFI_RANDOM_CNFG_REG32, nfi_ran_cnfg);
	#endif
}
static void NAND_COMMON_Turn_Off_Randomizer()
{
	#if 0
	uint32   nfi_ran_cnfg = 0;
	nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
	nfi_ran_cnfg &= ~0x00010001;
	DRV_WriteReg32(NFI_RANDOM_CNFG_REG32, nfi_ran_cnfg);
	#endif
}

#endif
 
#if	CFG_2CS_NAND
//#define CHIP_ADDRESS  (0x100000)
uint32 NAND_COMMON_CS_ON(uint16 cs, uint32 page)
{
	uint32 cs_page;
	cs_page = page / g_nanddie_pages;
	if (cs_page)
	{
		DRV_WriteReg(NFI_CSEL, cs);
		//if (NUTL_CHIP_NUM()	 == 2)
		//	return page;//return (page | CHIP_ADDRESS);
		return (page-g_nanddie_pages);
	}
	DRV_WriteReg(NFI_CSEL, 0);
	return page;
}
#endif
//------------------------------------------------------------------------------
// Read Device ID Callback Function                                             
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_ReadID_V3(
                const uint32  c_timeout
                ,uint32* id1
		   ,uint32* id2 //add by jiequn,2011.11.11
) {
    uint32      timeout = NFI_PSTA_TIMEOUT;
    //uint32      id1 = 0;
    uint32      nfi_pagefmt = 0;
    uint16      reg_val;
    STATUS_E    ret=S_DA_NAND_UNKNOWN_ERR;

    uint32 u4RandomSetting;

    //DRV_WriteReg(NFI_CSEL, 0); // modify here
#if	CFG_2CS_NAND
   if(g_bTryrun_CS)
   {
	DRV_WriteReg(NFI_CSEL, 1);
	//while(1);
   }else
   {
	DRV_WriteReg(NFI_CSEL, 0); // modify here
    }
#endif
/*modify by jiequnchen,2011.11.16*/
  // Modify by Kuohong, use H8CS0SI0MP for Golden timing.
    if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC) 
        DRV_WriteReg32(NFI_ACCCON, 0x10401023);//0x10401011 
    else
        DRV_WriteReg32(NFI_ACCCON, 0x10804222);//0x31C083F9);
    //DRV_WriteReg32(NFI_ACCCON, 0x31C083F9);
    //DRV_WriteReg32(NFI_ACCCON, 0x10818072);//0x31C083F9);    

    reg_val = DRV_Reg(NFI_CNFG);
    reg_val &= ~CNFG_AHB; //It can't be AHB mode for WAIT_NFI_PIO_READY.
    DRV_WriteReg(NFI_CNFG, reg_val);

#if 1
    /* Reset NFI state machine */
    NFI_Reset();

    /* Issue NAND chip reset command for Micron's MCP */
    NFI_ISSUE_COMMAND(RESET_CMD, 0, 0, 0, 0);

    timeout = 0xFFFF;

    while(timeout)
        timeout--; 
#endif    

    // reset NFI
    NFI_Reset();

    if(fgRandomEn)
    {
	  u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
	  DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
    }

    nfi_pagefmt = *NFI_PAGEFMT;
    DRV_WriteReg(NFI_PAGEFMT, 0);
    /* Config single read and read enable bit in NFI_CNFG register */
    DRV_WriteReg(NFI_CNFG, CNFG_OP_SRD|CNFG_READ_EN);

    /* read NAND flash ID */
    DRV_WriteReg(NFI_CMD, RD_ID_CMD);
    /* wait for the command status ready */
    while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

    /* Set addr registers */
    DRV_WriteReg32(NFI_COLADDR, 0);
    DRV_WriteReg32(NFI_ROWADDR, 0);
    DRV_WriteReg(NFI_ADDRNOB, 1);
    /* wait for the address status ready */
    while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE);

    /* Trigger read operations */
    DRV_WriteReg(NFI_CON, CON_NFI_SRD);
    while(DRV_Reg32(NFI_STA) & STA_DATAR_STATE);
    
    /* Check the PIO bit is ready or not */
    timeout = 0xFFFF;
    WAIT_NFI_PIO_READY(timeout);  

    /* If status is ready, read back the NFI_DATAR register */
    *id1 = DRV_Reg32(NFI_DATAR);
    *id2 = DRV_Reg32(NFI_DATAR); //add by jiequn,2011.11.11
    DRV_WriteReg(NFI_CON, 0x0);
#if 0
    /* If status is ready, read back the NFI_DATAR register */
    id1 = DRV_Reg32(NFI_DATAR);
	*p_ext_code3 = DRV_Reg8(NFI_DATAR); //add by jiequn,2011.11.11
    DRV_WriteReg(NFI_CON, 0x0);

    *p_maker_code = (id1&0xFF);
    *p_device_code = ((id1>>8)&0xFF);
    *p_ext_code1 = ((id1>>16)&0xFF);
    *p_ext_code2 = ((id1>>24)&0xFF);
#endif
    ret = S_DONE;

    // restore original page format setting 
    *NFI_PAGEFMT = nfi_pagefmt;

    if(fgRandomEn)
    {
	  DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
    }

	//DRV_WriteReg32(NFI_EMPTY_THRESH, 40); // empty threshold 40

    return ret;
}
//------------------------------------------------------------------------------
// Reset Device Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_Reset_V3(
				const uint32  c_timeout
) {
	uint32		timeout = NFI_PSTA_TIMEOUT;

	// reset NFI
	NFI_Reset();

	// set NFI_CNFG
	*NFI_CNFG = CNFG_OP_RESET;

	// reset cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_reset.m_cmd;
	
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }

    //MT6516 FPGA, 20080418: Ray: todo for clearify NFI empty slot question
	// wait for reset finish 
	timeout = c_timeout;
	NFI_Wait_Ready(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }
	
	return S_DONE;
}

//------------------------------------------------------------------------------
// Read Status Callback Function                                                
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_ReadStatus_V3(
				const uint32  c_timeout
) {
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32		status = 0xFF;
	uint32 u4RandomSetting;

	// reset NFI
	NFI_Reset();

	if(fgRandomEn)
      {
	    u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
	    DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
      }
	
	// set NFI_CNFG
	*NFI_CNFG = BOOTROM_SRD_CNFG;

	// read status cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_status.m_cmd;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){
		if(fgRandomEn)
	      {
		    DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	      }
		return S_DA_TIMEOUT; }


	// set single read by DWORD 
	*NFI_CON = (1 << CON_NFI_NOB_SHIFT)|CON_NFI_SRD;
	// wait til DATA_READ is completely issued 
	timeout = c_timeout;
	WAIT_NFI_PIO_READY(timeout);
	if(timeout){
		// single read doesn't need to polling FIFO 
		status = *NFI_DATAR;
	}
	
	//~  clear NOB
	*NFI_CON = 0;
       // QQ  bug here for TLC 
	// check READY/BUSY status first 
	if( !(STATUS_READY&status) ) {
		if(fgRandomEn)
	      {
		    DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	      }
		return S_DA_IN_PROGRESS;
	}

	#if defined(__NFI_SUPPORT_TLC__)
       if( (g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)&&
            (g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) )
       {
              if( 0x4 & status ) {
                   return S_DA_NAND_UNKNOWN_ERR;
              }
              else
              {
                   return S_DONE;
              }
       }
       #endif
	// flash is ready now, check status code 
	//hynix mlc will return pass when wp is protected. so this judgement is not strict. xiaolei
	if( STATUS_FAIL & status ) {
		if( !(STATUS_WR_ALLOW&status) ) {
			if(fgRandomEn)
		      {
			    DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
		      }
			return S_DA_NAND_BLOCK_IS_LOCKED;
		}
		else {
			if(fgRandomEn)
		      {
			    DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
		      }
			return S_DA_NAND_UNKNOWN_ERR;
		}
	}
	else {
		if(fgRandomEn)
	      {
		    DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	      }
		return S_DONE;
	}
}

STATUS_E  NAND_COMMON_CheckWP_V3(
				const uint32  c_timeout
) {
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32		status = 0xFF;
	uint32 u4RandomSetting;

	// reset NFI
	NFI_Reset();

	if(fgRandomEn)
      {
	    u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG);
	    DRV_WriteReg32(NFI_RANDOM_CNFG, (u4RandomSetting & (~(RAN_CNFG_ENCODE_EN | RAN_CNFG_DECODE_EN))));
      }
	
	// set NFI_CNFG
	*NFI_CNFG = BOOTROM_SRD_CNFG;

	// read status cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_status.m_cmd;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){
		if(fgRandomEn)
	      {
		    DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	      }
		return S_DA_TIMEOUT; }


	// set single read by DWORD 
	*NFI_CON = (1 << CON_NFI_NOB_SHIFT)|CON_NFI_SRD;
	// wait til DATA_READ is completely issued 
	timeout = c_timeout;
	WAIT_NFI_PIO_READY(timeout);
	if(timeout){
		// single read doesn't need to polling FIFO 
		status = *NFI_DATAR;
	}
	
	//~  clear NOB
	*NFI_CON = 0;

	if( !(STATUS_WR_ALLOW&status) )
	{
		if(fgRandomEn)
	      {
		    DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	      }
			return S_DA_NAND_BLOCK_IS_LOCKED;
	}
	else
	{
		if(fgRandomEn)
	      {
		    DRV_WriteReg32(NFI_RANDOM_CNFG, u4RandomSetting);
	      }
		return S_DONE;
	}
}



//------------------------------------------------------------------------------
// Bad Block Symbol Identification Related Callback Function                    
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_BadBlockSymbol_Check_V3(
				const uint32  *p_spare32 /* MUST be 32bits alignment addr */
) {
	uint32	page_size;
	uint32	spare_size;
	uint32	column_addr_bits;
	uint32	addr_cycle;
	uint32	io_interface;
	const uint8		*p_spare8 = (const uint8 *)p_spare32;
	const uint16	*p_spare16 = (const uint16 *)p_spare32;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();

	// check the invalid block status from spare area 
	if( 512 < page_size ) {
		// for 2048 page size flash, the invalid block status is defined by the 1st byte/word in spare area 
		if( NAND_IO_16BITS == io_interface ) {
			if( 0xFFFF != p_spare16[0] ) {
				return S_DA_NAND_BAD_BLOCK;
			}
		}
		else {
			if( 0xFF != p_spare8[0] ) {
				return S_DA_NAND_BAD_BLOCK;
			}
		}
	}
	else {
		// for 512 page size flash 
		if( NAND_IO_16BITS == io_interface ) {
			// for 16 bits I/O, the invalid block status is defined by the 1st word in spare area 
			if( 0xFFFF != p_spare16[0] ) {
				return S_DA_NAND_BAD_BLOCK;
			}
		}
		else {
			// for 8 bits I/O, the invalid block status is defined by the 6th byte in spare area 
			if( 0xFF != p_spare8[5] ) {
				return S_DA_NAND_BAD_BLOCK;
			}
		}
	}

	return S_DONE;
}

STATUS_E  NAND_COMMON_BadBlockSymbol_Set_V3(
				uint32  *p_spare32 /* MUST be 32bits alignment addr */
) {
	uint32	page_size;
	uint32	spare_size;
	uint32	io_interface;
	uint8	*p_spare8 = (uint8 *)p_spare32;
	uint16	*p_spare16 = (uint16 *)p_spare32;
	uint32	i;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	io_interface = NUTL_IO_INTERFACE();

	// reset spare 
	for(i=0; i<(spare_size>>2); i++) {
		p_spare32[i] = 0xAAAAAAAA;
	}

	// check the invalid block status from spare area 
	if( 512 < page_size ) {
		// for 2048 page size flash, the invalid block status is defined by the 1st byte/word in spare area 
		if( NAND_IO_16BITS == io_interface ) {
			p_spare16[0] = 0x0;
		}
		else {
			p_spare8[0] = 0x0;
		}
	}
	else {
		// for 512 page size flash 
		if( NAND_IO_16BITS == io_interface ) {
			// for 16 bits I/O, the invalid block status is defined by the 1st word in spare area 
			p_spare16[0] = 0x0;
		}
		else {
			// for 8 bits I/O, the invalid block status is defined by the 6th byte in spare area 
			p_spare8[5] = 0x0;
		}
	}

	return S_DONE;
}

//------------------------------------------------------------------------------
// TLC support  Function                                                  
//------------------------------------------------------------------------------

#if defined(__NFI_SUPPORT_TLC__)
bool tlc_lg_left_plane = TRUE; //logical left plane of tlc nand.  used to do page program
NFI_TLC_PG_CYCLE tlc_program_cycle;
uint32  ecc_parity_from_reg[8];
bool tlc_cache_program = FALSE; //whether use cache program
bool tlc_snd_phyplane = FALSE; // second plane operation

//tlc releated functions
void NFI_TLC_GetMappedWL(U32 pageidx, NFI_TLC_WL_INFO* WL_Info)
{ //this function is just for normal tlc
	WL_Info->word_line_idx = pageidx / 3;
	WL_Info->wl_pre = (NFI_TLC_WL_PRE)(pageidx % 3);
}

U32 NFI_TLC_GetRowAddr(U32 rowaddr)
{
	U32 real_row;
	U32 temp = 0xFF;
	if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc) // normal tlc
	{
	   // temp = g_pNandInfo->m_dev->m_hw_info.m_pages_per_block / 3;// SLC Mode view,mark it
	   //if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
	    temp = g_pNandInfo->m_dev->m_hw_info.m_pages_per_block ;
	   //else
          //  temp = g_pNandInfo->m_dev->m_hw_info.m_pages_per_block / 3;
	}
	else
	{
	    temp = g_pNandInfo->m_dev->m_hw_info.m_pages_per_block;
	}

	real_row = ((rowaddr / temp) << g_pNandInfo->m_dev->m_tlc_setting->block_bit) | (rowaddr % temp);
	
    return real_row;
}

U32 NFI_TLC_SetpPlaneAddr(U32 rowaddr, bool left_plane)
{ //this function is just for normal tlc
	U32 real_row;

	if(g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
	{
	    #if 0
	    if(left_plane)
		real_row = (rowaddr & (~(1 << 8)));
	    else
		real_row = (rowaddr | (1 << 8));
	    #else
	    if(left_plane)
		real_row = (rowaddr & (~(1 << g_pNandInfo->m_dev->m_tlc_setting->pPlane_bit)));
	    else
		real_row = (rowaddr | (1 << g_pNandInfo->m_dev->m_tlc_setting->pPlane_bit));
	    #endif
	}
	else
		real_row = rowaddr;
	
	return real_row;
}

U32 NFI_TLC_GetMappedPgAddr(U32 rowaddr) //rowaddr is the real address, the return value is the page increased by degree (pageidx = block no * page per block)
{
	U32 page_idx;
       U32 page_shift = 0;
	U32 real_row;

	real_row = rowaddr;
       #if 0
	if(pHandle->m_NFB_header.NFIinfo.tlcControl.normaltlc) // normal tlc
	{		
		if(pHandle->m_NFB_header.NFIinfo.tlcControl.pPlaneEn) //has pseudo plane
		{
			page_shift = 9;	
		}
		else
		{
			page_shift = 8;
		}
		page_idx = ((rowaddr >> page_shift) * pHandle->m_brom_nfi_cfg.nfi_pages_per_block) + ((rowaddr & 0xFF) * 3); //always get wl's low page
	}
	else//micron tlc
	{
		for(page_shift = 15; page_shift > 0; page_shift--)
		{
			if((pHandle->m_brom_nfi_cfg.nfi_pages_per_block & (1 << page_shift)) != 0)
				break;
		}
		
		page_idx = ((rowaddr >> page_shift) * pHandle->m_brom_nfi_cfg.nfi_pages_per_block) + ((rowaddr << (16-page_shift)) >> (16-page_shift));
	}
	#else
	if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc) // normal tlc
	{		
		page_shift = g_pNandInfo->m_dev->m_tlc_setting->block_bit;
		if(g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
		{
		    real_row &= (~(1 << g_pNandInfo->m_dev->m_tlc_setting->pPlane_bit));
		}
		page_idx = ((real_row >> page_shift) * g_pNandInfo->m_dev->m_hw_info.m_pages_per_block) + (((real_row << (32-page_shift)) >> (32-page_shift)) * 3); //always get wl's low page
	}
	else//micron tlc
	{
		page_shift = g_pNandInfo->m_dev->m_tlc_setting->block_bit;
		
		page_idx = ((real_row >> page_shift) * g_pNandInfo->m_dev->m_hw_info.m_pages_per_block) + ((real_row << (32-page_shift)) >> (32-page_shift));
	}
	#endif

	return page_idx;
}

STATUS_E NFI_TLC_PageWrite(const uint32  c_timeout, 
                                                        const uint32 page_index, 
                                                        const uint32 * sector_buff, 
                                                        const uint32 * spare_buff,
                                                        uint32  ecc_parity_from_reg[8])
{
    uint8* main_ptr = NULL;
    uint8* spare_ptr = NULL;
    U32  sector_size = NAND_SECTOR_SIZE;
    STATUS_E  status    = S_DONE;
    //uint32  ecc_parity_from_reg[8];
    U32   nfi_sector_num = g_pNandInfo->m_dev->m_hw_info.m_page_size /g_pNandInfo->m_dev->m_hw_info.m_sector_size;
	U32 page_per_block = NUTL_PAGES_PER_BLOCK();

    //DBG_MSG("page 0x%x,sector_buff=0x%x, tlc_program_cycle=%d \n\r", page_index, sector_buff, tlc_program_cycle);

    sector_size  = g_pNandInfo->m_dev->m_hw_info.m_sector_size;

    main_ptr = (P_U8)sector_buff;
    spare_ptr = (P_U8)spare_buff;

    if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC) && g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
    {
        nfi_sector_num = nfi_sector_num / 2;
		if(NUTL_2P_TLC() && (!bl_region_ing))
			nfi_sector_num >>= 1;

		tlc_snd_phyplane = FALSE;
        tlc_lg_left_plane = TRUE; //program left plane
        status = NAND_COMMON_PageProgram_V3(NFI_PSTA_TIMEOUT, page_index,  (uint32 *)main_ptr, (uint32 *)spare_ptr, ecc_parity_from_reg);
	 if( S_DONE != status )
        {
            return status;
        }

        tlc_lg_left_plane = FALSE; //program right plane
        main_ptr += (sector_size * nfi_sector_num);
        if(spare_buff)
            spare_ptr += (nfi_sector_num) * 8/*nfi_fdm_size*/;
        status = NAND_COMMON_PageProgram_V3(NFI_PSTA_TIMEOUT, page_index, (uint32 *)main_ptr, (uint32 *)spare_ptr, ecc_parity_from_reg);

		if(NUTL_2P_TLC() && (!bl_region_ing))
		{
			tlc_snd_phyplane = TRUE;
	        tlc_lg_left_plane = TRUE; //program left plane
	        main_ptr += (sector_size * nfi_sector_num);
        	if(spare_buff)
            	spare_ptr += (nfi_sector_num) * 8/*nfi_fdm_size*/;
	        status = NAND_COMMON_PageProgram_V3(NFI_PSTA_TIMEOUT, (page_index+page_per_block),  (uint32 *)main_ptr, (uint32 *)spare_ptr, ecc_parity_from_reg);
		 if( S_DONE != status )
	        {
	            return status;
	        }

	        tlc_lg_left_plane = FALSE; //program right plane
	        main_ptr += (sector_size * nfi_sector_num);
	        if(spare_buff)
	            spare_ptr += (nfi_sector_num) * 8/*nfi_fdm_size*/;
	        status = NAND_COMMON_PageProgram_V3(NFI_PSTA_TIMEOUT, (page_index+page_per_block), (uint32 *)main_ptr, (uint32 *)spare_ptr, ecc_parity_from_reg);
			tlc_snd_phyplane = FALSE;
		}
    }
    else
    {
    	if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC) && NUTL_2P_TLC()
			&& (!bl_region_ing))
			nfi_sector_num >>= 1;
		tlc_snd_phyplane = FALSE;
        status = NAND_COMMON_PageProgram_V3(NFI_PSTA_TIMEOUT, page_index,  (uint32 *)main_ptr, (uint32 *)spare_ptr, ecc_parity_from_reg);
		if( S_DONE != status )
        {
            return status;
        }

		if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC) && NUTL_2P_TLC()
			&& (!bl_region_ing))
		{
			tlc_snd_phyplane = TRUE;
			main_ptr += (sector_size * nfi_sector_num);
        	if(spare_buff)
            	spare_ptr += (nfi_sector_num) * 8/*nfi_fdm_size*/;
			status = NAND_COMMON_PageProgram_V3(NFI_PSTA_TIMEOUT, (page_index+page_per_block),  (uint32 *)main_ptr, (uint32 *)spare_ptr, ecc_parity_from_reg);
			tlc_snd_phyplane = FALSE;
		}
    }

    return status;
}


STATUS_E NFI_TLC_WLProgram(U32 wl_index, P_U32 sector_buff, P_U32 spare_buff,
                                NFI_TLC_PG_CYCLE program_cycle)
{
    U32 page_idx = 0;
    P_U8 main_ptr = NULL;
    bool hw_ecc_enable = TRUE;
    STATUS_E  status    = S_DONE;


    main_ptr = (P_U8)sector_buff;
    tlc_program_cycle = program_cycle;
    page_idx = wl_index * 3;
	
    status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, page_idx, (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);
    if( S_DONE != status )
    {
        return status;
    }
        main_ptr += g_pNandInfo->m_dev->m_hw_info.m_page_size;
    status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, (page_idx+1), (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);
    if( S_DONE != status )
    {
        return status;
    }
        main_ptr += g_pNandInfo->m_dev->m_hw_info.m_page_size;
    status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, (page_idx+2), (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);

    return status;
}

STATUS_E  NAND_COMMON_TLC_BlockProgram(const uint32  c_timeout, U32 blk_index, P_U32 sector_buff, P_U32 spare_buff)
{
    U32 idx = 0;
    U32 base_idx = 0;
    U32 pg_time_per_block = 0;
    P_U8 main_ptr = NULL;
    STATUS_E  status    = S_DONE;
    MSG(DEBUG,"line=%d,function= %s ,blk_index=%d \n ",  __LINE__,  __FUNCTION__, blk_index);

	
    main_ptr = (P_U8)sector_buff;
	
    if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)
    {
    	if(!g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
    		tlc_cache_program = TRUE;
		
        pg_time_per_block = (g_pNandInfo->m_dev->m_hw_info.m_pages_per_block );// SLC view, slc page number
	 for(idx = 0; idx < pg_time_per_block; idx++)
	 {        
	     if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)//not-exist this case
	     {
	         base_idx = blk_index * g_pNandInfo->m_dev->m_hw_info.m_pages_per_block;
		  
	   
	         MSG(DEBUG,"Program page 0x%x\n\r", (base_idx + idx*3));
		  status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, (base_idx + idx*3), (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);
		  if( S_DONE != status )
                {
                    return status;
                }
		  
	             main_ptr += g_pNandInfo->m_dev->m_hw_info.m_page_size;
	     }
  	     else
  	     {
  	            base_idx = blk_index * pg_time_per_block;
  	            MSG(DEBUG,"Program wl 0x%x\n\r", (base_idx + idx));
		     if(idx == 0)
		     {
                       main_ptr = ((P_U8)sector_buff) + (idx)*3*g_pNandInfo->m_dev->m_hw_info.m_page_size;
		         status = NFI_TLC_WLProgram((base_idx + idx), (P_U32)main_ptr, spare_buff,PROGRAM_1ST_CYCLE);
			  if( S_DONE != status )
	                {
	                    return status;
	                }
			  
                       main_ptr = ((P_U8)sector_buff) + (idx+1)*3*g_pNandInfo->m_dev->m_hw_info.m_page_size;
			  status = NFI_TLC_WLProgram((base_idx + idx + 1), (P_U32)main_ptr, spare_buff, PROGRAM_1ST_CYCLE);
			  if( S_DONE != status )
	                {
	                    return status;
	                }
			  
                       main_ptr = ((P_U8)sector_buff) + (idx)*3*g_pNandInfo->m_dev->m_hw_info.m_page_size;
			  status = NFI_TLC_WLProgram((base_idx + idx), (P_U32)main_ptr, spare_buff, PROGRAM_2ND_CYCLE);
			  if( S_DONE != status )
	                {
	                    return status;
	                }
		     }
		     
		     if((idx + 2) < pg_time_per_block)
		     {
                       main_ptr = ((P_U8)sector_buff) + (idx+2)*3*g_pNandInfo->m_dev->m_hw_info.m_page_size;
		         status = NFI_TLC_WLProgram((base_idx + idx + 2), (P_U32)main_ptr, spare_buff, PROGRAM_1ST_CYCLE);
			  if( S_DONE != status )
	                {
	                    return status;
	                }
		     }
		     if((idx + 1) < pg_time_per_block)
		     {
                       main_ptr = ((P_U8)sector_buff) + (idx+1)*3*g_pNandInfo->m_dev->m_hw_info.m_page_size;
		         status = NFI_TLC_WLProgram((base_idx + idx + 1), (P_U32)main_ptr, spare_buff, PROGRAM_2ND_CYCLE);
			  if( S_DONE != status )
	                {
	                    return status;
	                }
		     }

			 if(idx == (pg_time_per_block-1))
			     tlc_cache_program = FALSE;
			 
                   main_ptr = ((P_U8)sector_buff) + (idx)*3*g_pNandInfo->m_dev->m_hw_info.m_page_size;
		     status = NFI_TLC_WLProgram((base_idx + idx), (P_U32)main_ptr, spare_buff, PROGRAM_3RD_CYCLE);
		     if( S_DONE != status )
	            {
	                return status;
	            }
  	     }     
	 }
	 tlc_cache_program = FALSE;
    }
    else//micron
    {
        //to deal with slc/tlc mode later
        pg_time_per_block = g_pNandInfo->m_dev->m_hw_info.m_pages_per_block;
	 base_idx = blk_index * pg_time_per_block;
	 for(idx = 0; idx < pg_time_per_block; idx++)
	 {
	     MSG(DEBUG,"Program page 0x%x\n\r", (base_idx + idx));
	   
	         main_ptr += g_pNandInfo->m_dev->m_hw_info.m_page_size;
	     status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, (base_idx+idx), (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);
	     if( S_DONE != status )
            {
                return status;
            }
	 }
    }
    return status;
}

#endif

//------------------------------------------------------------------------------
// Page Read Callback Function                                                  
//------------------------------------------------------------------------------
#if 1
void dump_nfi(void) {
	uint32 cnt;
    DBG_MSG( "NFI_ACCCON: 0x%x\n", *NFI_ACCCON);
    DBG_MSG( "NFI_PAGEFMT: 0x%x\n", *NFI_PAGEFMT);
    DBG_MSG( "NFI_CNFG: 0x%x\n", *NFI_CNFG);
    DBG_MSG( "NFI_CON: 0x%x\n", *NFI_CON);
    DBG_MSG( "NFI_STRDATA: 0x%x\n", *NFI_STRDATA);
    DBG_MSG( "NFI_ADDRCNTR: 0x%x\n", *NFI_ADDRCNTR);
    DBG_MSG( "NFI_FIFOSTA: 0x%x\n", *NFI_FIFOSTA);
    DBG_MSG( "NFI_ADDRNOB: 0x%x\n", *NFI_ADDRNOB);
    DBG_MSG( "NFI_FDM_0L: 0x%x\n", *NFI_FDM0L);
    DBG_MSG( "NFI_STA: 0x%x\n", *NFI_STA);
    DBG_MSG( "NFI_FDM_0M: 0x%x\n", *NFI_FDM0M);
    DBG_MSG( "NFI_IOCON: 0x%x\n", *NFI_IOCON);
    DBG_MSG( "NFI_BYTELEN: 0x%x\n", *NFI_BYTELEN);
    DBG_MSG( "NFI_COLADDR: 0x%x\n", *NFI_COLADDR);
    DBG_MSG( "NFI_ROWADDR: 0x%x\n", *NFI_ROWADDR);
    DBG_MSG( "ECC_ENCCNFG: 0x%x\n", *ECC_ENCCNFG);
    DBG_MSG( "ECC_ENCCON: 0x%x\n", *ECC_ENCCON);
    DBG_MSG( "ECC_DECCNFG: 0x%x\n", *ECC_DECCNFG);
    DBG_MSG( "ECC_DECCON: 0x%x\n", *ECC_DECCON);
    DBG_MSG( "NFI_CSEL: 0x%x\n", *NFI_CSEL);
	//for(cnt = 0; cnt < 336; cnt++)
	//DBG_MSG("%x %x %x %x\n", *((volatile uint32 *)(0x1100D000+cnt*16+0)),*((volatile uint32 *)(0x1100D000+cnt*16+4)),*((volatile uint32 *)(0x1100D000+cnt*16+8)),*((volatile uint32 *)(0x1100D000+cnt*16+12)));
}

#ifndef NFI_MEM_OPTIMIZE
STATUS_E  NAND_COMMON_PageRead_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {
	uint32		page_size;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		row_addr_nob, col_addr_nob;	
	uint32		sectors;
	uint32            parePerSector;
    uint32      intr;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32      u4DECCODESize;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
	bool		bUsingDMA=TRUE;
	bool        readRetry = FALSE;
	uint32     readRetryCount = 0;
	uint32     readRetryVal= 0;
	uint8*     spare_reformat = (uint8*)p_data32;
	uint32     page_addr = (uint32)row_addr;
#ifdef DUMP_SPEED
    uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif

	if(reset_fdm_and_ecc)
	{
	    if(page_addr < second_part_start_page)
	    {
	    	mtk_nand_reset_fdm_ecc(FALSE);
	    }
		else
		{
			mtk_nand_reset_fdm_ecc(TRUE);
		}
	}
	
#if CFG_2CS_NAND
		if(g_bTricky_CS)
		{
			page_addr = NAND_COMMON_CS_ON(1, page_addr);
		}
#endif

	do{
	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	sectors = NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;
	// 1. Init NFI
	// 1.1 reset NFI
		memset(&dbg_MLC, 0, sizeof(struct debug_info_MLC));
		//memset(dbg_SLC, 0, sizeof(debug_info_SLC));
		//memset(debug_spare, 0, sizeof(u8)*1024);
		memcpy(&dbg_MLC.magic, magic ,7);
		//xiaolei macro reset
		if(*NFI_NAND_TYPE_CNFG & 0X3)
		{
			*NFI_MASTER_RST |= PAD_MACRO_RST;
			*NFI_MASTER_RST &= (~PAD_MACRO_RST);
		}
	NFI_Reset();

	if(page_addr >= 2048)
		{
		//DBG_MSG("RAN_TRUE\n");
		NAND_COMMON_Randomizer_Config(TRUE);
		NAND_COMMON_Turn_On_Randomizer();
		}
	else
		{
		//DBG_MSG("RAN_FALSE\n");
		NAND_COMMON_Randomizer_Config(FALSE);
		NAND_COMMON_Turn_Off_Randomizer();
		}

#ifdef DISABLE_NFI_DMA
	bUsingDMA = FALSE;
#else
	bUsingDMA = TRUE;
#endif


	bUsingDMA = TRUE;
	*NFI_INTR_EN = INTR_AHB_DONE_EN;

	// 1.2 read destination address
	intr = *NFI_INTR;
	*NFI_STRADDR = (uint32)p_data32;
	// 1.3 enable ECC Decoder
	// u4DECCODESize = (NAND_SECTOR_SIZE + 8) << 3 + 4 * 13;
	*ECC_DECCNFG |= (ECC_CNFG_NFI /*| DEC_CNFG_EMPTY_EN*/ | DEC_CNFG_CORRECT); // | (u4DECCODESize << DEC_CNFG_CODE_SHIFT));  mark empty en to fix empty page read fail   xiaolei
	STOP_ECC_DEC
	START_ECC_DEC
	// 1.4 set CNFG
	//*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
	*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
	*NFI_CNFG |= ((bUsingDMA)?(CNFG_AHB|CNFG_AHB_BURST):0);				// AHB mode or NFI mode
	
// Ray's todo:		
//	*NFI_CNFG |= CNFG_AUTO_FMT_EN;	// if read for verify, use auto format; if read back raw data, don't use it!
	
	// ?? *CNFG |= CNFG_BYTE_RW; ??

	if(fgRandomEn)
	{
		NFI_RANDOMIZER_DECODE(page_addr, TRUE, fgSSRandomizer, fgPageReload, 0);
	}
	// 2. Issue Read CMD
	// 2.1 read cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd;
	// 2.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ 
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_DECODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }		

	// 3. Issue Addr 
	// 3.1 set  row addr, col addr, and num of byte
	// fill 1~4 cycle addr 
	*NFI_ROWADDR = page_addr;
	*NFI_COLADDR = 0;

	// no. of addr cycle 
	col_addr_nob =  (page_size==512)?1:2;	// 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
	row_addr_nob = addr_cycle - col_addr_nob;
	*NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT)) ;
	
	// 3.2 wait til ADDR is completely issued 
	timeout = c_timeout;
	NFI_Wait_Adddress(timeout);
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_DECODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }

	// 4. Issue Read Confirm CMD
	if(g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_enable) {
		// 4.1 read confirm 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd;
		// 4.2 wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout){
			if(fgRandomEn)
			{
				NFI_RANDOMIZER_DECODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
			}
			return S_DA_TIMEOUT; }			
	}
	
	// 5. Wait for Read Buffer is ready
	// wait til device is from busy to ready
	timeout = c_timeout;
	NFI_Wait_Return_Ready(timeout);
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_DECODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }	
	
	// 6. set NFI_CON
	// 6.1 set burst read by DWORD 
	*NFI_CON = (CON_NFI_BRD | (sectors << CON_NFI_SEC_SHIFT));

	// 6.2 wait til DATA_READ is completely issued 
	//wait til Device is ready
	timeout=c_timeout;
	NFI_Wait_Ready(timeout);
	// 7. Read data
	// read page data 
	if( S_DONE != (ret=NUTL_FIFO_Read_V3(c_timeout, bUsingDMA, p_data32, page_size)) ) {
		//goto end;		
	}	
		if(S_DA_NAND_ECC_2BITS_ERR == ret) //xiaolei
		//if(S_DA_NAND_UNKNOWN_ERR == ret)
		{
			if(fgRandomEn)
			{
				NFI_RANDOMIZER_DECODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
			}
			
			if(NUTL_DEVICE_MLC())
			{
				if((NUTL_SET_FEATURE() != 0xFF))
				{
					#if 1
					if(readRetryCount < NUTL_READ_RETRY_NUM())
					{
						readRetryVal = NUTL_READ_RETRY_VALUE(readRetryCount);
						readRetry = TRUE;
					}
					else
					{
						readRetryVal = NUTL_READ_RETRY_DEFAULT();
						readRetry = FALSE;
					}
					#else
					if(readRetryCount < NUTL_READ_RETRY_NUM())
					{
						readRetryVal = NUTL_READ_RETRY_DEFAULT()+readRetryCount;
						readRetry = TRUE;
					}
					else
					{
						readRetryVal = NUTL_READ_RETRY_DEFAULT();
						readRetry = FALSE;
					}
					#endif	
					readRetryCount++;

					if(NUTL_READ_RETRY_TYPE() == RTYPE_MICRON)
					{
						NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&readRetryVal,4);	
					}
					else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK)
					{
						if(!readRetry)
						{
							NAND_COMMON_Reset_V3(timeout); //to exit
						}
							
						NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&readRetryVal,4);	
												
						if(readRetry)
						{
							DRV_WriteReg(NFI_CMD, NUTL_READ_RETRY_PRECMD());//to enable
							/* wait for the command status ready */
				           		while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
						}
					}
					else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA)
					{
						mtk_nand_parapage_rrtry(readRetryVal);
					}
					else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA_15NM)
					{
						mtk_nand_parapage_15nm_rrtry(readRetryVal, 0);
					}					
					else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX)
					{
						HYNIX_Set_RR_Para(readRetryVal);
					}
					else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_16NM)
					{
#if 1
						HYNIX_16NM_Set_RR_Para(readRetryVal, 0);
#else						
						HYNIX_16NM_Set_RR_Para(readRetryVal);
#endif						
					}
					else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_FDIE)
					{
						HYNIX_FDIE_Set_RR_Para(readRetryVal, 0);		
					}
					else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK_19NM)
					{
						sandisk_19nm_rr(readRetryVal, !readRetry);
					}
				}
				else
				{
					readRetry = FALSE;
				}
			}
			else
			{
				readRetry = FALSE;
			}
		}
		else if(S_DONE == ret)
		{
			if((readRetryCount != 0) && (NUTL_DEVICE_MLC()))
			{
				readRetryVal = NUTL_READ_RETRY_DEFAULT();
				readRetry = FALSE;
#if 0
				if(NUTL_READ_RETRY_TYPE() == RTYPE_MICRON)
				{
					NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&readRetryVal,4);	
				}
				else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK)
				{
					NAND_COMMON_Reset_V3(timeout); //to exit
						
					NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&readRetryVal,4);	
				}
				else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA)
				{
					mtk_nand_parapage_rrtry(readRetryVal);
				}
				else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX)
				{
					HYNIX_Set_RR_Para(readRetryVal);
				}
				else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_16NM)
				{
					HYNIX_16NM_Set_RR_Para(readRetryVal);
				}
				else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK_19NM)
				{
					sandisk_19nm_rr(readRetryVal, !readRetry);
				}
#endif				
			}
		}
	// <<<<  WARNING!! >>>>                                                               
	// 1. You MUST wait until the NFI FIFO is empty.                                      
	//    It means all data in the FIFO had been read out, and then you can start to read 
	//    ECC parity registers.   
	}while(TRUE == readRetry);
	
#if 1
    if((readRetryCount != 0) && NUTL_DEVICE_MLC())
	{
		readRetryVal = NUTL_READ_RETRY_DEFAULT();

		if(NUTL_READ_RETRY_TYPE() == RTYPE_MICRON)
		{
			NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&readRetryVal,4);	
		}
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK)
		{
			NAND_COMMON_Reset_V3(timeout); //to exit
				
			NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&readRetryVal,4);	
		}
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA)
		{
			mtk_nand_parapage_rrtry(readRetryVal);
		}
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA_15NM)
		{
			mtk_nand_parapage_15nm_rrtry(readRetryVal, 1);
		}				
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX)
		{
			HYNIX_Set_RR_Para(readRetryVal);
		}
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_16NM)
		{
			HYNIX_16NM_Set_RR_Para(0, 1);
			//DBG_MSG("readtry OK at page:%d count:%d\n",page_addr, readRetryCount);
		}     
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_FDIE)
		{
			HYNIX_FDIE_Set_RR_Para(0, 1);
			//DBG_MSG("readtry OK at page:%d count:%d\n",page_addr, readRetryCount);
		}
	}   
#endif   	
#ifdef DUMP_SPEED
    g_ds_nand_page_read_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_read_time += (tick_end - tick_start);
#endif

end:

	if(NUTL_DEVICE_MLC())
	{
       #if 0
	#if 0
		parePerSector = spare_size/sectors;
		//memcpy(debug_spare, (void*)(spare_reformat+NAND_SECTOR_SIZE+1),7);
		//memcpy((&debug_spare[7]), (void*)(spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+1),3);
		//memcpy((spare_reformat+NAND_SECTOR_SIZE+0) ,debug_spare, 10);
		
		//memcpy(spare_reformat, (spare_reformat+1),7);
		*(spare_reformat+NAND_SECTOR_SIZE+0) = *(spare_reformat+NAND_SECTOR_SIZE+1) ;
		*(spare_reformat+NAND_SECTOR_SIZE+1) = *(spare_reformat+NAND_SECTOR_SIZE+2) ;
		*(spare_reformat+NAND_SECTOR_SIZE+2) = *(spare_reformat+NAND_SECTOR_SIZE+3) ;
		*(spare_reformat+NAND_SECTOR_SIZE+3) = *(spare_reformat+NAND_SECTOR_SIZE+4) ;
		*(spare_reformat+NAND_SECTOR_SIZE+4) = *(spare_reformat+NAND_SECTOR_SIZE+5) ;
		*(spare_reformat+NAND_SECTOR_SIZE+5) = *(spare_reformat+NAND_SECTOR_SIZE+6) ;
		*(spare_reformat+NAND_SECTOR_SIZE+6) = *(spare_reformat+NAND_SECTOR_SIZE+7) ;
		//memcpy((spare_reformat+NAND_SECTOR_SIZE+8), (spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+1),4);
		*(spare_reformat+NAND_SECTOR_SIZE+8) = *(spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+1);
		*(spare_reformat+NAND_SECTOR_SIZE+9) = *(spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+2);
		//*(spare_reformat+NAND_SECTOR_SIZE+10) = *(spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+3);
		#endif
		memcpy((void*)(spare_reformat+NAND_SECTOR_SIZE+8), &dbg_MLC, sizeof(struct debug_info_MLC));
       #endif
		
	}
       #if 0
	else
	{
	
	}
       #endif

	NAND_COMMON_Turn_Off_Randomizer();
	// 8. Stop ECC Decode
	
	if(fgRandomEn)
	{
		NFI_RANDOMIZER_DECODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		/*
		if((DRV_Reg32(NFI_STA) & STA_READ_EMPTY) > 0) //mark this because cmd setting will trigger NFI to reset nfi_sta sta_read_empty xiaolei
		{
			if((*NFI_CNFG & CNFG_AUTO_FMT_EN) != 0)
				memset((void*)p_data32, 0xFF, page_size);
			else
				memset((void*)p_data32, 0xFF, page_size + spare_size);
			ret = S_DONE;
		}
		*/
	}

	//comment from xiaolei: 
	//For read retry mechanism, i changed the return value from S_DONE to S_DA_NAND_ECC_2BITS_ERR when it occurs ECC error.
	//This results in Format Fail when Force Erase is not selected.
	//Setting ret as S_DONE here is just a workaround with less risk to meet the orginal logical. 
	ret = S_DONE; 
	
	return ret;
}

//------------------------------------------------------------------------------
// Read From NFI FIFO                                                           
//------------------------------------------------------------------------------

STATUS_E  NUTL_FIFO_Read_V3(
				const uint32  c_timeout
				,const bool bUsingDMA
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,const uint32 data_len
) {
	uint32	timeout = c_timeout;
	uint32	i;
	uint32 	sectors;
	uint32     sector;
	uint32	sector_page_size32;
	uint32	sector_spare_size32;
	uint32	sector_pagespare_size32;
	uint32	dec_mask;
	uint32	ErrNum;
	uint32	ErrBitLoc;
	uint32     sector_shift = NAND_SECTOR_SHIFT;
	uint32     fail_sector = 0;

	sectors				= NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;
	sector_page_size32 	= (NUTL_PAGE_SIZE()/sectors)>>2;
	sector_spare_size32 = (NUTL_SPARE_SIZE()/sectors)>>2;
	sector_pagespare_size32 = sector_page_size32 + sector_spare_size32;
	if(NAND_SECTOR_SIZE == 1024)
		sector_shift+=1;
	if(bUsingDMA) {	// AHB mode
		uint32 reg_val;
		uint32 read_bytes;
		
		timeout = c_timeout;
		// while( (ADDRCNTR_CNTR((*NFI_ADDRCNTR)) < sectors) && (--timeout));

		timeout = 0xffff;
		// while( (read_bytes < data_len) && (timeout>0) ); 

        while(*NFI_INTR == 0)
			;
		//dump_nfi();
		while ((data_len >> sector_shift) > ((*NFI_BYTELEN & 0x1F000) >> 12))
			;
		#if 0
		dec_mask = (1 << sectors) - 1;
		while (dec_mask != (*ECC_DECDONE))
			;
		#else // to fix ecc timeout issue xiaolei
		dec_mask = (1 << (sectors-1)); 
		#if defined(BB_MT8163)
		while ((0 == (dec_mask & (*ECC_DECDONE))) ||(0x01011101 != ((*ECC_DECFSM) & 0x3F3FFF0F)))
					;
		#else
		while ((0 == (dec_mask & (*ECC_DECDONE))) ||(0x01010101 != ((*ECC_DECFSM) & 0x7F0F0F0F)))
			;
		#endif
		#endif
		*NFI_INTR_EN = 0;
		 fail_sector = 0;
		dbg_MLC.sector_number = sectors;
		for(sector=0;sector < sectors;sector++)
		{
			ErrNum = (DRV_Reg32((ECC_DECENUM0+(sector/4)))>>((sector%4)*8))& ERR_NUM0;
			//DBG_MSG("sector %d, ErrNum %d\n",sector,ErrNum);
			dbg_MLC.sector_bit_flip_num[sector] = ErrNum;
			if(ERR_NUM0 == ErrNum)
			{
				dbg_MLC.sector_ecc_bitmap |= 1<<sector;
				fail_sector++;
				//DBG_MSG("[xiaolei] fail sector %d\n", sector);
				//DBG_MSG("[xiaolei] sector %d, ET 0x%x\n",sector, *NFI_EMPTY_THRESH);
			}
		}
#if 1
		if(fail_sector == sectors)
		{
			if((*NFI_STA & STA_READ_EMPTY) != 0)
			{
				if(bad_block_remark_done)
				memset(p_data32, 0xFF,(data_len+NUTL_SPARE_SIZE()));
				//DBG_MSG("EMPTY PAGE!\n");
				fail_sector = 0;
				dbg_MLC.sector_ecc_bitmap = 0;
				//DBG_MSG("[xiaolei] FER 0x%x\n", *ECC_DECFER);
			}
		}
#endif
		STOP_ECC_DEC
	}
	else{			// NFI mode - dword access mode
		uint32 sector;
		// read page data 
		 fail_sector = 0;
		for(sector=0; sector < sectors; sector++)
		{
			// 1. Read main area per sector
			for(i=0; i<sector_page_size32; i++) 
			{
				// wait for data ready 
	
				timeout = c_timeout;
                WAIT_NFI_PIO_READY(timeout);
				if( !timeout ) { return S_DA_TIMEOUT;}
					
				//*(p_data32 + sector_page_size32*sector + i) = *NFI_DATAR;
				*(p_data32 + sector_pagespare_size32*sector + i) = *NFI_DATAR;
				
			}
			// 2. Read spare area per sector
			for(i=0; i<sector_spare_size32; i++) 
			{
				// wait for data ready 
	
				// when RD_EMPTY_MASK flag is poll-down, it means data is ready in FIFO at least 4 bytes. 
				timeout = c_timeout;
				WAIT_NFI_PIO_READY(timeout);
				if( !timeout ) { return S_DA_TIMEOUT;}
					
				//*(p_data32 + sector_page_size32*sectors + sector_spare_size32*sector + i) = *NFI_DATAR;
				*(p_data32 + sector_pagespare_size32*sector + sector_page_size32 + i) = *NFI_DATAR;
			}			

			//MT6516 FPGA: Ray's todo: ECC Correct
			// decoder done
			dec_mask = 1 << sector;		  
			while (!(dec_mask & (*ECC_DECDONE)))
				;
#if	defined(__MTK_BB_REG_BASEADDRESS_GROUP12__)  || defined(__MTK_BB_REG_BASEADDRESS_GROUP11__) //for 6572
			ErrNum = (DRV_Reg32((ECC_DECENUM0+(sector/4)))>>((sector%4)*8))& ERR_NUM0;
#else
			ErrNum = (*ECC_DECENUM >> (ERR_NUM_SHIFT * sector)) & ERR_NUM0;
#endif
			if (ErrNum && ErrNum != ERR_NUM0)
			{
				for (i = 0; i < ErrNum; i++)
				{
				       #if defined(BB_MT8163)
					if(i < 60)
						ErrBitLoc = *(ECC_DECEL0 + i / 2);
					else
						ErrBitLoc = *(ECC_DECEL30 + i / 2 - 30);
					#else
					ErrBitLoc = *(ECC_DECEL0 + i / 2);
					#endif
				}
			}
			else
			{
				 fail_sector ++;
			}
		}
		if((*NFI_STA & STA_READ_EMPTY) != 0)
		{
			if(bad_block_remark_done)
			memset(p_data32, 0xFF,(data_len+NUTL_SPARE_SIZE()));
		}


		STOP_ECC_DEC

		
	}
	if( fail_sector == 0)
		return S_DONE;
	else
		return S_DA_NAND_ECC_2BITS_ERR; //uncorrectable ECC  xiaolei
		//return S_DONE;
}
#else
#if defined(__NFI_SUPPORT_TLC__)
STATUS_E NUTL_TLC_ChangeColumn(U32* real_row_addr, bool   tlc_left_plane)
{
     
     //bool   tlc_left_plane = TRUE;
     U32 reg_val = 0;
     U32 timeout = NFI_PSTA_TIMEOUT;
     if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
     {
         if(g_pNandInfo->m_dev->m_tlc_setting->needchangecolumn)
         {
     //MSG(DEBUG,"line=%d,function= %s \n ",  __LINE__,  __FUNCTION__);

             ////////////change colunm address///////////
             if(g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
                 *real_row_addr = NFI_TLC_SetpPlaneAddr(*real_row_addr, tlc_left_plane);
             #if 1 //reset here to flush fifo status left by the left plane read
             reg_val = DRV_Reg32(NFI_CON);
             reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
             /* issue reset operation */
             DRV_WriteReg32(NFI_CON, reg_val); //not use SAL_NFI_Reset() here
             //SAL_NFI_Reset();
             #endif
             
             reg_val = DRV_Reg(NFI_CNFG);
             reg_val &= ~CNFG_READ_EN;
             reg_val &= ~CNFG_OP_MODE_MASK;
             reg_val |= CNFG_OP_CUST;
             DRV_WriteReg(NFI_CNFG, reg_val);
             
             DRV_WriteReg(NFI_CMD, CHANGE_COLUNM_ADDR_1ST_CMD);
             NFI_Wait_Command(timeout);
             
             //DRV_WriteReg32(NFI_COLADDR, (tlc_left_plane)?0:(NUTL_PAGE_SIZE()/2));
             DRV_WriteReg32(NFI_ROWADDR, *real_row_addr);
	      *NFI_ADDRNOB = (((NUTL_ADDR_CYCLE() -((NUTL_PAGE_SIZE()==512)?1:2))<<ADDR_ROW_NOB_SHIFT)
		  	| (((NUTL_PAGE_SIZE()==512)?1:2)<<ADDR_COL_NOB_SHIFT)) ;

             //DRV_WriteReg(NFI_ADDRNOB, reg_val);
             NFI_Wait_Adddress(timeout);
             
             DRV_WriteReg(NFI_CMD, CHANGE_COLUNM_ADDR_2ND_CMD);                        // read command
             NFI_Wait_Command(timeout);

             #if 0 //reset here will result no sample later
             reg_val = DRV_Reg32(NFI_CON);
             reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
             /* issue reset operation */
             DRV_WriteReg32(NFI_CON, reg_val); //not use SAL_NFI_Reset() here
             //SAL_NFI_Reset();
             #endif
             
             reg_val = DRV_Reg(NFI_CNFG);
             reg_val |= CNFG_READ_EN;
             reg_val &= ~CNFG_OP_MODE_MASK;
             reg_val |= CNFG_OP_READ;
             DRV_WriteReg(NFI_CNFG, reg_val);
         }
     }

}
#endif
STATUS_E NUTL_MLC_ReadRetry(STATUS_E readret, uint32 page_addr , uint32 timeout, uint32* readRetryCount, uint32* readRetryVal, bool*  readRetry)
{
     STATUS_E	ret = S_DA_NAND_UNKNOWN_ERR;
     uint32 mlcreadRetryCount = *readRetryCount;
     uint32 mlcreadRetryVal     = *readRetryVal;
     bool    mlcreadRetry          = *readRetry;

     if(S_DA_NAND_ECC_2BITS_ERR == readret) //xiaolei
     //if(S_DA_NAND_UNKNOWN_ERR == ret)
     {
         if(fgRandomEn)
         {
             NFI_RANDOMIZER_DECODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
         }
         
         if(NUTL_DEVICE_MLC())
         {
             if((NUTL_SET_FEATURE() != 0xFF))
             {
                 #if 1
                 if(mlcreadRetryCount < NUTL_READ_RETRY_NUM())
                 {
                     mlcreadRetryVal = NUTL_READ_RETRY_VALUE(mlcreadRetryCount);
                     mlcreadRetry = TRUE;
                 }
                 else
                 {
                     mlcreadRetryVal = NUTL_READ_RETRY_DEFAULT();
                     mlcreadRetry = FALSE;
                 }
                 #else
                 if(readRetryCount < NUTL_READ_RETRY_NUM())
                 {
                     readRetryVal = NUTL_READ_RETRY_DEFAULT()+readRetryCount;
                     readRetry = TRUE;
                 }
                 else
                 {
                     readRetryVal = NUTL_READ_RETRY_DEFAULT();
                     readRetry = FALSE;
                 }
                 #endif	
                 mlcreadRetryCount++;
          if(mlcreadRetry == TRUE){       
                 if(NUTL_READ_RETRY_TYPE() == RTYPE_MICRON)
                 {
                     NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&mlcreadRetryVal,4);	
                 }
                 else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK)
                 {
                     if(!mlcreadRetry)
                     {
                         NAND_COMMON_Reset_V3(timeout); //to exit
                     }
                     
                     NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&mlcreadRetryVal,4);	
                     	
                     if(mlcreadRetry)
                     {
                         DRV_WriteReg(NFI_CMD, NUTL_READ_RETRY_PRECMD());//to enable
                         /* wait for the command status ready */
                         while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
                     }
                 }
                 else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA)
                 {
                     mtk_nand_parapage_rrtry(mlcreadRetryVal);
                 }
		 else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA_15NM)
			{
				mtk_nand_parapage_15nm_rrtry(mlcreadRetryVal, 0);
			}		
                 else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX)
                 {
                     HYNIX_Set_RR_Para(mlcreadRetryVal);
                 }
                 else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_16NM)
                 {
                     #if 1
						HYNIX_16NM_Set_RR_Para(mlcreadRetryVal, 0);
#else						
						HYNIX_16NM_Set_RR_Para(readRetryVal);
#endif	
                 }
		 else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_FDIE)
			{
					HYNIX_FDIE_Set_RR_Para(mlcreadRetryVal, 0);
				}
                 else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK_19NM)
                 {
                     sandisk_19nm_rr(mlcreadRetryVal, !mlcreadRetry);
                 }
          	}
             }
             else
             {
                 mlcreadRetry = FALSE;
             }
         }
         else
         {
             mlcreadRetry = FALSE;
         }
     }
     else if(S_DONE == readret)
     {
         if((mlcreadRetryCount != 0) && (NUTL_DEVICE_MLC()))
         {
             mlcreadRetryVal = NUTL_READ_RETRY_DEFAULT();
             mlcreadRetry = FALSE;
         #if 0    
             if(NUTL_READ_RETRY_TYPE() == RTYPE_MICRON)
             {
                 NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&mlcreadRetryVal,4);	
             }
             else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK)
             {
                 NAND_COMMON_Reset_V3(timeout); //to exit
                 
                 NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&mlcreadRetryVal,4);	
             }
             else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA)
             {
                 mtk_nand_parapage_rrtry(mlcreadRetryVal);
             }
             else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX)
             {
                 HYNIX_Set_RR_Para(mlcreadRetryVal);
             }
             else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_16NM)
             {
                 #if 1
						HYNIX_16NM_Set_RR_Para(readRetryVal, 0);
#else						
						HYNIX_16NM_Set_RR_Para(readRetryVal);
#endif	
             }
             else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK_19NM)
             {
                 sandisk_19nm_rr(mlcreadRetryVal, !mlcreadRetry);
             }
#endif
         }
     }
     *readRetryCount = mlcreadRetryCount;
     *readRetryVal     = mlcreadRetryVal;
     *readRetry          = mlcreadRetry;
     ret = S_DONE;
     return ret;
}
STATUS_E NAND_READ_FDM_DATA(uint32 *p_data32, uint32 sectornum)
{
	uint32 i,ret;
	uint32 *p_spare32 = (uint32 *)p_data32;
//uint32 *p_spare = (uint32 *)p_data32;

	if(p_spare32)
	{
		for(i = 0; i < sectornum; i++)
		{
			*p_spare32++ = *(NFI_FDM0L + (i << 1));
			*p_spare32++ = *(NFI_FDM0M + (i << 1));
			//ret = *(NFI_FDM0L + (i << 1));
			//ret = *(NFI_FDM0M + (i << 1));
#if 0//defined (__NFI_SUPPORT_TLC__)
                     if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
                     {
                         DBG_MSG("fdm1:0x%x \n",  p_spare32);
                         p_spare32 -= 2;
                         ret = ((uint32)p_spare32);
                         ret += (*NFI_PAGEFMT &PAGEFMT_FDM_MASK)>>8;
                         //p_spare32 = ret;
                        DBG_MSG("fdm1:0x%x \n",  p_spare32);
                     }

#endif
		}
	}

	//DBG_MSG("fdm: ");
	//for(i = 0; i < (2*sectornum); i++)
	//	DBG_MSG("0x%x ",*p_spare32++);
	//DBG_MSG("\n");
}
STATUS_E  NAND_COMMON_PageRead_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32 *p_spare32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {
	uint32		page_size;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		row_addr_nob, col_addr_nob;	
	uint32		sectors;
	uint32            parePerSector;
    uint32      intr;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32      u4DECCODESize;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
	bool		bUsingDMA=TRUE;
	bool        readRetry = FALSE;
	uint32     readRetryCount = 0;
	uint32     readRetryVal= 0;
	uint8*     spare_reformat = (uint8*)p_data32;
	uint32     page_addr = (uint32)row_addr;
	uint32     colu_addr = 0;
#ifdef DUMP_SPEED
    uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif
       U32   logical_plane_num   = 1;
       U32   logical_plane_index = 0;
       NFI_Reset();
	MSG(DEBUG," qq read start  ,line=%d,function= %s,row_addr=0x%x, p_data32=0x%x, data 0x%x, 0x%x,fgslc=%d \n ",  __LINE__,  __FUNCTION__, row_addr,p_data32,*p_data32, *(p_data32+16384/4 - 1),g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn);

#if defined(__NFI_SUPPORT_TLC__)

       U32 reg_val = 0;
       NFI_TLC_WL_INFO  tlc_wl_info;
       uint32 u4PgPerBl = g_pNandInfo->m_dev->m_hw_info.m_pages_per_block *3;
       bool   tlc_left_plane = TRUE;
       U32   real_row_addr = 0;
       U32   nfi_sector_num = g_pNandInfo->m_dev->m_hw_info.m_page_size /g_pNandInfo->m_dev->m_hw_info.m_sector_size;
	   U32 page_per_block = NUTL_PAGES_PER_BLOCK();
	   U32   snd_real_row_addr = 0;
	   NFI_TLC_WL_INFO  snd_tlc_wl_info;
	   
       if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
       {
           if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc) // normal tlc
           {
               NFI_TLC_GetMappedWL(row_addr, &tlc_wl_info);//SLC view, page addr == WL index,mark it
               if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
                   real_row_addr = NFI_TLC_GetRowAddr(page_addr);
               else
                   real_row_addr = NFI_TLC_GetRowAddr(tlc_wl_info.word_line_idx);
               //*NFI_IOCON = 0;
               
               if(g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
               {
                   tlc_left_plane = TRUE; // begin at left logical plane
                   logical_plane_num = 2;
                   *NFI_PAGEFMT = (*NFI_PAGEFMT & ~(0x3));    
                   if(4096 == g_pNandInfo->m_dev->m_hw_info.m_page_size) {
                       *NFI_PAGEFMT |= PAGEFMT_4K;
                   }
                   else if(8192 == g_pNandInfo->m_dev->m_hw_info.m_page_size) {
                       *NFI_PAGEFMT |= PAGEFMT_8K_1KS;
                   }
                   else if(16384 == g_pNandInfo->m_dev->m_hw_info.m_page_size) {
                       *NFI_PAGEFMT |= PAGEFMT_16K_1KS;
                   }
                   real_row_addr = NFI_TLC_SetpPlaneAddr(real_row_addr, tlc_left_plane);	
               }

		   if(NUTL_2P_TLC() && (!bl_region_ing))
		   {					
				NFI_TLC_GetMappedWL(page_addr + page_per_block, &snd_tlc_wl_info);
				if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
                    			snd_real_row_addr = NFI_TLC_GetRowAddr(page_addr + page_per_block);
           			else
				    snd_real_row_addr = NFI_TLC_GetRowAddr(snd_tlc_wl_info.word_line_idx);

				logical_plane_num *= 2;

				if(32768 == g_pNandInfo->m_dev->m_hw_info.m_page_size) {
                   			*NFI_PAGEFMT |= PAGEFMT_16K_1KS;
                			}

				if(g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
				{
					tlc_left_plane = TRUE; // begin at left logical plane
					snd_real_row_addr = NFI_TLC_SetpPlaneAddr(snd_real_row_addr, tlc_left_plane);	
				}
			}
           }
           else
           {
               real_row_addr = NFI_TLC_GetRowAddr(page_addr);
           }
           page_addr = real_row_addr;
           if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn ) // slc mode
           {
               if(0xFF != g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd)
               {
                   reg_val = DRV_Reg(NFI_CNFG);
                   reg_val &= ~CNFG_READ_EN;
                   reg_val &= ~CNFG_OP_MODE_MASK;
                   reg_val |= CNFG_OP_CUST;
                   DRV_WriteReg(NFI_CNFG, reg_val);
                   
                   DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
                   NFI_Wait_Command(timeout);
                                      
                   reg_val = DRV_Reg32(NFI_CON);
                   reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
                   /* issue reset operation */
                   DRV_WriteReg32(NFI_CON, reg_val);
               }
           }
           else//tlc mode
           {
               if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)
               {				
                   reg_val = DRV_Reg(NFI_CNFG);
                   reg_val &= ~CNFG_READ_EN;
                   reg_val &= ~CNFG_OP_MODE_MASK;
                   reg_val |= CNFG_OP_CUST;
                   DRV_WriteReg(NFI_CNFG, reg_val);
                   
                   if(tlc_wl_info.wl_pre == WL_LOW_PAGE)
                   DRV_WriteReg(NFI_CMD, LOW_PG_SELECT_CMD);
                   else if(tlc_wl_info.wl_pre == WL_MID_PAGE)
                   DRV_WriteReg(NFI_CMD, MID_PG_SELECT_CMD);
                   else if(tlc_wl_info.wl_pre == WL_HIGH_PAGE)
                   DRV_WriteReg(NFI_CMD, HIGH_PG_SELECT_CMD);
                   NFI_Wait_Command(timeout);
                                     
                   reg_val = DRV_Reg32(NFI_CON);
                   reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
                   /* issue reset operation */
                   DRV_WriteReg32(NFI_CON, reg_val);
               }
           }
           reg_val = 0; // reset reg_val	      
       }
#endif
	if(reset_fdm_and_ecc)
	{
	    if(page_addr < second_part_start_page)
	    {
	    	mtk_nand_reset_fdm_ecc(FALSE);
	    }
		else
		{
			mtk_nand_reset_fdm_ecc(TRUE);
		}
	}

#if CFG_2CS_NAND
		if(g_bTricky_CS)
		{
			page_addr = NAND_COMMON_CS_ON(1, page_addr);
		}
#endif
	do{

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
               sectors = (NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE)/logical_plane_num;
	// 1. Init NFI
               NFI_Reset();
		memset(&dbg_MLC, 0, sizeof(struct debug_info_MLC));
		//memset(dbg_SLC, 0, sizeof(debug_info_SLC));
		//memset(debug_spare, 0, sizeof(u8)*1024);
		memcpy(&dbg_MLC.magic, magic ,7);
		//xiaolei macro reset
		if(*NFI_NAND_TYPE_CNFG & 0X3)
		{
			*NFI_MASTER_RST |= PAD_MACRO_RST;
			*NFI_MASTER_RST &= (~PAD_MACRO_RST);
		}
               //NFI_Reset();

	if(page_addr >= 2048)
		{
		//DBG_MSG("RAN_TRUE\n");
		NAND_COMMON_Randomizer_Config(TRUE);
		NAND_COMMON_Turn_On_Randomizer();
		}
	else
		{
		//DBG_MSG("RAN_FALSE\n");
		NAND_COMMON_Randomizer_Config(FALSE);
		NAND_COMMON_Turn_Off_Randomizer();
		}

#ifdef DISABLE_NFI_DMA
	bUsingDMA = FALSE;
#else
	bUsingDMA = TRUE;
#endif

       if(_fgMcuRead)
           bUsingDMA = FALSE;
       else
           bUsingDMA = TRUE;

	//*NFI_INTR_EN = INTR_AHB_DONE_EN;

	// 1.2 read destination address
	intr = *NFI_INTR;
               *NFI_STRADDR = ((uint32)(p_data32)) + colu_addr;//QQ
	// 1.3 enable ECC Decoder
	// u4DECCODESize = (NAND_SECTOR_SIZE + 8) << 3 + 4 * 13;
               *ECC_DECCNFG |= (ECC_CNFG_NFI  |DEC_CNFG_EMPTY_EN | DEC_CNFG_CORRECT | (1<<8)); // | (u4DECCODESize << DEC_CNFG_CODE_SHIFT));  mark empty en to fix empty page read fail   xiaolei
	STOP_ECC_DEC
	START_ECC_DEC
	// 1.4 set CNFG
	//*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
	*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
         *NFI_CNFG |= ((bUsingDMA)?(CNFG_AHB|CNFG_AHB_BURST):0);				// AHB mode or NFI mode
	
// Ray's todo:		
	*NFI_CNFG |= CNFG_AUTO_FMT_EN;	// if read for verify, use auto format; if read back raw data, don't use it!
	
	// ?? *CNFG |= CNFG_BYTE_RW; ??

	if(fgRandomEn)
	{
                   NFI_RANDOMIZER_DECODE(row_addr, TRUE, fgSSRandomizer, fgPageReload, 0);
	}
	// 2. Issue Read CMD
	// 2.1 read cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd;
	// 2.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ 
                   ret =  S_DA_TIMEOUT;
                   goto end;}		

	// 3. Issue Addr 
	// 3.1 set  row addr, col addr, and num of byte
	// fill 1~4 cycle addr 
	*NFI_ROWADDR = page_addr;
	*NFI_COLADDR = 0;

	// no. of addr cycle 
	col_addr_nob =  (page_size==512)?1:2;	// 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
	row_addr_nob = addr_cycle - col_addr_nob;
	*NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT)) ;
	
	// 3.2 wait til ADDR is completely issued 
	timeout = c_timeout;
	NFI_Wait_Adddress(timeout);
	if(!timeout){
                   ret =  S_DA_TIMEOUT;
                   goto end;}
	// 4. Issue Read Confirm CMD
	if(g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_enable) {
		// 4.1 read confirm 
		#if defined(__NFI_SUPPORT_TLC__)
		if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
			&& NUTL_2P_TLC()
			&& (!bl_region_ing))
			*NFI_CMD = MULTI_PLANE_READ_CMD; //multi plane read
		else
		#endif
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd;
		// 4.2 wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
                   if(!timeout){ 
                       ret =  S_DA_TIMEOUT;
                       goto end;}			
               }
	
	// 5. Wait for Read Buffer is ready
	// wait til device is from busy to ready
	timeout = c_timeout;
	NFI_Wait_Return_Ready(timeout);
               if(!timeout){ 
                   ret =  S_DA_TIMEOUT;
                   goto end;}
	#if defined(__NFI_SUPPORT_TLC__)
	if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
		&& g_pNandInfo->m_dev->m_tlc_setting->normaltlc
		&& NUTL_2P_TLC()
		&& (!bl_region_ing))
	{
		if(!g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) {
           reg_val = DRV_Reg(NFI_CNFG);
           reg_val &= ~CNFG_READ_EN;
           reg_val &= ~CNFG_OP_MODE_MASK;
           reg_val |= CNFG_OP_CUST;
           DRV_WriteReg(NFI_CNFG, reg_val);
           
           if(tlc_wl_info.wl_pre == WL_LOW_PAGE)
           DRV_WriteReg(NFI_CMD, LOW_PG_SELECT_CMD);
           else if(tlc_wl_info.wl_pre == WL_MID_PAGE)
           DRV_WriteReg(NFI_CMD, MID_PG_SELECT_CMD);
           else if(tlc_wl_info.wl_pre == WL_HIGH_PAGE)
           DRV_WriteReg(NFI_CMD, HIGH_PG_SELECT_CMD);
           NFI_Wait_Command(timeout);
                             
           reg_val = DRV_Reg32(NFI_CON);
           reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
           /* issue reset operation */
           DRV_WriteReg32(NFI_CON, reg_val);
		}

		reg_val = DRV_Reg(NFI_CNFG);
        reg_val |= CNFG_READ_EN;
        reg_val &= ~CNFG_OP_MODE_MASK;
        reg_val |= CNFG_OP_READ;
        DRV_WriteReg(NFI_CNFG, reg_val);

		// 2. Issue Read CMD
		// 2.1 read cmd 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd;
		// 2.2 wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout){ 
           ret =  S_DA_TIMEOUT;
           goto end;}		

		// 3. Issue Addr 
		// 3.1 set  row addr, col addr, and num of byte
		// fill 1~4 cycle addr 
		*NFI_ROWADDR = snd_real_row_addr;
		*NFI_COLADDR = 0;
		// no. of addr cycle 
		*NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT)) ;
		
		// 3.2 wait til ADDR is completely issued 
		timeout = c_timeout;
		NFI_Wait_Adddress(timeout);
		if(!timeout){
           ret =  S_DA_TIMEOUT;
           goto end;}

		// 4. Issue Read Confirm CMD
		if(g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_enable) {
			// 4.1 read confirm 
			*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd;
			// 4.2 wait til CMD is completely issued 
			timeout = c_timeout;
			NFI_Wait_Command(timeout);
	                   if(!timeout){ 
	                       ret =  S_DA_TIMEOUT;
	                       goto end;}			
	               }
		
		// 5. Wait for Read Buffer is ready
		// wait til device is from busy to ready
		timeout = c_timeout;
		NFI_Wait_Return_Ready(timeout);
	               if(!timeout){ 
	                   ret =  S_DA_TIMEOUT;
	                   goto end;}
    }
	#endif

	#if defined(__NFI_SUPPORT_TLC__)
    if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
		tlc_snd_phyplane = FALSE;
	#endif
 while(logical_plane_index < logical_plane_num)
               {
                   #if defined(__NFI_SUPPORT_TLC__)
                   if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
                   {
                       if(g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
                       {
                           real_row_addr = NFI_TLC_SetpPlaneAddr(real_row_addr, ((logical_plane_index%2) == 0)?TRUE:FALSE);	
			      snd_real_row_addr = NFI_TLC_SetpPlaneAddr(snd_real_row_addr, ((logical_plane_index%2) == 0)?TRUE:FALSE);	
                       }
                       page_addr = tlc_snd_phyplane?snd_real_row_addr : real_row_addr;
                       colu_addr = logical_plane_index * sectors * NAND_SECTOR_SIZE;
                   }
                   #endif
                   
                   *NFI_STRADDR = ((uint32)(p_data32)) + colu_addr;//QQ
                   
                   #if defined(__NFI_SUPPORT_TLC__)
                   if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
                       &&(g_pNandInfo->m_dev->m_tlc_setting->needchangecolumn || (NUTL_2P_TLC() && (!bl_region_ing))))
                   {
                       NUTL_TLC_ChangeColumn(tlc_snd_phyplane?&snd_real_row_addr : &real_row_addr, ((logical_plane_index%2) ==0)?TRUE:FALSE);
                   }
                   #endif       

		*NFI_CON = (sectors << CON_NFI_SEC_SHIFT);
		     //if(logical_plane_index > 0 )
                   {
                   	  intr = *NFI_INTR;
                       *NFI_INTR_EN = INTR_AHB_DONE_EN;
                       START_ECC_DEC
                   }

	// 6. set NFI_CON
	// 6.1 set burst read by DWORD 
	//*NFI_CON = (CON_NFI_BRD | (sectors << CON_NFI_SEC_SHIFT));
	*NFI_CON |= (CON_NFI_BRD);

	// 6.2 wait til DATA_READ is completely issued 
	//wait til Device is ready
	timeout=c_timeout;
	NFI_Wait_Ready(timeout);
	                   #if defined(__NFI_SUPPORT_TLC__)
                   if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
                       &&(g_pNandInfo->m_dev->m_tlc_setting->needchangecolumn || (NUTL_2P_TLC() && (!bl_region_ing))))
                   {
                       DRV_WriteReg(NFI_TLC_RD_WHR2, (TLC_RD_WHR2_EN | 0x055)); //trigger data sample
                   }
                   #endif

	// 7. Read data
	// read page data 
                   if( S_DONE != (ret=NUTL_FIFO_Read_V3(c_timeout, bUsingDMA, p_data32+colu_addr/4, page_size/logical_plane_num)) ) {
                   //goto end;		
                   }
                   NAND_READ_FDM_DATA(p_spare32+(logical_plane_index*sectors*8/4), sectors);//QQ. need recal FDM in tlc mode
	if((*NFI_STA & STA_READ_EMPTY) != 0)
	{
		//DBG_MSG("[pg] %d empty\n", page_addr);
		if(bad_block_remark_done)
		{
			//DBG_MSG("[pg] reset 0xFF\n");
			memset(p_spare32+(logical_plane_index*sectors*8/4), 0xFF, (sectors*8));
		}
	}

	                   #if defined(__NFI_SUPPORT_TLC__)
                   if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
                       &&(g_pNandInfo->m_dev->m_tlc_setting->needchangecolumn))
                   {
                       DRV_WriteReg(NFI_TLC_RD_WHR2, (0x055)); //disable trigger data sample
                   }
                   #endif
                   
                   logical_plane_index++;
		     if(logical_plane_index == (logical_plane_num >> 1))
			 	tlc_snd_phyplane = TRUE;
               }
#if !defined(__NFI_SUPPORT_TLC__)
              if(g_pNandInfo->m_dev->m_nand_type != NAND_FLASH_TLC)
                   NUTL_MLC_ReadRetry(ret, page_addr, timeout, &readRetryCount, &readRetryVal, &readRetry);	
#endif
	// <<<<  WARNING!! >>>>                                                               
	// 1. You MUST wait until the NFI FIFO is empty.                                      
	//    It means all data in the FIFO had been read out, and then you can start to read 
	//    ECC parity registers.   
	}while(TRUE == readRetry);
#if 1

    if((readRetryCount != 0) && NUTL_DEVICE_MLC())
	{
		readRetryVal = NUTL_READ_RETRY_DEFAULT();

		if(NUTL_READ_RETRY_TYPE() == RTYPE_MICRON)
		{
			NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&readRetryVal,4);	
		}
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_SANDISK)
		{
			NAND_COMMON_Reset_V3(timeout); //to exit
				
			NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(), (uint8*)&readRetryVal,4);	
		}
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA)
		{
			mtk_nand_parapage_rrtry(readRetryVal);
		}
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_TOSHIBA_15NM)
		{
			mtk_nand_parapage_15nm_rrtry(readRetryVal, 1);
		}				
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX)
		{
			HYNIX_Set_RR_Para(readRetryVal);
		}
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_16NM)
		{
			HYNIX_16NM_Set_RR_Para(0, 1);
			//DBG_MSG("readtry OK at page:%d count:%d\n",page_addr, readRetryCount);
		}
		else if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_FDIE)
		{
			HYNIX_FDIE_Set_RR_Para(0, 1);
			//DBG_MSG("readtry OK at page:%d count:%d\n",page_addr, readRetryCount);
		}  
	}   
#endif   	
	
#ifdef DUMP_SPEED
    g_ds_nand_page_read_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_read_time += (tick_end - tick_start);
#endif

end:

	if(NUTL_DEVICE_MLC())
	{
       #if 0
	#if 0
		parePerSector = spare_size/sectors;
		//memcpy(debug_spare, (void*)(spare_reformat+NAND_SECTOR_SIZE+1),7);
		//memcpy((&debug_spare[7]), (void*)(spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+1),3);
		//memcpy((spare_reformat+NAND_SECTOR_SIZE+0) ,debug_spare, 10);
		
		//memcpy(spare_reformat, (spare_reformat+1),7);
		*(spare_reformat+NAND_SECTOR_SIZE+0) = *(spare_reformat+NAND_SECTOR_SIZE+1) ;
		*(spare_reformat+NAND_SECTOR_SIZE+1) = *(spare_reformat+NAND_SECTOR_SIZE+2) ;
		*(spare_reformat+NAND_SECTOR_SIZE+2) = *(spare_reformat+NAND_SECTOR_SIZE+3) ;
		*(spare_reformat+NAND_SECTOR_SIZE+3) = *(spare_reformat+NAND_SECTOR_SIZE+4) ;
		*(spare_reformat+NAND_SECTOR_SIZE+4) = *(spare_reformat+NAND_SECTOR_SIZE+5) ;
		*(spare_reformat+NAND_SECTOR_SIZE+5) = *(spare_reformat+NAND_SECTOR_SIZE+6) ;
		*(spare_reformat+NAND_SECTOR_SIZE+6) = *(spare_reformat+NAND_SECTOR_SIZE+7) ;
		//memcpy((spare_reformat+NAND_SECTOR_SIZE+8), (spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+1),4);
		*(spare_reformat+NAND_SECTOR_SIZE+8) = *(spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+1);
		*(spare_reformat+NAND_SECTOR_SIZE+9) = *(spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+2);
		//*(spare_reformat+NAND_SECTOR_SIZE+10) = *(spare_reformat+2*NAND_SECTOR_SIZE+parePerSector+3);
		#endif
		memcpy((void*)(spare_reformat+NAND_SECTOR_SIZE+8), &dbg_MLC, sizeof(struct debug_info_MLC));
       #endif
		
	}
       #if 0
	else
	{
	
	}
       #endif

	NAND_COMMON_Turn_Off_Randomizer();
	// 8. Stop ECC Decode
	
	if(fgRandomEn)
	{
		NFI_RANDOMIZER_DECODE(row_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		/*
		if((DRV_Reg32(NFI_STA) & STA_READ_EMPTY) > 0) //mark this because cmd setting will trigger NFI to reset nfi_sta sta_read_empty xiaolei
		{
			if((*NFI_CNFG & CNFG_AUTO_FMT_EN) != 0)
				memset((void*)p_data32, 0xFF, page_size);
			else
				memset((void*)p_data32, 0xFF, page_size + spare_size);
			ret = S_DONE;
		}
		*/
	}
       //9.  
       #if defined(__NFI_SUPPORT_TLC__)
	if(g_pNandInfo->m_dev->m_nand_type  == NAND_FLASH_TLC)
	{
		if((g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn )
			&&(0xFF != g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd))
	       {
	           reg_val = DRV_Reg32(NFI_CON);
	           reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
	           /* issue reset operation */
	           DRV_WriteReg32(NFI_CON, reg_val);
			   
	           reg_val = DRV_Reg(NFI_CNFG);
	           reg_val &= ~CNFG_READ_EN;
	           reg_val &= ~CNFG_OP_MODE_MASK;
	           reg_val |= CNFG_OP_CUST;
	           DRV_WriteReg(NFI_CNFG, reg_val);

	           DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd);
	           //NFI_PollingReg_ms((P_U32) NFI_STA, STA_CMD_STATE, TRUE, 1000);
                  NFI_Wait_Command(timeout);

		    //while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
	       }
	}
	#endif
	MSG(DEBUG," qq read end  ,line=%d,function= %s,row_addr=0x%x, p_data32=0x%x, data 0x%x, 0x%x,fgslc=%d \n ",  __LINE__,  __FUNCTION__, row_addr,p_data32,*p_data32, *(p_data32+16384/4 - 1),g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn);
	//comment from xiaolei: 
	//For read retry mechanism, i changed the return value from S_DONE to S_DA_NAND_ECC_2BITS_ERR when it occurs ECC error.
	//This results in Format Fail when Force Erase is not selected.
	//Setting ret as S_DONE here is just a workaround with less risk to meet the orginal logical. 
	ret = S_DONE; 
	
	return ret;
}

//------------------------------------------------------------------------------
// Read From NFI FIFO                                                           
//------------------------------------------------------------------------------

STATUS_E  NUTL_FIFO_Read_V3(
				const uint32  c_timeout
				,const bool bUsingDMA
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,const uint32 data_len
) {
	uint32	timeout = c_timeout;
	uint32	i;
	uint32 	sectors;
	uint32     sector;
	uint32	sector_page_size32;
	uint32	sector_spare_size32;
	uint32	sector_pagespare_size32;
	uint32	dec_mask;
	uint32	ErrNum;
	uint32	ErrBitLoc, ErrByteLoc;
	uint32     sector_shift = NAND_SECTOR_SHIFT;
	uint32     fail_sector = 0;
	//MSG(DEBUG," qq read  ,line=%d,function= %s,, p_data32=0x%x, data 0x%x \n ",  __LINE__,  __FUNCTION__,p_data32,*p_data32);

	sectors				= data_len/NAND_SECTOR_SIZE;
	sector_page_size32 	= (data_len/sectors)>>2;
	sector_spare_size32 = (NUTL_SPARE_PER_SEC_SIZE())>>2;
	sector_pagespare_size32 = sector_page_size32 + sector_spare_size32;
	if(NAND_SECTOR_SIZE == 1024)
		sector_shift+=1;
	if(bUsingDMA) {	// AHB mode
		uint32 reg_val;
		uint32 read_bytes;
		
		timeout = c_timeout;
		// while( (ADDRCNTR_CNTR((*NFI_ADDRCNTR)) < sectors) && (--timeout));

		timeout = 0xffff;
		// while( (read_bytes < data_len) && (timeout>0) ); 

        	while((*NFI_INTR == 0) && (timeout--));
		if(timeout == 0)
			DBG_MSG("[NAND] read timeout\n");

		timeout = 0xffff;
		while (((data_len >> sector_shift) > ((*NFI_BYTELEN & 0x1F000) >> 12))  && (timeout--));
		if(timeout == 0)
			DBG_MSG("[NAND] read timeout\n");
		#if 0
		dec_mask = (1 << sectors) - 1;
		while (dec_mask != (*ECC_DECDONE))
			;
		#else // to fix ecc timeout issue xiaolei
		dec_mask = (1 << (sectors-1)); 
		timeout = 0xffff;
		#if defined(BB_MT8163)
		while (((0 == (dec_mask & (*ECC_DECDONE))) ||(0x01011101 != ((*ECC_DECFSM) & 0x3F3FFF0F))) &&(timeout--))
					;
		#else
		while ((0 == (dec_mask & (*ECC_DECDONE))) ||(0x01010101 != ((*ECC_DECFSM) & 0x7F0F0F0F)) &&(timeout--))
			;
		#endif
		#endif
		*NFI_INTR_EN = 0;
		 fail_sector = 0;
		dbg_MLC.sector_number = sectors;
		for(sector=0;sector < sectors;sector++)
		{
			ErrNum = (DRV_Reg32((ECC_DECENUM0+(sector/4)))>>((sector%4)*8))& ERR_NUM0;
			//DBG_MSG("sector %d, ErrNum %d\n",sector,ErrNum);
			dbg_MLC.sector_bit_flip_num[sector] = ErrNum;
			if(ERR_NUM0 == ErrNum)
			{
				dbg_MLC.sector_ecc_bitmap |= 1<<sector;
				fail_sector++;
				//DBG_MSG("[xiaolei] fail sector %d\n", sector);
				//DBG_MSG("[xiaolei] sector %d, ET 0x%x\n",sector, *NFI_EMPTY_THRESH);
			}
		}
#if 1
		if(fail_sector == sectors)
		{
			if((*NFI_STA & STA_READ_EMPTY) != 0)
			{
				if(bad_block_remark_done)
				memset(p_data32, 0xFF, data_len);
				//DBG_MSG("EMPTY PAGE!\n");
				fail_sector = 0;
				dbg_MLC.sector_ecc_bitmap = 0;
				//DBG_MSG("[xiaolei] FER 0x%x\n", *ECC_DECFER);
			}
		}
#endif
		STOP_ECC_DEC

	}
	else{			// NFI mode - dword access mode
		uint32 sector;
		// read page data 
		 fail_sector = 0;
		for(sector=0; sector < sectors; sector++)
		{
			// 1. Read main area per sector
			for(i=0; i<sector_page_size32; i++) 
			{
				// wait for data ready 
	
				timeout = c_timeout;
                WAIT_NFI_PIO_READY(timeout);
				if( !timeout ) { return S_DA_TIMEOUT;}
					
				//*(p_data32 + sector_page_size32*sector + i) = *NFI_DATAR;
				*(p_data32 + sector_page_size32*sector + i) = *NFI_DATAR;
				
			}
			#if 0
			// 2. Read spare area per sector
			for(i=0; i<sector_spare_size32; i++) 
			{
				// wait for data ready 
	
				// when RD_EMPTY_MASK flag is poll-down, it means data is ready in FIFO at least 4 bytes. 
				timeout = c_timeout;
				WAIT_NFI_PIO_READY(timeout);
				if( !timeout ) { return S_DA_TIMEOUT;}
					
				//*(p_data32 + sector_page_size32*sectors + sector_spare_size32*sector + i) = *NFI_DATAR;
				*(p_data32 + sector_pagespare_size32*sector + sector_page_size32 + i) = *NFI_DATAR;
			}			
                     #endif
			//MT6516 FPGA: Ray's todo: ECC Correct
			// decoder done
			dec_mask = 1 << sector;		  
			while (!(dec_mask & (*ECC_DECDONE)))
				;
#if !defined(__NFI_SUPPORT_TLC__)
#if	defined(__MTK_BB_REG_BASEADDRESS_GROUP12__)  || defined(__MTK_BB_REG_BASEADDRESS_GROUP11__) //for 6572
			ErrNum = (DRV_Reg32((ECC_DECENUM0+(sector/4)))>>((sector%4)*8))& ERR_NUM0;
#else
			ErrNum = (*ECC_DECENUM >> (ERR_NUM_SHIFT * sector)) & ERR_NUM0;
#endif
			if (ErrNum && ErrNum != ERR_NUM0)
			{
				for (i = 0; i < ErrNum; i++)
				{
					#if defined(BB_MT8163)
					if(i < 60)
						ErrBitLoc = *(ECC_DECEL0 + i / 2);
					else
						ErrBitLoc = *(ECC_DECEL30 + i / 2);
                                   if(i%2 == 0)
                                          ErrBitLoc = (ErrBitLoc) & 0x3FFF;
                                   else
                                          ErrBitLoc = (ErrBitLoc>>16) & 0x3FFF;

                                   if(ErrBitLoc < 0x2000)
                                   {
                                       ErrByteLoc = ErrBitLoc/8;
                                       ErrBitLoc  = ErrBitLoc % 8;
                                      (*( (uint8*)(((uint32)p_data32) + sector_page_size32*sector*4 + ErrByteLoc) )) ^= (1 << ErrBitLoc);
                                    }
					#else
					ErrBitLoc = *(ECC_DECEL0 + i / 2);
					#endif
					
				}
			}
			else
			{
				 fail_sector ++;
			}
 #endif
		}
		if((*NFI_STA & STA_READ_EMPTY) != 0)
		{
			if(bad_block_remark_done)//(g_nand_chip.bmt_exist)
			memset(p_data32, 0xFF,(data_len+NUTL_SPARE_SIZE()));
		}


		STOP_ECC_DEC

		
	}
	if( fail_sector == 0)
		return S_DONE;
	else
		return S_DA_NAND_ECC_2BITS_ERR; //uncorrectable ECC  xiaolei
		//return S_DONE;
}
#endif
#else
#error check nand implementation!!
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Block Erase Related Callback Function                                        
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_BlockErase_V3(
				const uint32  row_addr
) {
	uint32	page_size;
	uint32	spare_size;
	uint32	addr_cycle;
	uint32	row_addr_nob, col_addr_nob;
	uint32  c_timeout = NFI_PSTA_TIMEOUT;
	uint32	timeout = NFI_PSTA_TIMEOUT;
	uint32     page_addr = (uint32)row_addr;
#ifdef DUMP_SPEED
    uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif
#if defined(__NFI_SUPPORT_TLC__)
       NFI_TLC_WL_INFO  tlc_wl_info;
       U32 reg_val = 0;
       U32 u4EraseTimes = 1;
	   U32 snd_real_row_addr = 0;
	   U32 page_per_block = NUTL_PAGES_PER_BLOCK();
	   NFI_TLC_WL_INFO  snd_tlc_wl_info;
#endif
	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	addr_cycle = NUTL_ADDR_CYCLE();

	if(S_DA_NAND_BLOCK_IS_LOCKED == NAND_COMMON_CheckWP_V3(timeout))
	{
		DBG_MSG("!!!!!!!!!Warninig !NAND is WP protected!!!!!!!!\n");
		while(1);
	}
       MSG(DEBUG," qq read loop  ,line=%d,function= %s,row_addr=0x%x, %d \n ",  __LINE__,  __FUNCTION__, row_addr, g_pNandInfo->m_dev->m_nand_type);
	
#if defined(__NFI_SUPPORT_TLC__)
      g_pNandInfo->m_eraselevel_support = FALSE;//QQ for debug

      if(g_pNandInfo->m_eraselevel_support)
           u4EraseTimes = 2;
       while(u4EraseTimes)
       {
        
        if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
        {
            if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)//normal tlc
            {
               NFI_TLC_GetMappedWL(row_addr, &tlc_wl_info);//
               if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
                   page_addr = NFI_TLC_GetRowAddr(page_addr);
               else
                   page_addr = NFI_TLC_GetRowAddr(tlc_wl_info.word_line_idx);

			   if(NUTL_2P_TLC() && (!bl_region_ing))
			   {
			       NFI_TLC_GetMappedWL((row_addr+page_per_block), &snd_tlc_wl_info);
				   if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
                       snd_real_row_addr = NFI_TLC_GetRowAddr((row_addr+page_per_block));
				   else
				       snd_real_row_addr = NFI_TLC_GetRowAddr(snd_tlc_wl_info.word_line_idx);
			   }
            }
            else
            {
                page_addr = NFI_TLC_GetRowAddr(page_addr);
            }

            // reset NFI
	NFI_Reset();
            if((g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
                && (0xFF != g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd))
            {
                reg_val = DRV_Reg(NFI_CNFG);
                reg_val &= ~CNFG_READ_EN;
                reg_val &= ~CNFG_OP_MODE_MASK;
                reg_val |= CNFG_OP_CUST;
                DRV_WriteReg(NFI_CNFG, reg_val);
                
                DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
                while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
                
                reg_val = DRV_Reg32(NFI_CON);
                reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
                /* issue reset operation */
                DRV_WriteReg32(NFI_CON, reg_val);
            }
            else
            {
                if(g_pNandInfo->m_eraselevel_support && (2 == u4EraseTimes))
                {
                    reg_val = DRV_Reg(NFI_CNFG);
                    reg_val &= ~CNFG_READ_EN;
                    reg_val &= ~CNFG_OP_MODE_MASK;
                    reg_val |= CNFG_OP_CUST;
                    DRV_WriteReg(NFI_CNFG, reg_val);
                    
                    DRV_WriteReg(NFI_CMD, NOT_KEEP_ERASE_LVL_A19NM_CMD);
                    while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
                    
                    reg_val = DRV_Reg32(NFI_CON);
                    reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
                    /* issue reset operation */
                    DRV_WriteReg32(NFI_CON, reg_val);
                }
            }
        }

		if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
            && g_pNandInfo->m_dev->m_tlc_setting->normaltlc
            && NUTL_2P_TLC()
            && (!bl_region_ing))
		{
			reg_val = DRV_Reg(NFI_CNFG);
            reg_val &= ~CNFG_READ_EN;
            reg_val &= ~CNFG_OP_MODE_MASK;
            reg_val |= CNFG_OP_CUST;
            DRV_WriteReg(NFI_CNFG, reg_val);
            
            DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_cmd_set->m_erase.m_cmd);
            while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

			*NFI_ROWADDR = page_addr;
			*NFI_COLADDR = 0;
			*NFI_ADDRNOB = (3<<ADDR_ROW_NOB_SHIFT) ;
			// wait til ADDR is completely issued 
			timeout = c_timeout;
			NFI_Wait_Adddress(timeout);
			if(!timeout){ return S_DA_TIMEOUT; }

			DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_cmd_set->m_erase.m_cmd);
            while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

			*NFI_ROWADDR = snd_real_row_addr;
			*NFI_COLADDR = 0;
			*NFI_ADDRNOB = (3<<ADDR_ROW_NOB_SHIFT) ;
			// wait til ADDR is completely issued 
			timeout = c_timeout;
			NFI_Wait_Adddress(timeout);
			if(!timeout){ return S_DA_TIMEOUT; }

			// block erase confirm 
			*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_erase_cnf.m_cmd;
			// wait til CMD is completely issued 
			timeout = c_timeout;
			NFI_Wait_Command(timeout);
			if(!timeout){ return S_DA_TIMEOUT; }	
			/*wait til ready, add by jiequn,2011.11.8*/
			timeout = c_timeout;
			NFI_Wait_Ready(timeout);
		    if(!timeout){ return S_DA_TIMEOUT; }
		}
		else
#endif
{
	// set NFI_CNFG
	*NFI_CNFG = BOOTROM_ERASE_CNFG;

	// block erase cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_erase.m_cmd;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }	

#if CFG_2CS_NAND
	if(g_bTricky_CS)
	{
		page_addr = NAND_COMMON_CS_ON(1, page_addr);
	}
#endif
	// fill 1~4 cycle addr, erase command only fill row address, so column bits shift is unnecessary 
	*NFI_ROWADDR = page_addr;
	*NFI_COLADDR = 0;

	// no. of addr cycle 
	col_addr_nob =  (page_size==512)?1:2;	// 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
	row_addr_nob = addr_cycle - col_addr_nob;
	*NFI_ADDRNOB = (row_addr_nob<<ADDR_ROW_NOB_SHIFT) ;	// Block Erase don't need COL_ADDR_NOB

	
	// wait til ADDR is completely issued 
	timeout = c_timeout;
	NFI_Wait_Adddress(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }

	// block erase confirm 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_erase_cnf.m_cmd;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }	
	/*wait til ready, add by jiequn,2011.11.8*/
	timeout = c_timeout;
	NFI_Wait_Ready(timeout);
    if(!timeout){ return S_DA_TIMEOUT; }
}
	
#if defined(__NFI_SUPPORT_TLC__)
             //if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
                 u4EraseTimes--;
       }
#endif
	/*
	do
	{
		status = NAND_COMMON_ReadStatus_V3(timeout);
	}while(S_DA_IN_PROGRESS == status);

	if(status == S_DA_NAND_UNKNOWN_ERR)
	{
		DBG_MSG("[xiaolei] eb fail 0x%x\n",row_addr);
		return S_DA_NAND_ERASE_FAILED;
	}
	if(status != S_DONE)
	{
		DBG_MSG("[xiaolei] eb fail 2  0x%x\n",row_addr);
		return status;
	}
	*/
	
#ifdef DUMP_SPEED
    g_ds_nand_block_erase_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_block_erase_time += (tick_end - tick_start);
#endif
#if defined(__NFI_SUPPORT_TLC__)
       if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
           && (g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)) //hynix tlc need doule check
       {
       
           if((g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
               &&(0xFF != g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd))
           {
               reg_val = DRV_Reg32(NFI_CON);
               reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
               /* issue reset operation */
               DRV_WriteReg32(NFI_CON, reg_val);
               
               reg_val = DRV_Reg(NFI_CNFG);
               reg_val &= ~CNFG_READ_EN;
               reg_val &= ~CNFG_OP_MODE_MASK;
               reg_val |= CNFG_OP_CUST;
               DRV_WriteReg(NFI_CNFG, reg_val);
               
               DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd);
               //NFI_PollingReg_ms((P_U32) NFI_STA, STA_CMD_STATE, TRUE, 1000);
               NFI_Wait_Command(timeout);
               //while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
           }
       }
#endif
     MSG(DEBUG,"line=%d,function= %s,row_addr=0x%x, ,fgslc=%d\n ",  __LINE__,  __FUNCTION__, row_addr, g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn);

	return S_DONE;
}
// Page Program Callback Function                                               
//------------------------------------------------------------------------------
#ifndef NFI_MEM_OPTIMIZE
STATUS_E  NAND_COMMON_PageProgram_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {
	uint32		i;
	uint32		page_size;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		row_addr_nob, col_addr_nob;	
	uint32		sectors;
	uint32		* fdm;
	uint32		io_interface;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
	bool		bUsingDMA=TRUE;
	uint32     page_addr = (uint32)row_addr;
#ifdef DUMP_SPEED
    uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif


	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();
	sectors = NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;
	// 1. Init NFI
	// 1.1 reset NFI

	if(S_DA_NAND_BLOCK_IS_LOCKED == NAND_COMMON_CheckWP_V3(timeout))
	{
		DBG_MSG("!!!!!!!!!Warninig !NAND is WP protected!!!!!!!!\n");
		while(1);
	}

	if(reset_fdm_and_ecc)
	{
	    if(page_addr < second_part_start_page)
	    {
	    	mtk_nand_reset_fdm_ecc(FALSE);
	    }
		else
		{
			mtk_nand_reset_fdm_ecc(TRUE);
		}
	}
	
	NFI_Reset();
#if CFG_2CS_NAND
	if(g_bTricky_CS)
	{
		page_addr = NAND_COMMON_CS_ON(1, page_addr);
	}
#endif
#ifdef DISABLE_NFI_DMA
	bUsingDMA = FALSE;
#else
	bUsingDMA = TRUE;
#endif
	bUsingDMA = TRUE;

	// 1.2 !! MT6516 FPGA: Ray's todo: write FDM
	fdm = (uint32*)p_data32 + (page_size>>2);
	
	for(i=0; i<8; i++) //bug here.xiaolei
	{
		*(NFI_FDM0L+i) = *(fdm+i);
	}
	
	// 1.3 write source address
	*NFI_STRADDR = (uint32)p_data32;
	
	// 1.4 enable ECC Encode
	*ECC_ENCCNFG |= ECC_CNFG_NFI;
	//WAIT_ENC_READY
	//START_ECC_ENC	
	timeout = c_timeout;
	WAIT_ENC_READY_WITH_TIMEOUT(timeout);
	STOP_ECC_ENC
	START_ECC_ENC
	
	// 1.5 set NFI_CNFG
	*NFI_CNFG = (BOOTROM_WRITE_CNFG | CNFG_HW_ECC_EN | CNFG_AUTO_FMT_EN);	// write, HW ECC auto Encode, Auto FMT
	*NFI_CNFG |= ((bUsingDMA)?(CNFG_AHB|CNFG_AHB_BURST):0);									// AHB mode or NFI mode

	// ?? *CNFG |= CNFG_BYTE_RW; ??

	// 2. Issue 1st Half Write CMD
	// in most 512 page size NAND flash, you have to setup destination pointer to 1st half area 
	if(g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_enable) {
		// 2.1 1st half program cmd 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_cmd;
		// 2.2 wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout){ return S_DA_TIMEOUT; }	
			
		// 2.3 Reset for ambigous of read command and 1st-half program command
		NFI_Reset();
	}

if(fgRandomEn)
{
	NFI_RANDOMIZER_ENCODE(page_addr, TRUE, fgSSRandomizer, fgPageReload, 0);
}
	// 3. Issue Program CMD
	// 3.1 program cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd;
	// 3.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}

		return S_DA_TIMEOUT; }
		
	// 4. Issue Addr 
	// 4.1 set  row addr, col addr, and num of byte
	// fill 1~4 cycle addr 
	*NFI_ROWADDR = page_addr;
	*NFI_COLADDR = 0;

	// no. of addr cycle 
	col_addr_nob =  (page_size==512)?1:2;	// 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
	row_addr_nob = addr_cycle - col_addr_nob;
	*NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT)) ;

	// 4.2 wait til ADDR is completely issued 
	timeout = c_timeout;
	NFI_Wait_Adddress(timeout);
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }
	
	// 5. set NFI_CON
	// set burst program by DWORD 
	*NFI_CON = (CON_NFI_BWR | (sectors << CON_NFI_SEC_SHIFT));

	// 6. Program data
	if( S_DONE != (ret=NUTL_FIFO_Write_V3(bUsingDMA, p_data32, page_size, NUTL_PROGRAM_PAGE_ONLY)) ) {
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return ret;
	}
 
	// <<<<  WARNING!! >>>>                                                              
	// 1. You MUST read parity registers before issue program confirm (0x10) command.    
	//    Since the parity registers will be clean by NFI after issue program confirm.   
	// 2. You MUST wait until the NFI FIFO is empty!                                     
	//    It means all data in the FIFO had been written to NAND flash, and then you can 
	//    start to read ECC parity registers.                                            
	
	//ray-MT6238
	timeout = c_timeout;

	// 7. Stop ECC Encode
	timeout = c_timeout;
	WAIT_ENC_READY_WITH_TIMEOUT(timeout);
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }

	timeout = c_timeout;
	while( FIFO_WR_REMAIN( *NFI_FIFOSTA) && (--timeout) );
	STOP_ECC_ENC
	
	// 8. Issue Program Confirm CMD
	// 8.1 program confirm 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd;
	// 8.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }

	// 9. Wait for Program Buffer is ready
	timeout = c_timeout;
	NAND_COMMON_Turn_Off_Randomizer();
	while( (S_DA_IN_PROGRESS ==(ret = NAND_COMMON_ReadStatus_V3(c_timeout))) && (timeout--));
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }

	// 10. Stop ECC Encode
	//WAIT_ENC_READY
	STOP_ECC_ENC

	// 11.
	if(S_DONE != ret){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_NAND_UNKNOWN_ERR;}

#ifdef DUMP_SPEED
    g_ds_nand_page_program_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_program_time += (tick_end - tick_start);
#endif

	if(fgRandomEn)
	{
		NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
	}
	
	return S_DONE;
}
#else
STATUS_E  NAND_COMMON_PageProgramBad_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_data32 /* MUST be 32bits alignment addr */
				,const uint32 *p_spare32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {
	uint32		i;
	uint32		page_size;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		row_addr_nob, col_addr_nob;	
	uint32		sectors;
	uint32		* fdm;
	uint32		io_interface;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	STATUS_E	ret=S_DONE;
	bool		bUsingDMA=TRUE;
	uint32     page_addr = (uint32)row_addr;
	bool fgOrigEnslc = g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn;
        //DBG_MSG("line=%d,function= %s,row_addr=0x%x,fgslc=%d , \n ",  __LINE__,  __FUNCTION__, row_addr, g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn);
       MSG(DEBUG," qq   ,line=%d,function= %s,row_addr=0x%x, p_data32=0x%x, data 0x%x \n ",  __LINE__,  __FUNCTION__, row_addr,p_data32,*p_data32);

#if defined(__NFI_SUPPORT_TLC__)
       NFI_TLC_WL_INFO  tlc_wl_info;
       U32 reg_val = 0;       
#endif
       fgRandomEn = FALSE;
	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();
	sectors = NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;
	
       if(!fgOrigEnslc)
       {
           page_addr = page_addr/3;
       }
	   g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn = TRUE;
	NFI_Reset();

#if CFG_2CS_NAND
	if(g_bTricky_CS)
	{
		page_addr = NAND_COMMON_CS_ON(1, page_addr);
	}
#endif

	//1.21 for TLC
#if defined(__NFI_SUPPORT_TLC__)
       if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
       {
           if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)//normal tlc
           {
               NFI_TLC_GetMappedWL(row_addr, &tlc_wl_info);//SLC view, page addr == WL index,mark it
               if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn )
                   page_addr  = NFI_TLC_GetRowAddr(page_addr);
               else
                   page_addr  = NFI_TLC_GetRowAddr(tlc_wl_info.word_line_idx);                
           }
           else
           {
                page_addr = NFI_TLC_GetRowAddr(row_addr);
           }
           
           //if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) // slc mode
           {
               //if(!g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
               {
                   if(0xFF != g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd)
                   {
                                 //MSG(DEBUG,"line=%d,function= %s \n ",  __LINE__,  __FUNCTION__);

                       reg_val = DRV_Reg(NFI_CNFG);
                       reg_val &= ~CNFG_READ_EN;
                       reg_val &= ~CNFG_OP_MODE_MASK;
                       reg_val |= CNFG_OP_CUST;
                       DRV_WriteReg(NFI_CNFG, reg_val);
                       
                       DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
                       while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
                       
                       reg_val = DRV_Reg32(NFI_CON);
                       reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
                       /* issue reset operation */
                       DRV_WriteReg32(NFI_CON, reg_val); 
                   }
               }
           }
          
           
       }
#endif
	*NFI_CNFG = (BOOTROM_WRITE_CNFG );	// write, HW ECC auto Encode, Auto FMT

	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd;
		
	*NFI_ROWADDR = page_addr;
	*NFI_COLADDR = 0;

	// no. of addr cycle 
	col_addr_nob =  (page_size==512)?1:2;	// 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
	row_addr_nob = addr_cycle - col_addr_nob;
	*NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT)) ;


	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd;
      
      while( (S_DA_IN_PROGRESS ==(ret = NAND_COMMON_ReadStatus_V3(c_timeout))) && (timeout--));
	if(!timeout){
		      fgRandomEn = TRUE;
		ret= S_DA_TIMEOUT; }

#if defined(__NFI_SUPPORT_TLC__)
      if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
      {
          if( 0xFF != g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd)
          {
              reg_val = DRV_Reg32(NFI_CON);
              reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
              /* issue reset operation */
              DRV_WriteReg32(NFI_CON, reg_val);
              
              reg_val = DRV_Reg(NFI_CNFG);
              reg_val &= ~CNFG_READ_EN;
              reg_val &= ~CNFG_OP_MODE_MASK;
              reg_val |= CNFG_OP_CUST;
              DRV_WriteReg(NFI_CNFG, reg_val);
              
              DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd);
              NFI_Wait_Command(timeout);
              
          }
      }
#endif
      fgRandomEn = TRUE;
      g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn = fgOrigEnslc;

	return ret;
}
STATUS_E  NAND_COMMON_PageProgram_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_data32 /* MUST be 32bits alignment addr */
				,const uint32 *p_spare32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {
	uint32		i;
	uint32		page_size;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		row_addr_nob, col_addr_nob;	
	uint32		sectors;
	uint32		* fdm;
	uint32		io_interface;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
	bool		bUsingDMA=TRUE;
	uint32     page_addr = (uint32)row_addr;
#ifdef DUMP_SPEED
    uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif

#if defined(__NFI_SUPPORT_TLC__)
       NFI_TLC_WL_INFO  tlc_wl_info;
       U32 reg_val = 0;
       //bool  fgOriSlcopmodeEn = g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn ;
       //if(NULL == p_data32)
       {
         //  g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn = TRUE;
       }
#endif
        //     MSG(DEBUG,"line=%d,function= %s \n ",  __LINE__,  __FUNCTION__);
       MSG(DEBUG," qq   ,line=%d,function= %s,row_addr=0x%x, p_data32=0x%x, data 0x%x, fgslc = %d \n ",  __LINE__,  __FUNCTION__, row_addr,p_data32,*p_data32, g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn);

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();
	sectors = NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;

	if(S_DA_NAND_BLOCK_IS_LOCKED == NAND_COMMON_CheckWP_V3(timeout))
	{
		DBG_MSG("!!!!!!!!!Warninig !NAND is WP protected!!!!!!!!\n");
		while(1);
	}

	//DBG_MSG("PPgm: 0x%x\n", row_addr);
	if(reset_fdm_and_ecc)
	{
	    if(page_addr < second_part_start_page)
	    {
	    	mtk_nand_reset_fdm_ecc(FALSE);
	    }
		else
		{
			mtk_nand_reset_fdm_ecc(TRUE);
		}
	}
	
	// 1. Init NFI
	// 1.1 reset NFI
	NFI_Reset();
#if CFG_2CS_NAND
	if(g_bTricky_CS)
	{
		page_addr = NAND_COMMON_CS_ON(1, page_addr);
	}
#endif
#ifdef DISABLE_NFI_DMA
	bUsingDMA = FALSE;
#else
	bUsingDMA = TRUE;
#endif
	bUsingDMA = TRUE;
	
	if(use_HWECC)
	{
		// 1.2 !! MT6516 FPGA: Ray's todo: write FDM
		fdm = (uint32*)p_spare32;
	
		for(i=0; i<8; i++) //bug here.xiaolei
		{
			*(NFI_FDM0L+i) = *(fdm+i);
#if 0// defined (__NFI_SUPPORT_TLC__)
              if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
              {
                   *(NFI_FDM0L +(i << 1))  = *(fdm++);
                   *(NFI_FDM0M+(i << 1))  = *(fdm++);
                   DBG_MSG("fdm1:0x%x \n",  fdm);
                   fdm -= 2;
                   reg_val = ((uint32)fdm);
                   reg_val += (*NFI_PAGEFMT &PAGEFMT_FDM_MASK)>>8;
                   fdm = reg_val;
                   DBG_MSG("fdm1:0x%x \n",  fdm);
              }

#endif
	}
	}
	//1.21 for TLC
#if defined(__NFI_SUPPORT_TLC__)
       if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
       {
           if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)//normal tlc
           {
               NFI_TLC_GetMappedWL(row_addr, &tlc_wl_info);//SLC view, page addr == WL index,mark it
               if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn )
                   page_addr  = NFI_TLC_GetRowAddr(page_addr);
               else
                   page_addr  = NFI_TLC_GetRowAddr(tlc_wl_info.word_line_idx);
                if(g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
                {
                    page_addr =  NFI_TLC_SetpPlaneAddr(page_addr, tlc_lg_left_plane);
                    page_size  =  page_size/2;
                    sectors      =  sectors/2;
                    spare_size =  spare_size/2;
                    *NFI_PAGEFMT = (*NFI_PAGEFMT & ~(0x3));    
                   if(4096 == page_size) {
                       *NFI_PAGEFMT |= PAGEFMT_4K;
                   }
                   else if(8192 == page_size) {
                       *NFI_PAGEFMT |= PAGEFMT_8K_1KS;
                   }
                   else if(16384 == page_size) {
                       *NFI_PAGEFMT |= PAGEFMT_16K_1KS;
                   }
                }
				if(NUTL_2P_TLC() && (!bl_region_ing))
				{
					page_size /= 2;
					sectors /= 2;
					if(32768 == g_pNandInfo->m_dev->m_hw_info.m_page_size) {
                       *NFI_PAGEFMT |= PAGEFMT_16K_1KS;
                    }
				}
           }
           else
           {
                page_addr = NFI_TLC_GetRowAddr(row_addr);
           }
           
           if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) // slc mode
           {
               if(((!g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn) || tlc_lg_left_plane)
			   	   && (!tlc_snd_phyplane))
               {
                   if(0xFF != g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd)
                   {
                                 //MSG(DEBUG,"line=%d,function= %s \n ",  __LINE__,  __FUNCTION__);

                       reg_val = DRV_Reg(NFI_CNFG);
                       reg_val &= ~CNFG_READ_EN;
                       reg_val &= ~CNFG_OP_MODE_MASK;
                       reg_val |= CNFG_OP_CUST;
                       DRV_WriteReg(NFI_CNFG, reg_val);
                       
                       DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
                       while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
                       
                       reg_val = DRV_Reg32(NFI_CON);
                       reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
                       /* issue reset operation */
                       DRV_WriteReg32(NFI_CON, reg_val); 
                   }
               }
           }
           else //tlc mode
           {
               if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)
               {		
                   reg_val = DRV_Reg(NFI_CNFG);
                   reg_val &= ~CNFG_READ_EN;
                   reg_val &= ~CNFG_OP_MODE_MASK;
                   reg_val |= CNFG_OP_CUST;
                   DRV_WriteReg(NFI_CNFG, reg_val);
                   
                   if(PROGRAM_1ST_CYCLE == tlc_program_cycle)
                   {
                       DRV_WriteReg(NFI_CMD, PROGRAM_1ST_CYCLE_CMD);
                       while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
                   }
                   else if(PROGRAM_2ND_CYCLE == tlc_program_cycle)
                   {
                       DRV_WriteReg(NFI_CMD, PROGRAM_2ND_CYCLE_CMD);
                       while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
                   }
                   
                   if(tlc_wl_info.wl_pre == WL_LOW_PAGE)
                       DRV_WriteReg(NFI_CMD, LOW_PG_SELECT_CMD);
                   else if(tlc_wl_info.wl_pre == WL_MID_PAGE)
                       DRV_WriteReg(NFI_CMD, MID_PG_SELECT_CMD);
                   else if(tlc_wl_info.wl_pre == WL_HIGH_PAGE)
                       DRV_WriteReg(NFI_CMD, HIGH_PG_SELECT_CMD);
                   
                   while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
                   
                   reg_val = DRV_Reg32(NFI_CON);
                   reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
                   /* issue reset operation */
                   DRV_WriteReg32(NFI_CON, reg_val); 
               }
           }
       }
#endif

	
	// 1.3 write source address
	*NFI_STRADDR = (uint32)p_data32;

	if(use_HWECC)
	{
		// 1.4 enable ECC Encode
		*ECC_ENCCNFG |= ECC_CNFG_NFI;
	}
	//WAIT_ENC_READY
	//START_ECC_ENC	
	timeout = c_timeout;
	WAIT_ENC_READY_WITH_TIMEOUT(timeout);
	STOP_ECC_ENC
	if(use_HWECC)
	{
		START_ECC_ENC
	}
	
	// 1.5 set NFI_CNFG
	if(use_HWECC)
		*NFI_CNFG = (BOOTROM_WRITE_CNFG | CNFG_HW_ECC_EN | CNFG_AUTO_FMT_EN);	// write, HW ECC auto Encode, Auto FMT
	else
		*NFI_CNFG = (BOOTROM_WRITE_CNFG);	// write, HW ECC auto Encode, Auto FMT	
	*NFI_CNFG |= ((bUsingDMA)?(CNFG_AHB|CNFG_AHB_BURST):0);									// AHB mode or NFI mode

	// ?? *CNFG |= CNFG_BYTE_RW; ??

	// 2. Issue 1st Half Write CMD
	// in most 512 page size NAND flash, you have to setup destination pointer to 1st half area 
	if(g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_enable) {
		// 2.1 1st half program cmd 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_cmd;
		// 2.2 wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout){ return S_DA_TIMEOUT; }	
			
		// 2.3 Reset for ambigous of read command and 1st-half program command
		NFI_Reset();
	}

if(fgRandomEn)
{
           NFI_RANDOMIZER_ENCODE(row_addr, TRUE, fgSSRandomizer, fgPageReload, 0);
}
	// 3. Issue Program CMD
	// 3.1 program cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd;
	// 3.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(row_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}

		return S_DA_TIMEOUT; }
		
	// 4. Issue Addr 
	// 4.1 set  row addr, col addr, and num of byte
	// fill 1~4 cycle addr 
	*NFI_ROWADDR = page_addr;
	*NFI_COLADDR = 0;

	// no. of addr cycle 
	col_addr_nob =  (page_size==512)?1:2;	// 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
	row_addr_nob = addr_cycle - col_addr_nob;
	*NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT)) ;

	// 4.2 wait til ADDR is completely issued 
	timeout = c_timeout;
	NFI_Wait_Adddress(timeout);
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(row_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }
	
#if defined(__NFI_SUPPORT_TLC__)
       if(g_pNandInfo->m_dev->m_nand_type  == NAND_FLASH_TLC)
       {
           reg_val = DRV_Reg32(NFI_DEBUG_CON3);
           reg_val |= 0x4000; //enable ce hold
           DRV_WriteReg(NFI_DEBUG_CON3, reg_val);
       }
#endif

	// 5. set NFI_CON
	// set burst program by DWORD 
	*NFI_CON = (CON_NFI_BWR | (sectors << CON_NFI_SEC_SHIFT));

	// 6. Program data
       if(p_data32 != NULL)//  for TLC sandisk flash , bad block detect
	if( S_DONE != (ret=NUTL_FIFO_Write_V3(bUsingDMA, p_data32, page_size,spare_size,  NUTL_PROGRAM_PAGE_ONLY)) ) {
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(row_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return ret;
	}
 
	// <<<<  WARNING!! >>>>                                                              
	// 1. You MUST read parity registers before issue program confirm (0x10) command.    
	//    Since the parity registers will be clean by NFI after issue program confirm.   
	// 2. You MUST wait until the NFI FIFO is empty!                                     
	//    It means all data in the FIFO had been written to NAND flash, and then you can 
	//    start to read ECC parity registers.                                            
	
	//ray-MT6238
	timeout = c_timeout;

	// 7. Stop ECC Encode
	if(use_HWECC)
	{
		timeout = c_timeout;
		WAIT_ENC_READY_WITH_TIMEOUT(timeout);
		if(!timeout){
			if(fgRandomEn)
			{
			NFI_RANDOMIZER_ENCODE(row_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
			}
			return S_DA_TIMEOUT; }
	}
	timeout = c_timeout;
	while( FIFO_WR_REMAIN( *NFI_FIFOSTA) && (--timeout) );
	if(use_HWECC)
	{
		STOP_ECC_ENC
	}
#if defined(__NFI_SUPPORT_TLC__)
       if( (g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC) && !tlc_lg_left_plane)
       {
           reg_val = DRV_Reg32(NFI_DEBUG_CON3);
           reg_val &= (~0x4000); //disable ce hold
           DRV_WriteReg(NFI_DEBUG_CON3, reg_val);
       }
#endif

	// 8. Issue Program Confirm CMD
	// 8.1 program confirm 
#if defined(__NFI_SUPPORT_TLC__)
      if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
      {
          if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)//normal tlc
          {
              if((g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn) && tlc_lg_left_plane)
              {
                  DRV_WriteReg(NFI_CMD, PROGRAM_LEFT_PLANE_CMD);
              }
              else
              {
				if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
		        {
		        	if(NUTL_2P_TLC() && (!tlc_snd_phyplane) && (!bl_region_ing))
		        	{
		        		DRV_WriteReg(NFI_CMD, PROGRAM_LEFT_PLANE_CMD);
		        	}
					else
					{
						DRV_WriteReg(NFI_CMD, PROG_DATA_CMD);
					}
				}
  				else if(tlc_wl_info.wl_pre == WL_HIGH_PAGE)
  			    {
  			    	if(NUTL_2P_TLC() && (!tlc_snd_phyplane) && (!bl_region_ing))
  		        	{
  		        		DRV_WriteReg(NFI_CMD, PROGRAM_LEFT_PLANE_CMD);
  		        	}
  					else
  					{
  						if(tlc_cache_program)
  				        {
  				        	DRV_WriteReg(NFI_CMD, CACHE_PROG_CMD);
  				        }
  						else
  						{
  							DRV_WriteReg(NFI_CMD, PROG_DATA_CMD);
  						}
  					}
  				}
  			    else
  		        {
  		        	if(NUTL_2P_TLC() && (!tlc_snd_phyplane) && (!bl_region_ing))
  		        	{
  		        		DRV_WriteReg(NFI_CMD, PROGRAM_LEFT_PLANE_CMD);
  		        	}
  					else
  					{
  						DRV_WriteReg(NFI_CMD, PROGRAM_RIGHT_PLANE_CMD);
  					}
  		        }
            }
          }
          else //micron tlc
              DRV_WriteReg(NFI_CMD, PROG_DATA_CMD);
      }
      else
#endif
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd;
	// 8.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(row_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }

	// 9. Wait for Program Buffer is ready
	timeout = c_timeout;
	NAND_COMMON_Turn_Off_Randomizer();
	if( (g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn) && !tlc_lg_left_plane)
       {
	while( (S_DA_IN_PROGRESS ==(ret = NAND_COMMON_ReadStatus_V3(c_timeout))) && (timeout--));
       }
	if(!timeout){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(row_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_TIMEOUT; }

	if(use_HWECC)
	{
		// 10. Stop ECC Encode
		//WAIT_ENC_READY
		STOP_ECC_ENC
	}
	
	// 11.
	if(S_DONE != ret){
		if(fgRandomEn)
		{
			NFI_RANDOMIZER_ENCODE(row_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
		}
		return S_DA_NAND_UNKNOWN_ERR;}

#ifdef DUMP_SPEED
    g_ds_nand_page_program_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_program_time += (tick_end - tick_start);
#endif

	if(fgRandomEn)
	{
		NFI_RANDOMIZER_ENCODE(row_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
	}
#if defined(__NFI_SUPPORT_TLC__)
      if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
      {
          if( (g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
               &&(0xFF != g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd)
               && !tlc_lg_left_plane)
          {
              reg_val = DRV_Reg32(NFI_CON);
              reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
              /* issue reset operation */
              DRV_WriteReg32(NFI_CON, reg_val);
              
              reg_val = DRV_Reg(NFI_CNFG);
              reg_val &= ~CNFG_READ_EN;
              reg_val &= ~CNFG_OP_MODE_MASK;
              reg_val |= CNFG_OP_CUST;
              DRV_WriteReg(NFI_CNFG, reg_val);
              
              DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd);
              NFI_Wait_Command(timeout);
              
          }
      }
#endif
	
	return S_DONE;
}
#endif

//------------------------------------------------------------------------------
// Page+Spare Program Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E NAND_COMMON_PageSpareProgram_V3(uint32 row_addr,
                                         const uint32 *p_page_spare)
{
    const uint32 page_size = NUTL_PAGE_SIZE();
    const uint32 spare_size = NUTL_SPARE_SIZE();
    const uint32 addr_cycle = NUTL_ADDR_CYCLE();
    const uint32 sectors = NUTL_PAGE_SIZE() / NAND_SECTOR_SIZE;
    uint32 row_addr_nob, col_addr_nob;
    uint32 timeout;
    STATUS_E ret;
    uint32     page_addr = (uint32)row_addr;

    NFI_Reset();
    *NFI_CNFG = BOOTROM_WRITE_CNFG;
    // Issue 1st Half Write CMD:
    //   For most 512 page size NAND flash, we have to set destination pointer
    //   to 1st half area.
       if(reset_fdm_and_ecc)
	{
	    if(page_addr < second_part_start_page)
	    {
	    	mtk_nand_reset_fdm_ecc(FALSE);
	    }
		else
		{
			mtk_nand_reset_fdm_ecc(TRUE);
		}
	}
#if CFG_2CS_NAND
	if (g_bTricky_CS)
	{
		page_addr = NAND_COMMON_CS_ON(1, page_addr);
	}
#endif
    if(g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_enable)
    {
        // 1st half program cmd 
        *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_cmd;

        // Wait til CMD is completely issued
        timeout = NUTL_DEFAULT_TIMEOUT;
        NFI_Wait_Command(timeout);

        if (!timeout)
        {
            return S_DA_TIMEOUT;
        }
            
        // Reset for ambiguous of read command and 1st-half program command
        NFI_Reset();
    }

    //
    // Issue Program CMD
    //

    if(fgRandomEn)
    {
        NFI_RANDOMIZER_ENCODE(page_addr, TRUE, fgSSRandomizer, fgPageReload, 0);
    }
	
    // Program cmd 
    *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd;

    // Wait til CMD is completely issued 
    timeout = NUTL_DEFAULT_TIMEOUT;
    NFI_Wait_Command(timeout);

    if (!timeout)
    {
        if(fgRandomEn)
	{
		NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
	}
        return S_DA_TIMEOUT;
    }

    //
    // Issue address
    //

    // Set row addr, col addr, and num of bytes
    // fill 1~4 cycle addr 
    *NFI_ROWADDR = page_addr;
    *NFI_COLADDR = 0;

    // no. of addr cycle 
    col_addr_nob = (page_size==512)?1:2; // 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
    row_addr_nob = addr_cycle - col_addr_nob;
    *NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT));

    // Wait til ADDR is completely issued 
    timeout = NUTL_DEFAULT_TIMEOUT;
    NFI_Wait_Adddress(timeout);

    if (!timeout)
    {
        if(fgRandomEn)
	{
		NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
	}
        return S_DA_TIMEOUT;
    }

    // Set NFI_CON
    // set burst program by DWORD 
    *NFI_CON = (CON_NFI_BWR | (sectors << CON_NFI_SEC_SHIFT));

	// Program data
    if (S_DONE != (ret=NUTL_FIFO_Write_V3(FALSE, p_page_spare, page_size, spare_size, NUTL_PROGRAM_PAGE_SPARE)))
    {
        if(fgRandomEn)
	{
		NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
	}
        return ret;
    }

    //
	// Issue Program Confirm CMD
    //

    // Program confirm 
    *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd;

    // Wait til CMD is completely issued 
    timeout = NUTL_DEFAULT_TIMEOUT;
    NFI_Wait_Command(timeout);

    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }

    // Wait for Program Buffer is ready
    timeout = NUTL_DEFAULT_TIMEOUT;
	NAND_COMMON_Turn_Off_Randomizer();
    while ((S_DA_IN_PROGRESS == (ret=NAND_COMMON_ReadStatus_V3(NUTL_DEFAULT_TIMEOUT))) && (timeout--));

    if (!timeout)
    {
        if(fgRandomEn)
	{
		NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
	}
        return S_DA_TIMEOUT;
    }

    if (S_DONE != ret)
    {
        if(fgRandomEn)
	{
		NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
	}
        return S_DA_NAND_UNKNOWN_ERR;
    }

    if(fgRandomEn)
    {
        NFI_RANDOMIZER_ENCODE(page_addr, FALSE, fgSSRandomizer, fgPageReload, 0);
    }
    return S_DONE;
}

//------------------------------------------------------------------------------
// Write To NFI FIFO                                                            
//------------------------------------------------------------------------------

STATUS_E  NUTL_FIFO_Write_V3(const bool bUsingDMA,
                             const uint32 *p_data32, /* MUST be 32bits alignment addr */
                             const uint32 page_size, /* MUST be 32bits alignment addr */
                             const uint32   spare_size, /* MUST be 32bits alignment addr */
                             NUTL_ProgramFlag_E flag)
{
    uint32 data_size_in_dwords;

    switch (flag)
    {
        case NUTL_PROGRAM_PAGE_ONLY:
            data_size_in_dwords = (page_size) >> 2;
            break;

        case NUTL_PROGRAM_PAGE_SPARE:
            data_size_in_dwords = (page_size + spare_size) >> 2;//QQ, need check
            break;

        default:
            while (1);
    }

    if (bUsingDMA)
    {
        // AHB mode

        const uint32 sectors = page_size / NAND_SECTOR_SIZE;
        uint32 timeout = NUTL_DEFAULT_TIMEOUT;

        while ((ADDRCNTR_CNTR((*NFI_ADDRCNTR)) < sectors) && (--timeout));
    }
    else
    {
        // NFI mode - dword access mode

        uint32 timeout;
        uint32 delay = 0x1FF;
        uint32 i;

        for (i=0; i<data_size_in_dwords; ++i)
        {
            timeout = NUTL_DEFAULT_TIMEOUT;
            WAIT_NFI_PIO_READY(timeout);                         

            if (!timeout)
            {
                return S_DA_TIMEOUT;
            }

            *NFI_DATAW = *(p_data32 + i);
        }

        // Wait for data ready 
        while(delay--);

        timeout = NUTL_DEFAULT_TIMEOUT;
        while ((FIFO_WR_REMAIN (*NFI_FIFOSTA)) && (--timeout));

        if (!timeout)
        {
            return S_DA_TIMEOUT;
        }
    }

	return S_DONE;
}

//------------------------------------------------------------------------------
// Spare Read Callback Function                                                 
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_SpareRead_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_spare32 /* MUST be 32bits alignment addr */
) {
	uint32		page_size;
//	uint32		spare_addr;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		io_interface;
//	uint32		timeout = NFI_PSTA_TIMEOUT;
//	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
//	bool		bUsingDMA=TRUE;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();

	// reset NFI
	NFI_Reset();

	return S_DONE;
}

//------------------------------------------------------------------------------
// Spare Program Callback Function                                              
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_SpareProgram_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_spare32 /* MUST be 32bits alignment addr */
) {
	uint32		page_size;
//	uint32		spare_addr;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		io_interface;
//	uint32		timeout = NFI_PSTA_TIMEOUT;
//	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
//	bool		bUsingDMA=TRUE;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();

	// reset NFI
	NFI_Reset();

	return S_DONE;
}

//------------------------------------------------------------------------------
// CopyBack Callback Function                                                   
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_CopyBack_V3(
				const uint32  c_timeout
				,const uint32  src_row_addr
				,const uint32  dest_row_addr
) {
	uint32	page_size;
	uint32	spare_size;
	uint32	column_addr_bits;
	uint32	addr_cycle;
	uint32	io_interface;
//	uint32	timeout = NFI_PSTA_TIMEOUT;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();

// !!
// !! We don't maintain CopyBack operation after MT6235/MT6238/MT6516
// !!

	// reset NFI
	NFI_Reset();

	return ret;
}

uint32 micron_pp_mapping(uint32 page)//high to low mapping
{
    uint32 offset;
	
    if((page < 4) || (page > (NUTL_PAGES_PER_BLOCK() - 5)))
    {
        return page;
    }
    offset = page % 4;
    if((offset == 0) || (offset == 1))
    {
        return page;
    }
    else
    {
        return page - 6;
    }
}

uint32 hynix_pp_mapping(uint32 page) //high to low mapping
{
    uint32 offset;

    if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX) //20nm 8K
    {
	    if(page < 4)
	    {
	        return page;
	    }

	    offset = page % 4;

	    if((offset == 2) || (offset == 3))
	    {
	        return page;
	    }
	    else
	    {
	        if((page == 4) || (page == 5) || (page == 254) || (page == 255))
	        {
	            return page - 4;
	        }
		 else
		 {
		     return page - 6;
		 }
	    }
    }
    else //16nm 16K
    {
        if(page == 255)
	     return page - 2;
	 if((page == 0) || ((page %2) == 1))
	     return page;
	 else
	 {
	     if(page == 2)
	         return 0;
	     else
		  return page - 3;
	 }
    }
}

