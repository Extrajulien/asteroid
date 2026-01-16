#include "char_array.h"

#include <stdlib.h>
#include <string.h>

#include "logger.h"


typedef struct CharArray {
    char* data;
    size_t count;
} CharArray;



CharArray* CHAR_ARRAY_Create(const char* data, const size_t count) {
    CharArray* array = malloc(sizeof(CharArray));
    ASSERT_ALLOCATION(array);
    size_t charCount;
    if (count == 0) {
        charCount = strlen(data);
    } else {
        charCount = count;
    }


    char* temp = malloc(sizeof(char) * charCount);
    ASSERT_ALLOCATION(temp);

    memcpy(temp, data, charCount);

    array->data = temp;
    array->count = charCount;
    return array;
}

const char* CHAR_ARRAY_GetData(const CharArray *array) {
    return array->data;
}

size_t CHAR_ARRAY_GetCount(const CharArray *array) {
    return array->count;
}

void CHAR_ARRAY_Free(CharArray *array) {
    if (array->data) {
        free(array->data);
    }
    free(array);
}

const char* CHAR_ARRAY_GetCString(const CharArray *array) {
    char* s = malloc(array->count + 1);
    ASSERT_ALLOCATION(s);
    memcpy(s, array->data, array->count);
    s[array->count] = '\0';
    return s;
}