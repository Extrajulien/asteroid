#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct CharArray CharArray;

/** creates a copy of a given char[].
 * @param data char array to be copied
 * @param count size of the array to copy, if 0 then look for a \0 character
 * @return pointer to the heap allocated CharArray
 */
CharArray* CHAR_ARRAY_Create(const char* data, size_t count);

/**
 * compare the content of 2 CharArray (until '\0' or end of the array) to find if they are the same.
 * @param lsh array to be compared
 * @param rsh array to be compared
 * @return are the arrays content equal
 */
bool CHAR_ARRAY_CompareEquality(CharArray *lsh, CharArray *rsh);

/**
 * Free all heap allocations in the CharArray, thus invalidating the pointer.
 * @param array CharArray* to be freed
 */
void CHAR_ARRAY_Free(CharArray *array);

const char* CHAR_ARRAY_GetData(const CharArray *array);

size_t CHAR_ARRAY_GetCount(const CharArray *array);



/**
 * Make a heap allocated null terminated character array from a CharArray
 * @param array array to make the string from
 * @return ALLOCATED null terminated character array
 */
const char* CHAR_ARRAY_GetCString(const CharArray *array);
