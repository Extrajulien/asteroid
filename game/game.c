#include <limits.h>

#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "asteroid_bullet_hit_event_queue.h"
#include "asteroid_vertice_pool.h"
#include "bullet_array.h"
#include "char_array.h"
#include "game_api.h"
#include "menu.h"
#include "particle.h"
#include "screen.h"
#include "screen_virtual_table.h"
#include "wave.h"
#include "logger.h"

void updateGame(Player *player, BulletArray *bulletArr, AsteroidArray *asteroidArray);

void showGameoverScreen(Screen *currentScreen);

#define SUPPORT_FILEFORMAT_MP3

int StartAsteroidGame() {
    srand((unsigned int)time(NULL));
    InitWindow(1920, 1080, "Asteroids");
    //InitAudioDevice();
    //Music music = LoadMusicStream("../music");
    //PlayMusicStream(music);

    SetTargetFPS(MAX_FPS);
    loadThemes();
    GameContext context = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        SCREEN_TITLE
    };

    Screen currentScreen = SCREEN_TITLE;
    ScreenVTable screenFunction = SCREEN_GetVTable(currentScreen);
    screenFunction.open(&currentScreen, &context);

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        //UpdateMusicStream(music);

        screenFunction.update(&currentScreen, &context);
        if (currentScreen != screenFunction.screen) {
            screenFunction.close(&currentScreen, &context);
            screenFunction = SCREEN_GetVTable(currentScreen);
            screenFunction.open(&currentScreen, &context);
        }
        BeginDrawing();
        screenFunction.draw(&currentScreen, &context);

        EndDrawing();
    }
    //CloseAudioDevice();
    CloseWindow();
    return 0;
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