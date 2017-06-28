/*
* Copyright (c) 2008-2009 Travis Geiselbrecht
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
#ifndef __SYS_TYPES_H
#define __SYS_TYPES_H

#ifndef __cplusplus
#define false 0
#define true 1
#define FALSE 0
#define TRUE 1
#define bool int 
#endif

#include <stdint.h>
#include "type_define.h"

#ifndef _SIZE_T_DEFINED_
typedef unsigned int size_t;
#endif
typedef long          ssize_t;
typedef long long     off_t;

typedef unsigned int uint;

#ifndef NULL
#define NULL 0
#endif
#define INT_MAX 0x7FFFFFFF

//typedef int status_t;

typedef uintptr_t addr_t;
typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

typedef unsigned long time_t;
typedef unsigned long long bigtime_t;
#define INFINITE_TIME (0xFFFFFFFFUL)

#define TIME_GTE(a, b) ((long)((a) - (b)) >= 0)
#define TIME_LTE(a, b) ((long)((a) - (b)) <= 0)
#define TIME_GT(a, b) ((long)((a) - (b)) > 0)
#define TIME_LT(a, b) ((long)((a) - (b)) < 0)


/*
*	KAL for usb
*/
#define	KAL_FALSE	FALSE
#define	KAL_TRUE	TRUE
typedef bool			kal_bool;
typedef unsigned char	kal_uint8;
typedef unsigned short	kal_uint16;
typedef unsigned int	kal_uint32;
typedef unsigned long long	kal_uint64;
typedef char			kal_int8;
typedef short			kal_int16;
typedef int			kal_int32;
typedef char			kal_char;


/*
* For mmc
*/

#ifndef s8
typedef signed char         s8;
#endif
#ifndef u8
typedef unsigned char       u8;
#endif

#ifndef s16
typedef signed short        s16;
#endif
#ifndef u16
typedef unsigned short      u16;
#endif

#ifndef s32
typedef signed int          s32;
#endif
#ifndef u32
typedef unsigned int        u32;
#endif

#ifndef s64
typedef signed long long    s64;
#endif
#ifndef u64
typedef unsigned long long  u64;
#endif

#ifndef S8
typedef signed char         S8;
#endif
#ifndef U8
typedef unsigned char       U8;
#endif

#ifndef S16
typedef signed short        S16;
#endif
#ifndef U16
typedef unsigned short      U16;
#endif

#ifndef S32
typedef signed int          S32;
#endif
#ifndef U32
typedef unsigned int        U32;
#endif

#ifndef S64
typedef signed long long   S64;
#endif
#ifndef U64
typedef unsigned long long U64;
#endif


enum handler_return {
   INT_NO_RESCHEDULE = 0,
   INT_RESCHEDULE,
};

#endif
