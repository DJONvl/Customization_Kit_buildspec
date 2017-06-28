#ifndef _LOGGING_H
#define _LOGGING_H


//
// Logging macros
//
#define LOG_DEBUG(message) Log(LoggingLevel_Debug, message)
#define LOG_DEBUG_IF(condition, message) \
    do { \
        if (condition) \
        { \
            LOG_DEBUG(message); \
        } \
    } while (0)

#define LOG_INFO(message)   Log(LoggingLevel_Info, message)
#define LOG_INFO_IF(condition, message) \
    do { \
        if (condition) \
        { \
            LOG_INFO(message); \
        } \
    } while (0)

#define LOG_WARNING(message)   Log(LoggingLevel_Warning, message)
#define LOG_WARNING_IF(condition, message) \
    do { \
        if (condition) \
        { \
            LOG_WARNING(message); \
        } \
    } while (0)

#define LOG_FATAL(message)  Log(LoggingLevel_Fatal, message)
#define LOG_FATAL_IF(condition, message) \
    do { \
        if (condition) \
        { \
            LOG_FATAL(message); \
        } \
    } while (0)


//
// Logging levels
//
typedef enum
{
    LoggingLevel_Debug, // The most verbose logging level
    LoggingLevel_Info,
    LoggingLevel_Warning,
    LoggingLevel_Fatal,
    LoggingLevel_Invalid

} LoggingLevel;


void InitLoggingFacility(U8 pll_on);
void SetLoggingLevel(int newLoggingLevel);
LoggingLevel GetLoggingLevel(void);
void Log(LoggingLevel loggingLevel, const char *message);
//void LogPrintf(LoggingLevel loggingLevel,  const char *format, ...);
#define LogPrintf(level, ...) \
do {\
    if (level >= GetLoggingLevel())\
        DBG_MSG(__VA_ARGS__);\
} while(0)

#define _DEBUG_
#define PROFILING

#define LOGPRINTF

#ifdef LOGPRINTF
#define printf(...)         LogPrintf(__VA_ARGS__);
#else
#define printf(...)         Log(__VA_ARGS__);
#endif
 
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

extern int DBG_MSG (char *sz, ...);
#ifdef _DEBUG_
#define MSG(evt, ...) \
do {	\
    if ((MSG_EVT_##evt) & MSG_EVT_MASK) { \
        DBG_MSG((MSG_STR_##evt)); \
        DBG_MSG(__VA_ARGS__); \
    } \
} while(0)

#define MSG_FUNC(f)	MSG(FUC, "<FUNC>: %s\n", __FUNCTION__)
#else
#define MSG(evt, fmt, args...)
#define MSG_FUNC(f)
#endif

#define SEC_DBG_MSG(...)

#ifdef PROFILING
extern U64 get_timer (U64 base);
static U64 tmp_timer;
#define PROF_BEG(x) \
do { \
    tmp_timer = get_timer(0); \
} while(0)

#define PROF_END(x) \
do { \
    tmp_timer = get_timer(tmp_timer); \
} while(0)

#define PROF_PRINT(s) \
do { \
    MSG(PROF, "%s (%d ms)\n", s, (U32)tmp_timer); \
} while(0)
#else
#define PROF_BEG()         do{}while(0)
#define PROF_END()         do{}while(0)
#define PROF_PRINT(s)      do{}while(0)
#endif

#endif // _LOGGING_H
