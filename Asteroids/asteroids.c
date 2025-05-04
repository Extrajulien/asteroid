#include "asteroids.h"

#include <files.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 256
extern AsteroidTraits bigTraits;
extern AsteroidTraits midTraits;
extern AsteroidTraits smlTraits;

//TODO -----
// - Creer les collision peut tirer dessus peut tuer le joueur
// -
// - si ben temps libre asteroides peuvent se foncer dedans et repondir


void moveAsteroids(AsteroidArray *asteroidArr) {
    if (asteroidArr->size == 0) { return; }
    for (int i = 0; i < asteroidArr->size; i++) {
        ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.x +=
                ((SmlAsteroid *) asteroidArr->asteroid[i])->base.speed.x * GetFrameTime() * 10;
        ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.y +=
                ((SmlAsteroid *) asteroidArr->asteroid[i])->base.speed.y * GetFrameTime() * 10;
    }
}

void wrapAroundAsteroid(AsteroidArray *asteroidArr) {
    if (asteroidArr->size == 0) { return; }
    for (int i = 0; i < asteroidArr->size; i++) {
        float radius = ((SmlAsteroid *) asteroidArr->asteroid[i])->base.radius;
        Vector2 pos = {
            ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.x,
            ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.y
        };

        if (pos.x > GetScreenWidth() + radius) ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.x = -radius;
        if (pos.y > GetScreenHeight() + radius) ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.y = -radius;
        if (pos.x < -radius) ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.x = GetScreenWidth() + radius;
        if (pos.y < -radius) ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.y = GetScreenHeight() + radius;
    }
}

void rotateAsteroid(AsteroidArray *asteroidArr, AsteroidType type) {
    if (asteroidArr->size == 0) { return; }

    switch (type) {
        case SMALL: SmlAsteroid **sArr = ((SmlAsteroid**)asteroidArr->asteroid);
            for (int i = 0; i < asteroidArr->size; i++) {
                sArr[i]->base.angle += sArr[i]->base.rotation * GetFrameTime();
                if (sArr[i]->base.angle >= 2 * PI) sArr[i]->base.angle = 0;
                if (sArr[i]->base.angle < 0) sArr[i]->base.angle = 2 * PI;
                rotateAsteroidVertices(asteroidArr->asteroid[i], sArr[i]->nbVertices);
            }
            break;
        case MEDIUM: MidAsteroid **mArr = ((MidAsteroid**)asteroidArr->asteroid);
            for (int i = 0; i < asteroidArr->size; i++) {
                mArr[i]->base.angle += mArr[i]->base.rotation * GetFrameTime();
                if (mArr[i]->base.angle >= 2 * PI) mArr[i]->base.angle = 0;
                if (mArr[i]->base.angle < 0) mArr[i]->base.angle = 2 * PI;
                rotateAsteroidVertices(asteroidArr->asteroid[i], mArr[i]->nbVertices);
            }
            break;
        case BIG: BigAsteroid **bArr = ((BigAsteroid**)asteroidArr->asteroid);
            for (int i = 0; i < asteroidArr->size; i++) {
                bArr[i]->base.angle += bArr[i]->base.rotation * GetFrameTime();
                if (bArr[i]->base.angle >= 2 * PI) bArr[i]->base.angle = 0;
                if (bArr[i]->base.angle < 0) bArr[i]->base.angle = 2 * PI;
                rotateAsteroidVertices(asteroidArr->asteroid[i], bArr[i]->nbVertices);
            }
        break;
        default:
            printf("Unknown type\n");
            break;
    }
}

//asteroid Array
//-------------------------------------------------------------------------------------

void generateWave(AsteroidArray *asteroidArr, int waveNum) {
    int nbAsteroid = waveNum * WAVE_ASTEROID_AMNT + WAVE_ASTEROID_AMNT;

    if (waveNum % 10 == 0 && waveNum != 0) {
        //TODO ----big wave (+25)
        return;
    }
    createBigAsteroid(asteroidArr, nbAsteroid);
}

void createBigAsteroid(AsteroidArray *asteroids, int nbAsteroid) {
    asteroids->size += nbAsteroid;
    asteroids->asteroid = malloc(nbAsteroid * sizeof(void *)); //init asteroid arr

    for (int i = 0; i < nbAsteroid; i++) {
        asteroids->asteroid[i] = malloc(sizeof(BigAsteroid)); // Allocate memory for each asteroid
    }
    for (int i = 0; i < nbAsteroid; i++) {
        const float rotation = ((rand() % (int)((bigTraits.maxRotationSpeed) - bigTraits.minRotationSpeed))
            + bigTraits.minRotationSpeed)/100.0f;

        ((BigAsteroid *) asteroids->asteroid[i])->base.radius = bigTraits.radius; //init radius
        ((BigAsteroid *) asteroids->asteroid[i])->base.spread = bigTraits.spread; //init spread
        ((BigAsteroid *) asteroids->asteroid[i])->base.type = BIG; //init type
        ((BigAsteroid *) asteroids->asteroid[i])->base.angle = 0; //init angle
        ((BigAsteroid *) asteroids->asteroid[i])->base.score = bigTraits.score; //init score
        ((BigAsteroid *) asteroids->asteroid[i])->base.rotation = rotation; //init rotation
        randomPosition(&((BigAsteroid *) asteroids->asteroid[i])->base); //init pos
        randomSpeed(&((BigAsteroid *) asteroids->asteroid[i])->base, BIG); //init speed

        generateVertices(asteroids->asteroid[i], bigTraits.nbVertices, bigTraits.generationStyle); //init points
    }
    printf("--Big asteroids generated!\n");
}

//asteroid
//-------------------------------------------------------------------------------------


void rotateAsteroidVertices(void *asteroid, int verticesNb) {
    Vector2 *ppoints;

    float angle = (((SmlAsteroid *) asteroid)->base.angle);
    switch (((SmlAsteroid *) asteroid)->base.type) {
        case SMALL:
            for (int i = 0; i < verticesNb; i++) {
                ((SmlAsteroid *) asteroid)->points[i].x = ((SmlAsteroid *) asteroid)->generatedPoints[i].x;
                ((SmlAsteroid *) asteroid)->points[i].y = ((SmlAsteroid *) asteroid)->generatedPoints[i].y;
            }
            ppoints = ((SmlAsteroid *) asteroid)->points;
            break;
        case MEDIUM:
            for (int i = 0; i < verticesNb; i++) {
                ((MidAsteroid *) asteroid)->points[i].x = ((MidAsteroid *) asteroid)->generatedPoints[i].x;
                ((MidAsteroid *) asteroid)->points[i].y = ((MidAsteroid *) asteroid)->generatedPoints[i].y;
            }
            ppoints = ((MidAsteroid *) asteroid)->points;
            break;
        case BIG:
            for (int i = 0; i < verticesNb; i++) {
                ((BigAsteroid *) asteroid)->points[i].x = ((BigAsteroid *) asteroid)->generatedPoints[i].x;
                ((BigAsteroid *) asteroid)->points[i].y = ((BigAsteroid *) asteroid)->generatedPoints[i].y;
            }
            ppoints = ((BigAsteroid *) asteroid)->points;
            break;
        default:
            printf("Unknown Asteroid type!\n");
            exit(1);
    }

    for (int i = 0; i < verticesNb; i++) {
        float tempX = ppoints[i].x * cosf(angle) - ppoints[i].y * sinf(angle);
        float tempY = ppoints[i].x * sinf(angle) + ppoints[i].y * cosf(angle);

        ppoints[i] = (Vector2){tempX, tempY};
    }
}

void generateVertices(void *asteroid, int nbVertices, bool generationStyle) {
    const float radius = ((SmlAsteroid *) asteroid)->base.radius;
    const float spread = ((SmlAsteroid *) asteroid)->base.spread;
    float distanceFromCenter;
    float rndSpread;
    Vector2 *ppoints = NULL;
    float radSpacing = 0;
    switch (((AsteroidBase *) asteroid)->type) {
        case SMALL:
            ((SmlAsteroid *) asteroid)->generatedPoints = malloc(sizeof(Vector2) * nbVertices);
            ((SmlAsteroid *) asteroid)->points = malloc(sizeof(Vector2) * nbVertices);
            ((SmlAsteroid *) asteroid)->nbVertices = nbVertices;
            ppoints = ((SmlAsteroid *) asteroid)->generatedPoints;
            break;
        case MEDIUM:
            ((MidAsteroid *) asteroid)->generatedPoints = malloc(sizeof(Vector2) * nbVertices);
            ((MidAsteroid *) asteroid)->points = malloc(sizeof(Vector2) * nbVertices);
            ((MidAsteroid *) asteroid)->nbVertices = nbVertices;
            ppoints = ((MidAsteroid *) asteroid)->generatedPoints;
            break;
        case BIG:
            ((BigAsteroid *) asteroid)->generatedPoints = malloc(sizeof(Vector2) * nbVertices);
            ((BigAsteroid *) asteroid)->points = malloc(sizeof(Vector2) * nbVertices);
            ((BigAsteroid *) asteroid)->nbVertices = nbVertices;
            ppoints = ((BigAsteroid *) asteroid)->generatedPoints;
            break;
        default:
            printf("Unknown Asteroid type!\n");
            exit(1);
    }
    radSpacing = (2 * PI) / nbVertices;
    for (int i = 0; i < nbVertices; i++) {
        if (generationStyle == true) {
            rndSpread = ((float)rand() / (float)RAND_MAX) * spread*2;
            distanceFromCenter = rndSpread + radius - spread;
        } else {
            rndSpread = ((float)rand() / (float)RAND_MAX) * spread;
            distanceFromCenter = rndSpread + radius;
        }
        ppoints[i].x = cosf(radSpacing * i) * distanceFromCenter;
        ppoints[i].y = sinf(radSpacing * i) * distanceFromCenter;
        //printf("p%d, (%.2f, %.2f)\n",i ,ppoints[i].x, ppoints[i].y);
    }
}
void randomSpeed(AsteroidBase *asteroid, AsteroidType type) {
    Vector2 speed;
    int maxSpeedScaled;
    switch (type) {
        case BIG:   maxSpeedScaled = (int)(bigTraits.maxSpeed*10);
                    speed.x = (float)((rand() % (maxSpeedScaled*2 + 1))-maxSpeedScaled)/10.0f;
                    speed.y = (float)((rand() % (maxSpeedScaled*2 + 1))-maxSpeedScaled)/10.0f;
                    break;
        case MEDIUM:    maxSpeedScaled = (int)(midTraits.maxSpeed*10);
                        speed.x = (float)((rand() % (maxSpeedScaled*2 + 1))-maxSpeedScaled)/10.0f;
                        speed.y = (float)((rand() % (maxSpeedScaled*2 + 1))-maxSpeedScaled)/10.0f;
                        break;
        case SMALL: maxSpeedScaled = (int)(smlTraits.maxSpeed*10);
                    speed.x = (float)((rand() % (maxSpeedScaled*2 + 1))-maxSpeedScaled)/10.0f;
                    speed.y = (float)((rand() % (maxSpeedScaled*2 + 1))-maxSpeedScaled)/10.0f;
                    break;
        default:
            printf("Unknown Asteroid type!\n");
            exit(1);
    }

    asteroid->speed.x = speed.x;
    asteroid->speed.y = speed.y;
    printf("speed (%.2f, %.2f)\n", speed.x, speed.y);
}

void randomPosition(AsteroidBase *asteroids) {
    asteroids->position.x = rand() % GetScreenWidth();
    asteroids->position.y = rand() % GetScreenHeight();
}


//draw Asteroids
//-------------------------------------------------------------------------------------
void renderAsteroids(AsteroidArray *arr) {
    if (arr->size == 0) { return; }
    Vector2 startPos;
    Vector2 endPos;
    const float lineThickness = 3;
    int verticesNb = 0;
    switch (((SmlAsteroid *) arr->asteroid[0])->base.type) {
        case SMALL:
            verticesNb = ((SmlAsteroid *) arr->asteroid[0])->nbVertices;
            for (int i = 0; i < arr->size; i++) {
                //for each asteroid
                Vector2 pos = ((SmlAsteroid *) arr->asteroid[i])->base.position;
                for (int j = 0; j < verticesNb - 1; ++j) {
                    startPos = ((SmlAsteroid *) arr->asteroid[i])->points[j];
                    endPos = ((SmlAsteroid *) arr->asteroid[i])->points[j + 1];
                    startPos.x += pos.x;
                    startPos.y += pos.y;
                    endPos.x += pos.x;
                    endPos.y += pos.y;
                    DrawLineEx(startPos, endPos, lineThickness, ASTEROID_COLOR);
                }
                startPos = ((SmlAsteroid *) arr->asteroid[i])->points[verticesNb - 1];
                endPos = ((SmlAsteroid *) arr->asteroid[i])->points[0];
                startPos.x += pos.x;
                startPos.y += pos.y;
                endPos.x += pos.x;
                endPos.y += pos.y;
                DrawLineEx(startPos, endPos, lineThickness, ASTEROID_COLOR);
            }
            return;
        case MEDIUM:
            verticesNb = ((MidAsteroid *) arr->asteroid[0])->nbVertices;
            for (int i = 0; i < arr->size; i++) {
                //for each asteroid
                Vector2 pos = ((MidAsteroid *) arr->asteroid[i])->base.position;
                for (int j = 0; j < verticesNb - 1; ++j) {
                    startPos = ((MidAsteroid *) arr->asteroid[i])->points[j];
                    endPos = ((MidAsteroid *) arr->asteroid[i])->points[j + 1];
                    startPos.x += pos.x;
                    startPos.y += pos.y;
                    endPos.x += pos.x;
                    endPos.y += pos.y;
                    DrawLineEx(startPos, endPos, lineThickness, ASTEROID_COLOR);
                }
                startPos = ((MidAsteroid *) arr->asteroid[i])->points[verticesNb - 1];
                endPos = ((MidAsteroid *) arr->asteroid[i])->points[0];
                startPos.x += pos.x;
                startPos.y += pos.y;
                endPos.x += pos.x;
                endPos.y += pos.y;
                DrawLineEx(startPos, endPos, lineThickness, ASTEROID_COLOR);
            }
            return;
        case BIG:
            verticesNb = ((BigAsteroid *) arr->asteroid[0])->nbVertices;
            for (int i = 0; i < arr->size; i++) {
                //for each asteroid
                Vector2 pos = ((BigAsteroid *) arr->asteroid[i])->base.position;
                for (int j = 0; j < verticesNb - 1; ++j) {
                    startPos = ((BigAsteroid *) arr->asteroid[i])->points[j];
                    endPos = ((BigAsteroid *) arr->asteroid[i])->points[j + 1];
                    startPos.x += pos.x;
                    startPos.y += pos.y;
                    endPos.x += pos.x;
                    endPos.y += pos.y;
                    DrawLineEx(startPos, endPos, lineThickness, ASTEROID_COLOR);
                }
                startPos = ((BigAsteroid *) arr->asteroid[i])->points[verticesNb - 1];
                endPos = ((BigAsteroid *) arr->asteroid[i])->points[0];
                startPos.x += pos.x;
                startPos.y += pos.y;
                endPos.x += pos.x;
                endPos.y += pos.y;
                DrawLineEx(startPos, endPos, lineThickness, ASTEROID_COLOR);
            }
            return;
        default:
            printf("Unknown Asteroid type!\n");
            exit(1);
    }
}

void freeAsteroidArray(AsteroidArray *arr, AsteroidType type) {
    for (int i = 0; i < arr->size; i++) {
        switch (type) {
            case BIG:
                free(((BigAsteroid **) arr->asteroid)[i]->generatedPoints);
                free(((BigAsteroid **) arr->asteroid)[i]->points);
                free(((BigAsteroid **) arr->asteroid)[i]);
                break;
            case MEDIUM:
                free(((MidAsteroid **) arr->asteroid)[i]->generatedPoints);
                free(((MidAsteroid **) arr->asteroid)[i]->points);
                free(((MidAsteroid **) arr->asteroid)[i]);
                break;
            case SMALL:
                free(((SmlAsteroid **) arr->asteroid)[i]->generatedPoints);
                free(((SmlAsteroid **) arr->asteroid)[i]->points);
                free(((SmlAsteroid **) arr->asteroid)[i]);
                break;
        }
    }
    free(arr->asteroid);
    arr->asteroid = NULL;
    arr->size = 0;
}
