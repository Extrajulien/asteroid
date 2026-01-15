#include "logger.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "log_color.h"
#include "char_array.h"

#define LOG_BUFFER_SIZE 1024

void PrintWithColorCStr(const char* str);
void PrintWithColorCharArray(const CharArray *array);

void LOGGER_LogCstr(const char* str) {
    PrintWithColorCStr(str);
}

void LOGGER_LogCharArray(const CharArray* array) {
    PrintWithColorCharArray(array);
}

void LOGGER_LogFormatCstr(const char* str, ...) {
    va_list args;
    va_start(args, str);


    char temp[LOG_BUFFER_SIZE];
    vsnprintf(temp, sizeof(temp), str, args);

    va_end(args);

    PrintWithColorCStr(temp);
}

void LOGGER_LogFormatCharArray(const CharArray* array, ...) {
    va_list args;
    va_start(args, array);

    char temp[LOG_BUFFER_SIZE];
    vsnprintf(temp, sizeof(temp) - sizeof(char), CHAR_ARRAY_GetData(array), args);
    va_end(args);

    PrintWithColorCStr(temp);
}

void PrintWithColorCStr(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == COLORCODE_TOKEN && str[i + 1] != '\0') {
            ChangeColor(GetColorFromChar(str[i + 1]));
            ++i;
        } else {
            putchar(str[i]);
        }
        ++i;
    }
    ChangeColor(COLOR_RESET);
}

void PrintWithColorCharArray(const CharArray *array) {
    const char* text = CHAR_ARRAY_GetData(array);
    int i = 0;
    while (i != CHAR_ARRAY_GetCount(array)) {
        if (text[i] == COLORCODE_TOKEN && i + 1 != CHAR_ARRAY_GetCount(array)) {
            ChangeColor(GetColorFromChar(text[i + 1]));
            ++i;
        } else {
            putchar(text[i]);
        }
        ++i;
    }
    ChangeColor(COLOR_RESET);
}