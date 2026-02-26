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
#include "preset_create_elements.h"
void openCreatorScreen(const Screen *currentScreen, GameContext *gameContext);
void closeCreatorScreen(const Screen *currentScreen, GameContext *gameContext);
void updateCreatorScreen(Screen *currentScreen, GameContext *gameContext);
void drawCreatorScreen(const Screen *currentScreen, const GameContext *gameContext);
void drawAsteroids(const AsteroidArray *asteroidArray);
Vector2 convertValueToText(float value, char* buffer, float fontSize, float spacing);
Vector2 getTextRecCenter(Rectangle rec, Vector2 textSize);
bool GUI_Slider(float *value, const char* sliderName, Rectangle box);
void clickSelectAsteroid(const AsteroidArray* asteroidArray, AsteroidSize *selection, const AsteroidPresetArray *presetArr);
void drawAstOptions(AsteroidPreset *preset, GameContext *context, AsteroidSize asteroidSize);
const Asteroid* getAsteroid(const AsteroidArray* asteroidArray, AsteroidSize selection);
void checkForUpdate(float oldValue, float newValue, bool *isModified);
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
    gameContext->screenContext.presetCreateCtx.isSizeDropdownOn = false;
    OVERLAY_STACK_Reset(&gameContext->screenContext.overlayStack);
}

void closeCreatorScreen(const Screen *currentScreen, GameContext *gameContext) {
    ASTEROIDS_FreeArray(gameContext->asteroidArray);
    BULLETS_FreeArray(gameContext->bulletArray);
    WAVE_CONTEXT_Free(gameContext->wave);
    particleArrDestroy();
    OVERLAY_STACK_Reset(&gameContext->screenContext.overlayStack);
}

void updateCreatorScreen(Screen *currentScreen, GameContext *gameContext) {
    bool *isModified = &gameContext->screenContext.presetCreateCtx.isModified;
    bool *dropdownToggle = &gameContext->screenContext.presetCreateCtx.isSizeDropdownOn;
    PresetCreateTextArea *textArea = &gameContext->screenContext.presetCreateCtx.currentTextArea;
    PresetCreateContext *presetCreateCtx = &gameContext->screenContext.presetCreateCtx;
    ASTEROIDS_Update(gameContext->asteroidArray);

    const AsteroidPresetArray *presetArray = gameContext->wave->presetArr;
    const Vector2 mousePos = GetMousePosition();
    drawBackground();

    if (GuiDropdownBox(SIZE_SELECTION_BOX(),
    "Big;Medium;Small", (int*) &presetCreateCtx->selectedAsteroidSize, *dropdownToggle)) {
        *dropdownToggle = !*dropdownToggle;
    }

    clickSelectAsteroid(gameContext->asteroidArray, &presetCreateCtx->selectedAsteroidSize, gameContext->wave->presetArr);

    drawAstOptions(&presetArray->presets[presetCreateCtx->selectedAsteroidSize], gameContext, presetCreateCtx->selectedAsteroidSize);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { //chose text box and let go of it
        if (CheckCollisionPointRec(mousePos, VERTICES_NB_BOX())) {
            *textArea = PRESET_CREATE_VERTICES_SPINNER;
        }
        else if (CheckCollisionPointRec(mousePos, PRESET_NAME_BOX())) {
            *textArea = PRESET_CREATE_PRESET_NAME;
        }else {
            *textArea = -1;
        }
    }
    if (IsKeyPressed(KEY_ENTER)) *textArea = -1;

    if (GuiButton(EXIT_BOX(), "Exit")) {
        *currentScreen = SCREEN_TITLE;
        *isModified = true;
    }
    if (GuiButton(RESET_BOX(), "Reset")) {
        resetAsteroidAttributes();
        *isModified = true;
    }
    if (GuiButton(SAVE_BOX(), "Save")) {
        savePreset(&presetArray->presets[presetCreateCtx->selectedAsteroidSize], presetCreateCtx->presetName);
        resetAsteroidAttributes();
        *isModified = true;
        presetCreateCtx->presetName[0] = '\0';
    }

    if (GuiButton(SAVE_ALL_BOX(), "Save All")) {
        for (int i = 0; i < presetArray->presetCount; ++i) {
            savePreset(&presetArray->presets[i], presetCreateCtx->presetName);
        }

        resetAsteroidAttributes();
        *isModified = true;
        presetCreateCtx->presetName[0] = '\0';
    }

    GuiToggle(GENERATE_BOX(), "Generate", isModified);
    const int MAX_LINE_BUFFER_SIZE = 256;
    GuiTextBox(PRESET_NAME_BOX(), presetCreateCtx->presetName, MAX_LINE_BUFFER_SIZE,
        *textArea == PRESET_CREATE_PRESET_NAME);

    if (*isModified) {
        refreshAsteroids(gameContext->asteroidArray, gameContext->wave->presetArr);
        *isModified = false;
    }
}

void drawAstOptions(AsteroidPreset *preset, GameContext *context, const AsteroidSize asteroidSize) {
    bool *isModified = &context->screenContext.presetCreateCtx.isModified;
    Asteroid asteroid;
    const Asteroid *astPtr = getAsteroid(context->asteroidArray, asteroidSize);
    if (astPtr == NULL) {
        asteroid = ASTEROID_GetZeroInitializedAsteroid();
    } else {
        asteroid = *astPtr;
    }


    if (preset->isRandomBothSides == true) {
        DrawRing(asteroid.position,preset->radius-preset->spread,
        preset->radius + preset->spread, 0, 360, 0, SPREAD_COLOR);
    } else {
        DrawRing(asteroid.position,preset->radius,
        preset->radius + preset->spread, 0, 360, 0, SPREAD_COLOR);
    }
    DrawRing(asteroid.position, preset->radius-1, preset->radius+1,
        0, 360, 0, RADIUS_COLOR);

    if (GUI_Slider(&preset->radius, "Radius", RADIUS_BOX())) {
        *isModified = true;
    }

    if (GUI_Slider(&preset->spread, "Spread", SPREAD_BOX())) {
        *isModified = true;
    }

    if (GUI_Slider(&preset->minRotationSpeed, "Min Turn Speed", MIN_ROTATION_BOX())) {
        *isModified = true;
    }

    if (GUI_Slider(&preset->maxRotationSpeed, "Max Turn Speed", MAX_ROTATION_BOX())) {
        *isModified = true;
    }

    if (GUI_Slider(&preset->maxSpeed, "Max Speed", MAX_SPEED_BOX())) {
        *isModified = true;
    }

    float temp = preset->isRandomBothSides;
    GuiToggle(GENERATION_STYLE_BOX(),"2 Side Rnd", &preset->isRandomBothSides);
    checkForUpdate(temp, preset->isRandomBothSides, isModified);

    temp = preset->nbVertices;
    GuiSpinner(NB_VERTICES_BOX(),
        "Vertices",&preset->nbVertices, 2, 10000, context->screenContext.presetCreateCtx.currentTextArea == PRESET_CREATE_VERTICES_SPINNER);
    if (preset->nbVertices < 0) preset->nbVertices = 0;//prevent crash
    checkForUpdate(temp ,preset->nbVertices, isModified);
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

void checkForUpdate(const float oldValue, const float newValue, bool *isModified) {
    if (fabsf(oldValue - newValue) > 0.1f) *isModified = true;
}

const Asteroid* getAsteroid(const AsteroidArray* asteroidArray, const AsteroidSize selection) {
    for (int i = 0; i < asteroidArray->nbAsteroid; ++i) {
        const Asteroid *asteroid = &asteroidArray->asteroid[i];
        if (asteroid->type == selection) {
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


Vector2 convertValueToText(const float value, char* buffer, const float fontSize, const float spacing) {
    sprintf(buffer, "[%.2f]", value);
    return MeasureTextEx(GetFontDefault(), buffer, fontSize, spacing);
}

Vector2 getTextRecCenter(const Rectangle rec, const Vector2 textSize) {
    return (Vector2){rec.x+rec.width/2-textSize.x/2, rec.y+rec.height/2-textSize.y/2};
}

bool GUI_Slider(float *value, const char* sliderName, const Rectangle box) {
    char buffer[50] = "";
    const float spacing = 1;
    const float fontSize = 32;
    const Vector2 textSize = convertValueToText(*value, buffer, fontSize, spacing);
    const float old = *value;
    GuiSliderBar(box, sliderName, "",value, 0, 250);
    if (CheckCollisionPointRec(GetMousePosition(), box)) {
        DrawTextEx(GetFontDefault(), buffer,
        getTextRecCenter(box, textSize),fontSize, spacing, TEXT_INDICATOR_COLOR);
    }
    return (fabsf(old - *value) > 0.1f);
}