#pragma once
#include "asteroid_preset.h"
#include "asteroid_explosion_rule.h"
#include "asteroid_wave_spawn_rule.h"
#include "player_asteroid_hit_event_queue.h"

typedef struct AsteroidBulletHitEventQueue AsteroidBulletHitEventQueue;
typedef struct AsteroidArray AsteroidArray;

typedef struct {
    AsteroidPresetArray *presetArr;
    AsteroidWaveSpawnRuleArray *spawnRuleArr;
    AsteroidExplosionRuleArray *explosionRuleArr;
} WaveContext;


void WAVE_SpawnAsteroids(AsteroidArray *asteroidArr, const WaveContext *wave, SpawnExclusionCircle exclusionCircle);
void WAVE_ExplodeAsteroidsFromBullet(AsteroidArray *asteroidArray, const WaveContext *wave, const AsteroidBulletHitEventQueue *bulletHitEvent);
void WAVE_ExplodeAsteroidsFromPlayer(AsteroidArray *asteroidArray, const WaveContext *wave, const PlayerAsteroidHitEventQueue *eQueue);
WaveContext* WAVE_CONTEXT_Create();
void WAVE_CONTEXT_Free(WaveContext *wave);