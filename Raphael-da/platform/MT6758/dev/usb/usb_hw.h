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

#ifndef USB_HW_H
#define USB_HW_H
#include <autoconfig.h>
#define USB_IP_U3
#define SUPPORT_U3	

// USB_PIO_ONLY defined at project/MTXXXX.mk
#if (ENABLE_USB_DMA == 0)
#define USB_PIO_ONLY
#endif
		


/**
*  @U3D register map
*/              

/*U3D register bank*/
#define U3D_BASE                        (0x11800000)
#define SSUSB_DEV_BASE                  (U3D_BASE+0x1000)
#define SSUSB_EPCTL_CSR_BASE            (U3D_BASE+0x1800)
#define SSUSB_USB3_MAC_CSR_BASE         (U3D_BASE+0x2400)
#define SSUSB_USB3_SYS_CSR_BASE         (U3D_BASE+0x2400)
#define SSUSB_USB2_CSR_BASE             (U3D_BASE+0x3400)

#define SIFSLV_BASE                     (0x118A0000)
#define SSUSB_SIFSLV_IPPC_BASE          (SIFSLV_BASE+0x700)

#define U3D_USB30_PHYA_REG0 (SSUSB_USB30_PHYA_SIV_B_BASE+0x0000)
#define SSUSB_USB30_PHYA_SIV_B_BASE (USB_SIF2_BASE+0xB00)
#define USB_SIF2_BASE (0x10440000)
#define RG_SSUSB_VUSB10_ON (1<<5)

#define MAX_INTR_EP_NUM		6
#define USB_INTR_MAXP	8   /* maximum packet size for interrupt endpoints */
#define USB_EP_INTR_MAXP_FS 8
#define USB_PHY_BASE		(0x11210800) /* USB_SIF_BASE + 0x800 */

#define USB_BULK_SS_MAXP 		1024
#define USB_BULK_HS_MAXP 		512
#define USB_BULK_MAXP 	        64
#define USB_EP0_SS_MAXP			512
#define USB_EP0_SS_MAXP_EXP 	9
#define USB_EP0_MAXP			64

#define MAX_EP_NUM 2

#define QMU_MAX_SZ_PER_GPD	64512		// 63*1024

#ifdef SUPPORT_U3 
//EP0, TX, RX has separate SRAMs
#define USB_TX_FIFO_START_ADDRESS  0
#define USB_RX_FIFO_START_ADDRESS  0

#define EPX_bINTERVAL 0
#define EPX_SLOT 1				//04404_test
#define EPX_BURST 15
#define EPX_MULT 0 

#define U2_U3_SWITCH
#define U3D_DFT_SPEED SSUSB_SPEED_SUPER
#else
//EP0, TX, RX share one SRAM. 0-63 bytes are reserved for EP0
#define USB_TX_FIFO_START_ADDRESS  (64)
#define USB_RX_FIFO_START_ADDRESS  (64+512*MAX_EP_NUM)

#define EPX_bINTERVAL 0
#define EPX_SLOT 0
#define EPX_BURST 0
#define EPX_MULT 0 

#define U3D_DFT_SPEED SSUSB_SPEED_HIGH
#endif


//=================================================================//
#include "kal_release.h"

#include "ssusb_dev_c_header.h"
#include "ssusb_epctl_csr_c_header.h"
#include "ssusb_sifslv_ippc_c_header.h"

#include "ssusb_usb3_mac_csr_c_header.h"
#include "ssusb_usb3_sys_csr_c_header.h"
#include "ssusb_usb2_csr_c_header.h"
#include "mu3d_hal_qmu_drv.h"	// modify this

//#include "mu3d_hal_hw.h"

typedef enum {
   RET_SUCCESS = 0,
   RET_FAIL,
} USB_RESULT;

typedef enum
{
   USB_TX = 0, 
   USB_RX
}USB_DIR;

typedef enum {
   SSUSB_SPEED_INACTIVE = 0,
   SSUSB_SPEED_FULL = 1,
   SSUSB_SPEED_HIGH = 3,
   SSUSB_SPEED_SUPER = 4,
} USB_SPEED;


typedef struct
{
   TGPD gpd[2];
   kal_uint8 cur_gpd_index;
   TGPD *cur_ptr_gpd;
}gpd_pool;

#define PIO_MODE 1
#define DMA_MODE 2
#define QMU_MODE 3
#ifdef USB_PIO_ONLY
#define BUS_MODE PIO_MODE				
#else
#define BUS_MODE QMU_MODE		
#endif
#define EP0_BUS_MODE DMA_MODE


#ifndef CONFIG_USB_MU3D_DRV
#define POWER_SAVING_MODE
#endif

#define DIS_ZLP_CHECK_CRC32 //disable check crc32 in zlp
#ifndef CONFIG_USB_MU3D_DRV
//This should be enabled if VBUS_rise/fall are detected by external PMU instead of USB3 IP
#define EXT_VBUS_DET
#endif

//#define CFG_RX_COZ_EN  //makr this need to handle ZLP err
//#define CFG_RX_ZLP_EN	
#define CS_12B 1
#define CS_16B 2
#define CHECKSUM_TYPE CS_16B
#define U3D_COMMAND_TIMER 10

#if (CHECKSUM_TYPE==CS_16B)
#define CHECKSUM_LENGTH 16
#else
#define CHECKSUM_LENGTH 12
#endif

#define NO_ZLP 0
#define HW_MODE 1
#define GPD_MODE 2

#ifdef _USB_NORMAL_
#define TXZLP NO_ZLP
#else
#define TXZLP GPD_MODE
#endif

#define ISO_UPDATE_TEST 0
#define ISO_UPDATE_MODE 1

#define LPM_STRESS 0


#ifdef EXT_VBUS_DET
#define FPGA_REG 0xf0008098
#define VBUS_RISE_BIT (1<<11) //W1C
#define VBUS_FALL_BIT (1<<12) //W1C
#define VBUS_MSK (VBUS_RISE_BIT | VBUS_FALL_BIT)
#define VBUS_RISE_IRQ 13
#define VBUS_FALL_IRQ 14
#endif


#define USB_FIFO(ep_num) 				(U3D_FIFO0+ep_num*0x10)

#define USB_FIFOSZ_SIZE_8				(0x03)
#define USB_FIFOSZ_SIZE_16				(0x04)
#define USB_FIFOSZ_SIZE_32				(0x05)
#define USB_FIFOSZ_SIZE_64				(0x06)
#define USB_FIFOSZ_SIZE_128			    (0x07)
#define USB_FIFOSZ_SIZE_256			    (0x08)
#define USB_FIFOSZ_SIZE_512			    (0x09)
#define USB_FIFOSZ_SIZE_1024			(0x0A)
#define USB_FIFOSZ_SIZE_2048			(0x0B)
#define USB_FIFOSZ_SIZE_4096			(0x0C)
#define USB_FIFOSZ_SIZE_8192			(0x0D)
#define USB_FIFOSZ_SIZE_16384			(0x0E)
#define USB_FIFOSZ_SIZE_32768			(0x0F)


//U3D_EP0CSR
#define EP0_W1C_BITS 					(~(EP0_RXPKTRDY | EP0_SETUPPKTRDY | EP0_SENTSTALL))
//U3D_TX1CSR0
#define TX_W1C_BITS 				(~(TX_SENTSTALL))
/* USB_RXCSR */
#define RX_W1C_BITS 				(~(RX_SENTSTALL|RX_RXPKTRDY))


#define BIT0 (1<<0)
#define BIT16 (1<<16)

#define TYPE_BULK						(0x00)
#define TYPE_INT						(0x10)
#define TYPE_ISO						(0x20)
#define TYPE_MASK						(0x30)


/* QMU macros */
#define USB_QMU_RQCSR(n) 				(U3D_RXQCSR1+0x0010*((n)-1))
#define USB_QMU_RQSAR(n)    			(U3D_RXQSAR1+0x0010*((n)-1))
#define USB_QMU_RQCPR(n) 				(U3D_RXQCPR1+0x0010*((n)-1))
#define USB_QMU_RQLDPR(n) 				(U3D_RXQLDPR1+0x0010*((n)-1))
#define USB_QMU_TQCSR(n)  				(U3D_TXQCSR1+0x0010*((n)-1))
#define USB_QMU_TQSAR(n) 				(U3D_TXQSAR1+0x0010*((n)-1))
#define USB_QMU_TQCPR(n) 				(U3D_TXQCPR1+0x0010*((n)-1))

#define QMU_Q_START					(0x00000001)
#define QMU_Q_RESUME					(0x00000002)
#define QMU_Q_STOP					(0x00000004)
#define QMU_Q_ACTIVE					(0x00008000)

#define QMU_TX_EN(n) 					(BIT0<<(n))
#define QMU_RX_EN(n) 					(BIT16<<(n))
#define QMU_TX_CS_EN(n)				(BIT0<<(n))
#define QMU_RX_CS_EN(n)				(BIT16<<(n))
#define QMU_TX_ZLP(n)					(BIT0<<(n))
#define QMU_RX_MULTIPLE(n)				(BIT16<<((n)-1))
#define QMU_RX_ZLP(n)					(BIT0<<(n))
#define QMU_RX_COZ(n)					(BIT16<<(n))

#define QMU_RX_EMPTY(n) 				(BIT16<<(n))
#define QMU_TX_EMPTY(n) 				(BIT0<<(n))
#define QMU_RX_DONE(n)				(BIT16<<(n))
#define QMU_TX_DONE(n)				(BIT0<<(n))

#define QMU_RX_ZLP_ERR(n)			(BIT16<<(n))
#define QMU_RX_EP_ERR(n)				(BIT0<<(n)) 
#define QMU_RX_LEN_ERR(n)			(BIT16<<(n))
#define QMU_RX_CS_ERR(n)				(BIT0<<(n))

#define QMU_TX_LEN_ERR(n)			(BIT16<<(n))
#define QMU_TX_CS_ERR(n)				(BIT0<<(n))




#define USB_END_OFFSET(_bEnd, _bOffset)	((0x10*(_bEnd-1)) + _bOffset)
#define USB_ReadCsr32(_bOffset, _bEnd) \
   DRV_Reg32(USB_END_OFFSET(_bEnd, _bOffset))
#define USB_WriteCsr32(  _bOffset, _bEnd, _bData) \
   DRV_WriteReg32( USB_END_OFFSET(_bEnd, _bOffset), _bData)

#define	SSUSB_EP_TXCR0_OFFSET(_epnum, _offset)	\
   (U3D_TX1CSR0 + ((_epnum - 1)*0x10) + (_offset))

#define	SSUSB_EP_TXCR1_OFFSET(_epnum, _offset)	\
   (U3D_TX1CSR1 + ((_epnum - 1)*0x10) + (_offset))

#define	SSUSB_EP_TXCR2_OFFSET(_epnum, _offset)	\
   (U3D_TX1CSR2 + ((_epnum - 1)*0x10) + (_offset))

#define	SSUSB_EP_TXMAXP_OFFSET(_epnum, _offset)	\
   (U3D_TX1CSR0 + ((_epnum - 1)*0x10) + (_offset))

#define	SSUSB_EP_RXCR0_OFFSET(_epnum, _offset)	\
   (U3D_RX1CSR0 + ((_epnum - 1)*0x10) + (_offset))

#define	SSUSB_EP_RXCR1_OFFSET(_epnum, _offset)	\
   (U3D_RX1CSR1 + ((_epnum - 1)*0x10) + (_offset))

#define	SSUSB_EP_RXCR2_OFFSET(_epnum, _offset)	\
   (U3D_RX1CSR2 + ((_epnum - 1)*0x10) + (_offset))

#define	SSUSB_EP_RXCR3_OFFSET(_epnum, _offset)	\
   (U3D_RX1CSR3 + ((_epnum - 1)*0x10) + (_offset))


#define USB_WriteMsk(addr, data, msk) \
{ DRV_WriteReg32(addr, ((DRV_Reg32(addr) & ~(msk)) | ((data) & (msk)))); \
}

#define USB_SetMsk(addr, msk) \
{ DRV_WriteReg32(addr, DRV_Reg32(addr) | msk); \
}
#define USB_ClrMsk(addr, msk) \
{ DRV_WriteReg32(addr, DRV_Reg32(addr) &~ msk); \
}

#ifdef USB_DBG_ON
#define USB_DBG(fmt, args...)	do{LOGD(fmt, ##args);}while(0)
#else
#define USB_DBG(fmt, args...)	do{}while(0)
#endif

#endif  /* USB_HW_H */               

