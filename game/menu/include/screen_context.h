#pragma once
#include <overlay_stack.h>

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

typedef struct ScreenGameContext {
    time_t GameOverTime;
} ScreenGameContext;

typedef struct ScreenContext {
    Screen screen;
    OverlayStack overlayStack;
    union {
        TitleContext titleCtx;
        PresetCreateContext presetCreateCtx;
        ScreenGameContext gameCtx;
    };
} ScreenContext;

