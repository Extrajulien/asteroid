#pragma once

typedef struct AsteroidBulletHitEventQueue AsteroidBulletHitEventQueue;
typedef struct Player Player;
typedef struct BulletArray BulletArray;


void PLAYER_UpdateBulletHits(const AsteroidBulletHitEventQueue *queue, Player *player, BulletArray *bulletArr);