#include "particles.h"
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

// particle functions
// -------------------------------------------------------------------------------------------------------

void particleSpawn(Particle *particle) {
}

void particleMove(Particle *particle) {
}

void particleDraw(Particle *particle) {
}
