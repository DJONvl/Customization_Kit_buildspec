/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#include "nand_drv_KINGSTON.h"

//------------------------------------------------------------------------------
// KINGSTON Command Set                                                            
//------------------------------------------------------------------------------
const NAND_CommandSet_S		g_NAND_KINGSTON_512_CMD_SET={
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

const NAND_CommandSet_S		g_NAND_KINGSTON_2048_CMD_SET={
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
// KINGSTON Callback Function Set                                                  
//------------------------------------------------------------------------------
