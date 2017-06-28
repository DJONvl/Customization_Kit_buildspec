#ifndef _REG_BASE_H_
#define _REG_BASE_H_

#define VER_BASE            (0x08000000) /*Chip ID, HW, SW Version*/
#define IO_PHYS              (0x10000000)

#define CKSYS_BASE        (IO_PHYS + 0x0000) /*op-level Clock Generator*/
#define INFRACFG_AO_BASE    (IO_PHYS + 0x1000)

#define INFRA_GLOBALCON_RST0   (IO_PHYS+0x1030)

#define PERI_CON_BASE  (IO_PHYS + 0x3000) /*Perisys Configration Register*/

#define GPIO_BASE          (IO_PHYS +0x5000) /*GPIO Controller*/
#define SPM_BASE           (IO_PHYS + 0x6000) /*Top-level Sleep Manager*/
#define WDT_BASE           (IO_PHYS + 0x7000) /*Top-level Reset Generator*/
#define GPT_BASE            (IO_PHYS + 0x8000) /*GPT*/
#define APMIXED_BASE      (IO_PHYS + 0xC000)
#define PMIC_WRAP_BASE (IO_PHYS + 0xF000)  //0x1000F000

#define CONFIG_BASE         (IO_PHYS + 0x10000) /*Infrasys Configration Register*/
//#define APMIXED_BASE      (IO_PHYS + 0x18000) /*Perisys Configration Register*/

/* MCUSS Register */
#define ACLKEN_DIV                  (0x10200640)
#define EMI_BASE                      (0x10205000) /*EMI AO/NON-AO Register*/
#define DRAMC_NAO_BASE      (0x10206000) /*DRAM Control Non-AO Register*/

#define DRAMC0_BASE              (0x10207000) /*DRAM Control Register*/
#define DDRPHY_BASE              (0x10208000) /*DDRPHY Control Register*/
#define SRAMROM_BASE          (0x10209000) /*BootROM, SRAM*/

#define DMA_BASE            (0x11000000) /*DMA*/

#define UART1_BASE          (0x11005000) /*UART1*/
#define UART2_BASE          (0x11006000) /*UART2*/
#define UART3_BASE          (0x11007000) /*UART3*/

#define I2C0_BASE           (0x11009000) /*I2C0*/
#define I2C1_BASE           (0x1100A000) /*I2C1*/
#define I2C2_BASE           (0x1100B000) /*I2C2*/
#define I2C_BASE             I2C0_BASE 

#define AUDIO_BASE                (0x11220000)
#define MJC_CONFIG_BASE      (0x12000000)
#define MFGCFG_BASE              (0x13000000)
#define MMSYS_CONFIG_BASE (0x14000000)
#define IMGSYS_BASE               (0x15000000)
#define VDEC_GCON_BASE       (0x16000000)
#define VENC_GCON_BASE       (0x17000000)

/*WDT*/

#define WDT_MODE	 ((volatile uint16 *)(WDT_BASE + 0x0000))
#define WDT_LENGTH	 ((volatile uint16 *)(WDT_BASE + 0x0004))
#define WDT_RESTART	 ((volatile uint16 *)(WDT_BASE + 0x0008))
#define WDT_SWRST       ((volatile uint16 *)(WDT_BASE + 0x0014))
#define WDT_DEBUG_CTL ((volatile uint16 *)(WDT_BASE + 0x0040))

/*GPT*/
#define GPT_TIMER3_CON	((volatile uint16 *)(GPT_BASE + 0x001C))
#define GPT_TIMER3_DATA	((volatile uint16 *)(GPT_BASE + 0x0020))
#define GPT_TIMER3_PRESCALE	((volatile uint16 *)(GPT_BASE + 0x0024))		// 0(16K), 1(8K), 2(4K), 3(2K), 4(1K)
/*CONFIG*/
#define HW_CODE             ((volatile uint16 *)(VER_BASE + 0x0000))
#define HW_SUBCODE          ((volatile uint16 *)(VER_BASE + 0x0004))
#define HW_VERSION          ((volatile uint16 *)(VER_BASE + 0x0008))
#define SW_VERSION          ((volatile uint16 *)(VER_BASE + 0x000C))

/*GPIO*/
#define GPIO_base                   (0x10005000)    // gpio
#define TRAP_INFO                   ((P_U32)(GPIO_base+0x0144))
#define BOOT_TRAP                   0x07    // [2:0]
                                            // 3'b0XX   EMMC
                                            // 3'b100   PDDR2 MLC NAND
                                            // 3'b101   PDDR2 SLC NAND
                                            // 3'b110   Legacy NAND
                                            // 3'b111   Toggle/ONFI/TLC NAND
#define BOOT_SEL                    0x04    // [2]=1'b0: EMMC; 1'b1: NAND
#define BOOT_EMMC                   0x00    // [2]=1'b0
#define BOOT_NAND                   0x04    // [2]=1'b1

/*=======================================================================*/
/* BOOTROM power down mode                                               */
/*=======================================================================*/
#define BOOTROM_SEC_CTRL                                (CONFIG_BASE + 0x804)
#define SW_ROM_PD                                       (1U << 31)


#endif

