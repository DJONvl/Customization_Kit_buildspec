/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    usb_mode.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *    This file intends for usb mode selection definitions
 *
 * Author:
 * -------
 *   CJung Chen 
 *
  *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Log$
 *
 * Dec 2 2007 mtk01413
 * [STP100001568] [FlashTool] v3.0748.0 release
 * New features:
 *  1. USB Download on MT6225/MT6226/MT6227 Series BB
 *
 * Oct 3 2006 mtk01309
 * [MAUI_00334590] [USB normal mode][New feature] Add USB normal mode improve feature
 * 
 *
 * Jul 12 2006 MTK01309
 * [MAUI_00200353] [USB][New feature] Enable USB PictBridge feature
 * 
 *
 * Apr 4 2006 mtk00796
 * [MAUI_00184705] [OTG][Bug] Solve compile error for OTG handler
 * Define OTG handler only when OTG compile option is enabled.
 *
 * Mar 29 2006 mtk00796
 * [MAUI_00183402] [OTG][Bug] Support OTG when USB normal mode is on
 * Add OTG handler select function.
 *
 * Mar 26 2006 mtk00796
 * [MAUI_00182204] [FT, USB] Add new camera features for CDT, Defect Tool, 6228CCT, OQC and UART/USB mu
 * Add factory mode parameter.
 *
 * Mar 23 2006 mtk00796
 * [MAUI_00220212] USB-Handset stop USB mode after plug out USB
 * Modify init USB mass storage function.
 *
 * Jan 4 2006 mtk00796
 * [MAUI_00165830] [USB][New Feature] Add OTG, normal mode, and modify USB mode selection functions.
 * Add USB device and OTG mode selection functions.
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
 
#ifndef USB_MODE_H
#define USB_MODE_H

typedef enum
{
	USB_MODE_NVRAM_READ_DONE = 1<<0,
	USB_MODE_BMT_READ_DONE = 1<<1,
	USB_MODE_FT_START_DONE = 1<<2
}USB_MODE_PRARM;

typedef enum
{
	USB_MODE_CABLE_PLUGOUT,
	USB_MODE_CABLE_A_PLUGIN,
	USB_MODE_CABLE_B_PLUGIN
}USB_MODE_CABLE_TYPE;

typedef struct
{
	USB_MODE_PRARM 		mode_param;
	USB_MODE_CABLE_TYPE 	cable_type;
	USB_MODE_CABLE_TYPE 	previous_cable_type;
	kal_bool					b_start_srp;
	kal_bool					b_start_hnp;
	kal_bool 					b_usb_pdnmode;
}Usb_Mode_Struct;

extern Usb_Mode_Struct g_UsbMode;

extern void USB_Init_Mode(void);
extern void USB_Mode_Selection(void);
extern void OTG_Mode_Selection(void);

extern void USB_Config_Type(USB_DEVICE_TYPE type, kal_bool b_enable, kal_uint32 *parameter);
extern void USB_Release_Type(kal_bool b_enable, kal_bool b_plugout);
extern void USB_Init_Ms(kal_bool bFirst);
extern void USB_Init_ACM_In_Meta(void);
extern void USB_Change_Interface_Sequence(void);

#endif //USB_MODE_H

