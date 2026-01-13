#pragma once
#include "asteroid_size.h"
#include "components.h"
#include "entity_info.h"
#include "particle.h"

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
    AsteroidShape shape;
    EntityInfo info;
    Position position;
    Velocity velocity;
    Rotation rotation;
    ComponentLineInfo lineInfo;
    ParticlePreset particlePreset;
    Score score;
} Asteroid;

Asteroid ASTEROID_Create(const AsteroidPreset *preset);
void ASTEROID_Free(const Asteroid *asteroid);
void ASTEROID_Rotate(const Asteroid *asteroid);
void ASTEROID_MoveTo(Asteroid *asteroid, Vector2 position);
void ASTEROID_UpdateVertices(const Asteroid *asteroid);
void ASTEROID_Render(const Asteroid *asteroid);

bool ASTEROID_IsBulletColliding(const Asteroid *asteroid, const Bullet *bullet);

void* checkCollisionAstPlayer(void* arg);