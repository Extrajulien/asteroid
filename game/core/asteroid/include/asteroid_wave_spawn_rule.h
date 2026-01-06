#pragma once
#include <stddef.h>

#include "asteroid_size.h"
typedef struct AsteroidWaveSpawnRule {
    AsteroidSize spawnSize;
    unsigned int spawnCount;
} AsteroidWaveSpawnRule;

typedef struct AsteroidWaveSpawnRuleArray {
    AsteroidWaveSpawnRule* spawnRules;
    size_t count;
    size_t capacity;
} AsteroidWaveSpawnRuleArray;

AsteroidWaveSpawnRuleArray* ASTEROID_WAVE_SPAWN_RULE_CreateArray();
void ASTEROID_WAVE_SPAWN_RULE_Free(AsteroidWaveSpawnRuleArray *rules);
void ASTEROID_WAVE_SPAWN_RULE_Add(AsteroidWaveSpawnRuleArray *ruleArray, const AsteroidWaveSpawnRule *rule);