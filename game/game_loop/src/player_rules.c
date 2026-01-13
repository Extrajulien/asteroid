#include "player_rules.h"

#include "asteroid_bullet_hit_event_queue.h"
#include "bullet_array.h"
#include "player.h"

void PLAYER_UpdateBulletHits(const AsteroidBulletHitEventQueue *queue, Player *player, const BulletArray *bulletArr) {
    for (int i = 0; i < queue->count; ++i) {
        const AsteroidBulletHitEvent *event = &queue->events[i];
        BULLETS_Destroy(bulletArr, i);
        PLAYER_AddScore(player, event->score);
    }
}
