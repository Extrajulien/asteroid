#define LIGHTBLUE ((Color){ 240, 248, 255, 255 })

#include <stdio.h>

#include "files.h"
#include "raymath.h"
#include "screen_virtual_table.h"

void openPresetListScreen(GameContext *gameContext);
void closePresetListScreen(GameContext *gameContext);
void updatePresetListScreen(GameContext *gameContext);
void drawPresetListScreen(const GameContext *gameContext);

void DrawListItem(PresetListContext ctx, size_t index, Color BG_COLOR);

ScreenVTable SCREENS_GetPresetListVTable() {
    return (ScreenVTable) {
        .screen = SCREEN_PRESET_LIST,
        .open   = openPresetListScreen,
        .close  = closePresetListScreen,
        .update = updatePresetListScreen,
        .draw   = drawPresetListScreen
    };
}

void openPresetListScreen(GameContext *gameContext) {
    gameContext->screenContext.presetListCtx.asteroidArray = ASTEROIDS_CreateArray();
    gameContext->screenContext.presetListCtx.presetArray = ASTEROID_PRESETS_CreateArray();
    gameContext->screenContext.presetListCtx.asteroidArray->verticePool = VERTICE_POOL_Create();
    gameContext->screenContext.presetListCtx.scrollPosition = 0;
    FILE_LoadAllPresets(gameContext->screenContext.presetListCtx.presetArray);
    for (int i = 0; i < gameContext->screenContext.presetListCtx.presetArray->presetCount; ++i) {
        const Asteroid asteroid = ASTEROID_Create(&gameContext->screenContext.presetListCtx.presetArray->presets[i], gameContext->screenContext.presetListCtx.asteroidArray->verticePool);
        ASTEROIDS_Add(gameContext->screenContext.presetListCtx.asteroidArray, asteroid);
    }
    ASTEROIDS_Update(gameContext->screenContext.presetListCtx.asteroidArray);
}

void closePresetListScreen(GameContext *gameContext) {
    ASTEROIDS_FreeArray(gameContext->screenContext.presetListCtx.asteroidArray);
    ASTEROID_PRESETS_Free(gameContext->screenContext.presetListCtx.presetArray);
    VERTICE_POOL_Free(gameContext->screenContext.presetListCtx.asteroidArray->verticePool);
}

void updatePresetListScreen(GameContext *gameContext) {
    GetMouseWheelMove();
    ASTEROIDS_Update(gameContext->screenContext.presetListCtx.asteroidArray);
}

void drawPresetListScreen(const GameContext *const gameContext) {
    const Color BG_COLOR = BLACK;
    ClearBackground(BG_COLOR);

    for (int i = 0; i < gameContext->screenContext.presetListCtx.presetArray->presetCount; ++i) {
        DrawListItem(gameContext->screenContext.presetListCtx, i, BG_COLOR);
    }
}


void DrawListItem(PresetListContext ctx, const size_t index, const Color BG_COLOR) {
    const int LEFT_GUTTER_SIZE = 100;
    const int TOP_GUTTER_SIZE = 50;
    const int VERTICAL_SPACING = 20;
    const int listWidth = 1500;
    const int listHeight = 250;
    const int listY = TOP_GUTTER_SIZE + listHeight * index + VERTICAL_SPACING * index;
    const int borderWidth = 5;
    const Color ACCENT_COLOR = CheckCollisionPointRec(GetMousePosition(), (Rectangle) {LEFT_GUTTER_SIZE, listY, listWidth, listHeight}) ? ORANGE : WHITE;

    //PRESET MOCK RENDER
    DrawRectangle(LEFT_GUTTER_SIZE, listY,listHeight, listHeight, ACCENT_COLOR);
    DrawRectangle(LEFT_GUTTER_SIZE + borderWidth, listY + borderWidth, listHeight - borderWidth * 2, listHeight - borderWidth * 2, BG_COLOR);

    //PRESET DETAIL
    DrawRectangle(LEFT_GUTTER_SIZE + listHeight,listY,1100, listHeight, ACCENT_COLOR);
    DrawRectangle(LEFT_GUTTER_SIZE + listHeight + borderWidth,listY + borderWidth,1100 - borderWidth * 2, listHeight - borderWidth * 2, BG_COLOR);

    //PRESET STATUS
    DrawRectangle(LEFT_GUTTER_SIZE + listHeight + 1100,listY,listWidth - 1100 - listHeight, listHeight, ACCENT_COLOR);
    DrawRectangle(LEFT_GUTTER_SIZE + listHeight + borderWidth + 1100,listY + borderWidth,listWidth - 1100 - listHeight - borderWidth * 2, listHeight - borderWidth * 2, BG_COLOR);

    //PRESET NAME
    DrawRectangle(LEFT_GUTTER_SIZE + listHeight,listY,1100, 100, ACCENT_COLOR);
    DrawRectangle(LEFT_GUTTER_SIZE + listHeight + borderWidth,listY + borderWidth,1100 - borderWidth * 2, 100 - borderWidth * 2, DARKGRAY);

    const int sizeFontSize = 32;
    DrawText(ctx.presetArray->presets[index].presetName, LEFT_GUTTER_SIZE + 20 + listHeight,listY + 30,40, LIGHTBLUE);
    char size[20];
    sprintf(size, "%c", AsteroidSizeToChar(ctx.presetArray->presets[index].size));
    int txtSize = MeasureText(size, sizeFontSize);
    DrawText(size, LEFT_GUTTER_SIZE + listHeight - (txtSize + 8), listY + 5, sizeFontSize, LIGHTBLUE);

    if (index + 1 > ctx.asteroidArray->nbAsteroid) return;
    const VerticePool *pool = ctx.asteroidArray->verticePool;
    const VerticeArray *vert = VERTICE_POOL_GetVerticeArray(pool, index);
    const Asteroid asteroid = ctx.asteroidArray->asteroid[index];
    const Vector2 astPos = (Vector2){(float)LEFT_GUTTER_SIZE + ((float)listHeight / 2), (float) listY + ((float)listHeight / 2)};

    for (int j = 0; j < vert->count; ++j) {
        const Vector2 startPos = Vector2Add(vert->vertices[j], astPos);
        const Vector2 endPos = Vector2Add(vert->vertices[(j + 1) % vert->count], astPos);
        DrawLineEx(startPos, endPos, asteroid.lineInfo.thickness, asteroid.lineInfo.color);
    }
}


