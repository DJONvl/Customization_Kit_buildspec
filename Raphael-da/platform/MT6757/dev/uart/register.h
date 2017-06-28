/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  History:

*
*******************************************************************************/

#ifndef UART_REGISTER_H
#define UART_REGISTER_H
#include "inc/register_base.h"

#define	UART1_BASE		        AP_UART0_BASE //Sheet PerisysAPB, line 4
#define	UART2_BASE		        AP_UART1_BASE //Sheet PerisysAPB, line 5
#define	UART3_BASE		        AP_UART2_BASE //Sheet PerisysAPB, line 6
//------------------------------------------------------------------------------
// UART Register                                                                
//------------------------------------------------------------------------------
//#define base				AP_UART0_BASE
#define UART_RBR		    (AP_UART0_BASE+0x0000)		/* Read only */
#define UART_THR		    (AP_UART0_BASE+0x0000)		/* Write only */
#define UART_IER		    (AP_UART0_BASE+0x0004)
#define UART_IIR		    (AP_UART0_BASE+0x0008)		/* Read only */
#define UART_FCR		    (AP_UART0_BASE+0x0008)		/* Write only */
#define UART_LCR		    (AP_UART0_BASE+0x000c)
#define UART_MCR		    (AP_UART0_BASE+0x0010)
#define UART_LSR		    (AP_UART0_BASE+0x0014)
#define UART_MSR		    (AP_UART0_BASE+0x0018)
#define UART_SCR		    (AP_UART0_BASE+0x001c)
#define UART_DLL		    (AP_UART0_BASE+0x0090)
#define UART_DLH		    (AP_UART0_BASE+0x0094)
#define UART_FEATURE_SEL	(AP_UART0_BASE+0x009C)
#define UART_EFR		    (AP_UART0_BASE+0x0008)		/* Only when LCR = 0xbf */
#define UART_XON1	        (AP_UART0_BASE+0x0010)		/* Only when LCR = 0xbf */
#define UART_XON2	        (AP_UART0_BASE+0x0014)		/* Only when LCR = 0xbf */
#define UART_XOFF1	        (AP_UART0_BASE+0x0018)		/* Only when LCR = 0xbf */
#define UART_XOFF2	        (AP_UART0_BASE+0x001c)		/* Only when LCR = 0xbf */
#define UART_HIGHSPEED      (AP_UART0_BASE+0x0024)
#define UART_SAMPLE_COUNT   (AP_UART0_BASE+0x0028)
#define UART_SAMPLE_POINT   (AP_UART0_BASE+0x002C)

//UART1 MMP address
#define   UART1_RBR		(UART1_BASE+0x0000)		/* Read only */
#define   UART1_THR		(UART1_BASE+0x0000)		/* Write only */
#define   UART1_IER		(UART1_BASE+0x0004)
#define   UART1_IIR		(UART1_BASE+0x0008)		/* Read only */
#define   UART1_FCR		(UART1_BASE+0x0008)		/* Write only */
#define   UART1_LCR		(UART1_BASE+0x000c)
#define   UART1_MCR		(UART1_BASE+0x0010)
#define   UART1_LSR		(UART1_BASE+0x0014)
#define   UART1_MSR		(UART1_BASE+0x0018)
#define   UART1_SCR		(UART1_BASE+0x001c)
#define   UART1_DLL		(UART1_BASE+0x0000)
#define   UART1_DLH		(UART1_BASE+0x0004)
#define   UART1_EFR		(UART1_BASE+0x0008)		/* Only when LCR = 0xbf */
#define   UART1_XON1	(UART1_BASE+0x0010)		/* Only when LCR = 0xbf */
#define   UART1_XON2	(UART1_BASE+0x0014)		/* Only when LCR = 0xbf */
#define   UART1_XOFF1	(UART1_BASE+0x0018)		/* Only when LCR = 0xbf */
#define   UART1_XOFF2	(UART1_BASE+0x001c)		/* Only when LCR = 0xbf */
#define   UART1_HIGHSPEED       (volatile uint16 *)(UART1_BASE + 0x0024)
#define   UART1_SAMPLE_COUNT    (volatile uint16 *)(UART1_BASE + 0x0028)
#define   UART1_SAMPLE_POINT    (volatile uint16 *)(UART1_BASE + 0x002C)
//UART2 MMP address
#define   UART2_RBR		(UART2_BASE+0x0000)		/* Read only */
#define   UART2_THR		(UART2_BASE+0x0000)		/* Write only */
#define   UART2_IER		(UART2_BASE+0x0004)
#define   UART2_IIR		(UART2_BASE+0x0008)		/* Read only */
#define   UART2_FCR		(UART2_BASE+0x0008)		/* Write only */
#define   UART2_LCR		(UART2_BASE+0x000c)
#define   UART2_MCR		(UART2_BASE+0x0010)
#define   UART2_LSR		(UART2_BASE+0x0014)
#define   UART2_MSR		(UART2_BASE+0x0018)
#define   UART2_SCR		(UART2_BASE+0x001c)
#define   UART2_DLL		(UART2_BASE+0x0000)
#define   UART2_DLH		(UART2_BASE+0x0004)
#define   UART2_EFR		(UART2_BASE+0x0008)		/* Only when LCR = 0xbf */
#define   UART2_XON1	(UART2_BASE+0x0010)		/* Only when LCR = 0xbf */
#define   UART2_XON2	(UART2_BASE+0x0014)		/* Only when LCR = 0xbf */
#define   UART2_XOFF1	(UART2_BASE+0x0018)		/* Only when LCR = 0xbf */
#define   UART2_XOFF2	(UART2_BASE+0x001c)		/* Only when LCR = 0xbf */
#define   UART2_HIGHSPEED	(UART2_BASE+0x0024)		


//IER
#define   UART_IER_ERBFI     		0x0001
#define   UART_IER_ETBEI     		0x0002
#define   UART_IER_ELSI     		0x0004
#define   UART_IER_EDSSI   			0x0008
#define   UART_IER_XOFFI    		0x0020
#define   UART_IER_RTSI    			0x0040
#define   UART_IER_CTSI    			0x0080
#define   IER_HW_NORMALINTS     	0x0005
#define   IER_HW_ALLINTS        	0x0007
#define   IER_SW_NORMALINTS    		0x0025
#define   IER_SW_ALLINTS       		0x0027
#define   UART_IER_ALLOFF			0x0000

//FCR
#define   UART_FCR_FIFOEN       	0x0001
#define   UART_FCR_CLRR    			0x0002
#define   UART_FCR_CLRT    			0x0004
#define   UART_FCR_FIFOINI			0x0007
#define   UART_FCR_RX1Byte_Level    	0x0000
#define   UART_FCR_RX16Byte_Level    	0x0040
#define   UART_FCR_RX32Byte_Level    	0x0080
#define   UART_FCR_RX62Byte_Level    	0x00c0

#define   UART_FCR_TX1Byte_Level    	0x0000
#define   UART_FCR_TX16Byte_Level    	0x0010
#define   UART_FCR_TX32Byte_Level    	0x0020
#define   UART_FCR_TX62Byte_Level    	0x0030
#define   UART_FCR_Normal		(UART_FCR_TX16Byte_Level | UART_FCR_RX32Byte_Level | UART_FCR_FIFOINI)
#define   UART_FCR_MaxFIFO		(UART_FCR_RX62Byte_Level | UART_FCR_TX62Byte_Level | UART_FCR_FIFOINI)

//IIR,RO
#define   UART_IIR_INT_INVALID      	0x0001
#define   UART_IIR_RLS          	0x0006  // Receiver Line Status
#define   UART_IIR_RDA          	0x0004  // Receive Data Available
#define   UART_IIR_CTI          	0x000C  // Character Timeout Indicator
#define   UART_IIR_THRE         	0x0002  // Transmit Holding Register Empty
#define   UART_IIR_MS           	0x0000  // Check Modem Status Register
#define   UART_IIR_SWFlowCtrl          	0x0010  // Receive XOFF characters
#define   UART_IIR_HWFlowCtrl          	0x0020  // CTS or RTS Rising Edge
#define   UART_IIR_FIFOS_ENABLED    	0x00c0
#define   UART_IIR_NO_INTERRUPT_PENDING   0x0001
#define   UART_IIR_INT_MASK         	0x001f

//===============================LCR================================
//WLS
#define   UART_WLS_8    	0x0003
#define   UART_WLS_7    	0x0002
#define   UART_WLS_6    	0x0001
#define   UART_WLS_5    	0x0000
#define   UART_DATA_MASK      0x0003

//Parity
#define   UART_NONE_PARITY    0x0000
#define   UART_ODD_PARITY     0x0008
#define   UART_EVEN_PARITY    0x0018
#define   UART_MARK_PARITY    0x0028
#define   UART_SPACE_PARITY   0x0038
#define   UART_PARITY_MASK    0x0038

//Stop bits
#define   UART_1_STOP         0x0000
#define   UART_1_5_STOP       0x0004  // Only valid for 5 data bits
#define   UART_2_STOP         0x0004
#define   UART_STOP_MASK      0x0004

#define   UART_LCR_DLAB         0x0080
//===============================LCR================================


//MCR
#define   UART_MCR_DTR    	0x0001
#define   UART_MCR_RTS    	0x0002
#define   UART_MCR_LOOPB    	0x0010
#define   UART_MCR_IRE		0x0040	//Enable IrDA modulation/demodulation
#define   UART_MCR_Normal	(UART_MCR_DTR | UART_MCR_RTS)

//LSR
#define   UART_LSR_DR         	0x0001
#define   UART_LSR_OE         	0x0002
#define   UART_LSR_PE         	0x0004
#define   UART_LSR_FE         	0x0008
#define   UART_LSR_BI         	0x0010
#define   UART_LSR_THRE     	0x0020
#define   UART_LSR_TEMT       	0x0040
#define   UART_LSR_FIFOERR    	0x0080

//MSR
#define   UART_MSR_DCTS       0x0001
#define   UART_MSR_DDSR       0x0002
#define   UART_MSR_TERI       0x0004
#define   UART_MSR_DDCD       0x0008
#define   UART_MSR_CTS        0x0010
#define   UART_MSR_DSR        0x0020
#define   UART_MSR_RI         0x0040
#define   UART_MSR_DCD        0x0080

//DLL
//DLM
//EFR
#define   UART_EFR_AutoCTS       	0x0080
#define   UART_EFR_AutoRTS       	0x0040
#define   UART_EFR_Enchance      	0x0010
#define   UART_EFR_SWCtrlMask    	0x000f
#define   UART_EFR_NoSWFlowCtrl   	0x0000
#define   UART_EFR_ALLOFF		0x0000
#define   UART_EFR_AutoRTSCTS       	0x00d0
//Tx/Rx XON1/Xoff1 as flow control word
#define   UART_EFR_SWFlowCtrlX1   	0x001a	
//Tx/Rx XON2/Xoff2 as flow control word
#define   UART_EFR_SWFlowCtrlX2   	0x0005
//Tx/Rx XON1&XON2/Xoff1&Xoff2 as flow control word
#define   UART_EFR_SWFlowCtrlXAll   	0x000f	

#define   UART_TxFIFO_DEPTH		48
#define   UART_RxFIFO_DEPTH		62

#define UART_ReadReg(addr)   		(uint16)(*(volatile uint8 *)addr)
#define UART_WriteReg(addr,data)  *(volatile uint8 *)addr = (uint8)data
#endif

