#ifndef PARTICLES_H
#define PARTICLES_H
#include "raylib.h"
#define DEFAULT_PARTICLE_COLOR (Color) {230, 230, 230, 255}
typedef struct Particle{
    Rectangle bounds;
    float speed;
    float lifetime;
    float currentLifetime;
    float angle;
    bool isShown;
    Color color;
    void (*move)(struct Particle *self);
    void (*draw)(struct Particle *self);
}Particle;

typedef struct ParticleArray{
    Particle *particles;
    int size;
    int maxSize;
}ParticleArray;

Particle newParticle(Vector2 position, Vector2 size, float speed, float lifetime, float angle, Color color);
void particleMove(Particle *particle);
void particleDraw(Particle *particle);
void createParticles(float angle, Vector2 position, int nbParticles, float spread, Color color, float lifetime, float speed);
bool checkIfParticleIsNotValid(Particle const *particle);

void particleArrDestroy();
void particleArrInit(int size);
void particleArrExpand(int increment);
void particleArrRemoveAt(int index);
void particleArrAdd(Particle particle);
void drawParticles();
void moveParticles();
void clearDeadParticles();

#endif //PARTICLES_H
