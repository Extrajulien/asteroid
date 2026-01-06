#pragma once
#include "asteroid.h"
#include "asteroid_array.h"
#include "asteroid_event.h"
#include "asteroid_explosion_rule.h"
#include "asteroid_wave_spawn_rule.h"

typedef struct {
    AsteroidPresetArray *presetArr;
    AsteroidWaveSpawnRuleArray *spawnRuleArr;
    AsteroidExplosionRuleArray *explosionRuleArr;
} WaveContext;


void WAVE_SpawnAsteroids(AsteroidArray *asteroidArr, const WaveContext *wave, SpawnExclusionCircle exclusionCircle);
void WAVE_ExplodeAsteroids(AsteroidArray *asteroidArray, const WaveContext *wave, const AsteroidBulletHitEventQueue *bulletHitEvent);
WaveContext WAVE_CONTEXT_Create();