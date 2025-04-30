//
// Created by julie on 2025-04-27.
//

#ifndef MENU_H
#define MENU_H
#include "raylib.h"

void titleMenu();

void titleMenuInput(bool *isTitleMenu, bool *isGame, bool *isAsteroidEditScreen, bool *editPresets);
void initMenuBoxes();

#endif //MENU_H
