#include "asteroid_vertice_array.h"

#include <math.h>
#include <stdlib.h>
#include "asteroid_preset.h"
#include "logger.h"

void VERTICE_ARRAY_Expand(VerticeArray *verticeArray, size_t fullSize);
void VERTICE_ARRAY_InitOriginalVertices(const VerticeArray* verticeArray, float radius, float spread, bool isRandomBothSides);


VerticeArray VERTICE_ARRAY_Allocate() {
    VerticeArray verticeArray = {};
    verticeArray.isUsed = false;
    verticeArray.count = 0;
    verticeArray.capacity = 1;
    verticeArray.originalVertices = malloc(sizeof(Vector2) * verticeArray.capacity);
    verticeArray.vertices = malloc(sizeof(Vector2) * verticeArray.capacity);
    return verticeArray;
}

void VERTICE_ARRAY_LoadPreset(VerticeArray *verticeArray, const AsteroidPreset *preset) {
    if (verticeArray->capacity < verticeArray->count + 1) {
        VERTICE_ARRAY_Expand(verticeArray, preset->nbVertices);
    }
    verticeArray->count = preset->nbVertices;
    VERTICE_ARRAY_InitOriginalVertices(verticeArray, preset->radius, preset->spread, preset->isRandomBothSides);
}



void VERTICE_ARRAY_Free(VerticeArray *verticeArray) {
    free(verticeArray->originalVertices);
    free(verticeArray->vertices);
    verticeArray->capacity = 0;
    verticeArray->count = 0;
}


void VERTICE_ARRAY_Expand(VerticeArray *verticeArray, const size_t fullSize) {
    if (verticeArray && verticeArray->originalVertices && verticeArray->vertices) {
        if (verticeArray->capacity > fullSize) {
            LOG("$CUnnecessary VERTICE_ARRAY_Expand() use, capacity is already bigger than desired size!\n");
            return;
        }
        verticeArray->capacity = fullSize;
        Vector2 *temp = realloc(verticeArray->originalVertices, sizeof(Vector2) * verticeArray->capacity);
        ASSERT_ALLOCATION(temp);
        verticeArray->originalVertices = temp;

        temp = realloc(verticeArray->vertices, sizeof(Vector2) * verticeArray->capacity);
        ASSERT_ALLOCATION(temp);
        verticeArray->vertices = temp;
    }
}

void VERTICE_ARRAY_InitOriginalVertices(const VerticeArray* verticeArray, const float radius, const float spread, const bool isRandomBothSides) {
    const float radSpacing = (2 * PI) / (float) verticeArray->count;
    for (int i = 0; i < verticeArray->count; i++) {
        float distanceFromCenter;
        if (isRandomBothSides) {
            const float rndSpread = ((float)rand() / (float)RAND_MAX) * spread*2;
            distanceFromCenter = rndSpread + radius - spread;
        } else {
            const float rndSpread = ((float)rand() / (float)RAND_MAX) * spread;
            distanceFromCenter = rndSpread + radius;
        }
        verticeArray->originalVertices[i].x = cosf(radSpacing * (float) i) * distanceFromCenter;
        verticeArray->originalVertices[i].y = sinf(radSpacing * (float) i) * distanceFromCenter;
    }
}


