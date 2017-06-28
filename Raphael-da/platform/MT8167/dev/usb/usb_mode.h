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

