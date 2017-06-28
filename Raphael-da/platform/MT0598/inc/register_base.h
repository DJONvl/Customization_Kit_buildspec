#ifndef __REGISTER_BASE_
#define __REGISTER_BASE_

// Module: UART
// APB Module uart
#define AP_UART0_BASE (0x11020000)  //(0x11002000)

// APB Module uart
#define AP_UART1_BASE (0x11030000)  //(0x11003000)

// APB Module uart
#define AP_UART2_BASE (0x11040000)

// Module: USB
// APB Module usb2
#define USB_BASE (0x11200000)

// APB Module usb_sif
#define USBSIF_BASE (0x11210000)

// APB Module ssusb_top
#define USB3_BASE (0x11200000)

// APB Module ssusb_top_sif
#define USB3_SIF_BASE (0x112A0000)

// APB Module ssusb_top_sif2
#define USB3_SIF2_BASE (0x11C30000)

//#define ICUSB_BASE (0x11270000)

// APB Module apmixed	
// USB PLL
#define APMIXED_BASE (0x10212000)

// Module: MSDC
// APB Module msdc
#define APB_MSDC0_BASE (0x11240000)

// APB Module msdc
#define APB_MSDC1_BASE (0x11250000)

// APB Module msdc
#define APB_MSDC2_BASE (0x11260000)

// APB Module msdc
#define APB_MSDC3_BASE (0x11270000)
#define IOCFG_0_BASE (0x10002000)
#define IOCFG_1_BASE (0x10002200)
#define IOCFG_2_BASE (0x10002400)
#define IOCFG_3_BASE (0x10002600)
#define IOCFG_4_BASE (0x10002800)
#define IOCFG_5_BASE (0x10002A00)

// Module: emi
// APB Module 
#define DRAMC0_BASE (0x10332000)
#define DRAMC1_BASE (0x1033A000)

#define DDRPHY_BASE (0x10330000)    //(0x1000F000)
#define DRAMC_NAO_BASE (0x10334000)
#define EMI_BASE (0x10230000)   //(0x10203000)

// Module: gpio	
// efuse / msdc / pmic / uart
#define GPIO_BASE                   (0x10370000)
#define IO_RB_BASE                  (0x11C10000)
#define IO_BR_BASE                  (0x11D30000)
#define IO_LB_BASE                  (0x11E00000)
#define IO_LT_BASE                  (0x11E10000)
#define IO_TL_BASE                  (0x11F00000)
#define IO_TR_BASE                  (0x11F20000)

// Module: WDT 
// APB Module toprgu
#define TOPRGU_BASE (0x10211000) 

// Module: gpt_timer
// APB Module apxgpt	
#define APXGPT_BASE (0x10040000) 

// Module: mmu
// APB Module ca7mcucfg	
#define CA7MCUCFG_BASE (0x10200000)

// Module: efusec get random id
// APB Module 
#define EFUSEC_BASE (0x11D10000)    //(0x10206000)

/* hardware version register */
#define VER_BASE 0x08000000
#define APHW_CODE           (VER_BASE)
#define APHW_SUBCODE        (VER_BASE + 0x04)
#define APHW_VER            (VER_BASE + 0x08)
#define APSW_VER            (VER_BASE + 0x0C)


// rtc
#define RTC_BASE (0x4000)

//#define SYSRAM_BASE (0x19000000)
#define GIC_BASE 0x10230000		//irq
#define GIC_DIST_BASE (GIC_BASE + 0x1000)
#define GIC_CPU_BASE  (GIC_BASE + 0x2000)


//Module: security
// APB  sej	
#define SEJ_BASE (0x1000A000)


// Module: PLL PMIC PMIC Low power
// APB Module cksys 
#define CKSYS_BASE (0x10210000)

// DRAM PMIC MSDC low power
#define TOPCKGEN_BASE (0x10210000)  

// APB Module infracfg_ao	
//PLL sec low power pmic
#define INFRACFG_AO_BASE (0x10001000)

// APB Module pericfg	
// GPT_timer	UFS
#define PERICFG_BASE (0x10020000)


// APB Module sleep	
// pmic_pwarp / spm
#define SLEEP_BASE (0x108C0000)    
#define SPM_BASE            SLEEP_BASE


// APB Module 
// pmic_wrap	pmic
#define PWRAP_BASE (0x10010000)    

// APB Module mcucfg	
// low_power
#define MCUCFG_BASE (0x10200000)


// APB Module 
// gcpu
#define GCPU_BASE (0x10224000)  //(0x10210000)

// APB Module ap_dma	
// iic
#define AP_DMA_BASE (0x11000000)

// APB Module i2c
#define I2C0_BASE (0x11080000)  //(0x11007000)

// APB Module i2c
#define I2C1_BASE (0x11090000)  //(0x11008000)

// APB Module i2c
#define I2C2_BASE (0x110a0000)  //(0x11009000)

// APB Module audio	pll
#define AUDIO_BASE (0x11220000)

// APB Module mfg_top	pll
#define MFGCFG_BASE (0x13000000)

// APB Module mmsys_config	pll
#define MMSYS_CONFIG_BASE (0x14000000)

// APB Module imgsys	pll
#define IMGSYS_BASE       (0x15000000)

// APB Module vdecsys_config pll
#define VDEC_GCON_BASE    (0x16000000)

// APB Module venc_config	pll
#define VENC_GCON_BASE    (0x17000000)


// APB Module DDRPHY1_BASE
//#define DDRPHY1_BASE (0x10012000)
// APB Module md32
//#define MD32_BASE (0x10058000)

// APB Module dbgapb
//#define DBGAPB_BASE (0x10100000)

// APB Module ap_cirq_eint
//#define APIRQ_BASE (0x1000B000)

// APB Module fhctl
//#define FHCTL_BASE (0x1000CF00)

// APB Module infracfg
//#define INFRACFG_BASE (0x10340000)  //(0x10201000)

// APB Module sramrom
//#define SRAMROM_BASE (0x10220000)   //(0x10202000)

// APB Module device_apc_ao
//#define DEVAPC_AO_BASE (0x10070000) //(0x1000E000)

// APB Module kp
//#define KP_BASE (0x10010000)

// APB Module sys_cirq
//#define SYS_CIRQ_BASE (0x102C0000)  //(0x10204000)

// APB Module mm_iommu
//#define M4U_BASE (0x10205000)

// APB Module spi
//#define SPI1_BASE (0x110F0000)  //(0x1100A000)

// APB Module therm_ctrl
//#define THERM_CTRL_BASE (0x110B0000)    //(0x1100B000)

// APB Module btif
//#define BTIF_BASE (0x11AC0000)  //(0x1100C000)

// APB Module auxadc
//#define AUXADC_BASE (0x11001000)

// APB Module pwm
//#define PWM_BASE (0x11070000)   //(0x11006000)

// APB Module device_apc
//#define DEVAPC_BASE (0x10222000)    //(0x10207000)

// APB Module bus_dbg_tracker_cfg
//#define BUS_DBG_BASE (0x10341000)   //(0x10208000)

// APB Module infra_mbist
//#define INFRA_MBIST_BASE (0x10342000)   //(0x1020D000)


// APB Module trng
//#define TRNG_BASE (0x10223000)  //(0x1020F000)

// APB Module gcpu_ns
//#define GCE_BASE (0x102D0000)   //(0x10212000)

// APB Module cq_dma
//#define CQ_DMA_BASE (0x10212c00)


#include "register_act_util.h"
#endif
