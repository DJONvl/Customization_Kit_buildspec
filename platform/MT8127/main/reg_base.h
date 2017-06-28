#ifndef _REG_BASE_H_
#define _REG_BASE_H_
#if !defined(BB_MT8127)
#error "not MT8127 project"
#endif

#define	CONFIG_base				(0x10001000) //Sheet InfrasysAPB_AO, line 4
#define VER_base                (0x08000000) //Sheet MT6582 Top, line 7
#define	EMI_base			    (0x10203000) //Sheet InfrasysAPB, line 5
#define dramc0_BASE             (0x10004000) //Sheet InfrasysAPB_AO, line 7
#define WDT_base				(0x10007000) //Sheet InfrasysAPB_AO, line 10, RGU includes WDT
#define GPIO_base               (0x10005000) //Sheet InfrasysAPB_AO, line 8
#define	GPT_base				(0x10008000) //Sheet InfrasysAPB_AO, line 11
#define I2C_base                (0x11007000) //Sheet PerisysAPB, line 9
/*UART*/
#define	UART1_base		        (0x11002000) //Sheet PerisysAPB, line 4
#define	UART2_base		        (0x11003000) //Sheet PerisysAPB, line 5
#define	UART3_base		        (0x11004000) //Sheet PerisysAPB, line 6
#define	UART4_base		        (0x11005000) //Sheet PerisysAPB, line 7
/*WDT*/
#define WDT_MODE				((volatile uint16 *)(WDT_base + 0x0000))
#define WDT_LENGTH				((volatile uint16 *)(WDT_base + 0x0004))
#define WDT_RESTART				((volatile uint16 *)(WDT_base + 0x0008))
#define WDT_SWRST               ((volatile uint16 *)(WDT_base + 0x0014))
/*GPT*/
#define GPT_TIMER3_CON			((volatile uint16 *)(GPT_base + 0x001C))
#define GPT_TIMER3_DATA			((volatile uint16 *)(GPT_base + 0x0020))
#define GPT_TIMER3_PRESCALE		((volatile uint16 *)(GPT_base + 0x0024))		// 0(16K), 1(8K), 2(4K), 3(2K), 4(1K)
/*CONFIG*/
#define HW_CODE                 ((volatile uint16 *)(VER_base + 0x0000))
#define HW_SUBCODE              ((volatile uint16 *)(VER_base + 0x0004))
#define HW_VERSION              ((volatile uint16 *)(VER_base + 0x0008))
#define SW_VERSION              ((volatile uint16 *)(VER_base + 0x000C))


#endif
