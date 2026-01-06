#ifndef MENU_H
#define MENU_H
#include "raylib.h"
#include "asteroid.h"
#include "asteroid_array.h"

Rectangle getStartGameBox();
Rectangle getEditAsteroidsModeBox();
Rectangle getManagePresetsBox();

void titleMenu();
void editAsteroidMenu(const AsteroidArray *asteroidArray, AsteroidPresetArray *presetArray, bool *isTitleMenu, bool *isAsteroidEditScreen);
void resetAsteroidAttributes();
void checkForUpdate(float oldValue, float newValue);
void updateEditAsteroidMenu(AsteroidArray *bigArr, AsteroidArray *midArr, AsteroidArray *smlArr);
void refreshAsteroids(AsteroidArray *asteroidArray, const AsteroidPresetArray* presetArr);
void createBigAsteroidEditMode(AsteroidArray *asteroids);
void createMidAsteroidEditMode(AsteroidArray *asteroids);
void createSmlAsteroidEditMode(AsteroidArray *asteroids);

void titleMenuInput(bool *isTitleMenu, bool *isGame, bool *isAsteroidEditScreen, bool *editPresets);
void loadThemes();
void initMenuBoxes();

#endif //MENU_H
