#define TWHITE (Color){255,255,255,150}

#include <limits.h>

#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "include/game_api.h"
#include "asteroid.h"
#include "menu.h"
#include "particle.h"
#include "files.h"
#include "game_math.h"
#include "screen.h"

#include "player.h"
#include "raymath.h"
#include "wave.h"

void drawGrid(int x, int y, int size, Player *player, bool hasVectorDisplay);

void drawEntitiesPos(Vector2 position, Player *player, Bullet *bullet);

void renderBullets(Bullet *bullet);

void moveBullets(Bullet *bullets);

void initBullets(Bullet *bullets);

void deleteBullet(Bullet *bullet, int index);

void wrapAroundBullet(Bullet *bullets);

void updateGame(Player *player, Bullet *bulletArr, AsteroidArray *asteroidArray);

void showGameoverScreen(Screen *currentScreen);

int StartAsteroidGame() {
    Screen currentScreen = SCREEN_TITLE;
    bool hasDebugMode = false;

    Bullet bullets[PLAYER_MAX_BULLETS];
    AsteroidArray *asteroidArray = ASTEROIDS_CreateArray();
    WaveContext wave = WAVE_CONTEXT_Create();
    srand((unsigned int)time(NULL));

    int waveNumber = 0;
    readPresetFile(wave.presetArr);
    particleArrInit(10);
    InitWindow(1920, 1080, "Asteroids");
    initBullets(bullets);

    Player* player = PLAYER_Create();
    AsteroidBulletHitEventQueue *bulletHitEventQueue = ASTEROID_BULLET_HIT_QUEUE_Create();
    SetTargetFPS(MAX_FPS);
    loadThemes();


    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        //title menu
        if (currentScreen == SCREEN_TITLE) {
            titleMenuInput(&currentScreen);
            if (currentScreen == SCREEN_GAME) {
                PLAYER_Reset(player);
                WAVE_SpawnAsteroids(asteroidArray, &wave, PLAYER_GetExclusionCircle(player));
            }
        }

        if (currentScreen == SCREEN_PRESET_CREATE) {
            refreshAsteroids(asteroidArray, wave.presetArr);
        }

        //game
        if (currentScreen == SCREEN_GAME) {
            if (asteroidArray->nbAsteroid == 0) {
                WAVE_SpawnAsteroids(asteroidArray, &wave, PLAYER_GetExclusionCircle(player));
            }
            if (IsKeyPressed(KEY_E)) hasDebugMode = !hasDebugMode;

            updateGame(player, bullets, asteroidArray);

            ASTEROIDS_CollideBullets(asteroidArray, bullets, bulletHitEventQueue);
            WAVE_ExplodeAsteroids(asteroidArray, &wave, bulletHitEventQueue);
            PLAYER_UpdateBulletHits(bulletHitEventQueue, player, bullets);

            if (PLAYER_IsDead(player)) {
                currentScreen = SCREEN_GAME_OVER;
            }

            /*
            PackageCollisionPlayer packagePlayer = (PackageCollisionPlayer){player, &bigAstArr, &midAstArr, &smlAstArr};
            checkCollisionAstPlayer(&packagePlayer);
            */

            if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_TAB)) {
                ASTEROIDS_Purge(asteroidArray);
                initBullets(bullets);
                WAVE_SpawnAsteroids(asteroidArray, &wave, PLAYER_GetExclusionCircle(player));
                PLAYER_Reset(player);
                if (IsKeyPressed(KEY_TAB)) {
                    particleArrDestroy();
                    currentScreen = SCREEN_TITLE;
                }
            }
            ASTEROID_BULLET_HIT_QUEUE_Purge(bulletHitEventQueue);
        }

        if (currentScreen == SCREEN_GAME_OVER) {
            ClearBackground(BLACK);
            updateGame(player, bullets, asteroidArray);
        }

        {
            BeginDrawing();
            char rotation[100] = "";
            sprintf(rotation, "%.2f", PLAYER_GetAngle(player));
            char speedAmnt[100] = "";

            if (currentScreen == SCREEN_GAME_OVER) {
                drawParticles();
                ASTEROIDS_Render(asteroidArray);
                showGameoverScreen(&currentScreen);
            }

            if (currentScreen == SCREEN_GAME) {
                sprintf(speedAmnt, "%.2f", Vector2Length(PLAYER_GetSpeed(player)));
                ClearBackground(BLACK);
                drawParticles();
                renderBullets(bullets);

                PLAYER_Draw(player);
                ASTEROIDS_Render(asteroidArray);

                char scoreText[100] = "";
                sprintf(scoreText,"%d", PLAYER_GetScore(player));
                DrawText(scoreText, 10, 50, 32, WHITE);
                char livesText[100] = "";
                sprintf(livesText,"pv:%d", PLAYER_GetLifeCount(player));
                DrawText(livesText, 10, 10, 40, ORANGE);

                if (hasDebugMode) {
                    drawGrid(GetScreenWidth() - 200, 200, 300, player, true);
                    drawGrid(GetScreenWidth() / 2, GetScreenHeight() / 2, 4500, player, false);
                    DrawText(rotation, 30, 30, 20, LIGHTGRAY);
                    DrawText(speedAmnt, 30, 50, 20, LIGHTGRAY);
                    DrawFPS(30, 80);
                    drawEntitiesPos((Vector2){GetScreenWidth() - 800, 100}, player, bullets);
                }
            }
            if (currentScreen == SCREEN_TITLE) {
                ClearBackground(BLACK);
                titleMenu();
            }
            if (currentScreen == SCREEN_PRESET_CREATE) {
                ClearBackground(BLACK);
                editAsteroidMenu(asteroidArray, wave.presetArr, &currentScreen);
                ASTEROIDS_Render(asteroidArray);
                if (currentScreen == SCREEN_TITLE) {
                    refreshAsteroids(asteroidArray, wave.presetArr);
                }
            }
            EndDrawing();
        }
    }
    CloseWindow();

    ASTEROIDS_FreeArray(asteroidArray);
    PLAYER_Free(player);
    return 0;
}

void updateGame(Player *player, Bullet *bulletArr, AsteroidArray *asteroidArray) {
    PLAYER_Update(player, bulletArr);
    moveBullets(bulletArr);
    ASTEROIDS_Update(asteroidArray);
    moveParticles();
    wrapAroundBullet(bulletArr);
}

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

void drawEntitiesPos(Vector2 position, Player *player, Bullet *bullet) {
    int scale = 7;
    Vector2 topLeft = {position.x, position.y};
    const Vector2 playerPos = PLAYER_GetPosition(player);
    DrawRectangle(position.x, position.y, GetScreenWidth() / scale, GetScreenHeight() / scale, WHITE);
    DrawRectangleLines(position.x, position.y, GetScreenWidth() / scale, GetScreenHeight() / scale, GRAY);



    DrawRectangle(playerPos.x / scale + topLeft.x, playerPos.y / scale + topLeft.y, 10, 10, GREEN);


    for (int i = 0; i < PLAYER_MAX_BULLETS; ++i) {
        Vector2 size = {bullet[i].size.x / scale, bullet[i].size.y / scale};
        if (size.x < 3) {
            size.x = 3;
        }
        if (size.y < 3) {
            size.y = 3;
        }
        DrawRectangle(bullet[i].position.x / scale + topLeft.x, bullet[i].position.y / scale + topLeft.y, size.x,
                      size.y, BLACK);
    }
}

void renderBullets(Bullet *bullet) {
    for (int i = 0; i < PLAYER_MAX_BULLETS; i++) {
        if (bullet[i].distance > 0) {
            DrawRectangle((int) bullet[i].position.x, (int) bullet[i].position.y, (int) bullet[i].size.x,
                          (int) bullet[i].size.y, WHITE);
        }
    }
}

void moveBullets(Bullet *bullets) {
    for (int i = 0; i < PLAYER_MAX_BULLETS; ++i) {
        if (bullets[i].distance == 0) continue;
        float speedX = bullets[i].speed.x * GetFrameTime() * 120;
        float speedY = bullets[i].speed.y * GetFrameTime() * 120;
        bullets[i].position.x += speedX;
        bullets[i].position.y += speedY;
        bullets[i].distance -= sqrtf(Square(speedX) + Square(speedY));

        if (bullets[i].distance <= 0) {
            deleteBullet(bullets, i);
        }
    }
}

void deleteBullet(Bullet *bullet, int index) {
    bullet[index].speed = (Vector2){0, 0};
    bullet[index].distance = 0;
}

void wrapAroundBullet(Bullet *bullets) {
    for (int i = 0; i < PLAYER_MAX_BULLETS; ++i) {
        if (bullets[i].distance == 0) continue;
        if (bullets[i].position.x > GetScreenWidth() + bullets[i].size.x) bullets[i].position.x = -bullets[i].size.x;
        if (bullets[i].position.y > GetScreenHeight() + bullets[i].size.y) bullets[i].position.y = -bullets[i].size.y;
        if (bullets[i].position.x < -bullets[i].size.x) bullets[i].position.x = GetScreenWidth() + bullets[i].size.x;
        if (bullets[i].position.y < -bullets[i].size.y) bullets[i].position.y = GetScreenHeight() + bullets[i].size.y;
    }
}

void initBullets(Bullet *bullets) {
    for (int i = 0; i < PLAYER_MAX_BULLETS; i++) {
        bullets[i].distance = 0;
        bullets[i].position = (Vector2){50,50};
        bullets[i].speed = (Vector2){0,0};
        bullets[i].size = (Vector2){5,5};
    }
}

void showGameoverScreen(Screen *currentScreen) {
    static float timer = 0;
    const int fontSize = 256;
    const char* gameover = "Game Over";
    const Vector2 textSize = MeasureTextEx(GetFontDefault(), gameover, fontSize, 25);
    DrawTextEx(GetFontDefault(), gameover,
        (Vector2){GetScreenWidth()/2-textSize.x/2, GetScreenHeight()/2-textSize.y/2}, fontSize, 25, ORANGE);
    timer += GetFrameTime();
    if (timer > 3) {
        *currentScreen = SCREEN_TITLE;
        timer = 0;
        particleArrDestroy();
    }
}