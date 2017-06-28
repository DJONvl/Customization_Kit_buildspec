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
extern int ForceDownloadViaUsb(void);

extern PMIC_SETTING_MT6351 MT6351_pmic_setting[1];
extern uint32 Get_Battery_Voltage_common();
extern void  charger_chg_en(kal_uint32 val);
extern void  charger_set_iinlim(kal_uint32 val);
extern void charger_set_vreg(kal_uint32 val);
extern void charger_set_wd_timer(kal_uint32 val);


#endif 
