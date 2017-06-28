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
#ifndef _UART_INTERFACE_H_
#define _UART_INTERFACE_H_

#include "sys/types.h"

extern void UART0_Init(int32 id);
extern status_t UART0_GetByte_Buffer(uint8* buf, uint32 length);
extern status_t UART0_PutByte_Buffer(uint8* buf, uint32 length);


extern void UART0_PurgeFIFO(void);
extern void UART0_SetBaudRate(uint32 baud_rate);
extern void UART0_ReInit(uint32 baud_rate);
extern void UART0_SetErrThreshold(uint32 err_thres);


extern void UART_Log_Init(int32 id);
extern status_t UART_Log_GetByte_Buffer(uint8* buf, uint32 length);
extern status_t UART_Log_PutByte_Buffer(uint8* buf, uint32 length);


extern void UART_Log_PurgeFIFO(void);
extern void UART_Log_SetBaudRate(uint32 baud_rate);
extern void UART_Log_ReInit(uint32 baud_rate);
extern void UART_Log_SetErrThreshold(uint32 err_thres);

#endif


