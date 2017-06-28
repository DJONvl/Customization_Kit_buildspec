#ifndef __REGISTER_BASE_
#define __REGISTER_BASE_

#define IO_PHYS (0x10000000)  //lxo-tobedone

// APB Module cksys
#define CKSYS_BASE (0x10000000)
#define TOPCKGEN_BASE (0x10000000)
// APB Module infracfg_ao
#define INFRACFG_AO_BASE (0x10001000)//lxo

// APB Module pericfg
#define PERICFG_BASE (0x10003000)//lxo

// APB Module dramc
//#define DRAMC0_BASE (0x10207000)//lxo

// APB Module gpio
#define GPIO_BASE (0x10005000)//lxo

// APB Module sleep
#define SLEEP_BASE (0x10006000)//lxo

// APB Module toprgu
#define TOPRGU_BASE (0x10007000)//lxo

// APB Module apxgpt
#define APXGPT_BASE (0x10008000)//lxo

// APB Module sej
#define SEJ_BASE (0x1000A000)//lxo

// APB Module ap_cirq_eint
#define APIRQ_BASE (0x1000B000)//lxo

// APB Module apmixed
#define APMIXED_BASE (0x10018000)//lxo

// APB Module fhctl
#define FHCTL_BASE (0x10018F00)//lxo

// APB Module pmic_wrap
#define PWRAP_BASE (0x1000F000)//lxo

// APB Module device_apc_ao
#define DEVAPC_AO_BASE (0x10010000)//lxo

// APB Module ddrphy
//#define DDRPHY_BASE (0x10211000)//lxo

// APB Module kp
//#define KP_BASE (0x10010000)//lxo

// APB Module md32
//#define MD32_BASE (0x10058000)//lxo

// APB Module dbgapb
//#define DBGAPB_BASE (0x10100000)//lxo

// APB Module mcucfg
#define MCUCFG_BASE (0x10200000)//lxo

// APB Module ca7mcucfg
#define CA7MCUCFG_BASE (0x10200000)//lxo

// APB Module infracfg
#define INFRACFG_BASE (0x1020F000)//lxo

// APB Module sramrom
#define SRAMROM_BASE (0x10209000)//lxo

// APB Module emi
#define EMI_BASE (0x10205000)//LXO

// APB Module sys_cirq
#define SYS_CIRQ_BASE (0x10202000)//lxo

// APB Module mm_iommu
//#define M4U_BASE (0x10205000)//lxo

// APB Module efusec
#define EFUSEC_BASE (0x10009000)//lxo

// APB Module device_apc
#define DEVAPC_BASE (0x10204000)//lxo

// APB Module bus_dbg_tracker_cfg
//#define BUS_DBG_BASE (0x10208000)//lxo

// APB Module infra_mbist
//#define INFRA_MBIST_BASE (0x1020D000)//lxo

// APB Module dramc_conf_nao
//#define DRAMC_NAO_BASE (0x10206000)//lxo

// APB Module trng
//#define TRNG_BASE (0x1020F000)

// APB Module gcpu
#define GCPU_BASE (0x1020D000)//lxo

// APB Module gcpu_ns
//#define GCE_BASE (0x1020A000)//lxo

// APB Module cq_dma
#define CQ_DMA_BASE (0x10212c00)//lxo-tobedone

// APB Module ap_dma
#define AP_DMA_BASE (0x11000000)//lxo

// APB Module auxadc
//#define AUXADC_BASE (0x11001000)//lxo

// APB Module uart
#define AP_UART0_BASE (0x11005000)// LXO

// APB Module uart
#define AP_UART1_BASE (0x11006000)//LXO

// APB Module uart
#define AP_UART2_BASE (0x11007000)//LXO

// APB Module pwm
#define PWM_BASE (0x11008000)//lxo

// APB Module i2c
#define I2C0_BASE (0x11009000)//lxo

// APB Module i2c
#define I2C1_BASE (0x1100A000)//lxo

// APB Module i2c
#define I2C2_BASE (0x1100B000)//lxo

// APB Module spi
#define SPI1_BASE (0x1100C000)//lxo

// APB Module therm_ctrl
#define THERM_CTRL_BASE (0x1100D000)//lxo

// APB Module irda
#define BTIF_BASE (0x1100E000)//lxo

// APB Module usb2
#define USB_BASE (0x11100000)//lxo

// APB Module usb_sif
#define USBSIF_BASE (0x11110000)//lxo

// APB Module audio
#define AUDIO_BASE (0x11140000)//lxo

// APB Module msdc
#define APB_MSDC0_BASE (0x11120000)//lxo

// APB Module msdc
#define APB_MSDC1_BASE (0x11130000)//lxo

// APB Module msdc
#define APB_MSDC2_BASE (0x11170000)//lxo

// APB Module mfg_top
#define MFGCFG_BASE (0x13FFE000)//lxo, MFG, G3D

// APB Module mmsys_config
#define MMSYS_CONFIG_BASE (0x14000000)//lxo

// APB Module imgsys
#define IMGSYS_BASE       (0x15000000)//lxo

// APB Module vdecsys_config
#define VDEC_GCON_BASE    (0x16000000)//lxo

// APB Module venc_config
#define VENC_GCON_BASE    (0x15009000)//lxo

// rtc
#define RTC_BASE (0x8000)

/*WDT*/
#define WDT_BASE				(0x10007000)//Sheet InfrasysAPB_AO, line 10, RGU includes WDT

#define GIC_DIST_BASE       (0x10310000)//A
#define GIC_CPU_BASE        (0x10320000)

//#define SYSRAM_BASE (0x19000000)
//#define GIC_DIST_BASE (0x19000000)//lxo
//#define GIC_CPU_BASE  (CA9_BASE + 0x2000)


/* hardware version register */
#define VER_BASE 0x08000000  //lxo
#define APHW_CODE           (VER_BASE)     //lxo
#define APHW_SUBCODE        (VER_BASE + 0x04)//lxo
#define APHW_VER            (VER_BASE + 0x08)//lxo
#define APSW_VER            (VER_BASE + 0x0C)//lxo

#define SPM_BASE            SLEEP_BASE

//from rainer reg_base.h  for spm...
#define CONFIG_BASE         (IO_PHYS + 0x10000) /*Infrasys Configration Register*/
#define BOOTROM_SEC_CTRL        (CONFIG_BASE + 0x804)
#define SW_ROM_PD          (1U << 31)

#include "register_act_util.h"
#endif
