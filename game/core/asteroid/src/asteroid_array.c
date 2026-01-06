#include "asteroid_array.h"

#include <stdio.h>
#include <stdlib.h>

#include "asteroid_event.h"
#include "game_math.h"

void rotateAsteroids(const AsteroidArray *asteroidArr);

void wrapAroundAsteroids(const AsteroidArray *asteroidArr);

void moveAsteroids(const AsteroidArray *asteroidArr);

void ASTEROIDS_Update(AsteroidArray *asteroidArr) {
    if (asteroidArr->capacity == 0) {
        return;
    }

    rotateAsteroids(asteroidArr);
    moveAsteroids(asteroidArr);
    wrapAroundAsteroids(asteroidArr);

}

AsteroidArray* ASTEROIDS_CreateArray() {
    AsteroidArray *asteroidArr = malloc(sizeof(AsteroidArray));

    if (!asteroidArr) {
        printf("ERROR allocating asteroidArray\n");
        exit(1);
    }

    asteroidArr->asteroid = malloc(sizeof(Asteroid));
    if (!asteroidArr->asteroid) {
        printf("ERROR allocating asteroid array\n");
        exit(1);
    }

    asteroidArr->capacity = 1;
    asteroidArr->nbAsteroid = 0;
    return asteroidArr;
}

void ASTEROIDS_FreeArray(AsteroidArray *asteroidArr) {
    if (asteroidArr == NULL) return;
    for (int i = 0; i < asteroidArr->capacity; i++) {
        ASTEROID_Free(&asteroidArr->asteroid[i]);
    }
    free(asteroidArr->asteroid);
    asteroidArr->asteroid = NULL;
    asteroidArr->capacity = 0;
    asteroidArr->nbAsteroid = 0;
    free(asteroidArr);
}

void ASTEROIDS_Add(AsteroidArray *asteroidArr, const Asteroid asteroid) {
    if (asteroidArr->capacity < asteroidArr->nbAsteroid + 1) {
        asteroidArr->capacity *= 2;
        Asteroid *temp = realloc(asteroidArr->asteroid, sizeof(Asteroid) * (asteroidArr->capacity));
        if (temp == NULL) {
            printf("Asteroid Realloc Failed");
            exit(1);
        }
        asteroidArr->asteroid = temp;
    }
    asteroidArr->asteroid[asteroidArr->nbAsteroid] = asteroid;
    asteroidArr->nbAsteroid++;
}

void ASTEROIDS_Purge(AsteroidArray *asteroidArr) {
    const size_t count = asteroidArr->nbAsteroid;
    for (int i = 0; i < count; i++) {
        asteroidArr->asteroid[i].info.state = STATE_DEAD;
        asteroidArr->nbAsteroid--;
    }
}

void ASTEROIDS_Render(const AsteroidArray *asteroidArr) {
    if (asteroidArr->capacity == 0) {
        return;
    }
    for (int i = 0; i < asteroidArr->nbAsteroid; ++i) {
        const Asteroid *asteroid = &asteroidArr->asteroid[i];
        if (asteroid->info.state == STATE_DEAD) {
            continue;
        }
        ASTEROID_Render(asteroid);
    }
}

void ASTEROIDS_CollideBullets(const AsteroidArray *asteroidArray, const Bullet *bullets, AsteroidBulletHitEventQueue *eventQueue) {
    for (int i = 0; i < PLAYER_MAX_BULLETS; ++i) {
        if (bullets[i].distance == 0) continue;
        const Bullet *bullet = &bullets[i];
        for (int j = 0; j < asteroidArray->nbAsteroid; ++j) {

            if (asteroidArray->asteroid[j].info.state == STATE_DEAD) continue;
            const Asteroid *asteroid = &asteroidArray->asteroid[j];

            if (ASTEROID_IsBulletColliding(asteroid, bullet)) {
                const float hitAngle = AngleFromComponent(&bullet->speed);
                const AsteroidBulletHitEvent event = { j, i, hitAngle, asteroid->score, bullet->position};
                ASTEROID_BULLET_HIT_QUEUE_Add(eventQueue, event);
            }
        }
    }
}

// Moves dead asteroids past `count` using swap-remove.
// Order is not preserved.
void ASTEROIDS_Compact(AsteroidArray *asteroidArr) {
    int i = 0;
    while (i < asteroidArr->nbAsteroid) {
        if (asteroidArr->asteroid[i].info.state == STATE_DEAD) {
            asteroidArr->nbAsteroid--;
            asteroidArr->asteroid[i] = asteroidArr->asteroid[asteroidArr->nbAsteroid];
        } else {
            i++;
        }
    }
}

void rotateAsteroids(const AsteroidArray *asteroidArr) {
    if (asteroidArr->capacity == 0) { return; }

    for (int i = 0; i < asteroidArr->nbAsteroid; ++i) {
        Asteroid *asteroid = &asteroidArr->asteroid[i];
        asteroid->rotation.rad += asteroid->rotation.increment * GetFrameTime();
        WrapAroundRadAngle(&asteroid->rotation);
        ASTEROID_Rotate(&asteroidArr->asteroid[i]);
    }
}

void wrapAroundAsteroids(const AsteroidArray *asteroidArr) {
    if (asteroidArr->capacity == 0) {
        return;
    }

    for (int i = 0; i < asteroidArr->nbAsteroid; ++i) {
        Asteroid *asteroid = &asteroidArr->asteroid[i];
        WrapAroundScreen(&asteroid->position, asteroid->shape.radius);
    }
}

void moveAsteroids(const AsteroidArray *asteroidArr) {
    if (asteroidArr->capacity == 0) {
        return;
    }

    for (int i = 0; i < asteroidArr->nbAsteroid; ++i) {
        Asteroid *asteroid = &asteroidArr->asteroid[i];
        asteroid->position.x += asteroidArr->asteroid[i].velocity.x * GetFrameTime();
        asteroid->position.y += asteroidArr->asteroid[i].velocity.y * GetFrameTime();
        ASTEROID_UpdateVertices(asteroid);
    }

}

