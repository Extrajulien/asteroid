//
// Created by 2485548 on 2025-04-14.
//

#include "asteroids.h"

void generateWave(BigAsteroid **asteroids, int waveNum) {
    int nbAsteroid = waveNum * 5 + 5;

    if (waveNum % 10 == 0 && waveNum != 0) {
        //big wave (+25)
        return;
    }
    for (int i = 0; i < nbAsteroid; ++i) {
        createBigAsteroid(asteroids);
    }

}

void randomPosition() {
    GetScreenWidth();
    GetScreenHeight();
}

void createBigAsteroid(BigAsteroidArray **asteroids) {

}

void createMidAsteroid(MidAsteroidArray **asteroids) {

}

void createSmlAsteroid(SmlAsteroidArray **asteroids) {

}

void bigAsteroidShot(BigAsteroidArray **asteroid, MidAsteroidArray **newAsteroid, short index) {


}

void midAsteroidShot(MidAsteroidArray **asteroid, SmlAsteroidArray **newAsteroid, short index) {

}

void smlAsteroidShot(SmlAsteroidArray **asteroid, short index) {

}
