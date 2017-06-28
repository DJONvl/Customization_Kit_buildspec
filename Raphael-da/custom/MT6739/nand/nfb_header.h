/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001
*
*****************************************************************************/

/*
 *  Header format
 *  - Block 0
 *  |-- Page 0   => NFB (x2) or (x4)
 *  |-- Page 1   => BR_Layout
 *  |-- Page 2   => ttbl
 *  |-- Page 3-n => 0xFF
 *  - Block 1
 *  |-- Page 0-n => Preloader
 *  - Block 2
 *  |-- Page 0-n => Preloader ext     
 */

#ifndef _NFF_HEADER_H_
#define _NFF_HEADER_H_


#define HEADER_ID1					("BOOTLOADER!")
#define HEADER_ID2					("NFIINFO")
#define HEADER_VER					("V006")

#define SECTOR_1K					0x0100

#define ECC_PARITY_BYTES			(144)   //80*14bit with 4 Byte align +4

typedef struct _NFIMLC
{
	uint16		pPageAddr;
	uint16		dqsDelayMux;
	uint32		dqsDelayCtrl;
	uint32		accconSetting;
	uint32		acccon1Setting;
	uint32		delayCtrl;
} NFI_MLC;			// Total 20 bytes

typedef struct _NFITLC
{
	uint8		bTLC_NAND;
	uint8		bSLC_OpMode_En;			// TRUE: slc mode   FALSE: tlc mode
	uint8		bNeedChangeCloumn;		// read page with change column address command
	uint8		bRandomizer_En;			// randomizer init enable
	uint8		bWL_Addressing;
	uint8		fdmSize;

	uint16		tlcSbAddress;			// Need to specific SB of WL in TLC mode?;
	uint16		eccRequire;				// ECC requirement
	uint16		slcPreCmd;				// SLC mode pre command
#if 1 // for verify rainier 
	uint16		slcEndCmd;				// SLC mode pre command
#endif
	uint16		blockAddressShift;		// address transfer
	uint16		planeAddressShift;		// pseudo plane address shift
#if 0 // for verify rainier 
	uint8		rvz[2];
#endif
}NFI_TLC;			// Total 18 bytes

typedef struct _NFIType
{
   uint16 io_interface;
   uint16 page_size;
   uint16 address_cycle;
} NFI_MENU;


typedef struct NFB_HEADER_V6
{
	uint8		id1[12];
	uint8		version[4];
	uint8		id2[8];    

	NFI_MENU	nfi_info;

	uint16		spare_size;
	uint16		page_per_block;
	uint16		total_blocks;
	uint16		page_shift;
	uint16		block_shift;
	// 40 B

	NFI_MLC		mlcSetting;		// 20 B
	NFI_TLC		tlcSetting;		// 18 B
	uint8		padding[2];		// 2 B

	uint8		reserved[32];	// 32 B
	uint8		ecc_parities[ECC_PARITY_BYTES];	// 144 B
}NFB_HEADER_V6;		// Total 256 bytes


#endif /* _NFF_HEADER_H_ */
