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

void randomPosition();
void generateWave(BigAsteroid *asteroid, int waveNum);
void createAsteroid(BigAsteroid asteroids);

#endif //ASTEROIDCLION_ASTEROIDS_H
