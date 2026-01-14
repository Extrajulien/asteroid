#include <math.h>
#include "raygui.h"
#include <stdio.h>
#include <string.h>

#include "files.h"
#include "screen.h"
#include "screens.h"
#include "game_api.h"
#include "screen_virtual_table.h"
#include "menu.h"
void openCreatorScreen(const Screen *currentScreen, GameContext *gameContext);
void closeCreatorScreen(const Screen *currentScreen, GameContext *gameContext);
void updateCreatorScreen(Screen *currentScreen, GameContext *gameContext);
void drawCreatorScreen(const Screen *currentScreen, const GameContext *gameContext);
void drawAsteroids(const AsteroidArray *asteroidArray);

static bool isModified = true;
enum ChosenTextBox {
    verticesSpinner,
    presetName,
    dropdownType
};
enum ChosenTextBox chosenTextBox = -1;

void clickSelectAsteroid(const AsteroidArray* asteroidArray, AsteroidSize *selection, const AsteroidPresetArray *presetArr);
void drawAstOptions(AsteroidPreset *preset, const Asteroid* asteroidPtr);
const Asteroid* getAsteroid(const AsteroidArray* asteroidArray, const AsteroidSize *selection);
void checkForUpdate(float oldValue, float newValue);
void contextInit(PresetCreateContext *context);
void drawBackground();

ScreenVTable SCREENS_GetPresetCreateVTable() {
    return (ScreenVTable) {
        .screen = SCREEN_PRESET_CREATE,
        .open   = openCreatorScreen,
        .close  = closeCreatorScreen,
        .update = updateCreatorScreen,
        .draw   = drawCreatorScreen
    };
}

void openCreatorScreen(const Screen *currentScreen, GameContext *gameContext) {
    WaveContext *wave = WAVE_CONTEXT_Create();
    readPresetFile(wave->presetArr);
    particleArrInit(10);
    gameContext->asteroidArray = ASTEROIDS_CreateArray();
    gameContext->bulletArray = BULLETS_CreateArray();
    gameContext->wave = wave;
    refreshAsteroids(gameContext->asteroidArray, gameContext->wave->presetArr);

    contextInit(&gameContext->screenContext.presetCreateCtx);

}

void closeCreatorScreen(const Screen *currentScreen, GameContext *gameContext) {
    ASTEROIDS_FreeArray(gameContext->asteroidArray);
    BULLETS_FreeArray(gameContext->bulletArray);
    WAVE_CONTEXT_Free(gameContext->wave);
    particleArrDestroy();
}

void updateCreatorScreen(Screen *currentScreen, GameContext *gameContext) {
    PresetCreateContext *presetCreateCtx = &gameContext->screenContext.presetCreateCtx;

    ASTEROIDS_Update(gameContext->asteroidArray);


    AsteroidPresetArray* presetArray = gameContext->wave->presetArr;
    static bool dropdownToggle = false;
    Vector2 mousePos = GetMousePosition();

    drawBackground();


    if (GuiDropdownBox((Rectangle){GetScreenWidth()/2 + 10, GetScreenHeight()/8-70, 250, 75},
    "Big;Medium;Small", (int*) &presetCreateCtx->selectedAsteroidSize, dropdownToggle)) {
        dropdownToggle = !dropdownToggle;
    }

    clickSelectAsteroid(gameContext->asteroidArray, &presetCreateCtx->selectedAsteroidSize, gameContext->wave->presetArr);

    drawAstOptions(&presetArray->presets[presetCreateCtx->selectedAsteroidSize], getAsteroid(gameContext->asteroidArray, &presetCreateCtx->selectedAsteroidSize));
    //-----------------------------------------------------------------------------------------------------------------------
    //gui toggles
    //-----------------------------------------------------------------------------------------------------------------------

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {//chose text box and let go of it
        if (CheckCollisionPointRec(mousePos, NB_VERTICES_SET_BOX)) chosenTextBox = verticesSpinner;
        else if (CheckCollisionPointRec(mousePos, (Rectangle){GetScreenWidth()/2, GetScreenHeight()/8*7, 500, 100})) {
            chosenTextBox = presetName;
        }else {
            chosenTextBox = -1;
        }
    }


    if (IsKeyPressed(KEY_ENTER)) chosenTextBox = -1;

    GuiButton((Rectangle){GetScreenWidth()-270, GetScreenHeight()/8*7, 250, 100}, "Exit");
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(mousePos,(Rectangle){GetScreenWidth()-250, GetScreenHeight()/8*7, 200, 100})) {
        *currentScreen = SCREEN_TITLE;
        isModified = true;
        ASTEROID_PRESET_Purge(presetArray);
        readPresetFile(presetArray);
        }
    GuiButton((Rectangle){GetScreenWidth()-270, GetScreenHeight()/8*7-120, 250, 100}, "Reset");
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(mousePos, (Rectangle){GetScreenWidth()-250, GetScreenHeight()/8*7-120, 200, 100})) {
        resetAsteroidAttributes();
        isModified = true;
    }

    GuiButton((Rectangle){GetScreenWidth()/2+10, GetScreenHeight()/8*7-120, 250, 100}, "Save");
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(mousePos, (Rectangle){GetScreenWidth()/2+10, GetScreenHeight()/8*7-120, 250, 100})) {
        savePreset(&presetArray->presets[presetCreateCtx->selectedAsteroidSize], presetCreateCtx->presetName);
        resetAsteroidAttributes();
        isModified = true;
        presetCreateCtx->presetName[0] = '\0';
        }
    GuiToggle((Rectangle){GetScreenWidth()-270, GetScreenHeight()/8*7-240, 250, 100}, "Generate", &isModified);
    int MAX_LINE_BUFFER_SIZE = 256;
    GuiTextBox((Rectangle){GetScreenWidth()/2+10, GetScreenHeight()/8*7, 500, 100}, presetCreateCtx->presetName, MAX_LINE_BUFFER_SIZE, chosenTextBox == presetName);
}

void drawAstOptions(AsteroidPreset *preset, const Asteroid* asteroidPtr) {
    Vector2 mousepos = GetMousePosition();
    const int fontsize = 32;
    const int spacing = 1;
    float temp;
    Vector2 textSize;
    Asteroid asteroid;

    if (asteroidPtr == NULL) {
        asteroid = ASTEROID_GetZeroInitializedAsteroid();
    } else {
        asteroid = *asteroidPtr;
    }


    if (preset->isRandomBothSides == true) {
        DrawRing(asteroid.position,(preset->radius-preset->spread),
        (preset->radius+preset->spread), 0, 360, 0, SPREAD_COLOR);
    } else {
        DrawRing(asteroid.position,preset->radius,
        (preset->radius+preset->spread), 0, 360, 0, SPREAD_COLOR);
    }

    DrawRing(asteroid.position, preset->radius-1, preset->radius+1,
        0, 360, 0, RADIUS_COLOR);
    char radiusBig[15] = "";
    sprintf(radiusBig, "[%.2f]", preset->radius);
    textSize = MeasureTextEx(GetFontDefault(),radiusBig, fontsize, spacing);
    temp = preset->radius;
    GuiSliderBar(RADIUS_SET_BOX,
        "Radius", "",&preset->radius, 0, 250);
    if (CheckCollisionPointRec(mousepos, RADIUS_SET_BOX)) {
        DrawTextEx(GetFontDefault(), radiusBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8-70+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }

    checkForUpdate(temp, preset->radius);

    char spreadBig[15] = "";
    sprintf(spreadBig, "[%.2f]", preset->spread);
    textSize = MeasureTextEx(GetFontDefault(),spreadBig, fontsize, spacing);
    temp = preset->spread;
    GuiSliderBar(SPREAD_SET_BOX,
        "Spread", "",&preset->spread, 0, 250);
    if (CheckCollisionPointRec(mousepos, SPREAD_SET_BOX)) {
        DrawTextEx(GetFontDefault(), spreadBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }
    checkForUpdate(temp, preset->spread);

    char minRotationSpeedBig[15] = "";
    sprintf(minRotationSpeedBig, "[%.2f]", preset->minRotationSpeed);
    textSize = MeasureTextEx(GetFontDefault(),minRotationSpeedBig, fontsize, spacing);
    temp = preset->minRotationSpeed;
    GuiSliderBar(MIN_ROTATION_SET_BOX,
        "Min turn speed", "",&preset->minRotationSpeed, -1000, -1);
    if (CheckCollisionPointRec(mousepos, MIN_ROTATION_SET_BOX)) {
        DrawTextEx(GetFontDefault(), minRotationSpeedBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8+70+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }
    checkForUpdate(temp, preset->minRotationSpeed);

    char maxRotationSpeedBig[15] = "";
    sprintf(maxRotationSpeedBig, "[%.2f]", preset->maxRotationSpeed);
    textSize = MeasureTextEx(GetFontDefault(),maxRotationSpeedBig, fontsize, spacing);
    temp = preset->maxRotationSpeed;
    GuiSliderBar(MAX_ROTATION_SET_BOX,
        "Max turn speed", "",&preset->maxRotationSpeed, 1, 1000);
    if (CheckCollisionPointRec(mousepos, MAX_ROTATION_SET_BOX)) {
        DrawTextEx(GetFontDefault(), maxRotationSpeedBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8+140+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }
    checkForUpdate(temp, preset->maxRotationSpeed);

    char maxSpeedBig[15] = "";
    sprintf(maxSpeedBig, "[%.2f]", preset->maxSpeed);
    textSize = MeasureTextEx(GetFontDefault(),maxSpeedBig, fontsize, spacing);
    temp = preset->maxSpeed;
    GuiSliderBar(MAX_SPEED_SET_BOX,
        "Max speed", "",&preset->maxSpeed, 0, 300);
    if (CheckCollisionPointRec(mousepos, MAX_SPEED_SET_BOX)) {
        DrawTextEx(GetFontDefault(), maxSpeedBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8+210+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }
    checkForUpdate(temp, preset->maxSpeed);

    temp = preset->isRandomBothSides;
    GuiToggle(GENERATION_STYLE_SET_BOX,"2 Side", &preset->isRandomBothSides);
    checkForUpdate(temp, preset->isRandomBothSides);

    temp = preset->nbVertices;
    GuiSpinner(NB_VERTICES_SET_BOX,
        "Vertices",&preset->nbVertices, 2, 10000, chosenTextBox == verticesSpinner);
    if (preset->nbVertices < 0) preset->nbVertices = 0;//prevent crash
    checkForUpdate(temp ,preset->nbVertices);
}

void drawCreatorScreen(const Screen *currentScreen, const GameContext *gameContext) {
    ClearBackground(BLACK);
    drawAsteroids(gameContext->asteroidArray);
}





void clickSelectAsteroid(const AsteroidArray *asteroidArray, AsteroidSize *selection, const AsteroidPresetArray *presetArr) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        const Vector2 mousePos = GetMousePosition();
        for (int i = 0; i < asteroidArray->nbAsteroid; ++i) {
            const Asteroid *asteroid = &asteroidArray->asteroid[i];
            if (CheckCollisionPointRec(mousePos,MENU_GetAsteroidSquareHitBox(asteroid, presetArr))) {
                *selection = asteroid->type;
            }
        }
    }
}

void checkForUpdate(const float oldValue, const float newValue) {
    if (fabsf(oldValue - newValue) > 0.1f) isModified = true;
}

const Asteroid* getAsteroid(const AsteroidArray* asteroidArray, const AsteroidSize *selection) {
    for (int i = 0; i < asteroidArray->nbAsteroid; ++i) {
        const Asteroid *asteroid = &asteroidArray->asteroid[i];
        if (asteroid->type == *selection) {
            return asteroid;
        }
    }
    return NULL;
}

void drawBackground() {
    const Color asteroidContainerColor = BLACK;
    const Color borderColor = RAYWHITE;
    const Color backgroundColor = DARKERGRAY;

    const float screenWidth = (float) GetScreenWidth();
    const float screenHeight = (float) GetScreenHeight();

    DrawRectangleV((Vector2){0,0},
        (Vector2){screenWidth/2,screenHeight},
        borderColor);

    DrawRectangleV((Vector2){5,5},
        (Vector2){screenWidth/2-10,screenHeight-10},
        asteroidContainerColor);

    DrawRectangleV((Vector2){screenWidth/2,0},
        (Vector2){screenWidth/2,screenHeight},
        backgroundColor);
}



void contextInit(PresetCreateContext *context) {
    context->selectedAsteroidSize = SIZE_BIG;
    memcpy(context->presetName, "[PresetName]\0", 13);
}

void drawAsteroids(const AsteroidArray *asteroidArray) {
    ASTEROIDS_Render(asteroidArray);
}
