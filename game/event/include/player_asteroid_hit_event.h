#pragma once
#include <stddef.h>
#include "player.h"
#include "raylib.h"

typedef struct PlayerAsteroidHitEvent {
    Player* player;
    size_t asteroidId;
    Vector2 hitPosition;
    float hitAngle;
} PlayerAsteroidHitEvent;
