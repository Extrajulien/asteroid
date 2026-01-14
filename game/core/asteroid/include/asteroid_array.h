#pragma once
#include <stddef.h>

#include "bullet_array.h"
#include "event_sink_api.h"


typedef struct Asteroid Asteroid;


typedef struct AsteroidArray {
    Asteroid *asteroid;
    size_t capacity;
    size_t nbAsteroid;
    size_t maxReachedCount;
} AsteroidArray;

AsteroidArray* ASTEROIDS_CreateArray();
void ASTEROIDS_FreeArray(AsteroidArray *asteroidArr);
void ASTEROIDS_Update(AsteroidArray *asteroidArr);
void ASTEROIDS_Render(const AsteroidArray *asteroidArr);
void ASTEROIDS_Add(AsteroidArray *asteroidArr, const Asteroid* asteroid);
void ASTEROIDS_CollideBullets(const AsteroidArray *asteroidArray, const BulletArray *bullets, const AsteroidBulletHitEventSink *sink);
void ASTEROIDS_Compact(AsteroidArray *asteroidArr);
void ASTEROIDS_Purge(AsteroidArray *asteroidArr);

bool ASTEROIDS_AreAllSizesPresent(const AsteroidArray *asteroidArr);


typedef struct Player Player;
typedef struct PackageCollisionPlayer {
    Player *player;
    AsteroidArray *bigArr, *midArr, *smlArr;
} PackageCollisionPlayer;