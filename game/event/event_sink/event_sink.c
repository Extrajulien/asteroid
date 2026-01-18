#include "asteroid_bullet_hit_event_queue.h"
#include "event_sink_api.h"

void ASTEROID_BULLET_HIT_SINK_Emit(void* ctx, AsteroidBulletHitEvent event);

AsteroidBulletHitEventSink ASTEROID_BULLET_HIT_EVENT_QUEUE_GetSink(AsteroidBulletHitEventQueue* queue) {
    return (AsteroidBulletHitEventSink) {
        .ctx = queue,
        .emit = ASTEROID_BULLET_HIT_SINK_Emit
    };
}

void ASTEROID_BULLET_HIT_SINK_Emit(void* ctx, const AsteroidBulletHitEvent event) {
    AsteroidBulletHitEventQueue *queue = ctx;
    ASTEROID_BULLET_HIT_EVENT_QUEUE_Add(queue, event);
}