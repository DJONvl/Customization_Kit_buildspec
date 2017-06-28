#ifndef GPT_INTERFACE_H
#define GPT_INTERFACE_H

#include "sys/types.h"
extern void GPT_Timer_Init();
extern uint32 Get_Current_Time_us (void);
extern void Udelay (unsigned int usec);

extern uint32 GPT_Timer_Test();


#endif /* USB_INTERFACE_H */

