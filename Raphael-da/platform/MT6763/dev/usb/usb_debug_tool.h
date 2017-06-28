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
*  A. Patent Notice:
*  The software delivered hereunder, including any and all updates and upgrades, 
*  might be subject to patent rights of standard-setting organizations, patent pools 
*  or their respective patent owners, that will be necessarily infringed by the use 
*  and distribution of the products incorporating all or part of this software.  
*  According to the licensing terms of these patent holders, it is your sole 
*  obligation to obtain the necessary patent licenses from these patent holders 
*  before you can use or distribute the products incorporating all or part of this 
*  software. MediaTek shall not be liable for your failure to obtain or maintain such 
*  licenses.
*  
*    As a courtesy to our customers, the following are some of the software that might 
*    contain such patent licenses, but MediaTek does not warrant the accuracy or
*    completeness of the information below.  
*    (1) MPEG4/AAC/AACPLUS/AACVPLUSV2: essential patents license must be obtained
*        from Via Licensing: <www.vialicensing.com>
*    (2) WAP/MMS security RC5 algorithm belongs to RSA Data Security: 
*        <www.rsasecurity.com> 
*    Notice: Please contact RSA to get this license before shipping the products to 
*    USA which include RC5 security algorithm.
*  
*  B. In addition, customers must contact the respective licensors to obtain 
*  necessary software licenses before it can use or distribute the licensed 
*  products. 
*  
*    As a courtesy to our customers, the following are some of the software licensers 
*    and the notice or disclaimer required by their licenses, but MediaTek does not 
*    warrant the accuracy or completeness of the information below.  
*    (1) Microsoft Windows Media (WMA software): 
*        Microsoft: <www.microsoft.com> 
*        Approved OEM Manufacturers: <wmlicense.smdisp.net/oem_approved/>
*        Sample Windows Media Licensing Agreements: 
*        <www.microsoft.com/windows/windowsmedia/licensing/agreements.aspx>
*    Notice in header or documentation: "This product is protected by certain 
*    intellectual property rights of Microsoft and cannot be used or further 
*    distributed without a license from Microsoft."
*    
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   usbdl_debug_tool.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *  This file intends for usb download common driver debug buffer definitions
 *
 * Author:
 * -------
 *  Choco Chou (mtk01309)
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime$
 * $Log$
 *
 * Feb 24 2009 MTK02172
 * [STP100001912] [FlashTool] v3.0908.00 release
 * 
 *
 * Apr 24 2008 mtk01413
 * [STP100001691] [FlashTool] v3.0808.0 release
 * 
 *
 * Dec 2 2007 mtk01309
 * [MAUI_00585931] [USB USB_DOWNLOAD] [Change feature] Add USB debug info into bootloader
 * 
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#ifndef USBDL_DEBUG_TOOL_H
#define USBDL_DEBUG_TOOL_H

/***********************************************
	implement definition
************************************************/

#define USB_DBG_DATA_SIZE   64


typedef enum
{
	DEBUG_USB2COM_GetByte = 1,
	DEBUG_USB2COM_GetByteStatus,
	DEBUG_USB2COM_GetData16,
	DEBUG_USB2COM_GetData32,
	DEBUG_USB2COM_Init,
	DEBUG_USB2COM_PurgeFIFO,
	DEBUG_USB2COM_PutByte,
	DEBUG_USB2COM_PutByte_buffer,
	DEBUG_USB2COM_GetByte_Buffer,
	DEBUG_USB2COM_PutByte_Complete,
	DEBUG_USB2COM_PutData16,
	DEBUG_USB2COM_PutData32,
	DEBUG_USB2COM_SetBaudRate,
	DEBUG_USB_ACM_SET_LINE_CODING,
	DEBUG_USB_ACM_GET_LINE_CODING,
	DEBUG_USB_ACM_SETUP,
	DEBUG_USB_Init_Drv_Info,
	DEBUG_USB_EP0En,
	DEBUG_USB_InEPEn,
	DEBUG_USB_InEPInit,
	DEBUG_USB_OutEPEn,
	DEBUG_USB_OutEPInit,
	DEBUG_USB_EPFIFORead,
	DEBUG_USB_EPFIFOWrite,
	DEBUG_USB2COM_GetDatas,
	DEBUG_USB2COM_PutDatas,
	USB_DBG_MAX
}USB_DBG_INFO;


/***********************************************
	implement enum and structure
************************************************/

typedef struct
{
	USB_DBG_INFO	tag;
	kal_uint32      time;
	kal_uint32      data1;
	kal_uint32      data2;
}Usb_Dbg_Data;


typedef struct
{
	Usb_Dbg_Data	dbg_data[USB_DBG_DATA_SIZE];
	kal_uint16 	dbg_data_idx;
}Usb_Dbg_Struct;


/***********************************************
	function and global variable
************************************************/
extern void USB_Dbg_Trace(USB_DBG_INFO tag, kal_uint32 time, kal_uint32 data1, kal_uint32 data2);
extern kal_uint32 drv_get_current_time(void);

#endif /* USBDL_DEBUG_TOOL_H */


