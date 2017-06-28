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
#ifndef __PMIC_WRAP_REGS_H__
#define __PMIC_WRAP_REGS_H__

#define UINT32 unsigned int 


#define PMIC_WRAP_MUX_SEL               ((UINT32)(PMIC_WRAP_BASE+0x0))
#define PMIC_WRAP_WRAP_EN               ((UINT32)(PMIC_WRAP_BASE+0x4))
#define PMIC_WRAP_DIO_EN                ((UINT32)(PMIC_WRAP_BASE+0x8))
#define PMIC_WRAP_SI_SAMPLE_CTRL        ((UINT32)(PMIC_WRAP_BASE+0xC))
#define PMIC_WRAP_RDDMY                 ((UINT32)(PMIC_WRAP_BASE+0x10))
#define PMIC_WRAP_CSHEXT_WRITE          ((UINT32)(PMIC_WRAP_BASE+0x14))
#define PMIC_WRAP_CSHEXT_READ           ((UINT32)(PMIC_WRAP_BASE+0x18))
#define PMIC_WRAP_CSLEXT_WRITE          ((UINT32)(PMIC_WRAP_BASE+0x1C))
#define PMIC_WRAP_CSLEXT_READ           ((UINT32)(PMIC_WRAP_BASE+0x20))
#define PMIC_WRAP_STAUPD_CTRL           ((UINT32)(PMIC_WRAP_BASE+0x24))
#define PMIC_WRAP_STAUPD_GRPEN          ((UINT32)(PMIC_WRAP_BASE+0x28))
#define PMIC_WRAP_EINT_STA0_ADR         ((UINT32)(PMIC_WRAP_BASE+0x2C))
#define PMIC_WRAP_EINT_STA1_ADR         ((UINT32)(PMIC_WRAP_BASE+0x30))
#define PMIC_WRAP_EINT_STA              ((UINT32)(PMIC_WRAP_BASE+0x34))
#define PMIC_WRAP_EINT_CLR              ((UINT32)(PMIC_WRAP_BASE+0x38))
#define PMIC_WRAP_EINT_CTRL             ((UINT32)(PMIC_WRAP_BASE+0x3C))
#define PMIC_WRAP_STAUPD_MAN_TRIG       ((UINT32)(PMIC_WRAP_BASE+0x40))
#define PMIC_WRAP_STAUPD_STA            ((UINT32)(PMIC_WRAP_BASE+0x44))
#define PMIC_WRAP_WRAP_STA              ((UINT32)(PMIC_WRAP_BASE+0x48))
#define PMIC_WRAP_HARB_INIT             ((UINT32)(PMIC_WRAP_BASE+0x4C))
#define PMIC_WRAP_HARB_HPRIO            ((UINT32)(PMIC_WRAP_BASE+0x50))
#define PMIC_WRAP_HIPRIO_ARB_EN         ((UINT32)(PMIC_WRAP_BASE+0x54))
#define PMIC_WRAP_HARB_STA0             ((UINT32)(PMIC_WRAP_BASE+0x58))
#define PMIC_WRAP_HARB_STA1             ((UINT32)(PMIC_WRAP_BASE+0x5C))
#define PMIC_WRAP_MAN_EN                ((UINT32)(PMIC_WRAP_BASE+0x60))
#define PMIC_WRAP_MAN_CMD               ((UINT32)(PMIC_WRAP_BASE+0x64))
#define PMIC_WRAP_MAN_RDATA             ((UINT32)(PMIC_WRAP_BASE+0x68))
#define PMIC_WRAP_MAN_VLDCLR            ((UINT32)(PMIC_WRAP_BASE+0x6C))
#define PMIC_WRAP_WACS0_EN              ((UINT32)(PMIC_WRAP_BASE+0x70))
#define PMIC_WRAP_INIT_DONE0            ((UINT32)(PMIC_WRAP_BASE+0x74))
#define PMIC_WRAP_WACS0_CMD             ((UINT32)(PMIC_WRAP_BASE+0x78))
#define PMIC_WRAP_WACS0_RDATA           ((UINT32)(PMIC_WRAP_BASE+0x7C))
#define PMIC_WRAP_WACS0_VLDCLR          ((UINT32)(PMIC_WRAP_BASE+0x80))
#define PMIC_WRAP_WACS1_EN              ((UINT32)(PMIC_WRAP_BASE+0x84))
#define PMIC_WRAP_INIT_DONE1            ((UINT32)(PMIC_WRAP_BASE+0x88))
#define PMIC_WRAP_WACS1_CMD             ((UINT32)(PMIC_WRAP_BASE+0x8C))
#define PMIC_WRAP_WACS1_RDATA           ((UINT32)(PMIC_WRAP_BASE+0x90))
#define PMIC_WRAP_WACS1_VLDCLR          ((UINT32)(PMIC_WRAP_BASE+0x94))
#define PMIC_WRAP_WACS2_EN              ((UINT32)(PMIC_WRAP_BASE+0x98))
#define PMIC_WRAP_INIT_DONE2            ((UINT32)(PMIC_WRAP_BASE+0x9C))
#define PMIC_WRAP_WACS2_CMD             ((UINT32)(PMIC_WRAP_BASE+0xA0))
#define PMIC_WRAP_WACS2_RDATA           ((UINT32)(PMIC_WRAP_BASE+0xA4))
#define PMIC_WRAP_WACS2_VLDCLR          ((UINT32)(PMIC_WRAP_BASE+0xA8))
#define PMIC_WRAP_WACS3_EN              ((UINT32)(PMIC_WRAP_BASE+0xAC))
#define PMIC_WRAP_INIT_DONE3            ((UINT32)(PMIC_WRAP_BASE+0xB0))
#define PMIC_WRAP_WACS3_CMD             ((UINT32)(PMIC_WRAP_BASE+0xB4))
#define PMIC_WRAP_WACS3_RDATA           ((UINT32)(PMIC_WRAP_BASE+0xB8))
#define PMIC_WRAP_WACS3_VLDCLR          ((UINT32)(PMIC_WRAP_BASE+0xBC))
#define PMIC_WRAP_INT0_EN               ((UINT32)(PMIC_WRAP_BASE+0xC0))
#define PMIC_WRAP_INT0_FLG_RAW          ((UINT32)(PMIC_WRAP_BASE+0xC4))
#define PMIC_WRAP_INT0_FLG              ((UINT32)(PMIC_WRAP_BASE+0xC8))
#define PMIC_WRAP_INT0_CLR              ((UINT32)(PMIC_WRAP_BASE+0xCC))
#define PMIC_WRAP_INT1_EN               ((UINT32)(PMIC_WRAP_BASE+0xD0))
#define PMIC_WRAP_INT1_FLG_RAW          ((UINT32)(PMIC_WRAP_BASE+0xD4))
#define PMIC_WRAP_INT1_FLG              ((UINT32)(PMIC_WRAP_BASE+0xD8))
#define PMIC_WRAP_INT1_CLR              ((UINT32)(PMIC_WRAP_BASE+0xDC))
#define PMIC_WRAP_SIG_ADR               ((UINT32)(PMIC_WRAP_BASE+0xE0))
#define PMIC_WRAP_SIG_MODE              ((UINT32)(PMIC_WRAP_BASE+0xE4))
#define PMIC_WRAP_SIG_VALUE             ((UINT32)(PMIC_WRAP_BASE+0xE8))
#define PMIC_WRAP_SIG_ERRVAL            ((UINT32)(PMIC_WRAP_BASE+0xEC))
#define PMIC_WRAP_CRC_EN                ((UINT32)(PMIC_WRAP_BASE+0xF0))
#define PMIC_WRAP_TIMER_EN              ((UINT32)(PMIC_WRAP_BASE+0xF4))
#define PMIC_WRAP_TIMER_STA             ((UINT32)(PMIC_WRAP_BASE+0xF8))
#define PMIC_WRAP_WDT_UNIT              ((UINT32)(PMIC_WRAP_BASE+0xFC))
#define PMIC_WRAP_WDT_SRC_EN            ((UINT32)(PMIC_WRAP_BASE+0x100))
#define PMIC_WRAP_WDT_FLG               ((UINT32)(PMIC_WRAP_BASE+0x104))
#define PMIC_WRAP_DEBUG_INT_SEL         ((UINT32)(PMIC_WRAP_BASE+0x108))
#define PMIC_WRAP_DVFS_ADR0             ((UINT32)(PMIC_WRAP_BASE+0x10C))
#define PMIC_WRAP_DVFS_WDATA0           ((UINT32)(PMIC_WRAP_BASE+0x110))
#define PMIC_WRAP_DVFS_ADR1             ((UINT32)(PMIC_WRAP_BASE+0x114))
#define PMIC_WRAP_DVFS_WDATA1           ((UINT32)(PMIC_WRAP_BASE+0x118))
#define PMIC_WRAP_DVFS_ADR2             ((UINT32)(PMIC_WRAP_BASE+0x11C))
#define PMIC_WRAP_DVFS_WDATA2           ((UINT32)(PMIC_WRAP_BASE+0x120))
#define PMIC_WRAP_DVFS_ADR3             ((UINT32)(PMIC_WRAP_BASE+0x124))
#define PMIC_WRAP_DVFS_WDATA3           ((UINT32)(PMIC_WRAP_BASE+0x128))
#define PMIC_WRAP_DVFS_ADR4             ((UINT32)(PMIC_WRAP_BASE+0x12C))
#define PMIC_WRAP_DVFS_WDATA4           ((UINT32)(PMIC_WRAP_BASE+0x130))
#define PMIC_WRAP_DVFS_ADR5             ((UINT32)(PMIC_WRAP_BASE+0x134))
#define PMIC_WRAP_DVFS_WDATA5           ((UINT32)(PMIC_WRAP_BASE+0x138))
#define PMIC_WRAP_DVFS_ADR6             ((UINT32)(PMIC_WRAP_BASE+0x13C))
#define PMIC_WRAP_DVFS_WDATA6           ((UINT32)(PMIC_WRAP_BASE+0x140))
#define PMIC_WRAP_DVFS_ADR7             ((UINT32)(PMIC_WRAP_BASE+0x144))
#define PMIC_WRAP_DVFS_WDATA7           ((UINT32)(PMIC_WRAP_BASE+0x148))
#define PMIC_WRAP_DVFS_ADR8             ((UINT32)(PMIC_WRAP_BASE+0x14C))
#define PMIC_WRAP_DVFS_WDATA8           ((UINT32)(PMIC_WRAP_BASE+0x150))
#define PMIC_WRAP_DVFS_ADR9             ((UINT32)(PMIC_WRAP_BASE+0x154))
#define PMIC_WRAP_DVFS_WDATA9           ((UINT32)(PMIC_WRAP_BASE+0x158))
#define PMIC_WRAP_DVFS_ADR10            ((UINT32)(PMIC_WRAP_BASE+0x15C))
#define PMIC_WRAP_DVFS_WDATA10          ((UINT32)(PMIC_WRAP_BASE+0x160))
#define PMIC_WRAP_DVFS_ADR11            ((UINT32)(PMIC_WRAP_BASE+0x164))
#define PMIC_WRAP_DVFS_WDATA11          ((UINT32)(PMIC_WRAP_BASE+0x168))
#define PMIC_WRAP_DVFS_ADR12            ((UINT32)(PMIC_WRAP_BASE+0x16C))
#define PMIC_WRAP_DVFS_WDATA12          ((UINT32)(PMIC_WRAP_BASE+0x170))
#define PMIC_WRAP_DVFS_ADR13            ((UINT32)(PMIC_WRAP_BASE+0x174))
#define PMIC_WRAP_DVFS_WDATA13          ((UINT32)(PMIC_WRAP_BASE+0x178))
#define PMIC_WRAP_DVFS_ADR14            ((UINT32)(PMIC_WRAP_BASE+0x17C))
#define PMIC_WRAP_DVFS_WDATA14          ((UINT32)(PMIC_WRAP_BASE+0x180))
#define PMIC_WRAP_DVFS_ADR15            ((UINT32)(PMIC_WRAP_BASE+0x184))
#define PMIC_WRAP_DVFS_WDATA15          ((UINT32)(PMIC_WRAP_BASE+0x188))
#define PMIC_WRAP_DCXO_ENABLE           ((UINT32)(PMIC_WRAP_BASE+0x18C))
#define PMIC_WRAP_DCXO_CONN_ADR0        ((UINT32)(PMIC_WRAP_BASE+0x190))
#define PMIC_WRAP_DCXO_CONN_WDATA0      ((UINT32)(PMIC_WRAP_BASE+0x194))
#define PMIC_WRAP_DCXO_CONN_ADR1        ((UINT32)(PMIC_WRAP_BASE+0x198))
#define PMIC_WRAP_DCXO_CONN_WDATA1      ((UINT32)(PMIC_WRAP_BASE+0x19C))
#define PMIC_WRAP_DCXO_NFC_ADR0         ((UINT32)(PMIC_WRAP_BASE+0x1A0))
#define PMIC_WRAP_DCXO_NFC_WDATA0       ((UINT32)(PMIC_WRAP_BASE+0x1A4))
#define PMIC_WRAP_DCXO_NFC_ADR1         ((UINT32)(PMIC_WRAP_BASE+0x1A8))
#define PMIC_WRAP_DCXO_NFC_WDATA1       ((UINT32)(PMIC_WRAP_BASE+0x1AC))
#define PMIC_WRAP_SPMINF_STA            ((UINT32)(PMIC_WRAP_BASE+0x1B0))
#define PMIC_WRAP_CIPHER_KEY_SEL        ((UINT32)(PMIC_WRAP_BASE+0x1B4))
#define PMIC_WRAP_CIPHER_IV_SEL         ((UINT32)(PMIC_WRAP_BASE+0x1B8))
#define PMIC_WRAP_CIPHER_EN             ((UINT32)(PMIC_WRAP_BASE+0x1BC))
#define PMIC_WRAP_CIPHER_RDY            ((UINT32)(PMIC_WRAP_BASE+0x1C0))
#define PMIC_WRAP_CIPHER_MODE           ((UINT32)(PMIC_WRAP_BASE+0x1C4))
#define PMIC_WRAP_CIPHER_SWRST          ((UINT32)(PMIC_WRAP_BASE+0x1C8))
#define PMIC_WRAP_DCM_EN                ((UINT32)(PMIC_WRAP_BASE+0x1CC))
#define PMIC_WRAP_DCM_SPI_DBC_PRD       ((UINT32)(PMIC_WRAP_BASE+0x1D0))
#define PMIC_WRAP_DCM_DBC_PRD           ((UINT32)(PMIC_WRAP_BASE+0x1D4))
#define PMIC_WRAP_EXT_CK                ((UINT32)(PMIC_WRAP_BASE+0x1D8))
#define PMIC_WRAP_ADC_CMD_ADDR          ((UINT32)(PMIC_WRAP_BASE+0x1DC))
#define PMIC_WRAP_PWRAP_ADC_CMD         ((UINT32)(PMIC_WRAP_BASE+0x1E0))
#define PMIC_WRAP_GPS_ADC_RDATA_ADDR    ((UINT32)(PMIC_WRAP_BASE+0x1E4))
#define PMIC_WRAP_GPS_STA               ((UINT32)(PMIC_WRAP_BASE+0x1E8))
#define PMIC_WRAP_GPS_ADC_OPTIONS       ((UINT32)(PMIC_WRAP_BASE+0x1EC))
#define PMIC_WRAP_SWRST                 ((UINT32)(PMIC_WRAP_BASE+0x1F0))
#define PMIC_WRAP_SPM_SLEEP_GATING_CTRL  ((UINT32)(PMIC_WRAP_BASE+0x1F4))
#define PMIC_WRAP_SCP_SLEEP_GATING_CTRL  ((UINT32)(PMIC_WRAP_BASE+0x1F8))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR_LATEST  ((UINT32)(PMIC_WRAP_BASE+0x1FC))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR_WP  ((UINT32)(PMIC_WRAP_BASE+0x200))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR0    ((UINT32)(PMIC_WRAP_BASE+0x204))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR1    ((UINT32)(PMIC_WRAP_BASE+0x208))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR2    ((UINT32)(PMIC_WRAP_BASE+0x20C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR3    ((UINT32)(PMIC_WRAP_BASE+0x210))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR4    ((UINT32)(PMIC_WRAP_BASE+0x214))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR5    ((UINT32)(PMIC_WRAP_BASE+0x218))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR6    ((UINT32)(PMIC_WRAP_BASE+0x21C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR7    ((UINT32)(PMIC_WRAP_BASE+0x220))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR8    ((UINT32)(PMIC_WRAP_BASE+0x224))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR9    ((UINT32)(PMIC_WRAP_BASE+0x228))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR10   ((UINT32)(PMIC_WRAP_BASE+0x22C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR11   ((UINT32)(PMIC_WRAP_BASE+0x230))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR12   ((UINT32)(PMIC_WRAP_BASE+0x234))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR13   ((UINT32)(PMIC_WRAP_BASE+0x238))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR14   ((UINT32)(PMIC_WRAP_BASE+0x23C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR15   ((UINT32)(PMIC_WRAP_BASE+0x240))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR16   ((UINT32)(PMIC_WRAP_BASE+0x244))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR17   ((UINT32)(PMIC_WRAP_BASE+0x248))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR18   ((UINT32)(PMIC_WRAP_BASE+0x24C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR19   ((UINT32)(PMIC_WRAP_BASE+0x250))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR20   ((UINT32)(PMIC_WRAP_BASE+0x254))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR21   ((UINT32)(PMIC_WRAP_BASE+0x258))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR22   ((UINT32)(PMIC_WRAP_BASE+0x25C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR23   ((UINT32)(PMIC_WRAP_BASE+0x260))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR24   ((UINT32)(PMIC_WRAP_BASE+0x264))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR25   ((UINT32)(PMIC_WRAP_BASE+0x268))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR26   ((UINT32)(PMIC_WRAP_BASE+0x26C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR27   ((UINT32)(PMIC_WRAP_BASE+0x270))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR28   ((UINT32)(PMIC_WRAP_BASE+0x274))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR29   ((UINT32)(PMIC_WRAP_BASE+0x278))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR30   ((UINT32)(PMIC_WRAP_BASE+0x27C))
#define PMIC_WRAP_MD_ADC_RDATA_ADDR31   ((UINT32)(PMIC_WRAP_BASE+0x280))
#define PMIC_WRAP_MD_ADC_MODE_SEL       ((UINT32)(PMIC_WRAP_BASE+0x284))
#define PMIC_WRAP_MD_ADC_STA0           ((UINT32)(PMIC_WRAP_BASE+0x288))
#define PMIC_WRAP_MD_ADC_STA1           ((UINT32)(PMIC_WRAP_BASE+0x28C))
#define PMIC_WRAP_PRIORITY_USER_SEL_0   ((UINT32)(PMIC_WRAP_BASE+0x290))
#define PMIC_WRAP_PRIORITY_USER_SEL_1   ((UINT32)(PMIC_WRAP_BASE+0x294))
#define PMIC_WRAP_ARBITER_OUT_SEL_0     ((UINT32)(PMIC_WRAP_BASE+0x298))
#define PMIC_WRAP_ARBITER_OUT_SEL_1     ((UINT32)(PMIC_WRAP_BASE+0x29C))
#define PMIC_WRAP_STARV_COUNTER_0       ((UINT32)(PMIC_WRAP_BASE+0x2A0))
#define PMIC_WRAP_STARV_COUNTER_1       ((UINT32)(PMIC_WRAP_BASE+0x2A4))
#define PMIC_WRAP_STARV_COUNTER_2       ((UINT32)(PMIC_WRAP_BASE+0x2A8))
#define PMIC_WRAP_STARV_COUNTER_3       ((UINT32)(PMIC_WRAP_BASE+0x2AC))
#define PMIC_WRAP_STARV_COUNTER_4       ((UINT32)(PMIC_WRAP_BASE+0x2B0))
#define PMIC_WRAP_STARV_COUNTER_5       ((UINT32)(PMIC_WRAP_BASE+0x2B4))
#define PMIC_WRAP_STARV_COUNTER_6       ((UINT32)(PMIC_WRAP_BASE+0x2B8))
#define PMIC_WRAP_STARV_COUNTER_7       ((UINT32)(PMIC_WRAP_BASE+0x2BC))
#define PMIC_WRAP_STARV_COUNTER_8       ((UINT32)(PMIC_WRAP_BASE+0x2C0))
#define PMIC_WRAP_STARV_COUNTER_9       ((UINT32)(PMIC_WRAP_BASE+0x2C4))
#define PMIC_WRAP_STARV_COUNTER_10      ((UINT32)(PMIC_WRAP_BASE+0x2C8))
#define PMIC_WRAP_STARV_COUNTER_11      ((UINT32)(PMIC_WRAP_BASE+0x2CC))
#define PMIC_WRAP_STARV_COUNTER_12      ((UINT32)(PMIC_WRAP_BASE+0x2D0))
#define PMIC_WRAP_STARV_COUNTER_13      ((UINT32)(PMIC_WRAP_BASE+0x2D4))
#define PMIC_WRAP_STARV_COUNTER_14      ((UINT32)(PMIC_WRAP_BASE+0x2D8))
#define PMIC_WRAP_STARV_COUNTER_15      ((UINT32)(PMIC_WRAP_BASE+0x2DC))
#define PMIC_WRAP_STARV_COUNTER_0_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x2E0))
#define PMIC_WRAP_STARV_COUNTER_1_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x2E4))
#define PMIC_WRAP_STARV_COUNTER_2_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x2E8))
#define PMIC_WRAP_STARV_COUNTER_3_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x2EC))
#define PMIC_WRAP_STARV_COUNTER_4_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x2F0))
#define PMIC_WRAP_STARV_COUNTER_5_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x2F4))
#define PMIC_WRAP_STARV_COUNTER_6_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x2F8))
#define PMIC_WRAP_STARV_COUNTER_7_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x2FC))
#define PMIC_WRAP_STARV_COUNTER_8_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x300))
#define PMIC_WRAP_STARV_COUNTER_9_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x304))
#define PMIC_WRAP_STARV_COUNTER_10_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x308))
#define PMIC_WRAP_STARV_COUNTER_11_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x30C))
#define PMIC_WRAP_STARV_COUNTER_12_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x310))
#define PMIC_WRAP_STARV_COUNTER_13_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x314))
#define PMIC_WRAP_STARV_COUNTER_14_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x318))
#define PMIC_WRAP_STARV_COUNTER_15_STATUS  ((UINT32)(PMIC_WRAP_BASE+0x31C))
#define PMIC_WRAP_STARV_COUNTER_CLR     ((UINT32)(PMIC_WRAP_BASE+0x320))
#define PMIC_WRAP_STARV_PRIO_STATUS     ((UINT32)(PMIC_WRAP_BASE+0x324))
#define PMIC_WRAP_DEBUG_STATUS          ((UINT32)(PMIC_WRAP_BASE+0x328))
#define PMIC_WRAP_DEBUG_SQUENCE_0       ((UINT32)(PMIC_WRAP_BASE+0x32C))
#define PMIC_WRAP_DEBUG_SQUENCE_1       ((UINT32)(PMIC_WRAP_BASE+0x330))
#define PMIC_WRAP_DEBUG_SQUENCE_2       ((UINT32)(PMIC_WRAP_BASE+0x334))
#define PMIC_WRAP_DEBUG_SQUENCE_3       ((UINT32)(PMIC_WRAP_BASE+0x338))
#define PMIC_WRAP_DEBUG_SQUENCE_4       ((UINT32)(PMIC_WRAP_BASE+0x33C))
#define PMIC_WRAP_DEBUG_SQUENCE_5       ((UINT32)(PMIC_WRAP_BASE+0x340))
#define PMIC_WRAP_DEBUG_SQUENCE_6       ((UINT32)(PMIC_WRAP_BASE+0x344))
#define PMIC_WRAP_DEBUG_SW_DRIVER_0     ((UINT32)(PMIC_WRAP_BASE+0x348))
#define PMIC_WRAP_DEBUG_SW_DRIVER_1     ((UINT32)(PMIC_WRAP_BASE+0x34C))
#define PMIC_WRAP_DEBUG_SW_DRIVER_2     ((UINT32)(PMIC_WRAP_BASE+0x350))
#define PMIC_WRAP_DEBUG_SW_DRIVER_3     ((UINT32)(PMIC_WRAP_BASE+0x354))
#define PMIC_WRAP_DEBUG_SW_DRIVER_4     ((UINT32)(PMIC_WRAP_BASE+0x358))
#define PMIC_WRAP_DEBUG_SW_DRIVER_5     ((UINT32)(PMIC_WRAP_BASE+0x35C))
#define PMIC_WRAP_BWC_OPTIONS           ((UINT32)(PMIC_WRAP_BASE+0x360))
#define PMIC_WRAP_RESERVED              ((UINT32)(PMIC_WRAP_BASE+0x364))

#endif //__PMIC_WRAP_REGS_H__
