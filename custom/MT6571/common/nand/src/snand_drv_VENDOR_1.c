/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*******************************************************************************/
#if defined(NAND_SPI)

#include "snand_drv_VENDOR_1.h"

//------------------------------------------------------------------------------
// VENDOR Type 1 Command Set
//------------------------------------------------------------------------------
const NAND_CommandSet_S		g_SNAND_VENDOR_1_2048_CMD_SET={
	 { 1, 0x9F }	// read id
	,{ 0, 0x00 }	// read status
	,{ 1, 0xFF }	// reset device
	,{ 1, 0x13 }	// read                     // for SPI-NAND, it's Read Page To Cache
	,{ 0, 0x00 }	// read spare area
	,{ 1, 0x6B }	// read confirm             // for SPI-NAND, it's (Random) Read From Cache (x4)
	,{ 0, 0x00 }	// program 1st half page
	,{ 1, 0x32 }	// program                  // for SPI-NAND, it's (Random) Program Load (x4)
	,{ 1, 0x10 }	// program confirm          // for SPI-NAND, it's Program Execute
	,{ 1, 0xD8 }	// erase
	,{ 0, 0xD0 }	// erase confirm
	,{ 1, 0x0F }	// get feature              // SPI-NAND specific
	,{ 1, 0x1F }	// set feature              // SPI-NAND specific
	,{ 1, 0x06 }	// write enable             // SPI-NAND specific
};


//------------------------------------------------------------------------------
// SAMSUNG OTP
//------------------------------------------------------------------------------

const NAND_OTP_CMD_Callback_S	g_SNAND_VENDOR_1_OTP_CB_FUNC_SET={
	 NULL
	,NULL
	,NULL
	,NULL
	,NULL
	,NULL

};

const NAND_OTP_Layout_S SNAND_VENDOR_1_OTP = {
	6,

	{
		 { 0x00150000,	5,	0x00010000 }
		,{ 0x001B0000,	1,	0x00010000 }
		,{ 0, 0, 0 }
		,{ 0, 0, 0 }
	}
};



#endif