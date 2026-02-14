#include "wave.h"
#include <stdio.h>
#include "asteroid.h"
#include "asteroid_array.h"
#include <stdlib.h>
#include "game_math.h"
#include "asteroid_preset.h"
#include "event_api.h"
#include "logger.h"

void PlaceAsteroidRandomPosition(Asteroid *asteroid, SpawnExclusionCircle exclusionCircle);
void setAsteroidRandomSpeed(Asteroid *asteroid, const AsteroidPreset *preset);
void explodeAsteroid(const AsteroidArray *asteroidArray, const AsteroidBulletHitEvent *event);
void spawnAsteroidFromRule(AsteroidArray *asteroidArray, const AsteroidBulletHitEvent *event, const WaveContext *wave);


void WAVE_SpawnAsteroids(AsteroidArray *asteroidArr, const WaveContext *wave, const SpawnExclusionCircle exclusionCircle) {
    for (int i = 0; i < wave->spawnRuleArr->count; ++i) {
        const AsteroidWaveSpawnRule *rule = &wave->spawnRuleArr->spawnRules[i];
        if (rule->spawnSize >= wave->presetArr->presetCount) {
            LOGF("$Y[WARN]$y WAVE_SpawnAsteroids() invalid spawnSize %u\n", rule->spawnSize);
            continue;
        }

        const AsteroidPreset *preset = &wave->presetArr->presets[rule->spawnSize];
        for (int j = 0; j < rule->spawnCount; ++j) {
            Asteroid asteroid = ASTEROID_Create(preset, asteroidArr->verticePool);
            PlaceAsteroidRandomPosition(&asteroid, exclusionCircle);
            ASTEROID_MoveTo(&asteroid, asteroid.position, asteroidArr->verticePool);
            setAsteroidRandomSpeed(&asteroid, preset);
            ASTEROIDS_Add(asteroidArr, asteroid);
        }
    }
}

void WAVE_ExplodeAsteroids(AsteroidArray *asteroidArray, const WaveContext *wave, const AsteroidBulletHitEventQueue *bulletHitEvent) {
    for (int i = 0; i < bulletHitEvent->count; ++i) {
        const AsteroidBulletHitEvent event = bulletHitEvent->events[i];

        spawnAsteroidFromRule(asteroidArray, &event, wave);
        explodeAsteroid(asteroidArray, &event);
    }
    if (bulletHitEvent->count > 0) {
        ASTEROIDS_Compact(asteroidArray);
    }
}


WaveContext* WAVE_CONTEXT_Create() {
    WaveContext *wave = malloc(sizeof(WaveContext));
    ASSERT_ALLOCATION(wave);
    wave->presetArr = ASTEROID_PRESETS_CreateArray();
    wave->spawnRuleArr = ASTEROID_WAVE_SPAWN_RULE_CreateArray();
    wave->explosionRuleArr = ASTEROID_EXPLOSION_RULE_CreateArray();

    const AsteroidWaveSpawnRule bigSpawnRule = {SIZE_BIG, 5};
    ASTEROID_WAVE_SPAWN_RULE_Add(wave->spawnRuleArr, &bigSpawnRule);

    const AsteroidExplosionRule bigExplosion = {SIZE_MEDIUM, 2};
    ASTEROID_EXPLOSION_RULE_Add(wave->explosionRuleArr, &bigExplosion);


    const AsteroidWaveSpawnRule midSpawnRule = {SIZE_MEDIUM, 2};
    ASTEROID_WAVE_SPAWN_RULE_Add(wave->spawnRuleArr, &midSpawnRule);

    const AsteroidExplosionRule midExplosion = {SIZE_SMALL, 2};
    ASTEROID_EXPLOSION_RULE_Add(wave->explosionRuleArr, &midExplosion);

    const AsteroidWaveSpawnRule smlSpawnRule = {SIZE_SMALL, 1};
    ASTEROID_WAVE_SPAWN_RULE_Add(wave->spawnRuleArr, &smlSpawnRule);

    const AsteroidExplosionRule smlExplosion = {SIZE_SMALL, 0};
    ASTEROID_EXPLOSION_RULE_Add(wave->explosionRuleArr, &smlExplosion);


    return wave;
}

void WAVE_CONTEXT_Free(WaveContext *wave) {
    ASTEROID_PRESETS_Free(wave->presetArr);
    ASTEROID_WAVE_SPAWN_RULE_Free(wave->spawnRuleArr);
    ASTEROID_EXPLOSION_RULE_Free(wave->explosionRuleArr);
    free(wave);
}



void PlaceAsteroidRandomPosition(Asteroid *asteroid, const SpawnExclusionCircle exclusionCircle) {
    const float circleDiameter = exclusionCircle.radius * 2;
    const int xRange = GetScreenWidth() - (int) circleDiameter;
    const int yRange = GetScreenHeight() - (int) circleDiameter;
    Vector2 pos;
    pos.x = rand() % xRange;
    pos.y = rand() % yRange;
    if (pos.x > exclusionCircle.center.x - exclusionCircle.radius) pos.x += circleDiameter;
    if (pos.y > exclusionCircle.center.y - exclusionCircle.radius) pos.y += circleDiameter;
    asteroid->position = pos;
}

void setAsteroidRandomSpeed(Asteroid *asteroid, const AsteroidPreset *preset) {
    Vector2 speed;
    speed.x = (rand() % (int) (preset->maxSpeed + 1) - preset->maxSpeed);
    speed.y = (rand() % (int) (preset->maxSpeed + 1) - preset->maxSpeed);
    asteroid->velocity = speed;
}

void explodeAsteroid(const AsteroidArray *asteroidArray, const AsteroidBulletHitEvent *event) {
    Asteroid *asteroid = &asteroidArray->asteroid[event->asteroidId];
    const float particleDirection = flipRadAngle(event->hitAngle);
    createParticles(particleDirection, event->hitPosition, &asteroid->particlePreset);
    ASTEROID_Remove(asteroid, asteroidArray->verticePool);
}

void spawnAsteroidFromRule(AsteroidArray *asteroidArray, const AsteroidBulletHitEvent *event, const WaveContext *wave) {
    const AsteroidSize hitAsteroidSize = asteroidArray->asteroid[event->asteroidId].type;
    if (wave->explosionRuleArr->count <= hitAsteroidSize) {
        printf("Type '%d' is not present in Explosion Rules (count: %lu)\n", hitAsteroidSize, wave->explosionRuleArr->count);
        return;
    }

    const AsteroidExplosionRule *explosionRules = &wave->explosionRuleArr->explosionRules[hitAsteroidSize];
    const AsteroidPreset *preset = &wave->presetArr->presets[explosionRules->spawnedSize];

    for (int i = 0; i < explosionRules->spawnCount; ++i) {
        Asteroid asteroid = ASTEROID_Create(preset, asteroidArray->verticePool);
        setAsteroidRandomSpeed(&asteroid, preset);
        asteroid.position = asteroidArray->asteroid[event->asteroidId].position;
        ASTEROID_MoveTo(&asteroid, asteroid.position, asteroidArray->verticePool);
        ASTEROIDS_Add(asteroidArray, asteroid);
    }
}