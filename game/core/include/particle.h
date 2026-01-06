#ifndef PARTICLES_H
#define PARTICLES_H
#include "raylib.h"
#define DEFAULT_PARTICLE_COLOR (Color) {230, 230, 230, 255}
typedef struct Particle {
    Rectangle bounds;
    float speed;
    float lifetime;
    float currentLifetime;
    float angle;
    bool isShown;
    Color color;
    void (*move)(struct Particle *self);
    void (*draw)(const struct Particle *self);
} Particle;

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
