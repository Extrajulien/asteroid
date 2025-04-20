//
// Created by 2485548 on 2025-04-14.
//

#include "asteroids.h"

void generateWave(BigAsteroid *asteroids, int waveNum) {
    int nbAsteroid = waveNum * 5 + 5;

    if (waveNum % 10 == 0 && waveNum != 0) {
        //big wave (+25)
        return;
    }
    for (int i = 0; i < nbAsteroid; ++i) {
        createAsteroid(*asteroids);
    }

}

void randomPosition() {
    GetScreenWidth();
    GetScreenHeight();
}

void createAsteroid(BigAsteroid asteroids) {

}
