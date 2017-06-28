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


#define mcDELAY_US(x)       gpt4_busy_wait_us(x)
#define mcDELAY_MS(x)       gpt4_busy_wait_us(x*1000)


/**********************************************/
/* Priority of debug log                      */
/*--------------------------------------------*/
/* mcSHOW_DBG_MSG: High                       */
/* mcSHOW_DBG_MSG2: Medium High               */
/* mcSHOW_DBG_MSG3: Medium Low                */ 
/* mcSHOW_DBG_MSG4: Low                       */
/**********************************************/
#if __FLASH_TOOL_DA__
//  #define printf LOGD 
//  #define print LOGD 
#endif
#define print LOGD

#define mcSHOW_DBG_MSG(_x_)   print _x_     
#define mcSHOW_DBG_MSG2(_x_)  print _x_     
#define mcSHOW_DBG_MSG3(_x_)  print _x_ 
#define mcSHOW_DBG_MSG4(_x_)  print _x_
#define mcSHOW_USER_MSG(_x_)  print _x_
#define mcSHOW_ERR_MSG(_x_)   print _x_	


#define mcFPRINTF(_x_)           

extern int dump_log;
#endif   // _DRAMC_COMMON_H_
