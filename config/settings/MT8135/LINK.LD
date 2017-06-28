;*******************************************************************************
;  Copyright Statement:
;  --------------------
;  This software is protected by Copyright and the information contained
;  herein is confidential. The software may not be copied and the information
;  contained herein may not be used or disclosed except with the written
;  permission of MediaTek Inc. (C) 2006
;
;  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
;  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
;  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
;  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
;  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
;  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
;  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
;  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
;  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
;  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
;  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
;  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
;
;  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
;  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
;  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
;  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
;  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
;
;  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
;  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
;  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
;  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
;  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
;
;*******************************************************************************
;*******************************************************************************
;
;  Filename:
;  ---------
;    LINK.LD
;
;  Project:
;  --------
;    DOWNLOAD AGENT
;
;  Description:
;  ------------
;    Scatter file for building Download Agent
;
;  Author:
;  -------
;	 Amos Hsu
;
;*******************************************************************************
;ISRAM is 64KB size and base address is 0x100000 -- 0x110000:
;     0x100000 -- 0x101FFF is for BROM (8KB)
;     0x102000 -- 0x10DFFF is for INT_SYSRAM_BUF (48KB)
;     0x10E000 -- 0x10FFFF is for DA stack,refer to BOOTARM.s (8KB)

INT_SYSRAM_BUF 0x102000 0xC000  
{
	INT_SYSRAM_BUF 0x102000
	{
		Bloader.o			(+ZI)
		rx_buffer.o         (+ZI)
	}
	ZI_MAIN +0
	{
		* 					(+ZI)
	}
}

; 256 bytes are reserved for RSA-2048 encrypted digital signature for SV5.
; If the encrypt key size is larger, please remember to modify this region.
; FLEXL2 size =  0x26000 - 0x2000(M4U is 24KB but only 16KB can been used when power on)- 0x100 = 0x23F00
;INT_SYSRAM 0x12000000 0x23F00 
;0x26000 - 0x1000(reserve for preloader extension) - 0x2000(M4U is 24KB but only 16KB can been used when power on)- 0x100 = 0x22F00
;0x12000000 -- 0x12022EFF INT_SYSRAM (139.75KB)
;0x12022F00 -- 0x12022FFF RSA-2048 encrypted digital signature for SV5 (0.25KB)
;0x12023000 -- 0x12023FFF preloader extension (4KB)
INT_SYSRAM 0x12000000 0x22F00 
{
	; INT_SYSRAM begin addr must sync with BOOTARM_XX.S stack base address
	INT_SYSRAM 0x12000000
	{
		BOOTARM.o	(Init, +First)
		cache_ca7.o         (+RO, +RW)
		BLOADER.o			(+RO, +RW)
		hw_config.o			(+RO, +RW)
		MAIN.o				(+RO, +RW)
		UART.o				(+RO, +RW)
		flash_util_func.o	(+RO, +RW)
		main_nand.o			(+RO, +RW)
		nand_util_func.o	(+RO, +RW)
		get_fw_ver.o		(+RO, +RW)
		test_main.o			(+RO, +RW)
		test_bus.o			(+RO, +RW)
		test_ext_sram.o		(+RO, +RW)
		flash_fdm_initial.o	(+RO, +RW)
		flash_mtd.o			(+RO, +RW)
		flash_otp.o			(+RO, +RW)
		assert.o			(+RO, +RW)
		Drvflash.o			(+RO, +RW)
		nand_rom_writer.o	(+RO, +RW)

		*					(+RO, +RW)
	}
	SYMBOL_INIT +0
	{
		BLOADER.o			(SYMBOLINIT)
	}
	REGIONINIT +0
	{
		regioninit_ads.o	(+RO, +RW)
	}
	CUSTOMIZED_MODULE +0
	{
		; nor/nand driver
		flash_dev_tbl.o		(+RO, +RW)
		flash_drv* 			(+RO, +RW)
		nand_dev_tbl.o		(+RO, +RW)
		nand_drv* 			(+RO, +RW)
		; customized module
		soc_check.o			(+RO, +RW)
		test_custom.o		(+RO, +RW)
		*					(CUSTOMER_CODE)
		*					(CUSTOMER_RODATA)
		*					(CUSTOMER_RWDATA)
	}
	FREE_INT_SYSRAM_BEGIN +0
	{
		hw_config.o			(FREE_INT_SYSRAM_BEGIN)
	}
	INT_SYSRAM_END 0x12022F00
	{
	}
}

EXT_RAM 0x80080000
{
	; start from 0x10000020 to skip EXT_RAM auto-detection pattern
	EXT_RAM 0x80080000
	{
		hw_config.o			(EXT_RAM_BEGIN)
	}
	FREE_EXT_RAM_BEGIN +0
	{
		hw_config.o			(FREE_EXT_RAM_BEGIN)
	}
}

;=========================================================================
; The following load regions must be the end of all the load regions
;=========================================================================
; 1. DO NOT modify load region order, load region naming and load region address!
;=========================================================================
;DRAM 0x82040000--0x8213FFFF for DA ZI
NOR_FLASH_TABLE 0x82040000 0x100000
{
	NOR_FLASH_TABLE 0x82040000
	{
		flash_dev_tbl.o		(NOR_FLASH_TABLE)
	}
	ZI_CUSTOM +0
	{
		; nor/nand driver
		;flash_dev_tbl.o		(+ZI)
		;flash_drv* 			(+ZI)
		;nand_dev_tbl.o		(+ZI)
		;nand_drv* 			(+ZI)
		; customized module
		;soc_check.o			(+ZI)
		;test_custom.o		(+ZI)
		;*					(CUSTOMER_ZIDATA)
		
		;Heaton for MT8135
		test_main.o			(+ZI)
		;sdmmc.o					(+ZI)
		bmt.o							(+ZI)
		nand_util_func.o	(+ZI)
		security.o			(+ZI)
		sec_dev.o			(+ZI)
		main_nand.o         (+ZI)	
	}
}

NAND_FLASH_TABLE 0x82038000
{
	NAND_FLASH_TABLE 0x82038000
	{
		nand_dev_tbl.o		(NAND_FLASH_TABLE)
		sb_encode.o			(+RO, +RW)
		mac.o				(+RO, +RW)
		md5.o				(+RO, +RW)
		security.o			(+RO, +RW)
		;sdmmc_download_internal.o			(+RO, +RW)
		;sdmmc_handler.o			(+RO, +RW)
	}
}

;Dram 0x88000000 -- 0x88FFFFFF reserved for da heap.
DA_HEAP 0x88000000
{

}
