#ifndef ASTEROIDCLION_ASTEROIDS_H
#define ASTEROIDCLION_ASTEROIDS_H
#define ASTEROID_SPREAD 20.0
#define WAVE_ASTEROID_AMNT 5
#define MAX_ASTEROID_SPEED 1
#define ASTEROID_COLOR (Color){220,220,220,255}

#define SML_VERTICES 12
#define MID_VERTICES 18
#define BIG_VERTICES 25

#include "raylib.h"
#include "player.h"

typedef enum {
    BIG,
    MEDIUM,
    SMALL

} AsteroidType;

typedef struct {
    AsteroidType type;
    Vector2 position;
    Vector2 speed;
    float radius;
    float angle;
    float rotation;                 //in radians
    float spread;
    float weight;                   //area of polygon
    int score;
    bool isCollisionEnabled;
} AsteroidBase;

//petit asteroide composant
typedef struct {
    AsteroidBase base;
    Vector2 *generatedPoints;   //vertices unRotated
    int nbVertices;
    Vector2 *points;
} SmlAsteroid;

//moyen asteroide composant
typedef struct {
    AsteroidBase base;
    Vector2 *generatedPoints;   //vertices unRotated
    int nbVertices;
    Vector2 *points;
} MidAsteroid;

//Gros asteroide composant
typedef struct {
    AsteroidBase base;
    Vector2 *generatedPoints;   //vertices unRotated
    int nbVertices;
    Vector2 *points;
} BigAsteroid;

typedef struct {
    void **asteroid;
    short size;
    short nbAsteroid;
} AsteroidArray;

typedef struct {
    AsteroidType type;
    float radius;
    float spread;
    float minRotationSpeed;
    float maxRotationSpeed;
    float maxSpeed;
    int score;
    int nbVertices;
    bool generationStyle;
} AsteroidTraits;

typedef struct {
    AsteroidArray *bigArr, *midArr, *smlArr;
    Bullet *bullets;
    int *score;
} PackageCollisionBullet;

void moveAsteroids(AsteroidArray *asteroidArr);
void wrapAroundAsteroid(AsteroidArray *asteroidArr);
void rotateAsteroid(AsteroidArray *asteroidArr, AsteroidType type);
void rotateAsteroidVertices(void *asteroid, int verticesNb);

void readPresetFile();

void randomPosition(AsteroidBase *asteroids);
void generateWave(AsteroidArray *asteroid, int waveNum);

void randomSpeed(AsteroidBase *asteroid, AsteroidType type);

void generateVertices(void *asteroid, int nbVertices, bool generationStyle);

void renderAsteroids(AsteroidArray *arr);

void createBigAsteroid(AsteroidArray *asteroids, int nbAsteroid);
void* checkCollisionAstBullet(void* arg);
void freeAsteroidArray(AsteroidArray *arr, AsteroidType type);
void createMidAsteroid(AsteroidArray *arr, int nbAsteroid, Vector2 position);
void createSmlAsteroid(AsteroidArray *arr, int nbAsteroid, Vector2 position);

/*
void bigAsteroidShot(BigAsteroidArray **asteroid, MidAsteroidArray **newAsteroid, short index);
void midAsteroidShot(MidAsteroidArray **asteroid, SmlAsteroidArray **newAsteroid, short index);
void smlAsteroidShot(SmlAsteroidArray **asteroid, short index);

void createMidAsteroid(MidAsteroidArray **asteroids);
void createSmlAsteroid(SmlAsteroidArray **asteroids);
*/

#endif //ASTEROIDCLION_ASTEROIDS_H
