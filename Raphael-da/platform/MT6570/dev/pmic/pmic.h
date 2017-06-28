
#ifndef _PL_MT_PMIC_H_
#define _PL_MT_PMIC_H_

#include <sys/types.h>
//==============================================================================
// PMIC Define
//==============================================================================

#define PMIC6350_E1_CID_CODE    0x1050
#define PMIC6350_E2_CID_CODE    0x2050
#define PMIC6350_E3_CID_CODE    0x3050

#define VBAT_CHANNEL_NUMBER      7
#define ISENSE_CHANNEL_NUMBER	 6
#define VCHARGER_CHANNEL_NUMBER  4
#define VBATTEMP_CHANNEL_NUMBER  5

//==============================================================================
// PMIC-AUXADC related define
//==============================================================================
#define VOLTAGE_FULL_RANGE     	1800
#define ADC_PRECISE		32768 	// 15 bits
#define TIME_OUT_COUNT          1000


typedef enum {
    CHARGER_UNKNOWN = 0,
    STANDARD_HOST,          // USB : 450mA
    CHARGING_HOST,
    NONSTANDARD_CHARGER,    // AC : 450mA~1A 
    STANDARD_CHARGER,       // AC : ~1A
    APPLE_2_1A_CHARGER,     // 2.1A apple charger
    APPLE_1_0A_CHARGER,     // 1A apple charger
    APPLE_0_5A_CHARGER,     // 0.5A apple charger
} CHARGER_TYPE;


//==============================================================================
// PMIC Exported Function
//==============================================================================
extern uint32 PMIC_IMM_GetOneChannelValue(int dwChannel, int deCount, int trimd);


//==============================================================================
// PMIC Status Code
//==============================================================================
#define PMIC_TEST_PASS               0x0000
#define PMIC_TEST_FAIL               0xB001
#define PMIC_EXCEED_I2C_FIFO_LENGTH  0xB002
#define PMIC_CHRDET_EXIST            0xB003
#define PMIC_CHRDET_NOT_EXIST        0xB004
#define PMIC_VBAT_DROP				 0xB005
#define PMIC_VBAT_NOT_DROP			 0xB006

//==============================================================================
// PMIC Register Index
//==============================================================================
#include "upmu_hw.h"
extern int hw_check_battery(void);

U32 pmic_is_chr_det(void);


#endif

