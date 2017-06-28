/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2016. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */
#ifndef __PMIC_WRAP_INIT_H__
#define __PMIC_WRAP_INIT_H__

#include "pmic_wrap_init.h"

/****** SW ENV define *************************************/
#define PMIC_WRAP_PRELOADER      1
#define PMIC_WRAP_LK             0
#define PMIC_WRAP_KERNEL         0
#define PMIC_WRAP_SCP            0
#define PMIC_WRAP_CTP            0

#define PMIC_WRAP_DEBUG
#define PMIC_WRAP_SUPPORT
/****** For BringUp. if BringUp doesn't had PMIC, need open this ***********/
#if (PMIC_WRAP_PRELOADER)
#if CFG_FPGA_PLATFORM
#define PMIC_WRAP_NO_PMIC
#else
#undef PMIC_WRAP_NO_PMIC
#endif

#endif

#define MTK_PLATFORM_MT6757      1
/* #define MTK_PLATFORM_MT6757_OE2  0 */

/******  SW ENV header define *****************************/
#if (PMIC_WRAP_PRELOADER)
#include "sys/types.h"
#include "inc/register_base.h"
//#include <inc/register_act_util.h>
#include "debug.h"


#elif (PMIC_WRAP_KERNEL)
#ifndef CONFIG_OF
#include <mach/mtk_reg_base.h>
#include <mach/mtk_irq.h>
#endif
#include "mt-plat/sync_write.h"
#elif (PMIC_WRAP_SCP)
#include "stdio.h"
#include <string.h>
#include "FreeRTOS.h"
#elif (PMIC_WRAP_CTP)
#include <sync_write.h>
#include <typedefs.h>
#include <reg_base.H>
#else
### Compile error, check SW ENV define
#endif

/*******************start ---external API********************************/
extern signed int pwrap_read(unsigned int adr, unsigned int *rdata);
extern signed int pwrap_write(unsigned int adr, unsigned int wdata);
extern signed int pwrap_write_nochk(unsigned int adr, unsigned int wdata);
extern signed int pwrap_read_nochk(unsigned int adr, unsigned int *rdata);
extern signed int pwrap_wacs2(unsigned int write, unsigned int adr, unsigned int wdata, unsigned int *rdata);
extern void pwrap_dump_all_register(void);
extern signed int pwrap_init_preloader(void);
extern signed int pwrap_init_lk(void);
extern signed int pwrap_init_scp(void);
extern signed int pwrap_init(void);
extern signed int pwrap_init_DA_mt6355(void);


/******  DEBUG marco define *******************************/
#if 0
#define PWRAPTAG                "[PWRAP] "
#if (PMIC_WRAP_PRELOADER)
#ifdef PMIC_WRAP_DEBUG
#define PWRAPFUC(fmt, arg...)   print(PWRAPTAG "%s\n", __FUNCTION__)
#define PWRAPLOG(fmt, arg...)   print(PWRAPTAG fmt,##arg)
#define PWRAPERR(fmt, arg...)   print(PWRAPTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#endif
#define PWRAPFUC(fmt, arg...)   print(PWRAPTAG "%s\n", __FUNCTION__)
#define PWRAPLOG(fmt, arg...)
#define PWRAPERR(fmt, arg...)   print(PWRAPTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#elif (PMIC_WRAP_LK)
#ifdef PMIC_WRAP_DEBUG
#define PWRAPFUC(fmt, arg...)   dprintf(CRITICAL,PWRAPTAG "%s\n", __FUNCTION__)
#define PWRAPLOG(fmt, arg...)   dprintf(CRITICAL,PWRAPTAG fmt,##arg)
#define PWRAPERR(fmt, arg...)   dprintf(CRITICAL,PWRAPTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#endif
#define PWRAPFUC(fmt, arg...)   dprintf(CRITICAL,PWRAPTAG "%s\n", __FUNCTION__)
#define PWRAPLOG(fmt, arg...)
#define PWRAPERR(fmt, arg...)   dprintf(CRITICAL,PWRAPTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#elif (PMIC_WRAP_KERNEL)
#ifdef PMIC_WRAP_DEBUG
#define PWRAPDEB(fmt, arg...)   pr_debug(PWRAPTAG "cpuid=%d," fmt, raw_smp_processor_id(), ##arg)
#define PWRAPFUC(fmt, arg...)   pr_debug(PWRAPTAG "cpuid=%d,%s\n", raw_smp_processor_id(), __func__)
#endif
#define PWRAPLOG(fmt, arg...)   pr_debug(PWRAPTAG fmt, ##arg)
#define PWRAPERR(fmt, arg...)   pr_err(PWRAPTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#define PWRAPREG(fmt, arg...)   pr_debug(PWRAPTAG fmt, ##arg)
#elif (PMIC_WRAP_SCP)
#ifdef PMIC_WRAP_DEBUG
#define PWRAPFUC(fmt, arg...)   PRINTF_D(PWRAPTAG "%s\n", __FUNCTION__)
#define PWRAPLOG(fmt, arg...)   PRINTF_D(PWRAPTAG fmt, ##arg)
#else
#define PWRAPFUC(fmt, arg...)   /*PRINTF_D(PWRAPTAG "%s\n", __FUNCTION__)*/
#define PWRAPLOG(fmt, arg...)   /*PRINTF_D(PWRAPTAG fmt, ##arg)*/
#endif
#define PWRAPERR(fmt, arg...)   PRINTF_E(PWRAPTAG "ERROR, line=%d " fmt, __LINE__, ##arg)
#elif (PMIC_WRAP_CTP)
#ifdef PMIC_WRAP_DEBUG
#define PWRAPFUC(fmt, arg...)   dbg_print(PWRAPTAG "%s\n", __FUNCTION__)
#define PWRAPLOG(fmt, arg...)   dbg_print(PWRAPTAG fmt,##arg)
#define PWRAPERR(fmt, arg...)   dbg_print(PWRAPTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#endif
#define PWRAPFUC(fmt, arg...)   dbg_print(PWRAPTAG "%s\n", __FUNCTION__)
#define PWRAPLOG(fmt, arg...)   dbg_print(PWRAPTAG fmt,##arg)
#define PWRAPERR(fmt, arg...)   dbg_print(PWRAPTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#else
### Compile error, check SW ENV define
#endif
#endif
/**********************************************************/

/***********  platform info, PMIC info ********************/
//#define PMIC_WRAP_BASE          (PWRAP_BASE)
#define PMIC_WRAP_REG_RANGE_MT6355     (354)

#define DEFAULT_VALUE_READ_TEST                 (0x5aa5)
#define PWRAP_WRITE_TEST_VALUE                  (0x1234)

#if (PMIC_WRAP_SCP)
#define UINT32  unsigned int
#define UINT32P unsigned int *
#endif /* end #if (PMIC_WRAP_SCP) */
/**********************************************************/
#if 0
// have defined in pmic_wrap_init.h
#define ENABLE          (1)
#define DISABLE         (0)
#define DISABLE_ALL     (0)

/* HIPRIS_ARB */
#define MDINF		(1 << 0)
#define WACS0		(1 << 1)
#define WACS1		(1 << 2)
#define WACS2		(1 << 11)
#define DVFSINF		(1 << 3)
#define STAUPD		(1 << 5)
#define GPSINF		(1 << 6)

/* MUX SEL */
#define WRAPPER_MODE    (0)
#define MANUAL_MODE     (1)

/* macro for MAN_RDATA  FSM */
#define MAN_FSM_NO_REQ             (0x00)
#define MAN_FSM_IDLE               (0x00)
#define MAN_FSM_REQ                (0x02)
#define MAN_FSM_WFDLE              (0x04) /* wait for dle,wait for read data done, */
#define MAN_FSM_WFVLDCLR           (0x06)

/* macro for WACS_FSM */
#define WACS_FSM_IDLE               (0x00)
#define WACS_FSM_REQ                (0x02) /* request in process */
#define WACS_FSM_WFDLE              (0x04) /* wait for dle,wait for read data done, */
#define WACS_FSM_WFVLDCLR           (0x06) /* finish read data , wait for valid flag clearing */
#define WACS_INIT_DONE              (0x01)
#define WACS_SYNC_IDLE              (0x01)
#define WACS_SYNC_BUSY              (0x00)

/**** timeout time, unit :us ***********/
#define TIMEOUT_RESET           (0xFF)
#define TIMEOUT_READ            (0xFF)
#define TIMEOUT_WAIT_IDLE       (0xFF)

/*-----macro for manual commnd ---------------------------------*/
#define OP_WR    (0x1)
#define OP_RD    (0x0)
#define OP_CSH   (0x0)
#define OP_CSL   (0x1)
#define OP_CK    (0x2)
#define OP_OUTS  (0x8)
#define OP_OUTD  (0x9)
#define OP_OUTQ  (0xA)
#define OP_INS   (0xC)
#define OP_INS0  (0xD)
#define OP_IND   (0xE)
#define OP_INQ   (0xF)
#define OP_OS2IS (0x10)
#define OP_OS2ID (0x11)
#define OP_OS2IQ (0x12)
#define OP_OD2IS (0x13)
#define OP_OD2ID (0x14)
#define OP_OD2IQ (0x15)
#define OP_OQ2IS (0x16)
#define OP_OQ2ID (0x17)
#define OP_OQ2IQ (0x18)
#define OP_OSNIS (0x19)
#define OP_ODNID (0x1A)

/******************Error handle *****************************/
#define E_PWR_INVALID_ARG               (1)
#define E_PWR_INVALID_RW                (2)
#define E_PWR_INVALID_ADDR              (3)
#define E_PWR_INVALID_WDAT              (4)
#define E_PWR_INVALID_OP_MANUAL         (5)
#define E_PWR_NOT_IDLE_STATE            (6)
#define E_PWR_NOT_INIT_DONE             (7)
#define E_PWR_NOT_INIT_DONE_READ        (8)
#define E_PWR_WAIT_IDLE_TIMEOUT         (9)
#define E_PWR_WAIT_IDLE_TIMEOUT_READ    (10)
#define E_PWR_INIT_SIDLY_FAIL           (11)
#define E_PWR_RESET_TIMEOUT             (12)
#define E_PWR_TIMEOUT                   (13)
#define E_PWR_INIT_RESET_SPI            (20)
#define E_PWR_INIT_SIDLY                (21)
#define E_PWR_INIT_REG_CLOCK            (22)
#define E_PWR_INIT_ENABLE_PMIC          (23)
#define E_PWR_INIT_DIO                  (24)
#define E_PWR_INIT_CIPHER               (25)
#define E_PWR_INIT_WRITE_TEST           (26)
#define E_PWR_INIT_ENABLE_CRC           (27)
#define E_PWR_INIT_ENABLE_DEWRAP        (28)
#define E_PWR_READ_TEST_FAIL            (30)
#define E_PWR_WRITE_TEST_FAIL           (31)
#define E_PWR_SWITCH_DIO                (32)


/*-----macro for read/write register -------------------------------------*/

#define WRAP_RD32(addr)            (*(volatile unsigned int *)(addr))
#define WRAP_WR32(addr,data)       ((*(volatile unsigned int *)(addr)) = (unsigned int)data)
#define WRAP_SET_BIT(BS,REG)       ((*(volatile unsigned int*)(REG)) |= (unsigned int)(BS))
#define WRAP_CLR_BIT(BS,REG)       ((*(volatile unsigned int*)(REG)) &= ~((unsigned int)(BS)))

/**************** end ---external API***********************************/
#endif
/************* macro for spi clock config ******************************/
#if (MTK_PLATFORM_MT6757)
#define CLK_CFG_4_SET                       (TOPCKGEN_BASE+0x084)
//#define CLK_CFG_4_CLR                       (TOPCKGEN_BASE+0x088)
//#define CLK_CFG_5_CLR                       (TOPCKGEN_BASE+0x098)

//#define CLK_SPI_CK_26M                       0x1
//#define INFRA_GLOBALCON_RST0                (INFRACFG_AO_BASE+0x140)
//#define INFRA_GLOBALCON_RST1                (INFRACFG_AO_BASE+0x144)
//#define PMIC_CLOCK_DCM          		    (INFRACFG_AO_BASE+0x074)
//#define APB_CLOCK_GATING                    (INFRACFG_AO_BASE+0xF0C)
#endif /* end of MTK_PLATFORM_MT6757 */
#define MODULE_CLK_SEL                      (INFRACFG_AO_BASE+0x098)
//#define MODULE_SW_CG_0_SET                  (INFRACFG_AO_BASE+0x080)
//#define MODULE_SW_CG_0_CLR                  (INFRACFG_AO_BASE+0x084)
#define INFRA_GLOBALCON_RST2_SET            (INFRACFG_AO_BASE+0x140)
#define INFRA_GLOBALCON_RST2_CLR            (INFRACFG_AO_BASE+0x144)

/*****************************************************************/
//#include <reg_PMIC_WRAP.h>

#define PMIC_WRAP_DEBUG_SW_DRIVER_0     ((UINT32P)(PMIC_WRAP_BASE+0x340))
#define PMIC_WRAP_DEBUG_SW_DRIVER_1     ((UINT32P)(PMIC_WRAP_BASE+0x344))
#define PMIC_WRAP_DEBUG_SW_DRIVER_2     ((UINT32P)(PMIC_WRAP_BASE+0x348))
#define PMIC_WRAP_DEBUG_SW_DRIVER_3     ((UINT32P)(PMIC_WRAP_BASE+0x34C))
#define PMIC_WRAP_DEBUG_SW_DRIVER_4     ((UINT32P)(PMIC_WRAP_BASE+0x350))
#define PMIC_WRAP_DEBUG_SW_DRIVER_5     ((UINT32P)(PMIC_WRAP_BASE+0x354))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR_LATEST_1  ((UINT32P)(PMIC_WRAP_BASE+0x358))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR_WP_1  ((UINT32P)(PMIC_WRAP_BASE+0x35C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR0_1  ((UINT32P)(PMIC_WRAP_BASE+0x360))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR1_1  ((UINT32P)(PMIC_WRAP_BASE+0x364))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR2_1  ((UINT32P)(PMIC_WRAP_BASE+0x368))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR3_1  ((UINT32P)(PMIC_WRAP_BASE+0x36C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR4_1  ((UINT32P)(PMIC_WRAP_BASE+0x370))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR5_1  ((UINT32P)(PMIC_WRAP_BASE+0x374))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR6_1  ((UINT32P)(PMIC_WRAP_BASE+0x378))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR7_1  ((UINT32P)(PMIC_WRAP_BASE+0x37C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR8_1  ((UINT32P)(PMIC_WRAP_BASE+0x380))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR9_1  ((UINT32P)(PMIC_WRAP_BASE+0x384))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR10_1  ((UINT32P)(PMIC_WRAP_BASE+0x388))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR11_1  ((UINT32P)(PMIC_WRAP_BASE+0x38C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR12_1  ((UINT32P)(PMIC_WRAP_BASE+0x390))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR13_1  ((UINT32P)(PMIC_WRAP_BASE+0x394))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR14_1  ((UINT32P)(PMIC_WRAP_BASE+0x398))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR15_1  ((UINT32P)(PMIC_WRAP_BASE+0x39C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR16_1  ((UINT32P)(PMIC_WRAP_BASE+0x3A0))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR17_1  ((UINT32P)(PMIC_WRAP_BASE+0x3A4))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR18_1  ((UINT32P)(PMIC_WRAP_BASE+0x3A8))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR19_1  ((UINT32P)(PMIC_WRAP_BASE+0x3AC))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR20_1  ((UINT32P)(PMIC_WRAP_BASE+0x3B0))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR21_1  ((UINT32P)(PMIC_WRAP_BASE+0x3B4))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR22_1  ((UINT32P)(PMIC_WRAP_BASE+0x3B8))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR23_1  ((UINT32P)(PMIC_WRAP_BASE+0x3BC))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR24_1  ((UINT32P)(PMIC_WRAP_BASE+0x3C0))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR25_1  ((UINT32P)(PMIC_WRAP_BASE+0x3C4))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR26_1  ((UINT32P)(PMIC_WRAP_BASE+0x3C8))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR27_1  ((UINT32P)(PMIC_WRAP_BASE+0x3CC))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR28_1  ((UINT32P)(PMIC_WRAP_BASE+0x3D0))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR29_1  ((UINT32P)(PMIC_WRAP_BASE+0x3D4))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR30_1  ((UINT32P)(PMIC_WRAP_BASE+0x3D8))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR31_1  ((UINT32P)(PMIC_WRAP_BASE+0x3DC))
#define PMIC_WRAP_MD_ADC_MODE_SEL_1     ((UINT32P)(PMIC_WRAP_BASE+0x3E0))
#define PMIC_WRAP_MD_ADC_STA0_1         ((UINT32P)(PMIC_WRAP_BASE+0x3E4))
#define PMIC_WRAP_MD_ADC_STA1_1         ((UINT32P)(PMIC_WRAP_BASE+0x3E8))
#define PMIC_WRAP_ADC_CMD_ADDR_1        ((UINT32P)(PMIC_WRAP_BASE+0x3EC))
#define PMIC_WRAP_PWRAP_ADC_CMD_1       ((UINT32P)(PMIC_WRAP_BASE+0x3F0))
#define PMIC_WRAP_ADC_RDATA_ADDR_1      ((UINT32P)(PMIC_WRAP_BASE+0x3F4))
#define PMIC_WRAP_GPS_STA_1             ((UINT32P)(PMIC_WRAP_BASE+0x3F8))
#define PMIC_WRAP_HARB_STA2             ((UINT32P)(PMIC_WRAP_BASE+0x3FC))

//#include <reg_PMIC_WRAP_mac.h>
#define GET_PWRAP_MD_ADC_TEMP_DATA_1(x)  ((x>>16) & 0x0000ffff)
#define GET_PWRAP_GPS_ACK_1(x)       ((x>>0)  & 0x00000001)
#define GET_GPS_PWRAP_REQ_1(x)       ((x>>1)  & 0x00000001)
#define GET_GPSINF_DLE_CNT_1(x)      ((x>>4)  & 0x00000003)
#define GET_GPSINF_ALE_CNT_1(x)      ((x>>6)  & 0x00000003)
#define GET_GPS_INF_FSM_1(x)         ((x>>8)  & 0x00000007)
#define GET_PWRAP_GPS_WDATA_1(x)     ((x>>17) & 0x00007fff)
#define GET_AG_HARB_REQ_14_15(x)     ((x>>0)  & 0x00000003)
/*******************macro for  regsister@PMIC *******************************/
#include "upmu_hw.h"
#endif
