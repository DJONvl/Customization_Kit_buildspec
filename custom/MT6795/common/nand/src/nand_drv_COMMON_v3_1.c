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

#ifndef __MTK_NFI_V4__

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
	,NAND_COMMON_PageProgram_V3
	,NAND_COMMON_SpareRead_V3
	,NAND_COMMON_SpareProgram_V3
	,NAND_COMMON_PageSpareProgram_V3
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
    
//------------------------------------------------------------------------------
// Read Device ID Callback Function                                             
//------------------------------------------------------------------------------
STATUS_E  NAND_COMMON_ReadID_V3(
                const uint32  c_timeout
                ,uint16 *p_maker_code
                ,uint16 *p_device_code
                ,uint16 *p_ext_code1
                ,uint16 *p_ext_code2
				,uint16 *p_ext_code3 //add by jiequn,2011.11.11
) {
    uint32      timeout = NFI_PSTA_TIMEOUT;
    uint32      id1 = 0;
    uint16      nfi_pagefmt = 0;
    uint16      reg_val;
    STATUS_E    ret=S_DA_NAND_UNKNOWN_ERR;

    DRV_WriteReg(NFI_CSEL, 0);
/*modify by jiequnchen,2011.11.16*/
#if defined(MT6572) || defined(MT6571) 
  // Modify by Kuohong, use H8CS0SI0MP for Golden timing.
	DRV_WriteReg32(NFI_ACCCON, 0x21008144);
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

    timeout = 0xFFFF;

    while(timeout)
        timeout--; 
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
    id1 = DRV_Reg32(NFI_DATAR);
	*p_ext_code3 = DRV_Reg8(NFI_DATAR); //add by jiequn,2011.11.11
    DRV_WriteReg(NFI_CON, 0x0);

    *p_maker_code = (id1&0xFF);
    *p_device_code = ((id1>>8)&0xFF);
    *p_ext_code1 = ((id1>>16)&0xFF);
    *p_ext_code2 = ((id1>>24)&0xFF);

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

	// reset NFI
	NFI_Reset();
	
	// set NFI_CNFG
	*NFI_CNFG = BOOTROM_SRD_CNFG;

	// read status cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_status.m_cmd;
	// wait til CMD is completely issued 
	timeout = c_timeout;
	NFI_Wait_Command(timeout);
	if(!timeout){ return S_DA_TIMEOUT; }


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
		return S_DA_IN_PROGRESS;
	}

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
STATUS_E  NAND_COMMON_BlockErase_V3(
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
// Page Read Callback Function                                                  
//------------------------------------------------------------------------------
#if 1
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
    uint32      intr;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	uint32      u4DECCODESize;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
	bool		bUsingDMA=TRUE;
#ifdef DUMP_SPEED
    uint32 tick_start = gpt4_get_current_tick(), tick_end;
#endif

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


	bUsingDMA = TRUE;
	*NFI_INTR_EN = INTR_AHB_DONE_EN;

	// 1.2 read destination address
	intr = *NFI_INTR;
	*NFI_STRADDR = (uint32)p_data32;
	
	// 1.3 enable ECC Decoder
	// u4DECCODESize = (NAND_SECTOR_SIZE + 8) << 3 + 4 * 13;
	*ECC_DECCNFG |= (ECC_CNFG_NFI | DEC_CNFG_EMPTY_EN | DEC_CNFG_CORRECT); // | (u4DECCODESize << DEC_CNFG_CODE_SHIFT));
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
	
#ifdef DUMP_SPEED
    g_ds_nand_page_read_number++;
    tick_end = gpt4_get_current_tick();
    g_ds_nand_page_read_time += (tick_end - tick_start);
#endif

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
	uint32	ErrNum;
	uint32	ErrBitLoc;

	sectors				= NUTL_PAGE_SIZE()/NAND_SECTOR_SIZE;
	sector_page_size32 	= (NUTL_PAGE_SIZE()/sectors)>>2;
	sector_spare_size32 = (NUTL_SPARE_SIZE()/sectors)>>2;
	sector_pagespare_size32 = sector_page_size32 + sector_spare_size32;
	
	if(bUsingDMA) {	// AHB mode
		uint32 reg_val;
		uint32 read_bytes;
		
		timeout = c_timeout;
		// while( (ADDRCNTR_CNTR((*NFI_ADDRCNTR)) < sectors) && (--timeout));

		timeout = 0xffff;
		// while( (read_bytes < data_len) && (timeout>0) ); 

        while(*NFI_INTR == 0)
			;
		while ((data_len >> 9) > ((*NFI_BYTELEN & 0xF000) >> 12))
			;

		dec_mask = (1 << sectors) - 1;
		while (dec_mask != (*ECC_DECDONE))
			;

		*NFI_INTR_EN = 0;
		STOP_ECC_DEC
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
			// decoder done
			dec_mask = 1 << sector;		  
			while (!(dec_mask & (*ECC_DECDONE)))
				;
			ErrNum = *ECC_DECENUM0;
			if((sector>>2) != 0)                  //sector 4, 5, 6, 7
				ErrNum = *ECC_DECENUM1;
			ErrNum = (ErrNum >> (ERR_NUM_SHIFT * (sector % 4))) & ERR_NUM0;
			
			if (ErrNum && ErrNum != ERR_NUM0)
			{
				for (i = 0; i < ErrNum; i++)
				{
					ErrBitLoc = *(ECC_DECEL0 + i / 2);
				}
			}
		}
		STOP_ECC_DEC
	}

	return S_DONE;
}
#else
#error check nand implementation!!
#endif

//------------------------------------------------------------------------------
// Page Program Callback Function                                               
//------------------------------------------------------------------------------
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
	if( S_DONE != (ret=NUTL_FIFO_Write_V3(bUsingDMA, p_data32, NUTL_PROGRAM_PAGE_ONLY)) ) {
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
	while( (S_DA_IN_PROGRESS ==(ret = NAND_COMMON_ReadStatus_V3(c_timeout))) && (timeout--));
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

    NFI_Reset();
    *NFI_CNFG = BOOTROM_WRITE_CNFG;

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
    if (S_DONE != (ret=NUTL_FIFO_Write_V3(FALSE, p_page_spare, NUTL_PROGRAM_PAGE_SPARE)))
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
    while ((S_DA_IN_PROGRESS == (ret=NAND_COMMON_ReadStatus_V3(NUTL_DEFAULT_TIMEOUT))) && (timeout--));

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

STATUS_E  NUTL_FIFO_Write_V3(const bool bUsingDMA,
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

#endif

