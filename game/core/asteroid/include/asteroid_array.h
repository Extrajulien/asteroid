#pragma once
#include <stddef.h>
#include "player.h"

#include "asteroid_vertice_pool.h"
#include "bullet_array.h"
#include "event_sink_api.h"


typedef struct Asteroid Asteroid;


typedef struct AsteroidArray {
    Asteroid *asteroid;
    VerticePool *verticePool;
    size_t capacity;
    size_t nbAsteroid;
    size_t maxReachedCount;
} AsteroidArray;

AsteroidArray* ASTEROIDS_CreateArray();
void ASTEROIDS_FreeArray(AsteroidArray *asteroidArr);
void ASTEROIDS_Update(AsteroidArray *asteroidArr);
void ASTEROIDS_Render(const AsteroidArray *asteroidArr);
void ASTEROIDS_Add(AsteroidArray *asteroidArr, Asteroid asteroid);
void GAME_LOOP_ProcessASTEROIDS_CollideBullets(const AsteroidArray *asteroidArray, const BulletArray *bullets, const AsteroidBulletHitEventSink *sink);
void ASTEROIDS_CollidePlayer(const AsteroidArray *asteroidArray, const Player *player, const PlayerAsteroidHitEventSink *sink);
void ASTEROIDS_Compact(AsteroidArray *asteroidArray);
void ASTEROIDS_Purge(AsteroidArray *asteroidArr);

bool ASTEROIDS_AreAllSizesPresent(const AsteroidArray *asteroidArr);
void ASTEROIDS_Remove(AsteroidArray *asteroidArray, size_t index);


typedef struct Player Player;
typedef struct PackageCollisionPlayer {
    Player *player;
    AsteroidArray *bigArr, *midArr, *smlArr;
} PackageCollisionPlayer;