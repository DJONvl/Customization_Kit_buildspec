/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  History:
*  YYYYMMDD MTK12345 Description
*  --------------------
*  20131116 MTK81353 Delete unused code.
*
*
*******************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_

#include "sw_types.h"

extern void mdelay(unsigned long msec);
extern void udelay (unsigned long usec);
extern void timer_init(void);
extern U64 get_timer(U64 base);

#endif
