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

