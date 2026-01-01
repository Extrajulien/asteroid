#include "asteroid.h"
#include "files.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "particle.h"
#define BUFFER_SIZE 256
extern AsteroidTraits bigTraits;
extern AsteroidTraits midTraits;
extern AsteroidTraits smlTraits;

//TODO -----
// - Creer les collision peut tirer dessus peut tuer le joueur
// -
// - si ben temps libre asteroides peuvent se foncer dedans et repondir


void moveAsteroids(AsteroidArray *asteroidArr) {
    if (asteroidArr->reservedSize == 0) { return; }
    for (int i = 0; i < asteroidArr->reservedSize; i++) {
        ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.x +=
                ((SmlAsteroid *) asteroidArr->asteroid[i])->base.speed.x * GetFrameTime() * 10;
        ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.y +=
                ((SmlAsteroid *) asteroidArr->asteroid[i])->base.speed.y * GetFrameTime() * 10;
    }
}

void wrapAroundAsteroid(AsteroidArray *asteroidArr) {
    if (asteroidArr->reservedSize == 0) { return; }
    for (int i = 0; i < asteroidArr->reservedSize; i++) {
        float radius = ((SmlAsteroid *) asteroidArr->asteroid[i])->base.radius;
        Vector2 pos = {
            ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.x,
            ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.y
        };

        if (pos.x > GetScreenWidth() + radius) ((SmlAsteroid*) asteroidArr->asteroid[i])->base.position.x = -radius;
        if (pos.y > GetScreenHeight() + radius) ((SmlAsteroid*) asteroidArr->asteroid[i])->base.position.y = -radius;
        if (pos.x < -radius) ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.x = GetScreenWidth() + radius;
        if (pos.y < -radius) ((SmlAsteroid *) asteroidArr->asteroid[i])->base.position.y = GetScreenHeight() + radius;
    }
}

void rotateAsteroid(AsteroidArray *asteroidArr, AsteroidType type) {
    if (asteroidArr->reservedSize == 0) { return; }

    switch (type) {
        case SMALL: SmlAsteroid **sArr = ((SmlAsteroid**)asteroidArr->asteroid);
            for (int i = 0; i < asteroidArr->reservedSize; i++) {
                sArr[i]->base.angle += sArr[i]->base.rotation * GetFrameTime();
                if (sArr[i]->base.angle >= 2 * PI) sArr[i]->base.angle = 0;
                if (sArr[i]->base.angle < 0) sArr[i]->base.angle = 2 * PI;
                rotateAsteroidVertices(asteroidArr->asteroid[i], sArr[i]->nbVertices);
            }
            break;
        case MEDIUM: MidAsteroid **mArr = ((MidAsteroid**)asteroidArr->asteroid);
            for (int i = 0; i < asteroidArr->reservedSize; i++) {
                mArr[i]->base.angle += mArr[i]->base.rotation * GetFrameTime();
                if (mArr[i]->base.angle >= 2 * PI) mArr[i]->base.angle = 0;
                if (mArr[i]->base.angle < 0) mArr[i]->base.angle = 2 * PI;
                rotateAsteroidVertices(asteroidArr->asteroid[i], mArr[i]->nbVertices);
            }
            break;
        case BIG: BigAsteroid **bArr = ((BigAsteroid**)asteroidArr->asteroid);
            for (int i = 0; i < asteroidArr->reservedSize; i++) {
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

void generateWave(AsteroidArray *asteroidArr, int waveNum, const Player *player) {
    int nbAsteroid = waveNum * WAVE_ASTEROID_AMNT + WAVE_ASTEROID_AMNT;

    if (waveNum % 10 == 0 && waveNum != 0) {
        //TODO ----big wave (+25)
        return;
    }
    createBigAsteroid(asteroidArr, nbAsteroid, player);
}

void createBigAsteroid(AsteroidArray *asteroids, int nbAsteroid, const Player *player) {
    asteroids->reservedSize += nbAsteroid;
    asteroids->nbAsteroid += nbAsteroid;
    asteroids->asteroid = malloc(nbAsteroid * sizeof(void *)); //init asteroid arr

    for (int i = 0; i < nbAsteroid; i++) {
        asteroids->asteroid[i] = malloc(sizeof(BigAsteroid)); // Allocate memory for each asteroid
    }
    for (int i = 0; i < nbAsteroid; i++) {
        const float rotation = ((rand() % (int)((bigTraits.maxRotationSpeed) - bigTraits.minRotationSpeed))
            + bigTraits.minRotationSpeed)/100.0f;
        ((BigAsteroid *) asteroids->asteroid[i])->base.isCollisionEnabled = true;
        ((BigAsteroid *) asteroids->asteroid[i])->base.radius = bigTraits.radius; //init radius
        ((BigAsteroid *) asteroids->asteroid[i])->base.spread = bigTraits.spread; //init spread
        ((BigAsteroid *) asteroids->asteroid[i])->base.type = BIG; //init type
        ((BigAsteroid *) asteroids->asteroid[i])->base.angle = 0; //init angle
        ((BigAsteroid *) asteroids->asteroid[i])->base.score = bigTraits.score; //init score
        ((BigAsteroid *) asteroids->asteroid[i])->nbVertices = bigTraits.nbVertices; //init score
        ((BigAsteroid *) asteroids->asteroid[i])->base.rotation = rotation; //init rotation
        randomPosition(&((BigAsteroid *) asteroids->asteroid[i])->base, player); //init pos
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

void randomPosition(AsteroidBase *asteroids, const Player* player) {
    const Vector2 playerPos = PLAYER_GetPosition(player);
    const int spawnRadius = 150;
    Vector2 pos;
    pos.x = rand() % (GetScreenWidth() -spawnRadius*2);
    pos.y = rand() % (GetScreenHeight()-spawnRadius*2);
    if (pos.x > playerPos.x - spawnRadius) pos.x += spawnRadius*2;
    if (pos.y > playerPos.y - spawnRadius) pos.y += spawnRadius*2;
    asteroids->position.x = pos.x;
    asteroids->position.y = pos.y;

}


//draw Asteroids
//-------------------------------------------------------------------------------------
void renderAsteroids(AsteroidArray *arr) {
    if (arr->reservedSize == 0) { return; }
    Vector2 startPos;
    Vector2 endPos;
    const float lineThickness = 3;
    int verticesNb = 0;
    switch (((SmlAsteroid *) arr->asteroid[0])->base.type) {
        case SMALL:
            verticesNb = ((SmlAsteroid *) arr->asteroid[0])->nbVertices;
        for (int i = 0; i < arr->reservedSize; i++) {
            if (!((SmlAsteroid *) arr->asteroid[i])->base.isCollisionEnabled) continue;
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
            for (int i = 0; i < arr->reservedSize; i++) {
                if (!((MidAsteroid *) arr->asteroid[i])->base.isCollisionEnabled) continue;
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
            for (int i = 0; i < arr->reservedSize; i++) {
                if (!((BigAsteroid *) arr->asteroid[i])->base.isCollisionEnabled) continue;
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

void* checkCollisionAstBullet(void* arg) {
    PackageCollisionBullet package = *(PackageCollisionBullet*)arg;
    Vector2 startPos;
    Vector2 endPos;
    const int treshold = 15;
    Vector2 astPos;
    for (int i = 0; i < PLAYER_MAX_BULLETS; ++i) {//chaque balle
        if (package.bullets[i].distance == 0) continue;
        if (package.smlArr->reservedSize == 0) break;
        for (int j = 0; j < package.smlArr->reservedSize; ++j) {//chaque asteroid

            if (!((SmlAsteroid *) package.smlArr->asteroid[j])->base.isCollisionEnabled) continue;

            astPos = ((SmlAsteroid *) package.smlArr->asteroid[j])->base.position;
            for (int k = 0; k < smlTraits.nbVertices - 1; ++k) {// pour chaque point
                if (!((SmlAsteroid *) package.smlArr->asteroid[j])->base.isCollisionEnabled) break;
                startPos = ((SmlAsteroid *) package.smlArr->asteroid[j])->points[k];
                endPos = ((SmlAsteroid *) package.smlArr->asteroid[j])->points[k + 1];
                startPos.x += astPos.x;
                startPos.y += astPos.y;
                endPos.x += astPos.x;
                endPos.y += astPos.y;
                if (CheckCollisionPointLine(package.bullets[i].position, startPos, endPos, treshold)){
                    float angle = atan2f(-package.bullets[i].speed.y, package.bullets[i].speed.x);
                    angle -= M_PI;
                    if (angle < 0) angle += 2*M_PI;
                    if (angle > 2*M_PI) angle -= 2*M_PI;
                    createParticles(angle, package.bullets[i].position, 8, 50.0,  DEFAULT_PARTICLE_COLOR, 0.5, 900);
                    printf("collision petit\n");
                    ((SmlAsteroid *) package.smlArr->asteroid[j])->base.isCollisionEnabled = false;
                    package.bullets[i].speed = (Vector2){0,0};
                    package.bullets[i].distance = 0;
                    *package.score += ((SmlAsteroid *) package.smlArr->asteroid[j])->base.score;
                    package.smlArr->nbAsteroid--;
                }
            }
            startPos = ((SmlAsteroid *) package.smlArr->asteroid[j])->points[smlTraits.nbVertices - 1];
            endPos = ((SmlAsteroid *) package.smlArr->asteroid[j])->points[0];
            startPos.x += astPos.x;
            startPos.y += astPos.y;
            endPos.x += astPos.x;
            endPos.y += astPos.y;

        }
    }
    for (int i = 0; i < PLAYER_MAX_BULLETS; ++i) {//chaque balle
        if (package.bullets[i].distance == 0) continue;
        if (package.midArr->reservedSize == 0) break;
        for (int j = 0; j < package.midArr->reservedSize; ++j) {//chaque asteroid

            if (!((MidAsteroid *) package.midArr->asteroid[j])->base.isCollisionEnabled) continue;

            astPos = ((MidAsteroid *) package.midArr->asteroid[j])->base.position;
            for (int k = 0; k < midTraits.nbVertices - 1; ++k) {// pour chaque point
                if (!((MidAsteroid *) package.midArr->asteroid[j])->base.isCollisionEnabled) break;
                startPos = ((MidAsteroid *) package.midArr->asteroid[j])->points[k];
                endPos = ((MidAsteroid *) package.midArr->asteroid[j])->points[k + 1];
                startPos.x += astPos.x;
                startPos.y += astPos.y;
                endPos.x += astPos.x;
                endPos.y += astPos.y;
                if (CheckCollisionPointLine(package.bullets[i].position, startPos, endPos, treshold)){
                    printf("collision moyen\n");
                    float angle = atan2f(-package.bullets[i].speed.y, package.bullets[i].speed.x);
                    angle -= M_PI;
                    if (angle < 0) angle += 2*M_PI;
                    if (angle > 2*M_PI) angle -= 2*M_PI;
                    createParticles(angle, package.bullets[i].position, 11, 60.0,  DEFAULT_PARTICLE_COLOR, 0.5, 1000);
                    ((MidAsteroid *) package.midArr->asteroid[j])->base.isCollisionEnabled = false;
                    package.bullets[i].speed = (Vector2){0,0};
                    package.bullets[i].distance = 0;
                    *package.score += ((MidAsteroid *) package.midArr->asteroid[j])->base.score;
                    createSmlAsteroid(package.smlArr, 2, ((MidAsteroid *) package.midArr->asteroid[j])->base.position);
                    package.midArr->nbAsteroid--;
                }
            }
            startPos = ((MidAsteroid *) package.midArr->asteroid[j])->points[midTraits.nbVertices - 1];
            endPos = ((MidAsteroid *) package.midArr->asteroid[j])->points[0];
            startPos.x += astPos.x;
            startPos.y += astPos.y;
            endPos.x += astPos.x;
            endPos.y += astPos.y;

        }
    }

    for (int i = 0; i < PLAYER_MAX_BULLETS; ++i) {//chaque balle
        if (package.bullets[i].distance == 0) continue;
        if (package.bigArr->reservedSize == 0) break;
        for (int j = 0; j < package.bigArr->reservedSize; ++j) {//chaque asteroid

            if (!((BigAsteroid *) package.bigArr->asteroid[j])->base.isCollisionEnabled) continue;

            astPos = ((BigAsteroid *) package.bigArr->asteroid[j])->base.position;
            for (int k = 0; k < bigTraits.nbVertices - 1; ++k) {// pour chaque point
                if (!((BigAsteroid *) package.bigArr->asteroid[j])->base.isCollisionEnabled) break;
                startPos = ((BigAsteroid *) package.bigArr->asteroid[j])->points[k];
                endPos = ((BigAsteroid *) package.bigArr->asteroid[j])->points[k + 1];
                startPos.x += astPos.x;
                startPos.y += astPos.y;
                endPos.x += astPos.x;
                endPos.y += astPos.y;
                if (CheckCollisionPointLine(package.bullets[i].position, startPos, endPos, treshold)){
                    float angle = atan2f(-package.bullets[i].speed.y, package.bullets[i].speed.x);
                    angle -= M_PI;
                    if (angle < 0) angle += 2*M_PI;
                    if (angle > 2*M_PI) angle -= 2*M_PI;

                    createParticles(angle, package.bullets[i].position, 13, 70.0,  DEFAULT_PARTICLE_COLOR, 0.6, 900);
                    printf("collision gros\n");
                    ((BigAsteroid *) package.bigArr->asteroid[j])->base.isCollisionEnabled = false;
                    package.bullets[i].speed = (Vector2){0,0};
                    package.bullets[i].distance = 0;
                    *package.score += ((BigAsteroid *) package.bigArr->asteroid[j])->base.score;
                    createMidAsteroid(package.midArr, 2, ((BigAsteroid *) package.bigArr->asteroid[j])->base.position);
                    package.bigArr->nbAsteroid--;
                }
            }
            startPos = ((BigAsteroid *) package.bigArr->asteroid[j])->points[bigTraits.nbVertices - 1];
            endPos = ((BigAsteroid *) package.bigArr->asteroid[j])->points[0];
            startPos.x += astPos.x;
            startPos.y += astPos.y;
            endPos.x += astPos.x;
            endPos.y += astPos.y;

        }
    }
    return NULL;

}

void createMidAsteroid(AsteroidArray *arr, int nbAsteroid, Vector2 position) {
    if (arr->reservedSize == 0) {
        arr->asteroid = malloc(nbAsteroid * sizeof(void*));
        if (arr->asteroid == NULL) {
            printf("Malloc mid Asteroid failed, line 439");
            exit(1);
        }
    } else {
        arr->asteroid = realloc(arr->asteroid, (arr->reservedSize + nbAsteroid) * sizeof(void*));
        if (arr->asteroid == NULL) {
            printf("Realloc mid Asteroid failed, line 445");
            exit(1);
        }
    }
    for (int i = arr->reservedSize; i < arr->reservedSize+nbAsteroid; ++i) {
        arr->asteroid[i] = malloc(sizeof(MidAsteroid));
        printf("Allocating arr->asteroid[%d]\n", i);
        if (arr->asteroid[i] == NULL) {
            printf("Malloc individual [%d] mid Asteroid failed, line 452", i);
            exit(1);
        }
        const float rotation = ((rand() % (int)((midTraits.maxRotationSpeed) - midTraits.minRotationSpeed))
        + midTraits.minRotationSpeed)/100.0f;
        ((MidAsteroid*) arr->asteroid[i])->base.isCollisionEnabled = true;
        ((MidAsteroid*) arr->asteroid[i])->base.radius = midTraits.radius; //init radius
        ((MidAsteroid*) arr->asteroid[i])->base.spread = midTraits.spread; //init spread
        ((MidAsteroid*) arr->asteroid[i])->base.type = MEDIUM; //init type
        ((MidAsteroid*) arr->asteroid[i])->base.angle = 0; //init angle
        ((MidAsteroid*) arr->asteroid[i])->base.score = midTraits.score; //init score
        ((SmlAsteroid*) arr->asteroid[i])->nbVertices = midTraits.nbVertices;; //init pos
        ((MidAsteroid*) arr->asteroid[i])->base.rotation = rotation; //init rotation
        ((MidAsteroid*) arr->asteroid[i])->base.position = position;; //init pos
        randomSpeed(&((MidAsteroid*) arr->asteroid[i])->base, MEDIUM); //init speed

        generateVertices(arr->asteroid[i], midTraits.nbVertices, midTraits.generationStyle); //init points
    }
    arr->reservedSize += nbAsteroid;
    arr->nbAsteroid += nbAsteroid;
}

void createSmlAsteroid(AsteroidArray *arr, int nbAsteroid, Vector2 position) {
    if (arr->reservedSize == 0) {
        arr->asteroid = malloc(nbAsteroid * sizeof(void*));
        if (arr->asteroid == NULL) {
            printf("Malloc mid Asteroid failed, line 439");
            exit(1);
        }
    } else {
        arr->asteroid = realloc(arr->asteroid, (arr->reservedSize + nbAsteroid) * sizeof(void*));
        if (arr->asteroid == NULL) {
            printf("Realloc mid Asteroid failed, line 445");
            exit(1);
        }
    }
    for (int i = arr->reservedSize; i < arr->reservedSize+nbAsteroid; ++i) {
        arr->asteroid[i] = malloc(sizeof(SmlAsteroid));
        printf("Allocating arr->asteroid[%d]\n", i);
        if (arr->asteroid[i] == NULL) {
            printf("Malloc individual [%d] mid Asteroid failed, line 452", i);
            exit(1);
        }
        const float rotation = ((rand() % (int)((smlTraits.maxRotationSpeed) - smlTraits.minRotationSpeed))
        + smlTraits.minRotationSpeed)/100.0f;
        ((SmlAsteroid*) arr->asteroid[i])->base.isCollisionEnabled = true;
        ((SmlAsteroid*) arr->asteroid[i])->base.radius = smlTraits.radius; //init radius
        ((SmlAsteroid*) arr->asteroid[i])->base.spread = smlTraits.spread; //init spread
        ((SmlAsteroid*) arr->asteroid[i])->base.type = SMALL; //init type
        ((SmlAsteroid*) arr->asteroid[i])->base.angle = 0; //init angle
        ((SmlAsteroid*) arr->asteroid[i])->base.score = smlTraits.score; //init score
        ((SmlAsteroid*) arr->asteroid[i])->base.rotation = rotation; //init rotation
        ((SmlAsteroid*) arr->asteroid[i])->base.position = position;; //init pos
        ((SmlAsteroid*) arr->asteroid[i])->nbVertices = smlTraits.nbVertices;; //init pos
        randomSpeed(&((SmlAsteroid*) arr->asteroid[i])->base, SMALL); //init speed

        generateVertices(arr->asteroid[i], smlTraits.nbVertices, smlTraits.generationStyle); //init points
    }
    arr->reservedSize += nbAsteroid;
    arr->nbAsteroid += nbAsteroid;
}


void* checkCollisionAstPlayer(void *arg) {
    PackageCollisionPlayer package = *(PackageCollisionPlayer*) arg;
    Player *player = package.player;
    Vector2 startPos;
    Vector2 endPos;
    Vector2 astPos;
    for (int i = 0; i < package.bigArr->reservedSize; ++i) {
        for (int j = 0; j < ((BigAsteroid*)package.bigArr->asteroid[i])->nbVertices-1; ++j) {
            if (!((BigAsteroid *) package.bigArr->asteroid[i])->base.isCollisionEnabled) continue;
            astPos = ((BigAsteroid *) package.bigArr->asteroid[i])->base.position;
            startPos = ((BigAsteroid *) package.bigArr->asteroid[i])->points[j];
            endPos = ((BigAsteroid *) package.bigArr->asteroid[i])->points[j + 1];
            startPos.x += astPos.x;
            startPos.y += astPos.y;
            endPos.x += astPos.x;
            endPos.y += astPos.y;
            if (PLAYER_IsLineInBounds(player, &startPos, &endPos)) {
                createParticles(0, PLAYER_GetPosition(player), 80, 360.0, ORANGE, 1.0, 1300);
                PLAYER_Die(player);
                package.bigArr->nbAsteroid--;
                createMidAsteroid(package.midArr, 2, ((BigAsteroid*)package.bigArr->asteroid[i])->base.position);
                ((BigAsteroid *) package.bigArr->asteroid[i])->base.isCollisionEnabled = false;
                return NULL;
            }
        }
    }
    for (int i = 0; i < package.midArr->reservedSize; ++i) {
        for (int j = 0; j < ((MidAsteroid*)package.midArr->asteroid[i])->nbVertices-1; ++j) {
            if (!((MidAsteroid *) package.midArr->asteroid[i])->base.isCollisionEnabled) continue;
            astPos = ((MidAsteroid *) package.midArr->asteroid[i])->base.position;
            startPos = ((MidAsteroid *) package.midArr->asteroid[i])->points[j];
            endPos = ((MidAsteroid *) package.midArr->asteroid[i])->points[j + 1];
            startPos.x += astPos.x;
            startPos.y += astPos.y;
            endPos.x += astPos.x;
            endPos.y += astPos.y;
            if (PLAYER_IsLineInBounds(player, &startPos, &endPos)) {
                createParticles(0, PLAYER_GetPosition(package.player), 80, 360.0, ORANGE, 0.9, 1300);
                PLAYER_Die(package.player);
                package.midArr->nbAsteroid--;
                createSmlAsteroid(package.smlArr, 2, ((MidAsteroid*)package.midArr->asteroid[i])->base.position);
                ((MidAsteroid *) package.midArr->asteroid[i])->base.isCollisionEnabled = false;
                return NULL;
                }
        }
    }
    for (int i = 0; i < package.smlArr->reservedSize; ++i) {
        for (int j = 0; j < ((SmlAsteroid*)package.smlArr->asteroid[i])->nbVertices-1; ++j) {
            if (!((SmlAsteroid *) package.smlArr->asteroid[i])->base.isCollisionEnabled) continue;
            astPos = ((SmlAsteroid *) package.smlArr->asteroid[i])->base.position;
            startPos = ((SmlAsteroid *) package.smlArr->asteroid[i])->points[j];
            endPos = ((SmlAsteroid *) package.smlArr->asteroid[i])->points[j + 1];
            startPos.x += astPos.x;
            startPos.y += astPos.y;
            endPos.x += astPos.x;
            endPos.y += astPos.y;
            if (PLAYER_IsLineInBounds(player, &startPos, &endPos)) {
                createParticles(0, PLAYER_GetPosition(package.player), 80, 360.0, ORANGE, 0.8, 1300);
                PLAYER_Die(package.player);
                package.smlArr->nbAsteroid--;
                ((SmlAsteroid *) package.smlArr->asteroid[i])->base.isCollisionEnabled = false;
                return NULL;
                }
        }
    }
    return NULL;
}

void freeAsteroidArray(AsteroidArray *arr, AsteroidType type) {
    if (arr == NULL) return;
    for (int i = 0; i < arr->reservedSize; i++) {
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
    arr->reservedSize = 0;
    arr->nbAsteroid = 0;
}
