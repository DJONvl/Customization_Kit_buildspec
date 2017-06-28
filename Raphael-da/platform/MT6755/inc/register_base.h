#ifndef __REGISTER_BASE_
#define __REGISTER_BASE_

#define BOOTROM_BASE (0x00000000)
#define BOOTSRAM_BASE (0x00100000)
#define IO_PHYS (0x10000000)

// APB Module cksys
#define CKSYS_BASE (0x10000000)
#define TOPCKGEN_BASE (0x10000000)

// APB Module infracfg_ao
#define INFRACFG_AO_BASE (0x10001000)
#define IOCFG_0_BASE (0x10002000)
#define IOCFG_1_BASE (0x10002200)
#define IOCFG_2_BASE (0x10002400)
#define IOCFG_3_BASE (0x10002600)
#define IOCFG_4_BASE (0x10002800)
#define IOCFG_5_BASE (0x10002A00)

// APB Module pericfg
#define PERICFG_BASE (0x10003000)

// APB Module dramc
#define DRAMC0_BASE (0x10004000)

// APB Module gpio
#define GPIO_BASE (0x10005000)

// APB Module sleep
#define SLEEP_BASE (0x10006000)

// APB Module toprgu
#define TOPRGU_BASE (0x10007000)

// APB Module apxgpt
#define APXGPT_BASE (0x10008000)

// APB Module sej
#define SEJ_BASE (0x1000A000)

// APB Module ap_cirq_eint
#define APIRQ_BASE (0x1000B000)

// APB Module apmixed
#define APMIXED_BASE (0x1000C000)

// APB Module fhctl
#define FHCTL_BASE (0x1000CF00)

// APB Module pmic_wrap
#define PWRAP_BASE (0x1000D000)

// APB Module device_apc_ao
#define DEVAPC_AO_BASE (0x1000E000)

// APB Module ddrphy
#define DDRPHY_BASE (0x1000F000)

// APB Module kp
#define KP_BASE (0x10010000)

// APB Module md32
#define MD32_BASE (0x10058000)

// APB Module dbgapb
#define DBGAPB_BASE (0x10100000)

// APB Module mcucfg
#define MCUCFG_BASE (0x10200000)

// APB Module ca7mcucfg
#define CA7MCUCFG_BASE (0x10200000)

// APB Module infracfg
#define INFRACFG_BASE (0x10201000)

// APB Module sramrom
#define SRAMROM_BASE (0x10202000)

// APB Module emi
#define EMI_BASE (0x10203000)

// APB Module sys_cirq
#define SYS_CIRQ_BASE (0x10204000)

// APB Module mm_iommu
#define M4U_BASE (0x10205000)

// APB Module efusec
#define EFUSEC_BASE (0x10206000)

// APB Module device_apc
#define DEVAPC_BASE (0x10207000)

// APB Module bus_dbg_tracker_cfg
#define BUS_DBG_BASE (0x10208000)

// APB Module infra_mbist
#define INFRA_MBIST_BASE (0x1020D000)

// APB Module dramc_conf_nao
#define DRAMC_NAO_BASE (0x1020E000)

// APB Module trng
#define TRNG_BASE (0x1020F000)

// APB Module gcpu
#define GCPU_BASE (0x10210000)

// APB Module gcpu_ns
#define GCE_BASE (0x10212000)

// APB Module cq_dma
#define CQ_DMA_BASE (0x10212c00)

// APB Module ap_dma
#define AP_DMA_BASE (0x11000000)

// APB Module auxadc
#define AUXADC_BASE (0x11001000)

// APB Module uart
#define AP_UART0_BASE (0x11002000)

// APB Module uart
#define AP_UART1_BASE (0x11003000)

// APB Module uart
#define AP_UART2_BASE (0x11004000)

// APB Module pwm
#define PWM_BASE (0x11006000)

// APB Module i2c
#define I2C0_BASE (0x11007000)

// APB Module i2c
#define I2C1_BASE (0x11008000)

// APB Module i2c
#define I2C2_BASE (0x11009000)

// APB Module spi
#define SPI1_BASE (0x1100A000)

// APB Module therm_ctrl
#define THERM_CTRL_BASE (0x1100B000)

// APB Module irda
#define BTIF_BASE (0x1100C000)

// APB Module usb2
#define USB_BASE (0x11200000)

// APB Module usb_sif
#define USBSIF_BASE (0x11210000)

// APB Module audio
#define AUDIO_BASE (0x11220000)

// APB Module msdc
#define APB_MSDC0_BASE (0x11230000)

// APB Module msdc
#define APB_MSDC1_BASE (0x11240000)

// APB Module msdc
#define APB_MSDC2_BASE (0x11250000)

// APB Module msdc
#define APB_MSDC3_BASE (0x11260000)

//#define ICUSB_BASE (0x11270000)
// APB Module ssusb_top
#define USB3_BASE (0x11270000)

// APB Module ssusb_top_sif
#define USB3_SIF_BASE (0x11280000)

// APB Module ssusb_top_sif2
#define USB3_SIF2_BASE (0x11290000)

// APB Module mfg_top
#define MFGCFG_BASE (0x13000000)

// APB Module mmsys_config
#define MMSYS_CONFIG_BASE (0x14000000)

// APB Module imgsys
#define IMGSYS_BASE       (0x15000000)

// APB Module vdecsys_config
#define VDEC_GCON_BASE    (0x16000000)

// APB Module venc_config
#define VENC_GCON_BASE    (0x17000000)

// rtc
#define RTC_BASE (0x4000)

//#define SYSRAM_BASE (0x19000000)
#define GIC_BASE 0x10230000
#define GIC_DIST_BASE (GIC_BASE + 0x1000)
#define GIC_CPU_BASE  (GIC_BASE + 0x2000)

/* hardware version register */
#define VER_BASE 0x08000000
#define APHW_CODE           (VER_BASE)
#define APHW_SUBCODE        (VER_BASE + 0x04)
#define APHW_VER            (VER_BASE + 0x08)
#define APSW_VER            (VER_BASE + 0x0C)

#define SPM_BASE            SLEEP_BASE


#include "register_act_util.h"
#endif
