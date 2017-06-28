
#ifndef __PLATFORM_INTERRUPTS_H
#define __PLATFORM_INTERRUPTS_H

#include <sys/types.h>
#include <arch/arm.h>
void platform_init_interrupts(void);
void platform_deinit_interrupts(void);


enum handler_return platform_irq(struct arm_iframe *frame);


void platform_fiq(struct arm_iframe *frame);

#endif
