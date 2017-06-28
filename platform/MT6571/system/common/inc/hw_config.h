/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2004
*
*******************************************************************************/
#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_

#include "SW_TYPES.H"
#include "DOWNLOAD.H"
#include "mtk_mcu.h"

#if defined(BB_MT6572) || defined(BB_MT6582) || defined(BB_MT6571) || defined(BB_MT6593)
//Mason added, need further changed
#define FREE_INT_SYSRAM_BEGIN_ADDR	((uint32)0x82000000)
#define FREE_EXT_RAM_BEGIN_ADDR		((uint32)0x82000000)
#define EXT_RAM_BEGIN_ADDR			((uint32)0x82000000)
#define INT_SYSRAM_END_ADDR         ((uint32)0x82000000)
#else
extern uint32	Image$$FREE_INT_SYSRAM_BEGIN$$Base;
extern uint32	Image$$FREE_INT_TCM_BEGIN$$Base;
extern uint32	Image$$FREE_EXT_RAM_BEGIN$$Base;
extern uint32	Image$$EXT_RAM$$Base;

#define FREE_INT_SYSRAM_BEGIN_ADDR	((uint32)&Image$$FREE_INT_SYSRAM_BEGIN$$Base)
#define FREE_INT_TCM_BEGIN_ADDR		((uint32)&Image$$FREE_INT_TCM_BEGIN$$Base)
#define FREE_EXT_RAM_BEGIN_ADDR		((uint32)&Image$$FREE_EXT_RAM_BEGIN$$Base)
#define EXT_RAM_BEGIN_ADDR			((uint32)&Image$$EXT_RAM$$Base)

#endif

#define RAMSIZE_BEGIN_PATTERN	"<<<RAM_BEGIN>>>"
#define RAMSIZE_END_PATTERN		"<<<RAM_END>>>"
#define RAMRW_PATTERN           "<<<RAM_RW>>>"

//------------------------------------------------------------------------------
// ARM System
//------------------------------------------------------------------------------
#if defined(BB_MT6235) || defined(BB_MT6238) || defined(BB_MT6268B)
	#define	__ARM9__
#endif

//------------------------------------------------------------------------------
// New Sub-System Feature                                                          
//------------------------------------------------------------------------------
#if defined(BB_MT6238)    || defined(BB_MT6235) || defined(BB_TK6516_MD) || \
    defined(BB_TK6516_AP) || defined(BB_MT6268A)

    #define __MTK_EMI_V2__
    #define __MTK_RTC_V2__

#elif defined(BB_MT6268B) || defined(BB_MT6253) || defined(BB_MT6516_AP) 

    #define __MTK_EMI_V2__
    #define __MTK_RTC_V3__

 #else

    #define __MTK_RTC_V4__

#endif

//------------------------------------------------------------------------------
// BackDoor                                                          
//------------------------------------------------------------------------------
#if	defined(BB_TK6516_AP) || defined(BB_MT6516_AP)
	#define	BACKDOOR			((volatile uint16 *)(0x8003A000))
#elif	defined(BB_TK6516_MD) || defined(BB_MT6516_MD)
	#define	BACKDOOR			((volatile uint16 *)(0x811A0000))
#endif

//------------------------------------------------------------------------------
// Group of Register BaseAddress                                                
//------------------------------------------------------------------------------
#if defined(BB_MT6582) || defined(BB_MT6593)
	#define __MTK_BB_REG_BASEADDRESS_GROUP11__

#elif defined(BB_MT6572) || defined(BB_MT6571)
    #define __MTK_BB_REG_BASEADDRESS_GROUP12__
    
#elif defined(BB_MT6589) || defined(BB_MT8135)
    #define __MTK_BB_REG_BASEADDRESS_GROUP10__

#elif defined(BB_MT6575)
    #define __MTK_BB_REG_BASEADDRESS_GROUP9__

#elif defined(BB_MT6573)
	#define __MTK_BB_REG_BASEADDRESS_GROUP8__

#elif defined(BB_MT6255)
	#define __MTK_BB_REG_BASEADDRESS_GROUP7__

#elif defined(BB_MT6276)
	#define __MTK_BB_REG_BASEADDRESS_GROUP6__

#elif defined(BB_MT6268A) || defined(BB_MT6268B)
	#define __MTK_BB_REG_BASEADDRESS_GROUP5__
	
#elif	defined(BB_TK6516_AP) || defined(BB_MT6516_AP) || defined(BB_MT6516_NU) || defined(BB_MT6516)
	#define __MTK_BB_REG_BASEADDRESS_GROUP4__

#elif	defined(BB_TK6516_MD) || defined(BB_MT6516_MD)  
	#define __MTK_BB_REG_BASEADDRESS_GROUP3__

#elif	defined(BB_MT6238) || defined(BB_MT6235) || defined(BB_MT6253) || defined(BB_MT6251)
	#define __MTK_BB_REG_BASEADDRESS_GROUP2__
	
#else	//MT6205B, MT6217, MT6218, MT6218B, MT6219, MT6223, MT6225, MT6226, MT6227, MT6228, MT6229, MT6230
	#define __MTK_BB_REG_BASEADDRESS_GROUP1__
#endif	//BB_MT6238



//------------------------------------------------------------------------------
// CONFIG Register                                                          
//------------------------------------------------------------------------------
#if defined(__MTK_BB_REG_BASEADDRESS_GROUP11__)
	#define	CONFIG_base				(0x10001000)
    #define VER_base                (0x08000000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP12__)
	#define	CONFIG_base				(0x10003000)
    #define VER_base                (0x08000000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP10__)
	#define	CONFIG_base				(0x10001000)
    #define VER_base                (0x08000000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP9__)
	#define	CONFIG_base				(0xC0001000)
    #define VER_base                (0xF8000000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP8__)
	#define	CONFIG_base				(0x70026000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP7__)
	#define	CONFIG_base				(0x70000000)
#elif 	defined(__MTK_BB_REG_BASEADDRESS_GROUP6__)
	#define	CONFIG_base				(0x610D0000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP5__)
	#define	CONFIG_base				(0x84010000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__) || defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define CONFIG_base				(0x80010000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP4__)
	#define CONFIG_base				(0x80001000)
#else
	#define CONFIG_base				(0x80000000)
#endif	//BB_MT6238

#if defined(__MTK_BB_REG_BASEADDRESS_GROUP9__) || defined(__MTK_BB_REG_BASEADDRESS_GROUP12__) || defined(__MTK_BB_REG_BASEADDRESS_GROUP11__) ||defined(__MTK_BB_REG_BASEADDRESS_GROUP10__)
#define HW_CODE                 ((volatile uint16 *)(VER_base + 0x0000))
#define HW_SUBCODE              ((volatile uint16 *)(VER_base + 0x0004))
#define HW_VERSION              ((volatile uint16 *)(VER_base + 0x0008))
#define SW_VERSION              ((volatile uint16 *)(VER_base + 0x000C))
#else
#define HW_VERSION				((volatile uint16 *)(CONFIG_base + 0x0000))
#define SW_VERSION				((volatile uint16 *)(CONFIG_base + 0x0004))
#define HW_CODE				    ((volatile uint16 *)(CONFIG_base + 0x0008))
#endif

#define PDN_CON0				((volatile uint16 *)(CONFIG_base + 0x0300))
#define PDN_CON1				((volatile uint16 *)(CONFIG_base + 0x0304))

#define PDN_CLR0_MT6218B		((volatile uint16 *)(CONFIG_base + 0x0320))
#define PDN_CLR1_MT6218B		((volatile uint16 *)(CONFIG_base + 0x0324))

#define PDN_CLR0_TK6516			((volatile uint16 *)(CONFIG_base + 0x0340))
#define PDN_CLR1_TK6516			((volatile uint16 *)(CONFIG_base + 0x0344))

#define MDPLL_MT6227			((volatile uint16 *)(CONFIG_base + 0x0100))
#define CLK_CON_MT6227			((volatile uint16 *)(CONFIG_base + 0x0108))
#define MCUCLK_CON_MT6227		((volatile uint16 *)(CONFIG_base + 0x0114))

#define MDPLL_MT6228			((volatile uint16 *)(CONFIG_base + 0x0100))
#define CLK_CON_MT6228			((volatile uint16 *)(CONFIG_base + 0x0110))
#define MCUCLK_CON_MT6228		((volatile uint16 *)(CONFIG_base + 0x0118))

#define MCUCLK_CON_MT6238		((volatile uint16 *)(CONFIG_base + 0x0118))

#define PDN_CLR3_MT6253			((volatile uint16 *)(CONFIG_base + 0x032C))

#define GC_CLR3_MT6268			((volatile uint16 *)(CONFIG_base + 0x0320))

#define MCU_CG_CLR0_MT6276		((volatile uint16 *)(CONFIG_base + 0x0320))

#define MCU_CG_CLR0_MT6255		((volatile uint16 *)(CONFIG_base + 0x0720))
#define MCU_CG_CLR1_MT6255		((volatile uint16 *)(CONFIG_base + 0x0724))

#define MCU_CG_CLR0_MT6573		((volatile uint16 *)(CONFIG_base + 0x0318))

#define PDN_CLR3_MT6251			((volatile uint16 *)(CONFIG_base + 0x032C))

/* FIXME::  for Setup PLL and NUTL_NFIv3_Init
#define PDN_CLR0_MT6255			((volatile uint16 *)(CONFIG_base + 0x0720))     // Clock Gating Clear Register 0
#define PDN_CLR1_MT6255			((volatile uint16 *)(CONFIG_base + 0x0724))     // Clock Gating Clear Register 1
*/
//------------------------------------------------------------------------------
// PLL/Clock square configuration Register                                                          
//------------------------------------------------------------------------------
#if defined(__MTK_BB_REG_BASEADDRESS_GROUP7__)
	#define PLL_base				(0x701A0000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP6__)
	#define PLL_base				(0x61140000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP2__)
	#define PLL_base				(0x83000000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP4__)
	#define PLL_base				(0x80060000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define PLL_base				(0xF0060000)
#else
	#define PLL_base				(0)
#endif	

#define MDPLL_MT6238			((volatile uint16 *)(PLL_base + 0x0000))
#define CLK_CON_MT6238			((volatile uint16 *)(PLL_base + 0x0018))
#define PDN_CON_MT6238			((volatile uint16 *)(PLL_base + 0x001C))

#define PLL_TK6516				((volatile uint16 *)(PLL_base + 0x0000))
#define PLL2_TK6516				((volatile uint16 *)(PLL_base + 0x0004))
#define PLL3_TK6516				((volatile uint16 *)(PLL_base + 0x0060))
#define CPLL_TK6516				((volatile uint16 *)(PLL_base + 0x0008))
#define CPLL2_TK6516			((volatile uint16 *)(PLL_base + 0x000C))
#define TPLL_TK6516				((volatile uint16 *)(PLL_base + 0x0010))
#define TPLL2_TK6516			((volatile uint16 *)(PLL_base + 0x0014))
#define APLL_TK6516				((volatile uint16 *)(PLL_base + 0x0018))
#define APLL2_TK6516			((volatile uint16 *)(PLL_base + 0x001C))
#define LCDPLL_TK6516			((volatile uint16 *)(PLL_base + 0x0020))
#define LCDPLL2_TK6516			((volatile uint16 *)(PLL_base + 0x0024))
#define CEVAPLL_TK6516			((volatile uint16 *)(PLL_base + 0x0030))
#define CEVAPLL2_TK6516			((volatile uint16 *)(PLL_base + 0x0034))
#define CLK_CON_TK6516			((volatile uint16 *)(PLL_base + 0x0028))
#define PDN_CON_TK6516			((volatile uint16 *)(PLL_base + 0x002C))
#define PLL_BIAS_TK6516			((volatile uint16 *)(PLL_base + 0x0050))
#define IDN_TK6516				((volatile uint16 *)(PLL_base + 0x0070))

//FIXME: simluation on MT6516
// MT6516_AP or MT6516_MD
#define PDN_CON_MT6516          ((volatile uint16 *)(PLL_base + 0x0010))
#define CLK_CON_MT6516          ((volatile uint16 *)(PLL_base + 0x0014))
#define MPLL_MT6516             ((volatile uint16 *)(PLL_base + 0x0020))
#define MPLL2_MT6516            ((volatile uint16 *)(PLL_base + 0x0024))
#define UPLL_MT6516             ((volatile uint16 *)(PLL_base + 0x0030))
#define UPLL2_MT6516            ((volatile uint16 *)(PLL_base + 0x0034))
#define CPLL_MT6516             ((volatile uint16 *)(PLL_base + 0x0038))
#define CPLL2_MT6516            ((volatile uint16 *)(PLL_base + 0x003C))
#define TPLL_MT6516             ((volatile uint16 *)(PLL_base + 0x0040))
#define TPLL2_MT6516            ((volatile uint16 *)(PLL_base + 0x0044))
#define CPLL3_MT6516            ((volatile uint16 *)(PLL_base + 0x0048))
#define PLL_RES_CON0_MT6516     ((volatile uint16 *)(PLL_base + 0x004C))
#define PLL_BIAS_MT6516         ((volatile uint16 *)(PLL_base + 0x0050))
#define MCPLL_MT6516            ((volatile uint16 *)(PLL_base + 0x0058))
#define MCPLL2_MT6516           ((volatile uint16 *)(PLL_base + 0x005C))
#define CEVAPLL_MT6516          ((volatile uint16 *)(PLL_base + 0x0060))
#define CEVAPLL2_MT6516         ((volatile uint16 *)(PLL_base + 0x0064))
#define PLL_IDN_MT6516          ((volatile uint16 *)(PLL_base + 0x0070))
#define XOSC32_AC_CON_MT6516    ((volatile uint16 *)(PLL_base + 0x007C))

#define PLL_CON4_MT6276	((volatile uint16*)(PLL_base+0x0110))    
#define PLL_CON5_MT6276	((volatile uint16*)(PLL_base+0x0114))
#define MPLL_CON0_MT6276	((volatile uint16*)(PLL_base+0x0140))
#define MPLL_CON1_MT6276	((volatile uint16*)(PLL_base+0x0144))
#define DPLL_CON0_MT6276	((volatile uint16*)(PLL_base+0x0180))
#define DPLL_CON1_MT6276	((volatile uint16*)(PLL_base+0x0184))
#define EPLL_CON0_MT6276	((volatile uint16*)(PLL_base+0x01C0))
#define EPLL_CON1_MT6276	((volatile uint16*)(PLL_base+0x01C4))    
#define WPLL_CON0_MT6276	((volatile uint16*)(PLL_base+0x0240))		    
#define GPLL_CON0_MT6276	((volatile uint16*)(PLL_base+0x0280))    


// for SetupPLL_MT6255() before detecting D-SRAM
#define PLL_CON1_MT6255         ((volatile uint16*)(PLL_base + 0x0104))
#define PLL_CON2_MT6255         ((volatile uint16*)(PLL_base + 0x0108))
#define PLL_CON3_MT6255         ((volatile uint16*)(PLL_base + 0x010C))
#define PLL_CON4_MT6255         ((volatile uint16*)(PLL_base + 0x0110))
#define PLL_CON5_MT6255         ((volatile uint16*)(PLL_base + 0x0114))
#define UPLL_CON2_MT6255        ((volatile uint16*)(PLL_base + 0x0208))	
#define UPLL_CON3_MT6255        ((volatile uint16*)(PLL_base + 0x020C))	
#define MPLL_CON2_MT6255       ((volatile uint16*)(PLL_base + 0x0308))	
#define MPLL_CON3_MT6255        ((volatile uint16*)(PLL_base + 0x030C))
#define EPLL_CON2_MT6255        ((volatile uint16*)(PLL_base + 0x0508))
#define EPLL_CON3_MT6255        ((volatile uint16*)(PLL_base + 0x050C))
#define TDMA_FHCON4_MT6255   ((volatile uint16*)(PLL_base + 0x0810))
#define TDMA_FHCON6_MT6255   ((volatile uint16*)(PLL_base + 0x0818))
//------------------------------------------------------------------------------
// EMI Register                                                          
//------------------------------------------------------------------------------
#if defined(__MTK_BB_REG_BASEADDRESS_GROUP11__)
	#define	EMI_base			    (0x10203000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP12__)
	#define	EMI_base			    (0x10004000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP10__)
	#define	EMI_BASE			    (0x10203000)	
	#define	EMI_base			    (0x10203000)	
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP9__)
	#define	EMI_base			    (0xC0003000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP8__)
	#define	EMI_base			    (0x70000000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP7__)
	#define	EMI_base				(0x70010000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP6__)
	#define	EMI_base				(0x60000000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP5__)
	#define	EMI_base				(0x85000000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__)
	#define EMI_base				(0x81000000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP4__)
	#define EMI_base				(0x80020000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define EMI_base				(0xF0020000)
#else
	#define EMI_base				(0x80010000)
#endif	//BB_TK6516

#define EMI_CONX_DUMMY			((volatile uint32 *)(0xFFFFFFFF))

//MT6571
#define EMI_CONA_MT6571B		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6571B		((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC_MT6571B		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_COND_MT6571B		((volatile uint32 *)(EMI_base + 0x000C))

//MT8135
#define EMI_CONA_MT8135		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT8135		((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC_MT8135		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_COND_MT8135		((volatile uint32 *)(EMI_base + 0x000C))

//MT6572
#define EMI_CONA_MT6572B		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6572B		((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC_MT6572B		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_COND_MT6572B		((volatile uint32 *)(EMI_base + 0x000C))

//MT6582
#define EMI_CONA_MT6582B		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6582B		((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC_MT6582B		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_COND_MT6582B		((volatile uint32 *)(EMI_base + 0x000C))
//MT6593
#define EMI_CONA_MT6593		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6593		((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC_MT6593		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_COND_MT6593		((volatile uint32 *)(EMI_base + 0x000C))

//MT6589
#define EMI_CONA_MT6589		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6589		((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC_MT6589		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_COND_MT6589		((volatile uint32 *)(EMI_base + 0x000C))

//MT6575
#define EMI_CONA_MT6575B		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6575B		((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC_MT6575B		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_COND_MT6575B		((volatile uint32 *)(EMI_base + 0x000C))

//MT6205B
#define EMI_CONA_MT6205B		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6205B		((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC_MT6205B		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_COND_MT6205B		((volatile uint32 *)(EMI_base + 0x000C))
//MT6218
#define EMI_CONA_MT6218			((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6218			((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_CONC_MT6218			((volatile uint32 *)(EMI_base + 0x0010))
#define EMI_COND_MT6218			((volatile uint32 *)(EMI_base + 0x0018))
#define EMI_CONE_MT6218			((volatile uint32 *)(EMI_base + 0x0020))
#define EMI_CONF_MT6218			((volatile uint32 *)(EMI_base + 0x0028))
//MT6218B
#define EMI_CONA_MT6218B		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6218B		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_CONC_MT6218B		((volatile uint32 *)(EMI_base + 0x0010))
#define EMI_COND_MT6218B		((volatile uint32 *)(EMI_base + 0x0018))
#define EMI_CONE_MT6218B		((volatile uint32 *)(EMI_base + 0x0020))
#define EMI_CONF_MT6218B		((volatile uint32 *)(EMI_base + 0x0028))
#define EMI_CONG_MT6218B		((volatile uint32 *)(EMI_base + 0x0030))
#define EMI_CONH_MT6218B		((volatile uint32 *)(EMI_base + 0x0038))
//MT6228
#define EMI_CONA_MT6228			((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6228			((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_CONC_MT6228			((volatile uint32 *)(EMI_base + 0x0010))
#define EMI_COND_MT6228			((volatile uint32 *)(EMI_base + 0x0018))

#define EMI_CONI_MT6228			((volatile uint32 *)(EMI_base + 0x0040))
#define EMI_CONJ_MT6228			((volatile uint32 *)(EMI_base + 0x0048))
#define EMI_CONK_MT6228			((volatile uint32 *)(EMI_base + 0x0050))
#define EMI_CONL_MT6228			((volatile uint32 *)(EMI_base + 0x0058))
#define EMI_GENA_MT6228			((volatile uint32 *)(EMI_base + 0x0068))
#define EMI_GENB_MT6228			((volatile uint32 *)(EMI_base + 0x0070))
#define EMI_GENC_MT6228			((volatile uint32 *)(EMI_base + 0x0078))
//MT6238
#define EMI_CONA_MT6238			((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6238			((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_CONC_MT6238			((volatile uint32 *)(EMI_base + 0x0010))
#define EMI_COND_MT6238			((volatile uint32 *)(EMI_base + 0x0018))

#define EMI_CONI_MT6238			((volatile uint32 *)(EMI_base + 0x0040))
#define EMI_CONJ_MT6238			((volatile uint32 *)(EMI_base + 0x0048))
#define EMI_CONK_MT6238			((volatile uint32 *)(EMI_base + 0x0050))
#define EMI_CONL_MT6238			((volatile uint32 *)(EMI_base + 0x0058))
#define EMI_CONM_MT6238			((volatile uint32 *)(EMI_base + 0x0060))
#define EMI_CONN_MT6238			((volatile uint32 *)(EMI_base + 0x0068))
#define EMI_GENA_MT6238			((volatile uint32 *)(EMI_base + 0x0070))
#define EMI_GENB_MT6238			((volatile uint32 *)(EMI_base + 0x0078))
#define EMI_GENC_MT6238			((volatile uint32 *)(EMI_base + 0x0080))
#define EMI_GEND_MT6238			((volatile uint32 *)(EMI_base + 0x0088))
//TK6516
#define EMI_CONA_TK6516			((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_TK6516			((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_CONC_TK6516			((volatile uint32 *)(EMI_base + 0x0010))
#define EMI_COND_TK6516			((volatile uint32 *)(EMI_base + 0x0018))

#define EMI_CONI_TK6516			((volatile uint32 *)(EMI_base + 0x0040))
#define EMI_CONJ_TK6516			((volatile uint32 *)(EMI_base + 0x0048))
#define EMI_CONK_TK6516			((volatile uint32 *)(EMI_base + 0x0050))
#define EMI_CONL_TK6516			((volatile uint32 *)(EMI_base + 0x0058))
#define EMI_CONM_TK6516			((volatile uint32 *)(EMI_base + 0x0060))
#define EMI_CONN_TK6516			((volatile uint32 *)(EMI_base + 0x0068))
#define EMI_GENA_TK6516			((volatile uint32 *)(EMI_base + 0x0070))
#define EMI_GENB_TK6516			((volatile uint32 *)(EMI_base + 0x0078))
#define EMI_GENC_TK6516			((volatile uint32 *)(EMI_base + 0x0080))
#define EMI_GEND_TK6516			((volatile uint32 *)(EMI_base + 0x0088))
//FIXME: simluation on MT6516
//MT6516
#define EMI_CONA_MT6516    ((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6516    ((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_CONC_MT6516    ((volatile uint32 *)(EMI_base + 0x0010))
#define EMI_COND_MT6516    ((volatile uint32 *)(EMI_base + 0x0018))

#define EMI_CONI_MT6516    ((volatile uint32 *)(EMI_base + 0x0040))
#define EMI_CONJ_MT6516    ((volatile uint32 *)(EMI_base + 0x0048))
#define EMI_CONK_MT6516    ((volatile uint32 *)(EMI_base + 0x0050))
#define EMI_CONL_MT6516    ((volatile uint32 *)(EMI_base + 0x0058))
#define EMI_CONM_MT6516    ((volatile uint32 *)(EMI_base + 0x0060))
#define EMI_CONN_MT6516    ((volatile uint32 *)(EMI_base + 0x0068))

#define EMI_GENA_MT6516    ((volatile uint32 *)(EMI_base + 0x0070))
#define EMI_GENB_MT6516    ((volatile uint32 *)(EMI_base + 0x0078))
#define EMI_GENC_MT6516    ((volatile uint32 *)(EMI_base + 0x0080))
#define EMI_GEND_MT6516    ((volatile uint32 *)(EMI_base + 0x0088))

#define EMI_DELA_MT6516    ((volatile uint32 *)(EMI_base + 0x0098))
#define EMI_DELB_MT6516    ((volatile uint32 *)(EMI_base + 0x00A0))
#define EMI_DELC_MT6516    ((volatile uint32 *)(EMI_base + 0x00A8))
#define EMI_DELD_MT6516    ((volatile uint32 *)(EMI_base + 0x00B0))
#define EMI_DELE_MT6516    ((volatile uint32 *)(EMI_base + 0x00B8))
#define EMI_DELF_MT6516    ((volatile uint32 *)(EMI_base + 0x00C0))
#define EMI_DELG_MT6516    ((volatile uint32 *)(EMI_base + 0x00C8))
#define EMI_DELH_MT6516    ((volatile uint32 *)(EMI_base + 0x00D0))
#define EMI_DELI_MT6516    ((volatile uint32 *)(EMI_base + 0x00D8))
#define EMI_DELJ_MT6516    ((volatile uint32 *)(EMI_base + 0x00E0))

#define EMI_DQSA_MT6516    ((volatile uint32 *)(EMI_base + 0x01B0))
#define EMI_DQSB_MT6516    ((volatile uint32 *)(EMI_base + 0x01B8))
#define EMI_DQSC_MT6516    ((volatile uint32 *)(EMI_base + 0x01C0))
#define EMI_DQSD_MT6516    ((volatile uint32 *)(EMI_base + 0x01C8))

#define EMI_DRCT_MT6516    ((volatile uint32 *)(EMI_base + 0x01A0))

//MT6268A
#define EMI_CONA_MT6268A		((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6268A		((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_CONC_MT6268A		((volatile uint32 *)(EMI_base + 0x0010))
#define EMI_COND_MT6268A		((volatile uint32 *)(EMI_base + 0x0018))

#define EMI_CONI_MT6268A		((volatile uint32 *)(EMI_base + 0x0040))
#define EMI_CONJ_MT6268A		((volatile uint32 *)(EMI_base + 0x0048))
#define EMI_CONK_MT6268A		((volatile uint32 *)(EMI_base + 0x0050))
#define EMI_CONL_MT6268A		((volatile uint32 *)(EMI_base + 0x0058))
#define EMI_CONM_MT6268A		((volatile uint32 *)(EMI_base + 0x0060))
#define EMI_CONN_MT6268A		((volatile uint32 *)(EMI_base + 0x0068))
#define EMI_GENA_MT6268A		((volatile uint32 *)(EMI_base + 0x0070))
#define EMI_GENB_MT6268A		((volatile uint32 *)(EMI_base + 0x0078))
#define EMI_GENC_MT6268A		((volatile uint32 *)(EMI_base + 0x0080))
#define EMI_GEND_MT6268A		((volatile uint32 *)(EMI_base + 0x0088))
#define EMI_DQSA_MT6268A		((volatile uint32 *)(EMI_base + 0x01B0))

//MT6268B
#define EMI_DELA_MT6268B    ((volatile uint32 *)(EMI_base + 0x0098))
#define EMI_DELB_MT6268B    ((volatile uint32 *)(EMI_base + 0x00A0))
#define EMI_DELC_MT6268B    ((volatile uint32 *)(EMI_base + 0x00A8))
#define EMI_DELD_MT6268B    ((volatile uint32 *)(EMI_base + 0x00B0))
#define EMI_DELE_MT6268B    ((volatile uint32 *)(EMI_base + 0x00B8))
#define EMI_DELF_MT6268B    ((volatile uint32 *)(EMI_base + 0x00C0))
#define EMI_DELG_MT6268B    ((volatile uint32 *)(EMI_base + 0x00C8))
#define EMI_DELH_MT6268B    ((volatile uint32 *)(EMI_base + 0x00D0))
#define EMI_DELI_MT6268B    ((volatile uint32 *)(EMI_base + 0x00D8))
#define EMI_DELJ_MT6268B    ((volatile uint32 *)(EMI_base + 0x00E0))

#define EMI_DQSA_MT6268B    ((volatile uint32 *)(EMI_base + 0x01B0))
#define EMI_DQSB_MT6268B    ((volatile uint32 *)(EMI_base + 0x01B8))
#define EMI_DQSC_MT6268B    ((volatile uint32 *)(EMI_base + 0x01C0))
#define EMI_DQSD_MT6268B    ((volatile uint32 *)(EMI_base + 0x01C8))

#define EMI_ARBA_MT6268B    ((volatile uint32 *)(EMI_base + 0x0200))
#define EMI_ARBB_MT6268B    ((volatile uint32 *)(EMI_base + 0x0208))
#define EMI_ARBC_MT6268B    ((volatile uint32 *)(EMI_base + 0x0210))
#define EMI_ARBD_MT6268B    ((volatile uint32 *)(EMI_base + 0x0218))
#define EMI_ARBE_MT6268B    ((volatile uint32 *)(EMI_base + 0x0220))
#define EMI_ARBF_MT6268B    ((volatile uint32 *)(EMI_base + 0x0228))
#define EMI_ARBG_MT6268B    ((volatile uint32 *)(EMI_base + 0x0230))
#define EMI_ARBH_MT6268B    ((volatile uint32 *)(EMI_base + 0x0238))
#define EMI_ARBI_MT6268B    ((volatile uint32 *)(EMI_base + 0x0240))

#define EMI_DRCT_MT6268B    ((volatile uint32 *)(EMI_base + 0x01A0))

//MT6253
#define EMI_CONA_MT6253			((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6253			((volatile uint32 *)(EMI_base + 0x0004))
#define EMI_CONC_MT6253			((volatile uint32 *)(EMI_base + 0x0008))


/* FIXME:
//MT6255
#define EMI_CONA_MT6255			((volatile uint32 *)(EMI_base + 0x0000))
#define EMI_CONB_MT6255			((volatile uint32 *)(EMI_base + 0x0008))
#define EMI_GENB_MT6255			((volatile uint32 *)EMI_base + 0x0078))	// EMI_DRCT : EMI dummy read controls
#define EMI_GENC_MT6255			((volatile uint32 *)EMI_base + 0x0080))	// EMI_DDRV : LPDDR2 DRAM driving calibration
#define EMI_GEND_MT6255			((volatile uint32 *)EMI_base + 0x0088))	// config Driveing
*/
#define EMI_GENA_MT6255			((volatile uint32 *)EMI_base + 0x0070))	// config Driveing

//------------------------------------------------------------------------------
// DMA Register                                                          
//------------------------------------------------------------------------------
/* Koshi, 2011.01.19 { */
/* Note: SBC */
/* Koshi, 2011.01.19 } */
#if defined(__MTK_BB_REG_BASEADDRESS_GROUP11__)
	#define  DMA_base				(0x11000000) //Sheet PerisysAPB, line 2
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP12__)
	#define  DMA_base				(0x11000000) ) 
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP10__)
	#define  DMA_base				(0x11000000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP9__)
	#define  DMA_base				(0xC1001000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP8__)
	#define  DMA_base				(0x70002000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP7__)
	#define  DMA_base				(0x70030000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP6__)
	#define	DMA_base				(0x60020000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP5__)
	#define	DMA_base				(0x85020000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__) || defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define DMA_base				(0x81020000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP4__)
	#define DMA_base				(0x80022000)
#else
	#define DMA_base				(0x80030000)
#endif

#define DMA_GLBSTA				((volatile uint32 *)(DMA_base + 0x0000))

#define DMA1_SRC				((volatile uint32 *)(DMA_base + 0x0100))
#define DMA1_DST				((volatile uint32 *)(DMA_base + 0x0104))
#define DMA1_COUNT				((volatile uint32 *)(DMA_base + 0x0110))
#define DMA1_CON				((volatile uint32 *)(DMA_base + 0x0114))
#define DMA1_START				((volatile uint32 *)(DMA_base + 0x0118))
#define DMA1_LIMITER			((volatile uint32 *)(DMA_base + 0x0128))

//------------------------------------------------------------------------------
// RGU Register                                                          
//------------------------------------------------------------------------------
/* Koshi, 2011.01.19 { */
/* Note: SBC */
/* Koshi, 2011.01.19 } */
#if defined(__MTK_BB_REG_BASEADDRESS_GROUP11__)
    #define WDT_base				(0x10007000) //Sheet InfrasysAPB_AO, line 10, multi-define in reg_base.h
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP12__)       
	    #define WDT_base				(0x10007000)	 
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP10__)
    #define WDT_base				(0x10000000)	
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP9__)
    #define WDT_base				(0xC0000000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP8__)
    #define WDT_base				(0x70025000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP7__)
	#define WDT_base				(0x701E0000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP6__)
	#define	WDT_base				(0x610C0000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP5__)
	#define	WDT_base				(0x84030000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__) || defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define WDT_base				(0x80030000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP4__)
	#define WDT_base				(0x80003000)
#else
	#define WDT_base				(0x80040000)
#endif

//#define WDT_MODE				((volatile uint16 *)(WDT_base + 0x0000))
//#define WDT_LENGTH				((volatile uint16 *)(WDT_base + 0x0004))
//#define WDT_RESTART				((volatile uint16 *)(WDT_base + 0x0008))
//#define WDT_SWRST               ((volatile uint16 *)(WDT_base + 0x0014))

//------------------------------------------------------------------------------
// GPIO Register                                                                
//------------------------------------------------------------------------------
#if defined(__MTK_BB_REG_BASEADDRESS_GROUP11__)
    #define GPIO_base               (0x10005000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP12__)
    #define GPIO_base               (0x10005000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP10__)
    #define GPIO_base               (0x10005000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP9__)
    #define GPIO_base               (0xC0005000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP8__)
    #define GPIO_base               (0x70023000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP7__)
	#define GPIO_base				(0x701D0000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP5__)
	#define	GPIO_base				(0x84020000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__)
	#define GPIO_base				(0x80020000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP4__)
	#define GPIO_base				(0x80002000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define GPIO_base				(0xF0002000)
#else
	#define GPIO_base				(0x80120000)
#endif	//BB_TK6516

#define GPIO_MODE4_MT6218B		((volatile uint16 *)(GPIO_base + 0x0130))
#define GPIO_MODE4_MT6219		((volatile uint16 *)(GPIO_base + 0x0180))
#define GPIO_MODE4_MT6228		((volatile uint16 *)(GPIO_base + 0x01D0))
#define GPIO_MODE4_MT6238		((volatile uint16 *)(GPIO_base + 0x01D0))
#define GPIO_MODE7_TK6516_AP	((volatile uint16 *)(GPIO_base + 0x0660))
#define GPIO_MODE7_MT6268A		((volatile uint16 *)(GPIO_base + 0x0660))

#define GPIO_MODE5_MT6225		((volatile uint16 *)(GPIO_base + 0x0190))
#define GPIO_MODE6_MT6225		((volatile uint16 *)(GPIO_base + 0x01A0))

// GPS project 

// GPIO: MT6223, MT6225, MT6226/27
#define GPIO_DIR1_MT6223		((volatile uint16 *)(GPIO_base + 0x0000))
#define GPIO_DOUT1_MT6223		((volatile uint16 *)(GPIO_base + 0x00C0))
#define GPIO_MODE1_MT6223		((volatile uint16 *)(GPIO_base + 0x0150))

// GPIO: MT6228, MT6229/30
#define GPIO_DIR1_MT6228		((volatile uint16 *)(GPIO_base + 0x0000))
#define GPIO_DOUT1_MT6228		((volatile uint16 *)(GPIO_base + 0x00F0))
#define GPIO_MODE1_MT6228		((volatile uint16 *)(GPIO_base + 0x01A0))

// GPIO: MT6235, MT6238
#define GPIO_DIR1_MT6235		((volatile uint16 *)(GPIO_base + 0x0000))
#define GPIO_DOUT1_MT6235		((volatile uint16 *)(GPIO_base + 0x0800))
#define GPIO_MODE1_MT6235		((volatile uint16 *)(GPIO_base + 0x1000))


// GPO: MT6225, MT6226/27
#define GPO_DOUT1_MT6225		((volatile uint16 *)(GPIO_base + 0x0140))
#define GPO_MODE1_MT6225		((volatile uint16 *)(GPIO_base + 0x01C0))

// GPO: MT6228, MT6229/MT6230
#define GPO_DOUT1_MT6228		((volatile uint16 *)(GPIO_base + 0x0190))
#define GPO_MODE1_MT6228		((volatile uint16 *)(GPIO_base + 0x0240))

// GPIO: MT6255 : for GPIO_Init()
#define GPIO_DIR1_MT6255		((volatile uint16 *)(GPIO_base + 0x0000))
#define GPIO_DOUT1_MT6255		((volatile uint16 *)(GPIO_base + 0x0400))
#define GPIO_MODE1_MT6255		((volatile uint16 *)(GPIO_base + 0x0600))
#define GPIO_MODE4_MT6255		((volatile uint16 *)(GPIO_base + 0x0600))   // FIXME: g_PDN_Cfg_MT6255
#define GPIO_MODE7_MT6255		((volatile uint16 *)(GPIO_base + 0x0600))   // FIXME: g_PDN_Cfg_MT6255

//------------------------------------------------------------------------------
// PMIC Register                                                          
//------------------------------------------------------------------------------
#if	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__)
	#define	PMIC_COND		((volatile uint16 *)(0x83010834))
	#define PMIC_CONE		((volatile uint16 *)(0x83010838))
#endif	

//------------------------------------------------------------------------------
// RTC Register                                                          
//------------------------------------------------------------------------------
#if defined(__MTK_BB_REG_BASEADDRESS_GROUP11__)
    //#define RTC_base			    (0xC1003000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP12__)
    //#define RTC_base			    (0xC1003000)  //N
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP10__)
    //#define RTC_base			    (0xC1003000)
	//Heaton :;;mt6589 rtc base wrong!!;
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP9__)
    #define RTC_base			    (0xC1003000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP8__)
    #define RTC_base			    (0x70014000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP7__)
	#define RTC_base				(0x700C0000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP6__)
	#define RTC_base				(0x60170000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP5__)
	#define	RTC_base				(0x850C0000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__)
	#define	RTC_base				(0x810C0000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP4__)
	#define	RTC_base				(0x8002C000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define	RTC_base				(0xF002C000)
#else
	#define	RTC_base				(0x80210000)
#endif

#if !defined(BB_MT6589) && !defined(BB_MT8135)  //hi Heaton, just skip RTC operation for earlyporting
#define RTC_BBPU				((volatile uint16 *)(RTC_base + 0x0000))
#define RTC_POWERKEY1			((volatile uint16 *)(RTC_base + 0x0050))
#define RTC_POWERKEY2			((volatile uint16 *)(RTC_base + 0x0054))
// MT6238 || MT6235
#define	RTC_WRTGR				((volatile uint16 *)(RTC_base + 0x0074))

#if defined(__MTK_RTC_V3__) || defined(__MTK_RTC_V4__)
    #define RTC_PROT            ((volatile uint16 *)(RTC_base + 0x0068))
    #define RTC_MASK_BBPU_CBUSY 0x0040
#endif
#endif

//------------------------------------------------------------------------------
// GPT Register                                                          
//------------------------------------------------------------------------------
#if defined(__MTK_BB_REG_BASEADDRESS_GROUP11__)
    #define	GPT_base				(0x10008000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP12__)
    #define	GPT_base				(0x10008000)
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP10__)
    #define	GPT_base				(0x10008000) 
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP9__)
    #define	GPT_base				(0xC1002000) 
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP8__)
    #define	GPT_base				(0x70007000) 
#elif defined(__MTK_BB_REG_BASEADDRESS_GROUP6__)
	#define	GPT_base				(0x60060000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP5__)
	#define	GPT_base				(0x85060000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__)
	#define	GPT_base				(0x81060000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP4__)
	#define	GPT_base				(0x80026000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define	GPT_base				(0x81060000)
#else
	#define	GPT_base				(0x80100000)
#endif

#define GPT_TIMER3_CON				((volatile uint16 *)(GPT_base + 0x001C))
#define GPT_TIMER3_DATA				((volatile uint16 *)(GPT_base + 0x0020))
#define GPT_TIMER3_PRESCALE			((volatile uint16 *)(GPT_base + 0x0024))		// 0(16K), 1(8K), 2(4K), 3(2K), 4(1K)

//------------------------------------------------------------------------------
// TDMA Timer Register                                                          
//------------------------------------------------------------------------------

#if defined(__MTK_BB_REG_BASEADDRESS_GROUP6__)
	#define	TDMA_base				(0x61050000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP5__)
	#define	TDMA_base				(0x84060000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__)
	#define	TDMA_base				(0x82000000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP4__)
	#define	TDMA_base				(0xF11F0000)
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define	TDMA_base				(0x811F0000)
#else
	#define	TDMA_base				(0x80200000)
#endif

#define TDMA_RTC_32K				((volatile uint32 *)(TDMA_base + 0x0230))

//------------------------------------------------------------------------------
// DSP Shared Register                                                          
//------------------------------------------------------------------------------
/* DSP to CPU interrupt control (shared register) */
#if 	defined(__MTK_BB_REG_BASEADDRESS_GROUP5__)
	#define	DSP_base				(0xA0000000)
	#define	IDMA_base				(0xA1000000)	// Port 1: 0xA1000000, Port 2: 0xA2000000
	#define SHARE_base				(0x840A0000)	// Share1: 0x840A0000, Share2: 0x840B0000
#elif	defined(__MTK_BB_REG_BASEADDRESS_GROUP2__) || defined(__MTK_BB_REG_BASEADDRESS_GROUP3__)
	#define DSP_base				(0xA0000000)
	#define IDMA_base				(0xA2000000)	// Port 1: 0xA2000000, Port 2: 0xA3000000
	#define SHARE_base				(0x820A0000)	// Share1: 0x820A0000, Share2: 0x820C0000
#else
	#define DSP_base				(0x50000000)
	#define IDMA_base				(0x60310000L)
	#define SHARE_base				(0x80300000)
#endif

/* DSP Control Register                      */
#define DSP_VERSION				((volatile uint16 *)(DSP_base + 0x0014))
#define	SHARE_DSPCON			((volatile uint16 *)(SHARE_base + 0x0000))
#define	SHARE_DSPCTL			SHARE_DSPCON
#define IDMA_CM0				0x60000000L

/* for MT6205B */
#define DSP_VERSION_MT6205B		((volatile uint16 *)(DSP_base + 0x0000))
#define IDMA_base_MT6205B		(0x60300000L)

#define PATCH_base				(0x80310000)
#define PATCH_PAGE(n)			(PATCH_base + ((n) * 0x10))
#define PATCH_ADDR(n)			(PATCH_PAGE(n) + 0x04)
#define PATCH_INST_LOW(n)		(PATCH_PAGE(n) + 0x08)
#define PATCH_INST_HIGH(n)		(PATCH_PAGE(n) + 0x0c)
#define PATCH_EN				((volatile uint32 *)(PATCH_base + 0x0100))

//------------------------------------------------------------------------------

typedef struct {
	BBCHIP_TYPE		m_bbchip_type;
	uint32			m_hw_code;
	uint32			m_hw_ver;
} HW_BB_CHIP_ID_S;

typedef struct {
	uint32	m_baseaddr;
	volatile uint32 	*m_reg_emi_con;
} HW_BB_EMI_Bank_S;

typedef struct {
	HW_BB_EMI_Bank_S	m_bank[MAX_CS];
	bool				m_is_dram_supported;
	uint32              m_max_dram_size;
	uint32              m_dram_size_det_step;
	volatile uint32 	*m_reg_emi_gen_a;
	volatile uint32 	*m_reg_emi_gen_b;
	volatile uint32 	*m_reg_emi_gen_c;
	volatile uint32 	*m_reg_emi_gen_d;
	volatile uint32 	*m_reg_emi_con_i;
	volatile uint32 	*m_reg_emi_con_j;
	volatile uint32 	*m_reg_emi_con_k;
	volatile uint32 	*m_reg_emi_con_l;
	volatile uint32 	*m_reg_emi_con_m;
	volatile uint32 	*m_reg_emi_con_n;
	volatile uint32 	*m_reg_emi_del_a;
	volatile uint32 	*m_reg_emi_del_b;
	volatile uint32 	*m_reg_emi_del_c;
	volatile uint32 	*m_reg_emi_del_d;
	volatile uint32 	*m_reg_emi_del_e;
	volatile uint32 	*m_reg_emi_del_f;
	volatile uint32 	*m_reg_emi_del_g;
	volatile uint32 	*m_reg_emi_del_h;
	volatile uint32 	*m_reg_emi_del_i;
	volatile uint32 	*m_reg_emi_del_j;
	volatile uint32 	*m_reg_emi_arb_a;
	volatile uint32 	*m_reg_emi_arb_b;
	volatile uint32 	*m_reg_emi_arb_c;
	volatile uint32 	*m_reg_emi_arb_d;
	volatile uint32 	*m_reg_emi_arb_e;
	volatile uint32 	*m_reg_emi_arb_f;
	volatile uint32 	*m_reg_emi_arb_g;
	volatile uint32 	*m_reg_emi_arb_h;
	volatile uint32 	*m_reg_emi_arb_i;
	volatile uint32     *m_reg_emi_drv_a;
	volatile uint32     *m_reg_emi_drv_b;
	volatile uint32     *m_reg_emi_iocl;
	volatile uint32     *m_reg_emi_drct;
	volatile uint32     *m_reg_emi_ppct;
	volatile uint32     *m_reg_emi_slct;
	volatile uint32     *m_reg_emi_abct;
	volatile uint32     *m_reg_emi_dutb;
} HW_BB_EMI_Config_S;

typedef struct {
	volatile uint16 	*m_reg_highspeed_uart;
	volatile uint16 	*m_reg_sample_count;
	volatile uint16 	*m_reg_sample_point;
} HW_BB_UART_Config_S;

typedef struct {
	uint32	m_dummy;
} HW_BB_DMA_Config_S;

typedef struct {
	uint32	m_dummy;
} HW_BB_INT_Config_S;

typedef struct {
	uint16	m_timeout_shift_bits;
} HW_BB_RGU_Config_S;

typedef struct {
	volatile uint16 	*m_reg_gpio_mod_ctrl_4;
	uint32	m_nfi_mask;
	uint32	m_nfi_cs0;
	uint32	m_nfi_cs1;
} HW_BB_GPIO_Config_S;

typedef struct {
	volatile uint16 	*m_reg_power_down_clear_0;
	volatile uint16 	*m_reg_power_down_clear_1;
} HW_BB_PDN_Config_S;

typedef struct {
	bool	m_nfi_v2;
} HW_BB_NFI_Config_S;

struct _HW_Device_Config_S;
typedef struct _HW_Device_Config_S	HW_Device_Config_S;
typedef STATUS_E  (*CB_SETUP_PLL)(HW_Device_Config_S  *p_hw_dev_cfg);

typedef struct {
	volatile uint16 	*m_reg_mdpll;
	volatile uint16 	*m_reg_clk_con;
	volatile uint16 	*m_reg_mcuclk_con;
	CB_SETUP_PLL		m_cb_setup_pll;
} HW_BB_CLK_Config_S;

typedef struct {
	const HW_BB_CHIP_ID_S			m_ID;
	const HW_BB_EMI_Config_S		*m_EMI;
	const HW_BB_UART_Config_S		*m_UART;
	const HW_BB_DMA_Config_S		*m_DMA;
	const HW_BB_INT_Config_S		*m_INT;
	const HW_BB_RGU_Config_S		*m_RGU;
	const HW_BB_GPIO_Config_S		*m_GPIO;
	const HW_BB_PDN_Config_S		*m_PDN;
	const HW_BB_NFI_Config_S		*m_NFI;
	const HW_BB_CLK_Config_S		*m_CLK;
	const bool						m_SB_Support;
	const uint32					m_INT_SYSRAM_SIZE;
} HW_BB_Config_S;

typedef struct {
	// flash chip-select 
	HW_ChipSelect_E		m_chip_select[MAX_DIE_IN_MCP];
	// NOR flash base address 
	uint32				m_nor_baseaddr[MAX_DIE_IN_MCP];
	// NOR flash setting 
	uint32				m_nor_emi_con;
} HW_Storage_NOR_Config_S;

typedef struct {
	// flash chip-select 
	HW_ChipSelect_E		m_chip_select;
	// NAND flash setting 
	uint32				m_nand_acccon;
} HW_Storage_NAND_Config_S;

/* data structure used only by MT6573 combo mcp */
typedef struct {
    unsigned int m_nand_id;
    unsigned int m_emi_coni;
    unsigned int m_emi_drva;
    unsigned int m_emi_drvb;
    unsigned int m_emi_conj;
    unsigned int m_emi_conk;
    unsigned int m_emi_conl;
    unsigned int m_emi_iocl;
    unsigned int m_emi_gena;
    unsigned int m_emi_gend;
    unsigned int m_emi_drct;
    unsigned int m_emi_ppct;
    unsigned int m_emi_slct;
    unsigned int m_emi_abct;
    unsigned int m_emi_dutb;
} EMI_SETTINGS_v06;

/* data structure used only by MT6573 combo mcp */
typedef struct {
    unsigned int m_nand_id;
    unsigned int m_emi_coni;
    unsigned int m_emi_drva;
    unsigned int m_emi_drvb;
    unsigned int m_emi_conj;
    unsigned int m_emi_conk;
    unsigned int m_emi_conl;
    unsigned int m_emi_iocl;
    unsigned int m_emi_gena;
    unsigned int m_emi_gend;
    unsigned int m_emi_drct;
    unsigned int m_emi_ppct;
    unsigned int m_emi_slct;
    unsigned int m_emi_abct;
    unsigned int m_emi_dutb;
    unsigned int m_emi_conn;
} EMI_SETTINGS_v07;

#define MCP_MASK_DEV  0xFF00
#define MCP_MASK_RAM  0x00FF
#define MCP_NAND_DEV  0x0100
#define MCP_EMMC_DEV  0x0200
#define MCP_DDR1_RAM  0x0001
#define MCP_DDR2_RAM  0x0002
#define MCP_DDR3_RAM  0x0003
#define MCP_DISC_DDR1 MCP_DDR1_RAM
#define MCP_DISC_DDR2 MCP_DDR2_RAM
#define MCP_DISC_DDR3 MCP_DDR3_RAM
#define MCP_NAND_DDR1 (MCP_NAND_DEV | MCP_DDR1_RAM)
#define MCP_NAND_DDR2 (MCP_NAND_DEV | MCP_DDR2_RAM)
#define MCP_NAND_DDR3 (MCP_NAND_DEV | MCP_DDR3_RAM)
#define MCP_EMMC_DDR1 (MCP_EMMC_DEV | MCP_DDR1_RAM)
#define MCP_EMMC_DDR2 (MCP_EMMC_DEV | MCP_DDR2_RAM)
#define MCP_EMMC_DDR3 (MCP_EMMC_DEV | MCP_DDR3_RAM)
#define MCP_NAND_ID_LEN 4
#define MCP_EMMC_ID_LEN 9
typedef struct {
    unsigned int m_type;         /* 0x0000 : Invalid
                                    0x0001 : Discrete DDR1
                                    0x0002 : Discrete DDR2
                                    0x0003 : Discrete DDR3
                                    0x0101 : MCP(NAND+DDR1)
                                    0x0102 : MCP(NAND+DDR2)
                                    0x0103 : MCP(NAND+DDR3)
                                    0x0201 : MCP(eMMC+DDR1)
                                    0x0202 : MCP(eMMC+DDR2)
                                    0x0203 : MCP(eMMC+DDR3)
                                 */
    unsigned char m_id[12];
    int     m_emi_cona;             //@0x3000
    int     m_dramc_drvctl0;         //@0x40B8               -> customized TX I/O driving
    int     m_dramc_drvctl1;         //@0x40BC               -> customized TX I/O driving
    int     m_dramc_dle;            //40E4[4]:407c[6:4]     -> customized
    int     m_dramc_actim;          //@0x4000
    int     m_dramc_gddr3ctl;       //@0x40F4
    int     m_dramc_conf1;          //@0x4004
    int     m_dramc_ddr2ctl;        //@0x407C 
    int     m_dramc_test2_3;        //@0x4044
    int     m_dramc_conf2;          //@0x4008
    int     m_dramc_pd_ctrl;        //@0x41DC
    int     m_dramc_padctl3;        //@0x4014               -> customized TX DQS delay
    int     m_dramc_dqodly;         //@0x4200~0x420C        -> customized TX DQ delay
    int     m_dramc_addr_output_dly;// for E1 DDR2 only
    int     m_dramc_clk_output_dly; // for E1 DDR2 only
    int     m_reserved[10];

    union
    {
        struct _DDR1 
        {
            unsigned int m_mode;
            unsigned int m_ext_mode;    // dram driving strength -> customized
        } ddr1;
        struct _DDR2
        {
            unsigned int m_mode1;
            unsigned int m_mode2;
            unsigned int m_mode3;       // dram driving strength  -> customized
            unsigned int m_mode10;
            unsigned int m_mode63;
        } ddr2;
        struct _DDR3
        {
            unsigned int m_mode0;
            unsigned int m_mode1;
            unsigned int m_mode2;
            unsigned int m_mode3;            
        } ddr3;
    }u;    
} EMI_SETTINGS_v08;

/*For MT6572*/
typedef struct EMI_SETTINGS_v11
{
    int  type;                /* 0x0000 : Invalid
                                 0x0001 : Discrete DDR1
                                 0x0002 : Discrete DDR2
                                 0x0003 : Discrete DDR3
                                 0x0101 : MCP(NAND+LPDDR1)
                                 0x0102 : MCP(NAND+LPDDR2)
                                 0x0103 : MCP(NAND+LPDDR3)
                                 0x0201 : MCP(eMMC+LPDDR1)
                                 0x0202 : MCP(eMMC+LPDDR2)
                                 0x0203 : MCP(eMMC+LPDDR3)
                              */
    char  ID[16];
    int   id_length;              // EMMC and NAND ID/FW ID checking length
    int   EMI_Freq;              //200 / 266 /333 Mhz
    int EMI_DRVA_value;
    int EMI_DRVB_value;

    int EMI_ODLA_value;
    int EMI_ODLB_value;
    int EMI_ODLC_value;
    int EMI_ODLD_value;
    int EMI_ODLE_value;
    int EMI_ODLF_value;
    int EMI_ODLG_value;
    int EMI_ODLH_value;
    int EMI_ODLI_value;
    int EMI_ODLJ_value;
    int EMI_ODLK_value;
    int EMI_ODLL_value;
    int EMI_ODLM_value;
    int EMI_ODLN_value;

    int EMI_CONI_value;
    int EMI_CONJ_value;
    int EMI_CONK_value;
    int EMI_CONL_value;
    int EMI_CONN_value;

    int EMI_DUTA_value;
    int EMI_DUTB_value;
    int EMI_DUTC_value;

    int EMI_DUCA_value;
    int EMI_DUCB_value;
    int EMI_DUCE_value;

    int EMI_IOCL_value;

    int EMI_GEND_value;

    int   DRAM_RANK_SIZE[4];
    int   reserved[2];
} EMI_SETTINGS_v11;

typedef struct
{
    int   sub_version;            // sub_version: 0x1 for new version
    int  type;                /* 0x0000 : Invalid
                                 0x0001 : Discrete DDR1
                                 0x0002 : Discrete LPDDR2
                                 0x0003 : Discrete LPDDR3
                                 0x0004 : Discrete PCDDR3
                                 0x0101 : MCP(NAND+DDR1)
                                 0x0102 : MCP(NAND+LPDDR2)
                                 0x0103 : MCP(NAND+LPDDR3)
                                 0x0104 : MCP(NAND+PCDDR3)
                                 0x0201 : MCP(eMMC+DDR1)
                                 0x0202 : MCP(eMMC+LPDDR2)
                                 0x0203 : MCP(eMMC+LPDDR3)
                                 0x0204 : MCP(eMMC+PCDDR3)
                              */
    int   id_length;              // EMMC and NAND ID checking length
    int   fw_id_length;              // FW ID checking length
    char  ID[16];
    char  fw_id[8];               // To save fw id
    int   EMI_CONA_VAL;           //@0x3000
    int   DRAMC_DRVCTL0_VAL;      //@0x40B8               -> customized TX I/O driving
    int   DRAMC_DRVCTL1_VAL;      //@0x40BC               -> customized TX I/O driving
    int   DRAMC_ACTIM_VAL;        //@0x4000
    int   DRAMC_GDDR3CTL1_VAL;    //@0x40F4
    int   DRAMC_CONF1_VAL;        //@0x4004
    int   DRAMC_DDR2CTL_VAL;      //@0x407C 
    int   DRAMC_TEST2_3_VAL;      //@0x4044
    int   DRAMC_CONF2_VAL;        //@0x4008
    int   DRAMC_PD_CTRL_VAL;      //@0x41DC
    int   DRAMC_PADCTL3_VAL;      //@0x4014               -> customized TX DQS delay
    int   DRAMC_DQODLY_VAL;       //@0x4200~0x420C        -> customized TX DQ delay
    int   DRAMC_ADDR_OUTPUT_DLY;  // for E1 DDR2 only
    int   DRAMC_CLK_OUTPUT_DLY;   // for E1 DDR2 only
    int   DRAMC_ACTIM1_VAL;       //@0x41E8
    int   DRAMC_MISCTL0_VAL;      //@0x40FC
    int   DRAMC_ACTIM05T_VAL;     //@0x41F8
    int   DRAM_RANK_SIZE[4];
    int   reserved[10];

    union
    {
        struct
        {
            int   LPDDR2_MODE_REG_1;
            int   LPDDR2_MODE_REG_2;
            int   LPDDR2_MODE_REG_3;
            int   LPDDR2_MODE_REG_5;
            int   LPDDR2_MODE_REG_10;
            int   LPDDR2_MODE_REG_63;
        }lpddr2;
        struct
        {
            int   DDR1_MODE_REG;
            int   DDR1_EXT_MODE_REG;
        }ddr1;
        struct
        {
            int   PCDDR3_MODE_REG0;
            int   PCDDR3_MODE_REG1;
            int   PCDDR3_MODE_REG2;
            int   PCDDR3_MODE_REG3;
        }pcddr3;
        struct
        {
            int   LPDDR3_MODE_REG_1;
            int   LPDDR3_MODE_REG_2;
            int   LPDDR3_MODE_REG_3;
            int   LPDDR3_MODE_REG_5;
            int   LPDDR3_MODE_REG_10;
            int   LPDDR3_MODE_REG_63;
        }lpddr3;
    }ddr_mode;
} EMI_SETTINGS_v12;
typedef struct {
    int     m_bloader_sub_version;
    unsigned int m_type;         /* 0x0000 : Invalid
                                    0x0001 : Discrete DDR1
                                    0x0002 : Discrete DDR2
                                    0x0003 : Discrete DDR3
                                    0x0101 : MCP(NAND+DDR1)
                                    0x0102 : MCP(NAND+DDR2)
                                    0x0103 : MCP(NAND+DDR3)
                                    0x0201 : MCP(eMMC+DDR1)
                                    0x0202 : MCP(eMMC+DDR2)
                                    0x0203 : MCP(eMMC+DDR3)
                                 */
    int     m_id_length;
    int     m_fw_id_length;         //related size of m_emmc_fw_version
    unsigned char m_id[16];
    char    m_emmc_fw_version[8];   //For EMMC FW Version, Sandisk 6 byte, Others 1 byte
    int     m_emi_cona;             //@0x3000
    int     m_dramc_drvctl0;        //@0x40B8               -> customized TX I/O driving
    int     m_dramc_drvctl1;        //@0x40BC               -> customized TX I/O driving
    int     m_dramc_dle;            //40E4[4]:407c[6:4]     -> customized
    int     m_dramc_actim;          //@0x4000
    int     m_dramc_gddr3ctl;       //@0x40F4
    int     m_dramc_conf1;          //@0x4004
    int     m_dramc_ddr2ctl;        //@0x407C
    int     m_dramc_test2_3;        //@0x4044
    int     m_dramc_conf2;          //@0x4008
    int     m_dramc_pd_ctrl;        //@0x41DC
    int     m_dramc_padctl3;        //@0x4014               -> customized TX DQS delay
    int     m_dramc_dqodly;         //@0x4200~0x420C        -> customized TX DQ delay
    int     m_dramc_addr_output_dly;// for E1 DDR2 only
    int     m_dramc_clk_output_dly; // for E1 DDR2 only
    int     m_dramc_actim1;         //@0x41E8
    int     m_dramc_misctl0;        //@0x40FC
    int     DRAM_RANK_SIZE[4];
    int     MMD;                              //MMD info, for 6589 just has two types MMD1 and MMD2.
    int     m_reserved[8];

    union
    {
        struct
        {
            unsigned int m_mode;
            unsigned int m_ext_mode;    // dram driving strength -> customized
        } ddr1;
        struct
        {
            unsigned int m_mode1;
            unsigned int m_mode2;
            unsigned int m_mode3;       // dram driving strength  -> customized
            unsigned int m_mode10;
            unsigned int m_mode63;
            unsigned int m_mode5;
        } ddr2;
        struct
        {
            unsigned int m_mode0;
            unsigned int m_mode1;
            unsigned int m_mode2;
            unsigned int m_mode3;
        } ddr3;
    }u;
} EMI_SETTINGS_v10;

#define DRAM_CFG_ENABLE_CALIBRATION 0x00000001
#define DRAM_CFG_ENABLE_DUMP        0x80000000
typedef struct {
	// external ram type 
	HW_RAMType_E		m_type;
	// external ram chip-select 
	HW_ChipSelect_E		m_chip_select;
	// external ram base address 
	uint32				m_baseaddr;
	// external ram size 
	uint32				m_size;
	// external sram setting 
	uint32				m_sram_emi_con;
	// external dram setting 
	uint32				m_emi_gen_a;
	uint32				m_emi_gen_b;
	uint32				m_emi_gen_c;
	uint32				m_emi_gen_d;
	uint32				m_emi_con_i;
	uint32				m_emi_con_i_ext;
	uint32				m_emi_con_j;
	uint32				m_emi_con_k;
	uint32				m_emi_con_l;
	uint32				m_emi_con_m;
	uint32				m_emi_con_n;
	uint32				m_emi_del_a;
	uint32				m_emi_del_b;
	uint32				m_emi_del_c;
	uint32				m_emi_del_d;
	uint32				m_emi_del_e;
	uint32				m_emi_del_f;
	uint32				m_emi_del_g;
	uint32				m_emi_del_h;
	uint32				m_emi_del_i;
	uint32				m_emi_del_j;
	uint32				m_emi_arb_a;
	uint32				m_emi_arb_b;
	uint32				m_emi_arb_c;
	uint32				m_emi_arb_d;
	uint32				m_emi_arb_e;
	uint32				m_emi_arb_f;
	uint32				m_emi_arb_g;
	uint32				m_emi_arb_h;
	uint32				m_emi_arb_i;
	uint32              m_emi_drv_a;
	uint32              m_emi_drv_b;
	uint32              m_emi_iocl;
	uint32              m_emi_drct;
	uint32              m_emi_ppct;
	uint32              m_emi_slct;
	uint32              m_emi_abct;
	uint32              m_emi_dutb;
    
    uint32              m_ver; 
    uint32              m_cfg;
    union
    {
        EMI_SETTINGS_v06 ev06;
        EMI_SETTINGS_v07 ev07;
        EMI_SETTINGS_v08 ev08;
        EMI_SETTINGS_v11 ev11;
        EMI_SETTINGS_v12 ev12;
        EMI_SETTINGS_v10 ev10; //Temp for 6582
    } m_emi;     
	// internal sram size 
	uint32				m_int_sram_size;
} HW_RAM_Config_S;

typedef struct {
	// external clock 
	EXT_CLOCK			m_ext_clock;
} HW_CLK_Config_S;

struct _HW_Device_Config_S {
	// storage config 
	HW_Storage_NOR_Config_S		m_nor_cfg;
	HW_Storage_NAND_Config_S	m_nand_cfg;
	// ram config 
	HW_RAM_Config_S				m_ram_cfg;
	// clock config 
	HW_CLK_Config_S				m_clk_cfg;
	// baseband config 
	const HW_BB_Config_S	*	m_hw_bbcfg;
};

typedef struct {
	// bbchip 
	BBCHIP_TYPE			m_bbchip;
	uint16				m_bbchip_hw_code;
	uint16				m_bbchip_hw_ver;
	uint16				m_bbchip_sw_ver;
	// NOR storagte report 
	STATUS_E			m_nor_ret;
	HW_ChipSelect_E		m_nor_chip_select[2];
	uint16				m_nor_flash_id;
	uint32				m_nor_flash_size;
	uint16				m_nor_flash_dev_code_1;
	uint16				m_nor_flash_dev_code_2;
	uint16				m_nor_flash_dev_code_3;
	uint16				m_nor_flash_dev_code_4;
	STATUS_E			m_nor_flash_otp_status;
	uint32				m_nor_flash_otp_size;
	// NAND storagte report 
	STATUS_E			m_nand_ret;
	HW_ChipSelect_E		m_nand_chip_select;
	uint16				m_nand_flash_id;
	uint64				m_nand_flash_size;
	uint16				m_nand_flash_dev_code_1;
	uint16				m_nand_flash_dev_code_2;
	uint16				m_nand_flash_dev_code_3;
	uint16				m_nand_flash_dev_code_4;
	uint16				m_nand_flash_dev_code_5;
	uint16				m_nand_flash_dev_code_6;
	uint16				m_nand_flash_dev_code_7;
	uint16				m_nand_pagesize;
	uint16				m_nand_sparesize;
	uint16				m_nand_pages_per_block;
	uint8				m_nand_io_interface;
	uint8				m_nand_addr_cycle;
    	uint8				m_nand_bmt_exist;
	uint8				m_nand_sub_device_code; // SLC, SPI, MLC
	// internal sram report 
	STATUS_E			m_int_sram_ret;
	uint32				m_int_sram_size;
	// external ram report 
	STATUS_E			m_ext_ram_ret;
	HW_RAMType_E		m_ext_ram_type;
	HW_ChipSelect_E		m_ext_ram_chip_select;
	uint64				m_ext_ram_size;
	// random id
	uint8				m_random_id[16];
	// eMMC storage report
	STATUS_E			m_emmc_ret;
	uint64				m_emmc_boot1_size;
	uint64				m_emmc_boot2_size;
	uint64				m_emmc_rpmb_size;
	uint64				m_emmc_gp1_size;
	uint64				m_emmc_gp2_size;
	uint64				m_emmc_gp3_size;
	uint64				m_emmc_gp4_size;
	uint64				m_emmc_ua_size;
	uint32				m_emmc_cid[4];
	uint8				m_emmc_fwver[8];
	
	// SD/MMC storage report
	STATUS_E			m_sdmmc_ret;
	uint64				m_sdmmc_ua_size;
	uint32				m_sdmmc_cid[4];
} HW_DetectionResult_S;

#ifdef __cplusplus
extern "C" {
#endif

extern HW_Device_Config_S		g_HW_DevCfg;
extern HW_DetectionResult_S  	g_HW_DetectionResult;
extern uint32 g_FreeIntSysRAMBeginAddr;
extern uint32 g_FreeIntTCMBeginAddr;
extern uint32 g_FreeExtRAMBeginAddr;
extern uint32 g_ExtRAMBeginAddr;

#define GetEMIConfig() (g_HW_DevCfg.m_hw_bbcfg->m_EMI)
#define GetRAMConfig() (g_HW_DevCfg.m_ram_cfg)

#define EMI_BANK_BASEADDR(p_hw_bbcfg, ecs)			((CS_WITH_DECODER==ecs)?CS_WITH_DECODER:p_hw_bbcfg->m_EMI->m_bank[ecs].m_baseaddr)
#define EMI_BANK_REG(p_hw_bbcfg, ecs)				(p_hw_bbcfg->m_EMI->m_bank[ecs].m_reg_emi_con)

extern int32		mem_overlap_copy(void *dest, void *src, uint32 len);
extern void			mem_set(void *dest, uint8 default_value, uint32 len);
	
extern BBCHIP_TYPE  DetectBasebandChip(void);
extern STATUS_E  	SetupEMI(HW_ChipSelect_E  cs, uint32  emi_setting);
extern STATUS_E  	DetectIntSRAM(uint32 *p_size);
extern STATUS_E  	DetectExtSRAM(HW_RAMType_E *p_type, HW_ChipSelect_E *p_chip_select, uint32 *p_baseaddr, uint32 *p_size);
extern STATUS_E  	DetectExtDRAM(HW_RAMType_E *p_type, HW_ChipSelect_E *p_chip_select, uint32 *p_baseaddr, uint32 *p_size);
extern void			SchematicSetup(HW_DetectionResult_S  *p_result);
	
#ifdef __cplusplus
}
#endif

#endif
