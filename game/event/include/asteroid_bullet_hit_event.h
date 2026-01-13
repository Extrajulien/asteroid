#pragma once
#include <stddef.h>
#include "raylib.h"

typedef struct AsteroidBulletHitEvent {
    size_t asteroidId;
    size_t BulletId;
    float hitAngle;
    int score;
    Vector2 hitPosition;
} AsteroidBulletHitEvent;