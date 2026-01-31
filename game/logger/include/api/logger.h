#pragma once
/*
 * Simple logger supporting both null-terminated const char* and CharArray strings.
 *
 * Inline color control is supported using the '$' token followed by a single
 * character present in the color lookup table. When a valid token is encountered,
 * the corresponding ANSI color code is emitted.
 *
 * Invalid or unknown color tokens convert to the '$' token upon printing.
 *
 * Example:
 *   "$ythis text is yellow!$x"
 */

/* List of all Logging preprocessor macros and what depends on them.
 * - LOG_MEMORY
 *      ASSERT_ALLOCATION(ptr)
 *      LOG_ALLOCATION_FAILURE()
 *      LOG_MEMORY(str)
 *      LOGF_MEMORY(fmt, ...)
 */



typedef struct CharArray CharArray;

#define LOG(x) \
_Generic((x), \
const char*: LOGGER_LogCstr, \
char*:       LOGGER_LogCstr, \
const CharArray*:   LOGGER_LogCharArray, \
CharArray*:   LOGGER_LogCharArray \
)(x)

#define LOGF(str, ...) \
_Generic((str), \
const char*:      LOGGER_LogFormatCstr, \
char*:            LOGGER_LogFormatCstr, \
const CharArray*: LOGGER_LogFormatCharArray, \
CharArray*:       LOGGER_LogFormatCharArray \
)(str, __VA_ARGS__)

#define LOGGING_MEMORY
#define LOGGING_ARRAY_ACCESS


#ifdef LOGGING_MEMORY
#define ASSERT_ALLOCATION(ptr) \
    do { \
    if (!ptr) { \
        LOG_ALLOCATION_FAILURE(); \
        exit(1);\
    } \
} while (0)

#define LOG_ALLOCATION_FAILURE() \
LOGF("$R[ALLOCATION FAILURE]$r %s:%d %s()\n", __FILE__, __LINE__, __func__)

#define LOG_MEMORY(str) LOGF("$O[MEMORY]$o %s",str)
#define LOGF_MEMORY(fmt, ...) LOGF("$O[MEMORY]$o " fmt, __VA_ARGS__)

#else
#define ASSERT_ALLOCATION(ptr) ((void)0)
#define LOG_ALLOCATION_FAILURE() ((void)0)

#define LOG_MEMORY(str) ((void)0)
#define LOGF_MEMORY(fmt, ...) ((void)0)
#endif

#ifdef LOGGING_ARRAY_ACCESS
    #define ASSERT_ARRAY_ACCESS(arraySize, index) \
        do { \
        if (arraySize <= index) { \
           LOGF("$P[ARRAY ACCESS]$p arraySize '%u', index '%u'. %s:%d %s", arraySize, index, __FILE__, __LINE__, __func__); \
           exit(1); \
        } \
        } while (0)
#else
#define ASSERT_ARRAY_ACCESS(arraySize, index) ((void)0)
#endif










void LOGGER_LogCstr(const char* str);

void LOGGER_LogCharArray(const CharArray* array);

void LOGGER_LogFormatCstr(const char* str, ...);

void LOGGER_LogFormatCharArray(const CharArray* array, ...);