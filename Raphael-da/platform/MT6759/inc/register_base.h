#ifndef __REGISTER_BASE_
#define __REGISTER_BASE_

#define UINT32P unsigned int *

#define BOOTROM_BASE (0x00000000)
#define BOOTSRAM_BASE (0x00100000)
#define IO_PHYS             (0x10000000)

#define VER_BASE            (0x08000000)

// APB Module cksys
#define CKSYS_BASE		(0x10210000)

/*=======================================================================*/
/* Register Bases                                                        */
/*=======================================================================*/
#define TOPCKGEN_BASE       (IO_PHYS + 0x00210000)
#define INFRACFG_AO_BASE    (IO_PHYS)
#define IO_CFG_B_BASE       (IO_PHYS + 0x00460000)
#define IO_CFG_BR_BASE      (IO_PHYS + 0x00470000)
#define IO_CFG_L_BASE       (IO_PHYS + 0x004A0000)
#define IO_CFG_R_BASE       (IO_PHYS + 0x00410000)
#define IO_CFG_RT_BASE      (IO_PHYS + 0x00420000)
#define IO_CFG_T_BASE       (IO_PHYS + 0x004E0000)
#define PERICFG_BASE        (IO_PHYS + 0x01010000)

//PLL
#define MCUSYS_CFGREG_BASE  (0x0C530000)
#define MCUCFG_BASE		(MCUSYS_CFGREG_BASE)
#define CA57ACFG_BASE           (0x0C532000)
#define MMSYS_CONFIG_BASE	(0x14000000)
#define SPM_BASE            (IO_PHYS + 0x00A00000)
#define SLEEP_BASE   SPM_BASE
//////////////////////////////

// APB Module dramc
#define DRAMC0_BASE (IO_PHYS + 0x00332000) 

// APB Module gpio
#define GPIO_BASE (IO_PHYS + 0x002D0000)

// APB Module toprgu
#define TOPRGU_BASE (0x10211000)

// APB Module apxgpt
#define APXGPT_BASE (IO_PHYS + 0x00040000)

// APB Module apmixed
#define APMIXED_BASE		(0x10212000)

// APB Module pmic_wrap
#define PWRAP_BASE (IO_PHYS + 0x00010000)

// APB Module ddrphy
#define DDRPHY_BASE (IO_PHYS + 0x00330000)

// APB Module ca7mcucfg
#define CA7MCUCFG_BASE (0x10200000)

// APB Module emi
#define EMI_BASE (IO_PHYS + 0x00230000)

// APB Module efusec
#define EFUSEC_BASE (0x10206000)

// APB Module gcpu
#define GCPU_BASE (0x10210000)//lk delete on memory map

// APB Module ap_dma
#define AP_DMA_BASE (0x1030000)//not found

// APB Module uart
#define AP_UART0_BASE (0x11020000)

// APB Module uart
#define AP_UART1_BASE (0x11030000)

// APB Module uart
#define AP_UART2_BASE (0x11040000)

// APB Module i2c
#define I2C0_BASE (0x11080000)

// APB Module i2c
#define I2C1_BASE (0x11090000)

// APB Module i2c
#define I2C2_BASE (0x110A0000)

// APB Module msdc
#define MSDC_TOP_BASE       (IO_PHYS + 0x004D0000)

// APB Module msdc
#define APB_MSDC0_BASE (0x11840000)

// APB Module msdc
#define APB_MSDC1_BASE (0x11850000)

// APB Module msdc
#define APB_MSDC2_BASE (0x11260000)

// APB Module msdc
#define APB_MSDC3_BASE (0x11270000)

#define U3D_BASE		     (IO_PHYS + 0x01800000) /* MAC: 0x1180_0000 */
#define USB_SIF_BASE	     (IO_PHYS + 0x018A0000) /* IPPC: 0x118A_0000 */
#define USB_SIF2_BASE	     (IO_PHYS + 0x00440000) /* PHY: 0x1044_0000 */
#define USB1P_SIF_BASE      (IO_PHYS + 0x01C40000)

// rtc
#define RTC_BASE (0x4000)

//#define SYSRAM_BASE (0x19000000)
#define GIC_BASE 0x0C000000 //irq
#define GIC_DIST_BASE (GIC_BASE)

/* hardware version register */
#define APHW_CODE           (VER_BASE)
#define APHW_SUBCODE        (VER_BASE + 0x04)
#define APHW_VER            (VER_BASE + 0x08)
#define APSW_VER            (VER_BASE + 0x0C)

#include "register_act_util.h"
#endif
