#pragma once
#include "asteroid.h"

typedef struct {
    AsteroidSize spawnedSize;
    unsigned int spawnCount;
} AsteroidExplosionRule;

typedef struct AsteroidExplosionRuleArray {
    AsteroidExplosionRule* explosionRules;
    size_t count;
    size_t capacity;
} AsteroidExplosionRuleArray;

AsteroidExplosionRuleArray* ASTEROID_EXPLOSION_RULE_CreateArray();
void ASTEROID_EXPLOSION_RULE_Free(AsteroidExplosionRuleArray *rules);
void ASTEROID_EXPLOSION_RULE_Add(AsteroidExplosionRuleArray *ruleArray, const AsteroidExplosionRule *rule);
