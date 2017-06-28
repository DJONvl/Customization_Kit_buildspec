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

#ifndef __MT_GPT_H__
#define __MT_GPT_H__
#include <inc/register_base.h>
#include <sys/types.h>

enum {
	RGU_REG_CLR = 0,
	RGU_REG_SET = 1
};

/*WDT*/
#define WDT_BASE				(TOPRGU_BASE) 							//Sheet InfrasysAPB_AO, line 10, RGU includes WDT
#define WDT_MODE				((volatile uint16 *)(WDT_BASE + 0x0000))
#define WDT_LENGTH				((volatile uint16 *)(WDT_BASE + 0x0004))
#define WDT_RESTART				((volatile uint16 *)(WDT_BASE + 0x0008))
#define WDT_SWRST               ((volatile uint16 *)(WDT_BASE + 0x0014))

#define WDT_FLAG_REBOOT     				0x0001  //!<: 1:re-boot; 0:auto-boot
#define WDT_FLAG_ASYNC      				0x0002  //!<: 1:WDT is executed asynchronously; 0: WDT is executed synchronously
#define WDT_FLAG_NOTE_PRELD_KEEP_USB        0x0004  //!<: 1:Note preloader keep usb forever after system reset; 0: Note preloader enum usb with timeout.
#define WDT_FLAG_NOT_RELEASE_PWRKEY         0x0008 //!<:1: not reset RTC time; 0: reset RTC time if have formated flash before enable watchdog

#define WDT_AUTO_RESTART 		(0x0001 << 4)
#define WDT_AUTO_INDIS 			(0x0001 << 5)

#define MTK_WDT_MODE			(WDT_BASE+0x0000)
#define MTK_WDT_LENGTH			(WDT_BASE+0x0004)
#define MTK_WDT_RESTART			(WDT_BASE+0x0008)
#define MTK_WDT_STATUS			(WDT_BASE+0x000C)
#define MTK_WDT_INTERVAL		(WDT_BASE+0x0010)
#define MTK_WDT_SWRST			(WDT_BASE+0x0014)
#define MTK_WDT_SWSYSRST		(WDT_BASE+0x0018)
#define MTK_WDT_NONRST_REG		(WDT_BASE+0x0020)
#define MTK_WDT_NONRST_REG2		(WDT_BASE+0x0024)
#define MTK_WDT_REQ_MODE		(WDT_BASE+0x0030)
#define MTK_WDT_REQ_IRQ_EN		(WDT_BASE+0x0034)
#define MTK_WDT_DEBUG_CTL		(WDT_BASE+0x0040)
#define MTK_WDT_LATCH_CTL		(WDT_BASE+0x0044)
#define MTK_WDT_LATCH_CTL2		(WDT_BASE+0x0048)
#define MTK_WDT_RSTDEG_EN1		(WDT_BASE+0x0080)
#define MTK_WDT_RSTDEG_EN2		(WDT_BASE+0x0084)
#define MTK_WDT_RESET_PROTECT	(WDT_BASE+0x0090)
#define MTK_WDT_DEBUG_1_REG		(WDT_BASE+0x0504)
#define MTK_WDT_DEBUG_2_REG		(WDT_BASE+0x0508)
#define MTK_WDT_DEBUG_5_REG		(WDT_BASE+0x0514)

/*WDT_MODE*/
#define MTK_WDT_MODE_KEYMASK		(0xff00)
#define MTK_WDT_MODE_KEY		(0x22000000)

#define MTK_WDT_MODE_DDR_RESERVE  (0x0080)
#define MTK_WDT_MODE_DUAL_MODE  (0x0040)
#define MTK_WDT_MODE_IN_DIS		(0x0020) /* Reserved */
#define MTK_WDT_MODE_AUTO_RESTART	(0x0010) /* Reserved */
#define MTK_WDT_MODE_IRQ		(0x0008)
#define MTK_WDT_MODE_EXTEN		(0x0004)
#define MTK_WDT_MODE_EXT_POL		(0x0002)
#define MTK_WDT_MODE_ENABLE		(0x0001)


/*WDT_LENGTH*/
#define MTK_WDT_LENGTH_TIME_OUT		(0xffe0)
#define MTK_WDT_LENGTH_KEYMASK		(0x001f)
#define MTK_WDT_LENGTH_KEY		(0x0008)
#define MTK_WDT_LENGTH_CTL_KEY		(0x95<<24)

/*WDT_RESTART*/
#define MTK_WDT_RESTART_KEY		(0x1971)

/*WDT_STATUS*/
#define MTK_WDT_STATUS_HWWDT_RST	(0x80000000)
#define MTK_WDT_STATUS_SWWDT_RST	(0x40000000)
#define MTK_WDT_STATUS_IRQWDT_RST	(0x20000000)
#define MTK_WDT_STATUS_DEBUGWDT_RST	(0x00080000)
#define MTK_WDT_STATUS_SPMWDT_RST	(0x0002)
#define MTK_WDT_STATUS_SPM_THERMAL_RST	(0x0001)
#define MTK_WDT_STATUS_THERMAL_DIRECT_RST	(1<<18)
#define MTK_WDT_STATUS_SECURITY_RST	(1<<28)
#define MTK_WDT_STATUS_EINT_RST		(1<<2)
#define MTK_WDT_STATUS_SYSRST_RST	(1<<3)
#define MTK_WDT_STATUS_DVFSP_RST	(1<<4)
#define MTK_WDT_STATUS_SSPM_RST	    (1<<16)
#define MTK_WDT_STATUS_MDDBG_RST	(1<<17)


/*WDT_RST_DEGLITCH*/
#define MTK_WDT_RSTDEG_EN1_KEY		(0xa357)
#define MTK_WDT_RSTDEG_EN2_KEY		(0x67d2)
#define MTK_WDT_RSTDEG_CLOCK_SELETC_SHIFT	(31)
#define MTK_WDT_RSTDEG_CLOCK_SELECT_MASK	(0x1 << MTK_WDT_RSTDEG_CLOCK_SELETC_SHIFT)
#define MTK_WDT_RSTDEG_CLOCK_26M			(0)
#define MTK_WDT_RSTDEG_CLOCK_32K			(1)
#define MTK_WDT_RSTDEG_LATENCY_SHIFT		(24)
#define MTK_WDT_RSTDEG_LATENCY_MASK		(0xF << MTK_WDT_RSTDEG_LATENCY_SHIFT)
#define MTK_WDT_RSTDEG_LATENCY_118US_93MS	(0x0)
#define MTK_WDT_RSTDEG_LATENCY_59US_46MS	(0x1)
#define MTK_WDT_RSTDEG_LATENCY_29US_23MS	(0x2)
#define MTK_WDT_RSTDEG_LATENCY_14US_11MS	(0x3)
#define MTK_WDT_RSTDEG_LATENCY_7US_5MS		(0x4)
#define MTK_WDT_RSTDEG_LATENCY_3US_2MS		(0x5)
#define MTK_WDT_RSTDEG_LATENCY_1US_1MS		(0x6)
#define MTK_WDT_RSTDEG_LATENCY_923NS_732US	(0x7)
#define MTK_WDT_RSTDEG_LATENCY_461NS_366US	(0x8)
#define MTK_WDT_RSTDEG_LATENCY_230NS_183US	(0x9)
#define MTK_WDT_RSTDEG_LATENCY_115NS_91US	(0xA)

/*WDT_INTERVAL*/
#define MTK_WDT_INTERVAL_MASK		(0x0fff)

/*WDT_SWRST*/
#define MTK_WDT_SWRST_KEY		(0x1209)

/*WDT_SWSYSRST*/
#define MTK_WDT_SWSYS_RST_PWRAP_SPI_CTL_RST	(0x0800)
#define MTK_WDT_SWSYS_RST_APMIXED_RST	(0x0400)
#define MTK_WDT_SWSYS_RST_MD_LITE_RST	(0x0200)
#define MTK_WDT_SWSYS_RST_INFRA_AO_RST	(0x0100)
#define MTK_WDT_SWSYS_RST_MD_RST	(0x0080)
#define MTK_WDT_SWSYS_RST_DDRPHY_RST	(0x0040)
#define MTK_WDT_SWSYS_RST_IMG_RST	(0x0020)
#define MTK_WDT_SWSYS_RST_VDEC_RST	(0x0010)
#define MTK_WDT_SWSYS_RST_VENC_RST	(0x0008)
#define MTK_WDT_SWSYS_RST_MFG_RST	(0x0004)
#define MTK_WDT_SWSYS_RST_DISP_RST	(0x0002)
#define MTK_WDT_SWSYS_RST_INFRA_RST	(0x0001)


//#define MTK_WDT_SWSYS_RST_KEY		(0x1500)
#define MTK_WDT_SWSYS_RST_KEY		(0x88000000)
#define MTK_WDT_REQ_IRQ_KEY		(0x44000000)

#define MTK_WDT_REQ_MODE_KEY    (0x33000000)

/* WDT_NONRST_REG */
#define MTK_WDT_NONRST_DL               (0x00008000)

//MTK_WDT_DEBUG_CTL
#define MTK_DEBUG_CTL_KEY           (0x59000000)
#define MTK_RGU_EMI_DCS_EN             (0x00000002) /* bit 1: emi_dcs_en */
#define MTK_RGU_DVFSRC_EN              (0x00000004) /* bit 2: dvfsrc_en */
#define MTK_RG_DRAMC_SREF           (0x00100)
#define MTK_RG_DRAMC_ISO            (0x00200)
#define MTK_RG_CONF_ISO             (0x00400)
#define MTK_DDR_RESERVE_RTA         (0x10000)  /* bit 16: ddr_reserve_success */
#define MTK_DDR_SREF_STA            (0x20000)  /* bit 17: ddr_sref_status */
#define MTK_RGU_EMI_DCS_SUCCESS     (0x40000)  /* bit 18: emi_dcs_success */
#define MTK_RGU_DVFSRC_SUCCESS      (0x100000) /* bit 20: dvfsrc_success */

//MTK_WDT_LATCH_CTL2
#define MTK_LATCH_CTL_KEY2          (0x95000000)
#define MTK_RG_MCU_DFD_TIMEOUT      (0x30)
#define MTK_RG_MCU_DFD_EN           (1<<7)
#define MTK_RG_DVFSRC_LATCH_SELECT  (1<<20)
#define MTK_RG_EMI_LATCH_SELECT     (1<<21)
#define MTK_RG_MCU_LATCH_SELECT     (1<<22)
#define MTK_RG_SPM_LATCH_SELECT     (1<<23)

//MTK_WDT_LATCH_CTL
#define MTK_LATCH_CTL_KEY           (0x95000000)
#define MTK_RG_LATH_EN       	    (0x00001)
#define MTK_RG_MCU_LATH_EN   	    (0x00010)
#define MTK_RG_SPM_LATH_EN   	    (0x00020)
#define MTK_RG_DRAMC_LATH_EN   	    (0x00040)
#define MTK_RG_EMI_LATH_EN   	    (0x00080)
#define MTK_RG_DRAMC_RD_TEST_EN	(0x10000)
#define MTK_RG_DRAMC_RDWT_TEST_EN    (0x20000)

/* MTK_WDT_DEBUG_CTL3 */
#define MTK_WDT_DEBUG_CTL3		(WDT_BASE+0x00A4)
#define MTK_RGU_EMI_DRS_EN             (0x00000001) /* bit 1: emi_dcs_en */
#define MTK_RGU_EMI_DRS_SUCCESS_OFFSET (2)          /* bit 2: emi_dcs_success */
#define MTK_RGU_EMI_DRS_SUCCESS        (1 << MTK_RGU_EMI_DRS_SUCCESS_OFFSET)


/* WDT_RESET_PROTECT */
#define MTK_RESET_PROTECT_EN_SHIFT				(0)
#define MTK_RESET_PROTECT_EN_MASK				(0x1 << MTK_RESET_PROTECT_EN_SHIFT)
#define MTK_RESET_PROTECT_DIS					(0)
#define MTK_RESET_PROTECT_EN					(1)

#define MTK_RESET_PROTECT_TIME_SHIFT			(1)
#define MTK_RESET_PROTECT_TIME_MASK			(0x7 << MTK_RESET_PROTECT_TIME_SHIFT)
#define MTK_RESET_PROTECT_TIME_DEFAULT			(0)

#define MTK_RESET_PROTECT_CPU_EXT_ISO_EN_SHIFT	(4)
#define MTK_RESET_PROTECT_CPU_EXT_ISO_EN_MASK	(0x1 << MTK_RESET_PROTECT_CPU_EXT_ISO_EN_SHIFT)
#define MTK_RESET_PROTECT_CPU_EXT_ISO_DIS		(0)
#define MTK_RESET_PROTECT_CPU_EXT_ISO_EN		(1)

#define MTK_RESET_PROTECT_GPU_EXT_ISO_EN_SHIFT	(8)
#define MTK_RESET_PROTECT_GPU_EXT_ISO_EN_MASK	(0x1 << MTK_RESET_PROTECT_GPU_EXT_ISO_EN_SHIFT)
#define MTK_RESET_PROTECT_GPU_EXT_ISO_DIS		(0)
#define MTK_RESET_PROTECT_GPU_EXT_ISO_EN		(1)

#define MTK_RESET_PROTECT_MD_EXT_ISO_EN_SHIFT	(9)
#define MTK_RESET_PROTECT_MD_EXT_ISO_EN_MASK		(0x1 << MTK_RESET_PROTECT_MD_EXT_ISO_EN_SHIFT)
#define MTK_RESET_PROTECT_MD_EXT_ISO_DIS		(0)
#define MTK_RESET_PROTECT_MD_EXT_ISO_EN			(1)

#define MTK_RESET_PROTECT_CORE_EXT_ISO_EN_SHIFT	(10)
#define MTK_RESET_PROTECT_CORE_EXT_ISO_EN_MASK	(0x1 << MTK_RESET_PROTECT_CORE_EXT_ISO_EN_SHIFT)
#define MTK_RESET_PROTECT_CORE_EXT_ISO_DIS		(0)
#define MTK_RESET_PROTECT_CORE_EXT_ISO_EN		(1)

#define MTK_RESET_PROTECT_SOC0_ISO_EN_SHIFT		(16)
#define MTK_RESET_PROTECT_SOC0_ISO_EN_MASK		(0x1 << MTK_RESET_PROTECT_SOC0_ISO_EN_SHIFT)
#define MTK_RESET_PROTECT_SOC0_ISO_DIS			(0)
#define MTK_RESET_PROTECT_SOC0_ISO_EN			(1)

#define MTK_RESET_PROTECT_SOC1_ISO_EN_SHIFT		(17)
#define MTK_RESET_PROTECT_SOC1_ISO_EN_MASK		(0x1 << MTK_RESET_PROTECT_SOC1_ISO_EN_SHIFT)
#define MTK_RESET_PROTECT_SOC1_ISO_DIS			(0)
#define MTK_RESET_PROTECT_SOC1_ISO_EN			(1)

#define MTK_RESET_PROTECT_MEM_ISO_EN_SHIFT		(18)
#define MTK_RESET_PROTECT_MEM_ISO_EN_MASK		(0x1 << MTK_RESET_PROTECT_MEM_ISO_EN_SHIFT)
#define MTK_RESET_PROTECT_MEM_ISO_DIS			(0)
#define MTK_RESET_PROTECT_MEM_ISO_EN			(1)

#define MTK_RESET_PROTECT_DDR_ISO_EN_SHIFT		(19)
#define MTK_RESET_PROTECT_DDR_ISO_EN_MASK		(0x1 << MTK_RESET_PROTECT_DDR_ISO_EN_SHIFT)
#define MTK_RESET_PROTECT_DDR_ISO_DIS			(0)
#define MTK_RESET_PROTECT_DDR_ISO_EN			(1)

#define MTK_RESET_PROTECT_UNLOCK_KEY_SHIFT		(24)
#define MTK_RESET_PROTECT_UNLOCK_KEY_MASK		(0xFF << MTK_RESET_PROTECT_UNLOCK_KEY_SHIFT)
#define MTK_RESET_PROTECT_UNLOCK_KEY			(0x74)

#define MTK_RGU_DVFSRC_SUCCESS_OFFSET  (20)         /* bit 20: dvfsrc_success */
#define MTK_RGU_EMI_DCS_SUCCESS_OFFSET (18)         /* bit 18: emi_dcs_success */

extern void EnableWatchDog(uint16 interval_ms,uint32 flags) ;
extern void DisableWDT(void);
extern void rgu_release_rg_dramc_conf_iso(void);
extern void rgu_wait_for_reg_update_done(unsigned int reg_addr, unsigned mask, unsigned target_val);
extern void rgu_release_rg_dramc_iso(void);
extern void rgu_release_rg_dramc_sref(void);
extern int rgu_is_reserve_ddr_enabled(void);
extern int rgu_is_reserve_ddr_mode_success(void);
extern int rgu_is_dram_slf(void);
extern int rgu_dram_reserved(int enable);
extern int rgu_is_dvfsrc_success(void);
extern int rgu_is_emi_dcs_success(void);
extern int rgu_is_emi_dcs_enable(void);
extern int rgu_is_dvfsrc_enable(void);
extern int rgu_is_emi_drs_success(void);
extern int rgu_is_emi_drs_enable(void);
extern int rgu_cfg_emi_dcs_en(int enable);
extern int rgu_update_debug_ctl(U32 reg, u32 func, u32 bits);

#endif  
