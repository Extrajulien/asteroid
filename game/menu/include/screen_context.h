#pragma once
#include "screen.h"

typedef enum PresetCreateTextArea {
    PRESET_CREATE_VERTICES_SPINNER,
    PRESET_CREATE_PRESET_NAME
} PresetCreateTextArea;

typedef struct TitleContext {

} TitleContext;

typedef struct PresetCreateContext {
    bool isModified;
    char presetName[512];
    AsteroidSize selectedAsteroidSize;
    bool isSizeDropdownOn;
    PresetCreateTextArea currentTextArea;
} PresetCreateContext;

typedef struct ScreenContext {
    Screen screen;
    union {
        TitleContext titleCtx;
        PresetCreateContext presetCreateCtx;
    };
} ScreenContext;

