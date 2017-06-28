/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  History:
*  YYYYMMDD MTK12345 Description
*  --------------------
*  20131116 MTK81353 Initial version.
*
*
*******************************************************************************/

#ifndef __I2C_H__
#define __I2C_H__
#include <sys/types.h>
#include "../../inc/register_base.h"
#include "../../inc/register_act_util.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// I2C Register
//==============================================================================

#define MT_I2C_DATA_PORT                    (I2C_BASE + 0x0000)
#define MT_I2C_SLAVE_ADDR                   (I2C_BASE + 0x0004)
#define MT_I2C_INTR_MASK                    (I2C_BASE + 0x0008)
#define MT_I2C_INTR_STAT                    (I2C_BASE + 0x000c)
#define MT_I2C_CONTROL                      (I2C_BASE + 0x0010)
#define MT_I2C_TRANSFER_LEN                 (I2C_BASE + 0x0014)
#define MT_I2C_TRANSAC_LEN                  (I2C_BASE + 0x0018)
#define MT_I2C_DELAY_LEN                    (I2C_BASE + 0x001c)
#define MT_I2C_TIMING                       (I2C_BASE + 0x0020)
#define MT_I2C_START                        (I2C_BASE + 0x0024)
#define MT_I2C_FIFO_STAT                    (I2C_BASE + 0x0030)
#define MT_I2C_FIFO_THRESH                  (I2C_BASE + 0x0034)
#define MT_I2C_FIFO_ADDR_CLR                (I2C_BASE + 0x0038)
#define MT_I2C_IO_CONFIG                    (I2C_BASE + 0x0040)
#define MT_I2C_DEBUG                        (I2C_BASE + 0x0044)
#define MT_I2C_HS                           (I2C_BASE + 0x0048)
#define MT_I2C_SOFTRESET                    (I2C_BASE + 0x0050)
#define MT_I2C_DEBUGSTAT                    (I2C_BASE + 0x0064)
#define MT_I2C_DEBUGCTRL                    (I2C_BASE + 0x0068)

#define I2C_TRANS_LEN_MASK                  (0xff)
#define I2C_TRANS_AUX_LEN_MASK              (0x1f << 8)
#define I2C_CONTROL_MASK                    (0x3f << 1)

//----------- Register mask -------------------//
#define I2C_3_BIT_MASK                      0x07
#define I2C_4_BIT_MASK                      0x0f
#define I2C_6_BIT_MASK                      0x3f
#define I2C_8_BIT_MASK                      0xff
#define I2C_FIFO_THRESH_MASK                0x07
#define I2C_AUX_LEN_MASK                    0x1f00
#define I2C_MASTER_READ                     0x01
#define I2C_MASTER_WRITE                    0x00
#define I2C_CTL_RS_STOP_BIT                 0x02
#define I2C_CTL_DMA_EN_BIT                  0x04
#define I2C_CTL_CLK_EXT_EN_BIT              0x08
#define I2C_CTL_DIR_CHANGE_BIT              0x10
#define I2C_CTL_ACK_ERR_DET_BIT             0x20 
#define I2C_CTL_TRANSFER_LEN_CHG_BIT        0x40
#define I2C_DATA_READ_ADJ_BIT               0x8000
#define I2C_SCL_MODE_BIT                    0x01
#define I2C_SDA_MODE_BIT                    0x02
#define I2C_BUS_DETECT_EN_BIT               0x04
#define I2C_ARBITRATION_BIT                 0x01
#define I2C_CLOCK_SYNC_BIT                  0x02
#define I2C_BUS_BUSY_DET_BIT                0x04
#define I2C_HS_EN_BIT                       0x01
#define I2C_HS_NACK_ERR_DET_EN_BIT          0x02
#define I2C_HS_MASTER_CODE_MASK             0x70
#define I2C_HS_STEP_CNT_DIV_MASK            0x700
#define I2C_HS_SAMPLE_CNT_DIV_MASK          0x7000
#define I2C_FIFO_FULL_STATUS                0x01
#define I2C_FIFO_EMPTY_STATUS               0x02

#define I2C_DEBUG                           (1 << 3)
#define I2C_HS_NACKERR                      (1 << 2)
#define I2C_ACKERR                          (1 << 1)
#define I2C_TRANSAC_COMP                    (1 << 0)

#define I2C_TX_THR_OFFSET                   8
#define I2C_RX_THR_OFFSET                   0

/* i2c control bits */
#define TRANS_LEN_CHG                       (1 << 6)
#define ACK_ERR_DET_EN                      (1 << 5)
#define DIR_CHG                             (1 << 4)
#define CLK_EXT                             (1 << 3)
#define DMA_EN                              (1 << 2)
#define REPEATED_START_FLAG                 (1 << 1)
#define STOP_FLAG                           (0 << 1)

//==============================================================================
// I2C Internal Configuration
//==============================================================================
#define MAX_DMA_TRANS_SIZE                  252     // Max(255) aligned to 4 bytes = 252
#define MAX_DMA_TRANS_NUM                   256

#define MAX_SAMPLE_CNT_DIV                  8
#define MAX_STEP_CNT_DIV                    64
#define MAX_HS_STEP_CNT_DIV                 8
#define I2C_TIMEOUT_TH                      200     // i2c wait for response timeout value, 200ms

#define I2C_FIFO_LEN                        8       // 8 bytes

//==============================================================================
// I2C Macro
//==============================================================================
#define __raw_writel(data,addr)            DRV_WriteReg32(addr,data)
#define __raw_writew(data,addr)            DRV_WriteReg32(addr, data)
#define __raw_readw(addr)                  DRV_Reg32(addr)
#define __raw_writeb(data,addr)            DRV_WriteReg8(addr, data)
#define __raw_readb(addr)                  DRV_Reg8(addr)
#define I2C_START_TRANSAC                   __raw_writel(0x1,MT_I2C_START)
#define I2C_FIFO_CLR_ADDR                   __raw_writel(0x1,MT_I2C_FIFO_ADDR_CLR)
#define I2C_FIFO_OFFSET                     (__raw_readl(MT_I2C_FIFO_STAT)>>4&0xf)
#define I2C_FIFO_IS_EMPTY                   (__raw_readw(MT_I2C_FIFO_STAT)>>0&0x1)

#define I2C_SOFTRESET                       __raw_writel(0x1,MT_I2C_SOFTRESET)
#define I2C_INTR_STATUS                     __raw_readw(MT_I2C_INTR_STAT)

#define I2C_SET_BITS(BS,REG)                ((*(volatile u32*)(REG)) |= (u32)(BS))
#define I2C_CLR_BITS(BS,REG)                ((*(volatile u32*)(REG)) &= ~((u32)(BS)))

#define I2C_SET_FIFO_THRESH(tx,rx) \
    do { u32 tmp = (((tx) & 0x7) << I2C_TX_THR_OFFSET) | \
                   (((rx) & 0x7) << I2C_RX_THR_OFFSET); \
         __raw_writel(tmp, MT_I2C_FIFO_THRESH); \
    } while(0)

#define I2C_SET_INTR_MASK(mask)             __raw_writel(mask, MT_I2C_INTR_MASK)

#define I2C_CLR_INTR_MASK(mask)\
    do { U32 tmp = __raw_readl(MT_I2C_INTR_MASK); \
         tmp &= ~(mask); \
         __raw_writel(tmp, MT_I2C_INTR_MASK); \
    } while(0)

#define I2C_SET_SLAVE_ADDR(addr)            __raw_writel(addr, MT_I2C_SLAVE_ADDR)

#define I2C_SET_TRANS_LEN(len) \
    do { U32 tmp = __raw_readl(MT_I2C_TRANSFER_LEN) & \
                              ~I2C_TRANS_LEN_MASK; \
         tmp |= ((len) & I2C_TRANS_LEN_MASK); \
         __raw_writel(tmp, MT_I2C_TRANSFER_LEN); \
    } while(0)

#define I2C_SET_TRANS_AUX_LEN(len) \
    do { U32 tmp = __raw_readl(MT_I2C_TRANSFER_LEN) & \
                             ~(I2C_TRANS_AUX_LEN_MASK); \
         tmp |= (((len) << 8) & I2C_TRANS_AUX_LEN_MASK); \
         __raw_writel(tmp, MT_I2C_TRANSFER_LEN); \
    } while(0)

#define I2C_SET_TRANSAC_LEN(len)            __raw_writel(len, MT_I2C_TRANSAC_LEN)
#define I2C_SET_TRANS_DELAY(delay)          __raw_writel(delay, MT_I2C_DELAY_LEN)

#define I2C_SET_TRANS_CTRL(ctrl)\
    do { U32 tmp = __raw_readl(MT_I2C_CONTROL) & ~I2C_CONTROL_MASK; \
        tmp |= ((ctrl) & I2C_CONTROL_MASK); \
        __raw_writel(tmp, MT_I2C_CONTROL); \
    } while(0)

#define I2C_SET_HS_MODE(on_off) \
    do { U32 tmp = __raw_readl(MT_I2C_HS) & ~0x1; \
    tmp |= (on_off & 0x1); \
    __raw_writel(tmp, MT_I2C_HS); \
    } while(0)

#define I2C_READ_BYTE(byte)     \
    do { byte = __raw_readb(MT_I2C_DATA_PORT); } while(0)
    
#define I2C_WRITE_BYTE(byte) \
    do { __raw_writeb(byte, MT_I2C_DATA_PORT); } while(0)

#define I2C_CLR_INTR_STATUS(status) \
    do { __raw_writew(status, MT_I2C_INTR_STAT); } while(0)

#endif
//==============================================================================
// I2C Exported Configuration
//============================================================================== 
#define MAX_ST_MODE_SPEED                   100     /* khz */
#define MAX_FS_MODE_SPEED                   400     /* khz */
#define MAX_HS_MODE_SPEED                   3400    /* khz */

typedef enum {
    ST_MODE,
    FS_MODE,
    HS_MODE,
} I2C_SPD_MODE;
//==============================================================================
// I2C driver error
//==============================================================================
#define E_I2C_NOT_SUPPRT                                0xA000
#define E_I2C_SET_SPEED_FAIL_OVER_SPEED                 0xA001
#define E_I2C_READ_FAIL_ZERO_LENGTH                     0xA002
#define E_I2C_READ_FAIL_HS_NACKERR                      0xA003
#define E_I2C_READ_FAIL_ACKERR                          0xA004
#define E_I2C_READ_FAIL_TIMEOUT                         0xA005
#define E_I2C_WRITE_FAIL_ZERO_LENGTH                    0xA006
#define E_I2C_WRITE_FAIL_HS_NACKERR                     0xA007
#define E_I2C_WRITE_FAIL_ACKERR                         0xA008
#define E_I2C_WRITE_FAIL_TIMEOUT                        0xA009
//==============================================================================
// I2C Exported Function
//==============================================================================
extern unsigned int i2c_v1_init (void);
extern unsigned int i2c_v1_deinit (void);
extern unsigned int i2c_v1_set_speed (unsigned long clock, I2C_SPD_MODE mode, unsigned long khz);
extern unsigned int i2c_v1_read(unsigned char chip, unsigned char *buffer, int len);
extern unsigned int i2c_v1_write (unsigned char chip, unsigned char *buffer, int len);
