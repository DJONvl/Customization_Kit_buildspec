#ifndef WDT_INTERFACE_H
#define WDT_INTERFACE_H

#include "sys/types.h"

extern void Enable_WDT(uint16 interval_ms,uint32 flags) ;
extern void Disable_WDT(void);

#endif

