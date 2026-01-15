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




void LOGGER_LogCstr(const char* str);

void LOGGER_LogCharArray(const CharArray* array);

void LOGGER_LogFormatCstr(const char* str, ...);

void LOGGER_LogFormatCharArray(const CharArray* array, ...);