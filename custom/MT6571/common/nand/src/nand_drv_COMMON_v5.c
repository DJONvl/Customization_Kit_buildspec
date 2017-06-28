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


#define DRV_WriteReg(addr,data)     ((*(volatile uint16 *)(addr)) = (uint16)data)
#define DRV_Reg(addr)               (*(volatile uint16 *)(addr))
#define DRV_WriteReg32(addr,data)     ((*(volatile uint32 *)(addr)) = (uint32)data)
#define DRV_Reg32(addr)               (*(volatile uint32 *)(addr))
#define DRV_WriteReg8(addr,data)     ((*(volatile uint8 *)(addr)) = (uint8)data)
#define DRV_Reg8(addr)               (*(volatile uint8 *)(addr))


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

#ifdef DUMP_SPEED
extern U32 gpt4_get_current_tick (void);
extern uint32 g_ds_nand_block_erase_number,  g_ds_nand_block_erase_time;
extern uint32 g_ds_nand_page_program_number, g_ds_nand_page_program_time;
extern uint32 g_ds_nand_page_read_number,    g_ds_nand_page_read_time;
#endif
bool use_randomizer = FALSE;
bool device_need_rand = FALSE;

#if defined(BB_MT6571)
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
#endif
//------------------------------------------------------------------------------
// COMMON Callback Function Set                                                 
//------------------------------------------------------------------------------
const NAND_CMD_Callback_S	g_NAND_COMMON_CB_FUNC_SET={
	ID_COMMON_CB_FUNC_SET
	,NAND_COMMON_ReadID_V5
	,NAND_COMMON_Reset_V5
	,NULL
	,NAND_COMMON_ReadStatus_V5
	,NAND_COMMON_BlockErase_V5
	,NAND_COMMON_BadBlockSymbol_Check_V5
	,NAND_COMMON_BadBlockSymbol_Set_V5
	,NAND_COMMON_PageRead_V5
	,NAND_COMMON_PageProgram_V5
	,NAND_COMMON_SpareRead_V5
	,NAND_COMMON_SpareProgram_V5
	,NAND_COMMON_PageSpareProgram_V5
	,NAND_COMMON_Randomizer_Drv_Config
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
#if 1
void NAND_COMMON_Set_Feature(uint8 cmd,uint8 addr, uint8 *value,  uint8 bytes)
{
	uint16           reg_val     	 = 0;
	uint8            write_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;

	NFI_Reset();

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
	
}

void SAL_NFI_GetFeature(uint8 cmd,uint8 addr, uint8 *value,  uint8 bytes)
{
	uint16           reg_val     	 = 0;
	uint8            read_count     = 0;
	uint32           timeout=NFI_PSTA_TIMEOUT;//0xffff;

	NFI_Reset();

	reg_val |= (CNFG_OP_CUST | CNFG_BYTE_RW | CNFG_READ_EN);
	DRV_WriteReg(NFI_CNFG, reg_val);

	NFI_ISSUE_COMMAND(cmd, addr, 0, 1, 0)

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
}
#endif

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


#if 1
STATUS_E NAND_COMMON_Randomizer_Drv_Config(bool use, uint32 pageNo)
{
	if(device_need_rand)
		use_randomizer = use;
	else
		use_randomizer = FALSE;
	return S_DONE;
}
STATUS_E NAND_COMMON_Randomizer_Config(bool use, uint32 pageNo)
{
    uint16   nfi_cnfg = 0;
	uint32   nfi_ran_cnfg = 0;
	uint8 i;
	uint32 page_in_block;
	uint16 seed;
//DBG_MSG("Randomizer %d \n",use);
    /* set up NFI_CNFG */
	if(device_need_rand)
	{
	    	nfi_cnfg = DRV_Reg(NFI_CNFG);
		//nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
		
		if(use)
		{
			
			if(NUTL_PAGES_PER_BLOCK()==256)
			{
				seed = randomizer_table[pageNo%128];
			}
			else
			{
				seed = randomizer_table[pageNo%NUTL_PAGES_PER_BLOCK()];
			}
			//DBG_MSG("pageNo %d  %d seed 0x%x\n",pageNo,pageNo%128,seed);
			nfi_ran_cnfg = 0;
			nfi_ran_cnfg |= seed << EN_SEED_SHIFT;
			nfi_ran_cnfg |= seed << DE_SEED_SHIFT;
			nfi_cnfg |= CNFG_RAN_SEC;
			nfi_cnfg |= CNFG_RAN_SEL;
			
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
	}
	return S_DONE;
}	
static bool NAND_COMMON_Randomizer_Status()
{
	uint32   nfi_ran_cnfg = 0;
	nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
	if(nfi_ran_cnfg&0x00010001)
		return TRUE;

	return FALSE;
}
static void NAND_COMMON_Turn_On_Randomizer()
{
	uint32   nfi_ran_cnfg = 0;
	if(device_need_rand)
	{
		nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
		nfi_ran_cnfg |= 0x00010001;
		DRV_WriteReg32(NFI_RANDOM_CNFG_REG32, nfi_ran_cnfg);
	}
}
static void NAND_COMMON_Turn_Off_Randomizer()
{
	uint32   nfi_ran_cnfg = 0;
	if(device_need_rand)
	{
		nfi_ran_cnfg = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
		nfi_ran_cnfg &= ~0x00010001;
		DRV_WriteReg32(NFI_RANDOM_CNFG_REG32, nfi_ran_cnfg);
	}
}

#endif

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
U8 real_hynix_rr_table_idx_16nm = 0;

static bool hynix_rr_table_select_16nm(U8 table_index)
{
	uint32 i;
	
	for(i = 0; i < 32; i++)
	{
		if(0xFF != (hynix_rr_table_16nm.rr_table[table_index].rr_table[i] ^ hynix_rr_table_16nm.rr_table[table_index].inversed_rr_table[i]))
			return FALSE; // error table
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

	if(device_need_rand)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
		DRV_WriteReg32(NFI_RANDOM_CNFG_REG32, (u4RandomSetting & (~0x00010001)));
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

	if(device_need_rand)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG_REG32, u4RandomSetting);
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

	if(device_need_rand)
	{
		u4RandomSetting = DRV_Reg32(NFI_RANDOM_CNFG_REG32);
		DRV_WriteReg32(NFI_RANDOM_CNFG_REG32, (u4RandomSetting & (~0x00010001)));
	}
	
	reg_val = (CNFG_OP_CUST | CNFG_BYTE_RW);
	DRV_WriteReg(NFI_CNFG, reg_val);

    //DRV_WriteReg(NFI_CMD,0x36);        
    //while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);

	NFI_Reset();    

    for(reg_cunt=0; reg_cunt<4; reg_cunt++){
        
        DRV_WriteReg(NFI_CMD,0x36);        
        while (DRV_Reg32(NFI_STA) & STA_CMD_STATE);
        //udelay(2);
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
        reg_val = hynix_rr_table_16nm.rr_table[real_hynix_rr_table_idx_16nm].rr_table[rr_index*4+reg_cunt];
        udelay(2);

        DRV_WriteReg32(NFI_DATAW, reg_val);
        
        //DBG_MSG("W[%x]0x%x ",(0x38+reg_cunt),reg_val);

    	NFI_Reset();
    }
    
    //NFI_Reset();
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

	if(device_need_rand)
	{
		DRV_WriteReg32(NFI_RANDOM_CNFG_REG32, u4RandomSetting);
	} 
    
}

#endif
	
#endif
 
//------------------------------------------------------------------------------
// Read Device ID Callback Function                                             
//------------------------------------------------------------------------------
#define GPIO_MODE7_NFI        ((volatile uint32*)(GPIO_base+0x0370)) //3,2,1,0
#define GPIO_MODE8_NFI        ((volatile uint32*)(GPIO_base+0x0380)) //4,5,6,7,8,9,10,11
#define GPIO_MODE9_NFI        ((volatile uint32*)(GPIO_base+0x0390)) //10, ALE, CLE, 4,5,7,14
#define GPIO_MODE10_NFI       ((volatile uint32*)(GPIO_base+0x03A0))
#define GPIO_NFI_DRV          ((volatile uint32*)(0x10015060))
STATUS_E  NAND_COMMON_ReadID_V5(
                const uint32  c_timeout
                ,uint32* id1
		   ,uint32* id2 //add by jiequn,2011.11.11
) {
    uint32      timeout = NFI_PSTA_TIMEOUT;
    //uint32      id1 = 0;
    uint16      nfi_pagefmt = 0;
    uint16      reg_val;
    STATUS_E    ret=S_DA_NAND_UNKNOWN_ERR;
    device_need_rand = FALSE;
    uint32 timeout_tick,start_tick;
	 
    DRV_WriteReg(NFI_CSEL, 0);
/*modify by jiequnchen,2011.11.16*/
#if defined(MT6572) || defined(BB_MT6571) 
  // Modify by Kuohong, use H8CS0SI0MP for Golden timing.
	DRV_WriteReg32(NFI_ACCCON, 0x31C083F9);
#else
    DRV_WriteReg32(NFI_ACCCON, 0x44333/*0x33223*/);
#endif

    reg_val = DRV_Reg(NFI_CNFG);
    reg_val &= ~CNFG_AHB; //It can't be AHB mode for WAIT_NFI_PIO_READY.
    DRV_WriteReg(NFI_CNFG, reg_val);

#if 1
    /* Reset NFI state machine */
    NFI_Reset();

    /* Issue NAND chip reset command for Micron's MCP */
    NFI_ISSUE_COMMAND(RESET_CMD, 0, 0, 0, 0);
    timeout_tick = gpt4_time2tick_ms(1); //1ms
    start_tick = gpt4_get_current_tick();
    while(DRV_Reg(NFI_STA)&STA_NAND_BUSY && !gpt4_timeout_tick(start_tick,timeout_tick))
        ;
    start_tick = gpt4_get_current_tick()-start_tick;
    DBG_MSG("reset takes %d, status 0x%x\n",start_tick,*NFI_STA); //for record
#endif    

    // reset NFI
    NFI_Reset();
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
	if(*id1 == 0x9384D745 ||*id1 ==  0x9384D798 ||*id1 ==  0x9394DE45)
		device_need_rand = TRUE;
	if(*id1 == 0xA794DEAD ||*id1 ==  0xA714DEAD)
		device_need_rand = TRUE;
//    *p_maker_code = (id1&0xFF);
//    *p_device_code = ((id1>>8)&0xFF);
//    *p_ext_code1 = ((id1>>16)&0xFF);
//    *p_ext_code2 = ((id1>>24)&0xFF);

#if 0   
    // set NFI_CNFG
    *NFI_CNFG = BOOTROM_SRD_CNFG;
    //*NFI_CNFG = 0x2042;

    // always use 8bits I/O interface to read device id 
    nfi_pagefmt = *NFI_PAGEFMT;
    *NFI_PAGEFMT = nfi_pagefmt&(~PAGEFMT_16BITS)|PAGEFMT_8BITS;

    // read id cmd 
    *NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read_id.m_cmd;
    // wait til CMD is completely issued 
    timeout = c_timeout;
    NFI_Wait_Command(timeout);
    if(!timeout){ return S_DA_TIMEOUT; }

    // issue addr 
    *NFI_ROWADDR = 0;
    *NFI_COLADDR = 0;
    *NFI_ADDRNOB = 1;

    // wait til ADDR is completely issued 
    timeout = c_timeout;
    NFI_Wait_Adddress(timeout);
    if(!timeout){ return S_DA_TIMEOUT; }

    // set single read by DWORD 
    *NFI_CON = (4 << CON_NFI_NOB_SHIFT)|CON_NFI_SRD;
    // wait til DATA_READ is completely issued 
    //timeout = 0xC;
    timeout = 0xFF;
    WAIT_NFI_PIO_READY(timeout);
    if(timeout){
        // single read doesn't need to polling FIFO 
        id1 = *NFI_DATAR;
    }
    
    *p_maker_code = (id1&0xFF);
    *p_device_code = ((id1>>8)&0xFF);
    *p_ext_code1 = ((id1>>16)&0xFF);
    *p_ext_code2 = ((id1>>24)&0xFF);
#endif
    ret = S_DONE;

    // restore original page format setting 
    *NFI_PAGEFMT = nfi_pagefmt;

    return ret;
}

#if 0
//------------------------------------------------------------------------------
// Read Device ID Callback Function                                             
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_ReadID_V3(
				const uint32  c_timeout
				,uint16	*p_maker_code
				,uint16	*p_device_code
				,uint16 *p_ext_code1
				,uint16 *p_ext_code2
) {
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32	    id1 = 0;
	uint16	    nfi_pagefmt;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;

	// reset NFI
	NFI_Reset();
	
	// set NFI_CNFG
	//*NFI_CNFG = BOOTROM_SRD_CNFG;
	*NFI_CNFG = 0x2002;

	// always use 8bits I/O interface to read device id 
	nfi_pagefmt = *NFI_PAGEFMT;
	*NFI_PAGEFMT = nfi_pagefmt&(~PAGEFMT_16BITS)|PAGEFMT_8BITS;

	// read id cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read_id.m_cmd;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }

	// issue addr 
	*NFI_ROWADDR = 0;
	*NFI_COLADDR = 0;
	*NFI_ADDRNOB = 1;

	// wait til ADDR is completely issued 
	timeout = c_timeout;
	NFI_Wait_Adddress(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }

	// set single read by DWORD 
	*NFI_CON = (4 << CON_NFI_NOB_SHIFT)|CON_NFI_SRD;
	// wait til DATA_READ is completely issued 
	//timeout = 0xC;
	timeout = 0xFF;
	WAIT_NFI_PIO_READY(timeout);
	if(timeout){
		// single read doesn't need to polling FIFO 
		id1 = *NFI_DATAR;
	}
	
	*p_maker_code = (id1&0xFF);
	*p_device_code = ((id1>>8)&0xFF);
	*p_ext_code1 = ((id1>>16)&0xFF);
	*p_ext_code2 = ((id1>>24)&0xFF);

	ret = S_DONE;

	// restore original page format setting 
	*NFI_PAGEFMT = nfi_pagefmt;

	return ret;
}
#endif

//------------------------------------------------------------------------------
// Reset Device Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_Reset_V5(
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
STATUS_E  NAND_COMMON_ReadStatus_V5(
				const uint32  c_timeout
) {
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32		status = 0xFF;

	// reset NFI
	NFI_Reset();
	if(use_randomizer)
		NAND_COMMON_Turn_Off_Randomizer();
	
	// set NFI_CNFG
	*NFI_CNFG = BOOTROM_SRD_CNFG;

	// read status cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_status.m_cmd;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){
//		if(use_randomizer)
//			NAND_COMMON_Randomizer_Config(TRUE);
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

	// check READY/BUSY status first 
	if( !(STATUS_READY&status) ) {
//		if(use_randomizer)
//			NAND_COMMON_Randomizer_Config(TRUE);
		return S_DA_IN_PROGRESS;
	}
//	if(use_randomizer)
//		NAND_COMMON_Randomizer_Config(TRUE);

	// flash is ready now, check status code 
	if( STATUS_FAIL & status ) {
		if( !(STATUS_WR_ALLOW&status) ) {
			return S_DA_NAND_BLOCK_IS_LOCKED;
		}
		else {
			return S_DA_NAND_UNKNOWN_ERR;
		}
	}
	else {
		return S_DONE;
	}
}

//------------------------------------------------------------------------------
// Block Erase Related Callback Function                                        
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_BlockErase_V5(
				const uint32  row_addr
) {
	uint32	page_size;
	uint32	spare_size;
	uint32	addr_cycle;
	uint32	row_addr_nob, col_addr_nob;
	uint32  c_timeout = NFI_PSTA_TIMEOUT;
	uint32	timeout = NFI_PSTA_TIMEOUT;

#ifdef DUMP_SPEED
    uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif
	
	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	addr_cycle = NUTL_ADDR_CYCLE();

	// reset NFI
	NFI_Reset();

	// set NFI_CNFG
	*NFI_CNFG = BOOTROM_ERASE_CNFG;

	// block erase cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_erase.m_cmd;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }	

	// fill 1~4 cycle addr, erase command only fill row address, so column bits shift is unnecessary 
	*NFI_ROWADDR = row_addr;
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
    
#ifdef DUMP_SPEED
    g_ds_nand_block_erase_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_block_erase_time += (tick_end - tick_start);
#endif

	return S_DONE;
}

//------------------------------------------------------------------------------
// Bad Block Symbol Identification Related Callback Function                    
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_BadBlockSymbol_Check_V5(
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

STATUS_E  NAND_COMMON_BadBlockSymbol_Set_V5(
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
// Page Read Callback Function                                                  
//------------------------------------------------------------------------------
#if 1
STATUS_E  NAND_COMMON_PageRead_V5(
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
#ifdef DUMP_SPEED
    uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif
	do{
		page_size = NUTL_PAGE_SIZE();
		spare_size = NUTL_SPARE_SIZE();
		column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
		addr_cycle = NUTL_ADDR_CYCLE();
		sectors = NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;
		//DBG_MSG("NAND_COMMON_PageRead_V5 0x%x\n", row_addr);
		// 1. Init NFI
		// 1.1 reset NFI
#if defined(BB_MT6571)
		memset(&dbg_MLC, 0, sizeof(struct debug_info_MLC));
		//memset(dbg_SLC, 0, sizeof(debug_info_SLC));
		//memset(debug_spare, 0, sizeof(u8)*1024);
		memcpy(&dbg_MLC.magic, magic ,7);
#endif
		//DBG_MSG("row_addr 0x%x\n",row_addr);
		NFI_Reset();

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
		*ECC_DECCNFG |= (ECC_CNFG_NFI | DEC_CNFG_CORRECT); // | (u4DECCODESize << DEC_CNFG_CODE_SHIFT));
		STOP_ECC_DEC
		START_ECC_DEC

		// 1.4 set CNFG
		//*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
		*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
		*NFI_CNFG |= ((bUsingDMA)?CNFG_AHB:0);				// AHB mode or NFI mode
#if 0
		if(row_addr >= 2048)
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
#endif	
	if(use_randomizer)
			NAND_COMMON_Randomizer_Config(TRUE,row_addr);
	// Ray's todo:		
	//	*NFI_CNFG |= CNFG_AUTO_FMT_EN;	// if read for verify, use auto format; if read back raw data, don't use it!
		
		// ?? *CNFG |= CNFG_BYTE_RW; ??

		// 2. Issue Read CMD
		// 2.1 read cmd 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd;
		// 2.2 wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout){ return S_DA_TIMEOUT; }		

		// 3. Issue Addr 
		// 3.1 set  row addr, col addr, and num of byte
		// fill 1~4 cycle addr 
		*NFI_ROWADDR = row_addr;
		*NFI_COLADDR = 0;

		// no. of addr cycle 
		col_addr_nob =  (page_size==512)?1:2;	// 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
		row_addr_nob = addr_cycle - col_addr_nob;
		*NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT)) ;
		
		// 3.2 wait til ADDR is completely issued 
		timeout = c_timeout;
		NFI_Wait_Adddress(timeout);
		if(!timeout){ return S_DA_TIMEOUT; }

		// 4. Issue Read Confirm CMD
		if(g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_enable) {
			// 4.1 read confirm 
			*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd;
			// 4.2 wait til CMD is completely issued 
			timeout = c_timeout;
			NFI_Wait_Command(timeout);
			if(!timeout){ return S_DA_TIMEOUT; }			
		}
		
		// 5. Wait for Read Buffer is ready
		// wait til device is from busy to ready
		timeout = c_timeout;
		NFI_Wait_Return_Ready(timeout);
		if(!timeout){ return S_DA_TIMEOUT; }	
		
		// 6. set NFI_CON
		// 6.1 set burst read by DWORD 
		*NFI_CON = (CON_NFI_BRD | (sectors << CON_NFI_SEC_SHIFT));

		// 6.2 wait til DATA_READ is completely issued 
		//wait til Device is ready
		timeout=c_timeout;
		NFI_Wait_Ready(timeout);

		// 7. Read data
		// read page data 
		if( S_DONE != (ret=NUTL_FIFO_Read_V5(c_timeout, bUsingDMA, p_data32, page_size)) ) {
			//goto end;
		}
		if(S_DA_NAND_UNKNOWN_ERR == ret)
		{
			if(NUTL_DEVICE_MLC())
			{
				if((NUTL_SET_FEATURE() != 0xFF))
				{
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
                    if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_16NM)
                    {
                        HYNIX_16NM_Set_RR_Para(readRetryCount, 0);
                    }
                    else
                    {
					    NAND_COMMON_Set_Feature(NUTL_SET_FEATURE(),NUTL_READ_RETRY_ADDRESS(),&readRetryVal,4);
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

            
            readRetryCount++;
		}
        else
		{
			readRetry = FALSE;
		}
		// <<<<  WARNING!! >>>>                                                               
		// 1. You MUST wait until the NFI FIFO is empty.                                      
		//    It means all data in the FIFO had been read out, and then you can start to read 
		//    ECC parity registers.   
	}while(TRUE == readRetry);

    if((readRetryCount != 0) && (NUTL_DEVICE_MLC()))
    {
	    if(NUTL_READ_RETRY_TYPE() == RTYPE_HYNIX_16NM)
		{
			HYNIX_16NM_Set_RR_Para(0, 1);
			//DBG_MSG("readtry OK at page:%d count:%d\n",row_addr, readRetryCount);
		}
        if(ret == S_DONE)
            DBG_MSG("readtry OK at page:%d count:%d\n",row_addr, readRetryCount);
        else
            DBG_MSG("readtry failed at page:%d count:%d\n",row_addr, readRetryCount);
    }
    
#ifdef DUMP_SPEED
    g_ds_nand_page_read_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_read_time += (tick_end - tick_start);
#endif

end:
#if defined (BB_MT6571)
	if(NUTL_DEVICE_MLC())
	{
#if 1
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
#endif
	NAND_COMMON_Turn_Off_Randomizer();
	// 8. Stop ECC Decode
	return S_DONE;//ret;
}

//------------------------------------------------------------------------------
// Read From NFI FIFO                                                           
//------------------------------------------------------------------------------

STATUS_E  NUTL_FIFO_Read_V5(
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
		while ((data_len >> sector_shift) > ((*NFI_BYTELEN & 0x1F000) >> 12))
			;

		dec_mask = (1 << sectors) - 1;
		while (dec_mask != (*ECC_DECDONE))
			;

		*NFI_INTR_EN = 0;
		 fail_sector = 0;
#if defined(BB_MT6571)
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
			}
		}
#else
		for(sector=0;sector < sectors;sector++)
		{
			if(ERR_NUM0 == ErrNum)
				fail_sector++;
		}
#endif
#if 1
		if(fail_sector == sectors)
		{
			if((*NFI_STA & STA_READ_EMPTY) != 0)
			{
				memset(p_data32, 0xFF,(data_len+NUTL_SPARE_SIZE()));
				//DBG_MSG("EMPTY PAGE!\n");
				fail_sector = 0;
#if defined(BB_MT6571)
				dbg_MLC.sector_ecc_bitmap = 0;
#endif
			}
		}
#endif
		STOP_ECC_DEC
	}
	else{			// NFI mode - dword access mode
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
#if defined(BB_MT6571)
			ErrNum = (DRV_Reg32((ECC_DECENUM0+(sector/4)))>>((sector%4)*8))& ERR_NUM0;
#else
			ErrNum = *ECC_DECENUM0;
			if((sector>>2) != 0)                  //sector 4, 5, 6, 7
				ErrNum = *ECC_DECENUM1;
			ErrNum = (ErrNum >> (ERR_NUM_SHIFT * (sector % 4))) & ERR_NUM0;
#endif
#else
			ErrNum = (*ECC_DECENUM >> (ERR_NUM_SHIFT * sector)) & ERR_NUM0;
#endif
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
//		if((*NFI_STA & STA_READ_EMPTY) != 0)
//		{
//			memset(p_data32, 0xFF,data_len);
//		}


		STOP_ECC_DEC

		
	}
#if defined(BB_MT6571)
	if( fail_sector == 0)
		return S_DONE;
	else
		return S_DA_NAND_UNKNOWN_ERR;//S_DONE;
}
#else
	return S_DONE;
#endif
#if 0
//#elif defined(BB_MT6575)
STATUS_E  NAND_COMMON_PageRead_V3(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {
	uint32		page_size,i;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		row_addr_nob, col_addr_nob;	
	uint32		sectors;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
	bool		bUsingDMA=TRUE;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	sectors = NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;

	// 1. Init NFI
	// 1.1 reset NFI
	NFI_Reset();
	
#ifdef DISABLE_NFI_DMA
	bUsingDMA = FALSE;
#else
	bUsingDMA = TRUE;
#endif

	// 1.2 read destination address
	*NFI_STRADDR = (uint32)p_data32;
	
	// 1.3 enable ECC Decoder
	*ECC_DECCNFG |= (ECC_CNFG_NFI | DEC_CNFG_EMPTY_EN);
	STOP_ECC_DEC
	START_ECC_DEC

	// 1.4 set CNFG
	//*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
	*NFI_CNFG = (BOOTROM_READ_CNFG | CNFG_HW_ECC_EN); 	//Read, HW ECC auto Decode.  Because need read full page, so disabue AUTO_FMT
	*NFI_CNFG |= ((bUsingDMA)?CNFG_AHB:0);				// AHB mode or NFI mode

// Ray's todo:		
//	*NFI_CNFG |= CNFG_AUTO_FMT_EN;	// if read for verify, use auto format; if read back raw data, don't use it!
	
	// ?? *CNFG |= CNFG_BYTE_RW; ??

	// 2. Issue Read CMD
	// 2.1 read cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read.m_cmd;
	// 2.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }		

	// 3. Issue Addr 
	// 3.1 set  row addr, col addr, and num of byte
	// fill 1~4 cycle addr 
	*NFI_ROWADDR = row_addr;
	*NFI_COLADDR = 0;

	// no. of addr cycle 
	col_addr_nob =  (page_size==512)?1:2;	// 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
	row_addr_nob = addr_cycle - col_addr_nob;
	*NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT)) ;
	
	// 3.2 wait til ADDR is completely issued 
	timeout = c_timeout;
	NFI_Wait_Adddress(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }

	// 4. Issue Read Confirm CMD
	if(g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_enable) {
		// 4.1 read confirm 
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_read_cnf.m_cmd;
		// 4.2 wait til CMD is completely issued 
		timeout = c_timeout;
		NFI_Wait_Command(timeout);
		if(!timeout){ return S_DA_TIMEOUT; }			
	}
	
	// 5. Wait for Read Buffer is ready
	// wait til device is from busy to ready
	timeout = c_timeout;
	NFI_Wait_Return_Ready(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }	
	/*add by jiequn,2011.11.9{*/
	#if 1
	i = *NFI_INTR;
      *NFI_INTR = i;
      if(bUsingDMA)
	  	*NFI_INTR_EN |= INTR_AHB_DONE_EN;
	#endif
   	/*add by jiequn,2011.11.9}*/

	// 6. set NFI_CON
	// 6.1 set burst read by DWORD 
	*NFI_CON = (CON_NFI_BRD | (sectors << CON_NFI_SEC_SHIFT));

	// 6.2 wait til DATA_READ is completely issued 
	//wait til Device is ready
	timeout=c_timeout;
	NFI_Wait_Ready(timeout);

	// 7. Read data
	// read page data 
	if( S_DONE != (ret=NUTL_FIFO_Read_V3(c_timeout, bUsingDMA, p_data32, page_size+spare_size)) ) {
		goto end;
	}

	// <<<<  WARNING!! >>>>                                                               
	// 1. You MUST wait until the NFI FIFO is empty.                                      
	//    It means all data in the FIFO had been read out, and then you can start to read 
	//    ECC parity registers.                                                           


end:
	// 8. Stop ECC Decode
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
	uint32	sector_page_size32;
	uint32	sector_spare_size32;
	uint32	sector_pagespare_size32;
	uint32	dec_mask;

	sectors				= NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;
	sector_page_size32 	= (NUTL_PAGE_SIZE()/sectors)>>2;
	sector_spare_size32 = (NUTL_SPARE_SIZE()/sectors)>>2;
	sector_pagespare_size32 = sector_page_size32 + sector_spare_size32;
	
	if(bUsingDMA) {	// AHB mode
		uint32 reg_val;
		uint32 read_bytes;
	/*add by jiequn,2011.11.9{*/
	#if 1
		timeout = c_timeout;
	       while((INTR_AHB_DONE!=(*NFI_INTR&INTR_AHB_DONE))&(--timeout));
		 *NFI_INTR_EN &= ~INTR_AHB_DONE_EN;
		 i = *NFI_INTR;
		 *NFI_INTR = i;
	
		while ((data_len >> 9) > ((*NFI_BYTELEN & 0xF000) >> 12));
		dec_mask = (1 << sectors) - 1;
		while (dec_mask != (*ECC_DECDONE));
	#endif
	/*add by jiequn,2011.11.9}*/
	/*	
		timeout = c_timeout;
		while( (ADDRCNTR_CNTR((*NFI_ADDRCNTR)) < sectors) && (--timeout));

		timeout = 0xffff;
		do
		{
			reg_val = *NFI_ADDRCNTR;
			read_bytes = (ADDRCNTR_CNTR(reg_val)<<9) + (sector_spare_size32*ADDRCNTR_CNTR(reg_val)) + ADDRCNTR_OFFSET(reg_val);
         	timeout--;
		}
		while( (read_bytes < data_len) && (timeout>0) ); */
	}
	else{			// NFI mode - dword access mode
		uint32 sector;
		// read page data 
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
			// 2. ECC correct for each sector
			// 3. Read FDM data
		}
	}

	return S_DONE;
}
#endif
#else
#error check nand implementation!!
#endif

//------------------------------------------------------------------------------
// Page Program Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_PageProgram_V5(
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
	NFI_Reset();

#ifdef DISABLE_NFI_DMA
	bUsingDMA = FALSE;
#else
	bUsingDMA = TRUE;
#endif
	bUsingDMA = TRUE;
	// 1.2 !! MT6516 FPGA: Ray's todo: write FDM
	fdm = (uint32*)p_data32 + (page_size>>2);
	
	for(i=0; i<8; i++)
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
	*NFI_CNFG |= ((bUsingDMA)?CNFG_AHB:0);									// AHB mode or NFI mode
#if 0
	if(row_addr >= 2048)
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
#endif
	if(use_randomizer)
		NAND_COMMON_Randomizer_Config(TRUE,row_addr);
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

	// 3. Issue Program CMD
	// 3.1 program cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program.m_cmd;
	// 3.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }
		
	// 4. Issue Addr 
	// 4.1 set  row addr, col addr, and num of byte
	// fill 1~4 cycle addr 
	*NFI_ROWADDR = row_addr;
	*NFI_COLADDR = 0;

	// no. of addr cycle 
	col_addr_nob =  (page_size==512)?1:2;	// 1B(8 bits) is addressed 512 Bytes, 2B(16 bits) is address 64K
	row_addr_nob = addr_cycle - col_addr_nob;
	*NFI_ADDRNOB = ((row_addr_nob<<ADDR_ROW_NOB_SHIFT) | (col_addr_nob<<ADDR_COL_NOB_SHIFT)) ;

	// 4.2 wait til ADDR is completely issued 
	timeout = c_timeout;
	NFI_Wait_Adddress(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }

	// 5. set NFI_CON
	// set burst program by DWORD 
	*NFI_CON = (CON_NFI_BWR | (sectors << CON_NFI_SEC_SHIFT));

	// 6. Program data
	if( S_DONE != (ret=NUTL_FIFO_Write_V5(bUsingDMA, p_data32, NUTL_PROGRAM_PAGE_ONLY)) ) {
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
	if(!timeout){ return S_DA_TIMEOUT; }

	timeout = c_timeout;
	while( FIFO_WR_REMAIN( *NFI_FIFOSTA) && (--timeout) );
	STOP_ECC_ENC
	
	// 8. Issue Program Confirm CMD
	// 8.1 program confirm 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_cnf.m_cmd;
	// 8.2 wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }

	// 9. Wait for Program Buffer is ready
	timeout = c_timeout;
	if(use_randomizer)
		NAND_COMMON_Turn_Off_Randomizer();
	while( (S_DA_IN_PROGRESS ==(ret = NAND_COMMON_ReadStatus_V5(c_timeout))) && (timeout--));
	if(!timeout){ return S_DA_TIMEOUT; }

	// 10. Stop ECC Encode
	//WAIT_ENC_READY
	STOP_ECC_ENC

	// 11.
	if(S_DONE != ret){ return S_DA_NAND_UNKNOWN_ERR;}

#ifdef DUMP_SPEED
    g_ds_nand_page_program_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_program_time += (tick_end - tick_start);
#endif

	

	return S_DONE;
}

//------------------------------------------------------------------------------
// Page+Spare Program Callback Function                                               
//------------------------------------------------------------------------------
STATUS_E NAND_COMMON_PageSpareProgram_V5(uint32 row_addr,
                                         const uint32 *p_page_spare)
{
    const uint32 page_size = NUTL_PAGE_SIZE();
    const uint32 spare_size = NUTL_SPARE_SIZE();
    const uint32 addr_cycle = NUTL_ADDR_CYCLE();
    const uint32 sectors = NUTL_PAGE_SIZE() / NAND_SECTOR_SIZE;
    uint32 row_addr_nob, col_addr_nob;
    uint32 timeout;
    STATUS_E ret;
	//DBG_MSG("Drv Program.....\n");
    NFI_Reset();
    *NFI_CNFG = BOOTROM_WRITE_CNFG;
#if 0
		if(row_addr >= 2048)
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
#endif
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
            return S_DA_TIMEOUT;
        }
            
        // Reset for ambiguous of read command and 1st-half program command
        NFI_Reset();
    }
if(use_randomizer)
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
        return S_DA_TIMEOUT;
    }

    //
    // Issue address
    //

    // Set row addr, col addr, and num of bytes
    // fill 1~4 cycle addr 
    *NFI_ROWADDR = row_addr;
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
        return S_DA_TIMEOUT;
    }

    // Set NFI_CON
    // set burst program by DWORD 
    *NFI_CON = (CON_NFI_BWR | (sectors << CON_NFI_SEC_SHIFT));

	// Program data
    if (S_DONE != (ret=NUTL_FIFO_Write_V5(FALSE, p_page_spare, NUTL_PROGRAM_PAGE_SPARE)))
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

    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }

    // Wait for Program Buffer is ready
    timeout = NUTL_DEFAULT_TIMEOUT;
if(use_randomizer)
		NAND_COMMON_Turn_Off_Randomizer();
    while ((S_DA_IN_PROGRESS == (ret=NAND_COMMON_ReadStatus_V5(NUTL_DEFAULT_TIMEOUT))) && (timeout--));

    if (!timeout)
    {
        return S_DA_TIMEOUT;
    }

    if (S_DONE != ret)
    {
        return S_DA_NAND_UNKNOWN_ERR;
    }

    return S_DONE;
}

//------------------------------------------------------------------------------
// Write To NFI FIFO                                                            
//------------------------------------------------------------------------------

STATUS_E  NUTL_FIFO_Write_V5(const bool bUsingDMA,
                             const uint32 *p_data32, /* MUST be 32bits alignment addr */
                             NUTL_ProgramFlag_E flag)
{
    uint32 data_size_in_dwords;

    switch (flag)
    {
        case NUTL_PROGRAM_PAGE_ONLY:
            data_size_in_dwords = (NUTL_PAGE_SIZE()) >> 2;
            break;

        case NUTL_PROGRAM_PAGE_SPARE:
            data_size_in_dwords = (NUTL_PAGE_SIZE() + NUTL_SPARE_SIZE()) >> 2;
            break;

        default:
            while (1);
    }

    if (bUsingDMA)
    {
        // AHB mode

        const uint32 sectors = NUTL_PAGE_SIZE() / NAND_SECTOR_SIZE;
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
STATUS_E  NAND_COMMON_SpareRead_V5(
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
STATUS_E  NAND_COMMON_SpareProgram_V5(
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
STATUS_E  NAND_COMMON_CopyBack_V5(
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
#endif

