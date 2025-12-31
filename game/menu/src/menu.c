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
extern AsteroidTraits bigTraits;
extern AsteroidTraits midTraits;
extern AsteroidTraits smlTraits;
static bool isModified = true;
static char currentPresetName[MAX_LINE_BUFFER_SIZE];
static int currentlyModifiedType = BIG;
enum ChosenTextBox {
    verticesSpinner,
    presetName,
    dropdownType
};
enum ChosenTextBox chosenTextBox = -1;

// title menu
// ----------------------------------------------------------------------------------------------------------------

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

void editAsteroidMenu(bool *isTitleMenu, bool *isAsteroidEditScreen) {
    static bool dropdownToggle = false;
    Vector2 mousePos = GetMousePosition();
    DrawRectangleV((Vector2){0,0},//show change section
        (Vector2){GetScreenWidth()/2,GetScreenHeight()},GRAY);
    DrawRectangleV((Vector2){10,10},
        (Vector2){GetScreenWidth()/2-20,GetScreenHeight()-20},BLACK);
    DrawRectangleV((Vector2){GetScreenWidth()/2,0},//edit section
        (Vector2){GetScreenWidth()/2,GetScreenHeight()},DARKERGRAY);

    if (GuiDropdownBox((Rectangle){GetScreenWidth()/2 + 10, GetScreenHeight()/8-70, 250, 75},
    "Big;Medium;Small", &currentlyModifiedType, dropdownToggle)) {
        dropdownToggle = !dropdownToggle;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(mousePos,getAsteroidBox(&bigTraits, BIG_ASTEROID_POS))) {
        currentlyModifiedType = BIG;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(mousePos,getAsteroidBox(&midTraits, MID_ASTEROID_POS))) {
        currentlyModifiedType = MEDIUM;
        }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
    && CheckCollisionPointRec(mousePos,getAsteroidBox(&smlTraits, SML_ASTEROID_POS))) {
        currentlyModifiedType = SMALL;
    }

    switch (currentlyModifiedType) {
        case SMALL:
            drawAstOptions(&smlTraits, SML_ASTEROID_POS);
            break;
        case MEDIUM:
            drawAstOptions(&midTraits, MID_ASTEROID_POS);
            break;
        case BIG:
            drawAstOptions(&bigTraits, BIG_ASTEROID_POS);
            break;
        default:
            printf("Unknown Asteroid type in menu!\n");
        exit(1);
    }


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
        readPresetFile();
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
        savePreset(currentPresetName);
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

void updateEditAsteroidMenu(AsteroidArray *bigArr, AsteroidArray *midArr, AsteroidArray *smlArr) {
    if (IsKeyPressed(KEY_F)) isModified = true;

    if (isModified) {
        printf("%f", ((BigAsteroid*)bigArr->asteroid[0])->base.radius);
        //printf("%d", ((MidAsteroid*)midArr->asteroid[0])->base.type);
        //printf("%d", ((SmlAsteroid*)smlArr->asteroid[0])->base.type);
        freeAsteroidArray(bigArr, BIG);
        freeAsteroidArray(midArr, MEDIUM);
        //freeAsteroidArray(smlArr, ((SmlAsteroid*)smlArr->asteroid)->base.type);

        refreshAsteroids(bigArr, midArr, smlArr);
        isModified = false;
    }
    rotateAsteroid(bigArr, BIG);
    rotateAsteroid(midArr, MEDIUM);
    rotateAsteroid(smlArr, SMALL);
}

void refreshAsteroids(AsteroidArray *bigArr, AsteroidArray *midArr, AsteroidArray *smlArr) {
    createBigAsteroidEditMode(bigArr);
    createMidAsteroidEditMode(midArr);
    createSmlAsteroidEditMode(smlArr);
}

void createBigAsteroidEditMode(AsteroidArray *asteroids) {
    asteroids -> reservedSize = 1;
    asteroids -> asteroid = malloc(sizeof(void *));//init asteroid arr

        asteroids -> asteroid[0] = malloc(sizeof(BigAsteroid));  // Allocate memory for each asteroid

    const float rotation = ((rand() % (int)((bigTraits.maxRotationSpeed) - bigTraits.minRotationSpeed))
        + bigTraits.minRotationSpeed)/100.0f;
        ((BigAsteroid *) asteroids->asteroid[0])->base.isCollisionEnabled = true;
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.radius = bigTraits.radius;      //init radius
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.spread = bigTraits.spread;      //init spread
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.type = BIG;           //init type
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.angle = 0;            //init angle
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.score = bigTraits.score;           //init score
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.rotation = rotation;  //init rotation
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.position = BIG_ASTEROID_POS;     //init pos
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.speed = (Vector2){0,0};     //init speed

        generateVertices(asteroids -> asteroid[0], bigTraits.nbVertices, bigTraits.generationStyle);//init points

    printf("--Big asteroids generated!\n");
}

void createMidAsteroidEditMode(AsteroidArray *asteroids) {
    asteroids -> reservedSize = 1;
    asteroids -> asteroid = malloc(sizeof(void *));//init asteroid arr

    asteroids -> asteroid[0] = malloc(sizeof(MidAsteroid));  // Allocate memory for each asteroid

    const float rotation = ((rand() % (int)((midTraits.maxRotationSpeed) - midTraits.minRotationSpeed))
        + midTraits.minRotationSpeed)/100.0f;
    ((MidAsteroid *) asteroids->asteroid[0])->base.isCollisionEnabled = true;
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.radius = midTraits.radius;     //init radius
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.spread = midTraits.spread;     //init spread
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.type = MEDIUM;                 //init type
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.angle = 0;                     //init angle
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.score = midTraits.score;       //init score
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.rotation = rotation;           //init rotation
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.position = MID_ASTEROID_POS;   //init pos
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.speed = (Vector2){0,0};   //init speed
    ((MidAsteroid*)asteroids -> asteroid[0]) -> nbVertices = midTraits.nbVertices;  //set nb vertices

    generateVertices(asteroids -> asteroid[0], midTraits.nbVertices, midTraits.generationStyle);//init points

    printf("--Mid asteroids generated!\n");
}

void createSmlAsteroidEditMode(AsteroidArray *asteroids) {
    asteroids -> reservedSize = 1;
    asteroids -> asteroid = malloc(sizeof(void *));//init asteroid arr

    asteroids -> asteroid[0] = malloc(sizeof(SmlAsteroid));  // Allocate memory for each asteroid

    const float rotation = ((rand() % (int)((smlTraits.maxRotationSpeed) - smlTraits.minRotationSpeed))
        + smlTraits.minRotationSpeed)/100.0f;

    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.isCollisionEnabled = true;
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.radius = smlTraits.radius;     //init radius
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.spread = smlTraits.spread;     //init spread
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.type = MEDIUM;                 //init type
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.angle = 0;                     //init angle
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.score = smlTraits.score;       //init score
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.rotation = rotation;           //init rotation
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.position = SML_ASTEROID_POS;   //init pos
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.speed = (Vector2){0,0};   //init speed
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> nbVertices = smlTraits.nbVertices;  //set nb vertices

    generateVertices(asteroids -> asteroid[0], smlTraits.nbVertices, smlTraits.generationStyle);//init points

    printf("--Sml asteroids generated!\n");
}

void loadThemes() {
    GuiLoadStyleJungle();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 48);
}

void resetAsteroidAttributes() {
    bigTraits = (AsteroidTraits){BIG,100,20,-30,30, 100,20, 25, 0};
    midTraits = (AsteroidTraits){MEDIUM,65,25,-40,40, 100,50, 18, 0};
    smlTraits = (AsteroidTraits){SMALL,35,15,-50,50, 100, 100, 12, 0};
}

void drawAstOptions(AsteroidTraits *traits, Vector2 asteroidPos) {
    Vector2 mousepos = GetMousePosition();
    const int fontsize = 32;
    const int spacing = 1;
    float temp;
    Vector2 textSize;
    if (traits->generationStyle == true) {
        DrawRing(asteroidPos,(traits->radius-traits->spread),
        (traits->radius+traits->spread), 0, 360, 0, SPREAD_COLOR);
    } else {
        DrawRing(asteroidPos,traits->radius,
        (traits->radius+traits->spread), 0, 360, 0, SPREAD_COLOR);
    }

    DrawRing(asteroidPos, traits->radius-1, traits->radius+1,
        0, 360, 0, RADIUS_COLOR);
    char radiusBig[15] = "";
    sprintf(radiusBig, "[%.2f]", traits->radius);
    textSize = MeasureTextEx(GetFontDefault(),radiusBig, fontsize, spacing);
    temp = traits->radius;
    GuiSliderBar(RADIUS_SET_BOX,
        "Radius", "",&traits->radius, 0, 250);
    if (CheckCollisionPointRec(mousepos, RADIUS_SET_BOX)) {
        DrawTextEx(GetFontDefault(), radiusBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8-70+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }

    checkForUpdate(temp, traits->radius);

    char spreadBig[15] = "";
    sprintf(spreadBig, "[%.2f]", traits->spread);
    textSize = MeasureTextEx(GetFontDefault(),spreadBig, fontsize, spacing);
    temp = traits->spread;
    GuiSliderBar(SPREAD_SET_BOX,
        "Spread", "",&traits->spread, 0, 250);
    if (CheckCollisionPointRec(mousepos, SPREAD_SET_BOX)) {
        DrawTextEx(GetFontDefault(), spreadBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }
    checkForUpdate(temp, traits->spread);

    char minRotationSpeedBig[15] = "";
    sprintf(minRotationSpeedBig, "[%.2f]", traits->minRotationSpeed);
    textSize = MeasureTextEx(GetFontDefault(),minRotationSpeedBig, fontsize, spacing);
    temp = traits->minRotationSpeed;
    GuiSliderBar(MIN_ROTATION_SET_BOX,
        "Min turn speed", "",&traits->minRotationSpeed, -1000, -1);
    if (CheckCollisionPointRec(mousepos, MIN_ROTATION_SET_BOX)) {
        DrawTextEx(GetFontDefault(), minRotationSpeedBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8+70+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }
    checkForUpdate(temp, traits->minRotationSpeed);

    char maxRotationSpeedBig[15] = "";
    sprintf(maxRotationSpeedBig, "[%.2f]", traits->maxRotationSpeed);
    textSize = MeasureTextEx(GetFontDefault(),maxRotationSpeedBig, fontsize, spacing);
    temp = traits->maxRotationSpeed;
    GuiSliderBar(MAX_ROTATION_SET_BOX,
        "Max turn speed", "",&traits->maxRotationSpeed, 1, 1000);
    if (CheckCollisionPointRec(mousepos, MAX_ROTATION_SET_BOX)) {
        DrawTextEx(GetFontDefault(), maxRotationSpeedBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8+140+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }
    checkForUpdate(temp, traits->maxRotationSpeed);

    char maxSpeedBig[15] = "";
    sprintf(maxSpeedBig, "[%.2f]", traits->maxSpeed);
    textSize = MeasureTextEx(GetFontDefault(),maxSpeedBig, fontsize, spacing);
    temp = traits->maxSpeed;
    GuiSliderBar(MAX_SPEED_SET_BOX,
        "Max speed", "",&traits->maxSpeed, 0, 300);
    if (CheckCollisionPointRec(mousepos, MAX_SPEED_SET_BOX)) {
        DrawTextEx(GetFontDefault(), maxSpeedBig,
        (Vector2){GetScreenWidth()/8*7+200/2-textSize.x/2, GetScreenHeight()/8+210+50/2-textSize.y/2},fontsize, spacing, TEXT_INDICATOR_COLOR);
    }
    checkForUpdate(temp, traits->maxSpeed);

    temp = traits->generationStyle;
    GuiToggle(GENERATION_STYLE_SET_BOX,"2 Side", &traits->generationStyle);
    checkForUpdate(temp, traits->generationStyle);

    temp = traits->nbVertices;
    GuiSpinner(NB_VERTICES_SET_BOX,
        "Vertices",&traits->nbVertices, 2, 10000, chosenTextBox == verticesSpinner);
    if (traits->nbVertices < 0) traits->nbVertices = 0;//prevent crash
    checkForUpdate(temp ,traits->nbVertices);
}

Rectangle getAsteroidBox(AsteroidTraits *traits, Vector2 pos) {
    return (Rectangle){pos.x-traits->radius, pos.y-traits->radius,
            traits->radius*2, traits->radius*2};
}
