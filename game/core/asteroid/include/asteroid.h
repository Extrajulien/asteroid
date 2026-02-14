#pragma once
#include "asteroid_size.h"
#include "components.h"
#include "entity_info.h"
#include "particle.h"
#include "asteroid_vertice_array.h"
#include "asteroid_vertice_pool.h"

typedef struct AsteroidPreset AsteroidPreset;
typedef struct Bullet Bullet;

typedef struct {
    float radius;
    unsigned int nbVertices;
    Vector2 *originalVertices;
    Vector2 *vertices;
} AsteroidShape;

typedef struct Asteroid {
    AsteroidSize type;
    size_t verticeArrayIndex;
    float radius;
    EntityInfo info;
    Position position;
    Velocity velocity;
    Rotation rotation;
    ComponentLineInfo lineInfo;
    ParticlePreset particlePreset;
    Score score;
} Asteroid;

Asteroid ASTEROID_Create(const AsteroidPreset *preset, VerticePool *pool);
Asteroid ASTEROID_GetZeroInitializedAsteroid();
void ASTEROID_Free(Asteroid *asteroid, VerticePool *pool);
void ASTEROID_Rotate(const Asteroid *asteroid, VerticePool *pool);
void ASTEROID_MoveTo(Asteroid *asteroid, Vector2 position, VerticePool *pool);

// mark the asteroid as STATE_DEAD and free the vertices arrays
void ASTEROID_Remove(Asteroid *asteroid, VerticePool *pool);
void ASTEROID_UpdateVertices(const Asteroid *asteroid, VerticePool *pool);
void ASTEROID_Render(const Asteroid *asteroid, VerticePool *pool);

bool ASTEROID_IsBulletColliding(const Asteroid *asteroid, const Bullet *bullet, VerticePool *pool);

void* checkCollisionAstPlayer(void* arg);