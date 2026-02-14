#include "asteroid_array.h"
#include "event_sink_api.h"

#include <stdio.h>
#include <stdlib.h>


#include "asteroid.h"
#include "bullet.h"
#include "bullet_array.h"
#include "game_math.h"
#include "logger.h"
#include "player.h"

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
    ASSERT_ALLOCATION(asteroidArr);

    asteroidArr->asteroid = malloc(sizeof(Asteroid));
    ASSERT_ALLOCATION(asteroidArr->asteroid);

    asteroidArr->capacity = 1;
    asteroidArr->nbAsteroid = 0;
    asteroidArr->maxReachedCount = 0;
    asteroidArr->verticePool = VERTICE_POOL_Create();
    return asteroidArr;
}

void ASTEROIDS_FreeArray(AsteroidArray *asteroidArr) {
    if (asteroidArr == NULL) return;
    LOGF_MEMORY("ASTEROIDS_FreeArray(), freeing $O%d$o asteroids\n", asteroidArr->maxReachedCount);
    for (int i = 0; i < asteroidArr->maxReachedCount; i++) {
        ASTEROID_Free(&asteroidArr->asteroid[i], asteroidArr->verticePool);
    }
    free(asteroidArr->asteroid);
    asteroidArr->asteroid = NULL;
    asteroidArr->capacity = 0;
    asteroidArr->nbAsteroid = 0;
    asteroidArr->maxReachedCount = 0;
    VERTICE_POOL_Free(asteroidArr->verticePool);
    free(asteroidArr);
}

void ASTEROIDS_Add(AsteroidArray *asteroidArr, const Asteroid asteroid) {
    if (asteroidArr->capacity < asteroidArr->nbAsteroid + 1) {
        asteroidArr->capacity *= 2;
        Asteroid *temp = realloc(asteroidArr->asteroid, sizeof(Asteroid) * asteroidArr->capacity);
        ASSERT_ALLOCATION(temp);
        asteroidArr->asteroid = temp;
    }

    asteroidArr->asteroid[asteroidArr->nbAsteroid] = asteroid;
    asteroidArr->nbAsteroid++;

    if (asteroidArr->maxReachedCount < asteroidArr->nbAsteroid) {
        LOGF("$G[Max Count update, cnt {%d}] MaxCount then: %d",asteroidArr->nbAsteroid , asteroidArr->maxReachedCount);
        asteroidArr->maxReachedCount = asteroidArr->nbAsteroid;
        LOGF("$g, MaxCount now: %d\n", asteroidArr->maxReachedCount);
    }
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
        ASTEROID_Render(asteroid, asteroidArr->verticePool);
    }
}

bool ASTEROIDS_AreAllSizesPresent(const AsteroidArray *asteroidArr) {
    int uniqueSize = 0;
    for (int size = 0; size < SIZE_COUNT; size++) {
        for (size_t i = 0; i < asteroidArr->nbAsteroid; i++) {
            if (asteroidArr->asteroid[i].type == size) {
                uniqueSize++;
                break;
            }
        }
    }


    return uniqueSize == SIZE_COUNT;
}

void ASTEROIDS_Remove(AsteroidArray *asteroidArray, const size_t index) {
    ASTEROID_Remove(&asteroidArray->asteroid[index], asteroidArray->verticePool);
    ASTEROIDS_Compact(asteroidArray);
}

void ASTEROIDS_CollideBullets(const AsteroidArray *asteroidArray, const BulletArray *bullets, const AsteroidBulletHitEventSink *sink) {
    for (int i = 0; i < bullets->count; ++i) {
        const Bullet *bullet = BULLETS_GetBullet(bullets, i);
        for (int j = 0; j < asteroidArray->nbAsteroid; ++j) {

            if (asteroidArray->asteroid[j].info.state == STATE_DEAD) continue;
            const Asteroid *asteroid = &asteroidArray->asteroid[j];
            if (ASTEROID_IsBulletColliding(asteroid, bullet, asteroidArray->verticePool)) {
                Vector2 bulletSpeed = BULLET_GetSpeed(bullet);
                const float hitAngle = AngleFromComponent(&bulletSpeed);
                const AsteroidBulletHitEvent event = { j, i, hitAngle, asteroid->score, BULLET_GetPosition(bullet)};
                sink->emit(sink->ctx, event);
            }
        }
    }
}

// Moves dead asteroids past `count` using swap-remove.
// Order is not preserved.
void ASTEROIDS_Compact(AsteroidArray *asteroidArray) {
    const int originalAsteroidCount = asteroidArray->nbAsteroid;
    int deadAsteroids = 0;
    for (int i = 0; i < asteroidArray->nbAsteroid; ++i) {
        if (asteroidArray->asteroid[i].info.state == STATE_DEAD) {
            ++deadAsteroids;
        }
    }

    if (deadAsteroids == 0) {
        LOG("$R[WARN]$r ASTEROIDS_Compact() has been called even if no asteroid where DEAD.\n");
    }


    int i = 0;
    while (i < asteroidArray->nbAsteroid) {
        if (asteroidArray->asteroid[i].info.state == STATE_DEAD) {
            asteroidArray->nbAsteroid--;
            asteroidArray->asteroid[i] = asteroidArray->asteroid[asteroidArray->nbAsteroid];
        } else {
            i++;
        }
    }
    LOGF_MEMORY("ASTEROIDS_Compact() [size] %d $O->$o %d, DEAD %d\n", originalAsteroidCount, asteroidArray->nbAsteroid, deadAsteroids);
}

void rotateAsteroids(const AsteroidArray *asteroidArr) {
    if (asteroidArr->capacity == 0) { return; }

    for (int i = 0; i < asteroidArr->nbAsteroid; ++i) {
        Asteroid *asteroid = &asteroidArr->asteroid[i];
        asteroid->rotation.rad += asteroid->rotation.increment * GetFrameTime();
        WrapAroundRadAngle(&asteroid->rotation);
        ASTEROID_Rotate(&asteroidArr->asteroid[i], asteroidArr->verticePool);
    }
}

void wrapAroundAsteroids(const AsteroidArray *asteroidArr) {
    if (asteroidArr->capacity == 0) {
        return;
    }

    for (int i = 0; i < asteroidArr->nbAsteroid; ++i) {
        Asteroid *asteroid = &asteroidArr->asteroid[i];
        WrapAroundScreen(&asteroid->position, asteroid->radius);
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
        ASTEROID_UpdateVertices(asteroid, asteroidArr->verticePool);
    }

}

