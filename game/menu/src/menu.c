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
        Asteroid asteroid = ASTEROID_Create(preset, asteroidArray->verticePool);
        ASTEROID_MoveTo(&asteroid, getAsteroidSizePosition(preset->size), asteroidArray->verticePool);
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
    const float radius = asteroid->radius + presetArr->presets[asteroid->type].spread;
    return (Rectangle) {
        asteroid->position.x - radius,
        asteroid->position.y - radius,
        radius * 2,
        radius * 2
    };
}