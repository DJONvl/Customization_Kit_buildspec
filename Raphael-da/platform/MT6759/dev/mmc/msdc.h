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
#include <inc/register_base.h>

/*--------------------------------------------------------------------------*/
/* Common Definition                                                        */
/*--------------------------------------------------------------------------*/
#define MSDC_FIFO_SZ            (128)
#define MSDC_FIFO_THD           (64)
#define MSDC_MAX_NUM            (4)

#define MSDC_MS                 (0)
#define MSDC_SDMMC              (1)

#define MSDC_MODE_UNKNOWN       (0)
#define MSDC_MODE_PIO           (1)
#define MSDC_MODE_DMA_BASIC     (2)
#define MSDC_MODE_DMA_DESC      (3)
#define MSDC_MODE_DMA_ENHANCED  (4)
#define MSDC_MODE_MMC_STREAM    (5)

#define MSDC_BUS_1BITS          (0)
#define MSDC_BUS_4BITS          (1)
#define MSDC_BUS_8BITS          (2)

#define MSDC_BRUST_8B           (3)
#define MSDC_BRUST_16B          (4)
#define MSDC_BRUST_32B          (5)
#define MSDC_BRUST_64B          (6)

#define MSDC_PIN_PULL_NONE      (0)
#define MSDC_PIN_PULL_DOWN      (1)
#define MSDC_PIN_PULL_UP        (2)
#define MSDC_PIN_KEEP           (3)

#if !defined(CFG_FPGA_PLATFORM)
#define MSDC_MAX_SCLK           (200000000)
#define MSDC_MIN_SCLK           (260000)
#else
#define MSDC_SRC_CLK            (6000000)
#define MSDC_MAX_SCLK           (MSDC_SRC_CLK >> 1)
#define MSDC_MIN_SCLK           (260000)
#endif

#define MSDC_AUTOCMD12          (0x0001)
#define MSDC_AUTOCMD23          (0x0002)
#define MSDC_AUTOCMD19          (0x0003)

#include "msdc_reg.h"

#define MSDC_GPIO_BASE          GPIO_BASE
#define MSDC0_IO_PAD_BASE       (IO_CFG_T_BASE)

#define MSDC0_GPIO_MODE0       (MSDC_GPIO_BASE + 0x370)
#define MSDC0_GPIO_MODE1       (MSDC_GPIO_BASE + 0x380)
#define MSDC0_GPIO_IES     (MSDC0_IO_PAD_BASE + 0x20)
#define MSDC0_GPIO_SMT     (MSDC0_IO_PAD_BASE + 0xa0)
#define MSDC0_GPIO_TDSEL   (MSDC0_IO_PAD_BASE + 0xb0)
#define MSDC0_GPIO_RDSEL   (MSDC0_IO_PAD_BASE + 0x80)
#define MSDC0_GPIO_DRV     (MSDC0_IO_PAD_BASE + 0)
#define MSDC0_GPIO_PUPD   (MSDC0_IO_PAD_BASE + 0x40)
#define MSDC0_GPIO_R0   (MSDC0_IO_PAD_BASE + 0x60)
#define MSDC0_GPIO_R1   (MSDC0_IO_PAD_BASE + 0x70)

/*--------------------------------------------------------------------------*/
/* Descriptor Structure                                                     */
/*--------------------------------------------------------------------------*/
typedef struct {
   uint32  hwo:1; /* could be changed by hw */
   uint32  bdp:1;
   uint32  rsv0:6;
   uint32  chksum:8;
   uint32  intr:1;
   uint32  rsv1:7;
   uint32  nexth4:4;
   uint32  ptrh4:4;
   void   *next;
   void   *ptr;
   uint32  buflen:24;
   uint32  extlen:8;
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
   uint32  nexth4:4;
   uint32  ptrh4:4;
   void   *next;
   void   *ptr;
   uint32  buflen:24;
   uint32  rsv3:8;
} bd_t;

struct scatterlist {
   u32 addr;
   u32 len;
};

#define DMA_FLAG_NONE       (0x00000000)
#define DMA_FLAG_EN_CHKSUM  (0x00000001)
#define DMA_FLAG_PAD_BLOCK  (0x00000002)
#define DMA_FLAG_PAD_DWORD  (0x00000004)

struct dma_config {
   u32 flags;                   /* flags */
   u32 xfersz;                  /* xfer size in bytes */
   u32 sglen;                   /* size of scatter list */
   u32 blklen;                  /* block size */
   struct scatterlist *sg;      /* I/O scatter list */
   u8  mode;                    /* dma mode */
   u8  burstsz;                 /* burst size */
   u8  intr;                    /* dma done interrupt */
   u8  padding;                 /* padding */
   u32 cmd;                     /* enhanced mode command */
   u32 arg;                     /* enhanced mode arg */
   u32 rsp;                     /* enhanced mode command response */
   u32 autorsp;                 /* auto command response */
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
#define MSDC_READ32(addr)           reg32_read((volatile uint32*)addr)
#define MSDC_WRITE16(addr,data)     reg16_write((volatile uint32*)addr, data)
#define MSDC_READ16(addr)           reg16_read((volatile uint32*)addr)
#define MSDC_WRITE8(addr, data)     reg8_write((volatile uint32*)addr, data)
#define MSDC_READ8(addr)            reg8_read((volatile uint32*)addr)
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
#define MSDC_READ32(addr)           (*(volatile uint32*)(addr))
#define MSDC_WRITE16(addr,data)     (*(volatile uint16*)(addr) = (uint16)(data))
#define MSDC_READ16(addr)           (*(volatile uint16*)(addr))
#define MSDC_WRITE8(addr, data)     (*(volatile uint8*)(addr)  = (uint8)(data))
#define MSDC_READ8(addr)            (*(volatile uint8*)(addr))
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

#define MSDC_FIFO_WRITE32(val)  MSDC_WRITE32(MSDC_TXDATA, val)
#define MSDC_FIFO_READ32()      MSDC_READ32(MSDC_RXDATA)
#define MSDC_FIFO_WRITE16(val)  MSDC_WRITE16(MSDC_TXDATA, val)
#define MSDC_FIFO_READ16()      MSDC_READ16(MSDC_RXDATA)
#define MSDC_FIFO_WRITE8(val)   MSDC_WRITE8(MSDC_TXDATA, val)
#define MSDC_FIFO_READ8()       MSDC_READ8(MSDC_RXDATA)

#define MSDC_FIFO_WRITE(val)    MSDC_FIFO_WRITE32(val)
#define MSDC_FIFO_READ()        MSDC_FIFO_READ32()

#define MSDC_TXFIFOCNT() \
   ((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXCNT) >> 16)
#define MSDC_RXFIFOCNT() \
   ((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXCNT) >> 0)

#define MSDC_CARD_DETECTION_ON()  MSDC_SET_BIT32(MSDC_PS, MSDC_PS_CDEN)
#define MSDC_CARD_DETECTION_OFF() MSDC_CLR_BIT32(MSDC_PS, MSDC_PS_CDEN)

#define MSDC_DMA_ON()           MSDC_CLR_BIT32(MSDC_CFG, MSDC_CFG_PIO)
#define MSDC_DMA_OFF()          MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_PIO)

#define SDC_IS_BUSY()           (MSDC_READ32(SDC_STS) & SDC_STS_SDCBUSY)
#define SDC_IS_CMD_BUSY()       (MSDC_READ32(SDC_STS) & SDC_STS_CMDBUSY)

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
#define msdc_timer_init()       do{}while(0)
#define msdc_timer_start()      do{}while(0)
#define msdc_timer_stop()       do{}while(0)
#define msdc_timer_stop_clear() do{}while(0)
#define msdc_timer_get_count()  0
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

extern int msdc_pio_send_sandisk_fwid(struct mmc_host *host,uchar *buf, u32 opcode);

extern int msdc_pio_get_sandisk_fwid(struct mmc_host * host,uchar * src);
extern int msdc_dma_send_sandisk_fwid(struct mmc_host *host, uchar *buf,u32 opcode, ulong nblks);
extern void arch_clean_invalidate_cache_range(addr_t start, size_t len);
extern int msdc_cmd(struct mmc_host *host, struct mmc_command *cmd);
extern void msdc_intr_mask(struct mmc_host *host, u32 bits);
extern void msdc_set_reliable_write(struct mmc_host *host, int on);
extern void msdc_set_autocmd(struct mmc_host *host, int cmd, int on);


#endif /* end of _MSDC_H_ */

