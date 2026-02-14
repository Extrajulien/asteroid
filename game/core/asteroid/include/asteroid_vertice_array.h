#pragma once
#include <stddef.h>

#include "asteroid_preset.h"
#include "raylib.h"

typedef struct VerticeArray {
    bool isUsed;
    size_t count;
    size_t capacity;
    Vector2 *originalVertices;
    Vector2 *vertices;
} VerticeArray;

// creates a dummy VerticeArray object
VerticeArray VERTICE_ARRAY_Allocate();

void VERTICE_ARRAY_LoadPreset(VerticeArray *verticeArray, const AsteroidPreset *preset);


void VERTICE_ARRAY_Free(VerticeArray *verticeArray);


