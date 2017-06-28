/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2010. All rights reserved.
*
* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
* THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
* AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
* NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
* SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
* SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
* THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* The following software/firmware and/or related documentation ("MediaTek Software")
* have been modified by MediaTek Inc. All revisions are subject to any receiver's
* applicable license agreements with MediaTek Inc.
*/

#ifndef __MT_LOW_POWER_H__
#define __MT_LOW_POWER_H__

#include <inc/register_base.h>
#include <sys/types.h>

#define reg_read(addr)         *(volatile unsigned int *)addr
#define reg_write(addr, val)   *(volatile unsigned int *)addr = val

void close_cpus();

//#define TOPCKGEN_BASE           0x10000000
#define DRAMC_BASE              0x10004000


#define DRAMC_PD_CTRL                   (DRAMC_BASE + 0x1DC)
#define DFS_DRAMC_PD_CTRL               (DRAMC_BASE + 0x828)
#define INFRACFG_TOP_CKMUXSEL           (INFRACFG_AO_BASE +  0x0)
#define INFRACFG_TOP_CKDIV1             (INFRACFG_AO_BASE +  0x8) 
#define INFRACFG_TOP_DCMCTL             (INFRACFG_AO_BASE +  0x10)
#define INFRACFG_TOP_DCMDBC             (INFRACFG_AO_BASE +  0x14)
#define INFRACFG_DCMCTL                 (INFRACFG_AO_BASE +  0x50)
#define INFRACFG_INFRA_BUS_DCM_CTRL           (INFRACFG_AO_BASE +  0x70)
#define INFRACFG_PERI_BUS_DCM_CTRL      (INFRACFG_AO_BASE +  0x74)
#define INFRACFG_MEM_DCM_CTRL           (INFRACFG_AO_BASE +  0x78)
#define INFRACFG_DFS_MEM_DCM_CTRL       (INFRACFG_AO_BASE +  0x7C)

#define	TOPCKGEN_CLK_MISC_CFG_0         (TOPCKGEN_BASE +  0x0104) 

#define MCUCFG_L2C_SRAM_CTRL            (MCUCFG_BASE + 0x648)
#define MCUCFG_CCI_CLK_CTRL             (MCUCFG_BASE + 0x660)
#define MCUCFG_BUS_FABRIC_DCM_CTRL      (MCUCFG_BASE + 0x668)


#define and(v, a) ((v) & (a))
#define or(v, o) ((v) | (o))
#define aor(v, a, o) (((v) & (a)) | (o))


//Reduce CPU freq
#define reduce_CPU_freq(addr, freq)	reg_write(addr, freq)
#define DCM_OFF (0)
#define DCM_ON (1)

//Releated MD
#define sync_write32(v, a)			do { *((unsigned int *)a) = (unsigned int)v; } while(0)
#define sync_write16(v, a)			do { *((unsigned short *)a) = (unsigned short)v; } while(0)
#define sync_write8(v, a)			do { *((unsigned char *)a) = (unsigned char)v; } while(0)
#define ioread32(a)					(*(unsigned int *)a)
#define ioread16(a)					(*(unsigned short *)a)
#define ioread8(a)					(*(unsigned int *)a)
#define MD_TOPSM_BASE						(0x20030000)
//ning add begin
#define MD2_TOPSM_BASE						(0x30030000)
//ning add end
#define MD_TOPSM_RM_TMR_PWR0(base)			((base)+0x0018)
#define MD_TOPSM_RM_PWR_CON0(base)    	    ((base)+0x0800)
#define MD_TOPSM_RM_PWR_CON1(base)    	    ((base)+0x0804)
#define MD_TOPSM_RM_PWR_CON2(base)    	    ((base)+0x0808)
#define MD_TOPSM_RM_PWR_CON3(base)    	    ((base)+0x080C)
#define MD_TOPSM_RM_PLL_MASK0(base)    	    ((base)+0x0830)
#define MD_TOPSM_RM_PLL_MASK1(base)    	    ((base)+0x0834)
#define MD_TOPSM_SM_REQ_MASK(base)     	    ((base)+0x08B0)
#define MODEM_LITE_TOPSM_BASE 		    	(0x23010000)
//ning add begin
#define MODEM2_LITE_TOPSM_BASE 		    	(0x33010000)
//ning add end

#define MODEM_LITE_TOPSM_RM_TMR_PWR0(base)	((base)+0x0018)
#define MODEM_LITE_TOPSM_RM_TMR_PWR1(base)	((base)+0x001C)
#define MODEM_LITE_TOPSM_RM_PWR_CON0(base)	((base)+0x0800)
#define MODEM_LITE_TOPSM_RM_PLL_MASK0(base)	((base)+0x0830)
#define MODEM_LITE_TOPSM_RM_PLL_MASK1(base)	((base)+0x0834)
#define MODEM_LITE_TOPSM_SM_REQ_MASK(base)	((base)+0x08B0)

#define MODEM_TOPSM_BASE 					(0x27010000)
//ning add begin
#define MODEM2_TOPSM_BASE 					(0x37010000)
//ning add end
#define MODEM_TOPSM_RM_TMR_PWR0(base)		((base)+0x0018)
#define MODEM_TOPSM_RM_TMR_PWR1(base)		((base)+0x001C)
#define MODEM_TOPSM_RM_PWR_CON1(base)		((base)+0x0804)
#define MODEM_TOPSM_RM_PWR_CON2(base)		((base)+0x0808)
#define MODEM_TOPSM_RM_PWR_CON3(base)		((base)+0x080C)
#define MODEM_TOPSM_RM_PWR_CON4(base)		((base)+0x0810)
#define MODEM_TOPSM_RM_PLL_MASK0(base)		((base)+0x0830)
#define MODEM_TOPSM_RM_PLL_MASK1(base)		((base)+0x0834)
#define MODEM_TOPSM_SM_REQ_MASK(base)       ((base)+0x08B0)

#define TDD_BASE						(0x24000000)
#define TDD_HALT_CFG_ADDR(base)			((base)+0x00000000)
#define TDD_HALT_STATUS_ADDR(base)		((base)+0x00000002)

#define LTEL1_BASE						(0x26600000)

#define MD_PLL_MIXEDSYS_BASE    		(0x20120000)
#define PLL_PLL_CON2_1(base)     		((base) +0x0024)
#define PLL_PLL_CON4(base)				((base) +0x0050)
#define PLL_CLKSW_CKSEL4(base)       	((base) +0x0094)
#define PLL_CLKSW_CKSEL6(base)    		((base) +0x009C)
#define PLL_DFS_CON7(base)				((base) +0x00AC)
#define PLL_MDPLL_CON0(base)			((base) +0x0100)
#define PLL_ARM7PLL_CON0(base)			((base) +0x0150)
#define PLL_ARM7PLL_CON1(base)			((base) +0x0154)

//MD
//#define MD1_PROT_MASK        0x04B8//bit 3,4,5,7,10
#define MD2_PROT_MASK        0xF0000//16,17,18,19
#define MD_SRAM_PDN         (0x1 << 8)
#define MD1_PWR_STA_MASK    (0x1 << 0)
#define MD2_PWR_STA_MASK    (0x1 << 27)
#define spm_read(addr)			(*(volatile uint32 *)(addr))


#define TOPAXI_PROT_EN (INFRACFG_AO_BASE + 0x0220)
#define TOPAXI_PROT_STA1 (INFRACFG_AO_BASE + 0x0228)

#define PWR_RST_B (0x1 << 0)
#define PWR_ISO (0x1 << 1)
#define PWR_ON (0x1 << 2)
#define PWR_ON_2ND (0x1 << 3)
#define PWR_CLK_DIS (0x1 << 4)
#define SPM_MD_PWR_CON			(SPM_BASE + 0x284)
#define SPM_PWR_STATUS			(SPM_BASE + 0x60c)
#define SPM_PWR_STATUS_2ND		(SPM_BASE + 0x610)
#define SPM_MD2_PWR_CON (SPM_BASE + 0x2d0)
#define STA_POWER_DOWN 0

#define PMIC_ADDR_VGPU_EN 0x0612 /* [0] */
#define CPU_FREQ_ADDR	0x10001008
#define CLKDIV_ADDR	 0x10001008
#define CPU_CLK_SwitcherREG_ADDR	 0x10001000
#define CPU_Clock_ARMPLL	 0x01
#define CPU_Clock_26MHz	 0x00
#define CPU_CLKDIV_FREQ_MASK	0XFFFFFFE0//0x10001008[4:0] 
#define CPU_CLK_SwitcherREG_MASK	0XFFFFFFFC//0x10001000[1:0] 
#define CPU_FREQ_VALUE_585	0b11011 // 585MHz
#define CPU_FREQ_VALUE_default	0b10000 // default value(1.17GHz)	

#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")


extern void spm_write(unsigned int addr, unsigned int val);
extern void close_cpus(void);
extern void GPU_Core1_power_down(void);
extern void internal_md_power_down(void);
extern void internal_md2_power_down(void);



#endif 
