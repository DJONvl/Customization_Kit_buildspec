#ifndef _REG_BASE_H_
#define _REG_BASE_H_


#define VER_BASE                (0x08000000) //Sheet MT6582 Top, line 7



#define CKSYS_BASE 				(0x10000000)
#define	CONFIG_BASE				(0x10001000) //Sheet InfrasysAPB_AO, line 4
#define PERI_CON_BASE           (0x10003000) //Sheet InfrasysAPB_AO, line 6
#define DRAMC0_BASE             (0x10004000) //Sheet InfrasysAPB_AO, line 7
#define GPIO_BASE               (0x10005000) //Sheet InfrasysAPB_AO, line 8
#define WDT_BASE				(0x10007000) //Sheet InfrasysAPB_AO, line 10, RGU includes WDT
#define	GPT_BASE				(0x10008000) //Sheet InfrasysAPB_AO, line 11
#define DDRPHY_BASE             (0x1000F000)

#define	EMI_BASE			    (0x10203000) //Sheet InfrasysAPB, line 5

#define DMA_BASE				(0x11000000) //Sheet PerisysAPB, line 2
#define	UART1_BASE		        (0x11002000) //Sheet PerisysAPB, line 4
#define	UART2_BASE		        (0x11003000) //Sheet PerisysAPB, line 5
#define	UART3_BASE		        (0x11004000) //Sheet PerisysAPB, line 6
#define	UART4_BASE		        (0x11005000) //Sheet PerisysAPB, line 7
#define I2C_BASE                (0x11007000) //Sheet PerisysAPB, line 9


#define APMIXED_BASE 			(0x10209000)
#define DRAMC_NAO_BASE          (0x1020E000)


#define MMSYS_CONFIG_BASE       (0x14000000)
#define INFRACFG_AO_BASE        (0x10001000)
#define MCUCFG_BASE             (0x10200000)
#define CA15L_CONFIG_BASE       (0x10200200)


#define IO_PHYS             (0x10000000)
#define PWRAP_BASE          (IO_PHYS + 0xD000)




/*WDT*/
#define WDT_MODE				((volatile uint16 *)(WDT_BASE + 0x0000))
#define WDT_LENGTH				((volatile uint16 *)(WDT_BASE + 0x0004))
#define WDT_RESTART				((volatile uint16 *)(WDT_BASE + 0x0008))
#define WDT_SWRST               ((volatile uint16 *)(WDT_BASE + 0x0014))
#define WDT_NONRST_REG          ((volatile uint16 *)(WDT_BASE + 0x0020))
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

