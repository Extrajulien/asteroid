#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "game_api.h"
#include "screen.h"

typedef struct ScreenVTable {
    Screen screen;
    void (* open) (GameContext *context);
    void (* close) (GameContext *context);
    void (* update) (GameContext *context);
    void (* draw) (const GameContext *context);
} ScreenVTable;

ScreenVTable SCREEN_GetVTable(Screen screen);

#ifdef __cplusplus
}
#endif