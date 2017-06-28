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
 *    usb_drv_pri.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *  This file intends for usb hardware related function definitions. Only use for hardware driver.
 *
 * Author:
 * -------
 *   CJung Chen
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:   Jun 13 2005 22:54:14  $
 * $Log:   //mtkvs01/vmdata/Maui_sw/archives/mcu/drv/include/usb_drv_pri.h-arc  $
 *
 * Feb 24 2009 MTK02172
 * [STP100001912] [FlashTool] v3.0908.00 release
 * 
 *
 * May 16 2008 mtk01413
 * [STP100001691] [FlashTool] v3.0808.0 release
 * New features:
 *  1. USB Download for End User (MT6230)
 *
 * Dec 2 2007 mtk01413
 * [STP100001568] [FlashTool] v3.0748.0 release
 * New features:
 *  1. USB Download on MT6225/MT6226/MT6227 Series BB
 *
 * Mar 12 2007 mtk01309
 * [MAUI_00371565] [USB][New feature] Add debug information
 * 
 *
 * Feb 27 2007 mtk01309
 * [MAUI_00230177] [1] Assert fail: 0 usb_drv.c 2411-DRVHISR
 * 
 *
 * Jan 28 2007 mtk01309
 * [MAUI_00362308] [USB DRV][New Feature] Add driver feature management code to main trunk
 * 
 *
 * Nov 23 2006 mtk01309
 * [MAUI_00345639] [USB][New feature] Add compile option for MT6268T to maintrunk and Monza
 * 
 *
 * Jul 14 2006 MTK01309
 * [MAUI_00210598] [USB][Change feature] Add compile option for MT6230
 * 
 *
 * May 10 2006 MTK01309
 * [MAUI_00192497] [USB][Change feature] remove compile option for MT6230
 * 
 *
 * Apr 3 2006 mtk00796
 * [MAUI_00184302] [USB][New feature] Add compile option for MT6230
 * Add MT6230 compile option.
 *
 * Jan 19 2006 mtk00796
 * [MAUI_00169301] [USB][Modify] Modify USB HISR to central DRV HISR
 * Remove hisr item.
 *
 * Oct 28 2005 mtk00796
 * [MAUI_00153186] [USB][New feature] Add webcam feature
 * Add HISR protect item.
 * 
 *    Rev 1.1   Jun 13 2005 23:10:14   mtk00796
 * Add nIRQ related items to detect VBUS.
 * Resolution for 11522: [USB][Change Design] Use nIRQ to detect Vbus instead of EINT
 * 
 *    Rev 1.0   Jun 01 2005 16:41:42   BM
 * Initial revision.
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
 
#ifndef USB_DRV_PRI_H
#define USB_DRV_PRI_H

#ifdef USB_SUPPORT_MUTIPLE_APP
/* for future use, max 2 DMA channel are supported */
#define MAX_DMA_EP_NUM		2
#else /* USB_SUPPORT_MUTIPLE_APP */
/* for current application, max 1 logical endpoint use DMA channel */
#define MAX_DMA_EP_NUM		1
#endif /* USB_SUPPORT_MUTIPLE_APP */

/* power state */
typedef enum 
{
	USB_STATE_NORMAL = 0,
	USB_STATE_SUSPEND
} USB_POWER_STATE;


#define USB_BDT_NUMBER				10

/* BDT structure */
typedef struct
{
	kal_uint32	PID_OWN_DATA01_BC;
	kal_uint32	ADDRESS;
} USB_BDT, *USB_BDT_PTR;

/* Endpoint state*/
typedef enum
{
	USB_EP_DIS,
	USB_EP_ENB_NORMAL,
	USB_EP_ENB_DMA
}USB_EP_ENB_STATE;

/* EP0 state machine, used to handle abnormal cases*/
typedef enum
{
	USB_EP0_TRANS_NONE,
	USB_EP0_TRANS_TX_DATA,
	USB_EP0_TRANS_TX_DATA_END,
	USB_EP0_TRANS_TX_STATUS,
	USB_EP0_TRANS_RX_DATA,
	USB_EP0_TRANS_RX_STATUS,
	USB_EP0_TRANS_END
}USB_EP0_TRANS_TYPE;

/* BDT macro*/
#define USB_BDT_RX		0
#define USB_BDT_TX		1
#define USB_BDT_EVEN	0
#define USB_BDT_ODD	1
#define WRITE_EP0_BDT(direction, even_odd, value)  g_UsbDrvInfo.bdt_ptr[direction][even_odd] = value
#define WRITE_EPN_BDT(ep, direction, value)  g_UsbDrvInfo.bdt_ptr[ep+1][direction] = value

#define WRITE_EP0_BDT_PID(direction, even_odd, value)	g_UsbDrvInfo.bdt_ptr[direction][even_odd]->PID_OWN_DATA01_BC = value
#define WRITE_EPN_BDT_PID(ep, direction, value)	g_UsbDrvInfo.bdt_ptr[ep+1][direction]->PID_OWN_DATA01_BC = value
#define READ_EP0_BDT_PID(direction, even_odd)	g_UsbDrvInfo.bdt_ptr[direction][even_odd]->PID_OWN_DATA01_BC
#define READ_EPN_BDT_PID(ep, direction)	g_UsbDrvInfo.bdt_ptr[ep+1][direction]->PID_OWN_DATA01_BC

#define WRITE_EP0_BDT_ADDR(direction, even_odd, value)	g_UsbDrvInfo.bdt_ptr[direction][even_odd]->ADDRESS = value
#define WRITE_EPN_BDT_ADDR(ep, direction, value)	g_UsbDrvInfo.bdt_ptr[ep+1][direction]->ADDRESS = value;
#define READ_EP0_BDT_ADDR(direction, even_odd)	g_UsbDrvInfo.bdt_ptr[direction][even_odd]->ADDRESS
#define READ_EPN_BDT_ADDR(ep, direction)	g_UsbDrvInfo.bdt_ptr[ep+1][direction]->ADDRESS



typedef struct
{
	USB_POWER_STATE		power_state;
	kal_bool					is_unMaskUSB;	 /*Tricky, used to control interrupt */
	kal_bool					is_ProUSBHISR;

	/* interrupt handler */
	usb_drv_intr_handler_ptr	reset_hdlr;
	usb_drv_intr_handler_ptr	suspend_hdlr;
	usb_drv_intr_handler_ptr	resume_hdlr;
	usb_drv_intr_handler_ptr	ep0_hdlr;
	usb_drv_intr_handler_ptr	ep_in_hdlr[MAX_INTR_EP_NUM];
	usb_drv_intr_handler_ptr	ep_out_hdlr[MAX_INTR_EP_NUM];

	/* stall status */
	kal_bool					ep_in_stall_status[MAX_INTR_EP_NUM];
	kal_bool					ep_out_stall_status[MAX_INTR_EP_NUM];

	/* FIFO flush interrupt */
	kal_bool					ep_in_flush_intr[MAX_INTR_EP_NUM];
	kal_bool					ep_out_flush_intr[MAX_INTR_EP_NUM];

	/*HS FS flag*/
  	kal_bool					Is_HS_mode;


 	/* dma parameters*/
	kal_uint8					dma_pktlength[MAX_DMA_EP_NUM];

	/* dma and fast mode sequnce control parameters*/
	kal_uint8					dma_tx_ep;
 	kal_uint8					dma_res_tx_ep;
 	kal_uint8					fast_mode_tx_ep;
        usb_dma_callback			dma_callback[MAX_DMA_EP_NUM];
	/* BDT addr*/
 	USB_BDT_PTR			bdt_ptr[USB_BDT_NUMBER/2][2];

 	/* EP0 status*/
 	kal_uint8					ep0_rx_even_odd;
 	kal_uint8					ep0_tx_even_odd;
 	kal_uint8 				ep0_current_dir;
 	kal_uint8					ep0_tx_data01;
 	USB_EP0_TRANS_TYPE		ep0_transaction_type;

	/* EPn (others to EP0) status*/
 	USB_EP_ENB_STATE		ep_in_enb_state[MAX_INTR_EP_NUM];
 	USB_EP_ENB_STATE		ep_out_enb_state[MAX_INTR_EP_NUM];
 	kal_uint8					ep_in_max_data_size[MAX_INTR_EP_NUM];
 	kal_uint8					ep_out_max_data_size[MAX_INTR_EP_NUM];
 	kal_uint8					ep_in_data01[MAX_INTR_EP_NUM];
 	kal_uint8					ep_out_data01[MAX_INTR_EP_NUM];

  	kal_uint8					CSR;
	kal_uint8					INTRE;

 #ifdef _USB_VBUS_DETECT_WITH_NIRQ_
	kal_uint8					vbus_timer_handle;    		/*GPT handle*/	
	kal_bool					vbus_intr_allow;
	usb_vbus_callback			vbus_callback;
#endif
}USB_Drv_Info;

#ifdef DEBUG_USB_DRV
/* Only for debug */
#define DBG_BUFF_SIZE   1024

typedef enum 
{
	USB_DBG_HISR_RST,
	USB_DBG_PARSE_FM_TOKEN,
	USB_DBG_PARSE_TOKEN,
	USB_DBG_HISR_EP,
  USB_DBG_HISR_DIR,
  USB_DBG_HISR_EVENODD,
  USB_DBG_HISR_SET_DMA_NULL,
  USB_DBG_SETUPDMA,
  USB_DBG_DMA_CALLBACK,
  USB_DBG_HISR_CLR_FM_TOKEN,
  USB_DBG_HISR_CLR_TOKEN,
  USB_DBG_DRV_MAX
} USB_DBG_DRV_CMD;

/* implement enum and structure */
typedef struct
{
	USB_DBG_DRV_CMD	tag;
	kal_uint32	parameter;
}Usb_Dbg_Info;

typedef struct
{
	Usb_Dbg_Info	dbg_data[DBG_BUFF_SIZE];
	kal_uint16 	dbg_data_idx;
}Usb_Dbg_Drv;

extern void USB_Dbg_Drv_Init(void);
extern void USB_Dbg_Drv_Release(void);
extern void USB_Dbg_Drv_Trace(USB_DBG_DRV_CMD tag, kal_uint32 parameter);

#endif /* DEBUG_USB_DRV */

#endif /* USB_DRV_PRI_H */

