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

#define MTK_WDT_BASE			TOPRGU_BASE

#define MTK_WDT_DEBUG_CTL		(MTK_WDT_BASE+0x0040)
#define MTK_DEBUG_CTL_KEY              (0x59000000)
#define MTK_RGU_EMI_DCS_EN             (0x00000002) /* bit 1: emi_dcs_en */
#define MTK_RGU_DVFSRC_EN              (0x00000004) /* bit 2: dvfsrc_en */
#define MTK_RG_DRAMC_SREF              (0x00100)
#define MTK_RG_DRAMC_ISO               (0x00200)
#define MTK_RG_CONF_ISO                (0x00400)
#define MTK_DDR_RESERVE_RTA            (0x10000)    /* bit 16: ddr_reserve_success */

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


extern void EnableWatchDog(uint16 interval_ms,uint32 flags) ;
extern void DisableWDT(void);
#endif  
