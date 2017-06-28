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

/*WDT*/						
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

#define MTK_WDT_BASE			WDT_BASE
#define MTK_WDT_MODE			(MTK_WDT_BASE+0x0000)
#define MTK_WDT_LENGTH			(MTK_WDT_BASE+0x0004)
#define MTK_WDT_RESTART			(MTK_WDT_BASE+0x0008)
#define MTK_WDT_STATUS			(MTK_WDT_BASE+0x000C)
#define MTK_WDT_INTERVAL		(MTK_WDT_BASE+0x0010)
#define MTK_WDT_SWRST			(MTK_WDT_BASE+0x0014)
#define MTK_WDT_SWSYSRST		(MTK_WDT_BASE+0x0018)
#define MTK_WDT_NONRST_REG		(MTK_WDT_BASE+0x0020)
#define MTK_WDT_NONRST_REG2		(MTK_WDT_BASE+0x0024)
#define MTK_WDT_REQ_MODE		(MTK_WDT_BASE+0x0030)
#define MTK_WDT_REQ_IRQ_EN		(MTK_WDT_BASE+0x0034)
#define MTK_WDT_DEBUG_CTL		(MTK_WDT_BASE+0x0040)
#define MTK_WDT_LATCH_CTL		(MTK_WDT_BASE+0x0044)
#define MTK_WDT_RSTDEG_EN1		(MTK_WDT_BASE+0x0080)
#define MTK_WDT_RSTDEG_EN2		(MTK_WDT_BASE+0x0084)
#define MTK_WDT_DEBUG_2_REG		(MTK_WDT_BASE+0x0508)



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

/*WDT_RST_DEGLITCH*/
#define MTK_WDT_RSTDEG_EN1_KEY		(0xa357)
#define MTK_WDT_RSTDEG_EN2_KEY		(0x67d2)


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

/* WDT_NONRST_REG */
#define MTK_WDT_NONRST_DL               (0x00008000)

//MTK_WDT_DEBUG_CTL
#define MTK_DEBUG_CTL_KEY           (0x59000000)
#define MTK_RG_DDR_PROTECT_EN       (0x00001)
#define MTK_RG_DFD_COUNT_EN         (0x00002)
#define MTK_RG_DRAMC_SREF           (0x00100)
#define MTK_RG_DRAMC_ISO            (0x00200)
#define MTK_RG_CONF_ISO             (0x00400)
#define MTK_DDR_RESERVE_RTA         (0x10000)  //sta
#define MTK_DDR_SREF_STA            (0x20000)  //sta

//MTK_WDT_LATCH_CTL
#define MTK_LATCH_CTL_KEY           (0x95000000)
#define MTK_RG_LATH_EN       	    (0x00001)
#define MTK_RG_MCU_LATH_EN   	    (0x00002)
#define MTK_RG_SPM_LATH_EN   	    (0x00004)
#define MTK_RG_DRAMC_LATH_EN   	    (0x00010)
#define MTK_RG_DRAMC_RD_TEST_EN0	(0x10000)
#define MTK_RG_DRAMC_RD_TEST_EN1    (0x20000)
#define MTK_RG_DRAMC_WT_TEST_EN0	(0x40000)
#define MTK_RG_DRAMC_WT_TEST_EN1    (0x80000)
#define MTK_RG_DRAMC_LATCH_COUNT    (0x7FF<<5)


/* Reboot reason */
#define RE_BOOT_REASON_UNKNOW           (0x00)
#define RE_BOOT_BY_WDT_HW               (0x01)
#define RE_BOOT_BY_WDT_SW               (0x02)
#define RE_BOOT_WITH_INTTERUPT          (0x04)
#define RE_BOOT_BY_SPM_THERMAL          (0x08)
#define RE_BOOT_BY_SPM              	(0x10)
#define RE_BOOT_BY_THERMAL_DIRECT       (0x20)
#define RE_BOOT_BY_DEBUG        		(0x40)
#define RE_BOOT_BY_SECURITY        		(0x80)
#define RE_BOOT_BY_EINT	        	(0x100)
#define RE_BOOT_BY_SYSRST        	(0x200)



#define RE_BOOT_ABNORMAL                (0xF0)

/* Reboot from which stage */
#define RE_BOOT_FROM_UNKNOW             (0x00)
#define RE_BOOT_FROM_PRE_LOADER         (0x01)
#define RE_BOOT_FROM_U_BOOT             (0x02)
#define RE_BOOT_FROM_KERNEL             (0x03)
#define RE_BOOT_FROM_POWER_ON           (0x04)

#define WDT_NORMAL_REBOOT		(0x01)
#define WDT_BY_PASS_PWK_REBOOT		(0x02)
#define WDT_NOT_WDT_REBOOT		(0x00)

typedef enum wd_swsys_reset_type {
	WD_MD_RST,
}WD_SYS_RST_TYPE;


int rgu_is_reserve_ddr_enabled(void);

int rgu_is_dram_slf(void);

void rgu_release_rg_dramc_conf_iso(void);

void rgu_release_rg_dram_setting(void);

void rgu_release_rg_dramc_iso(void);

void rgu_release_rg_dramc_sref(void);
int rgu_is_reserve_ddr_mode_success(void);
void rgu_swsys_reset(WD_SYS_RST_TYPE reset_type);


extern void EnableWatchDog(uint16 interval_ms,uint32 flags) ;
extern void DisableWDT(void);


extern unsigned g_rgu_status;
#endif  
