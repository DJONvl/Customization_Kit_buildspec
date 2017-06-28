#ifndef __ASSERT_H
#define __ASSERT_H

#include <compiler.h>
#include <debug.h>

#define ASSERT(x) \
   do { if (unlikely(!(x))) { panic("ASSERT FAILED at (%s:%d): %s\n", __FILE__, __LINE__, #x); } } while (0)
#define assert(x) ASSERT(x)

#define EXT_ASSERT(_data, _data1, _data2, _data3)    \
{\
   if(!(_data))\
   while(1);\
}


#if DEBUGLEVEL > INFO
#define DEBUG_ASSERT(x) \
   do { if (unlikely(!(x))) { panic("DEBUG ASSERT FAILED at (%s:%d): %s\n", __FILE__, __LINE__, #x); } } while (0)
#else
#define DEBUG_ASSERT(x) \
   do { } while(0)
#endif

#endif

