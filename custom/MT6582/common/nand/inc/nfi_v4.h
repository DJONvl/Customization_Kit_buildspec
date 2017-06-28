/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/******************************************************************************
* [File]			NFI.h
* [Version]			v1.0
* [Revision Date]	2007-08-02
* [Author]			Meng-Chang Liu, MC_Liu@mtk.com.tw, 26615, 2007-08-02
* [Description]
*	MT8520 Download Agent NFI include file
******************************************************************************
* [Version]		v2.0		[For Nand_60bits]
* [Revision Date]	2011-08-30
* [Author]		Yang Lv, yang.lv@mediatek.com, 82723, 2011-08-30
* [Description]
*	MT8320 Download Agent NFI include file
* [Copyright]
*    Copyright (C) 2007 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/
#ifndef _NFI_H
#define _NFI_H

#include "SW_TYPES.H"
#include "hw_config.h"


#define NFI60_BASE                  0x11001000  // add by liuquan
#define NFI60ECC_BASE               0x11002000  // add by liuquan

/* Register definition */
#define NFI_CNFG		((volatile uint16 *)(NFI60_BASE+0x0000))
#define NFI_PAGEFMT		((volatile uint32 *)(NFI60_BASE+0x0004))
#define NFI_CON			((volatile uint32 *)(NFI60_BASE+0x0008))
#define NFI_ACCCON      ((volatile uint32 *)(NFI60_BASE+0x000C))
#define NFI_INTR_EN     ((volatile uint16 *)(NFI60_BASE+0x0010))
#define NFI_INTR        ((volatile uint16 *)(NFI60_BASE+0x0014))
#define NFI_CMD         ((volatile uint16 *)(NFI60_BASE+0x0020))
#define NFI_ADDRNOB     ((volatile uint16 *)(NFI60_BASE+0x0030))
#define NFI_COLADDR     ((volatile uint32 *)(NFI60_BASE+0x0034))
#define NFI_ROWADDR     ((volatile uint32 *)(NFI60_BASE+0x0038))
#define NFI_STRDATA   	((volatile uint16 *)(NFI60_BASE+0x0040))
#define NFI_CNRNB   	((volatile uint16 *)(NFI60_BASE+0x0044))
#define NFI_DATAW       ((volatile uint32 *)(NFI60_BASE+0x0050))
#define NFI_DATAR       ((volatile uint32 *)(NFI60_BASE+0x0054))
#define NFI_STA         ((volatile uint32 *)(NFI60_BASE+0x0060))
#define NFI_FIFOSTA     ((volatile uint32 *)(NFI60_BASE+0x0064))
#define NFI_LOCKSTA     ((volatile uint16 *)(NFI60_BASE+0x0068))
#define NFI_ADDRCNTR   	((volatile uint16 *)(NFI60_BASE+0x0070))
#define NFI_STRADDR    	((volatile uint32 *)(NFI60_BASE+0x0080))
#define NFI_BYTELEN     ((volatile uint16 *)(NFI60_BASE+0x0084))
#define NFI_CSEL	    ((volatile uint16 *)(NFI60_BASE+0x0090))
//#define NFI_IOCON		((volatile uint16 *)(NFI60_BASE+0x0094))


//Flash Lock
//#define NFI_LOCK                               ((volatile uint16 *)(NFI60_BASE+0x00100))
#define NFI_LOCKEN      					   ((volatile uint32 *)(NFI60_BASE+0x001E0))
#define NFI_LOCKCON                            ((volatile uint32 *)(NFI60_BASE+0x001E4))
#define NFI_LOCKANOB                           ((volatile uint32 *)(NFI60_BASE+0x001E8))
#define NFI_LOCK00ADD                          ((volatile uint32 *)(NFI60_BASE+0x001F0))
#define NFI_LOCK00FMT                          ((volatile uint32 *)(NFI60_BASE+0x001F4))
#define NFI_LOCK01ADD                          ((volatile uint32 *)(NFI60_BASE+0x001F8))
#define NFI_LOCK01FMT                          ((volatile uint32 *)(NFI60_BASE+0x001FC))

#define NFI_LOCK02ADD                          ((volatile uint32 *)(NFI60_BASE+0x00120))
#define NFI_LOCK02FMT                          ((volatile uint32 *)(NFI60_BASE+0x00124))
#define NFI_LOCK03ADD                          ((volatile uint32 *)(NFI60_BASE+0x00128))
#define NFI_LOCK03FMT                          ((volatile uint32 *)(NFI60_BASE+0x0012C))
#define NFI_LOCK04ADD                          ((volatile uint32 *)(NFI60_BASE+0x00130))
#define NFI_LOCK04FMT                          ((volatile uint32 *)(NFI60_BASE+0x00134))
#define NFI_LOCK05ADD                          ((volatile uint32 *)(NFI60_BASE+0x00138))
#define NFI_LOCK05FMT                          ((volatile uint32 *)(NFI60_BASE+0x0013C))
#define NFI_LOCK06ADD                          ((volatile uint32 *)(NFI60_BASE+0x00140))
#define NFI_LOCK06FMT                          ((volatile uint32 *)(NFI60_BASE+0x00144))
#define NFI_LOCK07ADD                          ((volatile uint32 *)(NFI60_BASE+0x00148))
#define NFI_LOCK07FMT                          ((volatile uint32 *)(NFI60_BASE+0x0014C))
#define NFI_LOCK08ADD                          ((volatile uint32 *)(NFI60_BASE+0x00150))
#define NFI_LOCK08FMT                          ((volatile uint32 *)(NFI60_BASE+0x00154))
#define NFI_LOCK09ADD                          ((volatile uint32 *)(NFI60_BASE+0x00158))
#define NFI_LOCK09FMT                          ((volatile uint32 *)(NFI60_BASE+0x0015C))
#define NFI_LOCK10ADD                          ((volatile uint32 *)(NFI60_BASE+0x00160))
#define NFI_LOCK10FMT                          ((volatile uint32 *)(NFI60_BASE+0x00164))
#define NFI_LOCK11ADD                          ((volatile uint32 *)(NFI60_BASE+0x00168))
#define NFI_LOCK11FMT                          ((volatile uint32 *)(NFI60_BASE+0x0016C))
#define NFI_LOCK12ADD                          ((volatile uint32 *)(NFI60_BASE+0x00170))
#define NFI_LOCK12FMT                          ((volatile uint32 *)(NFI60_BASE+0x00174))
#define NFI_LOCK13ADD                          ((volatile uint32 *)(NFI60_BASE+0x00178))
#define NFI_LOCK13FMT                          ((volatile uint32 *)(NFI60_BASE+0x0017C))
#define NFI_LOCK14ADD                          ((volatile uint32 *)(NFI60_BASE+0x00180))
#define NFI_LOCK14FMT                          ((volatile uint32 *)(NFI60_BASE+0x00184))
#define NFI_LOCK15ADD                          ((volatile uint32 *)(NFI60_BASE+0x00188))
#define NFI_LOCK15FMT                          ((volatile uint32 *)(NFI60_BASE+0x0018C))


#define NFI_FIFODATA0   ((volatile uint32 *)(NFI60_BASE+0x0190))
#define NFI_FIFODATA1   ((volatile uint32 *)(NFI60_BASE+0x0194))
#define NFI_FIFODATA2   ((volatile uint32 *)(NFI60_BASE+0x0198))
#define NFI_FIFODATA3   ((volatile uint32 *)(NFI60_BASE+0x019C))

#define NFI_FDM0L       ((volatile uint32 *)(NFI60_BASE+0xA0))
#define NFI_FDM0M       ((volatile uint32 *)(NFI60_BASE+0xA4))

#define NFI_FDM8L       ((volatile uint32 *)(NFI60_BASE+0x200))
#define NFI_FDM8M       ((volatile uint32 *)(NFI60_BASE+0x204))

#define NFI_RANDOM_CFG	((volatile uint32 *)(NFI60_BASE+0x1BC))

#define EFUSE_RANDOM_CFG	((volatile uint32 *)(0x10009048))
#define EFUSE_RANDOM_ENABLE 0x00000080
#if 0
// random mizer
#define NAND_NFI_RANDOM_CFG                         0x001BC
    #define NAND_NFI_ENCODING_RANDON_EN                 (((uint32) 1) << 0)
    #define NAND_NFI_ENCODING_RANDON_SEED(x)            ((((uint32) x ) & 0x7FFF) << 1)
    #define NAND_NFI_DECODING_RANDON_EN                 (((uint32) 1) << 16)
    #define NAND_NFI_DECODING_RANDON_SEED(x)            ((((uint32) x ) & 0x7FFF) << 17)

#define NAND_NFI_MISC                               0x001A0
    #define NAND_NFI_MISC_FLASH_PMUX                    (((uint32) 1) << 2)
#endif 

#define NFI_ACCCON2	((volatile uint32 *)(NFI60_BASE+0x1A4))
//#define NFI_ACCCON2                              0x001A4
    #define DATA_HOLD_TIME(x)                   ((((uint32) x ) & 0xF) << 16)   // added by liuquanji
	#define READ_ACCESS_TIME(x)                 ((((uint32) x ) & 0x1F) << 0)   // added by liuquanji

// clock division
//#define NAND_NFI_CLKDIV                             0x001AC
// #define NAND_NFI_CLKDIV_EN                          (((uint32) 1) << 0)
#if 1
// multi-page dma
#define NFI_MDMACON                            ((volatile uint16 *)(NFI60_BASE+0x001B0))
    #define NFI_MDMA_TRIG                          (((uint32) 1) << 0)
    #define NFI_MDMA_EN                            (((uint32) 1) << 4)
    #define NFI_MDMA_MODE(x)                       ((((uint32) x ) & 0x3) << 8)
        #define NFI_MDMA_READ                          (((uint32) 0) << 8)
        #define NFI_MDMA_WRITE                         (((uint32) 1) << 8)
        #define NFI_MDMA_ERASE                         (((uint32) 2) << 8)
    #define NFI_MDMA_LEN(x)                           ((((uint32) x ) & 0x7) << 12)

#define NAND_NFI_MDMA_PAGENUM                       (8)
#define NFI_MDMAADDR                           ((volatile uint16 *)(NFI60_BASE+0x001B4))

    #define NFI_MDMAADDR_DRAMADDR                  (NAND_NFI_MDMA_PAGENUM * 0)
    #define NFI_MDMAADDR_ROWADDR                   (NAND_NFI_MDMA_PAGENUM * 1)
    #define NFI_MDMAADDR_FDM                       (NAND_NFI_MDMA_PAGENUM * 2)
    #define NFI_MDMAADDR_DECDONE                   (NAND_NFI_MDMA_PAGENUM * 66)
    #define NFI_MDMAADDR_DECFER                    (NAND_NFI_MDMA_PAGENUM * 67)
    #define NFI_MDMAADDR_DECENUM0                  (NAND_NFI_MDMA_PAGENUM * 68)
    #define NFI_MDMAADDR_DECENUM1                  (NAND_NFI_MDMA_PAGENUM * 69)	
    #define NFI_MDMAADDR_DECENUM2                  (NAND_NFI_MDMA_PAGENUM * 70)
    #define NFI_MDMAADDR_DECENUM3                  (NAND_NFI_MDMA_PAGENUM * 71)	
    #define NFI_MDMAADDR_DECEL0					   (NAND_NFI_MDMA_PAGENUM * 72)
	
#define NFI_MDMADATA                           ((volatile uint32 *)(NFI60_BASE+0x001B8))
#endif


/*******************************************************************************
 * NFI register definition
 *******************************************************************************/
/* NFI_CNFG */
#define AHB_MODE		0x0001
#define READ_MODE		0x0002
#define DBYTE_EN_16BITS	0x0010
#define SEL_SEC_512BYTE 0x0020
#define BYTE_RW		    0x0040
#define HW_ECC_EN		0x0100
#define AUTO_FMT_EN  	0x0200
#define OP_IDLE         ((g_nand_sw_config.eSecSize==SECTOR_SIZE_512) ? (0x0000 | SEL_SEC_512BYTE) : (0x0000 & (~SEL_SEC_512BYTE)) )
#define OP_READ         ((g_nand_sw_config.eSecSize==SECTOR_SIZE_512) ? (0x1000 | SEL_SEC_512BYTE) : (0x1000 & (~SEL_SEC_512BYTE)) )
#define OP_READ_ID_ST   ((g_nand_sw_config.eSecSize==SECTOR_SIZE_512) ? (0x2000 | SEL_SEC_512BYTE) : (0x2000 & (~SEL_SEC_512BYTE)) )
#define OP_PROGRAM      ((g_nand_sw_config.eSecSize==SECTOR_SIZE_512) ? (0x3000 | SEL_SEC_512BYTE) : (0x3000 & (~SEL_SEC_512BYTE)) )
#define OP_ERASE        ((g_nand_sw_config.eSecSize==SECTOR_SIZE_512) ? (0x4000 | SEL_SEC_512BYTE) : (0x4000 & (~SEL_SEC_512BYTE)) )
#define OP_RESET        ((g_nand_sw_config.eSecSize==SECTOR_SIZE_512) ? (0x5000 | SEL_SEC_512BYTE) : (0x5000 & (~SEL_SEC_512BYTE)) )
#define OP_CUSTOM       ((g_nand_sw_config.eSecSize==SECTOR_SIZE_512) ? (0x6000 | SEL_SEC_512BYTE) : (0x6000 & (~SEL_SEC_512BYTE)) )
#define NAND_NFI_CNFG_OP_SINGLE                     (((uint32) 2) << 12)



/* NFI_PAGEFMT */
#define PAGEFMT_512		0x0000
#define PAGEFMT_2K		0x0001
#define PAGEFMT_4K		0x0002
#define PAGEFMT_8K		0x0003  //added by liuquanji
#define SPARE_16  		0x0000
#define SPARE_26  		0x0004
#define SPARE_27  		0x0008
#define SPARE_28  		0x000C
#define SPARE_32  		0x0010
#define SPARE_36  		0x0014
#define SPARE_40  		0x0018
#define SPARE_44  		0x001C
#define SPARE_48  		0x0020
#define SPARE_50  		0x0024
#define SPARE_52  		0x0028
#define SPARE_54  		0x002C
#define SPARE_56  		0x0030
#define SPARE_62  		0x0034
#define SPARE_63  		0x0038
#define SPARE_64  		0x003C 
#define FDM_NUM(x)      (((uint32)(x) &0x1F) << 6)
#define FDM_ECC_NUM(x)  (((uint32)(x) &0x1F) << 11)
#define SEC_SIZE(x)		(((uint32)(x) &0x7FF) << 16)


/* NFI_CON */
#define FIFO_FLUSH         0x0001
#define NFI_RST            0x0002
#define SINGLE_RD          0x0010
#define NOB_WORD           0x0040
#define NOB_DWORD          0x0080
#define BURST_RD           0x0100
#define BURST_WR           0x0200
#define NOB_BYTE           0x0020
#define NAND_NFI_CON_BRD_HW_EN	0x0400
#define SEC_NUM(x)                (((uint32)((x) &0x1F)) << 11)

/* NFI_ACCCON */
//#define ACCCON				0x07C6	// C2R=111, W2R=11, WH=00, WST=00, RLT=10
//#define ACCCON        0x071C0002  // C2R=111, W2R=11, WH=00, WST=00, RLT=10
#define ACCCON        (PRECS|C2R | W2R | WH | WST | RLT)

#define ACCCON2        (RLT)

// NFI_CLK
/*
#define PDN_NFI 0x00080000
#define NFI_CLK_144 0x00030000
#define NFI_CLK_27 0x0
#define NFI_CLK_200 0x00050000
#define NFI_CLK_234 0x00010000
*/
#define PDN_NFI 0x8
#define NFI_CLK_144 0x3
#define NFI_CLK_27 0x0
#define NFI_CLK_200 0x5
#define NFI_CLK_234 0x1
//#define NFI_CLK NFI_CLK_200
#define NFI_CLK NFI_CLK_27 

#if NFI_CLK == NFI_CLK_27
#define RLT           0x3        // 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=2
#define WST           (0xF << 4)   // 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=1
#define WH            (0xF<< 8)  // 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=1
#define W2R           (0xF << 12) // 0:2T, 1:4T, 2:6T, 3:8T ; recommanded value=3
#define C2R           ((uint32)0x3F << 16) // C2R=111
#define PRECS         ((uint32)0x3F << 22)
#define LCD2NAND      ((uint32)0xF << 28)    
#else
//#define RLT           0x07        // 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=2
#define RLT           (0x03)        // 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=2
#define WST           (0x05 << 4)   // 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=1
#define WH            (0x3<< 8)  // 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=1
#define W2R           (0xF << 12) // 0:2T, 1:4T, 2:6T, 3:8T ; recommanded value=3
#define C2R           ((uint32)0x8 << 16) // C2R=111
#define PRECS         ((uint32)0x1 << 22)
#define LCD2NAND      ((uint32)0x0 << 28)
#endif

/* NFI_INTR_EN */
#define RD_DONE_EN     0x01
#define WR_DONE_EN     0x02
#define RESET_DONE_EN  0x04
#define ERASE_DONE_EN  0x08
#define BUSY_RETURN_EN     0x0010
#define ACCESS_LOCK_EN     0x0020
#define AHB_DONE_EN          0x0040
//#define BUSY_RETURNEN2_EN   0x80
#define NAND_NFI_INTR_EN_RS232RD_DONE	0x100				
#define NAND_NFI_INTR_EN_RS232WR_DONE	0x200				
#define NAND_NFI_INTR_EN_MDMA_DONE  0x400  

/* NFI_INTR */
#define RD_DONE     0x01
#define WR_DONE     0x02
#define RESET_DONE  0x04
#define ERASE_DONE  0x08
//#define ERR_DET_EN         0x10
//#define ERR_COR_EN         0x100
#define BUSY_RETURN     0x0010
#define ACCESS_LOCK     0x0020
#define AHB_DONE        0x0040
//#define BUSY_RETURNEN2  0x80
#define NAND_NFI_INTR_RS232RD_DONE	0x100
#define NAND_NFI_INTR_RS232WR_DONE	0x200
#define NAND_NFI_INTR_MDMA_DONE 	0x400
#define NAND_NFI_INTR_MASK       (uint32) 0x44B)



/* NFI_ADDRNOB */
#define COL_ADDR_NOB(x)   (((uint32) x &0x07) )
#define ROW_ADDR_NOB(x)   (((uint32) x &0x07) << 4 )

/* NFI_STRDATA */
#define STR_DATA    0x01

/* NFI_STA  */
#define STATUS_CMD         	0x0001
#define STATUS_ADDR        	0x0002
#define STATUS_DATAR       	0x0004
#define STATUS_DATAW       	0x0008
#define STATUS_ACCESS_LOCK	0x0010
#define STATUS_BUSY        	0x0100
#define STATUS_READ_EMPTY	0x10000
#define NFI_FSM(x)			(((uint32)(x) &0x0F) <<16)
#define NAND_FSM(x)			(((uint32)(x) &0x1F) <<24)

#define NFI_Empty_Page()          ((*NFI_FIFOSTA & STATUS_READ_EMPTY)>0)
#define NFI_Empty_Tolerate  40
#define NFI_FIFO_Empty(timeout)   			while ( !(*NFI_FIFOSTA & WR_EMPTY_MASK)    && (timeout--) );
#define NFI_Wait_Command(timeout)   		while ( (*NFI_STA & STATUS_CMD)         && (timeout--) );
#define NFI_Wait_Adddress(timeout)   		while ( (*NFI_STA & STATUS_ADDR)        && (timeout--) );


#define NAND_FSM_MASK    (0x1F000000)
//#define NAND_BUSY        (0x00000100)
//#define NAND_BUSY_RETURN (0x00000200)
#define NFI_FSM_MASK     (0x000F0000)
//#define NFI_OP_MASK      (0x0000000F)




/* NFI_FIFOSTA */
#define RD_REMAIN_MASK		0x001F
#define RD_EMPTY_MASK      	0x0040
#define RD_FULL_MASK      	0x0080
#define WR_REMAIN_MASK     	0x1F00
#define WR_EMPTY_MASK      	0x4000
#define WR_FULL_MASK       	0x8000
#define NFI_EMPTY_BIT_NUM(x)    ((((uint32) x ) & 0x3F) << 24)
#define NFI_60BIT_EMPTY_BIT_CNT(x)  ((((uint32) x ) & 0x3F) << 18)
#define NFI_EMPTY_PAGE_FLAGE   0x1

/* NFI_CSEL */
#define CSEL(x)			((uint32)(x) &0x1)
#define RB_CS1          (0x0010)



/* NFI_RANDOM_CFG */
#define RANDOM_ENCODE_EN		(1 <<0)
#define RANDOM_ENCODE_SEED(x)	(((uint32)(x) &0x7FFF) <<1)
#define RANDOM_DECODE_EN		(1 <<16)
#define RANDOM_DECODE_SEED(x)	(((uint32)(x) &0x7FFF) <<17)



/* NFI_LOCKANOB */
#define PROG_RADD_NOB_MASK    (0x7000)
#define PROG_RADD_NOB_SHIFT   (12)
#define PROG_CADD_NOB_MASK    (0x0300)
#define PROG_CADD_NOB_SHIFT   (8)
#define ERASE_RADD_NOB_MASK   (0x0070)
#define ERASE_RADD_NOB_SHIFT  (4)
#define ERASE_CADD_NOB_MASK   (0x0007)
#define ERASE_CADD_NOB_SHIFT  (0)



typedef enum {
	SECTOR_SIZE_512 = 512
	,SECTOR_SIZE_1024 = 1024
	,SECTOR_SIZE_ERR = 0xFFFF
} SECTOR_SIZE_t;
#define NAND_SECTOR_SIZE (512)

/* Status register */
#define STATUS_FAIL			0x01
#define STATUS_READY		0x40
#define STATUS_WR_ALLOW		0x80
 

/*NFI_DMA*/
#define DMA_RDTRIG				((uint32) 1 << 1)
#define DMA_WRTRIG				((uint32) 1 << 0)

#define NFI_Wait_Ready(timeout)   while ( (*NFI_STA  & STATUS_BUSY) && (timeout--) )



#define NFI_EVENT_NAME 		"NFI_EVENT"

#ifdef _XOS_ISR_ENABLE_
	extern HANDLE_T _hNFIEvent;
	extern HANDLE_T _hNFISema;
#else

#endif //_XOS_ISR_ENABLE

extern bool _bUsingISR;

//NFI ECCC regiser
#define NFIECC_ENCCON		((volatile uint16 *)(NFI60ECC_BASE+0x0000))
    #define ENC_EN      		0x01
    
#define NFIECC_ENCCNFG		((volatile uint32 *)(NFI60ECC_BASE+0x0004))
    #define ENC_TNUM(x)			((((x) >24) ? (((uint32)(x) >>2) +4) : (((uint32)(x) >>1) -2)) & 0x1F)
    #define ENC_NFI_MODE        0x01 << 5
    #define ENC_MS(x)           (((uint32)(x) &0x3FFF) << 16)

#define NFIECC_ENCDIADDR	((volatile uint32 *)(NFI60ECC_BASE+0x0008))

#define NFIECC_ENCIDLE      ((volatile uint16 *)(NFI60ECC_BASE+0x000C))
    #define ENC_IDLE        	0x01


#define NFIECC_ENCIRQEN              ((volatile uint16 *)(NFI60ECC_BASE+0x00028))
#define ENC_IRQEN       	0x01

#define NFIECC_ENCIRQSTA             ((volatile uint16 *)(NFI60ECC_BASE+0x0002C))
#define ENC_IRQSTA      	0x01


	
#define NFIECC_ENCPAR0 		((volatile uint32 *)(NFI60ECC_BASE+0x0030))


#define NFIECC_DECCON		((volatile uint16 *)(NFI60ECC_BASE+0x0100))
    #define DEC_EN      		0x01

#define NFIECC_DECCNFG  	((volatile uint32 *)(NFI60ECC_BASE+0x0104))
    #define DEC_TNUM(x)     		((((x) >24) ? (((uint32)(x) >>2) +4) : (((uint32)(x) >>1) -2)) & 0x1F)
    #define DEC_NFI_MODE            0x01 << 5
	#define DEC_SEL_CHIEN_SEARCH	0x01 << 7
    #define DEC_CON(x)              (((uint32)(x) &0x03) << 12)     
    #define DEC_CS(x)               (((uint32)(x) &0x3FFF) << 16)
    #define DEC_EMPTY_EN            0x80000000

#define NFIECC_DECDIADDR    ((volatile uint32 *)(NFI60ECC_BASE+0x0108))

#define NFIECC_DECIDLE		((volatile uint16 *)(NFI60ECC_BASE+0x010C))
    #define DEC_IDLE			0x01

#define NFIECC_DECFER		((volatile uint16 *)(NFI60ECC_BASE+0x0110))
#define NFIECC_DECDONE      ((volatile uint16 *)(NFI60ECC_BASE+0x0118))

#define NFIECC_DECIRQEN		((volatile uint16 *)(NFI60ECC_BASE+0x0134))
    #define DEC_IRQEN       	0x01

#define NFIECC_DECIRQSTA	((volatile uint16 *)(NFI60ECC_BASE+0x0138))
    #define DEC_IRQSTA      	0x01

#define NFIECC_FDMADDR		((volatile uint32 *)(NFI60ECC_BASE+0x013C))


#define NFIECC_SYNSTA		((volatile uint32 *)(NFI60ECC_BASE+0x0144))
#define NFIECC_DECNFIDI		((volatile uint32 *)(NFI60ECC_BASE+0x0148))

#define NFIECC_DECENUM		((volatile uint32 *)(NFI60ECC_BASE+0x0150))
#define NFIECC_DECENUM2     ((volatile uint32 *)(NFI60ECC_BASE+0x0154))
#define NFIECC_DECENUM3		((volatile uint32 *)(NFI60ECC_BASE+0x0158))
#define NFIECC_DECENUM4     ((volatile uint32 *)(NFI60ECC_BASE+0x015c))


#define NFIECC_DECEL0		((volatile uint32 *)(NFI60ECC_BASE+0x160))


typedef enum {
	ECC_4_BITS = 4
   ,ECC_6_BITS = 6
   ,ECC_8_BITS = 8
   ,ECC_10_BITS = 10
   ,ECC_12_BITS = 12
   ,ECC_14_BITS = 14
   ,ECC_16_BITS = 16
   ,ECC_18_BITS = 18
   ,ECC_20_BITS = 20
   ,ECC_22_BITS = 22
   ,ECC_24_BITS = 24   
   ,ECC_28_BITS = 28
   ,ECC_32_BITS = 32
   ,ECC_36_BITS = 36
   ,ECC_40_BITS = 40
   ,ECC_44_BITS = 44
   ,ECC_48_BITS = 48
   ,ECC_52_BITS = 52
   ,ECC_56_BITS = 56
   ,ECC_60_BITS = 60   
} ECC_Level_t;

typedef enum {
   ECC_DEC_NONE,
   ECC_DEC_DETECT,
   ECC_DEC_LOCATE,
   ECC_DEC_CORRECT
} ECC_Decode_Type_t;

#define ECC_PARITY_NUM  14
#define TEST_MIRCON_16bit_NAND (0)

//
/* Nand flash command */
#define RD_1ST_CMD              (0x00)
#define RANDOM_RD_1ST_CMD       (0x05)
#define RD_2ND_HALF_CMD         (0x01)    // only for 512 bytes page-size
#define RD_SPARE_CMD            (0x50)    // only for 512 bytes page-size
#define RD_2ND_CYCLE_CMD        (0x30)    // only for 2K  bytes page-size
#define RANDOM_RD_2ND_CMD       (0xE0)
#define RD_FOR_COPYBACK         (0x35)
#define RD_FOR_CACHECOPY        (0x3A)
#define COPY_1ST_CMD            (0x85)
#define COPY_PAGE_CMD           (0x8C)
#define COPY_2ND_CMD            (0x10)
#define COPY_PROGRAM            (0x8A)
#define INPUT_DATA_CMD          (0x80)
#define PLANE_INPUT_DATA_CMD    (0x81)
#define RANDOM_INPUT_DATA_CMD   (0x85)
#define PROG_DATA_CMD           (0x10)
#define PLANE_PROG_DATA_CMD     (0x11)
#define CACHE_PROG_CMD          (0x15)
#define BLOCK_ERASE1_CMD        (0x60)
#define BLOCK_ERASE2_CMD        (0xD0)
#define RD_ID_CMD               (0x90)
#define RD_STATUS_CMD           (0x70)
#define RD_CACHE_STATUS_CMD     (0x71)
#define RESET_CMD               (0xFF)
#define CACHE_START_CMD         (0x11)
#define CACHE_RD_START_CMD      (0x31)
#define CACHE_RD_END_CMD        (0x3F)
#define CACHE_RD_COL_CMD        (0x05)
#define CACHE_RD_CONFIRM_CMD    (0xE0)


typedef struct {
	bool bUsingISR;				// use Interrupt handler, default is TRUE
	bool bInitialization;		// NFI initialization flag, default is FALSE
	
	bool bUsingDMA;				// AHB mode flag, default is TRUE
	bool bAUTO_FMT;				// Auto farmat enable, default is TRUE
	bool bECCSWCorrect;			// ECC SW correct flag, if g_nand_sw_config.bAUTO_FMT = FALSE, must is FALSE, default is FALSE
	
	bool bRandomizerEn;			// Randomizer feature enable flag, default is TRUE
	bool bDCMEnable;			// DCM feature enable flag, default is FALSE
	
	SECTOR_SIZE_t eSecSize;		// set sector size, value is SECTOR_SIZE_t, default is SECTOR_SIZE_1024
	
	ECC_Level_t eECC_BIT;		// set ECC bits,  value is ECC_Level_t, default is ECC_24_BITS
	uint32 u4SectCorrErrBits; 	//Record Sector Correct err bits counter, default is 0
	uint32 u4PageCorrErrBits; 	//Record Page Correct err bits counter, default is 0 
	bool bHW_ECC_EN;
	bool bDIE;
	bool nfi_interrupt;
} NAND_SW_CONFIG_S;
extern NAND_SW_CONFIG_S 	g_nand_sw_config;

#define FDM_BYTES           ((g_nand_sw_config.eSecSize==SECTOR_SIZE_512) ? 6 : 8)
#define FDM_ECC_BYTES       ((g_nand_sw_config.eSecSize==SECTOR_SIZE_512) ? 6 : 8)
#define SECTOR_BYTES		(g_nand_sw_config.eSecSize) 



#endif //_NFI_H

