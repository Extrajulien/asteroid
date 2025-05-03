//
// Created by julie on 2025-04-27.
//

#ifndef MENU_H
#define MENU_H
#include "raylib.h"
#include "asteroids.h"

Rectangle getStartGameBox();
Rectangle getEditAsteroidsModeBox();
Rectangle getManagePresetsBox();

void titleMenu();
void editAsteroidMenu( bool *isTitleMenu, bool *isAsteroidEditScreen);
void resetAsteroidAttributes();
void drawAstOptions(AsteroidTraits *traits, Vector2 asteroidPos);
void checkForUpdate(float oldValue, float newValue);
void updateEditAsteroidMenu(AsteroidArray *bigArr, AsteroidArray *midArr, AsteroidArray *smlArr);
void refreshAsteroids(AsteroidArray *bigArr, AsteroidArray *midArr, AsteroidArray *smlArr);
void createBigAsteroidEditMode(AsteroidArray *asteroids);
void createMidAsteroidEditMode(AsteroidArray *asteroids);
void createSmlAsteroidEditMode(AsteroidArray *asteroids);

void titleMenuInput(bool *isTitleMenu, bool *isGame, bool *isAsteroidEditScreen, bool *editPresets);
void loadThemes();
void initMenuBoxes();

#endif //MENU_H
