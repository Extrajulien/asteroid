#include "particle.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static ParticleArray particleArr = {
    .particles = NULL,
    .size = 0,
    .reservedSize = 0
};
// Array functions
// -------------------------------------------------------------------------------------------------------

void particleArrDestroy() {
    free(particleArr.particles);
    particleArr.size = 0;
    particleArr.reservedSize = 0;
}

void particleArrInit(const int size) {
    particleArr.reservedSize = size;
    particleArr.particles = malloc(size * sizeof(Particle));
    if (particleArr.particles == NULL) {
        printf("Particle Allocation Failed");
        exit(1);
    }
}

void particleArrExpand(const int increment) {
    const int size = particleArr.size;
    if (size + increment > particleArr.reservedSize) {
        particleArr.reservedSize *= 2;
        Particle *temp = realloc(particleArr.particles, sizeof(Particle) * (particleArr.reservedSize));
        if (temp == NULL) {
            printf("Particle Realloc Failed");
            exit(1);
        }
        particleArr.particles = temp;
    }
}

//pushes the particle at the end of the array
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
        if(particleIsNotValid(&particleArr.particles[i])) {
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

void createParticles(const float angle, const Vector2 position, const ParticlePreset *preset) {
    for (int i = 0; i < preset->quantity; ++i) {
        float const newAngle = angle + ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * (preset->angleSpread * DEG2RAD);
        particleArrAdd(newParticle(position, (Vector2) {10, 5}, preset->speed, preset->lifetime, newAngle, preset->color));
    }
}

Particle newParticle(Vector2 const position, Vector2 const size,
    float const speed, float const lifetime, float const angle, Color const color) {
    return (Particle){
        .bounds = (Rectangle) {position.x, position.y, size.x, size.y},
        .speed = speed,
        .lifetime = lifetime,
        .angle = angle,
        .draw = particleDraw,
        .move = particleMove,
        .color = color
    };
}

void particleMove(Particle *particle) {
    float const speed = particle->speed - particle->speed * (particle->currentLifetime / particle->lifetime);
    float const angleRad = particle->angle;
    if (speed > 0) {
        particle->bounds.x += cosf(angleRad) * GetFrameTime() * speed;
        particle->bounds.y -= sinf(angleRad) * GetFrameTime() * speed;
        particle->color.a = 255 - 255 * (particle->currentLifetime / particle->lifetime);
    }
    particle->currentLifetime += GetFrameTime();
}

bool particleIsNotValid(Particle const *particle) {
    return particle->currentLifetime > particle->lifetime;
}

void particleDraw(const Particle *particle) {
    const Vector2 position = { particle->bounds.width/2, particle->bounds.height/2 };
    DrawRectanglePro(particle->bounds, position, particle->angle * (RAD2DEG), particle->color);
}
