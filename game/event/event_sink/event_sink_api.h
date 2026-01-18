#pragma once
#include "asteroid_bullet_hit_event.h"

typedef struct AsteroidBulletHitEventSink {
    void (*emit)(void* ctx, AsteroidBulletHitEvent event);
    void* ctx;
} AsteroidBulletHitEventSink;


typedef struct AsteroidBulletHitEventQueue AsteroidBulletHitEventQueue;



AsteroidBulletHitEventSink ASTEROID_BULLET_HIT_EVENT_QUEUE_GetSink(AsteroidBulletHitEventQueue *queue);