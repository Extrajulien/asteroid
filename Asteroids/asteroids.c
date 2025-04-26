//
// Created by 2485548 on 2025-04-14.
//

#include "asteroids.h"

#include <stdio.h>
#include <stdlib.h>


//TODO -----
// - transformer les fonctions asteroids en void pointer
// - implementer les AsteroidArray correctement
// -

//asteroid Array
//-------------------------------------------------------------------------------------

void generateWave(AsteroidArray asteroidArr, int waveNum) {
    int nbAsteroid = waveNum * WAVE_ASTEROID_AMNT + 5;

    if (waveNum % 10 == 0 && waveNum != 0) {
        //big wave (+25)
        return;
    }
    createBigAsteroid(asteroidArr, nbAsteroid);
}

void createBigAsteroid(AsteroidArray asteroids, int nbAsteroid) {
    const float radius = 50;
    const float spread = 10;
    const int minRotationSpeed = -300;
    const int maxRotationSpeed = 301;
    *asteroids.asteroid = malloc(nbAsteroid * sizeof(BigAsteroid));//init asteroid arr

    for (int i = 0; i < nbAsteroid; i++) {
        const float rotation = (rand() % (maxRotationSpeed - minRotationSpeed)) + minRotationSpeed;// -300 to 300

        ((BigAsteroid*)asteroids.asteroid[i]) -> radius = radius;       //init radius
        ((BigAsteroid*)asteroids.asteroid[i]) -> spead = spread;        //init spread
        randomPosition((BigAsteroid*)asteroids.asteroid[i]);            //init pos
        randomSpeed((BigAsteroid*)asteroids.asteroid[i]);               //init speed
        ((BigAsteroid*)asteroids.asteroid[i]) -> angle = 0;             //init angle
        ((BigAsteroid*)asteroids.asteroid[i]) -> score = 20;            //init score
        ((BigAsteroid*)asteroids.asteroid[i]) -> rotation = rotation;   //init rotation

        //init points
    }
    printf("malloc");

}

//asteroid
//-------------------------------------------------------------------------------------

void randomSpeed(BigAsteroid *asteroids) {
    Vector2 speed = {0,0};
    speed.x = (rand() % MAX_ASTEROID_SPEED);
    speed.y = (rand() % MAX_ASTEROID_SPEED);
    asteroids -> speed.x = speed.x;
    asteroids -> speed.y = speed.y;
}

void randomPosition(BigAsteroid *asteroids) {
    Vector2 pos = {0,0};
    pos.x = (rand() % GetScreenWidth());
    pos.y = (rand() % GetScreenHeight());
    asteroids -> position.x = pos.x;
    asteroids -> position.y = pos.y;
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
