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
 *    usb_comm.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   This file is intends for usb common definitions.
 *
 * Author:
 * -------
 *  Jensen Hu
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:   May 16 2005 23:13:58  $
 * $Log:   //mtkvs01/vmdata/Maui_sw/archives/mcu/interface/hwdrv/usb_comm.h-arc  $
 *
 * Dec 2 2007 mtk01413
 * [STP100001568] [FlashTool] v3.0748.0 release
 * New features:
 *  1. USB Download on MT6225/MT6226/MT6227 Series BB
 *
 * Jul 5 2006 MTK01309
 * [MAUI_00200353] [USB][New feature] Enable USB PictBridge feature
 * 
 *
 * Mar 22 2006 MTK00796
 * [MAUI_00181365] [USB][New Feature] Support IRDBG on MT6229
 * Add IRDBG config type.
 *
 * Jan 4 2006 mtk00796
 * [MAUI_00165830] [USB][New Feature] Add OTG, normal mode, and modify USB mode selection functions.
 * Add stop mass storage device type.
 *
 * Oct 28 2005 mtk00796
 * [MAUI_00153186] [USB][New feature] Add webcam feature
 * Add USB video type.
 * 
 *    Rev 1.8   May 17 2005 00:29:18   BM_Trunk
 * Karlos:
 * add copyright and disclaimer statement
 * 
 *    Rev 1.7   Jan 18 2005 00:34:30   BM
 * append new line in W05.04
 * 
 *    Rev 1.6   May 13 2004 19:58:24   mtk00796
 * adjust USB code
 * Resolution for 5417: [USB][Adjust code]
 * 
 *    Rev 1.5   Apr 25 2004 18:00:36   mtk00288
 * add a definition USB_UNKNOWN_DMA_CHANNEL for usb dma channel initial value
 * Resolution for 1310: Assert fail: 0 dma.c 513 - USBMS
 * 
 *    Rev 1.4   Mar 31 2004 21:12:00   mtk00288
 * add Copyright Statement
 * Resolution for 4620: [USB][Add Feature] USB customize
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef USB_COMM_H
#define USB_COMM_H
#define __USB_DOWNLOAD__
#define USBACM_TX_WITHOUT_DMA
 
/* define usb device type*/
typedef enum 
{
	USB_UNKOWN=0,
	USB_CDC_ACM,
	USB_MASS_STORAGE,
	USB_VIDEO,
	USB_COMPOSITE,
	USB_STOP_MS,
	USB_CDC_ACM_IRDBG,
	USB_IMAGE,
	USB_MAX_DEVICE_TYPE
} USB_DEVICE_TYPE;

kal_bool USB_GetCableStatus(void);

#endif  /* USB_COMM_H */                   

