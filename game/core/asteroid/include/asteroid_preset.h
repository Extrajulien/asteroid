#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "asteroid_size.h"
#include "components.h"
#include "particle.h"


static const float PRESET_ROTATION_PRECISION = 100.0f;

typedef struct AsteroidPreset {
    const char *presetName;
    AsteroidSize size;
    float radius;
    float spread;
    float minRotationSpeed;
    float maxRotationSpeed;
    float maxSpeed;
    int score;
    int nbVertices;
    ComponentLineInfo lineInfo;
    ParticlePreset particlePreset;
    bool isRandomBothSides;
} AsteroidPreset;

typedef struct AsteroidPresetArray {
    AsteroidPreset *presets;
    size_t presetCount;
    size_t capacity;
} AsteroidPresetArray;

AsteroidPresetArray *ASTEROID_PRESETS_CreateArray();
void ASTEROID_PRESETS_Free(AsteroidPresetArray *presets);
void ASTEROID_PRESETS_Add(AsteroidPresetArray *presetArray, const AsteroidPreset *preset);
void ASTEROID_PRESET_Purge(AsteroidPresetArray *presetArray);