//
// Created by julie on 2025-04-27.
//

#include "menu.h"

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "asteroids.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define DARKERGRAY (Color){20,20,20,255}

static AsteroidTraits bigTraits = {BIG,100,20,-30,30,20, 25};
static AsteroidTraits midTraits = {MEDIUM,65,25,-40,40,50, 18};
static AsteroidTraits smlTraits = {SMALL,35,15,-50,50,100, 12};

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
        DrawRectangle(box.x, box.y, box.width, box.height, GRAY);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Start", box.x + (box.width-50) / 2, box.y + box.height / 2 - 10, 20, GRAY);
    } else {
        DrawRectangle(box.x, box.y, box.width, box.height, WHITE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Start", box.x + (box.width-50) / 2,
                 box.y + box.height / 2 - 10, 20, WHITE);
    }

    box = getEditAsteroidsModeBox();
    if (CheckCollisionPointRec(mousePos, getEditAsteroidsModeBox())) {
        DrawRectangle(box.x, box.y, box.width, box.height, GRAY);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Mode", box.x + (box.width - 96.0) / 2, box.y + box.height / 2 - 10, 20, GRAY);
    } else {
        DrawRectangle(box.x, box.y, box.width, box.height, WHITE);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Mode", box.x + (box.width - 96.0) / 2,
                 box.y + box.height / 2 - 10, 20, WHITE);
    }

    box = getManagePresetsBox();
    if (CheckCollisionPointRec(mousePos, getManagePresetsBox())) {
        DrawRectangle(box.x, box.y, box.width, box.height, GRAY);
        DrawRectangle(box.x + boxBorder / 2, box.y + boxBorder / 2, box.width - boxBorder,
                      box.height - boxBorder, BLACK);
        DrawText("Edit Presets", box.x + (box.width-120) / 2, box.y + box.height / 2 - 10, 20, GRAY);
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
            *isTitleMenu = false;
            *editPresets = true;
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

void editAsteroidMenu() {
    DrawRectangleV((Vector2){0,0},//show change section
        (Vector2){GetScreenWidth()/2,GetScreenHeight()},GRAY);
    DrawRectangleV((Vector2){10,10},
        (Vector2){GetScreenWidth()/2-20,GetScreenHeight()-20},BLACK);
    DrawRectangleV((Vector2){GetScreenWidth()/2,0},//edit section
        (Vector2){GetScreenWidth()/2,GetScreenHeight()},DARKERGRAY);
}

void updateEditAsteroidMenu(AsteroidArray *bigArr, AsteroidArray *midArr, AsteroidArray *smlArr) {
    static bool isModified = true;
    if (IsKeyPressed(KEY_F)) isModified = true;

    if (isModified) {
        printf("%d", ((BigAsteroid*)bigArr->asteroid[0])->base.type);
        //printf("%d", ((MidAsteroid*)midArr->asteroid[0])->base.type);
        //printf("%d", ((SmlAsteroid*)smlArr->asteroid[0])->base.type);
        freeAsteroidArray(bigArr, ((BigAsteroid*)bigArr->asteroid[0])->base.type);
        //freeAsteroidArray(midArr, ((MidAsteroid*)midArr->asteroid)->base.type);
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
    asteroids -> size = 1;
    asteroids -> asteroid = malloc(sizeof(void *));//init asteroid arr

        asteroids -> asteroid[0] = malloc(sizeof(BigAsteroid));  // Allocate memory for each asteroid

        const float rotation = ((rand() % ((bigTraits.maxRotationSpeed-10) - bigTraits.minRotationSpeed))
            + bigTraits.minRotationSpeed)/50.0f;// -300 to 300

        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.radius = bigTraits.radius;      //init radius
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.spread = bigTraits.spread;      //init spread
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.type = BIG;           //init type
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.angle = 0;            //init angle
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.score = bigTraits.score;           //init score
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.rotation = rotation;  //init rotation
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.position = (Vector2){GetScreenWidth()/4, GetScreenHeight()/4};     //init pos
        ((BigAsteroid*)asteroids -> asteroid[0]) -> base.speed = (Vector2){0,0};     //init speed

        generateVertices(asteroids -> asteroid[0], bigTraits.nbVertices);//init points

    printf("--Big asteroids generated!\n");
}

void createMidAsteroidEditMode(AsteroidArray *asteroids) {
    asteroids -> size = 1;
    asteroids -> asteroid = malloc(sizeof(void *));//init asteroid arr

    asteroids -> asteroid[0] = malloc(sizeof(MidAsteroid));  // Allocate memory for each asteroid

    const float rotation = ((rand() % ((midTraits.maxRotationSpeed-10) - midTraits.minRotationSpeed))
        + midTraits.minRotationSpeed)/50.0f;// -300 to 300

    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.radius = midTraits.radius;      //init radius
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.spread = midTraits.spread;      //init spread
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.type = MEDIUM;           //init type
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.angle = 0;            //init angle
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.score = midTraits.score;           //init score
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.rotation = rotation;  //init rotation
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.position = (Vector2){GetScreenWidth()/4, GetScreenHeight()/8*5};     //init pos
    ((MidAsteroid*)asteroids -> asteroid[0]) -> base.speed = (Vector2){0,0};     //init speed
    ((MidAsteroid*)asteroids -> asteroid[0]) ->nbVertices = midTraits.nbVertices;     //set nb vertices

    generateVertices(asteroids -> asteroid[0], midTraits.nbVertices);//init points

    printf("--Mid asteroids generated!\n");
}

void createSmlAsteroidEditMode(AsteroidArray *asteroids) {
    asteroids -> size = 1;
    asteroids -> asteroid = malloc(sizeof(void *));//init asteroid arr

    asteroids -> asteroid[0] = malloc(sizeof(SmlAsteroid));  // Allocate memory for each asteroid

    const float rotation = ((rand() % ((smlTraits.maxRotationSpeed-10) - smlTraits.minRotationSpeed))
        + smlTraits.minRotationSpeed)/50.0f;

    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.radius = smlTraits.radius;     //init radius
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.spread = smlTraits.spread;     //init spread
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.type = SMALL;                  //init type
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.angle = 0;                     //init angle
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.score = smlTraits.score;       //init score
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.rotation = rotation;  //init rotation
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.position = (Vector2){GetScreenWidth()/4, GetScreenHeight()/8*7};     //init pos
    ((SmlAsteroid*)asteroids -> asteroid[0]) -> base.speed = (Vector2){0,0};   //init speed

    generateVertices(asteroids -> asteroid[0], smlTraits.nbVertices);//init points

    printf("--Sml asteroids generated!\n");
}