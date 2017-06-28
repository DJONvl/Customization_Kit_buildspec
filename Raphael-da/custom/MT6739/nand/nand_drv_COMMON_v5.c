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

#ifdef __MTK_NFI_V5__

#include "nand_drv_COMMON_v5.h"
#include "nand_util_func.h"
#include "debug.h"

#define DRV_WriteReg(addr,data)     	((*(volatile uint16 *)(addr)) = (uint16)data)
#define DRV_Reg(addr)               	(*(volatile uint16 *)(addr))
#define DRV_WriteReg32(addr,data)     	((*(volatile uint32 *)(addr)) = (uint32)data)
#define DRV_Reg32(addr)               	(*(volatile uint32 *)(addr))
#define DRV_WriteReg8(addr,data)     	((*(volatile uint8 *)(addr)) = (uint8)data)
#define DRV_Reg8(addr)               	(*(volatile uint8 *)(addr))
bool tlc_snd_phyplane = FALSE; // second plane operation

bool disautofmt = FALSE;

uint16 randomizer_table[128]=
{
	0x576A, 0x05E8, 0x629D, 0x45A3,
	0x649C, 0x4BF0, 0x2342, 0x272E,
	0x7358, 0x4FF3, 0x73EC, 0x5F70,
	0x7A60, 0x1AD8, 0x3472, 0x3612,
	0x224F, 0x0454, 0x030E, 0x70A5,
	0x7809, 0x2521, 0x48F4, 0x5A2D,
	0x492A, 0x043D, 0x7F61, 0x3969,
	0x517A, 0x3B42, 0x769D, 0x0647,
	0x7E2A, 0x1383, 0x49D9, 0x07B8,
	0x2578, 0x4EEC, 0x4423, 0x352F,
	0x5B22, 0x72B9, 0x367B, 0x24B6,
	0x7E8E, 0x2318, 0x6BD0, 0x5519,
	0x1783, 0x18A7, 0x7B6E, 0x7602,
	0x4B7F, 0x3648, 0x2C53, 0x6B99,
	0x0C23, 0x67CF, 0x7E0E, 0x4D8C,
	0x5079, 0x209D, 0x244A, 0x747B,
	0x350B, 0x0E4D, 0x7004, 0x6AC3,
	0x7F3E, 0x21F5, 0x7A15, 0x2379,
	0x1517, 0x1ABA, 0x4E77, 0x15A1,
	0x04FA, 0x2D61, 0x253A, 0x1302,
	0x1F63, 0x5AB3, 0x049A, 0x5AE8,
	0x1CD7, 0x4A00, 0x30C8, 0x3247,
	0x729C, 0x5034, 0x2B0E, 0x57F2,
	0x00E4, 0x575B, 0x6192, 0x38F8,
	0x2F6A, 0x0C14, 0x45FC, 0x41DF,
	0x38DA, 0x7AE1, 0x7322, 0x62DF,
	0x5E39, 0x0E64, 0x6D85, 0x5951,
	0x5937, 0x6281, 0x33A1, 0x6A32,
	0x3A5A, 0x2BAC, 0x743A, 0x5E74,
	0x3B2E, 0x7EC7, 0x4FD2, 0x5D28,
	0x751F, 0x3EF8, 0x39B1, 0x4E49,
	0x746B, 0x6EF6, 0x44BE, 0x6DB7
};

#if 0 //def DUMP_SPEED
extern U32 gpt4_get_current_tick (void);
extern uint32 g_ds_nand_block_erase_number,  g_ds_nand_block_erase_time;
extern uint32 g_ds_nand_page_program_number, g_ds_nand_page_program_time;
extern uint32 g_ds_nand_page_read_number,    g_ds_nand_page_read_time;
#endif

bool use_cache_write 	= FALSE;
bool cache_last_wl      = FALSE;
extern int ftl_img;

bool use_randomizer 	= FALSE;
bool ddr_interface 		= FALSE;
bool device_need_rand 	= FALSE;
U32 NFI_TLC_GetRowAddr(U32 rowaddr);
void NFI_TLC_GetMappedWL(U32 pageidx, NFI_TLC_WL_INFO* WL_Info);

bool use_HWECC 			= TRUE;
extern bool bad_block_remark_done;

//to reset fdm size and ecc level
//2015.6.24 for l85a
bool reset_fdm_and_ecc 	= FALSE;
uint32 re_fdm_size 		= 2;
uint32 re_ecc_bit_cfg 	= ECC_CNFG_ECC40;
uint32 re_ecc_level 	= 40;
uint32 org_ecc_bit_cfg 	= ECC_CNFG_ECC40;
uint32 org_ecc_level 	= 40;
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
uint32 _u4TLCBlkSt     	= 0xFFFF;
uint32 _u4TLCBlkEnd  	= 0xFFFF;
uint32 _u4TLC2SLCRatio 	= 6;
#endif


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

void NFI_CheckMode(void)
{
	uint8 array[4] = {0,0,0,0};

	// Check SDR mode or Toggle mode
	NAND_COMMON_Get_Feature(NUTL_GET_FEATURE(), NUTL_SYNC_ADDR(), array, 4);
	if ((array[0] != NUTL_SYNC_VALUE()) && (array[0] != NUTL_ASYNC_VALUE()))
	{
		LOGE("%s Get feature is %d, Sync/Async %d/%d\n", __func__, array[0], NUTL_SYNC_VALUE(), NUTL_ASYNC_VALUE());
		NAND_COMMON_Switch_Sync(ddr_interface);
		NAND_COMMON_Get_Feature(NUTL_GET_FEATURE(), NUTL_SYNC_ADDR(), array, 4);
		LOGE("%s Get feature is %d, Switch and check again\n", __func__, array[0]);
	}
}

#if 0 //hynix read retry table read
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

#if 0 //hynix 16nm read retry table read
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

#if 0 //hynix fdie read retry table read
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
#if 0
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

#if 0 //sandisk 19nm read retry
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


void NAND_COMMON_Set_Feature(
			uint8	cmd,
			uint8	addr,
			uint8*	value,
			uint8	bytes
			)
{
	uint16	reg_val     = 0;
	uint8	write_count = 0;

	uint32	timeout = NFI_PSTA_TIMEOUT;
	uint32	reg;

	NFI_Reset();
	reg = DRV_Reg32(NFI_NAND_TYPE_CNFG);
	if(reg != 4)
		bytes = bytes * 2;

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
		if (reg == 4)
        	DRV_WriteReg32(NFI_DATAW, *value++);
		else
		{
			if ((write_count%2) == 1)
				DRV_WriteReg32(NFI_DATAW, *value++);
			else
				DRV_WriteReg32(NFI_DATAW, *value);
		}
        DRV_WriteReg32(NFI_DATAW, *value++);
        write_count++;
        timeout = NFI_PSTA_TIMEOUT;
    }
	*NFI_CNRNB = 0xF << 4;
	*NFI_CNRNB |=1;
	NFI_Wait_Return_Ready(timeout);
	
}

void NAND_COMMON_Get_Feature(uint8 cmd,uint8 addr, uint8 *value,  uint8 bytes)
{
	uint16	reg_val    = 0;
	uint8	read_count = 0;

	uint32	timeout = NFI_PSTA_TIMEOUT;
	//uint32 u4RandomSetting;

	NFI_Reset();

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
	*NFI_CNRNB = 0xF << 4;
	*NFI_CNRNB |=1;
	NFI_Wait_Return_Ready(timeout);
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
		*ECC_DECCNFG = reg_value|0x400;
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
		*ECC_DECCNFG = reg_value|0x400;
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

status_t NAND_COMMON_Randomizer_Drv_Config(bool use, uint32 pageNo)
{
	//LOGD("[NAND_COMMON_Randomizer_Drv_Config] use(%d)\n", use);
	use_randomizer = use;

	return STATUS_OK;
}

status_t NAND_COMMON_Switch_Sync(bool sync)
{
	uint32 reg1, reg2, reg3, delayctrl, reg4, reg5, reg6;
	uint32 i = 10;

	LOGD("%s - sync %d\n", __func__, sync);

	NFI_Reset();

	reg1 = DRV_Reg32(NFI_DEBUG_CON1);
	reg2 = DRV_Reg32(ECC_BYPASS);
	//reg3 = DRV_Reg32(NFI_1X_INFRA);
	reg5 = DRV_Reg32(ECC_DECCNFG);
	//reg6 = DRV_Reg32(NFI_ECC_FGMUX);
	LOGD("regs 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", reg1, reg2, reg3, reg5, reg6);

	// disable ecc bypass for async
	reg1 &= ~0x8000;
	reg2 &= ~0x1;
	DRV_WriteReg32(NFI_DEBUG_CON1,reg1);
	DRV_WriteReg32(ECC_BYPASS,reg2);

	delayctrl = ((uint32)(NUTL_DLY_MUX() & 0xFF) << 4) | NUTL_DLY_CTRL();
	if(sync) // toggle, onfi
	{
		// Switch ECC clock
		//reg6 &= ~0x0FC00000;
		DRV_WriteReg32(ECC_DECCNFG, (reg5|0x0400));
		//DRV_WriteReg32(NFI_ECC_FGMUX, (reg6|(0x9<<22)));

		// Switch NFI 1X clock
		#if 0 // diable NFI 1X clock setting
		reg3 &= 0xFFFFFF00;
		reg3 |= 0x12;
		DRV_WriteReg32(NFI_1X_INFRA,reg3);
		while(i)
			i--;
		reg3 |= 0x80;
		DRV_WriteReg32(NFI_1X_INFRA,reg3);
		#endif

		reg4 = 1;
		DRV_WriteReg32(NFI_ACCCON1,NUTL_SYNC_ACCCON1());
		DRV_WriteReg32(NFI_ACCCON,NUTL_SYNC_ACCCON());
		DRV_WriteReg32(NFI_DELAY_CTRL,delayctrl);
		//DRV_WriteReg32(NFI_DELAY_CTRL,0xA001);
		while(0 == (DRV_Reg(NFI_STA) && STA_FLASH_MACRO_IDLE))
			;
up:
		DRV_WriteReg32(NFI_NAND_TYPE_CNFG , reg4);
		LOGD("NFI_NAND_TYPE_CNFG_REG32 0x%x reg 0x%x\n",DRV_Reg(NFI_NAND_TYPE_CNFG),reg4);
		if(DRV_Reg(NFI_NAND_TYPE_CNFG) == 4)
			goto up;
	}
	else
	{
		//reg3 &= ~0x80;
		//reg1 |= 0x8000;
		//reg2 |= 0x1;
		reg4 = 0;
		//DRV_WriteReg32(NFI_1X_INFRA,reg3);
		//DRV_WriteReg(ECC_BYPASS,reg2);
		//DRV_WriteReg(NFI_DEBUG_CON1,reg1);
		DRV_WriteReg32(NFI_ACCCON,0x31C083F9);
		//DRV_WriteReg32(NFI_ACCCON,NUTL_SYNC_ACCCON());
		while(0 == (DRV_Reg(NFI_STA) && STA_FLASH_MACRO_IDLE))
			;
up1:
		DRV_WriteReg32(NFI_NAND_TYPE_CNFG,reg4);
		LOGD("NFI_NAND_TYPE_CNFG_REG32 0x%x reg 0x%x\n",DRV_Reg(NFI_NAND_TYPE_CNFG),reg4);
		if(DRV_Reg(NFI_NAND_TYPE_CNFG) == 1)
			goto up1;
	}

	return STATUS_OK;
}

status_t NAND_COMMON_Randomizer_Config(bool use, uint32 pageNo)
{
	uint16		nfi_cnfg = 0;
	uint32		nfi_ran_cnfg = 0;
	uint16		seed;

	//LOGD("%s line:%d, use %d\n", __func__, __LINE__, use);

	/* set up NFI_CNFG */
	if(device_need_rand)
	{
		nfi_cnfg = DRV_Reg(NFI_CNFG);
		//nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG);

		if(use)
		{
			if(NUTL_PAGES_PER_BLOCK() == 256) {
				seed = randomizer_table[pageNo%128];
			}
			else {
				seed = randomizer_table[(pageNo%NUTL_PAGES_PER_BLOCK())%128];
			}
			//LOGD("pageNo 0x%x seed 0x%x\n", pageNo, seed);
			nfi_ran_cnfg = 0;
			nfi_ran_cnfg |= seed << EN_SEED_SHIFT;
			nfi_ran_cnfg |= seed << DE_SEED_SHIFT;
			nfi_cnfg |= CNFG_RAN_SEC;
			nfi_cnfg |= CNFG_RAN_SEL;

			nfi_ran_cnfg |= 0x00010001;			
		}
		else
		{
			nfi_ran_cnfg &= ~0x00010001;
		}
		use_randomizer = use;

		DRV_WriteReg(NFI_CNFG, nfi_cnfg);
		DRV_WriteReg32(NFI_RANDOM_CNFG, nfi_ran_cnfg);
	}

	return STATUS_OK;
}

status_t NAND_COMMON_Interface_Config(bool use)
{
	uint8 array[4] = {0,0,0,0};

	LOGV("%s - use %d, ddr %d\n",__func__, use, ddr_interface);
	if(use)
	{
		if(NUTL_IO_INTERFACE_SYNC() == NAND_IO_TOGGLE ||NUTL_IO_INTERFACE_SYNC() == NAND_IO_ONFI)
		{
			NFI_CheckMode();
			if(ddr_interface == FALSE)
			{
				array[0] = NUTL_SYNC_VALUE();
				NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_SYNC_ADDR(),(uint8*)array,4);
				//do interface change;
				NAND_COMMON_Switch_Sync(TRUE);
				array[0] = 0;
				NAND_COMMON_Get_Feature(NUTL_GET_FEATURE(),NUTL_SYNC_ADDR(),(uint8*)array,4);
				LOGD("interface change 0x%x\n",array[0]);
			}
			ddr_interface = TRUE;
		}
	}
	else
	{
		if(NUTL_IO_INTERFACE_SYNC() == NAND_IO_TOGGLE ||NUTL_IO_INTERFACE_SYNC() == NAND_IO_ONFI)
		{
			if(ddr_interface == TRUE)
			{
				array[0] = NUTL_ASYNC_VALUE();
				LOGD("async value is %d\n",array[0]);
				NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(), NUTL_SYNC_ADDR(), array, 4);
				//do interface change;
				NAND_COMMON_Switch_Sync(FALSE);
				array[0] = 0;
				NAND_COMMON_Get_Feature(NUTL_GET_FEATURE(), NUTL_SYNC_ADDR(), array, 4);
				LOGD("interface change %d\n",array[0]);
			}
			ddr_interface = FALSE;
		}
	}

	return STATUS_OK;
}

static bool NAND_COMMON_Randomizer_Status()
{
	uint32   nfi_ran_cnfg = 0;
	nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG);
	if(nfi_ran_cnfg&0x00010001)
		return TRUE;

	return FALSE;
}
static void NAND_COMMON_Turn_On_Randomizer()
{
	uint32   nfi_ran_cnfg = 0;

	if(device_need_rand)
	{
		nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG);
		nfi_ran_cnfg |= 0x00010001;
		DRV_WriteReg32(NFI_RANDOM_CNFG, nfi_ran_cnfg);
	}
}
static void NAND_COMMON_Turn_Off_Randomizer()
{
	uint32   nfi_ran_cnfg = 0;
	//LOGD("[%s] device_need_rand(%d)\n", __func__, device_need_rand);

	if(device_need_rand)
	{
		nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG);
		nfi_ran_cnfg &= ~0x00010001;
		DRV_WriteReg32(NFI_RANDOM_CNFG, nfi_ran_cnfg);
	}
}


 
//------------------------------------------------------------------------------
// Read Device ID Callback Function                                             
//------------------------------------------------------------------------------
status_t  NAND_COMMON_ReadID_V5(
				const uint32  c_timeout,
				uint32* id1,
				uint32* id2
				)
{
	uint32		timeout = NFI_PSTA_TIMEOUT;
	//uint32		timeout_tick, start_tick;
	uint32		nfi_pagefmt = 0;
	uint16		reg_val;

	LOGE("[%s]\n", __func__);

	device_need_rand = FALSE;
	DRV_WriteReg(NFI_CSEL, 0);

	// Modify by Kuohong, use H8CS0SI0MP for Golden timing.
	DRV_WriteReg32(NFI_ACCCON, 0x10818022);
	LOGE("NFI_ACCCON 0x%x\n", *NFI_ACCCON);

	reg_val = DRV_Reg(NFI_CNFG);
	reg_val &= ~CNFG_AHB; //It can't be AHB mode for WAIT_NFI_PIO_READY.
	DRV_WriteReg(NFI_CNFG, reg_val);
	// DBG_MSG("GPIO_MODE1_MWR 0x%x GPIO_MODE2_MWR 0x%x GPIO_MODE5_MWR 0x%x, GPIO_MODE6_MWR 0x%x\n",DRV_Reg32(GPIO_MODE1_MWR),DRV_Reg32(GPIO_MODE2_MWR),DRV_Reg32(GPIO_MODE5_MWR),DRV_Reg32(GPIO_MODE6_MWR));
	// DBG_MSG("MSDC0_SELGP_SET 0x%x NFI_PULLEN_MODE 0x%x \n",DRV_Reg32(MSDC0_SELGP_SET),DRV_Reg32(NFI_PULLEN_MODE));
	LOGE("LINE %d NFI_CNFG 0x%x\n", __LINE__,*NFI_CNFG);

	ddr_interface = FALSE;
	NFI_Reset();

	/* Issue NAND chip reset command for Micron's MCP */
	NFI_ISSUE_COMMAND(RESET_CMD, 0, 0, 0, 0);
	//timeout_tick = gpt4_time2tick_ms(1); //1ms
	//start_tick = gpt4_get_current_tick();
//LOGE("LINE %d timeout_tick 0x%x, start_tick 0x%x\n", __LINE__, timeout_tick, start_tick);
LOGE("LINE %d NFI_STA 0x%x\n", __LINE__,*NFI_STA);
//LOGE("LINE %d NFI_CNFG 0x%x\n", __LINE__,*NFI_CNFG);

	//while(DRV_Reg(NFI_STA)&STA_NAND_BUSY && !gpt4_timeout_tick(start_tick, timeout_tick))
	while(DRV_Reg(NFI_STA)&STA_NAND_BUSY)
		;

	NAND_COMMON_Switch_Sync(FALSE);

	NFI_Reset();

	//start_tick = gpt4_get_current_tick()-start_tick;
	//LOGE("reset takes %d, status 0x%x\n", start_tick, *NFI_STA);

    // reset NFI
	NFI_Reset();
	nfi_pagefmt = *NFI_PAGEFMT;
	DRV_WriteReg32(NFI_PAGEFMT, 0);
	/* Config single read and read enable bit in NFI_CNFG register */
	DRV_WriteReg(NFI_CNFG, (CNFG_OP_SRD | CNFG_READ_EN));

	/* read NAND flash ID */
	DRV_WriteReg(NFI_CMD, RD_ID_CMD);
	/* wait for the command status ready */
	while (DRV_Reg32(NFI_STA) & STA_CMD_STATE)
		;

	/* Set addr registers */
	DRV_WriteReg32(NFI_COLADDR, 0);
	DRV_WriteReg32(NFI_ROWADDR, 0);
	DRV_WriteReg(NFI_ADDRNOB, 1);
	/* wait for the address status ready */
	while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE)
		;

	/* Trigger read operations */
	DRV_WriteReg(NFI_CON, CON_NFI_SRD);
	while(DRV_Reg32(NFI_STA) & STA_DATAR_STATE)
		;

	/* Check the PIO bit is ready or not */
	WAIT_NFI_PIO_READY(timeout);  

	/* If status is ready, read back the NFI_DATAR register */
	*id1 = DRV_Reg32(NFI_DATAR);
	*id2 = DRV_Reg32(NFI_DATAR);

    DRV_WriteReg(NFI_CON, 0x0);

	device_need_rand = TRUE;
	use_randomizer   = TRUE;

	// restore original page format setting 
	*NFI_PAGEFMT = nfi_pagefmt;


    return STATUS_OK;
}


//------------------------------------------------------------------------------
// Reset Device Callback Function                                               
//------------------------------------------------------------------------------
status_t  NAND_COMMON_Reset_V5(
				const uint32  c_timeout
				)
{
	uint32		timeout = NFI_PSTA_TIMEOUT;

	// reset NFI
	if(ddr_interface == FALSE)
	{
		NFI_Reset();

		// set NFI_CNFG
		*NFI_CNFG = CNFG_OP_RESET;
		// reset cmd 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_reset.m_cmd;

		// wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout) {
			return STATUS_NAND_TIMEOUT;
		}

		// wait for reset finish 
		timeout = c_timeout;
		NFI_Wait_Ready(timeout);
		if(!timeout) {
			return STATUS_NAND_TIMEOUT;
		}
	}
	else
	{
		return NAND_COMMON_Interface_Config(FALSE);
	}


	return STATUS_OK;
}

//------------------------------------------------------------------------------
// Read Status Callback Function                                                
//------------------------------------------------------------------------------
status_t  NAND_COMMON_ReadStatus_V5(
				const uint32	c_timeout
				)
{
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32		status  = STATUS_NAND_ERR;

	// reset NFI
	NFI_Reset();
	if(use_randomizer) {
		NAND_COMMON_Turn_Off_Randomizer();
	}

	// set NFI_CNFG
	*NFI_CNFG = BOOTROM_SRD_CNFG;

	// read status cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_status.m_cmd;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout) {
		return STATUS_NAND_TIMEOUT;
	}

	// set single read by DWORD 
	*NFI_CON = (1 << CON_NFI_NOB_SHIFT)|CON_NFI_SRD;
	// wait til DATA_READ is completely issued 
	timeout = c_timeout;
	WAIT_NFI_PIO_READY(timeout);
	if(timeout) {
		// single read doesn't need to polling FIFO 
		status = *NFI_DATAR;
	}

	//~  clear NOB
	*NFI_CON = 0;
	//DBG_MSG("read status 0x%x\n",status);

	// check READY/BUSY status first 
	if(!(STATUS_READY&status)) {
		return STATUS_NAND_IN_PROGRESS;
	}

#if defined(__NFI_SUPPORT_TLC__)
	if( (g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)&&
		(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) )
	{
		if( 0x4 & status ) {
			return STATUS_NAND_ERR;
		}
		else {
			return STATUS_OK;
		}
	}
#endif
	// flash is ready now, check status code 
	if( STATUS_FAIL & status )
	{
		if(!(STATUS_WR_ALLOW&status)) {
			return STATUS_NAND_ERR;
		}
		else {
			return STATUS_NAND_ERR;
		}
	}
	else {
		return STATUS_OK;
	}
}

//------------------------------------------------------------------------------
// Block Erase Related Callback Function                                        
//------------------------------------------------------------------------------
status_t  NAND_COMMON_BlockErase_V5(
				const uint32  row_addr
				)
{
	uint32		page_size;
	uint32		spare_size;
	uint32		addr_cycle;
	uint32		row_addr_nob, col_addr_nob;
	uint32		c_timeout = NFI_PSTA_TIMEOUT;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32		page_addr = (uint32)row_addr;
	status_t	status = STATUS_OK;
#if 0 //def DUMP_SPEED
	uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif
	//DBG_MSG("Drv NAND_COMMON_BlockErase_V5.....%x\n",row_addr);
#if defined(__NFI_SUPPORT_TLC__)
	NFI_TLC_WL_INFO  tlc_wl_info;
	U32 reg_val = 0;
	U32 u4EraseTimes = 1;
#endif
	page_size  = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	addr_cycle = NUTL_ADDR_CYCLE();

	LOGD("[%s] row_addr (0x%x) (gslc %d)\n", __func__, row_addr , (g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn));

#if defined(__NFI_SUPPORT_TLC__)
	g_pNandInfo->m_eraselevel_support = FALSE;//QQ for debug

	if(g_pNandInfo->m_eraselevel_support) {
		u4EraseTimes = 2;
	}

	while(u4EraseTimes)
	{
		if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
		{
			if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)//normal tlc
			{
				NFI_TLC_GetMappedWL(row_addr, &tlc_wl_info);
				if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
					page_addr = NFI_TLC_GetRowAddr(page_addr);
				else
					page_addr = NFI_TLC_GetRowAddr(tlc_wl_info.word_line_idx);
			}
			else
			{
				page_addr = NFI_TLC_GetRowAddr(page_addr);
			}

			// reset NFI
			NFI_Reset();
			if((g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) &&
				(0xFF != g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd))
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
#endif
		// set NFI_CNFG
		*NFI_CNFG = BOOTROM_ERASE_CNFG;

		// block erase cmd 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_erase.m_cmd;
		// wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout) {
			return STATUS_NAND_TIMEOUT;
		}

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
		if(!timeout) {
			return STATUS_NAND_TIMEOUT;
		}

		// block erase confirm 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_erase_cnf.m_cmd;
		// wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout) {
			return STATUS_NAND_TIMEOUT;
		}
		/*wait til ready, add by jiequn,2011.11.8*/
		timeout = c_timeout;
		NFI_Wait_Ready(timeout);
		if(!timeout) {
			return STATUS_NAND_TIMEOUT;
		}
#if defined(__NFI_SUPPORT_TLC__)
		//if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
		u4EraseTimes--;
	}
#endif

	do
	{
		status = NAND_COMMON_ReadStatus_V5(timeout);
	}while(STATUS_NAND_IN_PROGRESS == status);

	if(status == STATUS_NAND_ERR)
	{
		//DBG_MSG("[xiaolei] eb fail 0x%x\n",row_addr);
		return STATUS_NAND_ERASE_FAILED;
	}
	if(status != STATUS_OK)
	{
		//DBG_MSG("[xiaolei] eb fail 2  0x%x\n",row_addr);
		return status;
	}

#if 0 //def DUMP_SPEED
	g_ds_nand_block_erase_number++;
	tick_end = gpt4_get_current_tick();
	g_ds_nand_block_erase_time += (tick_end - tick_start);
#endif

#if defined(__NFI_SUPPORT_TLC__)
	if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC) &&
		(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)) //hynix tlc need doule check
	{
       
		if((g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) &&
			(0xFF != g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd))
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
	//LOGD("line=%d, function=%s, row_addr=0x%x, slcopmodeEn=%d\n",
	//	__LINE__, __FUNCTION__, row_addr, g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn);

	return STATUS_OK;
}
//------------------------------------------------------------------------------
// Bad Block Symbol Identification Related Callback Function                    
//------------------------------------------------------------------------------
#if 0
status_t  NAND_COMMON_BadBlockSymbol_Check_V5(
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
				return STATUS_NAND_BAD_BLOCK;
			}
		}
		else {
			if( 0xFF != p_spare8[0] ) {
				return STATUS_NAND_BAD_BLOCK;
			}
		}
	}
	else {
		// for 512 page size flash 
		if( NAND_IO_16BITS == io_interface ) {
			// for 16 bits I/O, the invalid block status is defined by the 1st word in spare area 
			if( 0xFFFF != p_spare16[0] ) {
				return STATUS_NAND_BAD_BLOCK;
			}
		}
		else {
			// for 8 bits I/O, the invalid block status is defined by the 6th byte in spare area 
			if( 0xFF != p_spare8[5] ) {
				return STATUS_NAND_BAD_BLOCK;
			}
		}
	}

	return STATUS_OK;
}

status_t  NAND_COMMON_BadBlockSymbol_Set_V5(
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

	return STATUS_OK;
}
#endif
//------------------------------------------------------------------------------
// TLC support  Function                                                  
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Page Program Callback Function                                               
//------------------------------------------------------------------------------
status_t  NAND_COMMON_Block_charge_V5(const uint32 row_addr)
{

	uint32		page_size;
	uint32		addr_cycle;
	uint32		row_addr_nob;
	uint32		c_timeout = NFI_PSTA_TIMEOUT;
	uint32		timeout   = NFI_PSTA_TIMEOUT;
	uint32		page_addr = (uint32)row_addr;
	uint32		reg_val   = 0;

	NFI_TLC_WL_INFO		tlc_wl_info;

	//DBG_MSG("Drv NAND_COMMON_BlockErase_V5.....%x\n",row_addr);

	if(TNAND_TC58TEG6TGLTA00 !=  NUTL_DEVICE_NAME()) {
		return STATUS_OK;
	}

	page_size  = NUTL_PAGE_SIZE();
	addr_cycle = NUTL_ADDR_CYCLE();

	if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)//normal tlc
	{
		NFI_TLC_GetMappedWL(row_addr, &tlc_wl_info);
		if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
			page_addr = NFI_TLC_GetRowAddr(page_addr);
		else
			page_addr = NFI_TLC_GetRowAddr(tlc_wl_info.word_line_idx);
	}

	// reset NFI
	NFI_Reset();

	reg_val = DRV_Reg(NFI_CNFG);
	reg_val &= ~CNFG_READ_EN;
	reg_val &= ~CNFG_OP_MODE_MASK;
	reg_val |= CNFG_OP_CUST;
	DRV_WriteReg(NFI_CNFG, reg_val);

	DRV_WriteReg(NFI_CMD, 0xDF);
	while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	reg_val = DRV_Reg32(NFI_CON);
	reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
	/* issue reset operation */
	DRV_WriteReg32(NFI_CON, reg_val);

	// set NFI_CNFG
	*NFI_CNFG = BOOTROM_ERASE_CNFG;

	// block erase cmd 
	*NFI_CMD = 0x60;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout) {
		return STATUS_NAND_TIMEOUT;
	}

	// fill 1~4 cycle addr, erase command only fill row address, so column bits shift is unnecessary 
	*NFI_ROWADDR = page_addr;
	*NFI_COLADDR = 0;
	row_addr_nob = 3;
	*NFI_ADDRNOB = (row_addr_nob << ADDR_ROW_NOB_SHIFT) ;	// Block Erase don't need COL_ADDR_NOB

	// wait til ADDR is completely issued 
	timeout = c_timeout;
	NFI_Wait_Adddress(timeout);
	if(!timeout){
		return STATUS_NAND_TIMEOUT;
	}

	// block erase confirm
	*NFI_CMD = 0xD0;
	// wait til CMD is completely issued
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){
		return STATUS_NAND_TIMEOUT;
	}
	/*wait til ready, add by jiequn,2011.11.8*/
	timeout = c_timeout;
	NFI_Wait_Ready(timeout);
	if(!timeout){
		return STATUS_NAND_TIMEOUT;
	}

	return STATUS_OK;
}

status_t  NAND_COMMON_PageProgramBad_V5(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_data32 /* MUST be 32bits alignment addr */
				,const uint32 *p_spare32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {
	uint32		page_size;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		row_addr_nob, col_addr_nob;	
	uint32		sectors;
	//uint32		* fdm;
	uint32		io_interface;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	status_t	ret=STATUS_OK;
	//bool		bUsingDMA=TRUE;
	uint32     page_addr = (uint32)row_addr;
	bool fgOrigEnslc = g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn;
        //DBG_MSG("line=%d,function= %s,row_addr=0x%x,fgslc=%d , \n ",  __LINE__,  __FUNCTION__, row_addr, g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn);
LOGD(" qq   ,line=%d,function= %s,row_addr=0x%x, p_data32=0x%x, data 0x%x \n ",
	__LINE__,  __FUNCTION__, row_addr,(u32)p_data32,*p_data32);

#if defined(__NFI_SUPPORT_TLC__)
       NFI_TLC_WL_INFO  tlc_wl_info;
       U32 reg_val = 0;       
#endif
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
	    if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC){
	   g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn = TRUE;}
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
                                 //DBG_MSG("line=%d,function= %s \n ",  __LINE__,  __FUNCTION__);

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
      
      while( (STATUS_NAND_IN_PROGRESS ==(ret = NAND_COMMON_ReadStatus_V5(c_timeout))) && (timeout--));
	if(!timeout){
		ret= STATUS_NAND_TIMEOUT; }

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
      g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn = fgOrigEnslc;

	return ret;
}

#if defined(__NFI_SUPPORT_TLC__)
bool tlc_lg_left_plane = TRUE; //logical left plane of tlc nand.  used to do page program
NFI_TLC_PG_CYCLE tlc_program_cycle;
uint32  ecc_parity_from_reg[8];
bool  NFI_IsSLC_Blk(U32 blockidx)
{    
       if( (blockidx >= _u4TLCBlkSt) && (blockidx <= _u4TLCBlkEnd) )
           return FALSE;
       else
           return TRUE;
}
//tlc releated functions
void NFI_TLC_GetMappedWL(U32 pageidx, NFI_TLC_WL_INFO* WL_Info)
{
	//this function is just for normal tlc
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

status_t NFI_TLC_PageWrite(
			const uint32	c_timeout, 
			const uint32	page_index, 
			const uint32*	sector_buff, 
			const uint32*	spare_buff,
			uint32			ecc_parity_from_reg[8]
			)
{
	uint8*	main_ptr  = NULL;
	uint8*	spare_ptr = NULL;
	U32		sector_size = NAND_SECTOR_SIZE;
	status_t  status    = STATUS_OK;
	uint32 fdm;
	uint8 tmp_spare[16 * 8];
	int i;
	//uint32  ecc_parity_from_reg[8];
	U32   nfi_sector_num = g_pNandInfo->m_dev->m_hw_info.m_page_size /g_pNandInfo->m_dev->m_hw_info.m_sector_size;

    //LOGD("%s - page 0x%x,sector_buff=0x%x, tlc_program_cycle=%d (pPlaneEn %d)\n\r",
	//	__func__, page_index, sector_buff, tlc_program_cycle, g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn);

	sector_size  = g_pNandInfo->m_dev->m_hw_info.m_sector_size;

	main_ptr  = (P_U8)sector_buff;
	spare_ptr = (P_U8)spare_buff;

	if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC) && g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
	{
		nfi_sector_num = nfi_sector_num / 2;

		tlc_lg_left_plane = TRUE; //program left plane
		status = NAND_COMMON_PageProgram_V5(NFI_PSTA_TIMEOUT, page_index,  (uint32 *)main_ptr, (uint32 *)spare_ptr, ecc_parity_from_reg);
		if( STATUS_OK != status )
		{
			return status;
		}

		tlc_lg_left_plane = FALSE; //program right plane
		main_ptr += (sector_size * nfi_sector_num);

		if(spare_buff)
			spare_ptr += (nfi_sector_num) * 8/*nfi_fdm_size*/;

		status = NAND_COMMON_PageProgram_V5(NFI_PSTA_TIMEOUT, page_index, (uint32 *)main_ptr, (uint32 *)spare_ptr, ecc_parity_from_reg);
	}
    else
	{
		tlc_snd_phyplane = FALSE;

		if (ftl_img)
		{
			fdm = ((*NFI_PAGEFMT & PAGEFMT_FDM_MASK) >> PAGEFMT_FDM_SHIFT);
			memset(spare_ptr, 0xFF, 16 * 8);
			for(i=0;i<nfi_sector_num;i++)
				memcpy(spare_ptr + (8*i), main_ptr + FTL_PAGE_SIZE - (nfi_sector_num - i)*fdm, fdm);
			memcpy(tmp_spare, main_ptr + FTL_PAGE_SIZE - nfi_sector_num*fdm, nfi_sector_num*fdm);
			memset(main_ptr + FTL_PAGE_SIZE - nfi_sector_num*fdm, 0xff, nfi_sector_num*fdm);
			memcpy(main_ptr + FTL_PAGE_SIZE - nfi_sector_num*fdm, main_ptr + FTL_PAGE_SIZE, 32);
		}
#if 0 // 2 plane support not yet
		if (ftl_img && g_pNandInfo->m_dev->m_hw_info.m_page_size == FTL_PAGE_SIZE/2)
		{
			uint8 *pdata;

			tlc_snd_phyplane = TRUE;
			pdata = main_ptr + FTL_PAGE_SIZE / 2;
			status = NAND_COMMON_PageProgram_V5(NFI_PSTA_TIMEOUT, (page_index+page_per_block),
						(uint32 *)pdata, (uint32 *)spare_ptr, ecc_parity_from_reg);
			if( STATUS_OK != status )
				return status;
			tlc_snd_phyplane = FALSE;
		} else
#endif
		{
			status = NAND_COMMON_PageProgram_V5(NFI_PSTA_TIMEOUT, page_index, (uint32 *)main_ptr,
	        									(uint32 *)spare_ptr, ecc_parity_from_reg);
			if( STATUS_OK != status )
				return status;
		}

		if (ftl_img)
			memcpy(main_ptr + FTL_PAGE_SIZE - nfi_sector_num*fdm, tmp_spare, nfi_sector_num*fdm);
	}

	return status;
}


status_t NFI_TLC_WLProgram(U32 wl_index, P_U32 sector_buff, P_U32 spare_buff,
                                NFI_TLC_PG_CYCLE program_cycle)
{
    U32 page_idx = 0;
    P_U8 main_ptr = NULL;
    status_t  status    = STATUS_OK;
	uint32	chunk_size = 0;

	if (!ftl_img)
		chunk_size = g_pNandInfo->m_dev->m_hw_info.m_page_size;
	else
		chunk_size = FTL_PAGE_SIZE + FTL_OOB_SIZE;

    main_ptr = (P_U8)sector_buff;
    tlc_program_cycle = program_cycle;
    page_idx = wl_index * 3;
	
    status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, page_idx, (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);
    if( STATUS_OK != status )
        return status;

    main_ptr += chunk_size;
    status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, (page_idx+1), (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);
    if( STATUS_OK != status )
        return status;

    main_ptr += chunk_size;
	// [Cache_Program] If is last wl for cache program, diable cache program for this block
	if (cache_last_wl && use_cache_write)
	{
		use_cache_write = FALSE;
	}
    status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, (page_idx+2), (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);

	// [Cache_Program] after program last wl, diable flag
	if (cache_last_wl)
		cache_last_wl = FALSE;

    return status;
}

extern int ftl_img;

status_t  NAND_COMMON_TLC_BlockProgram(const uint32  c_timeout, U32 blk_index, P_U32 sector_buff, P_U32 spare_buff)
{
	U32 idx = 0;
	U32 base_idx = 0;
	U32 pg_time_per_block = 0;
    uint32	chunk_size = 0;
	P_U8 main_ptr = NULL;
	status_t  status    = STATUS_OK;

    LOGD("line=%d,function= %s ,blk_index=%d \n ",  __LINE__,  __FUNCTION__, blk_index);

	// [Cache_Program] Program TLC block use cache program for speed up
	use_cache_write = TRUE;

	main_ptr = (P_U8)sector_buff;

	if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc)
    {
		pg_time_per_block = (g_pNandInfo->m_dev->m_hw_info.m_pages_per_block );// SLC view, slc page number
		for(idx = 0; idx < pg_time_per_block; idx++)
		{        
			// [Cache_Program] Record the last wl for cache program
			if (idx == (pg_time_per_block-1)) {
				cache_last_wl = TRUE;
			}

			if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)//not-exist this case
			{
				base_idx = blk_index * g_pNandInfo->m_dev->m_hw_info.m_pages_per_block;

				LOGD("Program page 0x%x\n\r", (base_idx + idx*3));
				status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, (base_idx + idx*3), (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);
				if( STATUS_OK != status )
				{
					return status;
				}

				main_ptr += g_pNandInfo->m_dev->m_hw_info.m_page_size;
			}
			else
			{
				base_idx = blk_index * pg_time_per_block;
				LOGD("Program wl 0x%x\n\r", (base_idx + idx));

				if (!ftl_img)
					chunk_size = g_pNandInfo->m_dev->m_hw_info.m_page_size;
				else
					chunk_size = FTL_PAGE_SIZE + FTL_OOB_SIZE;

				if(idx == 0)
				{
					main_ptr = ((P_U8)sector_buff) + (idx)*3*chunk_size;
					status = NFI_TLC_WLProgram((base_idx + idx), (P_U32)main_ptr, spare_buff,PROGRAM_1ST_CYCLE);
					if( STATUS_OK != status )
					{
						return status;
					}

					main_ptr = ((P_U8)sector_buff) + (idx+1)*3*chunk_size;
					status = NFI_TLC_WLProgram((base_idx + idx + 1), (P_U32)main_ptr, spare_buff, PROGRAM_1ST_CYCLE);
					if( STATUS_OK != status )
					{
						return status;
					}

					main_ptr = ((P_U8)sector_buff) + (idx)*3*chunk_size;
					status = NFI_TLC_WLProgram((base_idx + idx), (P_U32)main_ptr, spare_buff, PROGRAM_2ND_CYCLE);
					if( STATUS_OK != status )
					{
						return status;
					}
				}

				if((idx + 2) < pg_time_per_block)
				{
					main_ptr = ((P_U8)sector_buff) + (idx+2)*3*chunk_size;
					status = NFI_TLC_WLProgram((base_idx + idx + 2), (P_U32)main_ptr, spare_buff, PROGRAM_1ST_CYCLE);
					if( STATUS_OK != status )
					{
						return status;
					}
				}
				if((idx + 1) < pg_time_per_block)
				{
					main_ptr = ((P_U8)sector_buff) + (idx+1)*3*chunk_size;
					status = NFI_TLC_WLProgram((base_idx + idx + 1), (P_U32)main_ptr, spare_buff, PROGRAM_2ND_CYCLE);
					if( STATUS_OK != status )
					{
						return status;
					}
				}

				main_ptr = ((P_U8)sector_buff) + (idx)*3*chunk_size;
				status = NFI_TLC_WLProgram((base_idx + idx), (P_U32)main_ptr, spare_buff, PROGRAM_3RD_CYCLE);
				if( STATUS_OK != status )
				{
					return status;
				}
			}     
		}
#if 0 
		for(idx = 0; idx < pg_time_per_block*3; idx++)
		{   
			if(!g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)//not-exist this case
			{
				base_idx = blk_index * pg_time_per_block;
				memset(local_page_buf, 0xAA, sizeof(local_page_buf));
				NAND_COMMON_PageRead_V5(
					0xFFFF
					,(base_idx*3 + idx)
					,local_page_buf /* MUST be 32bits alignment addr */
					,local_spare_buf /* MUST be 32bits alignment addr */
					,local_spare_buf);
			}
		}
#endif
	}
	else//micron
	{
		//to deal with slc/tlc mode later
		pg_time_per_block = g_pNandInfo->m_dev->m_hw_info.m_pages_per_block;
		base_idx = blk_index * pg_time_per_block;

		if (!ftl_img)
			chunk_size = g_pNandInfo->m_dev->m_hw_info.m_page_size;
		else
			chunk_size = FTL_PAGE_SIZE + FTL_OOB_SIZE;

		for(idx = 0; idx < pg_time_per_block; idx++)
		{
			LOGD("Program page 0x%x\n\r", (base_idx + idx));

			main_ptr += chunk_size;
			status = NFI_TLC_PageWrite(NFI_PSTA_TIMEOUT, (base_idx+idx), (P_U32)main_ptr, spare_buff, ecc_parity_from_reg);
			if( STATUS_OK != status )
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
status_t NUTL_TLC_ChangeColumn(U32* real_row_addr, bool   tlc_left_plane)
{
	//bool   tlc_left_plane = TRUE;
	U32 reg_val = 0;
	U32 timeout = NFI_PSTA_TIMEOUT;

	status_t  status = STATUS_OK;

	LOGV("%s real_row_addr 0x%x, tlc_left_plane %d\n", __func__, real_row_addr, tlc_left_plane);

	if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
	{
		if(g_pNandInfo->m_dev->m_tlc_setting->needchangecolumn)
		{
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
			*NFI_ADDRNOB = (((NUTL_ADDR_CYCLE() - ((NUTL_PAGE_SIZE()==512)?1:2))<<ADDR_ROW_NOB_SHIFT)
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
	return status;
}

status_t NUTL_MLC_ReadRetry(status_t readret, uint32 page_addr , uint32 timeout, uint32* readRetryCount, uint32* readRetryVal, bool*  readRetry)
{
//uint32 mlcreadRetryCount = *readRetryCount;
//uint32 mlcreadRetryVal     = *readRetryVal;
//bool    mlcreadRetry          = *readRetry;

	*readRetryCount = 0;
	*readRetryVal   = 0;
	*readRetry      = FALSE;

	return STATUS_OK;
}
static void NAND_READ_FDM_DATA(uint32 *p_data32, uint32 sectornum)
{
	uint32 i;
	uint32 *p_spare32 = (uint32 *)p_data32;

	if(p_spare32)
	{
		for(i = 0; i < sectornum; i++)
		{
			*p_spare32++ = *(NFI_FDM0L + (i << 1));
			*p_spare32++ = *(NFI_FDM0M + (i << 1));

			if(ftl_img)
				LOGD("FDM[%d] 0x%x 0x%x\n", i, *(NFI_FDM0L + (i << 1)),*(NFI_FDM0M + (i << 1)));
		}
	}

	//LOGV("%s - fdm: ", __func__);
	//for(i = 0; i < (2*sectornum); i++)
		//LOGV("0x%x\n",*p_spare32++);
}

status_t  NAND_COMMON_PageRead_V5_hw(
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
	//uint32      u4DECCODESize;
	status_t	ret=STATUS_NAND_ERR;
	bool		bUsingDMA=TRUE;
	bool        readRetry = FALSE;
	//uint32     readRetryCount = 0;
	//uint32     readRetryVal= 0;
	uint32     page_addr = (uint32)row_addr;
	uint32     colu_addr = 0;
#if 0 //def DUMP_SPEED
    uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif
	U32   logical_plane_num   = 1;
	U32   logical_plane_index = 0;
	NFI_Reset();

	LOGD("[%s] row_addr(0x%x) (gslc %d)\n", __func__, row_addr,(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn));

#if defined(__NFI_SUPPORT_TLC__)

	U32 reg_val = 0;
	NFI_TLC_WL_INFO  tlc_wl_info = {0};
	uint32 u4PgPerBl = g_pNandInfo->m_dev->m_hw_info.m_pages_per_block *3;
	bool   tlc_left_plane = TRUE;
	U32   real_row_addr = 0;
	U32   nfi_sector_num = g_pNandInfo->m_dev->m_hw_info.m_page_size /g_pNandInfo->m_dev->m_hw_info.m_sector_size;

	if(g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
	{
		if(g_pNandInfo->m_dev->m_tlc_setting->normaltlc) // normal tlc
		{
			NFI_TLC_GetMappedWL(row_addr, &tlc_wl_info);//SLC view, page addr == WL index,mark it
			if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn && NFI_IsSLC_Blk(row_addr/u4PgPerBl))
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
		}
		else
		{
			real_row_addr = NFI_TLC_GetRowAddr(page_addr);
		}
		page_addr = real_row_addr;
		if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn  && NFI_IsSLC_Blk(row_addr/u4PgPerBl)) // slc mode
		{
			if(0xFF != g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd)
			{
				reg_val = DRV_Reg(NFI_CNFG);
				reg_val &= ~CNFG_READ_EN;
				reg_val &= ~CNFG_OP_MODE_MASK;
				reg_val |= CNFG_OP_CUST;
				DRV_WriteReg(NFI_CNFG, reg_val);
				if(g_pNandInfo->m_dev->m_tlc_setting->normalslcopen)
					DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->normal_slc_mode_cmd);
				else
					DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
				NFI_Wait_Command(timeout);
				          
				reg_val = DRV_Reg32(NFI_CON);
				reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
				/* issue reset operation */
				DRV_WriteReg32(NFI_CON, reg_val);
				//				   DBG_MSG("%d %s 0x%x\n",__LINE__,__FUNCTION__,g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
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
	if(NUTL_DEVICE_NAME() == NAND_H27UCG8T2ETR)
	{
		if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) // slc mode
		{
			//LOGD("%s - line:%d\n", __func__, __LINE__);
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
				//				   DBG_MSG("%d %s 0x%x\n",__LINE__,__FUNCTION__,g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
			} 
		}
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

	do{
		page_size = NUTL_PAGE_SIZE();
		spare_size = NUTL_SPARE_SIZE();
		column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
		addr_cycle = NUTL_ADDR_CYCLE();
		sectors = (NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE)/logical_plane_num;
		// 1. Init NFI
		NFI_Reset();

		// 1.1 reset NFI
#if defined(BB_MT0699)
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
#endif
		//DBG_MSG("row_addr 0x%x\n",row_addr);

		NAND_COMMON_Interface_Config(TRUE);
		NFI_Reset();
		//DBG_MSG("NFI_NAND_TYPE_CNFG 0x%x\n",*NFI_NAND_TYPE_CNFG);

#ifdef DISABLE_NFI_DMA
		bUsingDMA = FALSE;
#else
		bUsingDMA = TRUE;
#endif

		bUsingDMA = TRUE;//Set False for FPGA test!
		*NFI_INTR_EN = INTR_AHB_DONE_EN;

		// 1.2 read destination address
		intr = *NFI_INTR;
		*NFI_STRADDR = ((uint32)(p_data32)) + colu_addr;//QQ
		// 1.3 enable ECC Decoder
		// u4DECCODESize = (NAND_SECTOR_SIZE + 8) << 3 + 4 * 13;
		*ECC_DECCNFG |= (ECC_CNFG_NFI | DEC_CNFG_EMPTY_EN | DEC_CNFG_CORRECT | 0x400); // | (u4DECCODESize << DEC_CNFG_CODE_SHIFT));  mark empty en to fix empty page read fail   xiaolei
		STOP_ECC_DEC
		START_ECC_DEC

		// 1.4 set CNFG
		//*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
		*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
		*NFI_CNFG |= ((bUsingDMA)?(CNFG_AHB|CNFG_AHB_BURST):0);				// AHB mode or NFI mode

		// Ray's todo:	
		if( disautofmt == FALSE)
			*NFI_CNFG |= CNFG_AUTO_FMT_EN;	// if read for verify, use auto format; if read back raw data, don't use it!

		if(use_randomizer)
			NAND_COMMON_Randomizer_Config(TRUE,row_addr);
		else
			NAND_COMMON_Randomizer_Config(FALSE,row_addr);
		// ?? *CNFG |= CNFG_BYTE_RW; ??

		// 2. Issue Read CMD
		// 2.1 read cmd 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd;
		// 2.2 wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout){ return STATUS_NAND_TIMEOUT; }		

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
		if(!timeout){ return STATUS_NAND_TIMEOUT; }

		// 4. Issue Read Confirm CMD
		if(g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_enable)
		{
			// 4.1 read confirm 
			*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd;
			// 4.2 wait til CMD is completely issued 
			timeout = c_timeout;
			NFI_Wait_Command(timeout);
			if(!timeout){ return STATUS_NAND_TIMEOUT; }			
		}

		// 5. Wait for Read Buffer is ready
		// wait til device is from busy to ready
		timeout = c_timeout;
		NFI_Wait_Return_Ready(timeout);
		if(!timeout){ return STATUS_NAND_TIMEOUT; }	

        while(logical_plane_index < logical_plane_num)
		{
#if defined(__NFI_SUPPORT_TLC__)

			if( (g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
				&& (g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn) )
			{
				real_row_addr = NFI_TLC_SetpPlaneAddr(real_row_addr, (logical_plane_index == 0)?TRUE:FALSE);
				//LOGD("%d %s %x\n",__LINE__,__FUNCTION__,real_row_addr);
				page_addr = real_row_addr;
				colu_addr = (logical_plane_index == 0)?0:(NUTL_PAGE_SIZE()/2);
			}
#endif

			*NFI_STRADDR = ((uint32)(p_data32)) + colu_addr;//QQ
			if(logical_plane_index > 0 )
			{
				*NFI_INTR_EN = INTR_AHB_DONE_EN;
				START_ECC_DEC
			}

#if defined(__NFI_SUPPORT_TLC__)
			if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
				&&(g_pNandInfo->m_dev->m_tlc_setting->needchangecolumn))
			{
				NUTL_TLC_ChangeColumn(&real_row_addr, (logical_plane_index ==0)?TRUE:FALSE);
			}
#endif
			// 6. set NFI_CON
			// 6.1 set burst read by DWORD 
			*NFI_CON = (CON_NFI_BRD | (sectors << CON_NFI_SEC_SHIFT));

			// 6.2 wait til DATA_READ is completely issued 
			//wait til Device is ready
			timeout=c_timeout;
			NFI_Wait_Ready(timeout);

#if defined(__NFI_SUPPORT_TLC__)
			if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
				&&(g_pNandInfo->m_dev->m_tlc_setting->needchangecolumn))
			{
				DRV_WriteReg(NFI_TLC_RD_WHR2, (TLC_RD_WHR2_EN | 0x015)); //trigger data sample
			}
#endif

			// 7. Read data
			// read page data
			if( STATUS_OK != (ret=NUTL_FIFO_Read_V5(c_timeout, bUsingDMA, p_data32+colu_addr/4, page_size/logical_plane_num)) ) {
				//goto end;
			}

			if( disautofmt == FALSE)
				NAND_READ_FDM_DATA(p_spare32+(logical_plane_index*sectors*8/4), sectors);//QQ. need recal FDM in tlc mode
			if((*NFI_STA & STA_READ_EMPTY) != 0)
			{

				//DBG_MSG("[pg] %d empty\n", page_addr);
				if(bad_block_remark_done)
				{
					//DBG_MSG("[pg] reset 0xFF\n");
					memset(p_spare32, 0xFF, (sectors*8));
				}
			}
	else
	{
		if(ftl_img)
		{
			LOGD("last 32B: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \n",
				p_data32[4088],p_data32[4089],p_data32[4090],p_data32[4091],
			p_data32[4092],p_data32[4093],p_data32[4094],p_data32[4095]);
		}
	}

#if defined(__NFI_SUPPORT_TLC__)
			if((g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC)
				&&(g_pNandInfo->m_dev->m_tlc_setting->needchangecolumn))
			{
				DRV_WriteReg(NFI_TLC_RD_WHR2, (0x015)); //disable trigger data sample
			}
#endif

			logical_plane_index++;
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
#if 0

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


#if 0 //def DUMP_SPEED
    g_ds_nand_page_read_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_read_time += (tick_end - tick_start);
#endif

#if defined (BB_MT0699)
	if(NUTL_DEVICE_MLC() && g_pNandInfo->m_dev->m_nand_type  != NAND_FLASH_TLC)
	{
#if 0
		memcpy((void*)(p_spare32+8), &dbg_MLC, sizeof(struct debug_info_MLC));
#endif
	}
#endif
	// 8. Stop ECC Decode
	if(use_randomizer)
	{
		if(NUTL_NAND_TYPE() != NAND_FLASH_TLC)
			NAND_COMMON_Turn_Off_Randomizer();

		/*
		if((DRV_Reg32(NFI_STA) & STA_READ_EMPTY) > 0) //mark this because cmd setting will trigger NFI to reset nfi_sta sta_read_empty xiaolei
		{
		if((*NFI_CNFG & CNFG_AUTO_FMT_EN) != 0)
		memset((void*)p_data32, 0xFF, page_size);
		else
		memset((void*)p_data32, 0xFF, page_size + spare_size);
		ret = STATUS_OK;
		}
		*/
	}
#if defined(__NFI_SUPPORT_TLC__)
	if(g_pNandInfo->m_dev->m_nand_type  == NAND_FLASH_TLC)
	{
		if((g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn  && NFI_IsSLC_Blk(row_addr/u4PgPerBl))
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
	if(NUTL_DEVICE_NAME() == NAND_H27UCG8T2ETR)
	{
		if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) // slc mode
		{
			if(0xFF != g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd)
			{
				reg_val = DRV_Reg(NFI_CNFG);
				reg_val &= ~CNFG_READ_EN;
				reg_val &= ~CNFG_OP_MODE_MASK;
				reg_val |= CNFG_OP_CUST;
				DRV_WriteReg(NFI_CNFG, reg_val);

				DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd);
				NFI_Wait_Command(timeout);

				reg_val = DRV_Reg32(NFI_CON);
				reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
				/* issue reset operation */
				DRV_WriteReg32(NFI_CON, reg_val);
				//				   DBG_MSG("%d %s 0x%x\n",__LINE__,__FUNCTION__,g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
			}
		}
	}
	//LOGD(" qq read end  ,line=%d,function= %s,row_addr=0x%x, p_data32=0x%x, data 0x%x, 0x%x,fgslc=%d \n ", 
	//	__LINE__,  __FUNCTION__, row_addr,p_data32,*p_data32, *(p_data32+16384/4 - 1),g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn);
	//comment from xiaolei: 
	//For read retry mechanism, i changed the return value from STATUS_OK to S_DA_NAND_ECC_2BITS_ERR when it occurs ECC error.
	//This results in Format Fail when Force Erase is not selected.
	//Setting ret as STATUS_OK here is just a workaround with less risk to meet the orginal logical. 
	ret = STATUS_OK; 
	//use_randomizer = TRUE;//FIX

	return ret;
}

status_t  NAND_COMMON_PageRead_V5(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32 *p_spare32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) 
{
	
	uint8* main_ptr = NULL;
	uint8* spare_ptr = NULL;
	U32  sector_size = NAND_SECTOR_SIZE;
	//status_t  status	= STATUS_OK;
	//uint32  ecc_parity_from_reg[8];
	U32   nfi_sector_num = g_pNandInfo->m_dev->m_hw_info.m_page_size /g_pNandInfo->m_dev->m_hw_info.m_sector_size;
	U32 page_per_block = NUTL_PAGES_PER_BLOCK();
	uint8 oob[16*8];
	uint32 fdm, left, i;

	sector_size  = g_pNandInfo->m_dev->m_hw_info.m_sector_size;

    	main_ptr = (P_U8)p_data32;
    	spare_ptr = (P_U8)p_spare32;

	tlc_snd_phyplane = FALSE;
	NAND_COMMON_PageRead_V5_hw(c_timeout, row_addr, (uint32*)main_ptr, (uint32*)spare_ptr, ecc_parity_from_reg);

	if (ftl_img)
	{
		fdm = ((*NFI_PAGEFMT & PAGEFMT_FDM_MASK) >> PAGEFMT_FDM_SHIFT);

		for(i=0;i<nfi_sector_num;i++)
		{
			memcpy(oob + (fdm*i), spare_ptr + (8*i), fdm);
		}
		memcpy(spare_ptr, main_ptr + g_pNandInfo->m_dev->m_hw_info.m_page_size - fdm*nfi_sector_num, 32);
		memcpy(main_ptr + g_pNandInfo->m_dev->m_hw_info.m_page_size - fdm*nfi_sector_num, oob, fdm*nfi_sector_num);	
	}

	return STATUS_OK;
}


//------------------------------------------------------------------------------
// Read From NFI FIFO                                                           
//------------------------------------------------------------------------------

status_t  NUTL_FIFO_Read_V5(
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

	sectors				= data_len/NAND_SECTOR_SIZE;
	sector_page_size32 	= (data_len/sectors)>>2;
	sector_spare_size32 = (NUTL_SPARE_PER_SEC_SIZE())>>2;
	sector_pagespare_size32 = sector_page_size32 + sector_spare_size32;
	if(NAND_SECTOR_SIZE == 1024)
		sector_shift+=1;
	LOGV("[%s] line %d (gslc %d)\n", __func__,__LINE__,(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn));

	if(bUsingDMA) {	// AHB mode	
		uint32 reg_val;
		uint32 read_bytes;

		timeout = c_timeout;
		// while( (ADDRCNTR_CNTR((*NFI_ADDRCNTR)) < sectors) && (--timeout));

		timeout = 0xffff;
		// while( (read_bytes < data_len) && (timeout>0) ); 

        while(*NFI_INTR == 0)
			;
		while ((data_len >> sector_shift) > ((*NFI_BYTELEN & 0x1F000) >> 12))
			;

		#if 0
		dec_mask = (1 << sectors) - 1;
		while (dec_mask != (*ECC_DECDONE))
			;
		#else // to fix ecc timeout issue xiaolei
		dec_mask = (1 << (sectors-1)); 
		#if defined(BB_MT0699)
		while ((0 == (dec_mask & (*ECC_DECDONE))) ||(0x01011101 != ((*ECC_DECFSM) & 0x3F3FFF0F)))
					;
		#else
		while ((0 == (dec_mask & (*ECC_DECDONE))) ||(0x01010101 != ((*ECC_DECFSM) & 0x7F0F0F0F)))
			;
		#endif
		#endif
		*NFI_INTR_EN = 0;
		 fail_sector = 0;
#if defined(BB_MT0699)
		dbg_MLC.sector_number = sectors;
		for(sector=0; sector < sectors; sector++)
		{
			ErrNum = (DRV_Reg32((ECC_DECENUM0+(sector/4)))>>((sector%4)*8))& ERR_NUM0;
			LOGV("%s - sector %d, ErrNum %d\n",__func__, sector, ErrNum);
			dbg_MLC.sector_bit_flip_num[sector] = ErrNum;
			if(ERR_NUM0 == ErrNum)
			{
				dbg_MLC.sector_ecc_bitmap |= 1<<sector;
				fail_sector++;
			}
		}
#endif
		LOGV("%s - (%d), fail_sector %d, sectors %d\n", __func__, __LINE__,fail_sector, sectors);
		if(fail_sector == sectors)
		{
			if((*NFI_STA & STA_READ_EMPTY) != 0)
			{
				if(bad_block_remark_done)
				memset(p_data32, 0xFF, data_len);
				LOGD("EMPTY PAGE!\n");
				fail_sector = 0;
#if defined(BB_MT0699)
				dbg_MLC.sector_ecc_bitmap = 0;
#endif
			}
		}
		if(fail_sector) {
				LOGD("ECC FAIL\n");
			STOP_ECC_DEC
		}
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
				if( !timeout ) { return STATUS_NAND_TIMEOUT;}
					
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
				if( !timeout ) { return STATUS_NAND_TIMEOUT;}
					
				//*(p_data32 + sector_page_size32*sectors + sector_spare_size32*sector + i) = *NFI_DATAR;
				*(p_data32 + sector_pagespare_size32*sector + sector_page_size32 + i) = *NFI_DATAR;
			}			

			//MT6516 FPGA: Ray's todo: ECC Correct
			// decoder done
			dec_mask = 1 << sector;		  
			while (!(dec_mask & (*ECC_DECDONE)))
				;
			ErrNum = (DRV_Reg32((ECC_DECENUM0+(sector/4)))>>((sector%4)*8))& ERR_NUM0;
			if (ErrNum && ErrNum != ERR_NUM0)
			{
				for (i = 0; i < ErrNum; i++)
				{
					ErrBitLoc = *(ECC_DECEL0 + i / 2);
				}
			}
			else
			{
				 fail_sector ++;
			}
		}
		if((*NFI_STA & STA_READ_EMPTY) != 0)
		{
			if(bad_block_remark_done)//(g_nand_chip.bmt_exist)
			memset(p_data32, 0xFF,(data_len+NUTL_SPARE_SIZE()));
		}

		STOP_ECC_DEC

		
	}
#if defined(BB_MT0699)
	if( fail_sector == 0)
		return STATUS_OK;
	else
		return STATUS_OK;
#else
	return STATUS_OK;
#endif
}

static void NAND_WRITE_FDM_DATA(uint32 *p_spare32, uint32 sectornum)
{
	uint32 *fdm = (uint32*)p_spare32;
	uint32 i;

	LOGV("NFI_PAGEFMT : 0x%x\n", (*(NFI_PAGEFMT)));
	for(i=0; i<(sectornum*2); i+=4)
	{
		LOGV("0x%x 0x%x 0x%x 0x%x\n", p_spare32[i], p_spare32[i+1], p_spare32[i+2], p_spare32[i+3]);
	}

	for(i=0; i<(sectornum*2); i++)
	{
		*(NFI_FDM0L+i) = fdm[i];
	}

	//for(i=0; i<sectornum; i++)
	//{
	//	LOGV("NFI_FDM%dL : 0x%x, NFI_FDM%dM : 0x%x\n", i, (*(NFI_FDM0L+i*2)), i, (*(NFI_FDM0M+i*2)));
	//}
}

status_t  NAND_COMMON_PageProgram_V5(
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
	uint32		io_interface;
	uint32		*fdm;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	status_t	ret=STATUS_NAND_ERR;
	bool		bUsingDMA=TRUE;
	uint32     page_addr = (uint32)row_addr;

#if defined(__NFI_SUPPORT_TLC__)
	NFI_TLC_WL_INFO  tlc_wl_info = {0};
	U32 reg_val = 0;
	//bool  fgOriSlcopmodeEn = g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn ;
	//if(NULL == p_data32)
	//{
	//  g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn = TRUE;
	//}
#endif
	LOGV("function=%s, row_addr=0x%x fgslc %d\n", __FUNCTION__, row_addr, (g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn));

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();
	sectors = NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;

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
	
	// [Cache_Program] If use cache program, can't send other command to nand
	if (!use_cache_write)
		NAND_COMMON_Interface_Config(TRUE);

	NFI_Reset();

#if ( DA_MODE == DA_MODE_FPGA)
	if (1 == (DRV_Reg32(NFI_NAND_TYPE_CNFG) & 1)) {
		DRV_WriteReg32(NFI_DQS_HIGH_SET, 0x80000000);
	}
#endif

#ifdef DISABLE_NFI_DMA
	bUsingDMA = FALSE;
#else
	bUsingDMA = TRUE;
#endif
	bUsingDMA = TRUE;//Set False for FPGA test!

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
		}
		else
		{
			page_addr = NFI_TLC_GetRowAddr(row_addr);
		}

		if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) // slc mode
		{
			if((!g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn) || tlc_lg_left_plane)
			{
				if(0xFF != g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd)
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
	if(NUTL_DEVICE_NAME() == NAND_H27UCG8T2ETR)
	{
		if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) // slc mode
		{
			if(0xFF != g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd)
			{
				reg_val = DRV_Reg(NFI_CNFG);
				reg_val &= ~CNFG_READ_EN;
				reg_val &= ~CNFG_OP_MODE_MASK;
				reg_val |= CNFG_OP_CUST;
				DRV_WriteReg(NFI_CNFG, reg_val);
				if(g_pNandInfo->m_dev->m_tlc_setting->normalslcopen)
					DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->normal_slc_mode_cmd);
				else
					DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
				NFI_Wait_Command(timeout);

				reg_val = DRV_Reg32(NFI_CON);
				reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
				/* issue reset operation */
				DRV_WriteReg32(NFI_CON, reg_val);
				//DBG_MSG("%d %s 0x%x\n",__LINE__,__FUNCTION__,g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
			}
		}
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
	*NFI_CNFG |= ((bUsingDMA)?CNFG_AHB:0);									// AHB mode or NFI mode
	*NFI_CNFG |= ((bUsingDMA)?CNFG_AHB_BURST:0);

	if(NUTL_NAND_TYPE() != NAND_FLASH_TLC)
	{
		if(use_randomizer)
			NAND_COMMON_Randomizer_Config(TRUE,row_addr);
	}
	else
		NAND_COMMON_Randomizer_Config(TRUE,row_addr);

	// ?? *CNFG |= CNFG_BYTE_RW; ??
	// 2. Issue 1st Half Write CMD
	// in most 512 page size NAND flash, you have to setup destination pointer to 1st half area 
	if(g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_enable)
	{
		// 2.1 1st half program cmd 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_cmd;
		// 2.2 wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout){ return STATUS_NAND_TIMEOUT; }	
			
		// 2.3 Reset for ambigous of read command and 1st-half program command
		NFI_Reset();
	}

	// 3. Issue Program CMD
	// 3.1 program cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd;
	// 3.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return STATUS_NAND_TIMEOUT; }

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
	if(!timeout){ return STATUS_NAND_TIMEOUT; }
#if defined(__NFI_SUPPORT_TLC__)
	if(g_pNandInfo->m_dev->m_nand_type  == NAND_FLASH_TLC)
	{
		reg_val = DRV_Reg32(NFI_DEBUG_CON1);
		reg_val |= 0x4000; //enable ce hold
		DRV_WriteReg(NFI_DEBUG_CON1, reg_val);
	}
#endif

#if 0
	NAND_WRITE_FDM_DATA(p_spare32, sectors);
#else
	fdm = (uint32*)p_spare32;
	
	for(i = 0; i < sectors; i++)
	{
		*(NFI_FDM0L + (i << 1)) = *(fdm + (i<<1));
		*(NFI_FDM0M + (i << 1)) = *(fdm + (i<<1) + 1);
		if(ftl_img)
			LOGD("FDM[%d] 0x%x 0x%x\n", i, *(NFI_FDM0L + (i << 1)),*(NFI_FDM0M + (i << 1)));
	}
#endif

	// 5. set NFI_CON
	// set burst program by DWORD 
	*NFI_CON = (CON_NFI_BWR | (sectors << CON_NFI_SEC_SHIFT));

	// 6. Program data
       if(p_data32 != NULL)//  for TLC sandisk flash , bad block detect
	if( STATUS_OK != (ret=NUTL_FIFO_Write_V5(bUsingDMA, p_data32, page_size,spare_size, NUTL_PROGRAM_PAGE_ONLY)) ) {
		return ret;
	}

	// <<<<  WARNING!! >>>>                                                              
	// 1. You MUST read parity registers before issue program confirm (0x10) command.    
	//    Since the parity registers will be clean by NFI after issue program confirm.   
	// 2. You MUST wait until the NFI FIFO is empty!                                     
	//    It means all data in the FIFO had been written to NAND flash, and then you can 
	//    start to read ECC parity registers.                                            
	timeout = c_timeout;

	// 7. Stop ECC Encode
	timeout = c_timeout;
	WAIT_ENC_READY_WITH_TIMEOUT(timeout);
	if(!timeout){ return STATUS_NAND_TIMEOUT; }

	timeout = c_timeout;
	while( FIFO_WR_REMAIN( *NFI_FIFOSTA) && (--timeout) );
	STOP_ECC_ENC
#if defined(__NFI_SUPPORT_TLC__)
	if( (g_pNandInfo->m_dev->m_nand_type == NAND_FLASH_TLC) && !tlc_lg_left_plane)
	{
		reg_val = DRV_Reg32(NFI_DEBUG_CON1);
		reg_val &= (~0x4000); //disable ce hold
		DRV_WriteReg(NFI_DEBUG_CON1, reg_val);
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
				if((tlc_wl_info.wl_pre == WL_HIGH_PAGE) || g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn)
				{
					if (use_cache_write)
					{
						DRV_WriteReg(NFI_CMD, CACHE_PROG_CMD);
					}
					else
					{
					DRV_WriteReg(NFI_CMD, PROG_DATA_CMD);
				}
				}
				else
				{
					DRV_WriteReg(NFI_CMD, PROGRAM_RIGHT_PLANE_CMD);
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
	if(!timeout){ return STATUS_NAND_TIMEOUT; }

	// 9. Wait for Program Buffer is ready
	timeout = c_timeout;

#if 0
	if( (g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn) && !tlc_lg_left_plane)
	{
		while( (STATUS_NAND_IN_PROGRESS ==(ret = NAND_COMMON_ReadStatus_V5(c_timeout))) && (timeout--));
		if(!timeout){ return STATUS_NAND_TIMEOUT; }
	}
	if (!g_pNandInfo->m_dev->m_tlc_setting->pPlaneEn)
	{
		while( (STATUS_NAND_IN_PROGRESS ==(ret = NAND_COMMON_ReadStatus_V5(c_timeout))) && (timeout--));
		if(!timeout){ return STATUS_NAND_TIMEOUT; }
	}
#else
	while((DRV_Reg32(NFI_STA_REG32) & STA_NAND_BUSY) && (timeout--));
	if(!timeout){ LOGE("[TT] STA_NAND_BUSY timeout! 0x%x\n", DRV_Reg32(NFI_STA_REG32));}
//return STATUS_NAND_TIMEOUT; }

#endif

	// 10. Stop ECC Encode
	//WAIT_ENC_READY
	STOP_ECC_ENC

	// 11.
	if(STATUS_OK != ret){ return STATUS_NAND_ERR;}

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
	if(NUTL_DEVICE_NAME() == NAND_H27UCG8T2ETR)
	{
		if(g_pNandInfo->m_dev->m_tlc_setting->slcopmodeEn) // slc mode
		{
			if(0xFF != g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd)
			{
				reg_val = DRV_Reg(NFI_CNFG);
				reg_val &= ~CNFG_READ_EN;
				reg_val &= ~CNFG_OP_MODE_MASK;
				reg_val |= CNFG_OP_CUST;
				DRV_WriteReg(NFI_CNFG, reg_val);

				DRV_WriteReg(NFI_CMD, g_pNandInfo->m_dev->m_tlc_setting->dis_slc_mode_cmd);
				NFI_Wait_Command(timeout);

				reg_val = DRV_Reg32(NFI_CON);
				reg_val |= CON_FIFO_FLUSH|CON_NFI_RST;
				/* issue reset operation */
				DRV_WriteReg32(NFI_CON, reg_val);
				//DBG_MSG("%d %s 0x%x\n",__LINE__,__FUNCTION__,g_pNandInfo->m_dev->m_tlc_setting->en_slc_mode_cmd);
			}
		}
	}

	return STATUS_OK;
}

//------------------------------------------------------------------------------
// Page+Spare Program Callback Function                                               
//------------------------------------------------------------------------------
status_t NAND_COMMON_PageSpareProgram_V5(
			uint32			row_addr,
			const uint32	*p_page_spare
			)
{
    const uint32 page_size = NUTL_PAGE_SIZE();
    const uint32 spare_size = NUTL_SPARE_SIZE();
    const uint32 addr_cycle = NUTL_ADDR_CYCLE();
    const uint32 sectors = NUTL_PAGE_SIZE() / NAND_SECTOR_SIZE;
    uint32 row_addr_nob, col_addr_nob;
    uint32 timeout;
    status_t ret;
    uint32     page_addr = (uint32)row_addr;
	//DBG_MSG("Drv Program.....%x\n",row_addr);
    NAND_COMMON_Interface_Config(TRUE);
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
    // Issue 1st Half Write CMD:
    //   For most 512 page size NAND flash, we have to set destination pointer
    //   to 1st half area.

    if(g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_enable)
    {
        // 1st half program cmd 
        *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_cmd;

        // Wait til CMD is completely issued
        timeout = NUTL_DEFAULT_TIMEOUT;
        NFI_Wait_Command(timeout);

        if (!timeout)
        {
            return STATUS_NAND_TIMEOUT;
        }
            
        // Reset for ambiguous of read command and 1st-half program command
        NFI_Reset();
    }
	if(NUTL_NAND_TYPE() != NAND_FLASH_TLC){
if(use_randomizer)
		NAND_COMMON_Randomizer_Config(TRUE,row_addr);
		}else
		NAND_COMMON_Randomizer_Config(TRUE,row_addr);
    //
    // Issue Program CMD
    //

    // Program cmd 
    *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd;

    // Wait til CMD is completely issued 
    timeout = NUTL_DEFAULT_TIMEOUT;
    NFI_Wait_Command(timeout);

    if (!timeout)
    {
        return STATUS_NAND_TIMEOUT;
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
        return STATUS_NAND_TIMEOUT;
    }

    // Set NFI_CON
    // set burst program by DWORD 
    *NFI_CON = (CON_NFI_BWR | (sectors << CON_NFI_SEC_SHIFT));

	// Program data
    if (STATUS_OK != (ret=NUTL_FIFO_Write_V5(FALSE, p_page_spare, page_size, spare_size, NUTL_PROGRAM_PAGE_SPARE)))
    {
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

#if ( DA_MODE == DA_MODE_FPGA)
	if (1 == (DRV_Reg32(NFI_NAND_TYPE_CNFG) & 1)) {
		DRV_WriteReg32(NFI_DQS_HIGH_SET, 0x00000000);
	}
#endif

    if (!timeout)
    {
        return STATUS_NAND_TIMEOUT;
    }

    // Wait for Program Buffer is ready
    timeout = NUTL_DEFAULT_TIMEOUT;
//if(use_randomizer)
//		NAND_COMMON_Turn_Off_Randomizer();
    while ((STATUS_NAND_IN_PROGRESS == (ret=NAND_COMMON_ReadStatus_V5(NUTL_DEFAULT_TIMEOUT))) && (timeout--));

    if (!timeout)
    {
        return STATUS_NAND_TIMEOUT;
    }

    if (STATUS_OK != ret)
    {
        return STATUS_NAND_ERR;
    }

    return STATUS_OK;
}

//------------------------------------------------------------------------------
// Write To NFI FIFO                                                            
//------------------------------------------------------------------------------

status_t  NUTL_FIFO_Write_V5(const bool bUsingDMA,
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
                return STATUS_NAND_TIMEOUT;
            }

            *NFI_DATAW = *(p_data32 + i);
        }

        // Wait for data ready 
        while(delay--);

        timeout = NUTL_DEFAULT_TIMEOUT;
        while ((FIFO_WR_REMAIN (*NFI_FIFOSTA)) && (--timeout));

        if (!timeout)
        {
            return STATUS_NAND_TIMEOUT;
        }
    }

	return STATUS_OK;
}

//------------------------------------------------------------------------------
// Spare Read Callback Function                                                 
//------------------------------------------------------------------------------
status_t  NAND_COMMON_SpareRead_V5(
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
//	status_t	ret=STATUS_NAND_ERR;
//	bool		bUsingDMA=TRUE;
	LOGD("[%s] row_addr (0x%x)\n", __func__, row_addr);

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();

	// reset NFI
	NFI_Reset();

	return STATUS_OK;
}

//------------------------------------------------------------------------------
// Spare Program Callback Function                                              
//------------------------------------------------------------------------------
status_t  NAND_COMMON_SpareProgram_V5(
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
//	status_t	ret=S_DA_NAND_UNKNOWN_ERR;
//	bool		bUsingDMA=TRUE;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();

	// reset NFI
	NFI_Reset();

	return STATUS_OK;
}

//------------------------------------------------------------------------------
// CopyBack Callback Function                                                   
//------------------------------------------------------------------------------
status_t  NAND_COMMON_CopyBack_V5(
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
	//status_t	ret=STATUS_NAND_ERR;

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

	return STATUS_OK;
}


//------------------------------------------------------------------------------
// Reset Device Function for DA_init stage
//------------------------------------------------------------------------------
status_t  NAND_COMMON_Reset_For_DaInit(
			const uint32  c_timeout
			)
{
	uint32		timeout = NFI_PSTA_TIMEOUT;

	// reset NFI
	NFI_Reset();

	// set NFI_CNFG
	*NFI_CNFG = CNFG_OP_RESET;
	// reset cmd 
	*NFI_CMD = RESET_CMD;	//Fix 0xFF for reset cmd

	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout) {
		return STATUS_NAND_TIMEOUT;
	}

	// wait for reset finish 
	timeout = c_timeout;
	NFI_Wait_Ready(timeout);
	if(!timeout) {
		return STATUS_NAND_TIMEOUT;
	}

	return STATUS_OK;
}

//------------------------------------------------------------------------------
// Read Device ID Function for DA_init stage                                             
//------------------------------------------------------------------------------
status_t  NAND_COMMON_ReadID_For_DaInit(
			const uint32  c_timeout,
			uint32* id1,
			uint32* id2
			)
{
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32		timeout_tick, start_tick;
	uint32		nfi_pagefmt = 0;
	uint16		reg_val;

	//LOGD("[%s]\n", __func__);

	DRV_WriteReg(NFI_CSEL, 0);
	DRV_WriteReg32(NFI_ACCCON, 0x10818022);
	//LOGD("NFI_ACCCON 0x%x\n", *NFI_ACCCON);
	reg_val = DRV_Reg(NFI_CNFG);
	reg_val &= ~CNFG_AHB; //It can't be AHB mode for WAIT_NFI_PIO_READY.
	DRV_WriteReg(NFI_CNFG, reg_val);
	//LOGD("LINE %d NFI_CNFG 0x%x\n", __LINE__,*NFI_CNFG);

	NFI_Reset();

	/* Issue NAND chip reset command for Micron's MCP */
	NFI_ISSUE_COMMAND(RESET_CMD, 0, 0, 0, 0);
	while(DRV_Reg(NFI_STA)&STA_NAND_BUSY)
		;

	NAND_COMMON_Switch_Async_For_DaInit();

	NFI_Reset();
	nfi_pagefmt = *NFI_PAGEFMT;
	DRV_WriteReg32(NFI_PAGEFMT, 0);
	/* Config single read and read enable bit in NFI_CNFG register */
	DRV_WriteReg(NFI_CNFG, (CNFG_OP_SRD | CNFG_READ_EN));

	/* read NAND flash ID */
	DRV_WriteReg(NFI_CMD, RD_ID_CMD);
	/* wait for the command status ready */
	while (DRV_Reg32(NFI_STA) & STA_CMD_STATE)
		;

	/* Set addr registers */
	DRV_WriteReg32(NFI_COLADDR, 0);
	DRV_WriteReg32(NFI_ROWADDR, 0);
	DRV_WriteReg(NFI_ADDRNOB, 1);
	/* wait for the address status ready */
	while (DRV_Reg32(NFI_STA) & STA_ADDR_STATE)
		;

	/* Trigger read operations */
	DRV_WriteReg(NFI_CON, CON_NFI_SRD);
	while(DRV_Reg32(NFI_STA) & STA_DATAR_STATE)
		;

	/* Check the PIO bit is ready or not */
	while(!(0x1 & (*NFI_PIO_DIRDY)))
		; 
	//WAIT_NFI_PIO_READY(timeout);  

	/* If status is ready, read back the NFI_DATAR register */
	*id1 = DRV_Reg32(NFI_DATAR);
	*id2 = DRV_Reg32(NFI_DATAR);

    DRV_WriteReg(NFI_CON, 0x0);

	// restore original page format setting 
	*NFI_PAGEFMT = nfi_pagefmt;

    return STATUS_OK;
}

status_t NAND_COMMON_Switch_Async_For_DaInit()
{
	uint32 reg1, reg2, reg4;

	//LOGD("%s\n", __func__);

	NFI_Reset();

	reg1 = DRV_Reg32(NFI_DEBUG_CON1);
	reg2 = DRV_Reg32(ECC_BYPASS);
	//reg3 = DRV_Reg32(NFI_1X_INFRA);
	//LOGD("regs 0x%x, 0x%x, 0x%x\n", reg1, reg2, reg3);

	//reg3 &= ~0x80;
	reg1 &= ~0x8000;
	reg2 &= ~0x1;
	reg4 = 0;
	//DRV_WriteReg32(NFI_1X_INFRA,reg3);
	DRV_WriteReg(ECC_BYPASS,reg2);
	DRV_WriteReg(NFI_DEBUG_CON1,reg1);
	DRV_WriteReg32(NFI_ACCCON,0x31C083F9);
	while(0 == (DRV_Reg(NFI_STA) && STA_FLASH_MACRO_IDLE))
		;
up1:
	DRV_WriteReg32(NFI_NAND_TYPE_CNFG,reg4);
	//LOGD("NFI_NAND_TYPE_CNFG 0x%x reg 0x%x\n",DRV_Reg(NFI_NAND_TYPE_CNFG),reg4);
	if(DRV_Reg(NFI_NAND_TYPE_CNFG) == 1)
		goto up1;

	return STATUS_OK;
}

#if 0
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
#endif



#endif
