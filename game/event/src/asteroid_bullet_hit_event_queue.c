#include "asteroid_bullet_hit_event_queue.h"
#include "asteroid_bullet_hit_event.h"

#include <stdio.h>
#include <stdlib.h>


AsteroidBulletHitEventQueue* ASTEROID_BULLET_HIT_EVENT_QUEUE_Create() {
    AsteroidBulletHitEventQueue *queue = malloc(sizeof(AsteroidBulletHitEventQueue));

    if (!queue) {
        printf("ERROR allocating Event_Queue\n");
        exit(1);
    }

    queue->events = malloc(sizeof(AsteroidBulletHitEvent));
    if (!queue->events) {
        printf("ERROR allocating Event queue: queue->events\n");
        exit(1);
    }

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


void ASTEROID_BULLET_HIT_EVENT_QUEUE_Add(AsteroidBulletHitEventQueue *queue, const AsteroidBulletHitEvent *event) {
    if (queue->capacity < queue->count + 1) {
        queue->capacity *= 2;
        AsteroidBulletHitEvent *temp = realloc(queue->events, sizeof(AsteroidBulletHitEvent) * (queue->capacity));
        if (!temp) {
            printf("ASTEROID_BULLET_HIT_QUEUE Realloc Failed");
            exit(1);
        }
        queue->events = temp;
    }
    queue->events[queue->count] = *event;
    queue->count++;
}

void ASTEROID_BULLET_HIT_EVENT_QUEUE_Purge(AsteroidBulletHitEventQueue *queue) {
    queue->count = 0;
}