//
// Created by julie on 2025-04-27.
//

#include "menu.h"

void titleMenu(Rectangle *startGameBox, Rectangle *editAsteroidsMode, Rectangle *managePresets) {
    Vector2 mousePos = GetMousePosition();
    int boxBorder = 10;
    Rectangle *pBox;
    char *title = "Asteroids";
    int titleSizeX = MeasureText(title, 256);
    DrawText(title, (GetScreenWidth()-titleSizeX) / 2, (GetScreenHeight() / 4)-100, 256, WHITE);
    DrawText("By Julien Lamothe", (GetScreenWidth()-titleSizeX)/2 + titleSizeX-MeasureText("By Julien Lamothe", 20),
        (GetScreenHeight() / 4)+112, 20, GRAY);

    pBox = startGameBox;
    if (CheckCollisionPointRec(mousePos, *startGameBox)) {
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

    pBox = editAsteroidsMode;
    if (CheckCollisionPointRec(mousePos, *editAsteroidsMode)) {
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

    pBox = managePresets;
    if (CheckCollisionPointRec(mousePos, *managePresets)) {
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
