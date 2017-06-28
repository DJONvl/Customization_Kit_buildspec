/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#include "nand_drv_SAMSUNG.h"

//------------------------------------------------------------------------------
// SAMSUNG Command Set                                                          
//------------------------------------------------------------------------------
const NAND_CommandSet_S		g_NAND_SAMSUNG_512_CMD_SET={
	 { 1, 0x90 }	// read id 
	,{ 1, 0x70 }	// read status 
	,{ 1, 0xFF }	// reset device 
	,{ 1, 0x00 }	// read 
	,{ 1, 0x50 }	// read spare area 
	,{ 0, 0 }		// read confirm 
	,{ 1, 0x00 }	// program 1st half page 
	,{ 1, 0x80 }	// program 
	,{ 1, 0x10 }	// program confirm 
	,{ 1, 0x60 }	// erase 
	,{ 1, 0xD0 }	// erase confirm 
};

const NAND_CommandSet_S		g_NAND_SAMSUNG_2048_CMD_SET={
	 { 1, 0x90 }	// read id 
	,{ 1, 0x70 }	// read status 
	,{ 1, 0xFF }	// reset device 
	,{ 1, 0x00 }	// read 
	,{ 0, 0 }		// read spare area 
	,{ 1, 0x30 }	// read confirm 
	,{ 0, 0 }		// program 1st half page 
	,{ 1, 0x80 }	// program 
	,{ 1, 0x10 }	// program confirm 
	,{ 1, 0x60 }	// erase 
	,{ 1, 0xD0 }	// erase confirm 
};


//------------------------------------------------------------------------------
// SAMSUNG OTP
//------------------------------------------------------------------------------

const NAND_OTP_CMD_Callback_S	g_NAND_SAMSUNG_OTP_CB_FUNC_SET={
	NAND_SAMSUNG_OTP_Enter
	,NAND_SAMSUNG_OTP_PageRead
	,NAND_SAMSUNG_OTP_PageProgram
	,NAND_SAMSUNG_OTP_Lock
	,NAND_SAMSUNG_OTP_Lock_CheckStatus
	,NAND_SAMSUNG_OTP_Exit
	
};

const NAND_OTP_Layout_S SAMSUNG_OTP = {
	6,

	{
		 { 0x00150000,	5,	0x00010000 }
		,{ 0x001B0000,	1,	0x00010000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};

//------------------------------------------------------------------------------
// OTP Callback Function                                                   
//------------------------------------------------------------------------------

STATUS_E  NAND_SAMSUNG_OTP_Enter(
				const uint32  c_timeout
)
{
	return S_DONE;
}

STATUS_E  NAND_SAMSUNG_OTP_PageRead(
				const uint32  c_timeout
				,const uint32  row_addr
				,uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {

	return S_DONE;

// SAMPLE CODE

/*
	uint32		page_size;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
	bool		bUsingDMA=TRUE;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();

#ifdef __MTK_NFI_V2__
		*NFI_CON = MAIN_ECC_EN|SW_PROGSPARE_EN|AUTOECC_DEC_EN;	//for check ECC from NFI_ERRDET
		//*NFI_CON = MAIN_ECC_EN;
		*NFI_OPCON = OPCON_RESET;
		while ((*NFI_PSTA !=0) && timeout){timeout--;}
		if(timeout == 0){
			return S_DA_TIMEOUT;
		}
#else		
		// reset the NFI core state machine, data FIFO and flushing FIFO 		
		*NFI_OPCON = 0x0;
		*NFI_CON = 0x0;
		*NFI_FIFOCON = 0x30;
#endif

#ifdef DISABLE_NFI_DMA
	bUsingDMA = FALSE;
#else
	#if	defined(BB_TK6516_MD)
	bUsingDMA = FALSE;
	#else
	bUsingDMA = TRUE;
	// activating DMA transfer 
	*NFI_CON |= DMA_RD_EN;
	#endif
#endif

	//!! OTP read cmd (NEED CUSTOMIZE!)
	// 1. 0x30
	*NFI_CMD = 0x30;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );

	// 2. 0x65
	*NFI_CMD = 0x65;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );

	// 3. 0x00
	*NFI_CMD = 0x00;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );


	// fill 1~4 cycle addr 
	*NFI_ADDRL = (row_addr<<column_addr_bits);
	*NFI_ADDRM = (row_addr>>(32-column_addr_bits));
	
	// no. of addr cycle 
	*NFI_ADDNOB = 5;
	// wait til ADDR is completely issued 
	while( *NFI_PSTA & STATUS_ADDR );


	//!! OTP read confirm (NEED CUSTOMIZE!)
	if(page_size > 512) {
		*NFI_CMD = 0x30;
	}
	// set burst read by DWORD 
	*NFI_OPCON = BURST_RD | NOB_DWORD;
	// wait til DATA_READ is completely issued 
	//wait til Device is ready
	timeout=c_timeout;
	NFI_Wait_Ready(timeout);

#ifdef __MTK_NFI_V2__
	// read page data 
	if( S_DONE != (ret=NUTL_FIFO_Read(c_timeout, bUsingDMA, p_data32, page_size+spare_size)) ) {
		goto end;
	}
#else
	// read page data 
	if( S_DONE != (ret=NUTL_FIFO_Read(c_timeout, bUsingDMA, p_data32, page_size)) ) {
		goto end;
	}
#endif	

	// <<<<  WARNING!! >>>>                                                               
	// 1. You MUST wait until the NFI FIFO is empty.                                      
	//    It means all data in the FIFO had been read out, and then you can start to read 
	//    ECC parity registers.                                                           

#ifndef __MTK_NFI_V2__
		while(!(*NFI_FIFOCON & RD_EMPTY_MASK));
#endif

end:
	// disable burst read 
	*NFI_OPCON = 0x0;
	
	return ret;
*/
	
}

STATUS_E  NAND_SAMSUNG_OTP_PageProgram(
				const uint32  c_timeout
				,const uint32  row_addr
				,const uint32 *p_data32 /* MUST be 32bits alignment addr */
				,uint32  ecc_parity_from_reg[8]
) {

	return S_DONE;

// Sample Code	
/*
	uint32		page_size;
	uint32		spare_size;
	uint32		column_addr_bits;
	uint32		addr_cycle;
	uint32		io_interface;
	uint32		timeout = NFI_PSTA_TIMEOUT;
	STATUS_E	ret=S_DA_NAND_UNKNOWN_ERR;
	bool		bUsingDMA=TRUE;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	column_addr_bits = NUTL_PAGE_ADDR_SHIFT_BITS();
	addr_cycle = NUTL_ADDR_CYCLE();
	io_interface = NUTL_IO_INTERFACE();
	
#ifdef __MTK_NFI_V2__
		*NFI_CON = MAIN_ECC_EN|SW_PROGSPARE_EN;
		*NFI_OPCON = OPCON_RESET;
		while ((*NFI_PSTA !=0) && timeout){timeout--;}
		if(timeout == 0){
			return S_DA_TIMEOUT;
		}

#else		
		// reset the NFI core state machine, data FIFO and flushing FIFO 		
		*NFI_OPCON = 0x0;
		*NFI_CON = 0x0;
		*NFI_FIFOCON = 0x30;
#endif

#ifdef DISABLE_NFI_DMA
	bUsingDMA = FALSE;
#else
	#if	defined(BB_TK6516_MD)
	bUsingDMA = FALSE;
	#else
	bUsingDMA = TRUE;
	// activating DMA transfer 
	*NFI_CON |= DMA_WR_EN;
	#endif
#endif

	// in most 512 page size NAND flash, you have to setup destination pointer to 1st half area 
	if(g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_enable) {
		*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_program_1st_half_page.m_cmd;
		// wait til CMD is completely issued 
		while( *NFI_PSTA & STATUS_CMD );
#ifdef __MTK_NFI_V2__
		*NFI_OPCON = OPCON_RESET;
#endif		
	}

	//!! OTP program cmd (NEED CUSTOMIZE!)
	// 1. 0x30
	*NFI_CMD = 0x30;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );

	// 2. 0x65
	*NFI_CMD = 0x65;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );

	// 3. 0x80
	*NFI_CMD = 0x80;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );

	// fill 1~4 cycle addr 
	*NFI_ADDRL = (row_addr<<column_addr_bits);
	*NFI_ADDRM = (row_addr>>(32-column_addr_bits));
	
	// no. of addr cycle 
	*NFI_ADDNOB = 5;
	// wait til ADDR is completely issued 
	while( *NFI_PSTA & STATUS_ADDR );

	// set burst program by DWORD 
	*NFI_OPCON = BURST_WR | NOB_DWORD;
	// wait til DATA_WRITE is completely issued 
#ifdef __MTK_NFI_V2__
	// program page data 
	if( S_DONE != (ret=NUTL_FIFO_Write(c_timeout, bUsingDMA, p_data32, page_size+spare_size)) ) {
		return ret;
	}
#else
	// program page data 
	if( S_DONE != (ret=NUTL_FIFO_Write(c_timeout, bUsingDMA, p_data32, page_size)) ) {
		return ret;
	}
#endif	
 
	// <<<<  WARNING!! >>>>                                                              
	// 1. You MUST read parity registers before issue program confirm (0x10) command.    
	//    Since the parity registers will be clean by NFI after issue program confirm.   
	// 2. You MUST wait until the NFI FIFO is empty!                                     
	//    It means all data in the FIFO had been written to NAND flash, and then you can 
	//    start to read ECC parity registers.                                            
	
	timeout = c_timeout;
#ifdef __MTK_NFI_V2__
		NFI_Wait( (*NFI_ADDRCNTR < page_size),timeout);
		if( 0 == timeout ) {
			return S_DA_TIMEOUT;
		}
	
#else		
		//while(!(*NFI_FIFOCON & WR_EMPTY_MASK));
		NFI_Wait( !(*NFI_FIFOCON & WR_EMPTY_MASK), timeout);
		if( 0 == timeout ) {
			return S_DA_TIMEOUT;
		}
	
#endif

	//!! OTP program confirm (NEED CUSTOMIZE!)
	*NFI_CMD = 0x10;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );
	
	//wait til Device is ready
	NFI_Wait_Ready(timeout);

	return S_DONE;
*/	
}

STATUS_E  NAND_SAMSUNG_OTP_Lock(
				const uint32  c_timeout
)
{

	return S_DONE;

// SAMPLE CODE

/*	
	uint32	page_size;
	uint32	spare_size;
	uint32	addr_cycle;
	uint32	timeout = NFI_PSTA_TIMEOUT;

	page_size = NUTL_PAGE_SIZE();
	spare_size = NUTL_SPARE_SIZE();
	addr_cycle = NUTL_ADDR_CYCLE();

#ifdef __MTK_NFI_V2__
		*NFI_CON = 0x0;
		*NFI_OPCON = OPCON_RESET;
//		while (*NFI_PSTA !=0);
		while ((*NFI_PSTA !=0) && timeout){timeout--;}
		if(timeout == 0){
			return S_DA_TIMEOUT;
		}

#else		
		// reset the NFI core state machine, data FIFO and flushing FIFO 		
		*NFI_OPCON = 0x0;
		*NFI_CON = 0x0;
		*NFI_FIFOCON = 0x30;
#endif

	//!! OTP lock cmd (NEED CUSTOMIZE!)
	*NFI_CMD = SAMSUNG_OTP_LOCK_CMD;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );

	// fill 1~4 cycle addr, erase command only fill row address, so column bits shift is unnecessary 
	*NFI_ADDRL = row_addr;	(NEED CUSTOMIZE!)
	*NFI_ADDRM = 0;			(NEED CUSTOMIZE!)
	// no. of addr cycle 
	*NFI_ADDNOB = 5;

	// wait til ADDR is completely issued 
	while( *NFI_PSTA & STATUS_ADDR );

	//!! OTP lock confirm (NEED CUSTOMIZE!)
	*NFI_CMD = SAMSUNG_OTP_LOCK_CONFIRM_CMD;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );
	
	//wait til Device is ready
	NFI_Wait_Ready(timeout);	

	return S_DONE;
*/	
}

STATUS_E  NAND_SAMSUNG_OTP_Lock_CheckStatus(
				const uint32  c_timeout
)
{
	return S_DA_NAND_OTP_UNLOCKED;
	
// SAMPLE CODE
/*
	uint32	status;
	
#ifdef __MTK_NFI_V2__
	uint32  timeout = NFI_PSTA_TIMEOUT;
		*NFI_CON = 0x0;
		*NFI_OPCON = OPCON_RESET;
//		while (*NFI_PSTA !=0);
		while ((*NFI_PSTA !=0) && timeout){timeout--;}
		if(timeout == 0){
			return S_DA_TIMEOUT;
		}

#else		
		// reset the NFI core state machine, data FIFO and flushing FIFO 		
		*NFI_OPCON = 0x0;
		*NFI_CON = 0x0;
		*NFI_FIFOCON = 0x30;
#endif

	// read status cmd 
	*NFI_CMD = g_pNandInfo->m_dev->m_cmd_set->m_status.m_cmd;
	// wait til CMD is completely issued 
	while( *NFI_PSTA & STATUS_CMD );

	// set single read by DWORD 
	*NFI_OPCON = SINGLE_RD | NOB_DWORD;
	// wait til DATA_READ is completely issued 
	while( *NFI_PSTA & STATUS_DATAR );

	// single read doesn't need to polling FIFO 
	status = *NFI_DATAR;

	// check Lock status	(NEED CUSTOMIZED!)
	if( SAMSUNG_OTP_LOCK & status) ) 
	{
		return S_DA_NAND_OTP_LOCKED;
	}
	else
	{
		return S_DA_NAND_OTP_UNLOCKED;
	}

*/	
}

STATUS_E  NAND_SAMSUNG_OTP_Exit(
				const uint32  c_timeout
)
{
	return S_DONE;
}
