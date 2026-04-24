#include <climits>
extern "C" {
#include "raylib.h"
}
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "asteroid_bullet_hit_event_queue.h"
#include "bullet_array.h"
#include "game_api.h"
#include "menu.h"
#include "screen.h"
#include "screen_virtual_table.h"
#include "wave.h"
#define SUPPORT_FILEFORMAT_MP3

void update();

int StartAsteroidGame() {
    srand(static_cast<unsigned int>(time(nullptr)));
    InitWindow(1920, 1080, "Asteroids");
    InitAudioDevice();
    Music music = LoadMusicStream("../music.mp3");
    PlayMusicStream(music);

    SetTargetFPS(MAX_FPS);
    loadThemes();
    GameContext context = {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        {SCREEN_TITLE},
        false
    };

    ScreenVTable screenFunction = SCREEN_GetVTable(context.screenContext.screen);
    screenFunction.open(&context);
    UpdateMusicStream(music);
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {


        screenFunction.update(&context);
        if (context.screenContext.screen != screenFunction.screen) {
            screenFunction.close(&context);
            screenFunction = SCREEN_GetVTable(context.screenContext.screen);
            screenFunction.open(&context);
        }
        BeginDrawing();
        screenFunction.draw(&context);

        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void update() {
}