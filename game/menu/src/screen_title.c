#include "screens.h"

void openTitleScreen(const Screen *currentScreen, GameContext *gameContext);
void closeTitleScreen(const Screen *currentScreen, GameContext *gameContext);
void updateTitleScreen(Screen *currentScreen, GameContext *gameContext);
void drawTitleScreen(const Screen *currentScreen, const GameContext *gameContext);



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

void openTitleScreen(const Screen *currentScreen, GameContext *gameContext) {

}

void closeTitleScreen(const Screen *currentScreen, GameContext *gameContext) {

}

void updateTitleScreen(Screen *currentScreen, GameContext *gameContext) {
    const Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, getStartGameBox())) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *currentScreen = SCREEN_GAME;
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, getEditAsteroidsModeBox())) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *currentScreen = SCREEN_PRESET_CREATE;
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, getManagePresetsBox())) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *currentScreen = SCREEN_PRESET_LIST;
        }
    }
}

void drawTitleScreen(const Screen *const currentScreen, const GameContext *const gameContext) {
    ClearBackground(BLACK);
    Vector2 mousePos = GetMousePosition();
    int boxBorder = 10;
    Rectangle box;
    char *title = "Asteroids";
    int titleSizeX = MeasureText(title, 256);
    DrawText(title, (GetScreenWidth()-titleSizeX) / 2, (GetScreenHeight() / 4)-100, 256, WHITE);
    DrawText("By Julien Lamothe", (GetScreenWidth()-titleSizeX)/2 + titleSizeX-MeasureText("By Julien Lamothe", 20),
        (GetScreenHeight() / 4)+112, 20, ORANGE);

    box = getStartGameBox();
    if (CheckCollisionPointRec(mousePos, getStartGameBox())) {
        DrawRectangle(box.x, box.y, box.width, box.height, ORANGE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Start", box.x + (box.width-50) / 2, box.y + box.height / 2 - 10, 20, ORANGE);
    } else {
        DrawRectangle(box.x, box.y, box.width, box.height, WHITE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Start", box.x + (box.width-50) / 2,
                 box.y + box.height / 2 - 10, 20, WHITE);
    }

    box = getEditAsteroidsModeBox();
    if (CheckCollisionPointRec(mousePos, getEditAsteroidsModeBox())) {
        DrawRectangle(box.x, box.y, box.width, box.height, ORANGE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Mode", box.x + (box.width - 96.0) / 2, box.y + box.height / 2 - 10, 20, ORANGE);
    } else {
        DrawRectangle(box.x, box.y, box.width, box.height, WHITE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Mode", box.x + (box.width - 96.0) / 2,
                 box.y + box.height / 2 - 10, 20, WHITE);
    }

    box = getManagePresetsBox();
    if (CheckCollisionPointRec(mousePos, getManagePresetsBox())) {
        DrawRectangle(box.x, box.y, box.width, box.height, ORANGE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Presets", box.x + (box.width-120) / 2, box.y + box.height / 2 - 10, 20, ORANGE);
    } else {
        DrawRectangle(box.x, box.y, box.width, box.height, WHITE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Presets", box.x + (box.width-120) / 2,
                 box.y + box.height / 2 - 10, 20, WHITE);
    }
}