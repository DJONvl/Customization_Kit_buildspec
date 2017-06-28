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
#define PLL_H

#include <inc/register_base.h>

/***********************/
/* APMIXEDSYS Register */
/***********************/
#define AP_PLL_CON0       	  (APMIXED_BASE + 0x0000)
#define AP_PLL_CON1       	  (APMIXED_BASE + 0x0004)
#define AP_PLL_CON2       	  (APMIXED_BASE + 0x0008)
#define AP_PLL_CON3       	  (APMIXED_BASE + 0x000C)
#define AP_PLL_CON4       	  (APMIXED_BASE + 0x0010)
#define PLL_HP_CON0       	  (APMIXED_BASE + 0x0014)
#define PLL_HP_CON1           (APMIXED_BASE + 0x0018)
#define CLKSQ_STB_CON0        (APMIXED_BASE + 0x001C)
#define PLL_PWR_CON0          (APMIXED_BASE + 0x0020)
#define PLL_PWR_CON1          (APMIXED_BASE + 0x0024)
#define PLL_PWR_CON2          (APMIXED_BASE + 0x0028)
#define PLL_PWR_CON3          (APMIXED_BASE + 0x002C)
#define PLL_ISO_CON0          (APMIXED_BASE + 0x0030)
#define PLL_ISO_CON1          (APMIXED_BASE + 0x0034)
#define PLL_ISO_CON2          (APMIXED_BASE + 0x0038)
#define PLL_ISO_CON3          (APMIXED_BASE + 0x003C)
#define PLL_EN_CON0           (APMIXED_BASE + 0x0040)
#define PLL_EN_CON1           (APMIXED_BASE + 0x0044)
#define PLL_STB_CON0          (APMIXED_BASE + 0x0048)
#define PLL_STB_CON1          (APMIXED_BASE + 0x004C)
#define PLL_STB_CON2          (APMIXED_BASE + 0x0050)
#define DIV_STB_CON0          (APMIXED_BASE + 0x0054)
#define PLL_CHG_CON0          (APMIXED_BASE + 0x0058)
#define PLL_CHG_CON1          (APMIXED_BASE + 0x005C)
#define PLL_TEST_CON0     	  (APMIXED_BASE + 0x0060)
#define PLL_TEST_CON1     	  (APMIXED_BASE + 0x0064)
#define PLL_INT_CON0          (APMIXED_BASE + 0x0068)
#define PLL_INT_CON1          (APMIXED_BASE + 0x006C)
#define ARMPLL_CON0       	  (APMIXED_BASE + 0x0100)
#define ARMPLL_CON1       	  (APMIXED_BASE + 0x0104)
#define ARMPLL_CON2       	  (APMIXED_BASE + 0x0108)
#define ARMPLL_CON3       	  (APMIXED_BASE + 0x010C)
#define ARMPLL_PWR_CON0   	  (APMIXED_BASE + 0x0110)
#define MAINPLL_CON0      	  (APMIXED_BASE + 0x0120)
#define MAINPLL_CON1      	  (APMIXED_BASE + 0x0124)
#define MAINPLL_CON2      	  (APMIXED_BASE + 0x0128)
#define MAINPLL_CON3      	  (APMIXED_BASE + 0x012C)
#define MAINPLL_PWR_CON0  	  (APMIXED_BASE + 0x0130)
#define UNIVPLL_CON0      	  (APMIXED_BASE + 0x0140)
#define UNIVPLL_CON1      	  (APMIXED_BASE + 0x0144)
#define UNIVPLL_CON2      	  (APMIXED_BASE + 0x0148)
#define UNIVPLL_CON3      	  (APMIXED_BASE + 0x014C)
#define UNIVPLL_PWR_CON0  	  (APMIXED_BASE + 0x0150)
#define MCUPLL_CON0       	  (APMIXED_BASE + 0x0220)
#define MCUPLL_CON1       	  (APMIXED_BASE + 0x0224)
#define MCUPLL_CON2       	  (APMIXED_BASE + 0x0228)
#define MCUPLL_CON3       	  (APMIXED_BASE + 0x022C)
#define MCUPLL_PWR_CON0   	  (APMIXED_BASE + 0x0230)
#define MCUPLL_PATHSEL_CON	  (APMIXED_BASE + 0x0234)
#define WHPLL_CON0        	  (APMIXED_BASE + 0x0240)
#define WHPLL_CON1        	  (APMIXED_BASE + 0x0244)
#define WHPLL_CON2        	  (APMIXED_BASE + 0x0248)
#define WHPLL_CON3        	  (APMIXED_BASE + 0x024C)
#define WHPLL_PWR_CON0    	  (APMIXED_BASE + 0x0250)
#define WHPLL_PATHSEL_CON 	  (APMIXED_BASE + 0x0254)
#define WPLL_CON0         	  (APMIXED_BASE + 0x0260)
#define WPLL_CON1         	  (APMIXED_BASE + 0x0264)
#define WPLL_CON2         	  (APMIXED_BASE + 0x0268)
#define WPLL_CON3         	  (APMIXED_BASE + 0x026C)
#define WPLL_PWR_CON0     	  (APMIXED_BASE + 0x0270)
#define WPLL_PATHSEL_CON  	  (APMIXED_BASE + 0x0274)
#define AP_AUXADC_CON0    	  (APMIXED_BASE + 0x0400)
#define AP_AUXADC_CON1    	  (APMIXED_BASE + 0x0404)
#define AP_AUXADC_CON2    	  (APMIXED_BASE + 0x0408)
#define TS_CON0           	  (APMIXED_BASE + 0x0600)
#define TS_CON1           	  (APMIXED_BASE + 0x0604)
#define AP_ABIST_MON_CON0 	  (APMIXED_BASE + 0x0E00)
#define AP_ABIST_MON_CON1 	  (APMIXED_BASE + 0x0E04)
#define AP_ABIST_MON_CON2 	  (APMIXED_BASE + 0x0E08)
#define AP_ABIST_MON_CON3 	  (APMIXED_BASE + 0x0E0C)
#define OCCSCAN_CON0      	  (APMIXED_BASE + 0x0E1C)
#define CLKDIV_CON0			  (APMIXED_BASE + 0x0E20)
#define RSV_RW0_CON0          (APMIXED_BASE + 0x0F00) 
#define RSV_RW0_CON1          (APMIXED_BASE + 0x0F04)
#define RSV_RW1_CON0          (APMIXED_BASE + 0x0F08)
#define RSV_RW1_CON1          (APMIXED_BASE + 0x0F0C)
#define RSV_RO_CON0           (APMIXED_BASE + 0x0F10)
#define RSV_RO_CON1           (APMIXED_BASE + 0x0F14)
#define RSV_ATPG_CON0         (APMIXED_BASE + 0x0F18)
/***********************/
/* TOPCKGEN Register */
/***********************/
#define CLK_MUX_SEL0        	(CKSYS_BASE + 0x000)
#define CLK_MUX_SEL1        	(CKSYS_BASE + 0x004)
#define TOPBUS_DCMCTL       	(CKSYS_BASE + 0x008)
#define TOPEMI_DCMCTL       	(CKSYS_BASE + 0x00C)
#define FREQ_MTR_CTRL       	(CKSYS_BASE + 0x010)
#define CLK_MUX_SEL7			(CKSYS_BASE + 0x1C)
#define CLK_GATING_CTRL0    	(CKSYS_BASE + 0x020)
#define CLK_GATING_CTRL1    	(CKSYS_BASE + 0x024)
#define INFRABUS_DCMCTL0    	(CKSYS_BASE + 0x028)
#define INFRABUS_DCMCTL1    	(CKSYS_BASE + 0x02C)
#define MPLL_FREDIV_EN      	(CKSYS_BASE + 0x030)
#define UPLL_FREDIV_EN      	(CKSYS_BASE + 0x034)
#define TEST_DBG_CTRL       	(CKSYS_BASE + 0x038)
#define CLK_GATING_CTRL2    	(CKSYS_BASE + 0x03C)
#define SET_CLK_GATING_CTRL0	(CKSYS_BASE + 0x050)
#define SET_CLK_GATING_CTRL1	(CKSYS_BASE + 0x054)
#define SET_INFRABUS_DCMCTL0	(CKSYS_BASE + 0x058)
#define SET_INFRABUS_DCMCTL1	(CKSYS_BASE + 0x05C)
#define SET_MPLL_FREDIV_EN  	(CKSYS_BASE + 0x060)
#define SET_UPLL_FREDIV_EN  	(CKSYS_BASE + 0x064)
#define SET_TEST_DBG_CTRL   	(CKSYS_BASE + 0x068)
#define SET_CLK_GATING_CTRL2	(CKSYS_BASE + 0x06C)
#define CLR_CLK_GATING_CTRL0	(CKSYS_BASE + 0x080)
#define CLR_CLK_GATING_CTRL1	(CKSYS_BASE + 0x084)
#define CLR_INFRABUS_DCMCTL0	(CKSYS_BASE + 0x088)
#define CLR_INFRABUS_DCMCTL1	(CKSYS_BASE + 0x08C)
#define CLR_MPLL_FREDIV_EN  	(CKSYS_BASE + 0x090)
#define CLR_UPLL_FREDIV_EN  	(CKSYS_BASE + 0x094)
#define CLR_TEST_DBG_CTRL   	(CKSYS_BASE + 0x098)
#define CLR_CLK_GATING_CTRL2	(CKSYS_BASE + 0x09C)
#define LPM_CTRL            	(CKSYS_BASE + 0x100)
#define LPM_TOTAL_TIME      	(CKSYS_BASE + 0x104)
#define LPM_LOW2HIGH_COUNT  	(CKSYS_BASE + 0x108)
#define LPM_HIGH_DUR_TIME   	(CKSYS_BASE + 0x10C)
#define LPM_LONGEST_HIGHTIME	(CKSYS_BASE + 0x110)
#define LPM_GOODDUR_COUNT   	(CKSYS_BASE + 0x114)
#define CLK_GATING_CTRL0_SEN	(CKSYS_BASE + 0x220)
#define CLK_GATING_CTRL1_SEN	(CKSYS_BASE + 0x224)
#define CLK_GATING_CTRL2_SEN	(CKSYS_BASE + 0x23C)
/***********************/
/* INFRASYS Register */
/***********************/
#define TOP_CKMUXSEL            (INFRACFG_AO_BASE + 0x000)
#define TOP_CKDIV1              (INFRACFG_AO_BASE + 0x008)
#define INFRA_PDN_SET           (INFRACFG_AO_BASE + 0x030)
#define INFRA_PDN_CLR           (INFRACFG_AO_BASE + 0x034)
#define INFRA_PDN_STA           (INFRACFG_AO_BASE + 0x038)
#define TOPAXI_PROT_EN          (INFRACFG_AO_BASE + 0x220)
#define TOPAXI_PROT_STA0        (INFRACFG_AO_BASE + 0x224)
#define TOPAXI_PROT_STA1        (INFRACFG_AO_BASE + 0x228)
#define PMIC_WRAP_SEL	        (INFRACFG_AO_BASE + 0x520)
#define INFRA_RSVD1             (INFRACFG_AO_BASE + 0xA00)
/***********************/
/* Audio Register*/
/***********************/
#define AUDIO_TOP_CON0		(AUDIO_BASE + 0x0)
/***********************/
/* MFGCFG Register*/            
/***********************/
#define MFG_CG_CON		(MFGCFG_BASE + 0x0)
#define MFG_CG_SET				(MFGCFG_BASE + 0x4)
#define MFG_CG_CLR				(MFGCFG_BASE + 0x8)
/***********************/
/* MMSYS Register*/
/***********************/
#define MMSYS_CG_CON0            (MMSYS_CONFIG_BASE + 0x100)
#define MMSYS_CG_SET0		(MMSYS_CONFIG_BASE + 0x104)
#define MMSYS_CG_CLR0		(MMSYS_CONFIG_BASE + 0x108)
#define MMSYS_CG_CON1            (MMSYS_CONFIG_BASE + 0x110)
#define MMSYS_CG_SET1		(MMSYS_CONFIG_BASE + 0x114)
#define MMSYS_CG_CLR1		(MMSYS_CONFIG_BASE + 0x118)
/***********************/
/* IMGSYS Register */
/***********************/
#define IMG_CG_CON		(IMGSYS_BASE + 0x0)
#define IMG_CG_SET		(IMGSYS_BASE + 0x4)
#define IMG_CG_CLR		(IMGSYS_BASE + 0x8)
#if 0
#define DRAMC_ASYNC
#endif
#define ACLKEN_DIV          	(MCUSYS_CONFIG_BASE+0x060)
#define PCLKEN_DIV          	(MCUSYS_CONFIG_BASE+0x064)

 //#define DDRPHY_3PLL_MODE
 //#define DDRPHY_2PLL  //define DDRPHY_2PLL needs to define DDRPHY_3PLL_MODE also.
// #ifdef DDRPHY_3PLL_MODE
 // 2PLL/3PLL uses physync mode.
 //#define PHYSYNC_MODE
 //#endif

#define MEMPLL_CLK_533

#ifdef  MEMPLL_CLK_266 
#define DDR_533
#endif
#ifdef  MEMPLL_CLK_400
#define DDR_800
#endif
#ifdef  MEMPLL_CLK_667
#define DDR_1333
#endif
#ifdef  MEMPLL_CLK_733
#define DDR_1466
#endif
#ifdef  MEMPLL_CLK_800
#define DDR_1600
#endif
#ifdef MEMPLL_CLK_533
#define DDR_1066
#endif

enum {
    PLL_MODE_1  = 1,
    PLL_MODE_2  = 2,
    PLL_MODE_3  = 3,
};
enum {
    DDR533   = 533,
    DDR800   = 800,
    DDR900   = 900,
    DDR938   = 938,
    DDR1066  = 1066,
    DDR1333  = 1333,
    DDR1466  = 1466,
};

extern void mt_pll_init();
extern uint32 SAL_Query_UART_Clock(void);
#endif
