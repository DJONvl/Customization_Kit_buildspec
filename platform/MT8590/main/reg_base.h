#ifndef _REG_BASE_H_
#define _REG_BASE_H_
#if !defined(BB_MT8590)
#error "not MT8590 project"
#endif

#define	CONFIG_base				(0x10001000) //Sheet InfrasysAPB_AO, line 4 //no change
#define VER_base                (0x08000000) //Sheet MT6582 Top, line 7 //no change
#define	EMI_base			    (0x10203000) //Sheet InfrasysAPB, line 5//no change
#define dramc0_BASE             (0x10004000) //Sheet InfrasysAPB_AO, line 7//no change
#define WDT_base				(0x10007000) //Sheet InfrasysAPB_AO, line 10, RGU includes WDT//no change
#define GPIO_base               (0x10005000) //Sheet InfrasysAPB_AO, line 8//no change
#define	GPT_base				(0x10008000) //Sheet InfrasysAPB_AO, line 11//no change
#define I2C_base                (0x11007000) //Sheet PerisysAPB, line 9//no change
#define  DMA_base				(0x11000000) //Sheet PerisysAPB, line 2
/*UART*/
#define	UART1_base		        (0x11002000) //Sheet PerisysAPB, line 4//no change
#define	UART2_base		        (0x11003000) //Sheet PerisysAPB, line 5//no change
#define	UART3_base		        (0x11004000) //Sheet PerisysAPB, line 6//no change
#define	UART4_base		        (0x11005000) //Sheet PerisysAPB, line 7//no change
/*WDT*/
//8590 has 2 WDT. SPM issue reset signal via primary wd. M1, P1 evb has pmic.
#define WDT_MODE				((volatile uint16 *)(WDT_base + 0x0000))
#define WDT_LENGTH				((volatile uint16 *)(WDT_base + 0x0004))
#define WDT_RESTART				((volatile uint16 *)(WDT_base + 0x0008))
#define WDT_SWRST               ((volatile uint16 *)(WDT_base + 0x0014))
#define WDT_SPM_WDT               ((volatile uint16 *)(WDT_base + 0x0030))
/*GPT*/
#define GPT_TIMER3_CON			((volatile uint16 *)(GPT_base + 0x001C))
#define GPT_TIMER3_DATA			((volatile uint16 *)(GPT_base + 0x0020))
#define GPT_TIMER3_PRESCALE		((volatile uint16 *)(GPT_base + 0x0024))		// 0(16K), 1(8K), 2(4K), 3(2K), 4(1K)
/*CONFIG*/
#define HW_CODE                 ((volatile uint16 *)(VER_base + 0x0000))
#define HW_SUBCODE              ((volatile uint16 *)(VER_base + 0x0004))
#define HW_VERSION              ((volatile uint16 *)(VER_base + 0x0008))
#define SW_VERSION              ((volatile uint16 *)(VER_base + 0x000C))

//DMA
#define DMA_GLBSTA				((volatile uint32 *)(DMA_base + 0x0000))
#define DMA1_SRC				((volatile uint32 *)(DMA_base + 0x0100))
#define DMA1_DST				((volatile uint32 *)(DMA_base + 0x0104))
#define DMA1_COUNT				((volatile uint32 *)(DMA_base + 0x0110))
#define DMA1_CON				((volatile uint32 *)(DMA_base + 0x0114))
#define DMA1_START				((volatile uint32 *)(DMA_base + 0x0118))
#define DMA1_LIMITER			((volatile uint32 *)(DMA_base + 0x0128))

//EMI
#define EMI_CONX_DUMMY			((volatile uint32 *)(0xFFFFFFFF))
#define EMI_CONA		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB		((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_COND		((volatile uint32 *)(EMI_base + 0x000C))

#endif
