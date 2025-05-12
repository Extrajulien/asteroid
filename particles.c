#include "particles.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static ParticleArray particleArr = {
    .particles = NULL,
    .size = 0,
    .maxSize = 0
};
// Array functions
// -------------------------------------------------------------------------------------------------------

void particleArrDestroy() {
    free(particleArr.particles);
    particleArr.size = 0;
    particleArr.maxSize = 0;
}

void particleArrInit(const int size) {
    particleArr.maxSize = size;
    particleArr.particles = malloc(size * sizeof(Particle));
    if (particleArr.particles == NULL) {
        printf("Particle Allocation Failed");
        exit(1);
    }
}

void particleArrExpand(const int increment) {
    const int size = particleArr.size;
    if (size + increment > particleArr.maxSize) {
        particleArr.maxSize *= 2;
        Particle *temp = realloc(particleArr.particles, sizeof(Particle) * (particleArr.maxSize));
        if (temp == NULL) {
            printf("Particle Realloc Failed");
            exit(1);
        }
        particleArr.particles = temp;
    }
}

void particleArrRemoveAt(int const index) {
    for (int i = index; i < particleArr.size - 1; ++i) {
        particleArr.particles[i] = particleArr.particles[i + 1];
    }
    particleArr.size--;
}

void particleArrAdd(Particle const particle) {
    particleArrExpand(1);
    particleArr.particles[particleArr.size] = particle;
    particleArr.size++;
}

void drawParticles() {
    for (int i = 0; i < particleArr.size; ++i) {
        particleArr.particles[i].draw(&particleArr.particles[i]);
    }
}

void clearDeadParticles() {
    for (int i = 0; i < particleArr.size; ++i) {
        if(checkIfParticleIsNotValid(&particleArr.particles[i])) {
            particleArrRemoveAt(i);
        }
    }
}

void moveParticles() {
    for (int i = 0; i < particleArr.size; ++i) {
        particleArr.particles[i].move(&particleArr.particles[i]);
    }
    clearDeadParticles();
}

// particle functions
// -------------------------------------------------------------------------------------------------------

void createParticles(float angle, Vector2 position, int nbParticles, float spread,
    Color color, float lifetime, float speed) {
    for (int i = 0; i < nbParticles; ++i) {
        float const newAngle = angle + (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * (spread * DEG2RAD);
        particleArrAdd(newParticle(position, (Vector2){10, 5}
        , speed, lifetime, newAngle, color));
    }
}

Particle newParticle(Vector2 const position, Vector2 const size,
    float const speed, float const lifetime, float const angle, Color const color) {
    return (Particle){
        .bounds = (Rectangle){position.x, position.y, size.x, size.y},
        .speed = speed, .lifetime = lifetime,
        .angle = angle,
        .draw = particleDraw,
        .move = particleMove,
        .color = color
    };
}

void particleMove(Particle *particle) {
    float const speed = particle->speed - particle->speed*(particle->currentLifetime/particle->lifetime);
    float *posX = &particle->bounds.x;
    float *posY = &particle->bounds.y;
    float const angleRad = particle->angle;
    if (speed > 0) {
        *posX += cosf(angleRad) * GetFrameTime() * speed;
        *posY -= sinf(angleRad) * GetFrameTime() * speed;
        particle->color.a = 255 - 255*(particle->currentLifetime/particle->lifetime);
    }
    particle->currentLifetime += GetFrameTime();
}

bool checkIfParticleIsNotValid(Particle const *particle) {
    return particle->currentLifetime > particle->lifetime;
}

void particleDraw(Particle *particle) {
    Vector2 position = { particle->bounds.width/2, particle->bounds.height/2 };
    DrawRectanglePro(particle->bounds, position, particle->angle*(RAD2DEG), particle->color);
}
