#include "asteroid.h"
#include "asteroid_array.h"
#include "asteroid_preset.h"
#include "bullet.h"
#include "player.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "raymath.h"

void generateVertices(const Asteroid *asteroid, const AsteroidPreset *preset, VerticePool *pool);


// create an asteroid from the preset given (does not set a position or speed)
Asteroid ASTEROID_Create(const AsteroidPreset *preset, VerticePool *pool) {
    Asteroid asteroid = {};
    const int randomRotation = (int)(preset->maxRotationSpeed - preset->minRotationSpeed) == 0 ? 0 : rand() % (int)(preset->maxRotationSpeed - preset->minRotationSpeed);
    asteroid.rotation.increment = (randomRotation + preset->minRotationSpeed) / PRESET_ROTATION_PRECISION;
    asteroid.rotation.rad = 0;
    asteroid.info.state = STATE_ALIVE;
    asteroid.radius = preset->radius;
    asteroid.type = preset->size;
    asteroid.score = preset->score;
    asteroid.verticeArrayIndex = VERTICE_POOL_ReserveIndex(pool); // get from the pool the next free index
    VerticeArray *vert = VERTICE_POOL_GetVerticeArray(pool, asteroid.verticeArrayIndex);
    vert->count = preset->nbVertices;
    VERTICE_ARRAY_Allocate();
    asteroid.particlePreset = preset->particlePreset;
    asteroid.lineInfo = preset->lineInfo;
    generateVertices(&asteroid, preset, pool);
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
        .radius = 0,
        .verticeArrayIndex = 0,
        .velocity = (Velocity) {0,0},
    };
}

void ASTEROID_Render(const Asteroid *asteroid, VerticePool *pool) {
    if (!asteroid) return;
    const VerticeArray* vert = VERTICE_POOL_GetVerticeArray(pool, asteroid->verticeArrayIndex);
    if (!vert->isUsed || vert->count <=0) {
        LOG("$Rfaulty call of asteroid render\n");
        return;
    }

    for (int j = 0; j < vert->count; ++j) {
        const Vector2 startPos = vert->vertices[j];
        const Vector2 endPos = vert->vertices[(j + 1) % vert->count];
        DrawLineEx(startPos, endPos, asteroid->lineInfo.thickness, asteroid->lineInfo.color);
    }
}

void ASTEROID_Rotate(const Asteroid *asteroid, VerticePool *pool) {
    const VerticeArray *vert = VERTICE_POOL_GetVerticeArray(pool, asteroid->verticeArrayIndex);
    const float cos = cosf(asteroid->rotation.rad);
    const float sin = sinf(asteroid->rotation.rad);

    for (int i = 0; i < vert->count; i++) {
        vert->vertices[i].x = vert->originalVertices[i].x * cos - vert->originalVertices[i].y * sin;
        vert->vertices[i].y = vert->originalVertices[i].x * sin + vert->originalVertices[i].y * cos;
    }
}

void ASTEROID_MoveVertices(const Asteroid *asteroid, VerticePool *pool) {
    const VerticeArray *vert = VERTICE_POOL_GetVerticeArray(pool, asteroid->verticeArrayIndex);

    for (int i = 0; i < vert->count; ++i) {
        vert->vertices[i].x = vert->vertices[i].x + asteroid->position.x;
        vert->vertices[i].y = vert->vertices[i].y + asteroid->position.y;
    }
}

void ASTEROID_MoveTo(Asteroid *asteroid, const Vector2 position, VerticePool *pool) {
    const VerticeArray *vert = VERTICE_POOL_GetVerticeArray(pool, asteroid->verticeArrayIndex);

    asteroid->position = position;
    for (int i = 0; i < vert->count; ++i) {
        vert->vertices[i] = Vector2Add(vert->originalVertices[i], position);
    }
}

void ASTEROID_Remove(Asteroid *asteroid, VerticePool *pool) {
    if (asteroid) {
        LOGF_MEMORY("ASTEROID_REMOVE ($RKILL$o) asteroid $O[SIZE]$o:%d\n", asteroid->type);
        VERTICE_POOL_ReleaseIndex(pool, asteroid->verticeArrayIndex);
        asteroid->verticeArrayIndex = 0;
        asteroid->type = -1;
        asteroid->info.state = STATE_DEAD;
    }
}

void generateVertices(const Asteroid *asteroid, const AsteroidPreset *preset, VerticePool *pool) {
    VerticeArray *vert = VERTICE_POOL_GetVerticeArray(pool, asteroid->verticeArrayIndex);
    VERTICE_ARRAY_LoadPreset(vert, preset);
    ASTEROID_MoveVertices(asteroid, pool);
}

bool ASTEROID_IsBulletColliding(const Asteroid *asteroid, const Bullet *bullet, VerticePool *pool) {
    const VerticeArray *vert = VERTICE_POOL_GetVerticeArray(pool, asteroid->verticeArrayIndex);

    for (int k = 0; k < vert->count; ++k) {
        const int threshold = 10;
        const Vector2 startPos = vert->vertices[k];
        const Vector2 endPos = vert->vertices[(k + 1) % vert->count];
        const Rectangle bulletHitBox = BULLET_GetCollisionRectangle(bullet);
        if (CheckCollisionPointLine((Vector2){bulletHitBox.x, bulletHitBox.y}, startPos, endPos, threshold)) {
            return true;
        }
    }
    return false;
}

bool ASTEROID_IsPlayerColliding(const Asteroid *asteroid, const Player *player, const VerticePool *pool) {
    if (asteroid->info.state == STATE_DEAD) return false;
    const VerticeArray* vert = VERTICE_POOL_GetVerticeArray(pool, asteroid->verticeArrayIndex);
    for (int j = 0; j < vert->count; ++j) {
        const int threshold = 10;
        const Vector2 startPos = vert->vertices[j];
        const Vector2 endPos = vert->vertices[(j + 1) % vert->count];

        if (PLAYER_IsLineInBounds(player, &startPos, &endPos)) {
            return true;
        }
    }
    return false;
}

void ASTEROID_Free(Asteroid *asteroid, VerticePool *pool) {
    if (asteroid && asteroid->info.state != STATE_DEAD) {
        VerticeArray* vert = VERTICE_POOL_GetVerticeArray(pool, asteroid->verticeArrayIndex);
        if (vert->isUsed && vert->capacity > 0) {
            LOGF_MEMORY("Freed asteroid $O[SIZE]$o:%d\n", asteroid->type);
            LOGF_MEMORY("FREE Varray $bidx: $B%u$b, isUsed: $B%d$b, count: $B%u$b, capacity: $B%u$b \n", asteroid->verticeArrayIndex, vert->isUsed, vert->count, vert->capacity);
            asteroid->verticeArrayIndex = 0;
            VERTICE_ARRAY_Free(vert);
            asteroid->type = -1;
        }
    }
}
