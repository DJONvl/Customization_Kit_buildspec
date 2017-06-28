/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2010
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef _MSDC_H_
#define _MSDC_H_

#include "msdc_cfg.h"
#include "mmc_types.h"
#include "msdc_utils.h"
#include "mmc_core.h"
#include "mmc_test.h"
#include "inc/register_base.h"
#include "inc/register_act_util.h"
#include "msdc_cust.h"


/*--------------------------------------------------------------------------*/
/* Common Macro 							    */
/*--------------------------------------------------------------------------*/
#define REG_ADDR(x)		((volatile uint32*)(base + OFFSET_##x))

/*--------------------------------------------------------------------------*/
/* Common Definition							    */
/*--------------------------------------------------------------------------*/
#define MSDC_FIFO_SZ		(128)
#define MSDC_FIFO_THD		(64)
#define MSDC_MAX_NUM		(4)

#define MSDC_MS 		(0)
#define MSDC_SDMMC		(1)

#define MSDC_MODE_UNKNOWN	(0)
#define MSDC_MODE_PIO		(1)
#define MSDC_MODE_DMA_BASIC	(2)
#define MSDC_MODE_DMA_DESC	(3)
#define MSDC_MODE_DMA_ENHANCED	(4)
#define MSDC_MODE_MMC_STREAM	(5)

#define MSDC_BUS_1BITS		(0)
#define MSDC_BUS_4BITS		(1)
#define MSDC_BUS_8BITS		(2)

#define MSDC_BRUST_8B		(3)
#define MSDC_BRUST_16B		(4)
#define MSDC_BRUST_32B		(5)
#define MSDC_BRUST_64B		(6)

#define MSDC_PIN_PULL_NONE	(0)
#define MSDC_PIN_PULL_DOWN	(1)
#define MSDC_PIN_PULL_UP	(2)
#define MSDC_PIN_KEEP		(3)

/* FIXME. E1 doesn't support SDR104 (Need ECO) so we limit the max. card clock
* to 100MHz, which only allows card in DDR50, SDR50/25/12, HS, ST speed mode.
*/
//#define MSDC_MAX_SCLK 	  (197000000)
#define MSDC_MAX_SCLK		(200000000)
#define MSDC_MIN_SCLK		(260000)

#define MSDC_AUTOCMD12		(0x0001)
#define MSDC_AUTOCMD23		(0x0002)
#define MSDC_AUTOCMD19		(0x0003)

/*--------------------------------------------------------------------------*/
/* Register Offset							    */
/*--------------------------------------------------------------------------*/
#define OFFSET_MSDC_CFG 		 (0x0)
#define OFFSET_MSDC_IOCON		 (0x04)
#define OFFSET_MSDC_PS			 (0x08)
#define OFFSET_MSDC_INT 		 (0x0c)
#define OFFSET_MSDC_INTEN		 (0x10)
#define OFFSET_MSDC_FIFOCS		 (0x14)
#define OFFSET_MSDC_TXDATA		 (0x18)
#define OFFSET_MSDC_RXDATA		 (0x1c)
#define OFFSET_SDC_CFG			 (0x30)
#define OFFSET_SDC_CMD			 (0x34)
#define OFFSET_SDC_ARG			 (0x38)
#define OFFSET_SDC_STS			 (0x3c)
#define OFFSET_SDC_RESP0		 (0x40)
#define OFFSET_SDC_RESP1		 (0x44)
#define OFFSET_SDC_RESP2		 (0x48)
#define OFFSET_SDC_RESP3		 (0x4c)
#define OFFSET_SDC_BLK_NUM		 (0x50)
#define OFFSET_SDC_VOL_CHG		 (0x54)
#define OFFSET_SDC_CSTS 		 (0x58)
#define OFFSET_SDC_CSTS_EN		 (0x5c)
#define OFFSET_SDC_DCRC_STS		 (0x60)
#define OFFSET_EMMC_CFG0		 (0x70)
#define OFFSET_EMMC_CFG1		 (0x74)
#define OFFSET_EMMC_STS 		 (0x78)
#define OFFSET_EMMC_IOCON		 (0x7c)
#define OFFSET_SDC_ACMD_RESP		 (0x80)
#define OFFSET_SDC_ACMD19_TRG		 (0x84)
#define OFFSET_SDC_ACMD19_STS		 (0x88)
#define OFFSET_MSDC_DMA_SA_HIGH4BIT	 (0x8C)
#define OFFSET_MSDC_DMA_SA		 (0x90)
#define OFFSET_MSDC_DMA_CA		 (0x94)
#define OFFSET_MSDC_DMA_CTRL		 (0x98)
#define OFFSET_MSDC_DMA_CFG		 (0x9c)
#define OFFSET_MSDC_DBG_SEL		 (0xa0)
#define OFFSET_MSDC_DBG_OUT		 (0xa4)
#define OFFSET_MSDC_DMA_LEN		 (0xa8)
#define OFFSET_MSDC_PATCH_BIT0		 (0xb0)
#define OFFSET_MSDC_PATCH_BIT1		 (0xb4)
#define OFFSET_MSDC_PATCH_BIT2		 (0xb8)
#define OFFSET_DAT0_TUNE_CRC		 (0xc0)
#define OFFSET_DAT1_TUNE_CRC		 (0xc4)
#define OFFSET_DAT2_TUNE_CRC		 (0xc8)
#define OFFSET_DAT3_TUNE_CRC		 (0xcc)
#define OFFSET_CMD_TUNE_CRC		 (0xd0)
#define OFFSET_SDIO_TUNE_WIND		 (0xd4)
#define OFFSET_MSDC_PAD_TUNE0		 (0xf0)
#define OFFSET_MSDC_DAT_RDDLY0		 (0xf8)
#define OFFSET_MSDC_DAT_RDDLY1		 (0xfc)
#define OFFSET_MSDC_HW_DBG		 (0x110)
#define OFFSET_MSDC_VERSION		 (0x114)
#define OFFSET_MSDC_ECO_VER		 (0x118)
#define OFFSET_EMMC50_PAD_CTL0		 (0x180)
#define OFFSET_EMMC50_PAD_DS_CTL0	 (0x184)
#define OFFSET_EMMC50_PAD_DS_TUNE	 (0x188)
#define OFFSET_EMMC50_PAD_CMD_TUNE	 (0x18c)
#define OFFSET_EMMC50_PAD_DAT01_TUNE	 (0x190)
#define OFFSET_EMMC50_PAD_DAT23_TUNE	 (0x194)
#define OFFSET_EMMC50_PAD_DAT45_TUNE	 (0x198)
#define OFFSET_EMMC50_PAD_DAT67_TUNE	 (0x19c)
#define OFFSET_EMMC51_CFG0		 (0x204)
#define OFFSET_EMMC50_CFG0		 (0x208)
#define OFFSET_EMMC50_CFG1		 (0x20c)
#define OFFSET_EMMC50_CFG2		 (0x21c)
#define OFFSET_EMMC50_CFG3		 (0x220)
#define OFFSET_EMMC50_CFG4		 (0x224)

/*--------------------------------------------------------------------------*/
/* Register Address							    */
/*--------------------------------------------------------------------------*/

/* common register */
#define MSDC_CFG		    REG_ADDR(MSDC_CFG)
#define MSDC_IOCON		    REG_ADDR(MSDC_IOCON)
#define MSDC_PS 		    REG_ADDR(MSDC_PS)
#define MSDC_INT		    REG_ADDR(MSDC_INT)
#define MSDC_INTEN		    REG_ADDR(MSDC_INTEN)
#define MSDC_FIFOCS		    REG_ADDR(MSDC_FIFOCS)
#define MSDC_TXDATA		    REG_ADDR(MSDC_TXDATA)
#define MSDC_RXDATA		    REG_ADDR(MSDC_RXDATA)

/* sdmmc register */
#define SDC_CFG 		    REG_ADDR(SDC_CFG)
#define SDC_CMD 		    REG_ADDR(SDC_CMD)
#define SDC_ARG 		    REG_ADDR(SDC_ARG)
#define SDC_STS 		    REG_ADDR(SDC_STS)
#define SDC_RESP0		    REG_ADDR(SDC_RESP0)
#define SDC_RESP1		    REG_ADDR(SDC_RESP1)
#define SDC_RESP2		    REG_ADDR(SDC_RESP2)
#define SDC_RESP3		    REG_ADDR(SDC_RESP3)
#define SDC_BLK_NUM		    REG_ADDR(SDC_BLK_NUM)
#define SDC_VOL_CHG		    REG_ADDR(SDC_VOL_CHG)
#define SDC_CSTS		    REG_ADDR(SDC_CSTS)
#define SDC_CSTS_EN		    REG_ADDR(SDC_CSTS_EN)
#define SDC_DCRC_STS		    REG_ADDR(SDC_DCRC_STS)

/* emmc register*/
#define EMMC_CFG0		    REG_ADDR(EMMC_CFG0)
#define EMMC_CFG1		    REG_ADDR(EMMC_CFG1)
#define EMMC_STS		    REG_ADDR(EMMC_STS)
#define EMMC_IOCON		    REG_ADDR(EMMC_IOCON)

/* auto command register */
#define SDC_ACMD_RESP		    REG_ADDR(SDC_ACMD_RESP)
#define SDC_ACMD19_TRG		    REG_ADDR(SDC_ACMD19_TRG)
#define SDC_ACMD19_STS		    REG_ADDR(SDC_ACMD19_STS)

/* dma register */
#define MSDC_DMA_SA_HIGH4BIT	REG_ADDR(MSDC_DMA_SA_HIGH4BIT)
#define MSDC_DMA_SA		        REG_ADDR(MSDC_DMA_SA)
#define MSDC_DMA_CA		        REG_ADDR(MSDC_DMA_CA)
#define MSDC_DMA_CTRL		    REG_ADDR(MSDC_DMA_CTRL)
#define MSDC_DMA_CFG		    REG_ADDR(MSDC_DMA_CFG)
#define MSDC_DMA_LEN		    REG_ADDR(MSDC_DMA_LEN)

/* data read delay */
#define MSDC_DAT_RDDLY0 	    REG_ADDR(MSDC_DAT_RDDLY0)
#define MSDC_DAT_RDDLY1 	    REG_ADDR(MSDC_DAT_RDDLY1)

/* debug register */
#define MSDC_DBG_SEL		    REG_ADDR(MSDC_DBG_SEL)
#define MSDC_DBG_OUT		    REG_ADDR(MSDC_DBG_OUT)

/* misc register */
#define MSDC_PATCH_BIT0 	    REG_ADDR(MSDC_PATCH_BIT0)
#define MSDC_PATCH_BIT1 	    REG_ADDR(MSDC_PATCH_BIT1)
#define MSDC_PATCH_BIT2 	    REG_ADDR(MSDC_PATCH_BIT2)
#define DAT0_TUNE_CRC		    REG_ADDR(DAT0_TUNE_CRC)
#define DAT1_TUNE_CRC		    REG_ADDR(DAT1_TUNE_CRC)
#define DAT2_TUNE_CRC		    REG_ADDR(DAT2_TUNE_CRC)
#define DAT3_TUNE_CRC		    REG_ADDR(DAT3_TUNE_CRC)
#define CMD_TUNE_CRC		    REG_ADDR(CMD_TUNE_CRC)
#define SDIO_TUNE_WIND		    REG_ADDR(SDIO_TUNE_WIND)
#define MSDC_PAD_TUNE0		    REG_ADDR(MSDC_PAD_TUNE0)
#define MSDC_HW_DBG		        REG_ADDR(MSDC_HW_DBG)
#define MSDC_VERSION		    REG_ADDR(MSDC_VERSION)
#define MSDC_ECO_VER		    REG_ADDR(MSDC_ECO_VER)

/* eMMC 5.0 register */
#define EMMC50_PAD_CTL0 	    REG_ADDR(EMMC50_PAD_CTL0)
#define EMMC50_PAD_DS_CTL0	    REG_ADDR(EMMC50_PAD_DS_CTL0)
#define EMMC50_PAD_DS_TUNE	    REG_ADDR(EMMC50_PAD_DS_TUNE)
#define EMMC50_PAD_CMD_TUNE	    REG_ADDR(EMMC50_PAD_CMD_TUNE)
#define EMMC50_PAD_DAT01_TUNE	    REG_ADDR(EMMC50_PAD_DAT01_TUNE)
#define EMMC50_PAD_DAT23_TUNE	    REG_ADDR(EMMC50_PAD_DAT23_TUNE)
#define EMMC50_PAD_DAT45_TUNE	    REG_ADDR(EMMC50_PAD_DAT45_TUNE)
#define EMMC50_PAD_DAT67_TUNE	    REG_ADDR(EMMC50_PAD_DAT67_TUNE)
#define EMMC51_CFG0		    REG_ADDR(EMMC51_CFG0)
#define EMMC50_CFG0		    REG_ADDR(EMMC50_CFG0)
#define EMMC50_CFG1		    REG_ADDR(EMMC50_CFG1)
#define EMMC50_CFG2		    REG_ADDR(EMMC50_CFG2)
#define EMMC50_CFG3		    REG_ADDR(EMMC50_CFG3)
#define EMMC50_CFG4		    REG_ADDR(EMMC50_CFG4)

/*--------------------------------------------------------------------------*/
/* Register Mask							    */
/*--------------------------------------------------------------------------*/

/* MSDC_CFG mask */
#define MSDC_CFG_MODE		(0x1  << 0)	/* RW */
#define MSDC_CFG_CKPDN		(0x1  << 1)	/* RW */
#define MSDC_CFG_RST		(0x1  << 2)	/* A0 */
#define MSDC_CFG_PIO		(0x1  << 3)	/* RW */
#define MSDC_CFG_CKDRVEN	(0x1  << 4)	/* RW */
#define MSDC_CFG_BV18SDT	(0x1  << 5)	/* RW */
#define MSDC_CFG_BV18PSS	(0x1  << 6)	/* R  */
#define MSDC_CFG_CKSTB		(0x1  << 7)	/* R  */
#define MSDC_CFG_CKDIV		(0xfff << 8)	 /* RW */
#define MSDC_CFG_CKMOD		(0x3  << 20)	/* W1C */
#define MSDC_CFG_CKMOD_HS400	(0x1  << 22)	/* RW */
#define MSDC_CFG_START_BIT	(0x3  << 23)	/* RW */
#define MSDC_CFG_SCLK_STOP_DDR	(0x1  << 25)	/* RW */

/* MSDC_IOCON mask */
#define MSDC_IOCON_SDR104CKS	(0x1  << 0)	/* RW */
#define MSDC_IOCON_RSPL 	(0x1  << 1)	/* RW */
#define MSDC_IOCON_R_D_SMPL	    (0x1  << 2)     /* RW */
#define MSDC_IOCON_DDLSEL	(0x1  << 3)	/* RW */
#define MSDC_IOCON_DDR50CKD	(0x1  << 4)	/* RW */
#define MSDC_IOCON_R_D_SMPL_SEL      (0x1  << 5)     /* RW */
#define MSDC_IOCON_W_D_SMPL	(0x1  << 8)	/* RW */
#define MSDC_IOCON_W_D_SMPL_SEL (0x1  << 9)	/* RW */
#define MSDC_IOCON_W_D0SPL	(0x1  << 10)	 /* RW */
#define MSDC_IOCON_W_D1SPL	(0x1  << 11)	 /* RW */
#define MSDC_IOCON_W_D2SPL	(0x1  << 12)	 /* RW */
#define MSDC_IOCON_W_D3SPL	(0x1  << 13)	 /* RW */

#define MSDC_IOCON_R_D0SPL	  (0x1	<< 16)	  /* RW */
#define MSDC_IOCON_R_D1SPL	  (0x1	<< 17)	  /* RW */
#define MSDC_IOCON_R_D2SPL	  (0x1	<< 18)	  /* RW */
#define MSDC_IOCON_R_D3SPL	  (0x1	<< 19)	  /* RW */
#define MSDC_IOCON_R_D4SPL	  (0x1	<< 20)	  /* RW */
#define MSDC_IOCON_R_D5SPL	  (0x1	<< 21)	  /* RW */
#define MSDC_IOCON_R_D6SPL	  (0x1	<< 22)	  /* RW */
#define MSDC_IOCON_R_D7SPL	  (0x1	<< 23)	  /* RW */
#define MSDC_IOCON_RISCSZ	(0x3  << 24)	/* RW */

/* MSDC_PS mask */
#define MSDC_PS_CDEN		(0x1  << 0)	/* RW */
#define MSDC_PS_CDSTS		(0x1  << 1)	/* R  */
#define MSDC_PS_CDDEBOUNCE	(0xf  << 12)	/* RW */
#define MSDC_PS_DAT		(0xff << 16)	/* R  */
#define MSDC_PS_CMD		(0x1  << 24)	/* R  */
#define MSDC_PS_WP		(0x1UL<< 31)	/* R  */

/* MSDC_INT mask */
#define MSDC_INT_MMCIRQ 	(0x1  << 0)	/* W1C */
#define MSDC_INT_CDSC		(0x1  << 1)	/* W1C */
#define MSDC_INT_ACMDRDY	(0x1  << 3)	/* W1C */
#define MSDC_INT_ACMDTMO	(0x1  << 4)	/* W1C */
#define MSDC_INT_ACMDCRCERR	(0x1  << 5)	/* W1C */
#define MSDC_INT_DMAQ_EMPTY	(0x1  << 6)	/* W1C */
#define MSDC_INT_SDIOIRQ	(0x1  << 7)	/* W1C */
#define MSDC_INT_CMDRDY 	(0x1  << 8)	/* W1C */
#define MSDC_INT_CMDTMO 	(0x1  << 9)	/* W1C */
#define MSDC_INT_RSPCRCERR	(0x1  << 10)	/* W1C */
#define MSDC_INT_CSTA		(0x1  << 11)	/* R */
#define MSDC_INT_XFER_COMPL	(0x1  << 12)	/* W1C */
#define MSDC_INT_DXFER_DONE	(0x1  << 13)	/* W1C */
#define MSDC_INT_DATTMO 	(0x1  << 14)	/* W1C */
#define MSDC_INT_DATCRCERR	(0x1  << 15)	/* W1C */
#define MSDC_INT_ACMD19_DONE	(0x1  << 16)	/* W1C */
#define MSDC_INT_BDCSERR	(0x1  << 17)	/* W1C */
#define MSDC_INT_GPDCSERR	(0x1  << 18)	/* W1C */
#define MSDC_INT_DMAPRO 	(0x1  << 19)	/* W1C */
#define MSDC_INT_AXI_RESP_ERR	(0x1  << 23)	/* W1C */
#ifdef MTK_SDIO30_TEST_MODE_SUPPORT
#define MSDC_INT_GRAE_OUT_BOUND (0x1  << 20)	/* W1C */
#define MSDC_INT_ACMD53_DONE	(0x1  << 21)	/* W1C */
#define MSDC_INT_ACMD53_FAIL	(0x1  << 22)	/* W1C */
#endif	// MTK_SDIO30_TEST_MODE_SUPPORT

/* MSDC_INTEN mask */
#define MSDC_INTEN_MMCIRQ	(0x1  << 0)	/* RW */
#define MSDC_INTEN_CDSC 	(0x1  << 1)	/* RW */
#define MSDC_INTEN_ACMDRDY	(0x1  << 3)	/* RW */
#define MSDC_INTEN_ACMDTMO	(0x1  << 4)	/* RW */
#define MSDC_INTEN_ACMDCRCERR	(0x1  << 5)	/* RW */
#define MSDC_INTEN_DMAQ_EMPTY	(0x1  << 6)	/* RW */
#define MSDC_INTEN_SDIOIRQ	(0x1  << 7)	/* RW */
#define MSDC_INTEN_CMDRDY	(0x1  << 8)	/* RW */
#define MSDC_INTEN_CMDTMO	(0x1  << 9)	/* RW */
#define MSDC_INTEN_RSPCRCERR	(0x1  << 10)	/* RW */
#define MSDC_INTEN_CSTA 	(0x1  << 11)	/* RW */
#define MSDC_INTEN_XFER_COMPL	(0x1  << 12)	/* RW */
#define MSDC_INTEN_DXFER_DONE	(0x1  << 13)	/* RW */
#define MSDC_INTEN_DATTMO	(0x1  << 14)	/* RW */
#define MSDC_INTEN_DATCRCERR	(0x1  << 15)	/* RW */
#define MSDC_INTEN_ACMD19_DONE	(0x1  << 16)	/* RW */
#define MSDC_INTEN_BDCSERR	(0x1  << 17)	/* RW */
#define MSDC_INTEN_GPDCSERR	(0x1  << 18)	/* RW */
#define MSDC_INTEN_DMAPRO	(0x1  << 19)	/* RW */
#define MSDC_INTEN_GOBOUND	(0x1  << 20)	/* RW */
#define MSDC_INTEN_ACMD53_DONE	(0x1  << 21)	/* RW */
#define MSDC_INTEN_ACMD53_FAIL	(0x1  << 22)	/* RW */
#define MSDC_INTEN_AXI_RESP_ERR   (0x1	<< 23)	  /* RW */


/* MSDC_FIFOCS mask */
#define MSDC_FIFOCS_RXCNT	(0xff << 0)	/* R */
#define MSDC_FIFOCS_TXCNT	(0xff << 16)	/* R */
#define MSDC_FIFOCS_CLR 	(0x1UL<< 31)	/* RW */

/* SDC_CFG mask */
#define SDC_CFG_SDIOINTWKUP	(0x1  << 0)	/* RW */
#define SDC_CFG_INSWKUP 	(0x1  << 1)	/* RW */
#define SDC_CFG_BUSWIDTH	(0x3  << 16)	/* RW */
#define SDC_CFG_SDIO		(0x1  << 19)	/* RW */
#define SDC_CFG_SDIOIDE 	(0x1  << 20)	/* RW */
#define SDC_CFG_INTATGAP	(0x1  << 21)	/* RW */
#define SDC_CFG_DTOC		(0xffUL << 24)	/* RW */

/* SDC_CMD mask */
#define SDC_CMD_OPC		(0x3f << 0)	/* RW */
#define SDC_CMD_BRK		(0x1  << 6)	/* RW */
#define SDC_CMD_RSPTYP		(0x7  << 7)	/* RW */
#define SDC_CMD_DTYP		(0x3  << 11)	/* RW */
#define SDC_CMD_DTYP		(0x3  << 11)	/* RW */
#define SDC_CMD_RW		(0x1  << 13)	/* RW */
#define SDC_CMD_STOP		(0x1  << 14)	/* RW */
#define SDC_CMD_GOIRQ		(0x1  << 15)	/* RW */
#define SDC_CMD_BLKLEN		(0xfff<< 16)	/* RW */
#define SDC_CMD_AUTOCMD 	(0x3  << 28)	/* RW */
#define SDC_CMD_VOLSWTH 	(0x1  << 30)	/* RW */
#define SDC_CMD_ACMD53		(0x1UL	<< 31)	  /* RW */

/* SDC_VOL_CHG mask */
#define SDC_VOL_CHG_CNT 	(0xffff <<0)	/* RW */

#define SDC_STS_SDCBUSY 	(0x1  << 0)	/* RW */
#define SDC_STS_CMDBUSY 	(0x1  << 1)	/* RW */
#define SDC_STS_SWR_COMPL	(0x1UL	<< 31)	  /* RW */

/* SDC_DCRC_STS mask */
#define SDC_DCRC_STS_NEG	(0xff << 8)	/* RO */
#define SDC_DCRC_STS_POS	(0xff << 0)	/* RO */

/* EMMC_CFG0 mask */
#define EMMC_CFG0_BOOTSTART	(0x1  << 0)	/* W */
#define EMMC_CFG0_BOOTSTOP	(0x1  << 1)	/* W */
#define EMMC_CFG0_BOOTMODE	(0x1  << 2)	/* RW */
#define EMMC_CFG0_BOOTACKDIS	(0x1  << 3)	/* RW */
#define EMMC_CFG0_BOOTWDLY	(0x7  << 12)	/* RW */
#define EMMC_CFG0_BOOTSUPP	(0x1  << 15)	/* RW */

/* EMMC_CFG1 mask */
#define EMMC_CFG1_BOOTDATTMC	(0xfffff << 0)	/* RW */
#define EMMC_CFG1_BOOTACKTMC	(0xfffUL << 20) /* RW */

/* EMMC_STS mask */
#define EMMC_STS_BOOTCRCERR	(0x1  << 0)	/* W1C */
#define EMMC_STS_BOOTACKERR	(0x1  << 1)	/* W1C */
#define EMMC_STS_BOOTDATTMO	(0x1  << 2)	/* W1C */
#define EMMC_STS_BOOTACKTMO	(0x1  << 3)	/* W1C */
#define EMMC_STS_BOOTUPSTATE	(0x1  << 4)	/* R */
#define EMMC_STS_BOOTACKRCV	(0x1  << 5)	/* W1C */
#define EMMC_STS_BOOTDATRCV	(0x1  << 6)	/* R */

/* EMMC_IOCON mask */
#define EMMC_IOCON_BOOTRST	(0x1  << 0)	/* RW */

/* SDC_ACMD19_TRG mask */
#define SDC_ACMD19_TRG_TUNESEL	(0xf  << 0)	/* RW */

/* MSDC_DMA_SA_HIGH4BIT mask  */
#define MSDC_DMA_SURR_ADDR_HIGH4BIT (0xf << 0)	/* RW */

/* MSDC_DMA_CTRL mask */
#define MSDC_DMA_CTRL_START	(0x1  << 0)	/* W */
#define MSDC_DMA_CTRL_STOP	(0x1  << 1)	/* W */
#define MSDC_DMA_CTRL_RESUME	(0x1  << 2)	/* W */
#define MSDC_DMA_CTRL_MODE	(0x1  << 8)	/* RW */
#define MSDC_DMA_CTRL_LASTBUF	(0x1  << 10)	/* RW */
#define MSDC_DMA_CTRL_BRUSTSZ	(0x7  << 12)	/* RW */

/* MSDC_DMA_CFG mask */
#define MSDC_DMA_CFG_STS	(0x1  << 0)	/* R */
#define MSDC_DMA_CFG_DECSEN	(0x1  << 1)	/* RW */
#define MSDC_DMA_CFG_LOCKDISABLE (0x1 << 2)	/* RW */
#define MSDC_DMA_CFG_AHBEN	(0x3  << 8)	/* RW */
#define MSDC_DMA_CFG_ACTEN	(0x3  << 12)	/* RW */
#define MSDC_DMA_CFG_CS12B	(0x1  << 16)	/* RW */
#define MSDC_DMA_CFG_OUTB_STOP	(0x1  << 17)	/* RW */

/* MSDC_PATCH_BIT mask */
#define MSDC_PATCH_BIT_ODDSUPP	  (0x1	<<  1)	   /* RW */

#ifdef MTK_SDIO30_TEST_MODE_SUPPORT
#define MSDC_MASK_ACMD53_CRC_ERR_INTR	(0x1<<4)
#define MSDC_ACMD53_FAIL_ONE_SHOT	(0X1<<5)
#endif	// MTK_SDIO30_TEST_MODE_SUPPORT

/* MSDC_PAD_TUNE0 mask */
#define MSDC_PAD_TUNE0_DATWRDLY  (0x1F << 0)	 /* RW */
#define MSDC_PAD_TUNE0_DELAYEN	 (0x1  << 7)	 /* RW */
#define MSDC_PAD_TUNE0_DATRRDLY  (0x1F << 8)	 /* RW */
#define MSDC_PAD_TUNE0_CMDRDLY	 (0x1F << 16)	 /* RW */
#define MSDC_PAD_TUNE0_CMDRRDLY  (0x1FUL << 22)  /* RW */
#define MSDC_PAD_TUNE0_CLKTXDLY  (0x1FUL << 27)  /* RW */

/* MSDC_DAT_RDDLY0/1 mask */
#define MSDC_DAT_RDDLY0_D3	(0x1F << 0)	/* RW */
#define MSDC_DAT_RDDLY0_D2	(0x1F << 8)	/* RW */
#define MSDC_DAT_RDDLY0_D1	(0x1F << 16)	/* RW */
#define MSDC_DAT_RDDLY0_D0	(0x1FUL << 24)	  /* RW */

#define MSDC_DAT_RDDLY1_D7	(0x1F << 0)	/* RW */
#define MSDC_DAT_RDDLY1_D6	(0x1F << 8)	/* RW */
#define MSDC_DAT_RDDLY1_D5	(0x1F << 16)	/* RW */
#define MSDC_DAT_RDDLY1_D4	(0x1FUL << 24)	  /* RW */

/* MSDC_PATCH_BIT0 mask */
#define MSDC_PB0_RESV1		 (0x1 << 0)
#define MSDC_PB0_EN_8BITSUP	 (0x1 << 1)
#define MSDC_PB0_DIS_RECMDWR	 (0x1 << 2)
#define MSDC_PB0_RESV2		 (0x1 << 3)
#define MSDC_PB0_ACMD53_CRCINTR  (0x1 << 4)
#define MSDC_PB0_ACMD53_ONESHOT  (0x1 << 5)
#define MSDC_PB0_RESV3		 (0x1 << 6)
#define MSDC_PB0_INT_DAT_LATCH_CK_SEL (0x7 << 7)
#define MSDC_PB0_CKGEN_MSDC_DLY_SEL   (0x1F<<10)
#define MSDC_PB0_FIFORD_DIS	 (0x1 << 15)
#define MSDC_PB0_SDIO_DBSSEL	 (0x1 << 16)
#define MSDC_PB0_SDIO_INTCSEL	 (0x1 << 17)
#define MSDC_PB0_SDIO_BSYDLY	 (0xf << 18)
#define MSDC_PB0_SDC_WDOD	 (0xf << 22)
#define MSDC_PB0_CMDIDRTSEL	 (0x1 << 26)
#define MSDC_PB0_CMDFAILSEL	 (0x1 << 27)
#define MSDC_PB0_SDIO_INTDLYSEL  (0x1 << 28)
#define MSDC_PB0_SPCPUSH	 (0x1 << 29)
#define MSDC_PB0_DETWR_CRCTMO	 (0x1 << 30)
#define MSDC_PB0_EN_DRVRSP	 (0x1UL << 31)

/* MSDC_PATCH_BIT1 mask */
#define MSDC_PB1_WRDAT_CRCS_TA_CNTR    (0x7 << 0)
#define MSDC_PB1_CMD_RSP_TA_CNTR      (0x7 << 3)
#define MSDC_PB1_RESV3		 (0x3 << 6)
#define MSDC_PB1_BIAS_TUNE_28NM  (0xf << 8)
#define MSDC_PB1_BIAS_EN18IO_28NM (0x1 << 12)
#define MSDC_PB1_BIAS_EXT_28NM	 (0x1 << 13)
#define MSDC_PB1_RESV2		 (0x3 << 14)
#define MSDC_PB1_RESET_GDMA	 (0x1 << 15)
#define MSDC_PB1_SINGLEBURST	 (0x1 << 16)
#define MSDC_PB1_FORCE_STOP	 (0x1 << 17)
#define MSDC_PB1_DCM_DEV_SEL2	 (0x3 << 18)
#define MSDC_PB1_DCM_DEV_SEL1	 (0x1 << 20)
#define MSDC_PB1_DCM_EN 	 (0x1 << 21)
#define MSDC_PB1_AXI_WRAP_CKEN	 (0x1 << 22)
#define MSDC_PB1_AHBCKEN	 (0x1 << 23)
#define MSDC_PB1_CKSPCEN	 (0x1 << 24)
#define MSDC_PB1_CKPSCEN	 (0x1 << 25)
#define MSDC_PB1_CKVOLDETEN	 (0x1 << 26)
#define MSDC_PB1_CKACMDEN	 (0x1 << 27)
#define MSDC_PB1_CKSDEN 	 (0x1 << 28)
#define MSDC_PB1_CKWCTLEN	 (0x1 << 29)
#define MSDC_PB1_CKRCTLEN	 (0x1 << 30)
#define MSDC_PB1_CKSHBFFEN	 (0x1UL << 31)

/* MSDC_PATCH_BIT2 mask */
#define MSDC_PB2_ENHANCEGPD	 (0x1 << 0)
#define MSDC_PB2_SUPPORT64G	 (0x1 << 1)
#define MSDC_PB2_RESPWAITCNT	 (0x3 << 2)
#define MSDC_PB2_CFGRDATCNT	 (0x1F << 4)
#define MSDC_PB2_CFGRDAT	 (0x1 << 9)
#define MSDC_PB2_INTCRESPSEL	 (0x1 << 11)
#define MSDC_PB2_CFGRESPCNT	 (0x7 << 12)
#define MSDC_PB2_CFGRESP	 (0x1 << 15)
#define MSDC_PB2_RESPSTENSEL	 (0x7 << 16)
#define MSDC_PB2_DDR50SEL	 (0x1 << 19)
#define MSDC_PB2_POPENCNT	 (0xF << 20)
#define MSDC_PB2_CFG_CRCSTS_SEL  (0x1 << 24)
#define MSDC_PB2_CFGCRCSTSEDGE	 (0x1 << 25)
#define MSDC_PB2_CFGCRCSTSCNT	 (0x3 << 26)
#define MSDC_PB2_CFGCRCSTS	 (0x1 << 28)
#define MSDC_PB2_CRCSTSENSEL	 (0x7UL << 29)

/* MSDC_HW_DBG_SEL mask */
#define MSDC_HW_DBG3_SEL	 (0xff << 0)
#define MSDC_HW_DBG2_SEL	 (0x3f << 8)
#define MSDC_HW_DBG1_SEL	 (0x3f << 16)
#define MSDC_HW_DBG_WRAPTYPE_SEL (0x3 << 22)
#define MSDC_HW_DBG0_SEL	 (0x3f << 24)
#define MSDC_HW_DBG_WRAP_SEL	 (0x1  << 30)

/* EMMC50_PAD_DS_TUNE mask */
#define MSDC_EMMC50_PAD_DS_TUNE_DLYSEL	(0x1 << 0)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY2SEL (0x1 << 1)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY1	(0x1f << 2)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY2	(0x1f << 7)
#define MSDC_EMMC50_PAD_DS_TUNE_DLY3	(0x1F << 12)

/* EMMC50_PAD_CMD_TUNE mask */
#define MSDC_EMMC50_PAD_CMD_TUNE_DLY3SEL (0x1 << 0)
#define MSDC_EMMC50_PAD_CMD_TUNE_RXDLY3  (0x1f << 1)
#define MSDC_EMMC50_PAD_CMD_TUNE_TXDLY	 (0x1f < 6)

/* EMMC50_PAD_DAT01_TUNE mask */
#define MSDC_EMMC50_PAD_DAT0_RXDLY3SEL	 (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT0_RXDLY3	 (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT0_TXDLY	 (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT1_RXDLY3SEL	 (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT1_RXDLY3	 (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT1_TXDLY	 (0x1f << 22)

/* EMMC50_PAD_DAT23_TUNE mask */
#define MSDC_EMMC50_PAD_DAT2_RXDLY3SEL	 (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT2_RXDLY3	 (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT2_TXDLY	 (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT3_RXDLY3SEL	 (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT3_RXDLY3	 (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT3_TXDLY	 (0x1f << 22)

/* EMMC50_PAD_DAT45_TUNE mask */
#define MSDC_EMMC50_PAD_DAT4_RXDLY3SEL	 (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT4_RXDLY3	 (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT4_TXDLY	 (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT5_RXDLY3SEL	 (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT5_RXDLY3	 (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT5_TXDLY	 (0x1f << 22)

/* EMMC50_PAD_DAT67_TUNE mask */
#define MSDC_EMMC50_PAD_DAT6_RXDLY3SEL	 (0x1 << 0)
#define MSDC_EMMC50_PAD_DAT6_RXDLY3	 (0x1f << 1)
#define MSDC_EMMC50_PAD_DAT6_TXDLY	 (0x1f << 6)
#define MSDC_EMMC50_PAD_DAT7_RXDLY3SEL	 (0x1 << 16)
#define MSDC_EMMC50_PAD_DAT7_RXDLY3	 (0x1f << 17)
#define MSDC_EMMC50_PAD_DAT7_TXDLY	 (0x1f << 22)

/* EMMC51_CFG0 mask */
#define MSDC_EMMC51_CFG0_CMDQ_EN	 (0x1 << 0)
#define MSDC_EMMC51_CFG0_WDAT_CNT	 (0x3ff << 1)
#define MSDC_EMMC51_CFG0_RDAT_CNT	 (0x3ff << 11)
#define MSDC_EMMC51_CFG0_CMDQ_CMD_EN	 (0x1 << 21)

/* EMMC50_CFG0 mask */
#define MSDC_EMMC50_CFG_PADCMD_LATCHCK	 (0x1 << 0)
#define MSDC_EMMC50_CFG_CRC_STS_CNT	     (0x3 << 1)
#define MSDC_EMMC50_CFG_CRC_STS_EDGE	 (0x1 << 3)
#define MSDC_EMMC50_CFG_CRC_STS_SEL	     (0x1 << 4)
#define MSDC_EMMC50_CFG_END_BIT_CHK_CNT  (0xf << 5)
#define MSDC_EMMC50_CFG_CMD_RESP_SEL	 (0x1 << 9)
#define MSDC_EMMC50_CFG_CMD_EDGE_SEL	 (0x1 << 10)
#define MSDC_EMMC50_CFG_ENDBIT_CNT	     (0x3ff << 11)
#define MSDC_EMMC50_CFG_READ_DAT_CNT	 (0x7 << 21)
#define MSDC_EMMC50_CFG_EMMC50_MON_SEL	 (0x1 << 24)
#define MSDC_EMMC50_CFG_MSDC_WR_VALID	 (0x1 << 25)
#define MSDC_EMMC50_CFG_MSDC_RD_VALID	 (0x1 << 26)
#define MSDC_EMMC50_CFG_MSDC_WR_VALID_SLE    (0x1 << 27)
#define MSDC_EMMC50_CFG_EMMC50_MON_SE	 (0x1 << 28)
#define MSDC_EMMC50_CFG_EMMC50_TXSKEWSEL (0x1 << 29)
#define MSDC_EMMC50_CFG_GDMA_RESET	     (0x1 << 31)

/* EMMC50_CFG1 mask */
#define MSDC_EMMC50_CFG1_WRPTR_MARGIN	 (0xff << 0)
#define MSDC_EMMC50_CFG1_CKSWITCH_CNT	 (0x7 << 8)
#define MSDC_EMMC50_CFG1_RDDAT_STOP	     (0x1 << 11)
#define MSDC_EMMC50_CFG1_WAITCLK_CNT	 (0xf << 12)
#define MSDC_EMMC50_CFG1_DBG_SEL	     (0xff << 16)
#define MSDC_EMMC50_CFG1_PSHCNT 	     (0x7 << 24)
#define MSDC_EMMC50_CFG1_PSHPSSEL	     (0x1 << 27)
#define MSDC_EMMC50_CFG1_DSCFG		     (0x1 << 28)
#define MSDC_EMMC50_CFG1_RESV0		     (0x7UL << 29)


/* EMMC50_CFG2_mask */
//#define MSDC_EMMC50_CFG2_AXI_GPD_UP	         (0x1 << 0)
#define MSDC_EMMC50_CFG2_AXI_IOMMU_WR_EMI      (0x1 << 1)
#define MSDC_EMMC50_CFG2_AXI_SHARE_EN_WR_EMI   (0x1 << 2)
#define MSDC_EMMC50_CFG2_AXI_IOMMU_RD_EMI      (0x1 << 7)
#define MSDC_EMMC50_CFG2_AXI_SHARE_EN_RD_EMI   (0x1 << 8)
#define MSDC_EMMC50_CFG2_AXI_BOUND_128B        (0x1 << 13)
#define MSDC_EMMC50_CFG2_AXI_BOUND_256B        (0x1 << 14)
#define MSDC_EMMC50_CFG2_AXI_BOUND_512B        (0x1 << 15)
#define MSDC_EMMC50_CFG2_AXI_BOUND_1K	       (0x1 << 16)
#define MSDC_EMMC50_CFG2_AXI_BOUND_2K	       (0x1 << 17)
#define MSDC_EMMC50_CFG2_AXI_BOUND_4K	       (0x1 << 18)
#define MSDC_EMMC50_CFG2_AXI_RD_OUTS_NUM       (0x1f << 19)
#define MSDC_EMMC50_CFG2_AXI_SET_LEN	       (0xf << 24)
#define MSDC_EMMC50_CFG2_AXI_RESP_ERR_TYPE     (0x3 << 28)
#define MSDC_EMMC50_CFG2_AXI_BUSY	           (0x1 << 30)


/* EMMC50_CFG3_mask */
#define MSDC_EMMC50_CFG3_OUTS_WR	           (0x1f << 0)
#define MSDC_EMMC50_CFG3_ULTRA_SET_WR	       (0x3f << 5)
#define MSDC_EMMC50_CFG3_PREULTRA_SET_WR       (0x3f << 11)
#define MSDC_EMMC50_CFG3_ULTRA_SET_RD	       (0x3f << 17)
#define MSDC_EMMC50_CFG3_PREULTRA_SET_RD       (0x3f <<23)

/* EMMC50_CFG4_mask */
#define MSDC_EMMC50_CFG4_IMPR_ULTRA_SET_WR     (0xff << 0)
#define MSDC_EMMC50_CFG4_IMPR_ULTRA_SET_RD     (0xff < 8)
#define MSDC_EMMC50_CFG4_ULTRA_EN	           (0x3 << 16)

/* SDIO_TUNE_WIND mask*/
#define MSDC_SDIO_TUNE_WIND	 (0x1f << 0)

/*
* MSDC pad control at top layer
*/
/*--------------------------------------------------------------------------*/
/* MSDC0~3 GPIO and IO Pad Configuration Base                               */
/*--------------------------------------------------------------------------*/
#define GPIO_base               0x10005000
#ifndef GPIO_BASE
#define GPIO_BASE               GPIO_base
#endif
#define MSDC_GPIO_BASE		GPIO_BASE
#define IO_CFG_B_BASE           (0x10002400)    /* IO Config b base   */
#define io_cfg_r_base           (0x10002800)    /* IO Config r base   */
#define MSDC0_IO_PAD_BASE       (IO_CFG_B_BASE)
#define MSDC1_IO_PAD_BASE       (IO_CFG_B_BASE)
/*--------------------------------------------------------------------------*/
/* MSDC GPIO Related Register                                               */
/*--------------------------------------------------------------------------*/
#define MSDC0_GPIO_MODE14	(MSDC_GPIO_BASE   +  0x3e0)
#define MSDC0_GPIO_MODE15	(MSDC_GPIO_BASE   +  0x3f0)
#define MSDC1_GPIO_MODE16	(MSDC_GPIO_BASE   +  0x400)

#define MSDC0_GPIO_SMT_ADDR	(MSDC0_IO_PAD_BASE + 0x060)
#define MSDC0_GPIO_IES_ADDR	(MSDC0_IO_PAD_BASE + 0x020)
#define MSDC0_GPIO_PUPD_ADDR	(MSDC0_IO_PAD_BASE + 0x100)
#define MSDC0_GPIO_R0_ADDR	(MSDC0_IO_PAD_BASE + 0x110)
#define MSDC0_GPIO_R1_ADDR	(MSDC0_IO_PAD_BASE + 0x120)
#define MSDC0_GPIO_SR_ADDR	(MSDC0_IO_PAD_BASE + 0x030)
#define MSDC0_GPIO_TDSEL_ADDR	(MSDC0_IO_PAD_BASE + 0x0d0)
#define MSDC0_GPIO_RDSEL_ADDR	(MSDC0_IO_PAD_BASE + 0x080)
#define MSDC0_GPIO_DRV_ADDR	(MSDC0_IO_PAD_BASE + 0x1a0)
/* msdc1 smt is in IO_CFG_R register map */
#define MSDC1_GPIO_SMT_ADDR	(io_cfg_r_base     + 0x030)
#define MSDC1_GPIO_IES_ADDR	(MSDC1_IO_PAD_BASE + 0x020)
#define MSDC1_GPIO_PUPD_ADDR	(MSDC1_IO_PAD_BASE + 0x100)
#define MSDC1_GPIO_R0_ADDR	(MSDC1_IO_PAD_BASE + 0x110)
#define MSDC1_GPIO_R1_ADDR	(MSDC1_IO_PAD_BASE + 0x120)
#define MSDC1_GPIO_SR_ADDR	(MSDC1_IO_PAD_BASE + 0x030)
#define MSDC1_GPIO_TDSEL_ADDR	(MSDC1_IO_PAD_BASE + 0x0c0)
#define MSDC1_GPIO_RDSEL_ADDR	(MSDC1_IO_PAD_BASE + 0x0a0)
#define MSDC1_GPIO_DRV_ADDR	(MSDC1_IO_PAD_BASE + 0x1b0)
/*--------------------------------------------------------------------------*/
/* MSDC GPIO Related Register Mask                                               */
/*--------------------------------------------------------------------------*/
/* MSDC0_GPIO_MODE14, 001b is msdc mode*/
#define MSDC0_MODE_DAT5_MASK            (7 << 28)
#define MSDC0_MODE_DAT4_MASK            (7 << 24)
#define MSDC0_MODE_DAT3_MASK            (7 << 20)
#define MSDC0_MODE_DAT2_MASK            (7 << 16)
#define MSDC0_MODE_DAT1_MASK            (7 << 12)
#define MSDC0_MODE_DAT0_MASK            (7 << 8)
/* MSDC0_GPIO_MODE15, 001b is msdc mode */
#define MSDC0_MODE_RSTB_MASK            (7 << 20)
#define MSDC0_MODE_DSL_MASK             (7 << 16)
#define MSDC0_MODE_CLK_MASK             (7 << 12)
#define MSDC0_MODE_CMD_MASK             (7 << 8)
#define MSDC0_MODE_DAT7_MASK            (7 << 4)
#define MSDC0_MODE_DAT6_MASK            (7 << 0)
/* MSDC1_GPIO_MODE16, 0001b is msdc mode */
#define MSDC1_MODE_CMD_MASK             (0x7 << 4)
#define MSDC1_MODE_DAT0_MASK            (0x7 << 8)
#define MSDC1_MODE_DAT1_MASK            (0x7 << 12)
#define MSDC1_MODE_DAT2_MASK            (0x7 << 16)
#define MSDC1_MODE_DAT3_MASK            (0x7 << 20)
#define MSDC1_MODE_CLK_MASK             (0x7 << 24)

/* MSDC0 SMT mask*/
#define MSDC0_SMT_DAT3_0_MASK            (0x1  <<  0)
#define MSDC0_SMT_DAT7_4_MASK            (0x1  <<  1)
#define MSDC0_SMT_CMD_DSL_RSTB_MASK      (0x1  <<  2)
#define MSDC0_SMT_CLK_MASK               (0x1  <<  3)
#define MSDC0_SMT_ALL_MASK               (0xf <<  0)
/* MSDC1 SMT mask. is in IO_CFG_R register map*/
#define MSDC1_SMT_CMD_MASK             (0x1 << 0)
#define MSDC1_SMT_DAT_MASK             (0x1 << 1)
#define MSDC1_SMT_CLK_MASK             (0x1 << 2)
#define MSDC1_SMT_ALL_MASK             (0x7 << 0)

/* MSDC0 IES mask*/
#define MSDC0_IES_DAT0_MASK              (0x1  <<  0)
#define MSDC0_IES_DAT1_MASK              (0x1  <<  1)
#define MSDC0_IES_DAT2_MASK              (0x1  <<  2)
#define MSDC0_IES_DAT3_MASK              (0x1  <<  3)
#define MSDC0_IES_DAT4_MASK              (0x1  <<  4)
#define MSDC0_IES_DAT5_MASK              (0x1  <<  5)
#define MSDC0_IES_DAT6_MASK              (0x1  <<  6)
#define MSDC0_IES_DAT7_MASK              (0x1  <<  7)
#define MSDC0_IES_CMD_MASK               (0x1  <<  8)
#define MSDC0_IES_CLK_MASK               (0x1  <<  9)
#define MSDC0_IES_DSL_MASK               (0x1  <<  10)
#define MSDC0_IES_RSTB_MASK              (0x1  <<  11)
#define MSDC0_IES_ALL_MASK               (0xfff << 0)
/* MSDC1 IES mask*/
#define MSDC1_IES_CMD_MASK             (0x1 << 25)
#define MSDC1_IES_DAT0_MASK            (0x1 << 26)
#define MSDC1_IES_DAT1_MASK            (0x1 << 27)
#define MSDC1_IES_DAT2_MASK            (0x1 << 28)
#define MSDC1_IES_DAT3_MASK            (0x1 << 29)
#define MSDC1_IES_CLK_MASK             (0x1 << 30)
#define MSDC1_IES_ALL_MASK             (0x3f << 25)

/* MSDC0 PUPD mask*/
#define MSDC0_PUPD_DAT0_MASK            (0x1  << 6)
#define MSDC0_PUPD_DAT1_MASK            (0x1  << 7)
#define MSDC0_PUPD_DAT2_MASK            (0x1  << 8)
#define MSDC0_PUPD_DAT3_MASK            (0x1  << 9)
#define MSDC0_PUPD_DAT4_MASK            (0x1  << 10)
#define MSDC0_PUPD_DAT5_MASK            (0x1  << 11)
#define MSDC0_PUPD_DAT6_MASK            (0x1  << 12)
#define MSDC0_PUPD_DAT7_MASK            (0x1  << 13)
#define MSDC0_PUPD_CMD_MASK             (0x1  << 14)
#define MSDC0_PUPD_CLK_MASK             (0x1  << 15)
#define MSDC0_PUPD_DSL_MASK             (0x1  << 16)
#define MSDC0_PUPD_RSTB_MASK            (0x1  << 17)
#define MSDC0_PUPD_DAT_MASK             (0xff << 6)
#define MSDC0_PUPD_CMD_DAT_MASK         (0x1FF << 6)
#define MSDC0_PUPD_CLK_DSL_MASK         (0x3  << 15)
#define MSDC0_PUPD_ALL_MASK             (0xfff << 6)
/* MSDC0 R0 mask*/
#define MSDC0_R0_DAT0_MASK            (0x1  << 6)
#define MSDC0_R0_DAT1_MASK            (0x1  << 7)
#define MSDC0_R0_DAT2_MASK            (0x1  << 8)
#define MSDC0_R0_DAT3_MASK            (0x1  << 9)
#define MSDC0_R0_DAT4_MASK            (0x1  << 10)
#define MSDC0_R0_DAT5_MASK            (0x1  << 11)
#define MSDC0_R0_DAT6_MASK            (0x1  << 12)
#define MSDC0_R0_DAT7_MASK            (0x1  << 13)
#define MSDC0_R0_CMD_MASK             (0x1  << 14)
#define MSDC0_R0_CLK_MASK             (0x1  << 15)
#define MSDC0_R0_DSL_MASK             (0x1  << 16)
#define MSDC0_R0_RSTB_MASK            (0x1  << 17)
#define MSDC0_R0_DAT_MASK             (0xff << 6)
#define MSDC0_R0_CMD_DAT_MASK         (0x1FF << 6)
#define MSDC0_R0_CLK_DSL_MASK         (0x3  << 15)
#define MSDC0_R0_ALL_MASK             (0xfff << 6)
/* MSDC0 R1 mask*/
#define MSDC0_R1_DAT0_MASK            (0x1  << 6)
#define MSDC0_R1_DAT1_MASK            (0x1  << 7)
#define MSDC0_R1_DAT2_MASK            (0x1  << 8)
#define MSDC0_R1_DAT3_MASK            (0x1  << 9)
#define MSDC0_R1_DAT4_MASK            (0x1  << 10)
#define MSDC0_R1_DAT5_MASK            (0x1  << 11)
#define MSDC0_R1_DAT6_MASK            (0x1  << 12)
#define MSDC0_R1_DAT7_MASK            (0x1  << 13)
#define MSDC0_R1_CMD_MASK             (0x1  << 14)
#define MSDC0_R1_CLK_MASK             (0x1  << 15)
#define MSDC0_R1_DSL_MASK             (0x1  << 16)
#define MSDC0_R1_RSTB_MASK            (0x1  << 17)
#define MSDC0_R1_DAT_MASK             (0xff << 6)
#define MSDC0_R1_CMD_DAT_MASK         (0x1FF << 6)
#define MSDC0_R1_CLK_DSL_MASK         (0x3  << 15)
#define MSDC0_R1_ALL_MASK             (0xfff << 6)
/* MSDC1 PUPD mask*/
#define MSDC1_PUPD_CMD_MASK             (0x1  << 18)
#define MSDC1_PUPD_DAT0_MASK            (0x1  << 19)
#define MSDC1_PUPD_DAT1_MASK            (0x1  << 20)
#define MSDC1_PUPD_DAT2_MASK            (0x1  << 21)
#define MSDC1_PUPD_DAT3_MASK            (0x1  << 22)
#define MSDC1_PUPD_CMD_DAT_MASK         (0x1f << 18)
#define MSDC1_PUPD_CLK_MASK             (0x1  << 23)
#define MSDC1_PUPD_ALL_MASK             (0x3f << 18)
/* MSDC1 R0 mask*/
#define MSDC1_R0_CMD_MASK             (0x1  << 18)
#define MSDC1_R0_DAT0_MASK            (0x1  << 19)
#define MSDC1_R0_DAT1_MASK            (0x1  << 20)
#define MSDC1_R0_DAT2_MASK            (0x1  << 21)
#define MSDC1_R0_DAT3_MASK            (0x1  << 22)
#define MSDC1_R0_CMD_DAT_MASK         (0x1f << 18)
#define MSDC1_R0_CLK_MASK             (0x1  << 23)
#define MSDC1_R0_ALL_MASK             (0x3f << 18)
/* MSDC1 R1 mask*/
#define MSDC1_R1_CMD_MASK             (0x1  << 18)
#define MSDC1_R1_DAT0_MASK            (0x1  << 19)
#define MSDC1_R1_DAT1_MASK            (0x1  << 20)
#define MSDC1_R1_DAT2_MASK            (0x1  << 21)
#define MSDC1_R1_DAT3_MASK            (0x1  << 22)
#define MSDC1_R1_CMD_DAT_MASK         (0x1f << 18)
#define MSDC1_R1_CLK_MASK             (0x1  << 23)
#define MSDC1_R1_ALL_MASK             (0x3f << 18)

/* MSDC0 SR mask*/
#define MSDC0_SR_DAT3_0_MASK            (0x1  << 15)
#define MSDC0_SR_DAT7_4_MASK            (0x1  << 16)
#define MSDC0_SR_CMD_DSL_RSTB_MASK      (0x1  << 17)
#define MSDC0_SR_CLK_MASK               (0x1  << 18)
#define MSDC0_SR_ALL_MASK               (0xf  << 15)
/* MSDC1 SR mask*/
#define MSDC1_SR_CMD_MASK               (0x1 << 29)
#define MSDC1_SR_DAT_MASK               (0x1 << 30)
#define MSDC1_SR_CLK_MASK               (0x1 << 31)

/* MSDC0 TDSEL mask*/
#define MSDC0_TDSEL_DAT3_0_MASK         (0xf  <<  0)
#define MSDC0_TDSEL_DAT7_4_MASK         (0xf  <<  4)
#define MSDC0_TDSEL_CMD_DSL_RSTB_MASK   (0xf  <<  8)
#define MSDC0_TDSEL_CLK_MASK            (0xf  <<  12)
#define MSDC0_TDSEL_ALL_MASK            (0xffff << 0)
/* MSDC1 TDSEL mask*/
#define MSDC1_TDSEL_CMD_MASK            (0xf << 16)
#define MSDC1_TDSEL_DAT_MASK            (0xf << 20)
#define MSDC1_TDSEL_CLK_MASK            (0xf << 24)
#define MSDC1_TDSEL_ALL_MASK            (0xfff << 16)

/* MSDC0 RDSEL mask*/
#define MSDC0_RDSEL_DAT3_0_MASK         (0x3f <<  0)
#define MSDC0_RDSEL_DAT7_4_MASK         (0x3f <<  6)
#define MSDC0_RDSEL_CLK_CMD_DSL_RSTB_MASK  (0x3f <<  12)
#define MSDC0_RDSEL_ALL_MASK            (0x3ffff << 0)
/* MSDC1 RDSEL mask*/
#define MSDC1_RDSEL_CMD_MASK            (0x3f << 8)
#define MSDC1_RDSEL_DAT_MASK            (0x3f << 14)
#define MSDC1_RDSEL_CLK_MASK            (0x3f << 20)
#define MSDC1_RDSEL_ALL_MASK            (0x3ffff << 8)

/* MSDC0 DRV mask*/
#define MSDC0_DRV_DAT3_0_MASK           (0x7  <<  0)
#define MSDC0_DRV_DAT7_4_MASK           (0x7  <<  3)
#define MSDC0_DRV_DAT_MASK              (0x3f <<  0)
#define MSDC0_DRV_CMD_DSL_RSTB_MASK     (0x7  <<  6)
#define MSDC0_DRV_CLK_MASK              (0x7  <<  9)
#define MSDC0_DRV_ALL_MASK              (0xfff << 0)
/* MSDC1 DRV mask*/
#define MSDC1_DRV_CMD_MASK            (0x7 << 21)
#define MSDC1_DRV_DAT_MASK            (0x7 << 24)
#define MSDC1_DRV_CLK_MASK            (0x7 << 27)
#define MSDC1_DRV_ALL_MASK            (0x1ff << 21)

#ifndef CFG_FPGA_PLATFORM
void msdc_set_driving_by_id(u32	id, struct msdc_cust *hw, bool sd_18);
void msdc_set_smt_by_id(u32 id, int set_smt);
void msdc_set_tdsel_by_id(u32 id, bool sleep, bool sd_18);
void msdc_set_rdsel_by_id(u32 id, bool sleep, bool sd_18);
void msdc_dump_padctl_by_id(u32 id);
void msdc_pin_config_by_id(u32 id, u32 mode);
void msdc_set_pin_mode(struct mmc_host *host);
#else
#define msdc_set_driving_by_id(id, hw, sd_18)
#define msdc_set_smt_by_id(id, set_smt)
#define msdc_set_tdsel_by_id(id, sleep, sd_18)
#define msdc_set_rdsel_by_id(id, sleep, sd_18)
#define msdc_pin_config_by_id(id, mode)
#define msdc_set_pin_mode(host)
#endif

void msdc_gpio_and_pad_init(struct mmc_host *host);
#define msdc_set_driving(host, hw, sd_18) \
	msdc_set_driving_by_id(host->id, hw, sd_18)
#define msdc_set_smt(host, set_smt) \
	msdc_set_smt_by_id(host->id, set_smt)
#define msdc_set_tdsel(host, sleep, sd_18) \
	msdc_set_tdsel_by_id(host->id, sleep, sd_18)
#define msdc_set_rdsel(host, sleep, sd_18) \
	msdc_set_rdsel_by_id(host->id, sleep, sd_18)
#define msdc_dump_padctl(host) \
	msdc_dump_padctl_by_id(host->id)
#define msdc_pin_config(host, mode) \
	msdc_pin_config_by_id(host->id, mode)
/*--------------------------------------------------------------------------*/
/* Descriptor Structure 						    */
/*--------------------------------------------------------------------------*/
typedef struct {
   uint32  hwo:1; /* could be changed by hw */
   uint32  bdp:1;
   uint32  rsv0:6;
   uint32  chksum:8;
   uint32  intr:1;
   uint32  rsv1:7;
   uint32	nexth4:4;
   uint32	ptrh4:4;
   void   *next;
   void   *ptr;
   uint32  buflen:16;
   uint32  extlen:8;
   uint32  rsv2:8;
   uint32  arg;
   uint32  blknum;
   uint32  cmd;
} gpd_t;

typedef struct {
   uint32  eol:1;
   uint32  rsv0:7;
   uint32  chksum:8;
   uint32  rsv1:1;
   uint32  blkpad:1;
   uint32  dwpad:1;
   uint32  rsv2:5;
   uint32	nexth4:4;
   uint32	ptrh4:4;
   void   *next;
   void   *ptr;
   uint32  buflen:16;
   uint32  rsv3:16;
} bd_t;

/*--------------------------------------------------------------------------*/
/* Register Debugging Structure 					    */
/*--------------------------------------------------------------------------*/

typedef struct {
   uint32 msdc:1;
   uint32 ckpwn:1;
   uint32 rst:1;
   uint32 pio:1;
   uint32 ckdrven:1;
   uint32 start18v:1;
   uint32 pass18v:1;
   uint32 ckstb:1;
   uint32 ckdiv:8;
   uint32 ckmod:2;
   uint32 pad:14;
} msdc_cfg_reg;
typedef struct {
   uint32 sdr104cksel:1;
   uint32 rsmpl:1;
   uint32 dsmpl:1;
   uint32 ddlysel:1;
   uint32 ddr50ckd:1;
   uint32 dsplsel:1;
   uint32 pad1:10;
   uint32 d0spl:1;
   uint32 d1spl:1;
   uint32 d2spl:1;
   uint32 d3spl:1;
   uint32 d4spl:1;
   uint32 d5spl:1;
   uint32 d6spl:1;
   uint32 d7spl:1;
   uint32 riscsz:1;
   uint32 pad2:7;
} msdc_iocon_reg;
typedef struct {
   uint32 cden:1;
   uint32 cdsts:1;
   uint32 pad1:10;
   uint32 cddebounce:4;
   uint32 dat:8;
   uint32 cmd:1;
   uint32 pad2:6;
   uint32 wp:1;
} msdc_ps_reg;
typedef struct {
   uint32 mmcirq:1;
   uint32 cdsc:1;
   uint32 pad1:1;
   uint32 atocmdrdy:1;
   uint32 atocmdtmo:1;
   uint32 atocmdcrc:1;
   uint32 dmaqempty:1;
   uint32 sdioirq:1;
   uint32 cmdrdy:1;
   uint32 cmdtmo:1;
   uint32 rspcrc:1;
   uint32 csta:1;
   uint32 xfercomp:1;
   uint32 dxferdone:1;
   uint32 dattmo:1;
   uint32 datcrc:1;
   uint32 atocmd19done:1;
   uint32 pad2:15;
} msdc_int_reg;
typedef struct {
   uint32 mmcirq:1;
   uint32 cdsc:1;
   uint32 pad1:1;
   uint32 atocmdrdy:1;
   uint32 atocmdtmo:1;
   uint32 atocmdcrc:1;
   uint32 dmaqempty:1;
   uint32 sdioirq:1;
   uint32 cmdrdy:1;
   uint32 cmdtmo:1;
   uint32 rspcrc:1;
   uint32 csta:1;
   uint32 xfercomp:1;
   uint32 dxferdone:1;
   uint32 dattmo:1;
   uint32 datcrc:1;
   uint32 atocmd19done:1;
   uint32 pad2:15;
} msdc_inten_reg;
typedef struct {
   uint32 rxcnt:8;
   uint32 pad1:8;
   uint32 txcnt:8;
   uint32 pad2:7;
   uint32 clr:1;
} msdc_fifocs_reg;
typedef struct {
   uint32 val;
} msdc_txdat_reg;
typedef struct {
   uint32 val;
} msdc_rxdat_reg;
typedef struct {
   uint32 sdiowkup:1;
   uint32 inswkup:1;
   uint32 pad1:14;
   uint32 buswidth:2;
   uint32 pad2:1;
   uint32 sdio:1;
   uint32 sdioide:1;
   uint32 intblkgap:1;
   uint32 pad4:2;
   uint32 dtoc:8;
} sdc_cfg_reg;
typedef struct {
   uint32 cmd:6;
   uint32 brk:1;
   uint32 rsptyp:3;
   uint32 pad1:1;
   uint32 dtype:2;
   uint32 rw:1;
   uint32 stop:1;
   uint32 goirq:1;
   uint32 blklen:12;
   uint32 atocmd:2;
   uint32 volswth:1;
   uint32 pad2:1;
} sdc_cmd_reg;
typedef struct {
   uint32 arg;
} sdc_arg_reg;
typedef struct {
   uint32 sdcbusy:1;
   uint32 cmdbusy:1;
   uint32 pad:29;
   uint32 swrcmpl:1;
} sdc_sts_reg;
typedef struct {
   uint32 val;
} sdc_resp0_reg;
typedef struct {
   uint32 val;
} sdc_resp1_reg;
typedef struct {
   uint32 val;
} sdc_resp2_reg;
typedef struct {
   uint32 val;
} sdc_resp3_reg;
typedef struct {
   uint32 num;
} sdc_blknum_reg;
typedef struct {
   uint32 sts;
} sdc_csts_reg;
typedef struct {
   uint32 sts;
} sdc_cstsen_reg;
typedef struct {
   uint32 datcrcsts:8;
   uint32 ddrcrcsts:4;
   uint32 pad:20;
} sdc_datcrcsts_reg;
typedef struct {
   uint32 bootstart:1;
   uint32 bootstop:1;
   uint32 bootmode:1;
   uint32 pad1:9;
   uint32 bootwaidly:3;
   uint32 bootsupp:1;
   uint32 pad2:16;
} emmc_cfg0_reg;
typedef struct {
   uint32 bootcrctmc:16;
   uint32 pad:4;
   uint32 bootacktmc:12;
} emmc_cfg1_reg;
typedef struct {
   uint32 bootcrcerr:1;
   uint32 bootackerr:1;
   uint32 bootdattmo:1;
   uint32 bootacktmo:1;
   uint32 bootupstate:1;
   uint32 bootackrcv:1;
   uint32 bootdatrcv:1;
   uint32 pad:25;
} emmc_sts_reg;
typedef struct {
   uint32 bootrst:1;
   uint32 pad:31;
} emmc_iocon_reg;
typedef struct {
   uint32 val;
} msdc_acmd_resp_reg;
typedef struct {
   uint32 tunesel:4;
   uint32 pad:28;
} msdc_acmd19_trg_reg;
typedef struct {
   uint32 val;
} msdc_acmd19_sts_reg;
typedef struct {
   uint32 addr;
} msdc_dma_sa_reg;
typedef struct {
   uint32 addr;
} msdc_dma_ca_reg;
typedef struct {
   uint32 start:1;
   uint32 stop:1;
   uint32 resume:1;
   uint32 pad1:5;
   uint32 mode:1;
   uint32 pad2:1;
   uint32 lastbuf:1;
   uint32 pad3:1;
   uint32 brustsz:3;
   uint32 pad4:1;
   uint32 xfersz:16;
} msdc_dma_ctrl_reg;
typedef struct {
   uint32 status:1;
   uint32 decsen:1;
   uint32 pad1:2;
   uint32 bdcsen:1;
   uint32 gpdcsen:1;
   uint32 pad2:26;
} msdc_dma_cfg_reg;
typedef struct {
   uint32 sel:16;
   uint32 pad2:16;
} msdc_dbg_sel_reg;
typedef struct {
   uint32 val;
} msdc_dbg_out_reg;
typedef struct {
   uint32 clkdrvn:3;
   uint32 rsv0:1;
   uint32 clkdrvp:3;
   uint32 rsv1:1;
   uint32 clksr:1;
   uint32 rsv2:7;
   uint32 clkpd:1;
   uint32 clkpu:1;
   uint32 clksmt:1;
   uint32 clkies:1;
   uint32 clktdsel:4;
   uint32 clkrdsel:8;
} msdc_pad_ctl0_reg;
typedef struct {
   uint32 cmddrvn:3;
   uint32 rsv0:1;
   uint32 cmddrvp:3;
   uint32 rsv1:1;
   uint32 cmdsr:1;
   uint32 rsv2:7;
   uint32 cmdpd:1;
   uint32 cmdpu:1;
   uint32 cmdsmt:1;
   uint32 cmdies:1;
   uint32 cmdtdsel:4;
   uint32 cmdrdsel:8;
} msdc_pad_ctl1_reg;
typedef struct {
   uint32 datdrvn:3;
   uint32 rsv0:1;
   uint32 datdrvp:3;
   uint32 rsv1:1;
   uint32 datsr:1;
   uint32 rsv2:7;
   uint32 datpd:1;
   uint32 datpu:1;
   uint32 datsmt:1;
   uint32 daties:1;
   uint32 dattdsel:4;
   uint32 datrdsel:8;
} msdc_pad_ctl2_reg;
typedef struct {
   uint32 datwrdly:5;
   uint32 pad1:3;
   uint32 datrddly:5;
   uint32 pad2:3;
   uint32 cmdrxdly:5;
   uint32 pad3:1;
   uint32 cmdrsprxdly:5;
   uint32 clktxdly:5;
} msdc_pad_tune_reg;
typedef struct {
   uint32 dat0:5;
   uint32 rsv0:3;
   uint32 dat1:5;
   uint32 rsv1:3;
   uint32 dat2:5;
   uint32 rsv2:3;
   uint32 dat3:5;
   uint32 rsv3:3;
} msdc_dat_rddly0;
typedef struct {
   uint32 dat4:5;
   uint32 rsv4:3;
   uint32 dat5:5;
   uint32 rsv5:3;
   uint32 dat6:5;
   uint32 rsv6:3;
   uint32 dat7:5;
   uint32 rsv7:3;
} msdc_dat_rddly1;
typedef struct {
   uint32 dbg0sel:8;
   uint32 dbg1sel:6;
   uint32 pad1:2;
   uint32 dbg2sel:6;
   uint32 pad2:2;
   uint32 dbg3sel:6;
   uint32 pad3:2;
} msdc_hw_dbg_reg;
typedef struct {
   uint32 val;
} msdc_version_reg;
typedef struct {
   uint32 val;
} msdc_eco_ver_reg;

struct msdc_regs {
   msdc_cfg_reg	msdc_cfg;      /* base+0x00h */
   msdc_iocon_reg	msdc_iocon;    /* base+0x04h */
   msdc_ps_reg 	msdc_ps;       /* base+0x08h */
   msdc_int_reg	msdc_int;      /* base+0x0ch */
   msdc_inten_reg	msdc_inten;    /* base+0x10h */
   msdc_fifocs_reg	msdc_fifocs;   /* base+0x14h */
   msdc_txdat_reg	msdc_txdat;    /* base+0x18h */
   msdc_rxdat_reg	msdc_rxdat;    /* base+0x1ch */
   uint32		rsv1[4];
   sdc_cfg_reg 	sdc_cfg;       /* base+0x30h */
   sdc_cmd_reg 	sdc_cmd;       /* base+0x34h */
   sdc_arg_reg 	sdc_arg;       /* base+0x38h */
   sdc_sts_reg 	sdc_sts;       /* base+0x3ch */
   sdc_resp0_reg	sdc_resp0;     /* base+0x40h */
   sdc_resp1_reg	sdc_resp1;     /* base+0x44h */
   sdc_resp2_reg	sdc_resp2;     /* base+0x48h */
   sdc_resp3_reg	sdc_resp3;     /* base+0x4ch */
   sdc_blknum_reg	sdc_blknum;    /* base+0x50h */
   uint32		rsv2[1];
   sdc_csts_reg	sdc_csts;      /* base+0x58h */
   sdc_cstsen_reg	sdc_cstsen;    /* base+0x5ch */
   sdc_datcrcsts_reg	sdc_dcrcsta;   /* base+0x60h */
   uint32		rsv3[3];
   emmc_cfg0_reg	emmc_cfg0;     /* base+0x70h */
   emmc_cfg1_reg	emmc_cfg1;     /* base+0x74h */
   emmc_sts_reg	emmc_sts;      /* base+0x78h */
   emmc_iocon_reg	emmc_iocon;    /* base+0x7ch */
   msdc_acmd_resp_reg	acmd_resp;     /* base+0x80h */
   msdc_acmd19_trg_reg acmd19_trg;    /* base+0x84h */
   msdc_acmd19_sts_reg acmd19_sts;    /* base+0x88h */
   uint32		rsv4[1];
   msdc_dma_sa_reg	dma_sa;        /* base+0x90h */
   msdc_dma_ca_reg	dma_ca;        /* base+0x94h */
   msdc_dma_ctrl_reg	dma_ctrl;      /* base+0x98h */
   msdc_dma_cfg_reg	dma_cfg;       /* base+0x9ch */
   msdc_dbg_sel_reg	dbg_sel;       /* base+0xa0h */
   msdc_dbg_out_reg	dbg_out;       /* base+0xa4h */
   uint32		rsv5[2];
   uint32		patch0;        /* base+0xb0h */
   uint32		patch1;        /* base+0xb4h */
   uint32		rsv6[10];
   msdc_pad_ctl0_reg	pad_ctl0;      /* base+0xe0h */
   msdc_pad_ctl1_reg	pad_ctl1;      /* base+0xe4h */
   msdc_pad_ctl2_reg	pad_ctl2;      /* base+0xe8h */
   msdc_pad_tune_reg	pad_tune;      /* base+0xech */
   msdc_dat_rddly0	dat_rddly0;    /* base+0xf0h */
   msdc_dat_rddly1	dat_rddly1;    /* base+0xf4h */
   msdc_hw_dbg_reg	hw_dbg;        /* base+0xf8h */
   uint32		rsv7[1];
   msdc_version_reg	version;       /* base+0x100h */
   msdc_eco_ver_reg	eco_ver;       /* base+0x104h */
};

struct scatterlist {
   u32 addr;
   u32 len;
};

struct scatterlist_ex {
   u32 cmd;
   u32 arg;
   u32 sglen;
   struct scatterlist *sg;
};

#define DMA_FLAG_NONE	    (0x00000000)
#define DMA_FLAG_EN_CHKSUM  (0x00000001)
#define DMA_FLAG_PAD_BLOCK  (0x00000002)
#define DMA_FLAG_PAD_DWORD  (0x00000004)

struct dma_config {
   u32 flags;			 /* flags */
   u32 xfersz; 		 /* xfer size in bytes */
   u32 sglen;			 /* size of scatter list */
   u32 blklen; 		 /* block size */
   struct scatterlist *sg;	 /* I/O scatter list */
   struct scatterlist_ex *esg;  /* extended I/O scatter list */
   u8	mode;			 /* dma mode	    */
   u8	burstsz;		 /* burst size	    */
   u8	intr;			 /* dma done interrupt */
   u8	padding;		 /* padding */
   u32 cmd;			 /* enhanced mode command */
   u32 arg;			 /* enhanced mode arg */
   u32 rsp;			 /* enhanced mode command response */
   u32 autorsp;		 /* auto command response */
};

#if MSDC_USE_REG_OPS_DUMP
static void reg32_write(volatile uint32 *addr, uint32 data)
{
   *addr = (uint32)data;
   printf("[WR32] %x = %x\n", addr, data);
}

static uint32 reg32_read(volatile uint32 *addr)
{
   uint32 data = *(volatile uint32*)(addr);
   printf("[RD32] %x = %x\n", addr, data);
   return data;
}

static void reg16_write(volatile uint32 *addr, uint16 data)
{
   *(volatile uint16*)(addr) = data;
   printf("[WR16] %x = %x\n", addr, data);
}

static uint16 reg16_read(volatile uint32 *addr)
{
   uint16 data = *(volatile uint16*)addr;
   printf("[RD16] %x = %x\n", addr, data);
   return data;
}

static void reg8_write(volatile uint32 *addr, uint8 data)
{
   *(volatile uint8*)(addr) = data;
   printf("[WR8] %x = %x\n", addr, data);
}

static uint8 reg8_read(volatile uint32 *addr)
{
   uint8 data = *(volatile uint8*)addr;
   printf("[RD8] %x = %x\n", addr, data);
   return data;
}

#define MSDC_WRITE32(addr,data)     reg32_write((volatile uint32*)addr, data)
#define MSDC_READ32(addr)	    reg32_read((volatile uint32*)addr)
#define MSDC_WRITE16(addr,data)     reg16_write((volatile uint32*)addr, data)
#define MSDC_READ16(addr)	    reg16_read((volatile uint32*)addr)
#define MSDC_WRITE8(addr, data)     reg8_write((volatile uint32*)addr, data)
#define MSDC_READ8(addr)	    reg8_read((volatile uint32*)addr)
#define MSDC_SET_BIT32(addr,mask)   \
   do { \
   (*(volatile uint32*)(addr) |= (mask)); \
   printf("[SET32] %x |= %x\n", addr, mask); \
   }while(0)
#define MSDC_CLR_BIT32(addr,mask)   \
   do { \
   (*(volatile uint32*)(addr) &= ~(mask)); \
   printf("[CLR32] %x &= ~%x\n", addr, mask); \
   }while(0)
#define MSDC_SET_BIT16(addr,mask)   \
   do { \
   (*(volatile uint16*)(addr) |= (mask)); \
   printf("[SET16] %x |= %x\n", addr, mask); \
   }while(0)
#define MSDC_CLR_BIT16(addr,mask)   \
   do { \
   (*(volatile uint16*)(addr) &= ~(mask)); \
   printf("[CLR16] %x &= ~%x\n", addr, mask); \
   }while(0)
#else
#define MSDC_WRITE32(addr,data)     (*(volatile uint32*)(addr) = (uint32)(data))
#define MSDC_READ32(addr)	    (*(volatile uint32*)(addr))
#define MSDC_WRITE16(addr,data)     (*(volatile uint16*)(addr) = (uint16)(data))
#define MSDC_READ16(addr)	    (*(volatile uint16*)(addr))
#define MSDC_WRITE8(addr, data)     (*(volatile uint8*)(addr)  = (uint8)(data))
#define MSDC_READ8(addr)	    (*(volatile uint8*)(addr))
#define MSDC_SET_BIT32(addr,mask)   (*(volatile uint32*)(addr) |= (mask))
#define MSDC_CLR_BIT32(addr,mask)   (*(volatile uint32*)(addr) &= ~(mask))
#define MSDC_SET_BIT16(addr,mask)   (*(volatile uint16*)(addr) |= (mask))
#define MSDC_CLR_BIT16(addr,mask)   (*(volatile uint16*)(addr) &= ~(mask))
#endif

#define MSDC_SET_FIELD(reg,field,val) set_field(reg,field,val)
#define MSDC_GET_FIELD(reg,field,val) get_field(reg,field,val)

#define MSDC_SET_FIELD_16(reg,field,val) set_field_16(reg,field,val)
#define MSDC_GET_FIELD_16(reg,field,val) get_field_16(reg,field,val)

#define MSDC_RETRY(expr,retry,cnt) \
   do { \
   uint32 t = cnt; \
   uint32 r = retry; \
   uint32 c = cnt; \
   while (r) { \
   if (!(expr)) break; \
   if (c-- == 0) { \
   r--; udelay(200); c = t; \
   } \
   } \
   WARN_ON(r == 0); \
   } while(0)

#define MSDC_RESET() \
   do { \
   MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_RST); \
   MSDC_RETRY(MSDC_READ32(MSDC_CFG) & MSDC_CFG_RST, 5, 1000); \
   } while(0)

#define MSDC_CLR_INT() \
   do { \
   volatile uint32 val = MSDC_READ32(MSDC_INT); \
   MSDC_WRITE32(MSDC_INT, val); \
   if (MSDC_READ32(MSDC_INT)) { \
   MSG(ERR, "[ASSERT] MSDC_INT is NOT clear\n"); \
   } \
   } while(0)

#define MSDC_CLR_FIFO() \
   do { \
   MSDC_SET_BIT32(MSDC_FIFOCS, MSDC_FIFOCS_CLR); \
   MSDC_RETRY(MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_CLR, 5, 1000); \
   } while(0)

#define MSDC_FIFO_WRITE32(val)	MSDC_WRITE32(MSDC_TXDATA, val)
#define MSDC_FIFO_READ32()	MSDC_READ32(MSDC_RXDATA)
#define MSDC_FIFO_WRITE16(val)	MSDC_WRITE16(MSDC_TXDATA, val)
#define MSDC_FIFO_READ16()	MSDC_READ16(MSDC_RXDATA)
#define MSDC_FIFO_WRITE8(val)	MSDC_WRITE8(MSDC_TXDATA, val)
#define MSDC_FIFO_READ8()	MSDC_READ8(MSDC_RXDATA)

#define MSDC_FIFO_WRITE(val)	MSDC_FIFO_WRITE32(val)
#define MSDC_FIFO_READ()	MSDC_FIFO_READ32()

#define MSDC_TXFIFOCNT() \
   ((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXCNT) >> 16)
#define MSDC_RXFIFOCNT() \
   ((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXCNT) >> 0)

#define MSDC_CARD_DETECTION_ON()  MSDC_SET_BIT32(MSDC_PS, MSDC_PS_CDEN)
#define MSDC_CARD_DETECTION_OFF() MSDC_CLR_BIT32(MSDC_PS, MSDC_PS_CDEN)

#define MSDC_DMA_ON()	MSDC_CLR_BIT32(MSDC_CFG, MSDC_CFG_PIO)
#define MSDC_DMA_OFF()	MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_PIO)

#define SDC_IS_BUSY()	    (MSDC_READ32(SDC_STS) & SDC_STS_SDCBUSY)
#define SDC_IS_CMD_BUSY()   (MSDC_READ32(SDC_STS) & SDC_STS_CMDBUSY)

#define SDC_SEND_CMD(cmd,arg) \
   do { \
   MSDC_WRITE32(SDC_ARG, (arg)); \
   MSDC_WRITE32(SDC_CMD, (cmd)); \
   } while(0)

#define MSDC_INIT_GPD_EX(gpd,extlen,cmd,arg,blknum) \
   do { \
   ((gpd_t*)gpd)->extlen = extlen; \
   ((gpd_t*)gpd)->cmd    = cmd; \
   ((gpd_t*)gpd)->arg    = arg; \
   ((gpd_t*)gpd)->blknum = blknum; \
   }while(0)

#define MSDC_INIT_BD(bd, blkpad, dwpad, dptr, dlen) \
   do { \
   BUG_ON(dlen > 0xFFFFUL); \
   ((bd_t*)bd)->blkpad = blkpad; \
   ((bd_t*)bd)->dwpad  = dwpad; \
   ((bd_t*)bd)->ptr    = (void*)dptr; \
   ((bd_t*)bd)->buflen = dlen; \
   }while(0)

#define msdc_set_vol_change_wait_count(count) \
   do { \
   MSDC_SET_FIELD(SDC_VOL_CHG,SDC_VOL_CHG_CNT,(count)); \
   }while(0)

#ifdef MMC_PROFILING
static inline void msdc_timer_init(void)
{
   /* clear. CLR[1]=1, EN[0]=0 */
   __raw_writel(0x0, GPT_BASE + 0x30);
   __raw_writel(0x2, GPT_BASE + 0x30);

   __raw_writel(0, GPT_BASE + 0x38);
   __raw_writel(32768, GPT_BASE + 0x3C);

   /* 32678HZ RTC free run */
   __raw_writel(0x30, GPT_BASE + 0x34);
   __raw_writel(0x32, GPT_BASE + 0x30);
}
static inline void msdc_timer_start(void)
{
   *(volatile unsigned int*)(GPT_BASE + 0x30) |= (1 << 0);
}
static inline void msdc_timer_stop(void)
{
   *(volatile unsigned int*)(GPT_BASE + 0x30) &= ~(1 << 0);
}
static inline void msdc_timer_stop_clear(void)
{
   *(volatile unsigned int*)(GPT_BASE + 0x30) &= ~(1 << 0); /* stop  */
   *(volatile unsigned int*)(GPT_BASE + 0x30) |= (1 << 1);  /* clear */
}
static inline unsigned int msdc_timer_get_count(void)
{
   return __raw_readl(GPT_BASE + 0x38);
}
#else
#define msdc_timer_init()	do{}while(0)
#define msdc_timer_start()	do{}while(0)
#define msdc_timer_stop()	do{}while(0)
#define msdc_timer_stop_clear() do{}while(0)
#define msdc_timer_get_count()	0
#endif

extern int msdc_send_cmd(struct mmc_host *host, struct mmc_command *cmd);
extern int msdc_wait_rsp(struct mmc_host *host, struct mmc_command *cmd);
extern int msdc_init(struct mmc_host *host, int id);
extern int msdc_pio_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_pio_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_dma_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_dma_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_stream_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_stream_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_tune_bwrite(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
extern int msdc_tune_bread(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
extern int msdc_tune_cmdrsp(struct mmc_host *host, struct mmc_command *cmd);
extern void msdc_reset_tune_counter(struct mmc_host *host);
extern int msdc_abort_handler(struct mmc_host *host, int abort_card);
extern int msdc_tune_read(struct mmc_host *host);
extern void msdc_intr_sdio(struct mmc_host *host, int enable);
extern void msdc_intr_sdio_gap(struct mmc_host * host, int enable);
extern void msdc_config_clock(struct mmc_host *host, int ddr, u32 hz);
int msdc_pio_get_sandisk_fwid(struct mmc_host *host, uchar *dst);
extern int msdc_pio_get_sandisk_fwid(struct mmc_host * host,uchar * src);
extern int msdc_dma_send_sandisk_fwid(struct mmc_host *host, uchar *buf,u32 opcode, ulong nblks);
extern void arch_clean_invalidate_cache_range(addr_t start, size_t len);
extern int msdc_cmd(struct mmc_host *host, struct mmc_command *cmd);
extern void msdc_intr_mask(struct mmc_host *host, u32 bits);
extern void msdc_set_reliable_write(struct mmc_host *host, int on);
extern void msdc_set_autocmd(struct mmc_host *host, int cmd, int on);


#endif /* end of _MSDC_H_ */

