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


#ifndef USBACM_ADAP_H
#define USBACM_ADAP_H


typedef struct
{
   kal_uint8                      port_no;
   kal_bool                       initialized;
   kal_bool                       power_on;
} UARTStruct;


extern void USB2UART_init(void);
extern void USB2UART_Clear_Tx_Buffer(void);
extern void USB2UART_Clear_Tx_ISR_Buffer(void);
extern void USB2UART_Clear_Rx_Buffer(void);

#endif /* USBACM_ADAP_H */

