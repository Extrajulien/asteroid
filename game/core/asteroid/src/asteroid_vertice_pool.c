#include "asteroid_vertice_pool.h"
#include "logger.h"
#include <stdlib.h>

typedef struct VerticePool {
    VerticeArray *verticeObjArray;
    size_t size;
} VerticePool;

void defaultInitializeVerticeArray(const VerticePool *verticePool, size_t startIndex, size_t count);

VerticePool* VERTICE_POOL_Create() {
    VerticePool *pool = malloc(sizeof(VerticePool));
    ASSERT_ALLOCATION(pool);
    pool->verticeObjArray = malloc(sizeof(VerticeArray));
    ASSERT_ALLOCATION(pool->verticeObjArray);
    pool->size = 1;
    defaultInitializeVerticeArray(pool, 0, pool->size);
    return pool;
}

void VERTICE_POOL_Free(VerticePool *verticePool) {
    free(verticePool->verticeObjArray);
    free(verticePool);
}

size_t VERTICE_POOL_ReserveIndex(VerticePool *verticePool) {
    for (int i = 0; i < verticePool->size; ++i) {
        if (!verticePool->verticeObjArray[i].isUsed) {
            verticePool->verticeObjArray[i].isUsed = true;
            return i;
        }
    }
    const size_t index = verticePool->size;
    verticePool->size *= 2;
    VerticeArray *temp = realloc(verticePool->verticeObjArray, verticePool->size * sizeof(VerticeArray));
    ASSERT_ALLOCATION(temp);
    verticePool->verticeObjArray = temp;
    defaultInitializeVerticeArray(verticePool, index, index);
    verticePool->verticeObjArray[index].isUsed = true;
    return index;
}

void VERTICE_POOL_ReleaseIndex(const VerticePool *pool, const size_t index) {
    pool->verticeObjArray[index].isUsed = false;
}

VerticeArray* VERTICE_POOL_GetVerticeArray(const VerticePool *verticePool, const size_t index) {
    return verticePool->verticeObjArray + index;
}

void defaultInitializeVerticeArray(const VerticePool *verticePool, const size_t startIndex, const size_t count) {
    ASSERT_ARRAY_ACCESS(verticePool->size, startIndex+count-1);
    for (int i = 0; i < count; ++i) {
        verticePool->verticeObjArray[i + startIndex] = VERTICE_ARRAY_Allocate();
    }
}