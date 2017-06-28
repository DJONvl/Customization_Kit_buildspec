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

#define WDT_FLAG_REBOOT     				0x0001  //!<: 1:re-boot; 0:auto-boot
#define WDT_FLAG_ASYNC      				0x0002  //!<: 1:WDT is executed asynchronously; 0: WDT is executed synchronously
#define WDT_FLAG_NOTE_PRELD_KEEP_USB        0x0004  //!<: 1:Note preloader keep usb forever after system reset; 0: Note preloader enum usb with timeout.
#define WDT_FLAG_NOT_RELEASE_PWRKEY         0x0008 //!<:1: not reset RTC time; 0: reset RTC time if have formated flash before enable watchdog



/*WDT*/
#define MTK_WDT_BASE				(0x10007000) 							//Sheet InfrasysAPB_AO, line 10, RGU includes WDT
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
/*WDT_RESTART*/
#define MTK_WDT_RESTART_KEY		(0x1971)
#define MTK_DEBUG_CTL_KEY           (0x59000000)
#define MTK_RG_DRAMC_SREF           (0x00100)
#define WDT_FLAG_ASYNC      				0x0002  //!<: 1:WDT is executed asynchronously; 0: WDT is executed synchronously
#define MTK_RG_DRAMC_ISO            (0x00200)
#define MTK_RG_CONF_ISO             (0x00400)
#define MTK_DDR_RESERVE_RTA         (0x10000)  //sta
#define MTK_DDR_SREF_STA            (0x20000)  //sta
#define WDT_FLAG_NOTE_PRELD_KEEP_USB        0x0004  //!<: 1:Note preloader keep usb forever after system reset; 0: Note preloader enum usb with timeout.
#define WDT_FLAG_NOT_RELEASE_PWRKEY         0x0008 //!<:1: not reset RTC time; 0: reset RTC time if have formated flash before enable watchdog

#define WDT_AUTO_RESTART 		(0x0001 << 4)
#define WDT_AUTO_INDIS 			(0x0001 << 5)


extern void EnableWatchDog(uint16 interval_ms,uint32 flags) ;
extern void DisableWDT(void);
extern void mtk_wdt_restart(void);

extern void rgu_release_rg_dramc_conf_iso(void);

extern void rgu_release_rg_dram_setting(void);

extern void rgu_release_rg_dramc_iso(void);

extern void rgu_release_rg_dramc_sref(void);
extern int rgu_is_reserve_ddr_mode_success(void);
extern int rgu_is_dram_slf(void);
extern int rgu_is_reserve_ddr_enabled(void);



#endif  
