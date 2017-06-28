#ifndef WDT_INTERFACE_H
#define WDT_INTERFACE_H

#include "sys/types.h"


/******************************************************************************
* GPIO Driver interface
******************************************************************************/
/*direction*/
s32 mt_set_gpio_dir(u32 pin, u32 dir);
s32 mt_get_gpio_dir(u32 pin);

/*pull enable*/
s32 mt_set_gpio_pull_enable(u32 pin, u32 enable);
s32 mt_get_gpio_pull_enable(u32 pin);
/*pull select*/
s32 mt_set_gpio_pull_select(u32 pin, u32 select);
s32 mt_get_gpio_pull_select(u32 pin);

/*schmitt trigger*/
s32 mt_set_gpio_smt(u32 pin, u32 enable);
s32 mt_get_gpio_smt(u32 pin);

/*IES*/
s32 mt_set_gpio_ies(u32 pin, u32 enable);
s32 mt_get_gpio_ies(u32 pin);

/*input/output*/
s32 mt_set_gpio_out(u32 pin, u32 output);
s32 mt_get_gpio_out(u32 pin);
s32 mt_get_gpio_in(u32 pin);

/*mode control*/
s32 mt_set_gpio_mode(u32 pin, u32 mode);
s32 mt_get_gpio_mode(u32 pin);

void mt_gpio_init(void);
void mt_gpio_set_default(void);

#endif

