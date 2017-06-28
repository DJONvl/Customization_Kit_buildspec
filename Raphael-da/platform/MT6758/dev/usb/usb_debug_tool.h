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


