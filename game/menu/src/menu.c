#include "menu.h"
#include "raylib.h"
#include "asteroid.h"
#include "asteroid_array.h"
#include "asteroid_preset.h"
#include "raygui.h"
#include "raygui_styles.h"
#define EPSILON 0.1f
const Asteroid* getAsteroid(const AsteroidArray* asteroidArray, const AsteroidSize *selection);
void drawAstOptions(AsteroidPreset *preset, const Asteroid* asteroid);



// managePresets
// ----------------------------------------------------------------------------------------------------------------

// edit asteroids
// ----------------------------------------------------------------------------------------------------------------


/*
void updateEditAsteroidMenu(AsteroidArray *bigArr, AsteroidArray *midArr, AsteroidArray *smlArr) {
    if (IsKeyPressed(KEY_F)) isModified = true;

    if (isModified) {
        printf("%f", ((BigAsteroid*)bigArr->asteroid[0])->base.radius);
        //printf("%d", ((MidAsteroid*)midArr->asteroid[0])->base.type);
        //printf("%d", ((SmlAsteroid*)smlArr->asteroid[0])->base.type);
        freeAsteroidArray(bigArr, SIZE_BIG);
        freeAsteroidArray(midArr, SIZE_MEDIUM);
        //freeAsteroidArray(smlArr, ((SmlAsteroid*)smlArr->asteroid)->base.type);

        refreshAsteroids(bigArr, midArr, smlArr);
        isModified = false;
    }
    rotateAsteroid(bigArr, SIZE_BIG);
    rotateAsteroid(midArr, SIZE_MEDIUM);
    rotateAsteroid(smlArr, SIZE_SMALL);
}
*/

void refreshAsteroids(AsteroidArray *asteroidArray, const AsteroidPresetArray* presetArr) {
    ASTEROIDS_Purge(asteroidArray);
    if (presetArr->presetCount == SIZE_COUNT) {
        Asteroid big = ASTEROID_Create(&presetArr->presets[SIZE_BIG]);
        ASTEROID_MoveTo(&big, BIG_ASTEROID_POS);
        ASTEROIDS_Add(asteroidArray, &big);

        Asteroid mid = ASTEROID_Create(&presetArr->presets[SIZE_MEDIUM]);
        ASTEROID_MoveTo(&mid, MID_ASTEROID_POS);
        ASTEROIDS_Add(asteroidArray, &mid);

        Asteroid small = ASTEROID_Create(&presetArr->presets[SIZE_SMALL]);
        ASTEROID_MoveTo(&small, SML_ASTEROID_POS);
        ASTEROIDS_Add(asteroidArray, &small);
    }
}

void loadThemes() {
    GUI_LoadStyle(GUI_STYLE_AMBER);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 48);
}

void resetAsteroidAttributes() {
    // reset preset
}

Rectangle MENU_GetAsteroidSquareHitBox(const Asteroid* asteroid) {
    return (Rectangle) {
        asteroid->position.x - asteroid->shape.radius,
        asteroid->position.y - asteroid->shape.radius,
        asteroid->shape.radius * 2,
        asteroid->shape.radius * 2
    };
}