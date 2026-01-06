#include "asteroid_event.h"

#include <stdio.h>
#include <stdlib.h>

AsteroidBulletHitEventQueue* ASTEROID_BULLET_HIT_QUEUE_Create() {
    AsteroidBulletHitEventQueue *queue = malloc(sizeof(AsteroidBulletHitEventQueue));

    if (!queue) {
        printf("ERROR allocating ASTEROID_BULLET_HIT_QUEUE\n");
        exit(1);
    }

    queue->events = malloc(sizeof(AsteroidBulletHitEvent));
    if (!queue->events) {
        printf("ERROR allocating queue->events\n");
        exit(1);
    }

    queue->capacity = 1;
    queue->eventCount = 0;
    return queue;
}


void ASTEROID_BULLET_HIT_QUEUE_Add(AsteroidBulletHitEventQueue *queue, const AsteroidBulletHitEvent event) {
    if (queue->capacity < queue->eventCount + 1) {
        queue->capacity *= 2;
        AsteroidBulletHitEvent *temp = realloc(queue->events, sizeof(AsteroidBulletHitEvent) * (queue->capacity));
        if (!temp) {
            printf("ASTEROID_BULLET_HIT_QUEUE Realloc Failed");
            exit(1);
        }
        queue->events = temp;
    }
    queue->events[queue->eventCount] = event;
    queue->eventCount++;
}

void ASTEROID_BULLET_HIT_QUEUE_Purge(AsteroidBulletHitEventQueue *queue) {
    queue->eventCount = 0;
}
