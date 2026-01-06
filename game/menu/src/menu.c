#include "menu.h"

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "asteroid.h"
#include "files.h"
#include "particle.h"
#include "raygui.h"
#include "style_jungle.h"
#define DARKERGRAY (Color){20,20,20,255}
#define SPREAD_COLOR (Color){0,255,0,50}
#define RADIUS_COLOR (Color){0,255,0,150}
#define TEXT_INDICATOR_COLOR (Color){140,255,241,255}
#define RADIUS_SET_BOX              (Rectangle){GetScreenWidth()/8*7, GetScreenHeight()/8-70, 200, 50}
#define SPREAD_SET_BOX              (Rectangle){GetScreenWidth()/8*7, GetScreenHeight()/8, 200, 50}
#define MIN_ROTATION_SET_BOX        (Rectangle){GetScreenWidth()/8*7, GetScreenHeight()/8+70, 200, 50}
#define MAX_ROTATION_SET_BOX        (Rectangle){GetScreenWidth()/8*7, GetScreenHeight()/8+140, 200, 50}
#define MAX_SPEED_SET_BOX           (Rectangle){GetScreenWidth()/8*7, GetScreenHeight()/8+210, 200, 50}
#define NB_VERTICES_SET_BOX         (Rectangle){GetScreenWidth()/8*7, GetScreenHeight()/8+280, 200, 50}
#define GENERATION_STYLE_SET_BOX    (Rectangle){GetScreenWidth()/8*7, GetScreenHeight()/8+350, 200, 50}
#define BIG_ASTEROID_POS        (Vector2){GetScreenWidth()/4, GetScreenHeight()/4}
#define MID_ASTEROID_POS        (Vector2){GetScreenWidth()/4, GetScreenHeight()/8*5}
#define SML_ASTEROID_POS        (Vector2){GetScreenWidth()/4, GetScreenHeight()/8*7}
#define MAX_LINE_BUFFER_SIZE 256
#define EPSILON 0.1f
static bool isModified = true;
static char currentPresetName[MAX_LINE_BUFFER_SIZE];
enum ChosenTextBox {
    verticesSpinner,
    presetName,
    dropdownType
};
enum ChosenTextBox chosenTextBox = -1;

// title menu
// ----------------------------------------------------------------------------------------------------------------
Rectangle getAsteroidBox(const Asteroid* asteroid);
void clickSelectAsteroid(const AsteroidArray* asteroidArray, AsteroidSize *selection);
const Asteroid* getAsteroid(const AsteroidArray* asteroidArray, const AsteroidSize *selection);
void drawAstOptions(AsteroidPreset *preset, const Asteroid* asteroid);

Rectangle getStartGameBox() {
    return (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2, 350, 100};
}
Rectangle getEditAsteroidsModeBox() {
    return (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 150, 350, 100};
}
Rectangle getManagePresetsBox() {
    return (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 300, 350, 100};
}

void titleMenu() {
    Vector2 mousePos = GetMousePosition();
    int boxBorder = 10;
    Rectangle box;
    char *title = "Asteroids";
    int titleSizeX = MeasureText(title, 256);
    DrawText(title, (GetScreenWidth()-titleSizeX) / 2, (GetScreenHeight() / 4)-100, 256, WHITE);
    DrawText("By Julien Lamothe", (GetScreenWidth()-titleSizeX)/2 + titleSizeX-MeasureText("By Julien Lamothe", 20),
        (GetScreenHeight() / 4)+112, 20, ORANGE);

    box = getStartGameBox();
    if (CheckCollisionPointRec(mousePos, getStartGameBox())) {
        DrawRectangle(box.x, box.y, box.width, box.height, ORANGE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Start", box.x + (box.width-50) / 2, box.y + box.height / 2 - 10, 20, ORANGE);
    } else {
        DrawRectangle(box.x, box.y, box.width, box.height, WHITE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Start", box.x + (box.width-50) / 2,
                 box.y + box.height / 2 - 10, 20, WHITE);
    }

    box = getEditAsteroidsModeBox();
    if (CheckCollisionPointRec(mousePos, getEditAsteroidsModeBox())) {
        DrawRectangle(box.x, box.y, box.width, box.height, ORANGE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Mode", box.x + (box.width - 96.0) / 2, box.y + box.height / 2 - 10, 20, ORANGE);
    } else {
        DrawRectangle(box.x, box.y, box.width, box.height, WHITE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Mode", box.x + (box.width - 96.0) / 2,
                 box.y + box.height / 2 - 10, 20, WHITE);
    }

    box = getManagePresetsBox();
    if (CheckCollisionPointRec(mousePos, getManagePresetsBox())) {
        DrawRectangle(box.x, box.y, box.width, box.height, ORANGE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Presets", box.x + (box.width-120) / 2, box.y + box.height / 2 - 10, 20, ORANGE);
    } else {
        DrawRectangle(box.x, box.y, box.width, box.height, WHITE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Presets", box.x + (box.width-120) / 2,
                 box.y + box.height / 2 - 10, 20, WHITE);
    }

}

void titleMenuInput(bool *isTitleMenu, bool *isGame, bool *isAsteroidEditScreen, bool *editPresets) {
    const Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, getStartGameBox())) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            particleArrInit(10);
            *isTitleMenu = false;
            *isGame = true;
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, getEditAsteroidsModeBox())) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *isTitleMenu = false;
            *isAsteroidEditScreen = true;
        }
    }
    if (CheckCollisionPointRec(mousePos, getManagePresetsBox())) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            /*
            *isTitleMenu = false;
            *editPresets = true;
            */
        }
    }
    if (IsKeyPressed(KEY_SPACE)) {
        *isTitleMenu = false;
        *isGame = true;
    }
}


// managePresets
// ----------------------------------------------------------------------------------------------------------------

// edit asteroids
// ----------------------------------------------------------------------------------------------------------------

void editAsteroidMenu(const AsteroidArray *asteroidArray, AsteroidPresetArray *presetArray, bool *isTitleMenu, bool *isAsteroidEditScreen) {
    static AsteroidSize currentlyModifiedType = SIZE_BIG;
    static bool dropdownToggle = false;
    Vector2 mousePos = GetMousePosition();
    DrawRectangleV((Vector2){0,0},//show change section
        (Vector2){GetScreenWidth()/2,GetScreenHeight()},GRAY);
    DrawRectangleV((Vector2){10,10},
        (Vector2){GetScreenWidth()/2-20,GetScreenHeight()-20},BLACK);
    DrawRectangleV((Vector2){GetScreenWidth()/2,0},//edit section
        (Vector2){GetScreenWidth()/2,GetScreenHeight()},DARKERGRAY);

    if (GuiDropdownBox((Rectangle){GetScreenWidth()/2 + 10, GetScreenHeight()/8-70, 250, 75},
    "Big;Medium;Small", (int*) &currentlyModifiedType, dropdownToggle)) {
        dropdownToggle = !dropdownToggle;
    }

    clickSelectAsteroid(asteroidArray, &currentlyModifiedType);

    drawAstOptions(&presetArray->presets[currentlyModifiedType], getAsteroid(asteroidArray, &currentlyModifiedType));
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
        *isTitleMenu = true;
        *isAsteroidEditScreen = false;
        isModified = true;
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
        savePreset(&presetArray->presets[currentlyModifiedType], currentPresetName);
        resetAsteroidAttributes();
        isModified = true;
        currentPresetName[0] = '\0';
        }
    GuiToggle((Rectangle){GetScreenWidth()-270, GetScreenHeight()/8*7-240, 250, 100}, "Generate", &isModified);
    GuiTextBox((Rectangle){GetScreenWidth()/2+10, GetScreenHeight()/8*7, 500, 100}, currentPresetName, MAX_LINE_BUFFER_SIZE, chosenTextBox == presetName);
}

void checkForUpdate(float oldValue, float newValue) {
    if (fabs(oldValue - newValue) > 0.1f) isModified = true;
}

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
    if (presetArr->presetCount == ASTEROID_SIZE_COUNT) {
        Asteroid big = ASTEROID_Create(&presetArr->presets[SIZE_BIG]);
        big.position = BIG_ASTEROID_POS;
        ASTEROIDS_Add(asteroidArray, big);

        Asteroid mid = ASTEROID_Create(&presetArr->presets[SIZE_MEDIUM]);
        mid.position = MID_ASTEROID_POS;
        ASTEROIDS_Add(asteroidArray, mid);

        Asteroid small = ASTEROID_Create(&presetArr->presets[SIZE_SMALL]);
        small.position = SML_ASTEROID_POS;
        ASTEROIDS_Add(asteroidArray, small);
    }
}

void loadThemes() {
    GuiLoadStyleJungle();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 48);
}

void resetAsteroidAttributes() {
    // reset preset
}

void drawAstOptions(AsteroidPreset *preset, const Asteroid* asteroid) {
    Vector2 mousepos = GetMousePosition();
    const int fontsize = 32;
    const int spacing = 1;
    float temp;
    Vector2 textSize;
    if (preset->isRandomBothSides == true) {
        DrawRing(asteroid->position,(preset->radius-preset->spread),
        (preset->radius+preset->spread), 0, 360, 0, SPREAD_COLOR);
    } else {
        DrawRing(asteroid->position,preset->radius,
        (preset->radius+preset->spread), 0, 360, 0, SPREAD_COLOR);
    }

    DrawRing(asteroid->position, preset->radius-1, preset->radius+1,
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

Rectangle getAsteroidBox(const Asteroid* asteroid) {
    return (Rectangle) {
        asteroid->position.x - asteroid->shape.radius,
        asteroid->position.y - asteroid->shape.radius,
        asteroid->shape.radius * 2,
        asteroid->shape.radius * 2
    };
}

void clickSelectAsteroid(const AsteroidArray *asteroidArray, AsteroidSize *selection) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        const Vector2 mousePos = GetMousePosition();
        for (int i = 0; i < asteroidArray->nbAsteroid; ++i) {
            const Asteroid *asteroid = &asteroidArray->asteroid[i];
            if (CheckCollisionPointRec(mousePos,getAsteroidBox(asteroid))) {
                *selection = asteroid->type;
            }
        }
    }
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