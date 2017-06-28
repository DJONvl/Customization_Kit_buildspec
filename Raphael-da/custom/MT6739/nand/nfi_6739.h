/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001
*
*****************************************************************************/
#ifndef _NFI_6570_H_
#define _NFI_6570_H_

#include "sys/types.h"
//#include "hw_config.h"

//------------------------------------------------------------------------------
// NFI Register
//------------------------------------------------------------------------------

/* Register definition */

#define	NFI_base	    (0x11018000)

#ifndef BB_MT0699
#define BB_MT0699
#endif
/*******************************************************************************
 * NFI register definition
 *******************************************************************************/
#define NFI_CNFG        ((volatile uint16 *)(NFI_base+0x0000))
#define NFI_PAGEFMT     ((volatile uint32 *)(NFI_base+0x0004))
#define NFI_CON         ((volatile uint32 *)(NFI_base+0x0008))
#define NFI_ACCCON      ((volatile uint32 *)(NFI_base+0x000C))
#define NFI_INTR_EN     ((volatile uint16 *)(NFI_base+0x0010))
#define NFI_INTR        ((volatile uint16 *)(NFI_base+0x0014))

#define NFI_CMD         ((volatile uint16 *)(NFI_base+0x0020))

#define NFI_ADDRNOB     ((volatile uint16 *)(NFI_base+0x0030))
#define NFI_COLADDR     ((volatile uint32 *)(NFI_base+0x0034))
#define NFI_ROWADDR     ((volatile uint32 *)(NFI_base+0x0038))

#define NFI_STRDATA     ((volatile uint16 *)(NFI_base+0x0040))
#define NFI_CNRNB       ((volatile uint16 *)(NFI_base+0x0044))
#define NFI_DATAW       ((volatile uint32 *)(NFI_base+0x0050))
#define NFI_DATAR       ((volatile uint32 *)(NFI_base+0x0054))

#define NFI_PIO_DIRDY   ((volatile uint16 *)(NFI_base+0x0058))

#define NFI_STA         ((volatile uint32 *)(NFI_base+0x0060))
#define NFI_FIFOSTA     ((volatile uint16 *)(NFI_base+0x0064))
//#define NFI_LOCKSTA     ((volatile uint16 *)(NFI_base+0x0068))

#define NFI_ADDRCNTR    ((volatile uint32 *)(NFI_base+0x0070))

#define NFI_STRADDR     ((volatile uint32 *)(NFI_base+0x0080))
#define NFI_BYTELEN     ((volatile uint32 *)(NFI_base+0x0084))

#define NFI_CSEL        ((volatile uint16 *)(NFI_base+0x0090))
#define NFI_IOCON       ((volatile uint32 *)(NFI_base+0x0094))

#define NFI_FDM0L       ((volatile uint32 *)(NFI_base+0x00A0))
#define NFI_FDM0M       ((volatile uint32 *)(NFI_base+0x00A4))

#define NFI_CRC01       ((volatile uint32 *)(NFI_base+0x0120))

#define NFI_FIFODATA0   ((volatile uint32 *)(NFI_base+0x0190))
#define NFI_FIFODATA1   ((volatile uint32 *)(NFI_base+0x0194))
#define NFI_FIFODATA2   ((volatile uint32 *)(NFI_base+0x0198))
#define NFI_FIFODATA3   ((volatile uint32 *)(NFI_base+0x019C))

#define NFI_DEBUG_CON1  ((volatile uint16 *)(NFI_base+0x0220))
#define NFI_MASTER_STA  ((volatile uint16 *)(NFI_base+0x0224))
#define NFI_MASTER_RST  ((volatile uint32 *)(NFI_base+0x0228))

#define NFI_RANDOM_CNFG  				((volatile uint32 *)(NFI_base+0x0238))
#define NFI_EMPTY_THRESH 				((volatile uint32 *)(NFI_base+0x023C))
#define NFI_NAND_TYPE_CNFG  			((volatile uint32 *)(NFI_base+0x0240))
#define NFI_ACCCON1  					((volatile uint32 *)(NFI_base+0x0244))
#define NFI_DELAY_CTRL  				((volatile uint32 *)(NFI_base+0x0248))

#define NFI_RANDOM_TSB_SEED0 			((volatile uint32 *)(NFI_base+0x024C))
#define NFI_RANDOM_TSB_SEED1 			((volatile uint32 *)(NFI_base+0x0250))
#define NFI_RANDOM_TSB_SEED15 			((volatile uint32 *)(NFI_base+0x0290))

#define NFI_TLC_RD_WHR2 				((volatile uint16 *)(NFI_base+0x0300))
#define NFI_DQS_HIGH_SET 				((volatile uint32 *)(NFI_base+0x0304))
#define NFI_DQS_CMP_MODE 				((volatile uint32 *)(NFI_base+0x0308))

#define NFI_FLUSH 						((volatile uint32 *)(NFI_base+0x0490))

//#define NFI_1X_INFRA   					((volatile uint32 *)(0x10000004))
//#define NFI_ECC_FGMUX					((volatile uint32 *)(0x1000001C))
//#define NFI_DQS_DELAY_CTRL   			((volatile uint32 *)(0x100150B4))

/*******************************************************************************
 * NFI register definition
 *******************************************************************************/

/* NFI_SPIDMA */
#define SPIDMA_SEC_EN        (0x00010000)
#define SPIDMA_SEC_SIZE_MASK (0x0000FFFF)

/* NFI_CNFG */
#define CNFG_AHB             (0x0001)
#define CNFG_READ_EN         (0x0002)
#define CNFG_AHB_BURST       (0x0004)
#define CNFG_BYTE_RW         (0x0040)
#define CNFG_HW_ECC_EN       (0x0100)
#define CNFG_AUTO_FMT_EN     (0x0200)
#define CNFG_OP_IDLE         (0x0000)
#define CNFG_OP_READ         (0x1000)
#define CNFG_OP_SRD          (0x2000)
#define CNFG_OP_PRGM         (0x3000)
#define CNFG_OP_ERASE        (0x4000)
#define CNFG_OP_RESET        (0x5000)
#define CNFG_OP_CUST         (0x6000)

#define CNFG_OP_MODE_MASK    (0x7000)
#define CNFG_OP_MODE_SHIFT   (12)

#define SEED_MASK             (0x7FFF)
#define EN_SEED_SHIFT		  (0x1)
#define DE_SEED_SHIFT		  (0x11)
#define CNFG_RAN_SEC		 (0x0010)
#define CNFG_RAN_SEL		 (0x0020)


/* NFI_PAGEFMT */
#define PAGEFMT_512          (0x0000)
#define PAGEFMT_2K           (0x0001)
#define PAGEFMT_4K           (0x0002)
#define PAGEFMT_2K_1KS       (0x0000)
#define PAGEFMT_4K_1KS       (0x0001)
#define PAGEFMT_8K_1KS       (0x0002)
#define PAGEFMT_16K_1KS      (0x0003)

#define PAGEFMT_PAGE_MASK    (0x0003)

#define PAGEFMT_SEC_SEL_512  (0x0004)

//#define PAGEFMT_DBYTE_EN     (0x0008)
#define PAGEFMT_SECTOR_SIZE  (0x0004)

#define PAGEFMT_8BITS        (0x0000)
#define PAGEFMT_16BITS       (0x0008)

#define PAGEFMT_SPARE_16     (0x0000)
#define PAGEFMT_SPARE_26     (0x0001)
#define PAGEFMT_SPARE_27     (0x0002)
#define PAGEFMT_SPARE_28     (0x0003)
#define PAGEFMT_SPARE_32     (0x0004)
#define PAGEFMT_SPARE_36     (0x0005)
#define PAGEFMT_SPARE_40     (0x0006)
#define PAGEFMT_SPARE_44     (0x0007)
#define PAGEFMT_SPARE_48     (0x0008)
#define PAGEFMT_SPARE_49     (0x0009)
#define PAGEFMT_SPARE_50     (0x000A)
#define PAGEFMT_SPARE_51     (0x000B)
#define PAGEFMT_SPARE_52     (0x000C)
#define PAGEFMT_SPARE_62     (0x000D)
#define PAGEFMT_SPARE_63     (0x000E)
#define PAGEFMT_SPARE_64     (0x000F)

#define PAGEFMT_SPARE_32_1KS (0x0000)
#define PAGEFMT_SPARE_52_1KS (0x0001)
#define PAGEFMT_SPARE_54_1KS (0x0002)
#define PAGEFMT_SPARE_56_1KS (0x0003)
#define PAGEFMT_SPARE_64_1KS (0x0004)
#define PAGEFMT_SPARE_72_1KS (0x0005)
#define PAGEFMT_SPARE_80_1KS (0x0006)
#define PAGEFMT_SPARE_88_1KS (0x0007)
#define PAGEFMT_SPARE_96_1KS (0x0008)
#define PAGEFMT_SPARE_98_1KS (0x0009)
#define PAGEFMT_SPARE_100_1KS (0x000A)
#define PAGEFMT_SPARE_102_1KS (0x000B)
#define PAGEFMT_SPARE_104_1KS (0x000C)
#define PAGEFMT_SPARE_124_1KS (0x000D)
#define PAGEFMT_SPARE_122_1KS (0x000E)
#define PAGEFMT_SPARE_126_1KS (0x000F)
#define PAGEFMT_SPARE_128_1KS (0x0010)
#define PAGEFMT_SPARE_134_1KS (0x0011)
#define PAGEFMT_SPARE_148_1KS (0x0012)

#define PAGEFMT_SPARE_MASK   (0x1F0000)
#define PAGEFMT_SPARE_SHIFT  (16)

#define PAGEFMT_FDM_MASK     (0x0F00)
#define PAGEFMT_FDM_SHIFT    (8)

#define PAGEFMT_FDM_ECC_MASK  (0xF000)
#define PAGEFMT_FDM_ECC_SHIFT (12)

/* NFI_CON */
#define CON_FIFO_FLUSH       (0x0001)
#define CON_NFI_RST          (0x0002)
#define CON_NFI_SRD          (0x0010)

#define CON_NFI_NOB_MASK     (0x0060)
#define CON_NFI_NOB_SHIFT    (5)

#define CON_NFI_BRD          (0x0100)
#define CON_NFI_BWR          (0x0200)

#define CON_NFI_SEC_MASK     (0x1F000)
#define CON_NFI_SEC_SHIFT    (12)

/* NFI_ACCCON */
#define ACCCON_SETTING       ()

/* NFI_INTR_EN */
#define INTR_RD_DONE_EN      (0x0001)
#define INTR_WR_DONE_EN      (0x0002)
#define INTR_RST_DONE_EN     (0x0004)
#define INTR_ERASE_DONE_EN   (0x0008)
#define INTR_BSY_RTN_EN      (0x0010)
#define INTR_ACC_LOCK_EN     (0x0020)
#define INTR_AHB_DONE_EN     (0x0040)
#define INTR_CUSTOM_PROG_DONE_INTR_EN    (0x0080)
#define INTR_AUTO_BLKER_INTR_EN          (0x0800)

/* NFI_INTR */
#define INTR_RD_DONE         (0x0001)
#define INTR_WR_DONE         (0x0002)
#define INTR_RST_DONE        (0x0004)
#define INTR_ERASE_DONE      (0x0008)
#define INTR_BSY_RTN         (0x0010)
#define INTR_ACC_LOCK        (0x0020)
#define INTR_AHB_DONE        (0x0040)

/* NFI_ADDRNOB */
#define ADDR_COL_NOB_MASK    (0x0007)
#define ADDR_COL_NOB_SHIFT   (0)
#define ADDR_ROW_NOB_MASK    (0x00730)
#define ADDR_ROW_NOB_SHIFT   (4)

/* NFI_STA */
#define STA_READ_EMPTY       (0x00001000)
#define STA_ACC_LOCK         (0x00000010)
#define STA_CMD_STATE        (0x00000001)
#define STA_ADDR_STATE       (0x00000002)
#define STA_DATAR_STATE      (0x00000004)
#define STA_DATAW_STATE      (0x00000008)

#define STA_NAND_FSM_MASK    (0x3F000000)
#define STA_NAND_BUSY        (0x00000100)
#define STA_NAND_BUSY_RETURN (0x00000200)
#define STA_NFI_FSM_MASK     (0x000F0000)
#define STA_NFI_OP_MASK      (0x0000000F)
#define STA_FLASH_MACRO_IDLE (0x00000020)

/* NFI_MASTER_RST */
#define PAD_MACRO_RST        (2)
#define NFI_RST              (1)

/* NFI_FIFOSTA */
#define FIFO_RD_EMPTY        (0x0040)
#define FIFO_RD_FULL         (0x0080)
#define FIFO_WR_FULL         (0x8000)
#define FIFO_WR_EMPTY        (0x4000)
#define FIFO_RD_REMAIN(x)    (0x1F&(x))
#define FIFO_WR_REMAIN(x)    ((0x1F00&(x))>>8)

/* NFI_IOCON */
#define IOCON_CB2R_LAT       0xF00

/*NFI_PIO_DIRDY*/
#define FIFO_PIO_READY(x)  (0x1 & x)
#define WAIT_NFI_PIO_READY(timeout) \
	do { \
	   while( (!FIFO_PIO_READY(*NFI_PIO_DIRDY)) && (--timeout) ); \
	   if(timeout == 0) \
	   { \
	        while(1); \
	   } \
	} while(0);

/* NFI_ADDRCNTR */
#define ADDRCNTR_CNTR(x)     			((0x1F000&(x))>>12)
#define ADDRCNTR_OFFSET(x)   			(0x03FF&(x))

/*NFI_TLC_RD_WHR2*/
#define TLC_RD_WHR2_LAT_MASK 			(0xFFF)
#define TLC_RD_WHR2_EN 					(0x1000)

#if defined(__NFI_SUPPORT_TLC__)
#define SET_SLC_MODE_CMD 				0xA2
#define LOW_PG_SELECT_CMD 				0x01
#define MID_PG_SELECT_CMD 				0x02
#define HIGH_PG_SELECT_CMD 				0x03
#define PROGRAM_1ST_CYCLE_CMD 			0x09
#define PROGRAM_2ND_CYCLE_CMD 			0x0D
#define CHANGE_COLUNM_ADDR_1ST_CMD 		0x05
#define CHANGE_COLUNM_ADDR_2ND_CMD 		0xE0
#define PROGRAM_LEFT_PLANE_CMD 			0x11
#define PROGRAM_RIGHT_PLANE_CMD 		0x1A
#define NOT_KEEP_ERASE_LVL_15NM_CMD 	0xC6
#define NOT_KEEP_ERASE_LVL_A19NM_CMD 	0xDF

typedef enum
{
    PROGRAM_1ST_CYCLE = 1,
    PROGRAM_2ND_CYCLE = 2,
    PROGRAM_3RD_CYCLE = 3
}NFI_TLC_PG_CYCLE;

typedef enum
{
    WL_LOW_PAGE = 0,
    WL_MID_PAGE = 1,
    WL_HIGH_PAGE = 2
}NFI_TLC_WL_PRE;

typedef struct
{
    U32    word_line_idx;
    NFI_TLC_WL_PRE wl_pre;
}NFI_TLC_WL_INFO;
#endif


#define NFI_CNFG_REG16          NFI_CNFG
#define NFI_PAGEFMT_REG16       NFI_PAGEFMT
#define NFI_ACCCON_REG32        NFI_ACCCON
#define NFI_INTR_EN_REG16       NFI_INTR_EN
#define NFI_INTR_REG16          NFI_INTR
#define NFI_CMD_REG16           NFI_CMD
#define NFI_STA_REG32           NFI_STA
#define NFI_FIFOSTA_REG16       NFI_FIFOSTA
#define NFI_STRADDR_REG32       NFI_STRADDR
#define NFI_CSEL_REG16          NFI_CSEL
#define NFI_FDM0L_REG32         NFI_FDM0L
#define NFI_FDM0M_REG32         NFI_FDM0M

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

#define RD_CHIP1_STATUS_CMD     (0xF1)
#define RD_CHIP2_STATUS_CMD     (0xF2)

/* device status */
#define STATUS_FAIL				(0x01)
#define STATUS_READY			(0x40)
#define STATUS_WR_ALLOW			(0x80)

#define DEVICE_READY            (0x40)
#define OPERATION_FAIL          (0x01)

#define CHIP1_DISTRICT0_FAIL    (0x02)
#define CHIP1_DISTRICT1_FAIL    (0x04)
#define CHIP2_DISTRICT0_FAIL    (0x08)
#define CHIP2_DISTRICT1_FAIL    (0x10)
#define MULTI_DEVICE_READY      (0x20)
#define DATA_CACHE_READY        (0x40)
#define CHIP1_FAIL              (0x01)
#define CHIP2_FAIL              (0x02)

#define NAND_PLANE_OPERATIONS   (2)

#define NAND_CMD_DUMMYREAD      (0x00)  // SPI-NAND specific
#define NAND_CMD_DUMMYPROG      (0x80)  // SPI-NAND specific

/*******************************************************************************
 * Macro definition
 *******************************************************************************/
#define NFI_Empty_Page()          		((*NFI_STA & STA_READ_EMPTY)>0)
#define NFI_FIFO_Empty(timeout)   		while ( !(*NFI_FIFOSTA & FIFO_WR_EMPTY)    && (timeout--) );
#define NFI_Wait_Ready(timeout)   		while ( (*NFI_STA & STA_NAND_BUSY)         && (timeout--) );
#define NFI_Wait_Return_Ready(timeout)	while ( !(*NFI_STA & STA_NAND_BUSY_RETURN) && (timeout--) );
#define NFI_Wait_Command(timeout)   	while ( (*NFI_STA & STA_CMD_STATE)         && (timeout--) );
#define NFI_Wait_Adddress(timeout)   	while ( (*NFI_STA & STA_ADDR_STATE)        && (timeout--) );

#define NFI_FIFO_DEPTH 					(16) /* in bytes */

#define BOOTROM_ACCON      				(0x00033222)

#define BOOTROM_WRITE_CNFG 				(CNFG_OP_PRGM)
#define BOOTROM_READ_CNFG  				(CNFG_READ_EN|CNFG_OP_READ)
#define BOOTROM_SRD_CNFG   				(CNFG_READ_EN|CNFG_BYTE_RW|CNFG_OP_SRD)
#define BOOTROM_ERASE_CNFG 				(CNFG_OP_ERASE)

/* refer to the value specified in uart.c for BootROM to wait for request from flashtool */
#define DELAY_COUNT        				(54113000)
#define NORMAL_BUSY_PERIOD 				(DELAY_COUNT/10000)
#define PROG_BUSY_PERIOD   				(DELAY_COUNT*2/1000)
#define ERASE_BUSY_PERIOD				(DELAY_COUNT*2/100)

#ifdef __SECURE_BOOTROM__
	#define REPLICATION_NUMBER_H2		(512/sizeof(BOOTL_HEADER2))
#endif

#define NAND_SECTOR_SHIFT          		(9)

#define NAND_SECTOR_SIZE           		NUTL_SECTOR_SIZE()
//#define NAND_SECTOR_SIZE           		(512)


#define NAND_MAX_SECOTR_NUM        		(8)
#define NAND_BAD_MARK_OFFSET       		(512)
#define NAND_BAD_MARK_SPARE_OFFSET 		(0)
#define NAND_GOOD_BLOCK_THRESHOLD  		(6)
#define NAND_MAX_FDM_META_SIZE     		(8)

#define IO_8BITS   						0x0000
#define IO_16BITS  						0x0001
#define IO_TOGGLE_SDR					0x0002
#define IO_TOGGLE_DDR                 	0x0004
#define IO_ONFI                       	0x0008
#define IO_WIDTH_MASK                 	0x0001
#define IO_MASK                       	0x00FF
#define IO_WIDTH_MASK  					0x0001

#define NAND_SECTOR512					0x0000
#define NAND_SECTOR1K					0x0100
#define NAND_SECTOR_MASK				0xFF00

#define MAX_SPARE_SIZE					(128)

#define NFI_DEFAULT_ACCESS_TIMING		(0x44333)
#define NFI_DEFAULT_CS					(0)

#endif	/* _NFI_6739_H_ */
