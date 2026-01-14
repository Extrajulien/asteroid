#include "asteroid.h"
#include "asteroid_array.h"
#include "asteroid_preset.h"
#include "bullet.h"
#include "player.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "raymath.h"

void generateVertices(Asteroid *asteroid, const AsteroidPreset *preset);
void ASTEROID_SHAPE_Init(Asteroid *asteroid, const AsteroidPreset *preset);

// create an asteroid from the preset given (does not set a position or speed)
Asteroid ASTEROID_Create(const AsteroidPreset *preset) {
    Asteroid asteroid = {};
    int randomRotation = (int)(preset->maxRotationSpeed - preset->minRotationSpeed) == 0 ? 0 : rand() % (int)(preset->maxRotationSpeed - preset->minRotationSpeed);
    asteroid.rotation.increment = (randomRotation + preset->minRotationSpeed) / PRESET_ROTATION_PRECISION;
    asteroid.rotation.rad = 0;
    asteroid.info.state = STATE_ALIVE;
    asteroid.shape.radius = preset->radius;
    asteroid.type = preset->size;
    asteroid.score = preset->score;
    asteroid.shape.nbVertices = preset->nbVertices;
    asteroid.particlePreset = preset->particlePreset;
    asteroid.lineInfo = preset->lineInfo;
    generateVertices(&asteroid, preset);
    return asteroid;
}

Asteroid ASTEROID_GetZeroInitializedAsteroid() {
    return (Asteroid) {
        .type = SIZE_BIG,
        .info = STATE_DEAD,
        .lineInfo = (ComponentLineInfo) {
            .color = RED,
            .thickness = 5
        },
        .particlePreset = (ParticlePreset) {
            .color = RED,
            .angleSpread = 30,
            .lifetime = 1000,
            .quantity = 10,
            .speed = 100
        },
        .position = (Position) {0,0},
        .rotation = 0,
        .score = 0,
        .shape = (AsteroidShape) {
            .nbVertices = 0,
            .originalVertices = NULL,
            .vertices = NULL
        },
        .velocity = (Velocity) {0,0},
    };
}

void ASTEROID_Render(const Asteroid *asteroid) {
    for (int j = 0; j < asteroid->shape.nbVertices; ++j) {
        const Vector2 startPos = asteroid->shape.vertices[j];
        const Vector2 endPos = asteroid->shape.vertices[(j + 1) % asteroid->shape.nbVertices];
        DrawLineEx(startPos, endPos, asteroid->lineInfo.thickness, asteroid->lineInfo.color);
    }
}

void ASTEROID_Rotate(const Asteroid *asteroid) {
    const float cos = cosf(asteroid->rotation.rad);
    const float sin = sinf(asteroid->rotation.rad);

    for (int i = 0; i < asteroid->shape.nbVertices; i++) {
        asteroid->shape.vertices[i].x = asteroid->shape.originalVertices[i].x * cos - asteroid->shape.originalVertices[i].y * sin;
        asteroid->shape.vertices[i].y = asteroid->shape.originalVertices[i].x * sin + asteroid->shape.originalVertices[i].y * cos;
    }
}

void ASTEROID_UpdateVertices(const Asteroid *asteroid) {
    for (int i = 0; i < asteroid->shape.nbVertices; ++i) {
        asteroid->shape.vertices[i].x = asteroid->shape.originalVertices[i].x + asteroid->position.x;
        asteroid->shape.vertices[i].y = asteroid->shape.originalVertices[i].y + asteroid->position.y;
    }
}

void ASTEROID_SHAPE_Init(Asteroid *asteroid, const AsteroidPreset *preset) {
    Vector2 *newVertices = malloc(sizeof(Vector2) * asteroid->shape.nbVertices);
    Vector2 *newOriginal = malloc(sizeof(Vector2) * asteroid->shape.nbVertices);

    if (!newVertices || !newOriginal) {
        free(newVertices);
        free(newOriginal);
        return;
    }

    free(asteroid->shape.vertices);
    free(asteroid->shape.originalVertices);

    asteroid->shape.vertices = newVertices;
    asteroid->shape.originalVertices = newOriginal;
    asteroid->shape.nbVertices = preset->nbVertices;
}

void ASTEROID_MoveTo(Asteroid *asteroid, const Vector2 position) {
    asteroid->position = position;
    for (int i = 0; i < asteroid->shape.nbVertices; ++i) {
        asteroid->shape.vertices[i] = Vector2Add(asteroid->shape.originalVertices[i], position);
    }
}

void generateVertices(Asteroid *asteroid, const AsteroidPreset *preset) {
    ASTEROID_SHAPE_Init(asteroid, preset);
    const float radius = asteroid->shape.radius;
    const float spread = preset->spread;
    float distanceFromCenter;

    const float radSpacing = (2 * PI) / (float) asteroid->shape.nbVertices;
    for (int i = 0; i < asteroid->shape.nbVertices; i++) {
        if (preset->isRandomBothSides) {
            const float rndSpread = ((float)rand() / (float)RAND_MAX) * spread*2;
            distanceFromCenter = rndSpread + radius - spread;
        } else {
            const float rndSpread = ((float)rand() / (float)RAND_MAX) * spread;
            distanceFromCenter = rndSpread + radius;
        }
        asteroid->shape.originalVertices[i].x = cosf(radSpacing * (float) i) * distanceFromCenter;
        asteroid->shape.originalVertices[i].y = sinf(radSpacing * (float) i) * distanceFromCenter;
    }
}

// TODO MAKE COLLISION SAT
bool ASTEROID_IsBulletColliding(const Asteroid *asteroid, const Bullet *bullet) {
    for (int k = 0; k < asteroid->shape.nbVertices; ++k) {
        const int threshold = 10;
        const Vector2 startPos = asteroid->shape.vertices[k];
        const Vector2 endPos = asteroid->shape.vertices[(k + 1) % asteroid->shape.nbVertices];
        Rectangle bulletHitBox = BULLET_GetCollisionRectangle(bullet);
        if (CheckCollisionPointLine((Vector2){bulletHitBox.x, bulletHitBox.y}, startPos, endPos, threshold)) {
            return true;
        }
    }
    return false;
}


void* checkCollisionAstPlayer(void *arg) {
    PackageCollisionPlayer package = *(PackageCollisionPlayer*) arg;
    Player *player = package.player;
    Vector2 startPos;
    Vector2 endPos;
    Vector2 astPos;
    /*
    for (int i = 0; i < package.bigArr->capacity; ++i) {
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
    for (int i = 0; i < package.midArr->capacity; ++i) {
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
    for (int i = 0; i < package.smlArr->capacity; ++i) {
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
    */
    return NULL;
}

void ASTEROID_Free(const Asteroid *asteroid) {
    if (asteroid) {
        if (asteroid->shape.originalVertices) {
            free(asteroid->shape.originalVertices);
        }
        if (asteroid->shape.vertices) {
            free(asteroid->shape.vertices);
        }
    }
}
