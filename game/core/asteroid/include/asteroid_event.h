#pragma once
#include <stddef.h>

#include "raylib.h"

typedef struct {
    size_t asteroidId;
    size_t BulletId;
    float hitAngle;
    int score;
    Vector2 hitPosition;
} AsteroidBulletHitEvent;


typedef struct {
    AsteroidBulletHitEvent *events;
    unsigned int eventCount;
    unsigned int capacity;
} AsteroidBulletHitEventQueue;


AsteroidBulletHitEventQueue* ASTEROID_BULLET_HIT_QUEUE_Create();
void ASTEROID_BULLET_HIT_QUEUE_Add(AsteroidBulletHitEventQueue *queue, AsteroidBulletHitEvent event);
void ASTEROID_BULLET_HIT_QUEUE_Purge(AsteroidBulletHitEventQueue *queue);




