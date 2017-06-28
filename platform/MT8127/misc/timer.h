#ifndef _TIMER_H_
#define _TIMER_H_

#include "SW_TYPES.H"

extern void mdelay(unsigned long msec);
extern void udelay (unsigned long usec);
extern void timer_init(void);
extern U64 get_timer(U64 base);

#endif
