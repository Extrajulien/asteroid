#pragma once
#include "asteroid_size.h"
#include "asteroid_preset.h"
#include "player.h"
#include "components.h"
#include "entity_info.h"
#include "particle.h"

#define ASTEROID_SIZE_COUNT 3

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
void ASTEROID_UpdateVertices(const Asteroid *asteroid);
void ASTEROID_Render(const Asteroid *asteroid);

bool ASTEROID_IsBulletColliding(const Asteroid *asteroid, const Bullet *bullet);

void* checkCollisionAstPlayer(void* arg);