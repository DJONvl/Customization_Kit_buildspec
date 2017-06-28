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
 * MediaTek Inc. (C) 2010. All rights reserved.
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

#ifndef PLL_H
//#include "custom_emi.h"
#define PLL_H

#define APMIXED_BASE		(0x10212000)
#define CKSYS_BASE		(0x10210000)
//#define INFRACFG_AO_BASE	(0x10000000)
//#define PERICFG_BASE	        (0x11010000)
#define MFGCFG_BASE		(0x13ffe000)
#define MMSYS_CONFIG_BASE	(0x14000000)
#define IMGSYS_BASE		(0x15020000)
#define VDEC_GCON_BASE		(0x16000000)
#define MJC_CONFIG_BASE		(0x12000000)
#define VENC_GCON_BASE		(0x17000000)
//#define MCUMIXED_BASE     (0x1001A000)//
#define MCUCFG_BASE		(0x10200000)
//#define SPM_BASE		(0x10b00000)
#define CA57ACFG_BASE           (0x10202000)
/* SPM Register*/ 
//#define POWERON_CONFIG_EN       (SPM_BASE + 0x0)

/* CA57A Register */
#define PWR_RST_CTL             (CA57ACFG_BASE + 0x08)


/* APMIXEDSYS Register */
#define AP_PLL_CON0             (APMIXED_BASE + 0x00)
#define AP_PLL_CON1             (APMIXED_BASE + 0x04)
#define AP_PLL_CON2             (APMIXED_BASE + 0x08)
#define AP_PLL_CON3             (APMIXED_BASE + 0x0C)
#define AP_PLL_CON4             (APMIXED_BASE + 0x10)
#define AP_PLL_CON5             (APMIXED_BASE + 0x14)
#define AP_PLL_CON6             (APMIXED_BASE + 0x18)
#define AP_PLL_CON7             (APMIXED_BASE + 0x1C)
#define AP_PLL_CON8             (APMIXED_BASE + 0x20)
#define CLKSQ_STB_CON0          (APMIXED_BASE + 0x24)
#define PLL_PWR_CON0            (APMIXED_BASE + 0x28)
#define PLL_PWR_CON1            (APMIXED_BASE + 0x2C)
#define PLL_ISO_CON0            (APMIXED_BASE + 0x30)
#define PLL_ISO_CON1            (APMIXED_BASE + 0x34)
#define PLL_STB_CON0            (APMIXED_BASE + 0x38)
#define DIV_STB_CON0            (APMIXED_BASE + 0x3C)
#define PLL_CHG_CON0            (APMIXED_BASE + 0x40)
#define PLL_TEST_CON0           (APMIXED_BASE + 0x44)

#define FSMIPLL_CON0            (APMIXED_BASE + 0x200)
#define FSMIPLL_CON1            (APMIXED_BASE + 0x204)
#define FSMIPLL_PWR_CON0        (APMIXED_BASE + 0x20C)

#define GPUPLL_CON0            (APMIXED_BASE + 0x210)
#define GPUPLL_CON1            (APMIXED_BASE + 0x214)
#define GPUPLL_PWR_CON0        (APMIXED_BASE + 0x21C)

#define MPLL_CON0               (APMIXED_BASE + 0x220)
#define MPLL_CON1               (APMIXED_BASE + 0x224)
#define MPLL_PWR_CON0           (APMIXED_BASE + 0x22C)

#define MAINPLL_CON0            (APMIXED_BASE + 0x230)
#define MAINPLL_CON1            (APMIXED_BASE + 0x234)
#define MAINPLL_PWR_CON0        (APMIXED_BASE + 0x23C)
                                                
#define UNIVPLL_CON0            (APMIXED_BASE + 0x240)
#define UNIVPLL_CON1            (APMIXED_BASE + 0x244)
#define UNIVPLL_PWR_CON0        (APMIXED_BASE + 0x24C)
                                                
#define MSDCPLL_CON0            (APMIXED_BASE + 0x250)
#define MSDCPLL_CON1            (APMIXED_BASE + 0x254)
#define MSDCPLL_PWR_CON0        (APMIXED_BASE + 0x25C)

#define MMPLL_CON0               (APMIXED_BASE + 0x260)
#define MMPLL_CON1               (APMIXED_BASE + 0x264)
#define MMPLL_PWR_CON0           (APMIXED_BASE + 0x26C)

#define VCODECPLL_CON0          (APMIXED_BASE + 0x270)
#define VCODECPLL_CON1          (APMIXED_BASE + 0x274)
#define VCODECPLL_PWR_CON0      (APMIXED_BASE + 0x27C)

#define TVDPLL_CON0             (APMIXED_BASE + 0x280)
#define TVDPLL_CON1             (APMIXED_BASE + 0x284)
#define TVDPLL_PWR_CON0         (APMIXED_BASE + 0x28C)
                                                
#define EMIPLL_CON0            (APMIXED_BASE + 0x290)
#define EMIPLL_CON1            (APMIXED_BASE + 0x294)
#define EMIPLL_PWR_CON0        (APMIXED_BASE + 0x29C)
                              
#define APLL1_CON0              (APMIXED_BASE + 0x2A0)
#define APLL1_CON1              (APMIXED_BASE + 0x2A4)
#define APLL1_CON2              (APMIXED_BASE + 0x2A8)
#define APLL1_CON5              (APMIXED_BASE + 0x2B4)
#define APLL1_PWR_CON0          (APMIXED_BASE + 0x2B8)
                                                
#define APLL2_CON0              (APMIXED_BASE + 0x2C0)
#define APLL2_CON1              (APMIXED_BASE + 0x2C4)
#define APLL2_CON2              (APMIXED_BASE + 0x2C8)
#define APLL2_CON5              (APMIXED_BASE + 0x2D4)
#define APLL2_PWR_CON0          (APMIXED_BASE + 0x2D8)


#define ARMPLL1_CON0		(APMIXED_BASE + 0x310)
#define ARMPLL1_CON1		(APMIXED_BASE + 0x314)
#define ARMPLL1_PWR_CON0	(APMIXED_BASE + 0x31C)

#define ARMPLL2_CON0		(APMIXED_BASE + 0x320)
#define ARMPLL2_CON1		(APMIXED_BASE + 0x324)
#define ARMPLL2_PWR_CON0	(APMIXED_BASE + 0x32C)

#define ARMPLL3_CON0		(APMIXED_BASE + 0x330)
#define ARMPLL3_CON1		(APMIXED_BASE + 0x334)
#define ARMPLL3_PWR_CON0	(APMIXED_BASE + 0x33C)

#define CCIPLL_CON0		(APMIXED_BASE + 0x350)
#define CCIPLL_CON1		(APMIXED_BASE + 0x354)
#define CCIPLL_PWR_CON0		(APMIXED_BASE + 0x35C)

/* MCUCFG Register */
#define MP0_PLL_DIV_CFG          (MCUCFG_BASE + 0x7A0) //ARMPLL_M
#define MP1_PLL_DIV_CFG          (MCUCFG_BASE + 0x7A4) //ARMPLL_L
#define MP2_PLL_DIV_CFG          (MCUCFG_BASE + 0x7A8) //ARMPLL_B
#define MP3_PLL_DIV_CFG          (MCUCFG_BASE + 0x7AC) //no used
#define BUS_PLL_DIV_CFG          (MCUCFG_BASE + 0x7C0) //CCIPLL

/* INFRASYS Register */
#define INFRA_SW_CG0_SET          (INFRACFG_AO_BASE + 0x80)
#define INFRA_SW_CG0_CLR          (INFRACFG_AO_BASE + 0x84)
#define INFRA_SW_CG1_SET          (INFRACFG_AO_BASE + 0x88)
#define INFRA_SW_CG1_CLR          (INFRACFG_AO_BASE + 0x8C)
#define INFRA_SW_CG0_STA          (INFRACFG_AO_BASE + 0x90)
#define INFRA_SW_CG1_STA          (INFRACFG_AO_BASE + 0x94)
#define INFRA_SW_CG2_SET          (INFRACFG_AO_BASE + 0xA4)
#define INFRA_SW_CG2_CLR          (INFRACFG_AO_BASE + 0xA8)
#define INFRA_SW_CG2_STA          (INFRACFG_AO_BASE + 0xAC)
#define INFRA_SW_CG3_SET          (INFRACFG_AO_BASE + 0xB0)
#define INFRA_SW_CG3_CLR          (INFRACFG_AO_BASE + 0xB4)
#define INFRA_SW_CG3_STA          (INFRACFG_AO_BASE + 0xB8)
#define INFRA_SW_CG4_SET          (INFRACFG_AO_BASE + 0xBC)
#define INFRA_SW_CG4_CLR          (INFRACFG_AO_BASE + 0xC0)
#define INFRA_SW_CG4_STA          (INFRACFG_AO_BASE + 0xC4)
#define INFRA_BUS_DCM_CTRL        (INFRACFG_AO_BASE + 0x70)

/* PERISYS Register */
#define PERI_SW_CG0_SET          (PERICFG_BASE + 0x270)
#define PERI_SW_CG0_CLR          (PERICFG_BASE + 0x274)
#define PERI_SW_CG0_STA          (PERICFG_BASE + 0x278)
#define PERI_SW_CG1_SET          (PERICFG_BASE + 0x280)
#define PERI_SW_CG1_CLR          (PERICFG_BASE + 0x284)
#define PERI_SW_CG1_STA          (PERICFG_BASE + 0x288)
#define PERI_SW_CG2_SET          (PERICFG_BASE + 0x290)
#define PERI_SW_CG2_CLR          (PERICFG_BASE + 0x294)
#define PERI_SW_CG2_STA          (PERICFG_BASE + 0x298)
#define PERI_SW_CG3_SET          (PERICFG_BASE + 0x2A0)
#define PERI_SW_CG3_CLR          (PERICFG_BASE + 0x2A4)
#define PERI_SW_CG3_STA          (PERICFG_BASE + 0x2A8)
#define PERI_SW_CG4_SET          (PERICFG_BASE + 0x2B0)
#define PERI_SW_CG4_CLR          (PERICFG_BASE + 0x2B4)
#define PERI_SW_CG4_STA          (PERICFG_BASE + 0x2B8)
#define PERI_SW_CG5_SET          (PERICFG_BASE + 0x2C0)
#define PERI_SW_CG5_CLR          (PERICFG_BASE + 0x2C4)
#define PERI_SW_CG5_STA          (PERICFG_BASE + 0x2C8)
#define PERI_BUS_DCM_CTRL_0      (PERICFG_BASE + 0x200)

/* MMSYS Register*/
#define MMSYS_CG_CON0            (MMSYS_CONFIG_BASE + 0x100)
#define MMSYS_CG_SET0            (MMSYS_CONFIG_BASE + 0x104)
#define MMSYS_CG_CLR0            (MMSYS_CONFIG_BASE + 0x108)
#define MMSYS_CG_CON1            (MMSYS_CONFIG_BASE + 0x110)
#define MMSYS_CG_SET1            (MMSYS_CONFIG_BASE + 0x114)
#define MMSYS_CG_CLR1            (MMSYS_CONFIG_BASE + 0x118)
#define MMSYS_CG_CON2            (MMSYS_CONFIG_BASE + 0x140)
#define MMSYS_CG_SET2            (MMSYS_CONFIG_BASE + 0x144)
#define MMSYS_CG_CLR2            (MMSYS_CONFIG_BASE + 0x148)

/* TOPCKGEN Register */
//#define CLK_MODE                (CKSYS_BASE + 0x000)
//#define CLK_CFG_UPDATE          (CKSYS_BASE + 0x004)
#define CLK_CFG_0               (CKSYS_BASE + 0x100)
#define CLK_CFG_1               (CKSYS_BASE + 0x110)
#define CLK_CFG_2               (CKSYS_BASE + 0x120)
#define CLK_CFG_3               (CKSYS_BASE + 0x130)
#define CLK_CFG_4               (CKSYS_BASE + 0x140)
#define CLK_CFG_5               (CKSYS_BASE + 0x150)
#define CLK_CFG_6               (CKSYS_BASE + 0x160)
#define CLK_CFG_7               (CKSYS_BASE + 0x170)
#define CLK_CFG_8               (CKSYS_BASE + 0x180)
#define CLK_CFG_9               (CKSYS_BASE + 0x190)
#define CLK_CFG_10               (CKSYS_BASE + 0x1A0)
#define CLK_CFG_11               (CKSYS_BASE + 0x1B0)
#define CLK_CFG_12               (CKSYS_BASE + 0x1C0)
#define CLK_CFG_13               (CKSYS_BASE + 0x1D0)
#define CLK_CFG_14               (CKSYS_BASE + 0x1E0)
#define CLK_CFG_20		 (CKSYS_BASE + 0x210)
#define CLK_CFG_21		 (CKSYS_BASE + 0x214)
#define CLK_MISC_CFG_1           (CKSYS_BASE + 0x414)
#define CLK_MISC_CFG_7           (CKSYS_BASE + 0x42C)
#define CLK26CALI_0             (CKSYS_BASE + 0x520)
#define CLK26CALI_1             (CKSYS_BASE + 0x524)
#define CLK26CALI_2             (CKSYS_BASE + 0x528)
#if 0
#define AP_AUXADC_CON0          (APMIXED_BASE + 0x400)
#define AP_AUXADC_CON1          (APMIXED_BASE + 0x404)
#define TS_CON0                 (APMIXED_BASE + 0x600)
#define TS_CON1                 (APMIXED_BASE + 0x604)
#define AP_ABIST_MON_CON0       (APMIXED_BASE + 0x800)
#define AP_ABIST_MON_CON1       (APMIXED_BASE + 0x804)
#define AP_ABIST_MON_CON2       (APMIXED_BASE + 0x808)
#define AP_ABIST_MON_CON3       (APMIXED_BASE + 0x80C)
#define OCCSCAN_CON             (APMIXED_BASE + 0x810)
#define CLKDIV_CON0             (APMIXED_BASE + 0x814)
#endif
#if 0
/* MCUMIXEDSYS Register */
#define MCU_PLL_CON0            (MCUMIXED_BASE + 0x000)
#define MCU_PLL_CON1            (MCUMIXED_BASE + 0x004)
#define MCU_PLL_CON2            (MCUMIXED_BASE + 0x008)

#define ARMCAXPLL0_CON0         (MCUMIXED_BASE + 0x200)
#define ARMCAXPLL0_CON1         (MCUMIXED_BASE + 0x204)
#define ARMCAXPLL0_CON2         (MCUMIXED_BASE + 0x208)
#define ARMCAXPLL0_PWR_CON0      (MCUMIXED_BASE + 0x20c)

#define ARMCAXPLL1_CON0         (MCUMIXED_BASE + 0x210)
#define ARMCAXPLL1_CON1         (MCUMIXED_BASE + 0x214)
#define ARMCAXPLL1_CON2         (MCUMIXED_BASE + 0x218)
#define ARMCAXPLL1_PWR_CON0      (MCUMIXED_BASE + 0x21c)

#define ARMCAXPLL2_CON0         (MCUMIXED_BASE + 0x220)
#define ARMCAXPLL2_CON1         (MCUMIXED_BASE + 0x224)
#define ARMCAXPLL2_CON2         (MCUMIXED_BASE + 0x228)
#define ARMCAXPLL2_PWR_CON0      (MCUMIXED_BASE + 0x22c)

#define ARMCAXPLL3_CON0         (MCUMIXED_BASE + 0x230)
#define ARMCAXPLL3_CON1         (MCUMIXED_BASE + 0x234)
#define ARMCAXPLL3_CON2         (MCUMIXED_BASE + 0x238)
#define ARMCAXPLL3_PWR_CON0     (MCUMIXED_BASE + 0x23c)

#define ARMPLLDIV_MUXSEL        (MCUMIXED_BASE + 0x270)
#define ARMPLLDIV_CKDIV         (MCUMIXED_BASE + 0x274)
#define ARMPLLDIV_MON_EN        (MCUMIXED_BASE + 0x284)

/* MCUCFG Register */
#define ACLKEN_DIV              (MCUCFG_BASE + 0x640)


#endif
#if 0
#define CLK_CFG_UPDATE1         (CKSYS_BASE + 0x008)
#define TST_SEL_0               (CKSYS_BASE + 0x020)
#define TST_SEL_1               (CKSYS_BASE + 0x024)
#define TST_SEL_2               (CKSYS_BASE + 0x028)
#endif

#if 0
#define CLK_MISC_CFG_0          (CKSYS_BASE + 0x104)
#define CLK_DBG_CFG             (CKSYS_BASE + 0x10C)
#define CLK_SCP_CFG_0           (CKSYS_BASE + 0x200)
#define CLK_SCP_CFG_1           (CKSYS_BASE + 0x204)
#define CLK26CALI_0             (CKSYS_BASE + 0x220)
#define CLK26CALI_1             (CKSYS_BASE + 0x224)
#define CKSTA_REG               (CKSYS_BASE + 0x22C)
#endif

/* INFRASYS Register */
#if 0
#define TOP_CKMUXSEL            (INFRACFG_AO_BASE + 0x00)
#define TOP_CKDIV1_BIG          (INFRACFG_AO_BASE + 0x24)
#define TOP_CKDIV1_SML          (INFRACFG_AO_BASE + 0x28)
#define TOP_CKDIV1_BUS          (INFRACFG_AO_BASE + 0x2C)
#endif
#if 0
#define TOP_CKDIV1              (INFRACFG_AO_BASE + 0x08)
#define TOP_DCMCTL              (INFRACFG_AO_BASE + 0x10)
#define INFRA_BUS_DCM_CTRL      (INFRACFG_AO_BASE + 0x70)
#define PERI_BUS_DCM_CTRL       (INFRACFG_AO_BASE + 0x74)
#define INFRA_SW_CG0_SET          (INFRACFG_AO_BASE + 0x80)
#define INFRA_SW_CG0_CLR          (INFRACFG_AO_BASE + 0x84)
#define INFRA_SW_CG1_SET          (INFRACFG_AO_BASE + 0x88)
#define INFRA_SW_CG1_CLR          (INFRACFG_AO_BASE + 0x8C)
#define INFRA_SW_CG0_STA          (INFRACFG_AO_BASE + 0x90)
#define INFRA_SW_CG1_STA          (INFRACFG_AO_BASE + 0x94)
#define INFRA_SW_CG2_SET          (INFRACFG_AO_BASE + 0xA8)
#define INFRA_SW_CG2_CLR          (INFRACFG_AO_BASE + 0xAC)
#define INFRA_SW_CG2_STA          (INFRACFG_AO_BASE + 0xB0)
#define TOPAXI_PROT_EN           (INFRACFG_AO_BASE + 0x0220)
#define TOPAXI_PROT_STA1         (INFRACFG_AO_BASE + 0x0228)
#define INFRA_TOPAXI_PROTECTEN   (INFRACFG_AO_BASE + 0x0220)
#define INFRA_TOPAXI_PROTECTSTA1 (INFRACFG_AO_BASE + 0x0228) 
#define INFRA_PLL_ULPOSC_CON0          (INFRACFG_AO_BASE + 0xB00)
#define INFRA_PLL_ULPOSC_CON1          (INFRACFG_AO_BASE + 0xB04)
#endif
/* MCUSS Register */
//#define ACLKEN_DIV              (0x10200640)
#if 0
/* Audio Register*/
#define AUDIO_TOP_CON0          (AUDIO_BASE + 0x0000)
                                
/* MFGCFG Register*/            
#define MFG_CG_CON              (MFGCFG_BASE + 0)
#define MFG_CG_SET              (MFGCFG_BASE + 4)
#define MFG_CG_CLR              (MFGCFG_BASE + 8)

/* MMSYS Register*/
#define MMSYS_CG_CON0            (MMSYS_CONFIG_BASE + 0x100)
#define MMSYS_CG_SET0            (MMSYS_CONFIG_BASE + 0x104)
#define MMSYS_CG_CLR0            (MMSYS_CONFIG_BASE + 0x108)
#define MMSYS_CG_CON1            (MMSYS_CONFIG_BASE + 0x110)
#define MMSYS_CG_SET1            (MMSYS_CONFIG_BASE + 0x114)
#define MMSYS_CG_CLR1            (MMSYS_CONFIG_BASE + 0x118)

/* IMGSYS Register */
#define IMG_CG_CON              (IMGSYS_BASE + 0x0000)
#define IMG_CG_SET              (IMGSYS_BASE + 0x0004)
#define IMG_CG_CLR              (IMGSYS_BASE + 0x0008)

/* VDEC Register */                                
#define VDEC_CKEN_SET           (VDEC_GCON_BASE + 0x0000)
#define VDEC_CKEN_CLR           (VDEC_GCON_BASE + 0x0004)
#define VDEC_LARB_CKEN1_SET      (VDEC_GCON_BASE + 0x0008)
#define VDEC_LARB_CKEN1_CLR     (VDEC_GCON_BASE + 0x000C)

/* MJC Register*/
#define MJC_CG_CON              (MJC_CONFIG_BASE + 0x0000)
#define MJC_CG_SET              (MJC_CONFIG_BASE + 0x0004)
#define MJC_CG_CLR              (MJC_CONFIG_BASE + 0x0008)

/* VENC Register*/
#define VENC_CG_CON             (VENC_GCON_BASE + 0x0)
#define VENC_CG_SET             (VENC_GCON_BASE + 0x4)
#define VENC_CG_CLR             (VENC_GCON_BASE + 0x8)


#define ULPOSC_CON              (SPM_BASE + 0x458)
#endif
void mt_pll_init(void);
unsigned int SAL_Query_UART_Clock(void);

//#define SMI_LARB0_BASE          (0x14010000)
//#define SMI_LARB0_STAT	        ((UINT32P)(SMI_LARB0_BASE+0x000))
//#define SMI_LARB0_OSTD_CTRL_EN	((UINT32P)(SMI_LARB0_BASE+0x064))


/* for MTCMOS */
/*
#define STA_POWER_DOWN  0
#define STA_POWER_ON    1

#define DIS_PWR_STA_MASK    (0x1 << 3)

#define PWR_RST_B           (0x1 << 0)
#define PWR_ISO             (0x1 << 1)
#define PWR_ON              (0x1 << 2)
#define PWR_ON_S            (0x1 << 3)
#define PWR_CLK_DIS         (0x1 << 4)

#define SRAM_PDN            (0xf << 8)
#define DIS_SRAM_ACK        (0x1 << 12)

#define MD1_PROT_MASK     0x04B8//bit 3,4,5,7,10
#define MD_SRAM_PDN         (0x1 << 8)
#define MD1_PWR_STA_MASK    (0x1 << 0)

int spm_mtcmos_ctrl_disp(int state);
*/
#endif
