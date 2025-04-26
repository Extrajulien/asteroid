//
// Created by 2485548 on 2025-04-14.
//

#ifndef ASTEROIDCLION_ASTEROIDS_H
#define ASTEROIDCLION_ASTEROIDS_H
#define ASTEROID_SPREAD 20.0
#define WAVE_ASTEROID_AMNT 5
#define MAX_ASTEROID_SPEED 10

#define SML_VERTICES 5
#define MID_VERTICES 9
#define BIG_VERTICES 13

#include "raylib.h"

typedef struct {
    Vector2 position;
    Vector2 speed;
    Vector2 points[SML_VERTICES];   //verticies
    float radius;
    float angle;
    float rotation;
    float spead;
    float weight;                   //area of polygon
    int score;//100pts
} SmlAsteroid;
typedef struct {
    Vector2 position;
    Vector2 speed;
    Vector2 points[MID_VERTICES];   //verticies
    float radius;
    float angle;
    float rotation;
    float spead;
    float weight;                   //area of polygon
    int score;//50pts
} MidAsteroid;

typedef struct {
    Vector2 position;
    Vector2 speed;
    Vector2 points[BIG_VERTICES];   //verticies
    float radius;
    float angle;
    float rotation;
    float spead;
    float weight;                   //area of polygon
    int score;//20pts
} BigAsteroid;

typedef struct {
    void **asteroid;
    short size;
}AsteroidArray;


void randomPosition(BigAsteroid *asteroids);
void generateWave(AsteroidArray asteroid, int waveNum);

void randomSpeed(BigAsteroid *asteroids);

void generateVertices(void *asteroid);//TODO------------------------------------------------------

void bigAsteroidShot(BigAsteroidArray **asteroid, MidAsteroidArray **newAsteroid, short index);
void midAsteroidShot(MidAsteroidArray **asteroid, SmlAsteroidArray **newAsteroid, short index);
void smlAsteroidShot(SmlAsteroidArray **asteroid, short index);

void createBigAsteroid(AsteroidArray asteroidArr, int nbAsteroid);
void createBigAsteroid(MidAsteroidArray **asteroids);
void createMidAsteroid(MidAsteroidArray **asteroids);
void createSmlAsteroid(SmlAsteroidArray **asteroids);

#endif //ASTEROIDCLION_ASTEROIDS_H
