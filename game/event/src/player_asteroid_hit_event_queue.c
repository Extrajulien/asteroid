#include "player_asteroid_hit_event.h"
#include "player_asteroid_hit_event_queue.h"

#include <stdlib.h>

#include "asteroid_bullet_hit_event.h"
#include "logger.h"


typedef struct PlayerAsteroidHitEventQueue {
    PlayerAsteroidHitEvent *events;
    unsigned int count;
    unsigned int capacity;
} PlayerAsteroidHitEventQueue;


PlayerAsteroidHitEventQueue* PLAYER_ASTEROID_HIT_EVENT_QUEUE_Create() {
   PlayerAsteroidHitEventQueue *queue = malloc(sizeof(PlayerAsteroidHitEventQueue));

    ASSERT_ALLOCATION(queue);

    queue->events = malloc(sizeof(PlayerAsteroidHitEvent));
    ASSERT_ALLOCATION(queue->events);

    queue->capacity = 1;
    queue->count = 0;
    return queue;
}

void PLAYER_ASTEROID_HIT_EVENT_QUEUE_Free(PlayerAsteroidHitEventQueue *queue) {
    if (queue->events) {
        free(queue->events);
    }
    free(queue);
}

void PLAYER_ASTEROID_HIT_EVENT_QUEUE_Add(PlayerAsteroidHitEventQueue *queue, const PlayerAsteroidHitEvent event) {
    if (queue->capacity < queue->count + 1) {
        queue->capacity *= 2;
        PlayerAsteroidHitEvent *temp = realloc(queue->events, sizeof(PlayerAsteroidHitEvent) * (queue->capacity));
        ASSERT_ALLOCATION(temp);
        queue->events = temp;
    }
    queue->events[queue->count] = event;
    queue->count++;
}

void PLAYER_ASTEROID_HIT_EVENT_QUEUE_Purge(PlayerAsteroidHitEventQueue *queue) {
    queue->count = 0;
}