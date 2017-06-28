/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _UART_H_
#define _UART_H_

#include "SW_TYPES.H"
#include "mtk_mcu.h"
//-----------------------------------------------------------------------------
typedef enum {
    COM_UNKNOWN =   0x00,
    COM_UART1   =   0x01,
    COM_USB     =   0x02,
    COM_MAX
} COM_TYPE;
//-----------------------------------------------------------------------------
#define NR_UART             (4)
//-----------------------------------------------------------------------------
#define UART_PORT1          0x0000
#define UART_PORT2          0x0001
#define UART_PORT3          0x0002
#define UART_PORT4          0x0003
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
//-----------------------------------------------------------------------------
typedef struct {
   void    	(* m_COM_Init)						(void);
   uint8   	(* m_COM_GetByte)					(void);
   bool    	(* m_COM_GetByteStatus)				(uint8 *data);
   bool		(* m_COM_GetByte_Buffer)			(uint32* buf, uint32 length);
   void    	(* m_COM_PutByte)					(uint8 data);
   void 	(* m_COM_PutByte_Buffer)			(uint32* buf, uint32 length);
   void 	(* m_COM_PutByte_Complete)			(uint8 data);
   uint16  	(* m_COM_GetData16)					(void);
   void    	(* m_COM_PutData16)					(uint16 data);
   uint32  	(* m_COM_GetData32)					(void);
   void    	(* m_COM_PutData32)					(uint32 data);
   uint64  	(* m_COM_GetData64)					(void);
   void    	(* m_COM_PutData64)					(uint64 data);
   void		(* m_COM_PurgeFIFO)					(void);
   void		(* m_COM_SetBaudRate)				(uint8 baud_rate, BBCHIP_TYPE bbchip_type);
} COM_Driver;
//-----------------------------------------------------------------------------
extern 	COM_Driver	g_COM_Driver;
//-----------------------------------------------------------------------------
#define	COM_Init									g_COM_Driver.m_COM_Init
#define	COM_GetByte									g_COM_Driver.m_COM_GetByte
#define	COM_GetByteStatus(data)						g_COM_Driver.m_COM_GetByteStatus(data)
#define	COM_GetByte_Buffer(buf,length)				g_COM_Driver.m_COM_GetByte_Buffer(buf, length)
#define	COM_PutByte(data)							g_COM_Driver.m_COM_PutByte(data)
#define	COM_PutByte_Buffer(buf,length)				g_COM_Driver.m_COM_PutByte_Buffer(buf, length)
#define	COM_PutByte_Complete(data)					g_COM_Driver.m_COM_PutByte_Complete(data)
#define	COM_GetData16								g_COM_Driver.m_COM_GetData16
#define	COM_PutData16(data)							g_COM_Driver.m_COM_PutData16(data)
#define	COM_GetData32								g_COM_Driver.m_COM_GetData32
#define	COM_PutData32(data)							g_COM_Driver.m_COM_PutData32(data)
#define	COM_GetData64								g_COM_Driver.m_COM_GetData64
#define	COM_PutData64(data)							g_COM_Driver.m_COM_PutData64(data)
#define	COM_PurgeFIFO								g_COM_Driver.m_COM_PurgeFIFO
#define	COM_SetBaudRate(baud_rate, bbchip_type)		g_COM_Driver.m_COM_SetBaudRate(baud_rate, bbchip_type)
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//-- UART1

//- UART1 DL Mode
extern void		COM_Delay(void);
extern bool		COM_Is_USB_Download_Mode(void);
extern uint32	COM_Get_HW_Packet_Size(void);

extern struct uart_drv *uart_init(int id, uint32 clock, uint32 baudrate);
extern bool     uart_reinit(struct uart_drv *drv, uint32 clock, uint32 baudrate);
extern uint8    uart_getc(struct uart_drv* drv);
extern bool     uart_getc_status(struct uart_drv* drv, uint8 *data);
extern bool     uart_gets(struct uart_drv* drv, uint32* buf, uint32 length);
extern void     uart_putc(struct uart_drv* drv, uint8 data);
extern void     uart_puts(struct uart_drv* drv, uint32* buf, uint32 length);
extern void     uart_putc_complete(struct uart_drv* drv, uint8 data);
extern uint16   uart_getw(struct uart_drv *drv);
extern void     uart_putw(struct uart_drv *drv, uint16 data);
extern uint32   uart_getl(struct uart_drv *drv);
extern void     uart_putl(struct uart_drv *drv, uint32 data);
extern void     uart_purge(struct uart_drv *drv);
extern void     uart_set_baudrate(struct uart_drv *drv, uint32 baudrate);


//- UART1 Internal Definition
extern void		UART1_Init(void);
extern uint8	UART1_GetByte(void);
extern bool		UART1_GetByteStatus(uint8 *data);
extern bool		UART1_GetByte_Buffer(uint32* buf, uint32 length);
extern void		UART1_PutByte(uint8 data);
extern void 	UART1_PutByte_Buffer(uint32* buf, uint32 length);
extern void 	UART1_PutByte_Complete(uint8 data);
extern uint16	UART1_GetData16(void);			//ok, high byte is first
extern void		UART1_PutData16(uint16 data);	//ok, high byte is first
extern uint32	UART1_GetData32(void);			//ok, high byte is first
extern void		UART1_PutData32(uint32 data);	//ok, high byte is first
extern uint64	UART1_GetData64(void);			//ok, high byte is first
extern void		UART1_PutData64(uint64 data);	//ok, high byte is first
extern void		UART1_PurgeFIFO(void);
extern void		UART1_SetBaudRate(uint8 baud_rate, BBCHIP_TYPE bbchip_type);
extern void     UART1_ReInit(uint8 baud_rate, BBCHIP_TYPE bbchip_type);
extern void     UART1_SetErrThreshold(uint32 err_thres);

//- USB Internal Definition
extern void		USB2COM_Init(void);
extern void		USB2COM_Setup_PORT(void);
extern uint8	USB2COM_GetByte(void);
extern bool		USB2COM_GetByteStatus(uint8 *data);
extern bool		USB2COM_GetByte_Buffer(uint32* buf, uint32 length);
extern void		USB2COM_PutByte(uint8 data);
extern void 	USB2COM_PutByte_Buffer(uint32* buf, uint32 length);
extern void 	USB2COM_PutByte_Complete(uint8 data);
extern uint16	USB2COM_GetData16(void);			//ok, high byte is first
extern void		USB2COM_PutData16(uint16 data);	//ok, high byte is first
extern uint32	USB2COM_GetData32(void);			//ok, high byte is first
extern void		USB2COM_PutData32(uint32 data);	//ok, high byte is first
extern uint64	USB2COM_GetData64(void);			//ok, high byte is first
extern void		USB2COM_PutData64(uint64 data);	//ok, high byte is first
extern void		USB2COM_PurgeFIFO(void);
extern void		USB2COM_SetBaudRate(uint8 baud_rate, BBCHIP_TYPE bbchip_type);
extern uint32   USB2COM_Get_USB_MAX_PACKET(void);
extern void     USB2COM_SetErrThreshold(uint32 err_thres);

//-- UART Utility function
extern void 	COM_ReturnNAckStatus(uint32 error_status);

#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------
#endif

