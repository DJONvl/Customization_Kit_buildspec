#ifndef __REGISTER_BASE_
#define __REGISTER_BASE_

#define BOOTROM_BASE (0x00000000)
#define BOOTSRAM_BASE (0x00100000)
#define IO_PHYS (0x10000000)

// APB Module cksys
#define CKSYS_BASE (0x10210000)     //LXO
#define TOPCKGEN_BASE (0x10210000)	//LXO

// APB Module infracfg_ao
#define INFRACFG_AO_BASE (0x10000000)//LXO

//#define IOCFG_RB_BASE (0x11C10000)
//#define IOCFG_BR_BASE (0x11D30000)
#define IOCFG_BL_BASE (0x11D40000) //??
#define IOCFG_BM_BASE (0x11D50000)
#define IOCFG_LB_BASE (0x11E60000)
#define IOCFG_LT_BASE (0x11E10000)
#define IOCFG_TR_BASE (0x11F00000)

#define MSDC_TOP_BASE (0x104D0000)//??
#define IO_CFG_2_BASE (0x104E0000)

// APB Module pericfg
#define PERICFG_BASE (IO_PHYS + 0x01010000) //lxo

// APB Module dramc
#define DRAMC0_BASE         (IO_PHYS + 0x00332000)//lxo

// APB Module gpio
#define GPIO_BASE           (IO_PHYS + 0x002D0000)//lxo

// APB Module sleep
#define SPM_BASE            (IO_PHYS + 0x00A00000) //lxo
#define SLEEP_BASE           SPM_BASE

// APB Module toprgu
#define TOPRGU_BASE        (IO_PHYS + 0x00211000)//lxo

// APB Module apxgpt
#define APXGPT_BASE (0x10040000) //lxo, mem

// APB Module sej
#define SEJ_BASE (0x10080000) //lxo, mem

// APB Module apmixed
#define APMIXED_BASE (0x10212000)//pll£¬ lxo, mem

// APB Module pmic_wrap
#define PWRAP_BASE (0x10010000)//lxo, mem

// APB Module ddrphy
#define DDRPHY_BASE (0x10330000)//lxo, mem

// APB Module mcucfg
#define MCUCFG_BASE (0x0C530000) //irq &  low power, lxo, mem

// APB Module ca7mcucfg
#define CA7MCUCFG_BASE (0x10200000) //To be modified.

// APB Module infracfg
//#define INFRACFG_BASE (0x10201000) //pll

// APB Module sramrom
//#define SRAMROM_BASE (0x10202000) //spm

// APB Module emi
#define EMI_BASE (0x10230000)//lxo, mem

// APB Module efusec
#define EFUSEC_BASE (0x10450000) //ramdom id, lxo, mem

// APB Module dramc_conf_nao
#define DRAMC_NAO_BASE (0x10334000)//lxo, mem

// APB Module gcpu
//#define GCPU_BASE (0x10210000)//???

// APB Module dramc_conf_nao
#define DRAMC1_NAO_BASE (0x1033C000)//lxo, mem

// APB Module cq_dma
#define CQ_DMA_BASE (0x10200000) //DRAM, lxo, mem

// APB Module ap_dma
#define AP_DMA_BASE (0x11000000)//i2c, lxo, mem

// APB Module uart
#define AP_UART0_BASE (0x11020000)	// lxo, mem

// APB Module uart
#define AP_UART1_BASE (0x11030000)	// lxo, mem

// APB Module uart
#define AP_UART2_BASE (0x11040000)	// lxo, mem

// APB Module uart
#define AP_UART3_BASE (AP_UART2_BASE)	// lxo, mem

// APB Module i2c
#define I2C0_BASE (0x11080000)// lxo, mem

// APB Module i2c
#define I2C1_BASE (0x11090000)// lxo, mem

// APB Module i2c
#define I2C2_BASE (0x110A0000)// lxo, mem

// APB Module i2c
#define I2C3_BASE (0x11100000)// lxo, mem

// APB Module i2c
#define I2C4_BASE (0x110C0000)// lxo, mem

// APB Module i2c
#define I2C5_BASE (0x110D0000)// lxo, mem

// APB Module i2c
#define I2C6_BASE (0x110E0000)// lxo, mem

// APB Module i2c
#define I2C7_BASE (0x11190000)// lxo, mem

// APB Module usb2
#define USB_BASE (0x11210000)//(0x11800000)// lxo, mem

// APB Module usb_sif
//#define USBSIF_BASE (0x11210000)

// APB Module audio
#define AUDIO_BASE (0x11820000) //pll,  lxo, mem

// APB Module msdc
#define APB_MSDC0_BASE (0x11840000)// lxo, mem

// APB Module msdc
#define APB_MSDC1_BASE (0x11850000)// lxo, mem

// APB Module msdc
#define APB_MSDC3_BASE (0x11870000)// lxo, mem

// APB Module ssusb_top
//#define USB3_BASE (0x11C30000)

// APB Module ssusb_top_sif
//#define USB3_SIF_BASE (0x11280000)

// rtc
#define RTC_BASE (0x4000)//To be modified.

//#define SYSRAM_BASE (0x19000000)
#define GIC_BASE 0x0C000000 //irq // lxo, mem
#define GIC_DIST_BASE (GIC_BASE)
#define GIC_REDIS_BASE (0x0C100000)
//#define GIC_DIST_BASE (GIC_BASE + 0x1000)
#define GIC_CPU_BASE  (GIC_BASE + 0x2000)

/* hardware version register */
#define VER_BASE 0x08000000      //Chip ID/HW, SW Version, lxo, mem
#define APHW_CODE           (VER_BASE)
#define APHW_SUBCODE        (VER_BASE + 0x04)
#define APHW_VER            (VER_BASE + 0x08)
#define APSW_VER            (VER_BASE + 0x0C)

//#define SPM_BASE            SLEEP_BASE

/*dual stoarge*/
#define GPIO_BANK     (GPIO_BASE+0x6E0)  //To be modified.//GPIO_BASE lxo, mem
/*bit 13 1:UFS   bit 13 0: eMMC*/
#define TRAP_UFS_FIRST (1U<<13) //To be modified.

#include "register_act_util.h"
#endif
