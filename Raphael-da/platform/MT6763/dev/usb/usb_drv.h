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
 *    usb_drv.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *  This file intends for usb hardware related function definitions
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
 * $Modtime:   Jul 04 2005 13:16:18  $
 * $Log:   //mtkvs01/vmdata/Maui_sw/archives/mcu/interface/hwdrv/usb_drv.h-arc  $
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
 * Jul 5 2006 MTK01309
 * [MAUI_00200353] [USB][New feature] Enable USB PictBridge feature
 * 
 *
 * Jan 27 2006 mtk00796
 * [MAUI_00170775] [USB][Bug] Webcam in 6228 platform sometimes stop when changing size.
 * Add clear data01 functions.
 *
 * Dec 30 2005 mtk00796
 * [MAUI_00164746] [USB][Modify] Create HISR when USB task initialize instead of after selecting USB fu
 * Add USB HISR create function.
 *
 * Dec 28 2005 mtk00796
 * [MAUI_00164272] [USB][Bug] 6219 USB device can not be detected by 6228 OTG host.
 * Add clear OUT FIFO function.
 *
 * Nov 7 2005 mtk00796
 * [MAUI_00154612] [UPLL] Turn on USB PDN when enable UPLL
 * Add UPLL as USB PDN owner. 
 * MT6228 has to turn on USB PDN in order to turn on UPLL.
 *
 * Oct 28 2005 mtk00796
 * [MAUI_00153186] [USB][New feature] Add webcam feature
 * Add USB driver interface.
 * 
 *    Rev 1.9   Jul 04 2005 13:20:00   mtk00796
 * Add USB PDN enable/disable functions.
 * Resolution for 11804: [USB][New Feature] Provide USB PDN enable/disable function for IRDA
 * 
 *    Rev 1.8   Jun 13 2005 23:07:24   mtk00796
 * Add nIRQ registration function to detect VBUS.
 * Resolution for 11522: [USB][Change Design] Use nIRQ to detect Vbus instead of EINT
 * 
 *    Rev 1.7   May 31 2005 19:43:58   mtk00796
 * Support 6228 Device and OTG driver.
 * Resolution for 11310: [USB][New Feature] Add MT6228 functions
 * 
 *    Rev 1.6   May 17 2005 00:29:20   BM_Trunk
 * Karlos:
 * add copyright and disclaimer statement
 * 
 *    Rev 1.5   Mar 17 2005 15:40:42   mtk00796
 * Add polling function for the use of exception log.
 * Resolution for 10169: [USB][New Feature] Add feature of dump exception log via USB
 * 
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef USB_DRV_H
#define USB_DRV_H

/* reserved for future use*/
//#define USB_SUPPORT_MUTIPLE_APP
#define USB_QMU

#if defined (DRV_USB_IP_V3)
#define MAX_EP_NUM		4


#define MAX_TX_EP_NUM		4
#define MAX_RX_EP_NUM		3


#define USB_EP_BULK_MAXP_HS	512  	/* maximum packet size for high-speed bulk endpoints */
#define USB_EP_BULK_MAXP_FS	64  		/* maximum packet size for full-speed bulk endpoints */

#define USB_EP_ISO_MAXP_HB      (1024|(0x02<<11))
#define USB_EP_ISO_MAXP_HB_HS	3072 	/* maximum packet size for high-speed, high bandwidth isochronous endpoints */
#define USB_EP_ISO_MAXP_HS	1024 	/* maximum packet size for high-speed isochronous endpoints */
#define USB_EP_ISO_MAXP_FS	 	  512  	/* maximum packet size for full-speed isochronous endpoints */

#define USB_EP_INTR_MAXP_HB	(1024|(0x02<<11))

#define USB_EP_INTR_MAXP_HB_HS	3072   	/* maximum packet size for high-speed, high bandwidth interrupt endpoints */
#define USB_EP_INTR_MAXP_HS	 64//1024  	/* maximum packet size for high-speed interrupt endpoints */
#define USB_EP_INTR_MAXP_FS	64   		/* maximum packet size for full-speed interrupt endpoints */

#define USB_EP_RX_MAX_COUNT	8192	/* because endpoint rx count only minus 4, so maybe we see here and ot is a minus number */

#define USB_EP_INTR_MAXP  64
#define USBDL_EP_BULK_MAXP		512

#else  //USB_IP_V2 /V1

#define MAX_EP_NUM			3

#define MAX_TX_EP_NUM		3
#define MAX_RX_EP_NUM		2


#define USB_EP_BULK_MAXP	64  /* maximum packet size for bulk endpoints */

#define USB_EP_ISO_MAXP	64  /* maximum packet size for bulk endpoints */

 /* maximum packet size for interrupt endpoints */
#if defined(DRV_USB_IP_V1)
#define USB_EP_INTR_MAXP	16
#elif  defined(DRV_USB_IP_V2) 
#define USB_EP_INTR_MAXP	8
#endif


/* maximum packet size for USB Download */
#if defined(DRV_USB_IP_V1)
#define USBDL_EP_BULK_MAXP		32
#elif  defined(DRV_USB_IP_V2) 
#define USBDL_EP_BULK_MAXP		64
#endif

#endif


/* USB PDN owner id */
typedef enum
{
	USB_PDN_OWNER_USB,
	USB_PDN_OWNER_IRDA,
	USB_PDN_OWNER_UPLL	
} USB_PDN_OWNER;

/* IN, OUT pipe index for ep_number */
typedef enum 
{
	USB_IN_EP_TYPE=0,
	USB_OUT_EP_TYPE
} USB_EP_TYPE;


/* CTRL, BULK, INTR, ISO endpoint */
typedef enum
{
	USB_ENDPT_CTRL,
	USB_ENDPT_BULK,
	USB_ENDPT_INTR,
	USB_ENDPT_ISO
}USB_ENDPT_TXFER_TYPE;

/* add for debug*/
typedef enum
{
	USB_CTRL_STALL_ENTRY_1 = 0,
	USB_CTRL_STALL_ENTRY_2,	
	USB_CTRL_STALL_ENTRY_3,
	USB_CTRL_STALL_ENTRY_4,
	USB_CTRL_STALL_ENTRY_5		/* for image class */	
}USB_CTRL_STALL_ENTRY;

/* endpoint 0 hardware control state*/
typedef enum
{
	USB_EP0_DRV_STATE_READ_END,
	USB_EP0_DRV_STATE_WRITE_RDY,
	USB_EP0_DRV_STATE_TRANSACTION_END,
	USB_EP0_DRV_STATE_CLEAR_SENT_STALL
}USB_EP0_DRV_STATE;

/* interrupt handler type*/
typedef enum
{
	USB_DRV_HDLR_RESET,
	USB_DRV_HDLR_EP0,
	USB_DRV_HDLR_EP_IN,
	USB_DRV_HDLR_EP_OUT
}USB_DRV_HDLR_TYPE;

/* The set address action may be different for different hardware, 
    This is the current state for the set address function */
typedef enum
{
	USB_SET_ADDR_DATA,
	USB_SET_ADDR_STATUS
}USB_SET_ADDR_STATE;


/* interrupt handler function type */
typedef void (*usb_drv_intr_handler_ptr)(void);
/* application dma callback function type */
typedef void (*usb_dma_callback)(void);

#ifdef _USB_VBUS_DETECT_WITH_NIRQ_
typedef void (*usb_vbus_callback)(void);
#endif


/* driver info functions */
extern void USB_Init_Drv_Info(void);
extern void USB_ResetDrv_Info(void);
extern void USB_Register_Drv_Info(USB_DRV_HDLR_TYPE type, kal_uint32 ep_num, usb_drv_intr_handler_ptr hdlr);
extern void USB_Set_UnMask_Irq(kal_bool set);
extern kal_bool USB_Get_UnMask_Irq(void);
extern kal_bool USB_Get_Pro_USB_HISR(void);
extern kal_bool USB_Get_EP_Stall_Status(kal_uint8 ep_num, USB_EP_TYPE type);
extern void USB_Drv_Activate_ISR(void);
extern void USB_Drv_Create_ISR(void);


/* system ctrl functions */
extern void USB_Pull_Up_DP_Line(void);
extern void USB_Reset_Drv(void);
extern void USB_Initialize_Drv(void);
extern void USB_EnSysIntr(void);
extern void USB_Resume(void);
extern void USB_Suspend(void);
extern void USB_SetAddress(kal_uint8 addr, USB_SET_ADDR_STATE state);
extern kal_uint16 USB_GetFrameCount(void);

/* EP ctrl functinos */
extern void USB_InEPInit(kal_uint8 epno,kal_uint8 data_size, USB_ENDPT_TXFER_TYPE type);
extern void USB_OutEPInit(kal_uint8 epno,kal_uint8 data_size, USB_ENDPT_TXFER_TYPE type);

extern void USB_EP0En(void);
extern void USB_InEPEn(kal_uint8 no, kal_bool bDMA);
extern void USB_InEPDis(kal_uint8 no, kal_bool bDMA);
extern void USB_OutEPEn(kal_uint8 no, kal_bool bDMA);
extern void USB_OutEPDis(kal_uint8 no, kal_bool bDMA);
extern void USB_InEPClearData01(kal_uint8 no);
extern void USB_OutEPClearData01(kal_uint8 no);
extern kal_uint16 USB_OutEP_PK_SIZE(kal_uint8 no);
//extern kal_uint8 USB_InEP_PK_SIZE(kal_uint8 no);

extern void USB_Reset_FIFO(void);

extern void USB_EPFIFORead(kal_uint8 nEP, kal_uint16 nBytes, void *pDst);
extern void USB_EPFIFOWrite (kal_uint8 nEP, kal_uint16 nBytes, void *pSrc);
extern void USB_CtrlEPStall(kal_uint8 EPno, USB_EP_TYPE type, kal_bool en, USB_CTRL_STALL_ENTRY entry);
extern void USB_Get_EP0_Status(kal_bool *p_transaction_end, kal_bool *p_sent_stall);
extern void USB_Update_EP0_State(USB_EP0_DRV_STATE state, kal_bool stall, kal_bool end);
extern kal_uint32 USB_EP0_Pkt_Len(void);
extern void USB_EP_Bulk_In_Ready(kal_uint8 ep_num);
extern void USB_EP_Poll_Bulk_In_Ready(kal_uint8 ep_num);
extern kal_bool USB_EP_Poll_Bulk_Out_Ready(kal_uint8 ep_num);
extern kal_bool USB_EP_Poll_Bulk_Out_Intr(kal_uint8 ep_num);
extern kal_bool USB_Poll_RX_Data(kal_uint8 ep_num,kal_uint32* addr, kal_uint32 length);
extern kal_uint32 USB_EP_Out_Pkt_Len(kal_uint8 ep_num);
extern void USB_EP_Bulk_Out_Ready(kal_uint8 ep_num);
extern kal_bool USB_Is_EP_Bulk_In_Empty(kal_uint8 ep_num);
extern void USB_Clear_IN_EP_FIFO(kal_uint8 ep_num);
extern void USB_Clear_OUT_EP_FIFO(kal_uint8 ep_num);

/* polling functions (used for exception log) */
extern void USB_Polling_Transmit_Done(kal_uint8 ep_num);
extern void USB_Polling_Transmit_Data(kal_uint8 ep_num, USB_EP_TYPE  type, kal_uint32 addr, kal_uint32 length, 
						usb_dma_callback callback, kal_bool callback_upd_run);
extern void USB_Polling_Recv_Data(kal_uint8 ep_num);

extern kal_bool USB_IS_High_Speed(void);
extern void USB_DMA_Setup(kal_uint8 ep_num, USB_EP_TYPE  direction,kal_uint32 addr, kal_uint32 length);
extern void USBDMA_HISR(void);

extern void USB_HISR(void);
								

#ifdef _USB_VBUS_DETECT_WITH_NIRQ_
extern void USB_NIRQ_Registration(void (reg_hisr)(void));
#endif
								
#endif //USB_DRV_H
 


