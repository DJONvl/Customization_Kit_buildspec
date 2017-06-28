#ifndef KAL_RELEASE_H
#define KAL_RELEASE_H

#include <sys/types.h>



typedef void (* VOID_FUNC)(void);


/* Debug message event */
#define MSG_EVT_NONE        0x00000000      /* No event */
#define MSG_EVT_ERR         0x00000001      /* ERR related event */
#define MSG_EVT_INIT        0x00000002      /* INIT related event */
#define MSG_EVT_DABMT       0x00000004      /* DA BMT related event */
#define MSG_EVT_DAPMT       0x00000008      /* DA PMT related event */
#define MSG_EVT_DEBUG       0x00000010      /* DA DBG related event */
#define MSG_EVT_MIG         0x00000020      /* DA Program failed migration related event */
#define MSG_EVT_PROF        0x00000040      /* DA performance benchmark related event */
#define MSG_EVT_USB         0x00000080      /* USB related event */
#define MSG_EVT_ALL         0xffffffff

//#define MSG_EVT_MASK       (MSG_EVT_INIT | MSG_EVT_MIG | MSG_EVT_ERR)
#define MSG_EVT_MASK       (MSG_EVT_NONE)
//#define MSG_EVT_MASK       (MSG_EVT_INIT | MSG_EVT_PROF)

#define MSG_STR_INIT        "[INIT] "
#define MSG_STR_DABMT       "[BMT ] "
#define MSG_STR_DAPMT       "[PMT ] "
#define MSG_STR_ERR         "[ERR ] "
#define MSG_STR_DEBUG       "[DBG ] "
#define MSG_STR_MIG         "[MIG ] "
#define MSG_STR_PROF        "[PROF] "
#define MSG_STR_USB         "[USB ] "


#ifdef MSG_DEBUG
#define MSG(evt,...) \
   do {	\
   if ((MSG_EVT_##evt) & MSG_EVT_MASK) { \
   MSG(USB,__VA_ARGS__); \
   } \
   } while(0)

#define MSG_FUNC(f)	MSG(FUC, "<FUNC>: %s\n", __FUNCTION__)
#else
#define MSG(evt, fmt, args...)
#define MSG_FUNC(f)
#endif


#endif  /* USB_HW_H */               

