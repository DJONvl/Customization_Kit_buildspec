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

#ifndef _UART_H_
#define _UART_H_

#include "sys/types.h"


//-----------------------------------------------------------------------------
#define NR_UART             (3)
//-----------------------------------------------------------------------------
#define UART_PORT0          0x0
#define UART_PORT1          0x1
#define UART_PORT2          0x2
//-----------------------------------------------------------------------------
struct uart_drv {
   volatile uint8  last_rx_data;
   volatile uint8  _align0;
   volatile uint16 last_lsr;
   uint8           rest_data[32];
   uint32          rest_count;
   uint32          err_threshold;
   uint32          clock;
   uint32          baudrate;
   uint32          base;
};

#ifdef __cplusplus
extern "C" {
#endif

   void uart_set_baudrate(struct uart_drv *drv, uint32 baudrate);
   status_t 	uart_gets(struct uart_drv* drv, uint8* buf, uint32 length);
   void 	uart_puts(struct uart_drv* drv, uint8* buf, uint32 length);

//UART_Log Definition
   void UART_Log_Init(int32 id);
   status_t UART_Log_GetByte_Buffer(uint8* buf, uint32 length);
   status_t UART_Log_PutByte_Buffer(uint8* buf, uint32 length);
   void UART_Log_PurgeFIFO(void);
   void UART_Log_SetBaudRate(uint32 baud_rate);
   void UART_Log_ReInit(uint32 baud_rate);
   void UART_Log_SetErrThreshold(uint32 err_thres);

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
#endif


