//
// Created by 2485548 on 2025-04-14.
//

#include "asteroids.h"

#include <stdio.h>
#include <stdlib.h>

void generateWave(BigAsteroidArray **asteroids, int waveNum) {
    int nbAsteroid = waveNum * WAVE_ASTEROID_AMNT + 5;

    if (waveNum % 10 == 0 && waveNum != 0) {
        //big wave (+25)
        return;
    }

    for (int i = 0; i < nbAsteroid; ++i) {
        createBigAsteroid(asteroids);
    }

}

void randomPosition(BigAsteroid *asteroids) {
    Vector2 pos = {0,0};
    pos.x = (rand() % GetScreenWidth());
    pos.y = (rand() % GetScreenHeight());
    asteroids -> position.x = pos.x;
    asteroids -> position.y = pos.y;
}

void createBigAsteroid(BigAsteroidArray **asteroids) {
    const float radius = 50;
    const float spread = 10;
    *asteroids = realloc(*asteroids, WAVE_ASTEROID_AMNT * sizeof(BigAsteroidArray));
}

void createMidAsteroid(MidAsteroidArray **asteroids) {
    const float radius = 30;
}

void createSmlAsteroid(SmlAsteroidArray **asteroids) {
    const float radius = 15;
}

void bigAsteroidShot(BigAsteroidArray **asteroid, MidAsteroidArray **newAsteroid, short index) {
    const int splitNumber = 2;
    for (int i = 0; i < splitNumber; i++) {
        createMidAsteroid(newAsteroid);
    }
}

void midAsteroidShot(MidAsteroidArray **asteroid, SmlAsteroidArray **newAsteroid, short index) {
    const int splitNumber = 2;
    for (int i = 0; i < splitNumber; i++) {
        createSmlAsteroid(newAsteroid);
    }
}

void smlAsteroidShot(SmlAsteroidArray **asteroid, short index) {

}
