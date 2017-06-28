#ifndef _REG_BASE_H_
#define _REG_BASE_H_


/*=======================================================================*/
/* Constant Definitions                                                  */
/*=======================================================================*/

#define IO_PHYS             (0x10000000)
#define IO_SIZE             (0x02000000)

#define VER_BASE            (0x08000000)

/*=======================================================================*/
/* Register Bases                                                        */
/*=======================================================================*/
/* FIXME, these registers are defined for mt6589, not sure the corresponding register in mt6582*/
#define TOPCKGEN_BASE       (IO_PHYS + 0x0000)
#define CONFIG_BASE         (IO_PHYS + 0x1000)		/* FIXME do not list in MT8163 memory map*/
#define INFRACFG_AO_BASE    (IO_PHYS + 0x1000)
#define IOCFG_L_BASE        (IO_PHYS + 0x2000)
#define IOCFG_B_BASE        (IO_PHYS + 0x2400)
#define IOCFG_R_BASE        (IO_PHYS + 0x2800)
#define IOCFG_T_BASE        (IO_PHYS + 0x2C00)
#define PERICFG_BASE        (IO_PHYS + 0x3000)
#define PERI_CON_BASE       (IO_PHYS + 0x00003000)  /* CHECKME & FIXME */

#define DRAMC0_BASE         (IO_PHYS + 0x4000)
#define GPIO_BASE           (IO_PHYS + 0x5000)
#define SPM_BASE            (IO_PHYS + 0x6000)
#define RGU_BASE            (IO_PHYS + 0x7000)
#define GPT_BASE            (IO_PHYS + 0x8000)
#define PWRAP_BASE          (IO_PHYS + 0xD000)
#define DDRPHY_BASE         (IO_PHYS + 0xF000)
#define KPD_BASE            (IO_PHYS + 0x00010000)
#define SRAMROM_BASE        (IO_PHYS + 0x00202000)

#define CKSYS_BASE 				(0x10000000)
#define	CONFIG_BASE				(0x10001000) //Sheet InfrasysAPB_AO, line 4
#define WDT_BASE				(0x10007000) //Sheet InfrasysAPB_AO, line 10, RGU includes WDT
#define	EMI_BASE			    (0x10203000) //Sheet InfrasysAPB, line 5

#define DMA_BASE				(0x11000000) //Sheet PerisysAPB, line 2
#define	UART1_BASE		        (0x11002000) //Sheet PerisysAPB, line 4
#define	UART2_BASE		        (0x11003000) //Sheet PerisysAPB, line 5
#define	UART3_BASE		        (0x11004000) //Sheet PerisysAPB, line 6
#define	UART4_BASE		        (0x11005000) //Sheet PerisysAPB, line 7
#define I2C_BASE                (0x11009000) //Sheet PerisysAPB, line 9


#define DRAMC_NAO_BASE          (0x1020E000)


/*WDT*/
#define WDT_MODE				((volatile uint16 *)(WDT_BASE + 0x0000))
#define WDT_LENGTH				((volatile uint16 *)(WDT_BASE + 0x0004))
#define WDT_RESTART				((volatile uint16 *)(WDT_BASE + 0x0008))
#define WDT_SWRST               ((volatile uint16 *)(WDT_BASE + 0x0014))
/*GPT*/
#define GPT_TIMER3_CON			((volatile uint16 *)(GPT_BASE + 0x001C))
#define GPT_TIMER3_DATA			((volatile uint16 *)(GPT_BASE + 0x0020))
#define GPT_TIMER3_PRESCALE		((volatile uint16 *)(GPT_BASE + 0x0024))		// 0(16K), 1(8K), 2(4K), 3(2K), 4(1K)
/*CONFIG*/
#define HW_CODE                 ((volatile uint16 *)(VER_BASE + 0x0000))
#define HW_SUBCODE              ((volatile uint16 *)(VER_BASE + 0x0004))
#define HW_VERSION              ((volatile uint16 *)(VER_BASE + 0x0008))
#define SW_VERSION              ((volatile uint16 *)(VER_BASE + 0x000C))


#endif

