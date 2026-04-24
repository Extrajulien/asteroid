#include "screens.h"
#include "screen_context.h"
#include "title_elements.h"
void openTitleScreen(GameContext *gameContext);
void closeTitleScreen(GameContext *gameContext);
void updateTitleScreen(GameContext *gameContext);
void drawTitleScreen(const GameContext *gameContext);



Rectangle getStartGameBox() {
    return (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2, 350, 100};
}
Rectangle getEditAsteroidsModeBox() {
    return (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 150, 350, 100};
}
Rectangle getManagePresetsBox() {
    return (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 300, 350, 100};
}


ScreenVTable SCREENS_GetTitleVTable() {
    return (ScreenVTable) {
        .screen = SCREEN_TITLE,
        .open   = openTitleScreen,
        .close  = closeTitleScreen,
        .update = updateTitleScreen,
        .draw   = drawTitleScreen
    };
}

void openTitleScreen(GameContext *gameContext) {
    OVERLAY_STACK_Reset(&gameContext->screenContext.overlayStack);
}

void closeTitleScreen(GameContext *gameContext) {
    OVERLAY_STACK_Reset(&gameContext->screenContext.overlayStack);
}

void updateTitleScreen(GameContext *gameContext) {
    const Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, getStartGameBox())) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            gameContext->screenContext.screen = SCREEN_GAME;
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, getEditAsteroidsModeBox())) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            gameContext->screenContext.screen = SCREEN_PRESET_CREATE;
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, getManagePresetsBox())) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            gameContext->screenContext.screen = SCREEN_PRESET_LIST;
        }
    }
}

void drawTitleScreen(const GameContext *const gameContext) {
    ClearBackground(BLACK);
    Vector2 mousePos = GetMousePosition();
    int boxBorder = 10;
    Rectangle box;
    char *title = "Asteroids";
    int titleSizeX = MeasureText(title, 256);
    DrawText(title, (GetScreenWidth()-titleSizeX) / 2, (GetScreenHeight() / 4)-100, 256, WHITE);
    DrawText("By Julien Lamothe", (GetScreenWidth()-titleSizeX)/2 + titleSizeX-MeasureText("By Julien Lamothe", 20),
        (GetScreenHeight() / 4)+112, 20, ORANGE);
    DRAW_START_BOX();
    DRAW_MAKE_PRESET_BOX();
    DRAW_EDIT_PRESET_BOX();
}