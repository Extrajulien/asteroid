//
// Created by 2485548 on 2025-04-14.
//

#ifndef ASTEROIDCLION_ASTEROIDS_H
#define ASTEROIDCLION_ASTEROIDS_H
#define ASTEROID_SPREAD 20.0

#include "raylib.h"

typedef struct {
    Vector2 position;
    Vector2 speed;
    Vector2 points[5];
    float radius;
    float angle;
    float rotation;
} SmlAsteroid;
typedef struct {
    Vector2 position;
    Vector2 speed;
    Vector2 points[9];
    float radius;
    float angle;
    float rotation;
} MidAsteroid;

typedef struct {
    Vector2 position;
    Vector2 speed;
    Vector2 points[13];
    float radius;
    float angle;
    float rotation;
} BigAsteroid;

typedef struct {
    BigAsteroid *asteroid;
    short index;
}BigAsteroidArray;

typedef struct {
    MidAsteroid *asteroid;
    short index;
}MidAsteroidArray;

typedef struct {
    SmlAsteroid *asteroid;
    short index;
}SmlAsteroidArray;


void randomPosition();
void generateWave(BigAsteroid **asteroid, int waveNum);
void createBigAsteroid(BigAsteroid **asteroids);

void bigAsteroidShot(BigAsteroidArray **asteroid, MidAsteroidArray **newAsteroid, short index);
void midAsteroidShot(MidAsteroidArray **asteroid, SmlAsteroidArray **newAsteroid, short index);
void smlAsteroidShot(SmlAsteroidArray **asteroid, short index);

void createBigAsteroid(BigAsteroidArray **asteroids);
void createMidAsteroid(MidAsteroidArray **asteroids);
void createSmlAsteroid(SmlAsteroidArray **asteroids);

#endif //ASTEROIDCLION_ASTEROIDS_H
