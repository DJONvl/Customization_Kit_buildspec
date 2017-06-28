/*
* Copyright (c) 2008 Travis Geiselbrecht
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files
* (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef __PLATFORM_TIMER_H
#define __PLATFORM_TIMER_H

#define GPT_IRQEN_REG		((volatile unsigned int*)(APXGPT_BASE))
#define GPT_IRQSTA_REG		((volatile unsigned int*)(APXGPT_BASE+0x04))
#define GPT_IRQACK_REG		((volatile unsigned int*)(APXGPT_BASE+0x08))

#define GPT5_CON_REG		((volatile unsigned int*)(APXGPT_BASE+0x50))
#define GPT5_CLK_REG		((volatile unsigned int*)(APXGPT_BASE+0x54))
#define GPT5_COUNT_REG		((volatile unsigned int*)(APXGPT_BASE+0x58))
#define GPT5_COMPARE_REG	((volatile unsigned int*)(APXGPT_BASE+0x5C))

#define GPT_MODE4_ONE_SHOT (0x00 << 4)
#define GPT_MODE4_REPEAT   (0x01 << 4)
#define GPT_MODE4_KEEP_GO  (0x02 << 4)
#define GPT_MODE4_FREERUN  (0x03 << 4)

#define GPT_CLEAR		2

#define GPT_ENABLE		1
#define GPT_DISABLE		0

#define GPT_CLK_SYS		(0x0 << 4)
#define GPT_CLK_RTC		(0x1 << 4)

#define GPT_DIV_BY_1		0
#define GPT_DIV_BY_2       	1


typedef enum handler_return (*platform_timer_callback)(void *arg, time_t now);
void lk_scheduler(void);
time_t current_time(void);

#endif

