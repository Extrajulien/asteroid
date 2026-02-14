#pragma once
#include "asteroid_vertice_array.h"

typedef struct VerticePool VerticePool;

VerticePool* VERTICE_POOL_Create();
void VERTICE_POOL_Free(VerticePool *verticePool);

// returns the index of the reserved object
size_t VERTICE_POOL_ReserveIndex(VerticePool *verticePool);

void VERTICE_POOL_ReleaseIndex(const VerticePool *pool, size_t index);

VerticeArray* VERTICE_POOL_GetVerticeArray(const VerticePool *verticePool, size_t index);