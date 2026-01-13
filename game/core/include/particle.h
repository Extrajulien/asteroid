#ifndef PARTICLES_H
#define PARTICLES_H
#include <stdbool.h>
#include "raylib.h"

typedef struct Particle Particle;

typedef struct ParticleArray{
    Particle *particles;
    int size;
    int reservedSize;
} ParticleArray;

typedef struct ParticlePreset {
    unsigned int quantity;
    float angleSpread;
    float lifetime;
    Color color;
    float speed;
} ParticlePreset;

Particle newParticle(Vector2 position, Vector2 size, float speed, float lifetime, float angle, Color color);
void particleMove(Particle *particle);
void particleDraw(const Particle *particle);
void createParticles(float angle, Vector2 position, const ParticlePreset* preset);
bool particleIsNotValid(Particle const *particle);

void particleArrDestroy();
void particleArrInit(int size);
void particleArrExpand(int increment);
void particleArrRemoveAt(int index);
void particleArrAdd(Particle particle);
void drawParticles();
void moveParticles();
void clearDeadParticles();

#endif //PARTICLES_H
