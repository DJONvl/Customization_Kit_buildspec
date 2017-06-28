#ifndef POWER_INTERFACE_H
#define POWER_INTERFACE_H

#include "sys/types.h"

extern bool Is_Battery_Exist();

extern uint32 Get_Battery_Voltage();

extern void Latch_Power();

extern void Release_Power();

extern void Set_Reset_Key(uint32 config);

extern int Get_Power_Via_Usb();
extern void disable_Charger();
extern void Charger_VBUS_limit();
extern void Clear_Power();

#endif

