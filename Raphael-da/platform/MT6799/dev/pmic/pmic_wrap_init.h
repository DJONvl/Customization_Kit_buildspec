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

#define PMIC_WRAP_DEBUG
#define PMIC_WRAP_SUPPORT
/****** For BringUp. if BringUp doesn't had PMIC, need open this ***********/

#if CFG_FPGA_PLATFORM
#define PMIC_WRAP_NO_PMIC
#else
#undef PMIC_WRAP_NO_PMIC
#endif

/******  SW ENV header define *****************************/
#include "sys/types.h"
#include "inc/register_base.h"
#include <inc/register_act_util.h>
#include "debug.h"

/*******************start ---external API********************************/
extern signed int pwrap_read(unsigned int adr, unsigned int *rdata);
extern signed int pwrap_write(unsigned int adr, unsigned int wdata);
extern signed int pwrap_write_nochk(unsigned int adr, unsigned int wdata);
extern signed int pwrap_read_nochk(unsigned int adr, unsigned int *rdata);
extern signed int pwrap_wacs2(unsigned int write, unsigned int adr, unsigned int wdata, unsigned int *rdata);
extern void pwrap_dump_all_register(void);
extern signed int pwrap_init_DA(void);
extern signed int pwrap_init(void);

/******  DEBUG marco define *******************************/
#define PWRAPTAG                "[PWRAP] "

#ifdef PMIC_WRAP_DEBUG
#define PWRAPFUC(fmt, arg...)   print(PWRAPTAG "%s\n", __FUNCTION__)
#define PWRAPLOG(fmt, arg...)   print(PWRAPTAG fmt,##arg)
#define PWRAPERR(fmt, arg...)   print(PWRAPTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#endif
/**********************************************************/

/***********  platform info, PMIC info ********************/
#define PMIC_WRAP_BASE          (PWRAP_BASE)
#define PMIC_WRAP_REG_RANGE     (354)

#define DEFAULT_VALUE_READ_TEST                 (0x5aa5)
#define DEFAULT_VALUE_WRITE_TEST                (0xa55a)
#define PWRAP_WRITE_TEST_VALUE                  (0x1234)

#if (PMIC_WRAP_SCP)
#define UINT32  unsigned int
#define UINT32P unsigned int *
#endif /* end #if (PMIC_WRAP_SCP) */
/**********************************************************/

#define ENABLE          (1)
#define DISABLE         (0)
#define DISABLE_ALL     (0)

/* HIPRIS_ARB */
#define MDINF       (1 << 0)
#define C2KINF          (1 << 1)
#define MD_DVFSINF      (1 << 2)
#define WACS1           (1 << 3)
#define WACS0           (1 << 4)
#define DVFSINF         (1 << 5)
#define WACS3           (1 << 6)
#define WACSMD32        (1 << 7)
#define DCXO_CONNINF    (1 << 8)
#define DCXO_NFCINF     (1 << 9)
#define MD_ADCINF       (1 << 10)
#define STAUPD          (1 << 11)
#define GPSINF          (1 << 12)
#define WACS2           (1 << 13)/* 0x2000 */
#define WACSP2P         (1 << 14)

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

/************* macro for spi clock config ******************************/
#define CLK_CFG_4_SET                       (TOPCKGEN_BASE+0x144)
#define CLK_CFG_4_CLR                       (TOPCKGEN_BASE+0x148)
#define CLK_CFG_6_CLR                       (TOPCKGEN_BASE+0x168)
#define CLK_SPI_CK_26M                       0xf3000000
#define MODULE_CLK_SEL                      (INFRACFG_AO_BASE+0x098)
#define MODULE_SW_CG_0_SET                  (INFRACFG_AO_BASE+0x080)
#define MODULE_SW_CG_0_CLR                  (INFRACFG_AO_BASE+0x084)
#define INFRA_GLOBALCON_RST2_SET            (INFRACFG_AO_BASE+0x140)
#define INFRA_GLOBALCON_RST2_CLR            (INFRACFG_AO_BASE+0x144)

#define GET_WACS2_FSM(x)             ((x>>16) & 0x00000007)
#define GET_WACS2_REQ(x)             ((x>>19) & 0x00000001)
#define GET_WACS2_SYNC_IDLE2(x)      ((x>>20) & 0x00000001)
#define GET_WACS2_INIT_DONE2(x)      ((x>>21) & 0x00000001)
#define GET_WACS2_RDATA(x)           ((x>>0)  & 0x0000ffff)
#define GET_WRAP_FSM(x)              ((x>>11) & 0x0000000f)
#define GET_WRAP_AG_DLE_RESTCNT(x)   ((x>>5)  & 0x00000003)
#define GET_WRAP_CH_DLE_RESTCNT(x)   ((x>>0)  & 0x00000007)
#define GET_MAN_REQ(x)               ((x>>11) & 0x00000001)
#define GET_MAN_FSM(x)               ((x>>8)  & 0x00000007)
#define GET_STAUPD_FSM(x)            ((x>>26) & 0x00000007)

#endif /*__PMIC_WRAP_INIT_H__*/
