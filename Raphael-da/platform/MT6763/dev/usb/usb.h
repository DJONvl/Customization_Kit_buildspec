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
 *    usb.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *    This file intends for usb1.1 definitions.
 *
 * Author:
 * -------
 *   Jensen Hu
 *   CJung Chen
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:   May 31 2005 18:46:38  $
 * $Log:   //mtkvs01/vmdata/Maui_sw/archives/mcu/usb/include/usb.h-arc  $
 *
 * Dec 2 2007 mtk01413
 * [STP100001568] [FlashTool] v3.0748.0 release
 * New features:
 *  1. USB Download on MT6225/MT6226/MT6227 Series BB
 *
 * Mar 9 2007 mtk01309
 * [MAUI_00371068] [USB][New feature] Add code for USB endpoint2 DVT
 * 
 *
 * Jan 24 2007 mtk01309
 * [MAUI_00229395] Fatal Error (a01): Unhandled RTFiles exception (ffffff9a) - USB
 * 
 *
 * Oct 3 2006 mtk01309
 * [MAUI_00334590] [USB normal mode][New feature] Add USB normal mode improve feature
 * 
 *
 * Aug 16 2006 MTK01309
 * [MAUI_00322486] [USB][Change feature] Add a USB reset checking flag to protect abnormal reset condit
 * 
 *
 * Aug 14 2006 MTK01309
 * [MAUI_00321809] [USB][New feature] Modify for engineering mode in order to turn on comport type with
 * 
 *
 * Jul 17 2006 MTK01309
 * [MAUI_00200353] [USB][New feature] Enable USB PictBridge feature
 * 
 *
 * Jul 12 2006 MTK01309
 * [MAUI_00200353] [USB][New feature] Enable USB PictBridge feature
 * 
 *
 * Apr 6 2006 MTK01309
 * [MAUI_00185143] [USB][Change feature]Support OTG testing at EVB without PMIC
 * 
 *
 * Mar 20 2006 MTK01309
 * [MAUI_00180872] [USB][new feature] USB client multiple charging current support
 * 
 *
 * Jan 4 2006 mtk00796
 * [MAUI_00165830] [USB][New Feature] Add OTG, normal mode, and modify USB mode selection functions.
 * Modify endpoint descriptor structure to solve alignment issue.
 *
 * Oct 28 2005 mtk00796
 * [MAUI_00153186] [USB][New feature] Add webcam feature
 * Add IAD related structure and interface.
 * 
 *    Rev 1.8   May 31 2005 19:52:40   mtk00796
 * Support 6228 OTG driver.
 * Resolution for 11310: [USB][New Feature] Add MT6228 functions
 * 
 *    Rev 1.7   May 31 2005 17:50:40   mtk00796
 * Change serial string length from 12 to 15
 * Resolution for 11307: [USB][Bug] Change serial string length from 12 to 15
 * 
 *    Rev 1.6   May 17 2005 03:01:48   BM_Trunk
 * Karlos:
 * add copyright and disclaimer statement
 * 
 *    Rev 1.5   Dec 01 2004 17:55:04   mtk00796
 * Add serial string item to "Usb_Device" strcture.
 * Resolution for 8982: [USB][Bug] Two mass storage phone can not be identified by PC at the same time
 * 
 *    Rev 1.4   Oct 13 2004 11:54:28   mtk00796
 * Add configuration number and interface number variable for compliance test.
 * Resolution for 8207: [USB][BugFix] Modify some feature to pass USB compliance test
 * 
 *    Rev 1.3   Sep 29 2004 21:18:14   mtk00796
 * Remove some element and functions to drv folder and reduce memory usage.
 * Resolution for 7946: [USB][Modify] Move hardware related code from usb folder to drv folder
 * 
 *    Rev 1.2   Jul 01 2004 15:52:42   mtk00796
 * Add assert information.
 * Add parameter for control EP stall function.
 * Resolution for 6404: [USB][Add Feature] Add debug information for assert fail
 * 
 *    Rev 1.1   May 19 2004 10:27:38   mtk00796
 * modify customization and add source header format
 * Resolution for 5510: [USB][modify] modify customization and add source header format
 * 
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#ifndef USB_H
#define USB_H

#include "usb_custom.h"
#include "usb_drv.h"
#include "usb_comm.h"

/***********************************************
	standard command definition
************************************************/

/* Standard Request Codes */
#define USB_GET_STATUS			0x00
#define USB_CLEAR_FEATURE		0x01
#define USB_SET_FEATURE			0x03
#define USB_SET_ADDRESS			0x05
#define USB_GET_DESCRIPTOR		0x06
#define USB_SET_DESCRIPTOR		0x07
#define USB_GET_CONFIGURATION	0x08
#define USB_SET_CONFIGURATION	0x09
#define USB_GET_INTERFACE		0x0A
#define USB_SET_INTERFACE		0x0B
#define USB_SYNCH_FRAME			0x0C

/* Command bit fields */
#define USB_CMD_DATADIR			0x80

/* Request Type Field */
#define USB_CMD_TYPEMASK		0x60
#define USB_CMD_STDREQ			0x00
#define USB_CMD_CLASSREQ		0x20
#define USB_CMD_VENDREQ			0x40
#define USB_CMD_STDDEVIN		0x80
#define USB_CMD_STDDEVOUT		0x00
#define USB_CMD_STDIFIN			0x81
#define USB_CMD_STDIFOUT		0x01
#define USB_CMD_STDEPIN			0x82
#define USB_CMD_STDEPOUT		0x02
#define USB_CMD_CLASSIFIN		0xa1
#define USB_CMD_CLASSIFOUT		0x21

/* Standard command descriptor type*/
#define USB_DEVICE				0x01
#define USB_CONFIG				0x02
#define USB_STRING				0x03
#define USB_INTERFACE			0x04
#define USB_ENDPOINT			0x05
#define USB_DEVICE_QUALIFIER 		0x06
#define USB_OTHER_SPEED			0x07
#define USB_INTERFACE_POWER		0x08
#define USB_OTG_DESC			0x09
#define USB_INTERFACE_ASSOCIATION		0x0b

#define USB_CMD_DESCMASK		0xFF00
#define USB_CMD_DEVICE			(USB_DEVICE<<8)
#define USB_CMD_CONFIG			(USB_CONFIG<<8)
#define USB_CMD_STRING			(USB_STRING<<8)
#define USB_CMD_INTERFACE		(USB_INTERFACE<<8)
#define USB_CMD_ENDPOINT		(USB_ENDPOINT<<8)
#define USB_CMD_DEVICE_QUALIFIER		(USB_DEVICE_QUALIFIER<<8)
#define USB_CMD_OTHER_SPEED			(USB_OTHER_SPEED<<8)
#define USB_CMD_INTERFACE_POWER		(USB_INTERFACE_POWER<<8)

/* Standard Device Feature Selectors */
#define USB_FTR_DEVREMWAKE			0x0001
#define USB_FTR_EPHALT				0x0000
#define USB_FTR_B_HNP_ENB			0x0003
#define USB_FTR_A_HNP_SUPPORT		0x0004
#define USB_FTR_A_ALT_HNP_SUPPORT	0x0005

/***********************************************
	standard descriptor definition
************************************************/

/*Scriptor length*/
#define USB_DEVDSC_LENGTH			18
#define USB_CFGDSC_LENGTH			9
#define USB_IFDSC_LENGTH			9
//#define USB_MAX_IFDSC_LENGTH		28	/* this will change depend on different class interface */
#define USB_MAX_IFDSC_LENGTH		258	/* this will change depend on different class interface */
						/*descriptor requirment*/
						/* 28 is for CDC ACM communication class interface */	
						/* 258 is for video streaming class interface */	
#define USB_EPDSC_LENGTH			7
#define USB_OTGDSC_LENGTH			3
#define USB_MAX_CLASS_EPDESC_LENGTH	12	/* this will change depend on different class interface */
						/*descriptor requirment*/
						/* 12 is for video control interrupt ep */	
#define USB_IAD_LENGTH				8
#define USB_DEV_QUA_DSC_LENGTH		10
#define USB_OTHER_CFGDSC_LENGTH			9

/* Endpoint direction */
#define USB_EP_DIR				0x80
#define USB_EP_DIR_IN			0x80
#define USB_EP_DIR_OUT			0x00

/* Endpoint transfer types */
#define USB_EP_TFMASK			0x03
#define USB_EP_CONTROL			0x00
#define USB_EP_ISO				0x01
#define USB_EP_BULK				0x02
#define USB_EP_INTR				0x03

/*****Descriptor Element*****/
/* Define values for Standard Device Descriptor */
/*For Device descriptor*/
#define USB_DEVDSC_PRODUCT_COMPOSITE	0x0004   /* Product ID */
#define USB_DEVDSC_MAXPKTSIZE			0x08     /* Packet Size */
#define USB_DEVDSC_BCDDEVICE			0x0100   /*Release number of Device*/

#ifdef   __USB_MULTI_CHARGE_CURRENT__
	#if ( defined(MT6305) || defined(MT6318) )
	#define USB_DEVDSC_CONFIGS    			USB_GetChargeCurrentTableSize() 
	#else 
	#define USB_DEVDSC_CONFIGS    			1
	#endif
#else
#define USB_DEVDSC_CONFIGS    			1  /* Number of possible configurations */
#endif


/*For Configuration descriptor*/
#define USB_CFGDSC_ATTR_NATURE			0x80
#define USB_CFGDSC_ATTR_SELFPOWER		0x40
#define USB_CFGDSC_ATTR_REMOTEWAKEUP	0x20
#define USB_CFGDSC_MAXPOWER				(500/2)

#define USB_OTG_HNP_SUPPORT				0x02
#define USB_OTG_SRP_SUPPORT				0x01

/***********************************************
	implement definition
************************************************/

/* define configuration,interface, ep number */
#define USB_MAX_CONFIG				8  /* configuration number */
#define USB_MAX_IAD					1
#define USB_MAX_INTERFACE			3  /* interface number */

#ifdef __ENDPOINT_TWO_TEST__
#define USB_MAX_EP_BULK_IN			2  /* max bulk in ep number */
#define USB_MAX_EP_BULK_OUT		2  /* max bulk in ep number */
#else
#define USB_MAX_EP_BULK_IN			1  /* max bulk in ep number */
#define USB_MAX_EP_BULK_OUT			1  /* max bulk in ep number */
#endif
#define USB_MAX_EP_INTR				1  /* max interrupt ep number */
#define USB_MAX_STRING				10 /* string number */

#define USB_MAX_EP_PER_IF			3 /* 3 is for image class */

/* serial string array size  for that unique Serial number*/
#define USB_SERIAL_STRING_LEN		16

/* define endpoint data parameters */
#define USB_EP_NODATA				-1  /* no more data for endpoint to send */


/***********************************************
	standard command structure
************************************************/

/* usb standard command structure */
typedef struct 
{
	kal_uint8	bmRequestType;
	kal_uint8	bRequest;
	kal_uint16	wValue;
	kal_uint16	wIndex;
	kal_uint16	wLength;
} Usb_Command;



/***********************************************
	standard descriptor structure
************************************************/

/* standard device descriptor */
typedef struct 
{
	kal_uint8    bLength;
	kal_uint8    bDescriptorType;
	kal_uint16   bcdUSB;
	kal_uint8    bDeviceClass;
	kal_uint8    bDeviceSubClass;
	kal_uint8    bDeviceProtocol;
	kal_uint8    bMaxPacketSize0;
	kal_uint16   idVendor;
	kal_uint16   idProduct;
	kal_uint16   bcdDevice;
	kal_uint8    iManufacturer;
	kal_uint8    iProduct;
	kal_uint8    iSerialNumber;
	kal_uint8    bNumConfigurations;
} Usb_Dev_Dscr;

/* standard configuration descriptor */
typedef struct 
{
	kal_uint8    bLength;
	kal_uint8    bDescriptorType;
	kal_uint16   wTotalLength;
	kal_uint8    bNumInterfaces;
	kal_uint8    bConfigurationValue;
	kal_uint8    iConfiguration;
	kal_uint8    bmAttributes;
	kal_uint8    bMaxPower;
} Usb_Cfg_Dscr;

/* standard IAD descriptor */
typedef struct 
{
	kal_uint8    bLength;
	kal_uint8    bDescriptorType;
	kal_uint8    bFirstInterface;
	kal_uint8    bInterfaceCount;
	kal_uint8    bFunctionClass;
	kal_uint8    bFunctionSubClass;
	kal_uint8    bFunctionProtocol;
	kal_uint8    iFunction;
} Usb_IAD_Dscr;

/* standard interface descriptor */
typedef struct 
{
	kal_uint8    bLength;
	kal_uint8    bDescriptorType;
	kal_uint8    bInterfaceNumber;
	kal_uint8    bAlternateSetting;
	kal_uint8    bNumEndpoints;
	kal_uint8    bInterfaceClass;
	kal_uint8    bInterfaceSubClass;
	kal_uint8    bInterfaceProtocol;
	kal_uint8    iInterface;
} Usb_If_Dscr;

/* standard endpoint descriptor */
typedef struct 
{
	kal_uint8    bLength;
	kal_uint8    bDescriptorType;
	kal_uint8    bEndpointAddress;
	kal_uint8    bmAttributes;
	kal_uint8    wMaxPacketSize[2];	
	kal_uint8    bInterval;
} Usb_Ep_Dscr;

/* standard string descriptor */
typedef struct 
{
	kal_uint8    bLength;
	kal_uint8    bDescriptorType;
	kal_uint16   wData[1];
}Usb_String_Dscr;

/* standard string descriptor */
typedef struct 
{
	kal_uint8    bLength;
	kal_uint8    bDescriptorType;
	kal_uint8    bAttribute;
}Usb_Otg_Dscr;

typedef struct 
{
	kal_uint8    bLength;
	kal_uint8    bDescriptorType;
	kal_uint16  bcdUSB;
	kal_uint8    bDeviceClass;
	kal_uint8    bDeviceSubClass;
	kal_uint8    bDeviceProtocol;
	kal_uint8    bMaxPacketSize0;
	kal_uint8    bNumConfigurations;
	kal_uint8    bReserved;
}Usb_Dev_Qual_Dscr;

typedef struct 
{
	kal_uint8    bLength;
	kal_uint8    bDescriptorType;
	kal_uint16  wTotalLength;
	kal_uint8    bNumInterfaces;
	kal_uint8    bConfigurationValue;
	kal_uint8    iConfiguration;
	kal_uint8    bmAttributes;
	kal_uint8    MaxPower;
}Usb_Other_Speed_Cfg_Dscr;

/***********************************************
	implement enum and structure 
************************************************/

/* device states */
typedef enum
{
	DEVSTATE_DEFAULT = 0,
	DEVSTATE_SET_ADDRESS,
	DEVSTATE_ADDRESS,
	DEVSTATE_CONFIG
} USB_DEVICE_STATE;


/* Endpoint 0 state */
typedef enum 
{
	USB_EP0_IDLE=0,
	USB_EP0_RX,
	USB_EP0_TX,
	USB_EP0_RX_STATUS
} USB_EP0_STATE;


/* EP0 status */
typedef struct 
{
	kal_uint8	byFAddr;	/* new function address */
	kal_int32	nBytesLeft;	/* number of bytes left to send in TX mode */
	void		*pData;		/* pointer to data to transmit/receive */
	kal_int32	nBytesRecv;	/* number of bytes received in RX mode */
} Usb_Ep0_Status;

/* Bulk IN endpoint status */
typedef struct 
{
	kal_uint8	byEP;		/* endpoint number */
	void		*pData;		/* pointer to data buffer */
	kal_int32	nBytesLeft;	/* number of bytes left to send */
} Usb_EpBIn_Status;

/* Bulk OUT endpoint status */
typedef struct 
{
	kal_uint8	byEP;		/* endpoint number */
	void		*pData;		/* pointer to data buffer */
	kal_int32	nBuffLen;	/* length of data buffer */
	kal_int32	nBytesRecv;	/* number of bytes received */
} Usb_EpBOut_Status;


/* interface create function pointer */
typedef void (*usb_create_if_func_ptr)(void *);
/* interface speed reset function pointer */
typedef void (* usb_speed_if_func_ptr)(kal_bool bset);
/* EP0 rx interrupt handler */
typedef void (*usb_ep0_rx_ptr)(void *);
/* class specific command interrupt handler */
typedef void (*usb_class_specific_handler_ptr)(Usb_Ep0_Status *, Usb_Command*);
/* endpoint interrupt handler */
typedef void (*usb_ep_handler_ptr)(void);
/* EP0 command  interrupt handler */
typedef void (*usb_ep0_cmd_ptr)(Usb_Command *pcmd);

/* endpoint information, including endpoint status */
typedef struct 
{
	union 
	{
		Usb_Ep_Dscr			stdep;			/* ep descriptor */
		kal_uint8			classep[USB_MAX_CLASS_EPDESC_LENGTH];
	}epdesc;
	kal_uint16				epdscr_size;		/* descriptor length */
	usb_ep_handler_ptr		ep_reset;		/* ep reset handler */
	union 
	{
		Usb_EpBIn_Status	epin_status;
		Usb_EpBOut_Status	epout_status;
	}ep_status;						/* ep status */
} Usb_Ep_Info;


/* interface information, including endpoint information and interface information*/
//extern struct Usb_Cdc_If_Dscr;
typedef struct 
{
	kal_char			*interface_name_ptr;	/* interface name */
	usb_class_specific_handler_ptr	if_class_specific_hdlr;	/* interface specific handler, handle ep0 class specific request */
	kal_uint16			ifdscr_size;		/* descriptor length */
	union 
	{
		Usb_If_Dscr		stdif;
		kal_uint8		classif[USB_MAX_IFDSC_LENGTH];
	}ifdscr;	
	Usb_Ep_Info			*ep_info[USB_MAX_EP_PER_IF];/* endpoint attach to the interface info */	
} Usb_Interface_Info;

/* configuration information */
typedef struct 
{	
	kal_char			*cfg_name_ptr;		/* config name */
	Usb_Cfg_Dscr		stdcfg;			/* config descriptor */
}Usb_Config_Info;

typedef struct
{
	kal_bool			b_enable;
	kal_uint8			cmd;
	usb_ep0_cmd_ptr	ep0_cmd_hdlr;
}Usb_EPo_Cmd_Hdler;

/* device information, used for usb level */
typedef struct 
{
	Usb_Dev_Dscr		devdscr;
	/* Usb_Config_Info		cfg_info[USB_MAX_CONFIG]; */
	/*  the configuration only one for saving memory  */
	Usb_Config_Info		cfg_info;
	Usb_Interface_Info	if_info[USB_MAX_INTERFACE];
	Usb_Ep_Info			ep_bulkin_info[USB_MAX_EP_BULK_IN];
	Usb_Ep_Info			ep_bulkout_info[USB_MAX_EP_BULK_OUT];
	Usb_Ep_Info			ep_intr_info[USB_MAX_EP_INTR];
	Usb_String_Dscr		*resource_string[USB_MAX_STRING];
//	Usb_IAD_Dscr		iad_desc[USB_MAX_IAD];
	
#ifdef USB_HS_ENABLE
	Usb_Dev_Qual_Dscr			dev_qual_dscr;
	Usb_Other_Speed_Cfg_Dscr		other_speed_cfg_dscr;
	USB_TEST_MODE_TYPE			usb_test_type;
	kal_bool				usb_enter_test_mode;
#endif	
	kal_uint8 			*conf;				/*just for descriptor transfer*/
	Usb_Command			cmd;				/*USB_COMMAND*/
	USB_DEVICE_TYPE 	device_type;
	USB_DEVICE_STATE	nDevState;			/*DEVSTATE_CONFIG,DEVSTATE_ADDRESS,DEVSTATE_DEFAULT*/		
	kal_bool			remoteWk;
	kal_bool			self_powered;
	kal_bool  		usb_comport_boot;		/* a flag to check if usb boot as com port */
	kal_bool			is_configured_now;		/* a flag to check that after reset and before set configuration, the all DMA call back messages will be ignored */
	kal_uint8			config_num;
	kal_uint8			interface_num;
	USB_EP0_STATE 		ep0_state;
   	Usb_Ep0_Status    	ep0info;
   	usb_ep0_rx_ptr  	ep0_rx_handler;
   	Usb_EPo_Cmd_Hdler	ep0_class_cmd_handler; /* class specific ep0 cmd */
	kal_bool 			usb_config_result; 	/* configure may be fail */

	/* used for resource management*/
   	kal_uint8			resource_ep_bulkin_number;  		
   	kal_uint8			resource_ep_bulkout_number;		
   	kal_uint8			resource_ep_intr_number;
   	kal_uint8			resource_interface_number;
   	kal_uint8			resource_string_number;   
	kal_uint8			resource_iad_number;   	
   	
#ifdef __USB_MULTI_CHARGE_CURRENT__
	#if ( defined(MT6305) || defined(MT6318) )
       /* add for multi-configurations*/
	kal_uint8			multi_configuration_value[USB_MAX_CONFIG];
	kal_uint8			multi_Max_Power[USB_MAX_CONFIG];
	#endif
#endif
   	/* serial string unique for each phone*/
   	kal_uint16			serial_string[USB_SERIAL_STRING_LEN]; 

	/* customizable variables*/
	const USB_DEVICE_PARAM	*device_param;
} Usb_Device;

/*add for setup preloader comport when scan comport*/
typedef struct
{
	U16 pid;
	U16 vid;
} Usb_SetUp_Info;

/***********************************************
	function and global variable
************************************************/
extern Usb_Device gUsbDevice;
extern Usb_SetUp_Info gUsbSetupInfo;


extern void USB_Init_Device_Status(void);
extern void USB_Release_Device_Status(void);
extern void USB_Register_EP0_RxHdlr(usb_ep0_rx_ptr handler);
extern void USB_Register_EP0_Class_CmdHdlr(kal_uint8 cmd, usb_ep0_cmd_ptr handler);
extern void USB_Init(USB_DEVICE_TYPE type, kal_bool b_enable);
extern void USB_Release(void);

extern void USB_Generate_EP0Data(Usb_Ep0_Status*pep0state, Usb_Command*pcmd, void *data, kal_int32 len);
#ifdef  __USB_MULTI_CHARGE_CURRENT__
	#if ( defined(MT6305) || defined(MT6318) )
	extern const kal_uint16  USB_CHARGE_CURRENT[];
	extern const kal_uint8  USB_GetChargeCurrentTableSize(void);
	#endif
#endif

#endif /* USB_H */

