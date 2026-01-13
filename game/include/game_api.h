#pragma once
#define MAX_FPS INT_MAX
#include "wave.h"
#include "asteroid_array.h"
#include "bullet_array.h"
#include "screen_context.h"

typedef struct Bullet Bullet;

typedef struct GameContext {
    AsteroidArray *asteroidArray;
    WaveContext *wave;
    Player *player;
    AsteroidBulletHitEventQueue *bulletHitEventQueue;
    BulletArray *bulletArray;
    ScreenContext screenContext;
} GameContext;

int StartAsteroidGame();

