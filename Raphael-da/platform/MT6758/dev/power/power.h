#ifndef __MT_POWER_H__
#define __MT_POWER_H__

#include <inc/register_base.h>
#include <sys/types.h>

typedef struct {
   unsigned short cmd;
   unsigned short mask;
   unsigned short dat;
} PMIC_SETTING_MT6351;

extern void PMIC_HW_Reset_Enable(uint32 hw_reset_configure);
extern bool IsBatteryExist(void);
extern PMIC_SETTING_MT6351 MT6351_pmic_setting[1];

#endif 
