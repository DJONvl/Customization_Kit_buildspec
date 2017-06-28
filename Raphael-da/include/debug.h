#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <compiler.h>
#include <stdarg.h>
#include <debug_impl.h>
#include "autoconfig.h"


/* debug levels */
#define VERBOSE 0
#define DEBUG 1
#define INFO 2
#define WARNING 3
#define CRITICAL 4
#define FATAL 5



int _dprintf(const char *fmt, ...) __PRINTFLIKE(1, 2);
int _dvprintf(const char *fmt, va_list ap);

#define dprintf(level, ...) //do { if ((level) >= get_log_level()) { _dprintf(__VA_ARGS__); } } while (0)


#define LOGF(...)   do { if ((FATAL) >= get_log_level()) { _dprintf(__VA_ARGS__); } } while (0)
#define LOGE(...)   do { if ((CRITICAL) >= get_log_level()) { _dprintf(__VA_ARGS__); } } while (0)
#define LOGI(...)   do { if ((INFO) >= get_log_level()) { _dprintf(__VA_ARGS__); } } while (0)
   
#if ( defined(STAGE_DA_INIT) && ENABLE_LOG_FOR_INIT_STAGE == 0 )
   #define LOGD(...)
   #define LOGV(...) 
#else
   #define LOGD(...)  do { if ((DEBUG) >= get_log_level()) { _dprintf(__VA_ARGS__); } } while (0)
   #define LOGV(...)   do { if ((VERBOSE) >= get_log_level()) { _dprintf(__VA_ARGS__); } } while (0)
#endif


#define print LOGD
/*Driver debug enable */

//#define PLL_DBG  /*enable during debug pll*/



void _panic(void *caller, const char *fmt, ...) __PRINTFLIKE(2, 3);
#define panic(...) _panic(__GET_CALLER(), __VA_ARGS__)

/* dump memory */
void hexdump(const void *ptr, size_t len);
void hexdump8(const void *ptr, size_t len);
/*external function*/

/* trace routines */
#define TRACE_ENTRY printf("%s: entry\n", __func__)
#define TRACE_EXIT printf("%s: exit\n", __func__)
#define TRACE_ENTRY_OBJ printf("%s: entry obj %p\n", __func__, this)
#define TRACE_EXIT_OBJ printf("%s: exit obj %p\n", __func__, this)
#define TRACE printf("%s:%d\n", __func__, __LINE__)
#define TRACEF(x...) do { printf("%s:%d: ", __func__, __LINE__); printf(x); } while (0)

/* trace routines that work if LOCAL_TRACE is set */
#define LTRACE_ENTRY do { if (LOCAL_TRACE) { TRACE_ENTRY; } } while (0)
#define LTRACE_EXIT do { if (LOCAL_TRACE) { TRACE_EXIT; } } while (0)
#define LTRACE do { if (LOCAL_TRACE) { TRACE; } } while (0)
#define LTRACEF(x...) do { if (LOCAL_TRACE) { TRACEF(x); } } while (0)


#endif
