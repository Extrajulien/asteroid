#pragma once
#include "asteroid_preset.h"
#include "asteroid_explosion_rule.h"
#include "asteroid_wave_spawn_rule.h"

typedef struct AsteroidBulletHitEventQueue AsteroidBulletHitEventQueue;
typedef struct AsteroidArray AsteroidArray;

typedef struct {
    AsteroidPresetArray *presetArr;
    AsteroidWaveSpawnRuleArray *spawnRuleArr;
    AsteroidExplosionRuleArray *explosionRuleArr;
} WaveContext;


void WAVE_SpawnAsteroids(AsteroidArray *asteroidArr, const WaveContext *wave, SpawnExclusionCircle exclusionCircle);
void WAVE_ExplodeAsteroids(AsteroidArray *asteroidArray, const WaveContext *wave, const AsteroidBulletHitEventQueue *bulletHitEvent);
WaveContext* WAVE_CONTEXT_Create();
void WAVE_CONTEXT_Free(WaveContext *wave);