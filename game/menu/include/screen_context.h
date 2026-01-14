#pragma once
#include "screen.h"


typedef struct TitleContext {

} TitleContext;

typedef struct PresetCreateContext {
    AsteroidSize selectedAsteroidSize;
    char presetName[512];
} PresetCreateContext;

typedef struct ScreenContext {
    Screen screen;
    union {
        TitleContext titleCtx;
        PresetCreateContext presetCreateCtx;
    };
} ScreenContext;

