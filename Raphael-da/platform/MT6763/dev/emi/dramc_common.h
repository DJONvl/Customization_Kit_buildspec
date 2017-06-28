//
//  DRAMC_COMMON.H    
//

#ifndef _DRAMC_COMMON_H_
#define _DRAMC_COMMON_H_

#include "dramc_register.h"
#include "../gpt_timer/gpt_timer.h"
#include <debug.h>
#include "dramc_pi_api.h"
#include "emi.h"


/***********************************************************************/
/*                  Public Types                                       */
/***********************************************************************/
#if 0
#include <typedefs.h>
#else
typedef unsigned char   UCHAR;
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned int    UINT32;
typedef signed char     INT8;
typedef signed short    INT16;
typedef signed int      INT32;

typedef signed char     S8;
typedef signed short    S16;
typedef signed int      S32;
typedef unsigned char   U8;
typedef unsigned short  U16;
typedef unsigned int    U32;

#ifndef UINT32P
typedef volatile unsigned int* UINT32P; 
#endif //UINT32P
#endif

/*------------------------------------------------------------*/
/*                  macros, defines, typedefs, enums          */
/*------------------------------------------------------------*/
/************************** Common Macro *********************/
#if ((!defined(_WIN32)) && (FOR_DV_SIMULATION_USED==0) && (SW_CHANGE_FOR_SIMULATION==0))
#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")
#else
#define dsb()
#endif

// K2?? : The following needs to be porting.
// choose a proper mcDELAY
#if __ETT__
#if (FOR_DV_SIMULATION_USED==0)
#define mcDELAY_US(x)       GPT_Delay_us((U32) (x))
#define mcDELAY_MS(x)       GPT_Delay_ms((U32) (x))
#else
#define mcDELAY_US(x)       GPT_Delay_us((U32) (1))
#define mcDELAY_MS(x)       GPT_Delay_ms((U32) (1))
#endif
#else
//====
  #if __FLASH_TOOL_DA__
    #define mcDELAY_US(x)       gpt4_busy_wait_us(x)
    #define mcDELAY_MS(x)       gpt4_busy_wait_us(x*1000)
  #else
        #if (FOR_DV_SIMULATION_USED==1)  
            #define mcDELAY_US(x)       delay_us(1)
            #define mcDELAY_MS(x)       delay_us(1)
        #elif (SW_CHANGE_FOR_SIMULATION==1)
            #define mcDELAY_US(x)       *MDM_TM_WAIT_US = x; while (*MDM_TM_WAIT_US>0);
            #define mcDELAY_MS(x)       *MDM_TM_WAIT_US = x; while (*MDM_TM_WAIT_US>0);
        #else
            #define mcDELAY_US(x)       gpt_busy_wait_us(x)
            #define mcDELAY_MS(x)       gpt_busy_wait_us(x*1000)
        #endif
#endif
#endif

/**********************************************/
/* Priority of debug log                      */
/*--------------------------------------------*/
/* mcSHOW_DBG_MSG: High                       */
/* mcSHOW_DBG_MSG2: Medium High               */
/* mcSHOW_DBG_MSG3: Medium Low                */ 
/* mcSHOW_DBG_MSG4: Low                       */
/**********************************************/
#if __FLASH_TOOL_DA__
//  #define printf print
//  #define print DBG_MSG
#endif

#if __ETT__
    #if defined(DDR_INIT_TIME_PROFILING)   
    #define mcSHOW_DBG_MSG(_x_) 
    #define mcSHOW_DBG_MSG2(_x_)   
    #define mcSHOW_DBG_MSG3(_x_) 
    #define mcSHOW_DBG_MSG4(_x_) 
    #define mcSHOW_DBG_MSG5(_x_) 
    #define mcSHOW_USER_MSG(_x_)   
    #define mcSHOW_ERR_MSG(_x_)  opt_print _x_
    #elif defined(RELEASE)
    #define mcSHOW_DBG_MSG(_x_)  //opt_print _x_
    #define mcSHOW_DBG_MSG2(_x_)   
    #define mcSHOW_DBG_MSG3(_x_) 
    #define mcSHOW_DBG_MSG4(_x_) opt_print _x_  //mcSHOW_DBG_MSG4(_x_) is for BU parsing tool
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_USER_MSG(_x_)  //must_print _x_
    #define mcSHOW_ERR_MSG(_x_)   //must_print _x_  
    #elif VENDER_JV_LOG
    #define mcSHOW_DBG_MSG(_x_)  
    #define mcSHOW_DBG_MSG2(_x_)   
    #define mcSHOW_DBG_MSG3(_x_) 
    #define mcSHOW_DBG_MSG4(_x_) 
    #define mcSHOW_DBG_MSG5(_x_)    opt_print _x_   //mcSHOW_DBG_MSG5(_x_) is for vendor JV
    #define mcSHOW_USER_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)
    #elif FOR_DV_SIMULATION_USED
    #define mcSHOW_DBG_MSG(_x_)   printf _x_    
    #define mcSHOW_DBG_MSG2(_x_)  printf _x_    
    #define mcSHOW_DBG_MSG3(_x_)  printf _x_    
    //#define mcSHOW_DBG_MSG4(_x_)  if (RXPERBIT_LOG_PRINT) {printf _x_;}
    #define mcSHOW_DBG_MSG4(_x_)  printf _x_
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_USER_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)   printf _x_
    #elif SW_CHANGE_FOR_SIMULATION
    #define mcSHOW_DBG_MSG(_x_)     
    #define mcSHOW_DBG_MSG2(_x_)    
    #define mcSHOW_DBG_MSG3(_x_)    
    #define mcSHOW_DBG_MSG4(_x_)   
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_USER_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)  
    #else   // ETT real chip
    #define mcSHOW_DBG_MSG(_x_)   {mcDELAY_US(10);opt_print _x_;}
    #define mcSHOW_DBG_MSG2(_x_)  {mcDELAY_US(10);opt_print _x_;}
    #define mcSHOW_DBG_MSG3(_x_)  {mcDELAY_US(10);opt_print _x_;}
    #define mcSHOW_DBG_MSG4(_x_)  {mcDELAY_US(10);opt_print _x_;}
    #define mcSHOW_DBG_MSG5(_x_)  {mcDELAY_US(10);opt_print _x_;}
    #define mcSHOW_USER_MSG(_x_)  {mcDELAY_US(10);opt_print _x_;}
    #define mcSHOW_ERR_MSG(_x_)   opt_print _x_
    #endif    
    
#else  // preloader
    #if defined(TARGET_BUILD_VARIANT_ENG) && !defined(MTK_EFUSE_WRITER_SUPPORT) && !CFG_TEE_SUPPORT && !MTK_EMMC_SUPPORT
    #define mcSHOW_DBG_MSG(_x_)   print _x_    
    #define mcSHOW_DBG_MSG2(_x_)  print _x_    
    #define mcSHOW_DBG_MSG3(_x_)      
    #define mcSHOW_DBG_MSG4(_x_)  
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_USER_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)   print _x_
    #elif FOR_DV_SIMULATION_USED
    #define mcSHOW_DBG_MSG(_x_)   printf _x_    
    #define mcSHOW_DBG_MSG2(_x_)  printf _x_    
    #define mcSHOW_DBG_MSG3(_x_)  printf _x_    
    #define mcSHOW_DBG_MSG4(_x_)  printf _x_
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_USER_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)   printf _x_
    #else 
    #define mcSHOW_DBG_MSG(_x_)    // print _x_  
    #define mcSHOW_DBG_MSG2(_x_)    //print _x_  
    #define mcSHOW_DBG_MSG3(_x_)     
    #define mcSHOW_DBG_MSG4(_x_)    
    #define mcSHOW_DBG_MSG5(_x_)
    #define mcSHOW_USER_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)     
    #endif
#endif

#define mcFPRINTF(_x_)           
//#define mcFPRINTF(_x_)          fprintf _x_;

#define USE_PMIC_CHIP_MT6356	1

extern int dump_log;

extern void Reg_Sync_Writel(uint32 addr, uint32 val);
extern uint32 Reg_Readl( uint32 addr);
extern uint32 pmic_config_interface (uint32 RegNum, uint32 val, uint32 MASK, uint32 SHIFT);
extern uint32 upmu_get_reg_value(kal_uint32 reg);

#endif   // _DRAMC_COMMON_H_
