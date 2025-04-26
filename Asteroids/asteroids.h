//
// Created by 2485548 on 2025-04-14.
//

#ifndef ASTEROIDCLION_ASTEROIDS_H
#define ASTEROIDCLION_ASTEROIDS_H
#define ASTEROID_SPREAD 20.0
#define WAVE_ASTEROID_AMNT 5
#define MAX_ASTEROID_SPEED 10
#define ASTEROID_COLOR (Color){255,255,255,200}

#define SML_VERTICES 12
#define MID_VERTICES 18
#define BIG_VERTICES 25

#include "raylib.h"

typedef enum {
    SMALL,
    MEDIUM,
    BIG
} AsteroidType;

typedef struct {
    AsteroidType type;
    Vector2 position;
    Vector2 speed;
    float radius;
    float angle;
    float rotation;
    float spread;
    float weight;                   //area of polygon
    int score;
} AsteroidBase;

//100pts
typedef struct {
    AsteroidBase base;
    Vector2 points[SML_VERTICES];   //verticies
} SmlAsteroid;

//50pts
typedef struct {
    AsteroidBase base;
    Vector2 points[MID_VERTICES];   //verticies
} MidAsteroid;

//20pts
typedef struct {
    AsteroidBase base;
    Vector2 points[BIG_VERTICES];   //verticies
} BigAsteroid;

typedef struct {
    void **asteroid;
    short size;
}AsteroidArray;


void randomPosition(AsteroidBase *asteroids);
void generateWave(AsteroidArray *asteroid, int waveNum);

void randomSpeed(AsteroidBase *asteroids);

void generateVertices(void *asteroid);

void renderAsteroids(AsteroidArray *arr);

void createBigAsteroid(AsteroidArray *asteroids, int nbAsteroid);
/*
void bigAsteroidShot(BigAsteroidArray **asteroid, MidAsteroidArray **newAsteroid, short index);
void midAsteroidShot(MidAsteroidArray **asteroid, SmlAsteroidArray **newAsteroid, short index);
void smlAsteroidShot(SmlAsteroidArray **asteroid, short index);

void createBigAsteroid(MidAsteroidArray **asteroids);
void createMidAsteroid(MidAsteroidArray **asteroids);
void createSmlAsteroid(SmlAsteroidArray **asteroids);
*/

#endif //ASTEROIDCLION_ASTEROIDS_H
