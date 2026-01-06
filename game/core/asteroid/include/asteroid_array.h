#pragma once
#include <stddef.h>

#include "asteroid.h"
#include "asteroid_event.h"

typedef struct {
    Asteroid *asteroid;
    size_t capacity;
    size_t nbAsteroid;
} AsteroidArray;


typedef struct {
    Player *player;
    AsteroidArray *bigArr, *midArr, *smlArr;
} PackageCollisionPlayer;


AsteroidArray* ASTEROIDS_CreateArray();
void ASTEROIDS_FreeArray(AsteroidArray *asteroidArr);
void ASTEROIDS_Update(AsteroidArray *asteroidArr);
void ASTEROIDS_Render(const AsteroidArray *asteroidArr);
void ASTEROIDS_Add(AsteroidArray *asteroidArr, Asteroid asteroid);
void ASTEROIDS_CollideBullets(const AsteroidArray *asteroidArray, const Bullet *bullets, AsteroidBulletHitEventQueue *eventQueue);
void ASTEROIDS_Compact(AsteroidArray *asteroidArr);
void ASTEROIDS_Purge(AsteroidArray *asteroidArr);