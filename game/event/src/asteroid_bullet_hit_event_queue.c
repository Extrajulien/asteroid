#include "asteroid_bullet_hit_event_queue.h"
#include "asteroid_bullet_hit_event.h"

#include <stdio.h>
#include <stdlib.h>
#include "logger.h"


AsteroidBulletHitEventQueue* ASTEROID_BULLET_HIT_EVENT_QUEUE_Create() {
    AsteroidBulletHitEventQueue *queue = malloc(sizeof(AsteroidBulletHitEventQueue));

    ASSERT_ALLOCATION(queue);

    queue->events = malloc(sizeof(AsteroidBulletHitEvent));
    ASSERT_ALLOCATION(queue->events);

    queue->capacity = 1;
    queue->count = 0;
    return queue;
}


void ASTEROID_BULLET_HIT_EVENT_QUEUE_Free(AsteroidBulletHitEventQueue *queue) {
    if (queue->events) {
        free(queue->events);
    }
    free(queue);
}


void ASTEROID_BULLET_HIT_EVENT_QUEUE_Add(AsteroidBulletHitEventQueue *queue, const AsteroidBulletHitEvent event) {
    if (queue->capacity < queue->count + 1) {
        queue->capacity *= 2;
        AsteroidBulletHitEvent *temp = realloc(queue->events, sizeof(AsteroidBulletHitEvent) * (queue->capacity));
        ASSERT_ALLOCATION(temp);
        queue->events = temp;
    }
    queue->events[queue->count] = event;
    queue->count++;
}

void ASTEROID_BULLET_HIT_EVENT_QUEUE_Purge(AsteroidBulletHitEventQueue *queue) {
    queue->count = 0;
}