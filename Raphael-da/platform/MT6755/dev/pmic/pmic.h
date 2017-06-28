
#ifndef _PL_MT_PMIC_H_
#define _PL_MT_PMIC_H_

//==============================================================================
// The CHIP INFO
//==============================================================================
//MT6351
#define PMIC6351_E1_CID_CODE    0x5110
#define PMIC6351_E2_CID_CODE    0x5120
#define PMIC6351_E3_CID_CODE    0x5130
#define PMIC6351_E4_CID_CODE    0x5140
#define PMIC6351_E5_CID_CODE    0x5150
#define PMIC6351_E6_CID_CODE    0x5160
//MT6353
#define PMIC6353_E1_CID_CODE    0x5110
#define PMIC6353_E2_CID_CODE    0x5120
#define PMIC6353_E3_CID_CODE    0x5130
#define PMIC6353_E4_CID_CODE    0x5140
#define PMIC6353_E5_CID_CODE    0x5150
#define PMIC6353_E6_CID_CODE    0x5160



//==============================================================================
// PMIC define
//==============================================================================
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
// PMIC define
//==============================================================================
typedef enum {
	VPROC,
	VCORE,
	VCORE2,
/*	VPA,	*/
	VS1,
} BUCK_TYPE;

typedef enum {
	PMIC_VOLT_00_700000_V,
	PMIC_VOLT_00_725000_V,
	PMIC_VOLT_00_750000_V,
	PMIC_VOLT_00_775000_V,
	PMIC_VOLT_00_800000_V,
	PMIC_VOLT_00_825000_V,
	PMIC_VOLT_00_850000_V,
	PMIC_VOLT_00_875000_V,
	PMIC_VOLT_00_900000_V,
	PMIC_VOLT_00_925000_V,
	PMIC_VOLT_00_950000_V,
	PMIC_VOLT_00_975000_V,
	PMIC_VOLT_01_000000_V,
	PMIC_VOLT_01_025000_V,
	PMIC_VOLT_01_050000_V,
	PMIC_VOLT_01_075000_V,
	PMIC_VOLT_01_100000_V,
	PMIC_VOLT_01_125000_V,
	PMIC_VOLT_01_150000_V,
	PMIC_VOLT_01_175000_V,
	PMIC_VOLT_01_200000_V,
	PMIC_VOLT_01_225000_V,
	PMIC_VOLT_01_250000_V,
	PMIC_VOLT_01_275000_V,
	PMIC_VOLT_01_300000_V,
	PMIC_VOLT_01_325000_V,
	PMIC_VOLT_01_350000_V,
	PMIC_VOLT_01_375000_V,
	PMIC_VOLT_01_400000_V,
} BUCK_VOLTAGE_UV;

static const int pmic_buck_voltages[] = {
	700000,
	725000,
	750000,
	775000,
	800000,
	825000,
	850000,
	875000,
	900000,
	925000,
	950000,
	975000,
	1000000,
	1025000,
	1050000,
	1075000,
	1100000,
	1125000,
	1150000,
	1175000,
	1200000,
	1225000,
	1250000,
	1275000,
	1300000,
	1325000,
	1350000,
	1375000,
	1400000,
};
//==============================================================================
// PMIC Register Index
//==============================================================================
//register number
//#include "upmu_hw.h"
#include <sys/types.h>
#include "../gpt_timer/gpt_timer.h"


//==============================================================================
// PMIC Status Code
//==============================================================================
#define PMIC_TEST_PASS               0x0000
#define PMIC_TEST_FAIL               0xB001
#define PMIC_EXCEED_I2C_FIFO_LENGTH  0xB002
#define PMIC_CHRDET_EXIST            0xB003
#define PMIC_CHRDET_NOT_EXIST        0xB004
#define PMIC_VBAT_DROP		0xB005
#define PMIC_VBAT_NOT_DROP	0xB006
//==============================================================================
// PMIC Exported Function
//==============================================================================
extern CHARGER_TYPE mt_charger_type_detection(void);
//extern int pmic_detect_powerkey(void);
//extern int pmic_detect_homekey(void);
extern void hw_set_cc(int cc_val);
extern void pl_charging(int en_chr);
extern void pl_kick_chr_wdt(void);
extern void pl_close_pre_chr_led(void);
extern void pl_hw_ulc_det(void);
extern uint32 pmic_init (void);
//extern void mt6351_upmu_set_baton_tdet_en(uint32 val);
//extern void mt6351_upmu_set_rg_baton_en(uint32 val);
//extern uint32 mt6351_upmu_get_rgs_baton_undet(void);
//==============================================================================
// PMIC-Charger Type Detection
//==============================================================================
//extern int pmic_IsVbatDrop(void);
extern CHARGER_TYPE mt_charger_type_detection(void);
extern int hw_check_battery(void);
extern void pl_check_bat_protect_status();
extern uint32 pmic_config_interface (uint32 RegNum, uint32 val, uint32 MASK, uint32 SHIFT);
extern uint32 pmic_read_interface (uint32 RegNum, uint32 *val, uint32 MASK, uint32 SHIFT);

extern uint32 upmu_get_reg_value(kal_uint32 reg);
extern void upmu_set_reg_value(kal_uint32 reg, kal_uint32 reg_val);
extern bool hw_check_battery_mt6353(void);
extern uint32 get_battery_value (kal_uint8 dwChannel, int deCount, int trimd);



#endif
