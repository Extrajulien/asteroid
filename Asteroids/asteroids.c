//
// Created by 2485548 on 2025-04-14.
//

#include "asteroids.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 256
static AsteroidTraits bigTraits = {BIG,0,0,0,0,0};
static AsteroidTraits midTraits = {MEDIUM,0,0,0,0,0};
static AsteroidTraits smlTraits = {SMALL,0,0,0,0,0};

//TODO -----
// - Creer les collision peut tirer dessus peut tuer le joueur
// -
// - si ben temps libre asteroides peuvent se foncer dedans et repondir

void moveAsteroids(AsteroidArray *asteroidArr) {
    if (asteroidArr->size == 0) {return;}
    for (int i = 0; i < asteroidArr->size; i++) {
        ((SmlAsteroid*)asteroidArr->asteroid[i])->base.position.x +=
            ((SmlAsteroid*)asteroidArr->asteroid[i])->base.speed.x * GetFrameTime() * 120;
        ((SmlAsteroid*)asteroidArr->asteroid[i])->base.position.y +=
            ((SmlAsteroid*)asteroidArr->asteroid[i])->base.speed.y * GetFrameTime() * 120;
    }
}

void wrapAroundAsteroid(AsteroidArray *asteroidArr) {
    if (asteroidArr->size == 0) {return;}
    for (int i = 0; i < asteroidArr->size; i++) {
        float radius = ((SmlAsteroid*)asteroidArr->asteroid[i])->base.radius;
        Vector2 pos = {((SmlAsteroid*)asteroidArr->asteroid[i])->base.position.x,
        ((SmlAsteroid*)asteroidArr->asteroid[i])->base.position.y};

        if (pos.x > GetScreenWidth() + radius) ((SmlAsteroid*)asteroidArr->asteroid[i])->base.position.x = -radius;
        if (pos.y > GetScreenHeight() + radius) ((SmlAsteroid*)asteroidArr->asteroid[i])->base.position.y = -radius;
        if (pos.x < -radius) ((SmlAsteroid*)asteroidArr->asteroid[i])->base.position.x = GetScreenWidth() + radius;
        if (pos.y < -radius) ((SmlAsteroid*)asteroidArr->asteroid[i])->base.position.y = GetScreenHeight() + radius;
    }
}

void rotateAsteroid(AsteroidArray *asteroidArr) {
    if (asteroidArr->size == 0) {return;}
    for (int i = 0; i < asteroidArr->size; i++) {
        ((SmlAsteroid*)asteroidArr->asteroid[i])->base.angle +=
            ((SmlAsteroid*)asteroidArr->asteroid[i])->base.rotation * GetFrameTime();

        if (((SmlAsteroid*)asteroidArr->asteroid[i])->base.angle >= 2*PI) {
            ((SmlAsteroid*)asteroidArr->asteroid[i])->base.angle = 0;
        }
        if (((SmlAsteroid*)asteroidArr->asteroid[i])->base.angle < 0) {
            ((SmlAsteroid*)asteroidArr->asteroid[i])->base.angle = 2*PI;
        }
        rotateAsteroidVertices(asteroidArr->asteroid[i]);
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
    const float radius = 100;
    const float spread = 20;
    const int minRotationSpeed = -30;
    const int maxRotationSpeed = 30-10;
    asteroids -> size += nbAsteroid;
    asteroids -> asteroid = malloc(nbAsteroid * sizeof(void *));//init asteroid arr

    for (int i = 0; i < nbAsteroid; i++) {
        asteroids -> asteroid[i] = malloc(sizeof(BigAsteroid));  // Allocate memory for each asteroid
    }
    for (int i = 0; i < nbAsteroid; i++) {
        const float rotation = ((rand() % (maxRotationSpeed - minRotationSpeed)) + minRotationSpeed)/50.0f;// -300 to 300

        ((BigAsteroid*)asteroids -> asteroid[i]) -> base.radius = bigTraits.radius;      //init radius
        ((BigAsteroid*)asteroids -> asteroid[i]) -> base.spread = spread;      //init spread
        ((BigAsteroid*)asteroids -> asteroid[i]) -> base.type = BIG;           //init type
        ((BigAsteroid*)asteroids -> asteroid[i]) -> base.angle = 0;            //init angle
        ((BigAsteroid*)asteroids -> asteroid[i]) -> base.score = 20;           //init score
        ((BigAsteroid*)asteroids -> asteroid[i]) -> base.rotation = rotation;  //init rotation
        randomPosition(&((BigAsteroid*)asteroids -> asteroid[i]) -> base);     //init pos
        randomSpeed(&((BigAsteroid*)asteroids    -> asteroid[i]) -> base);     //init speed

        generateVertices(asteroids -> asteroid[i]);//init points
    }
    printf("--Big asteroids generated!\n");

}

//asteroid
//-------------------------------------------------------------------------------------


void rotateAsteroidVertices(void *asteroid) {
    Vector2 *ppoints;

    float angle = (((SmlAsteroid*)asteroid)->base.angle);
    int verticesNb = 0;
    switch (((SmlAsteroid*)asteroid)->base.type) {
        case SMALL:
        verticesNb = SML_VERTICES;
        for (int i = 0; i < verticesNb;i++) {
            ((SmlAsteroid*)asteroid)->points[i].x = ((SmlAsteroid*)asteroid)->generatedPoints[i].x;
            ((SmlAsteroid*)asteroid)->points[i].y = ((SmlAsteroid*)asteroid)->generatedPoints[i].y;
        }
        ppoints = ((SmlAsteroid*)asteroid)->points;
        break;
        case MEDIUM:
        verticesNb = MID_VERTICES;
        for (int i = 0; i < verticesNb;i++) {
            ((MidAsteroid*)asteroid)->points[i].x = ((MidAsteroid*)asteroid)->generatedPoints[i].x;
            ((MidAsteroid*)asteroid)->points[i].y = ((MidAsteroid*)asteroid)->generatedPoints[i].y;
        }
        ppoints = ((MidAsteroid*)asteroid)->points;
        break;
        case BIG:
        verticesNb = BIG_VERTICES;
        for (int i = 0; i < verticesNb;i++) {
            ((BigAsteroid*)asteroid)->points[i].x = ((BigAsteroid*)asteroid)->generatedPoints[i].x;
            ((BigAsteroid*)asteroid)->points[i].y = ((BigAsteroid*)asteroid)->generatedPoints[i].y;
        }
        ppoints = ((BigAsteroid*)asteroid)->points;
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

void generateVertices(void *asteroid) {
    const float radius = ((SmlAsteroid*)asteroid)->base.radius;
    const float spread = ((SmlAsteroid*)asteroid)->base.spread;
    Vector2 *ppoints;
    float radSpacing = 0;
    int verticesNb = 0;
    switch (((SmlAsteroid*)asteroid)->base.type) {
        case SMALL:
            printf("Vertex --Small\n");
            verticesNb = SML_VERTICES;
            ppoints = ((SmlAsteroid*)asteroid)->generatedPoints;
            break;
        case MEDIUM:
            printf("Vertex --Medium\n");
            verticesNb = MID_VERTICES;
            ppoints = ((MidAsteroid*)asteroid)->generatedPoints;
            break;
        case BIG:
            printf("Vertex --Big\n");
            verticesNb = BIG_VERTICES;
            ppoints = ((BigAsteroid*)asteroid)->generatedPoints;
            break;
        default:
            printf("Unknown Asteroid type!\n");
            exit(1);
    }
    radSpacing = (2 * PI)/verticesNb;
    for (int i = 0; i < verticesNb; i++) {
        const float rndSpread = (rand() % ((int)(spread*100) +1))/100.0f;
        const float distanceFromCenter = rndSpread + radius;
        ppoints[i].x = cosf(radSpacing * i) * distanceFromCenter;
        ppoints[i].y = sinf(radSpacing * i) * distanceFromCenter;
        printf("p%d, (%.2f, %.2f)\n",i ,ppoints[i].x, ppoints[i].y);
    }
}

void randomSpeed(AsteroidBase *asteroids) {
    Vector2 speed;
    speed.x = ((rand() % (MAX_ASTEROID_SPEED*200))-100)/100.0f;
    speed.y = ((rand() % (MAX_ASTEROID_SPEED*200))-100)/100.0f;
    asteroids -> speed.x = speed.x;
    asteroids -> speed.y = speed.y;
    printf("speed (%.2f, %.2f)\n",speed.x ,speed.y);
}

void randomPosition(AsteroidBase *asteroids) {
    asteroids->position.x = rand() % GetScreenWidth();
    asteroids->position.y = rand() % GetScreenHeight();
}


//draw Asteroids
//-------------------------------------------------------------------------------------
void renderAsteroids(AsteroidArray *arr) {
    if (arr->size == 0) {return;}
    Vector2 startPos;
    Vector2 endPos;
    const float lineThickness = 3;
    int verticesNb = 0;
    switch (((SmlAsteroid*)arr->asteroid[0])->base.type) {
        case SMALL:
        verticesNb = SML_VERTICES;
        for (int i = 0; i < arr->size; i++) {//for each asteroid
            Vector2 pos = ((SmlAsteroid*)arr->asteroid[i]) ->base.position;
            for (int j = 0; j < verticesNb-1; ++j) {
                startPos = ((SmlAsteroid*)arr->asteroid[i])->points[j];
                endPos = ((SmlAsteroid*)arr->asteroid[i])->points[j+1];
                startPos.x += pos.x;
                startPos.y += pos.y;
                endPos.x += pos.x;
                endPos.y += pos.y;
                DrawLineEx(startPos, endPos , lineThickness, ASTEROID_COLOR);

            }
            startPos = ((SmlAsteroid*)arr->asteroid[i])->points[verticesNb-1];
            endPos = ((SmlAsteroid*)arr->asteroid[i])->points[0];
            startPos.x += pos.x;
            startPos.y += pos.y;
            endPos.x += pos.x;
            endPos.y += pos.y;
            DrawLineEx(startPos, endPos , lineThickness, ASTEROID_COLOR);
        }
        return;
        case MEDIUM:
        verticesNb = MID_VERTICES;
        for (int i = 0; i < arr->size; i++) {//for each asteroid
            Vector2 pos = ((MidAsteroid*)arr->asteroid[i]) ->base.position;
            for (int j = 0; j < verticesNb-1; ++j) {
                startPos = ((MidAsteroid*)arr->asteroid[i])->points[j];
                endPos = ((MidAsteroid*)arr->asteroid[i])->points[j+1];
                startPos.x += pos.x;
                startPos.y += pos.y;
                endPos.x += pos.x;
                endPos.y += pos.y;
                DrawLineEx(startPos, endPos , lineThickness, ASTEROID_COLOR);

            }
            startPos = ((MidAsteroid*)arr->asteroid[i])->points[verticesNb-1];
            endPos = ((MidAsteroid*)arr->asteroid[i])->points[0];
            startPos.x += pos.x;
            startPos.y += pos.y;
            endPos.x += pos.x;
            endPos.y += pos.y;
            DrawLineEx(startPos, endPos , lineThickness, ASTEROID_COLOR);
        }
        return;
        case BIG:
        verticesNb = BIG_VERTICES;
        for (int i = 0; i < arr->size; i++) {//for each asteroid
            Vector2 pos = ((BigAsteroid*)arr->asteroid[i]) ->base.position;
            for (int j = 0; j < verticesNb-1; ++j) {
                startPos = ((BigAsteroid*)arr->asteroid[i])->points[j];
                endPos = ((BigAsteroid*)arr->asteroid[i])->points[j+1];
                startPos.x += pos.x;
                startPos.y += pos.y;
                endPos.x += pos.x;
                endPos.y += pos.y;
                DrawLineEx(startPos, endPos , lineThickness, ASTEROID_COLOR);

            }
            startPos = ((BigAsteroid*)arr->asteroid[i])->points[verticesNb-1];
            endPos = ((BigAsteroid*)arr->asteroid[i])->points[0];
            startPos.x += pos.x;
            startPos.y += pos.y;
            endPos.x += pos.x;
            endPos.y += pos.y;
            DrawLineEx(startPos, endPos , lineThickness, ASTEROID_COLOR);
        }
        return;
        default:
            printf("Unknown Asteroid type!\n");
        exit(1);
    }
}


/*
void createMidAsteroid(MidAsteroidArray **asteroids) {
    const float radius = 30;
}

void createSmlAsteroid(SmlAsteroidArray **asteroids) {
    const float radius = 15;
}

void bigAsteroidShot(BigAsteroidArray **asteroid, MidAsteroidArray **newAsteroid, short index) {
    const int splitNumber = 2;
    for (int i = 0; i < splitNumber; i++) {
        createMidAsteroid(newAsteroid);
    }
}

void midAsteroidShot(MidAsteroidArray **asteroid, SmlAsteroidArray **newAsteroid, short index) {
    const int splitNumber = 2;
    for (int i = 0; i < splitNumber; i++) {
        createSmlAsteroid(newAsteroid);
    }
}

void smlAsteroidShot(SmlAsteroidArray **asteroid, short index) {

}
*/

void updateAsteroidsTraits() {
    const int maxSemicolon = 7;
    const int indexName = 1;
    const int indexType = 2;
    const int indexRadius = 3;
    const int indexSpread = 4;
    const int indexMinRotationSpeed = 5;
    const int indexMaxRotationSpeed = 6;
    const int indexScore = 7;
    char buffer[BUFFER_SIZE];
    char *filename = "../asteroidsTraits.csv";
    FILE *asteroidsTraits = fopen(filename, "r");
    if (asteroidsTraits == NULL) {
        printf("le fichier '%s' n'existe pas!\n", filename);
        exit(1);
    }

    fgets(buffer, BUFFER_SIZE, asteroidsTraits);//skip titres
    while (fgets(buffer, BUFFER_SIZE, asteroidsTraits)) {
        if (buffer[0] != '1') continue;// ne prend en consideration que ceux qui sont actifs
        int indexEntry = 0;
        char *entry[maxSemicolon+1];
        char *token = strtok(buffer, ";");

        while (token && indexEntry < maxSemicolon+1) {
            entry[indexEntry++] = token;
            token = strtok(NULL, ";");
        }

        AsteroidTraits *pTraits = NULL;
        switch (entry[indexType][0]) {
            case 'B': pTraits = &bigTraits;break;
            case 'M': pTraits = &midTraits;break;
            case 'S': pTraits = &smlTraits;break;
            default:
                printf("le type d'astéroide 'S/M/B' est invalide");
            exit(1);
        }

        pTraits->radius = atof(entry[indexRadius]);
        pTraits->spread = atof(entry[indexSpread]);
        pTraits->minRotationSpeed = atof(entry[indexMinRotationSpeed]);
        pTraits->maxRotationSpeed = atof(entry[indexMaxRotationSpeed]);
        pTraits->score = atoi(entry[indexScore]);

        printf("radius:\t\t %.2f\n", pTraits->radius);
        printf("Spread:\t\t %.2f\n", pTraits->spread);
        printf("Min Spin Speed:\t %.2f\n", pTraits->minRotationSpeed);
        printf("Max Spin Speed:\t %.2f\n", pTraits->maxRotationSpeed);
        printf("Score:\t\t %d\n\n", pTraits->score);
    }
    fclose(asteroidsTraits);
}