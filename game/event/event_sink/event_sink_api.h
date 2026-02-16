#pragma once
#include "asteroid_bullet_hit_event.h"
#include "player_asteroid_hit_event_queue.h"

typedef struct AsteroidBulletHitEventSink {
    void (*emit)(void* ctx, AsteroidBulletHitEvent event);
    void* ctx;
} AsteroidBulletHitEventSink;

typedef struct PlayerAsteroidHitEventSink {
    void (*emit)(void* ctx, PlayerAsteroidHitEvent event);
    void* ctx;
} PlayerAsteroidHitEventSink;


typedef struct AsteroidBulletHitEventQueue AsteroidBulletHitEventQueue;
typedef struct PlayerAsteroidHitEventQueue PlayerAsteroidHitEventQueue;



AsteroidBulletHitEventSink ASTEROID_BULLET_HIT_EVENT_QUEUE_GetSink(AsteroidBulletHitEventQueue *queue);

PlayerAsteroidHitEventSink PLAYER_ASTEROID_HIT_EVENT_QUEUE_GetSink(PlayerAsteroidHitEventQueue *queue);