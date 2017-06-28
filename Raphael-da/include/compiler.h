#ifndef __COMPILER_H
#define __COMPILER_H

#ifndef __ASSEMBLY__

#if __GNUC__
	#define likely(x)       __builtin_expect(!!(x), 1)
	#define unlikely(x)     __builtin_expect(!!(x), 0)
	#define __UNUSED __attribute__((__unused__))
	#define __PACKED __attribute__((packed))
	#define __ALIGNED(x) __attribute__((aligned(x)))
	#define __PRINTFLIKE(__fmt,__varargs) __attribute__((__format__ (__printf__, __fmt, __varargs)))
	#define __SCANFLIKE(__fmt,__varargs) __attribute__((__format__ (__scanf__, __fmt, __varargs)))
	#define __SECTION(x) __attribute((section(x)))
	#define __PURE __attribute((pure))
	#define __CONST __attribute((const))
	#define __NO_RETURN __attribute__((noreturn)) 
	#define __MALLOC __attribute__((malloc))
	#define __WEAK __attribute__((weak))
	#define __GNU_INLINE __attribute__((gnu_inline))
	#define __GET_CALLER(x) __builtin_return_address(0)
	#define __GET_FRAME(x) __builtin_frame_address(0)
	#define __EXTERNALLY_VISIBLE __attribute__((externally_visible))
	#define __FORCE_INLINE __attribute__((always_inline))

#else

	#define likely(x)       (x)
	#define unlikely(x)     (x)
	#define __UNUSED 
	#define __PACKED 
	#define __ALIGNED(x)
	#define __PRINTFLIKE(__fmt,__varargs)
	#define __SCANFLIKE(__fmt,__varargs)
	#define __SECTION(x)
	#define __PURE
	#define __CONST
	#define __NONNULL(x)
	#define __DEPRECATED
	#define __WARN_UNUSED_RESULT
	#define __ALWAYS_INLINE
	#define __MAY_ALIAS
	#define __NO_RETURN
	#define __MALLOC
	#define __GET_CALLER(x)
	#define __EXTERNALLY_VISIBLE 
	#define __FORCE_INLINE inline
#endif

#endif

#define countof(a) (sizeof(a) / sizeof((a)[0]))

#endif
