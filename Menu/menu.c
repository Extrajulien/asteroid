//
// Created by julie on 2025-04-27.
//

#include "menu.h"
#include "raylib.h"

static Rectangle startGameBox;
static Rectangle editAsteroidsModeBox ;
static Rectangle managePresetsBox;

void titleMenu() {
    Vector2 mousePos = GetMousePosition();
    int boxBorder = 10;
    Rectangle *pBox;
    char *title = "Asteroids";
    int titleSizeX = MeasureText(title, 256);
    DrawText(title, (GetScreenWidth()-titleSizeX) / 2, (GetScreenHeight() / 4)-100, 256, WHITE);
    DrawText("By Julien Lamothe", (GetScreenWidth()-titleSizeX)/2 + titleSizeX-MeasureText("By Julien Lamothe", 20),
        (GetScreenHeight() / 4)+112, 20, GRAY);

    pBox = &startGameBox;
    if (CheckCollisionPointRec(mousePos, startGameBox)) {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, GRAY);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Start", pBox->x + (pBox->width-50) / 2, pBox->y + pBox->height / 2 - 10, 20, GRAY);
    } else {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, WHITE);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Start", pBox->x + (pBox->width-50) / 2,
                 pBox->y + pBox->height / 2 - 10, 20, WHITE);
    }

    pBox = &editAsteroidsModeBox;
    if (CheckCollisionPointRec(mousePos, editAsteroidsModeBox)) {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, GRAY);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Edit Mode", pBox->x + (pBox->width - 96.0) / 2, pBox->y + pBox->height / 2 - 10, 20, GRAY);
    } else {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, WHITE);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Edit Mode", pBox->x + (pBox->width - 96.0) / 2,
                 pBox->y + pBox->height / 2 - 10, 20, WHITE);
    }

    pBox = &managePresetsBox;
    if (CheckCollisionPointRec(mousePos, managePresetsBox)) {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, GRAY);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Edit Presets", pBox->x + (pBox->width-120) / 2, pBox->y + pBox->height / 2 - 10, 20, GRAY);
    } else {
        DrawRectangle(pBox->x, pBox->y, pBox->width, pBox->height, WHITE);
        DrawRectangle(pBox->x + boxBorder / 2, pBox->y + boxBorder / 2, pBox->width - boxBorder,
                      pBox->height - boxBorder, BLACK);
        DrawText("Edit Presets", pBox->x + (pBox->width-120) / 2,
                 pBox->y + pBox->height / 2 - 10, 20, WHITE);
    }

}

void titleMenuInput(bool *isTitleMenu, bool *isGame, bool *isAsteroidEditScreen, bool *editPresets) {
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, startGameBox)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *isTitleMenu = false;
            *isGame = true;
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, editAsteroidsModeBox)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *isTitleMenu = false;
            *isAsteroidEditScreen = true;
        }
    }
    if (CheckCollisionPointRec(mousePos, managePresetsBox)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *isTitleMenu = false;
            *editPresets = true;
        }
    }
    if (IsKeyPressed(KEY_SPACE)) {
        *isTitleMenu = false;
        *isGame = true;
    }
}


void initMenuBoxes() {
    startGameBox = (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2, 350, 100};
    editAsteroidsModeBox = (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 150, 350, 100};
    managePresetsBox = (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 300, 350, 100};
}