#pragma once
#include "game_api.h"
#include "screen.h"

typedef struct ScreenVTable {
    Screen screen;
    void (* open) (const Screen *current, GameContext *context);
    void (* close) (const Screen *current, GameContext *context);
    void (* update) (Screen *current, GameContext *context);
    void (* draw) (const Screen *current, const GameContext *context);
} ScreenVTable;

ScreenVTable SCREEN_GetVTable(Screen screen);