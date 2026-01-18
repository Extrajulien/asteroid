
#include <stdio.h>

#include "bullet.h"
#include "screen.h"
#include "screens.h"
#include "game_api.h"
#include "screen_virtual_table.h"
#include "player.h"
#include "player_rules.h"

#include "event_api.h"
#include "files.h"
#include "menu_style.h"
#include "raymath.h"


void openGameScreen(const Screen *currentScreen, GameContext *gameContext);
void closeGameScreen(const Screen *currentScreen, GameContext *gameContext);
void updateGameScreen(Screen *currentScreen, GameContext *gameContext);
void drawGameScreen(const Screen *currentScreen, const GameContext *gameContext);
void updateGame(Player *player, BulletArray *bulletArr, AsteroidArray *asteroidArray);

void drawGrid(int x, int y, int size, Player *player, bool hasVectorDisplay);

void drawEntitiesPos(Vector2 position, Player *player, BulletArray *bulletArray);




ScreenVTable SCREENS_GetGameVTable() {
    return (ScreenVTable) {
        .screen = SCREEN_GAME,
        .open   = openGameScreen,
        .close  = closeGameScreen,
        .update = updateGameScreen,
        .draw   = drawGameScreen
    };
}

static bool hasDebugMode = false;

void openGameScreen(const Screen *currentScreen, GameContext *gameContext) {
    WaveContext *wave = WAVE_CONTEXT_Create();
    readPresetFile(wave->presetArr);
    particleArrInit(10);
    gameContext->asteroidArray = ASTEROIDS_CreateArray();
    gameContext->bulletArray = BULLETS_CreateArray();
    gameContext->bulletHitEventQueue = ASTEROID_BULLET_HIT_EVENT_QUEUE_Create();
    gameContext->player = PLAYER_Create();
    gameContext->wave = wave;
    WAVE_SpawnAsteroids(gameContext->asteroidArray, gameContext->wave, PLAYER_GetExclusionCircle(gameContext->player));
}

void closeGameScreen(const Screen *currentScreen, GameContext *gameContext) {
    ASTEROIDS_FreeArray(gameContext->asteroidArray);
    BULLETS_FreeArray(gameContext->bulletArray);
    ASTEROID_BULLET_HIT_EVENT_QUEUE_Free(gameContext->bulletHitEventQueue);
    PLAYER_Free(gameContext->player);
    WAVE_CONTEXT_Free(gameContext->wave);
    particleArrDestroy();
}

void updateGameScreen(Screen *currentScreen, GameContext *gameContext) {
    AsteroidBulletHitEventSink bulletHitAsteroidEventSink = ASTEROID_BULLET_HIT_EVENT_QUEUE_GetSink(gameContext->bulletHitEventQueue);
    if (gameContext->asteroidArray->nbAsteroid == 0) {
        WAVE_SpawnAsteroids(gameContext->asteroidArray, gameContext->wave, PLAYER_GetExclusionCircle(gameContext->player));
    }

    if (IsKeyPressed(KEY_E)) hasDebugMode = !hasDebugMode;

    updateGame(gameContext->player, gameContext->bulletArray, gameContext->asteroidArray);

    ASTEROIDS_CollideBullets(gameContext->asteroidArray, gameContext->bulletArray, &bulletHitAsteroidEventSink);
    WAVE_ExplodeAsteroids(gameContext->asteroidArray, gameContext->wave, gameContext->bulletHitEventQueue);
    PLAYER_UpdateBulletHits(gameContext->bulletHitEventQueue, gameContext->player, gameContext->bulletArray);

    if (PLAYER_IsDead(gameContext->player)) {
        *currentScreen = SCREEN_GAME_OVER;
    }

    /*
    PackageCollisionPlayer packagePlayer = (PackageCollisionPlayer){gameContext->player, &bigAstArr, &midAstArr, &smlAstArr};
    checkCollisionAstPlayer(&packagePlayer);
    */

    if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_TAB)) {
        ASTEROIDS_Purge(gameContext->asteroidArray);
        BULLETS_Purge(gameContext->bulletArray);
        WAVE_SpawnAsteroids(gameContext->asteroidArray, gameContext->wave, PLAYER_GetExclusionCircle(gameContext->player));
        PLAYER_Reset(gameContext->player);
        if (IsKeyPressed(KEY_TAB)) {
            *currentScreen = SCREEN_TITLE;
        }
    }
    ASTEROID_BULLET_HIT_EVENT_QUEUE_Purge(gameContext->bulletHitEventQueue);
}

void drawGameScreen(const Screen *currentScreen, const GameContext *gameContext) {
    ClearBackground(BLACK);
    drawParticles();
    BULLETS_Render(gameContext->bulletArray);

    PLAYER_Draw(gameContext->player);
    ASTEROIDS_Render(gameContext->asteroidArray);

    char scoreText[100] = "";
    sprintf(scoreText,"%d", PLAYER_GetScore(gameContext->player));
    DrawText(scoreText, 10, 50, 32, WHITE);
    char livesText[100] = "";
    sprintf(livesText,"pv:%d", PLAYER_GetLifeCount(gameContext->player));
    DrawText(livesText, 10, 10, 40, ORANGE);


    char asteroidCountText[100] = "";
    sprintf(asteroidCountText,"%lu", gameContext->asteroidArray->nbAsteroid);
    DrawText(asteroidCountText, 10, 100, 32, RED);

    if (hasDebugMode) {

        char rotation[100] = "";
        sprintf(rotation, "%.2f", PLAYER_GetAngle(gameContext->player));
        char speedAmnt[100] = "";
        sprintf(speedAmnt, "%.2f", Vector2Length(PLAYER_GetSpeed(gameContext->player)));

        drawGrid(GetScreenWidth() - 200, 200, 300, gameContext->player, true);
        drawGrid(GetScreenWidth() / 2, GetScreenHeight() / 2, 4500, gameContext->player, false);
        DrawText(rotation, 30, 30, 20, LIGHTGRAY);
        DrawText(speedAmnt, 30, 50, 20, LIGHTGRAY);
        DrawFPS(30, 80);
        drawEntitiesPos((Vector2){GetScreenWidth() - 800, 100}, gameContext->player, gameContext->bulletArray);
    }
}


void updateGame(Player *player, BulletArray *bulletArr, AsteroidArray *asteroidArray) {
    PLAYER_Update(player, bulletArr);
    BULLETS_Update(bulletArr);
    ASTEROIDS_Update(asteroidArray);
    moveParticles();
}


//debug draw should be its own screen probably
void drawGrid(int x, int y, int size, Player *player, bool hasVectorDisplay) {
    Vector2 topLeftCorner = {x - size / 2, y - size / 2};
    int nbHorizontalLine = 9;
    int nbVerticalLine = 9;


    float distanceVerticalLine = size / nbVerticalLine;
    float distanceHorizontalLine = size / nbHorizontalLine;
    for (int i = 0; i < nbHorizontalLine; i++) {
        DrawLine(topLeftCorner.x, (topLeftCorner.y + distanceHorizontalLine * i + distanceHorizontalLine / 2),
                 topLeftCorner.x + size, (topLeftCorner.y + distanceHorizontalLine * i + distanceHorizontalLine / 2),
                 TWHITE);
    }
    for (int i = 0; i < nbVerticalLine; i++) {
        DrawLine((topLeftCorner.x + distanceVerticalLine * i + distanceVerticalLine / 2), topLeftCorner.y,
                 (topLeftCorner.x + distanceVerticalLine * i + distanceVerticalLine / 2), topLeftCorner.y + size,
                 TWHITE);
    }
    if (hasVectorDisplay) {
        const Vector2 speed = PLAYER_GetSpeed(player);
        const float horizontalVector = x + speed.x * 10;
        const float verticalVector = y - speed.y * 10;
        const Vector2 startVector = {x, y};
        const Vector2 endVector = {horizontalVector, verticalVector};
        DrawLineEx(startVector, endVector, 4.0f, RED);
    }
}

void drawEntitiesPos(Vector2 position, Player *player, BulletArray *bulletArray) {
    int scale = 7;
    Vector2 topLeft = {position.x, position.y};
    const Vector2 playerPos = PLAYER_GetPosition(player);
    DrawRectangle(position.x, position.y, GetScreenWidth() / scale, GetScreenHeight() / scale, WHITE);
    DrawRectangleLines(position.x, position.y, GetScreenWidth() / scale, GetScreenHeight() / scale, GRAY);



    DrawRectangle(playerPos.x / scale + topLeft.x, playerPos.y / scale + topLeft.y, 10, 10, GREEN);


    for (int i = 0; i < bulletArray->count; ++i) {
        Rectangle bullet = BULLET_GetCollisionRectangle(BULLETS_GetBullet(bulletArray, i));
        Vector2 size = {bullet.width / scale, bullet.height / scale};
        if (size.x < 3) {
            size.x = 3;
        }
        if (size.y < 3) {
            size.y = 3;
        }
        DrawRectangle(bullet.x / scale + topLeft.x, bullet.y / scale + topLeft.y, size.x,
                      size.y, BLACK);
    }
}

