/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  History:
*  YYYYMMDD MTK12345 Description
*  --------------------
*  20131116 MTK81353 Delete unused code.
*
*
*******************************************************************************/

#ifndef USB_DRV_H
#define USB_DRV_H

/* reserved for future use*/
//#define USB_SUPPORT_MUTIPLE_APP

#include "kal_release.h"
#include "usb_hw.h"
#include "usb_comm.h"

#if defined (DRV_USB_IP_V3)
//#define MAX_EP_NUM		4


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

//#define MAX_EP_NUM			3

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

//04404
#if defined(USB_IP_U3)
typedef enum
{
   USB_EP0_DRV_STATE_SETUP_END,
   USB_EP0_DRV_STATE_SETUP_TO_TX,
   USB_EP0_DRV_STATE_SETUP_TO_RX,
   USB_EP0_DRV_STATE_TX,
   USB_EP0_DRV_STATE_TX_END,
   USB_EP0_DRV_STATE_RX,
   USB_EP0_DRV_STATE_RX_END,
   USB_EP0_DRV_STATE_TRANSACTION_END,
   USB_EP0_DRV_STATE_CLEAR_SENT_STALL
}USB_EP0_DRV_STATE;
#else
typedef enum
{
   USB_EP0_DRV_STATE_READ_END,
   USB_EP0_DRV_STATE_WRITE_RDY,
   USB_EP0_DRV_STATE_TRANSACTION_END,
   USB_EP0_DRV_STATE_CLEAR_SENT_STALL
}USB_EP0_DRV_STATE;
#endif

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
extern void USB_Config_Type(USB_DEVICE_TYPE type, kal_bool b_enable, kal_uint32 *parameter);
extern kal_uint8 USB_ReadAddress(void);
extern kal_bool USB_IS_QMU_Init_Done(void);
extern void USB_QMU_Prepare_GPD(kal_uint8 ep_num, kal_uint8* pBuf, kal_uint32 length, USB_DIR dir);
extern void USB_QMU_Resume(kal_uint8 ep_num, USB_DIR dir);
extern kal_uint32 USB_QMU_Update_Gpd(kal_uint8 ep_num);
extern kal_uint32 USB_QMU_Write(kal_uint8 ep_num, kal_uint8* addr, kal_uint32 length);
extern kal_uint32 USB_QMU_Read(kal_uint8 ep_num, kal_uint8* addr, kal_uint32 length);

extern void USB_Enable_HS(bool enable_hs);

/* system ctrl functions */
extern void USB_Pull_Up_DP_Line(void);
extern void USB_Reset_Drv(void);
extern void USB_Initialize_Drv(void);
extern void USB_EnSysIntr(void);
extern void USB_Resume(void);
extern void USB_Suspend(void);
extern void USB_SetAddress(kal_uint8 addr, USB_SET_ADDR_STATE state);
extern kal_uint16 USB_GetFrameCount(void);
extern void USB_Set_DP_Pull_High(kal_bool bset);

/* EP ctrl functinos */
#ifdef USB_IP_U3
extern void USB_InEPInit(kal_uint8 epno,kal_uint16 data_size, USB_ENDPT_TXFER_TYPE type);
extern void USB_OutEPInit(kal_uint8 epno,kal_uint16 data_size, USB_ENDPT_TXFER_TYPE type);
#else
extern void USB_InEPInit(kal_uint8 epno,kal_uint16 data_size, USB_ENDPT_TXFER_TYPE type);
extern void USB_OutEPInit(kal_uint8 epno,kal_uint16 data_size, USB_ENDPT_TXFER_TYPE type);
#endif

extern void USB_EP0En(void);
extern void USB_InEPEn(kal_uint8 no, kal_bool bDMA);
extern void USB_InEPDis(kal_uint8 no, kal_bool bDMA);
extern void USB_OutEPEn(kal_uint8 no, kal_bool bDMA);
extern void USB_OutEPDis(kal_uint8 no, kal_bool bDMA);
extern void USB_InEPClearData01(kal_uint8 no);
extern void USB_OutEPClearData01(kal_uint8 no);
extern kal_uint16 USB_OutEP_PK_SIZE(kal_uint8 no);
//extern kal_uint8 USB_InEP_PK_SIZE(kal_uint8 no);
extern void USB_Set_BulkIn_Ep(uint8 p_num);

extern void USB_Reset_FIFO(void);

extern void USB_EPFIFORead(kal_uint8 nEP, kal_uint16 nBytes, void *pDst);
extern void USB_EPFIFOWrite (kal_uint8 nEP, kal_uint16 nBytes, void *pSrc);
extern void USB_CtrlEPStall(kal_uint8 EPno, USB_EP_TYPE type, kal_bool en, USB_CTRL_STALL_ENTRY entry);
extern void USB_Get_EP0_Status(kal_bool *p_transaction_end, kal_bool *p_sent_stall);
#ifdef USB_IP_U3
extern void USB_U3_Update_EP0_State(USB_EP0_DRV_STATE state, kal_bool stall, kal_bool end);
#else
extern void USB_Update_EP0_State(USB_EP0_DRV_STATE state, kal_bool stall, kal_bool end);
#endif
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
extern kal_bool USB_IS_Super_Speed(void);


/* polling functions (used for exception log) */
extern void USB_Polling_Transmit_Done(kal_uint8 ep_num);
extern void USB_Polling_Transmit_Data(kal_uint8 ep_num, USB_EP_TYPE  type, kal_uint32 addr, kal_uint32 length, 
                                      usb_dma_callback callback, kal_bool callback_upd_run);
extern void USB_Polling_Recv_Data(kal_uint8 ep_num);

extern kal_bool USB_IS_High_Speed(void);
extern void USB_DMA_Setup(kal_uint8 ep_num, USB_EP_TYPE  direction,kal_uint32 addr, kal_uint32 length);
extern void USBDMA_HISR(void);
extern void USB_Enable_DMA(bool enabled);

extern void USB_HISR(void);


#ifdef _USB_VBUS_DETECT_WITH_NIRQ_
extern void USB_NIRQ_Registration(void (reg_hisr)(void));
#endif

extern void arch_clean_invalidate_cache_range(addr_t start, size_t len);
extern void usb_phy_poweron (void);
#endif //USB_DRV_H



