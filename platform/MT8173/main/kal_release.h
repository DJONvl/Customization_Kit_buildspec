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
*  20131116 MTK81353 Initial version.
*
*
*******************************************************************************/

#ifndef _KAL_RELEASE_H
#define _KAL_RELEASE_H

#include "sw_types.H"
#include "assert.h"


#define ASSERT(exp) \
{\
	if( !(exp) )\
		_assert(#exp, __FILE__, __LINE__);\
}\


/*
 *	KAL
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
typedef int				kal_int32;
typedef char			kal_char;

typedef void (* VOID_FUNC)(void);


typedef void* kal_taskid;

typedef struct {
    kal_uint8   unused;
} *kal_semid;

extern void kal_mem_set(void *dest,uint32 value,uint32 size);
extern void kal_mem_cpy(void * dest,void *src,uint32 size);

extern void kal_neon_mem_set(void *dest,uint32 value,uint32 size);
extern void kal_neon_mem_cpy(void * dest,void *src,uint32 size);

#endif //#define _KAL_RELEASE_H

