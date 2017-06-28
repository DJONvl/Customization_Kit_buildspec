#ifndef _KAL_RELEASE_H
#define _KAL_RELEASE_H

#include "SW_TYPES.H"
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


/*
 *	FDM+MTD
 */
typedef	unsigned char	BYTE;
typedef	unsigned short	WORD;
typedef	unsigned int	DWORD;
typedef unsigned int	UINT;
typedef unsigned short	WCHAR;
typedef	void * HANDLE;

typedef int   RTFHANDLE;
typedef DWORD RTFSector;
typedef DWORD RTFCluster;

//#define ONE_BYTE_ALIGN_ADS

#ifdef __cplusplus
extern "C" {
#endif

extern void kal_mem_set(void *dest,uint32 value,uint32 size);
extern void kal_mem_cpy(void * dest,void *src,uint32 size);

extern void kal_neon_mem_set(void *dest,uint32 value,uint32 size);
extern void kal_neon_mem_cpy(void * dest,void *src,uint32 size);

#ifdef __cplusplus
}
#endif


#endif //#define _KAL_RELEASE_H
