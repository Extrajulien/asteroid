#ifndef PARTICLES_H
#define PARTICLES_H
#include "raylib.h"
typedef struct Particle{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    float lifetime;
    bool isShown;
    void (*spawn)(struct Particle *self);
    void (*move)(struct Particle *self);
    void (*draw)(struct Particle *self);

}Particle;

typedef struct ParticleArray{
    Particle *particles;
    int size;
    int maxSize;
}ParticleArray;

void particleSpawn(Particle *particle);
void particleMove(Particle *particle);
void particleDraw(Particle *particle);

void particleArrDestroy();
void particleArrInit(int size);
void particleArrExpand(int increment);
void particleArrRemoveAt(int index);
void particleArrAdd(Particle particle);

#endif //PARTICLES_H
