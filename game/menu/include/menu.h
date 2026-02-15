#ifndef MENU_H
#define MENU_H
#include "raylib.h"

#define DARKERGRAY (Color){20,20,20,255}
#define SPREAD_COLOR (Color){0,255,0,50}
#define RADIUS_COLOR (Color){0,255,0,150}
#define TEXT_INDICATOR_COLOR (Color){140,255,241,255}
#define BIG_ASTEROID_POS        (Vector2){(float) GetScreenWidth()/4, (float) GetScreenHeight()/4}
#define MID_ASTEROID_POS        (Vector2){(float) GetScreenWidth()/4, (float) GetScreenHeight()/8*5}
#define SML_ASTEROID_POS        (Vector2){(float) GetScreenWidth()/4, (float) GetScreenHeight()/8*7}

typedef struct Asteroid Asteroid;
typedef struct AsteroidArray AsteroidArray;
typedef struct AsteroidPresetArray AsteroidPresetArray;

Rectangle MENU_GetAsteroidSquareHitBox(const Asteroid* asteroid, const AsteroidPresetArray* presetArr);

void resetAsteroidAttributes();
void refreshAsteroids(AsteroidArray *asteroidArray, const AsteroidPresetArray* presetArr);


void loadThemes();
void initMenuBoxes();

#endif //MENU_H
