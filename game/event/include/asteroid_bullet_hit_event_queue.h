#pragma once
#include "asteroid_bullet_hit_event.h"


typedef struct AsteroidBulletHitEventQueue {
    AsteroidBulletHitEvent *events;
    unsigned int count;
    unsigned int capacity;
} AsteroidBulletHitEventQueue;

AsteroidBulletHitEventQueue* ASTEROID_BULLET_HIT_EVENT_QUEUE_Create();
void ASTEROID_BULLET_HIT_EVENT_QUEUE_Free(AsteroidBulletHitEventQueue *queue);
void ASTEROID_BULLET_HIT_EVENT_QUEUE_Add(AsteroidBulletHitEventQueue *queue, AsteroidBulletHitEvent event);
void ASTEROID_BULLET_HIT_EVENT_QUEUE_Purge(AsteroidBulletHitEventQueue *queue);