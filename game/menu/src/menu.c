#include "menu.h"

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "asteroid.h"
#include "asteroid_array.h"
#include "asteroid_preset.h"
#include "raygui.h"
#include "raygui_styles.h"
#define EPSILON 0.1f
const Asteroid* getAsteroid(const AsteroidArray* asteroidArray, const AsteroidSize *selection);
Vector2 getAsteroidSizePosition(AsteroidSize asteroidSize);
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

Vector2 getAsteroidSizePosition(const AsteroidSize asteroidSize) {
    switch (asteroidSize) {
        case SIZE_BIG: return BIG_ASTEROID_POS;
        case SIZE_MEDIUM: return MID_ASTEROID_POS;
        case SIZE_SMALL: return SML_ASTEROID_POS;
        default: printf("Unrecognized asteroidSize");
        exit(1);
    }
}




void refreshAsteroids(AsteroidArray *asteroidArray, const AsteroidPresetArray* presetArr) {
    ASTEROID_PRESETS_OrderArray(presetArr);
    ASTEROIDS_Purge(asteroidArray);

    for (int s = 0; s < SIZE_COUNT; s++) {
        const AsteroidPreset* preset = &presetArr->presets[s];
        Asteroid asteroid = ASTEROID_Create(preset);
        ASTEROID_MoveTo(&asteroid, getAsteroidSizePosition(preset->size));
        ASTEROIDS_Add(asteroidArray, asteroid);
    }
}

void loadThemes() {
    GUI_LoadStyle(GUI_STYLE_AMBER);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 48);
}

void resetAsteroidAttributes() {
    // reset preset
}

Rectangle MENU_GetAsteroidSquareHitBox(const Asteroid* asteroid, const AsteroidPresetArray* presetArr) {
    const float radius = asteroid->shape.radius + presetArr->presets[asteroid->type].spread;
    return (Rectangle) {
        asteroid->position.x - radius,
        asteroid->position.y - radius,
        radius * 2,
        radius * 2
    };
}