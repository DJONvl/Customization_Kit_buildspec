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


#ifndef USB_RESOURCE_H
#define USB_RESOURCE_H


#define __USB_HS_ENABLE__  //QQ


/***********************************************
implement enum and structure 
************************************************/

/* interface create infomration structrue, use this information to create interface */
typedef struct 
{
   kal_char		*if_name_ptr;
   usb_create_if_func_ptr	if_create_func;
   VOID_FUNC		if_init_func;
#ifdef __USB_HS_ENABLE__
   usb_speed_if_func_ptr	if_speed_func;
#endif
} Usb_IfCreate_Info;


typedef struct
{
   kal_uint8 	device_code; 	/*device class*/
   kal_uint8 	subclass_code;	
   kal_uint8 	protocol_code;
   kal_uint16 	product_code;
   kal_bool	b_registerd;
} Usb_Device_Code;


/***********************************************
function and global variable
************************************************/

extern void USB_Register_CreateFunc(kal_char* if_name, usb_create_if_func_ptr if_create_func, VOID_FUNC if_init_func, 
                                    usb_speed_if_func_ptr if_speed_func);
extern void USB_Register_Device_Code(kal_uint8 device_code, kal_uint8 subclass_code, kal_uint8 protocol_code, kal_uint16 product_code);
extern void USB_DeRegister_CreateFunc(void);
extern void USB_DeRegister_Device_Code(void);
extern Usb_Ep_Info* USB_Get_Intr_Ep(kal_uint8 *p_num);
extern Usb_Ep_Info* USB_Get_BulkIn_Ep(kal_uint8 *p_num);
extern Usb_Ep_Info* USB_Get_BulkOut_Ep(kal_uint8 *p_num);
extern Usb_Interface_Info* USB_Get_Interface_Number(kal_uint8 *p_num);
extern kal_uint8 USB_Get_String_Number(void *usb_string);
//extern Usb_IAD_Dscr* USB_Get_IAD_Number(kal_uint8 *p_num);
extern void USB_Software_Create(void);
extern void USB_Software_Speed_Init(kal_bool b_other_speed);
extern void USB_Software_Init(void);
/* set interrupt endpoint number */
extern void USB_Set_Intr_Ep(kal_uint8 p_num);
/* set bulk in endpoint number */
extern void USB_Set_BulkIn_Ep(kal_uint8 p_num);
/* set bulk out endpoint number */
extern void USB_Set_BulkOut_Ep(kal_uint8 p_num);

#endif /* USB_RESOURCE_H */

