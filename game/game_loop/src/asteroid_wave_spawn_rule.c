#include "asteroid_wave_spawn_rule.h"

#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

AsteroidWaveSpawnRuleArray* ASTEROID_WAVE_SPAWN_RULE_CreateArray() {
    AsteroidWaveSpawnRuleArray *rule = malloc(sizeof(AsteroidWaveSpawnRuleArray));
    ASSERT_ALLOCATION(rule);

    rule->spawnRules = malloc(sizeof(AsteroidWaveSpawnRule));
    ASSERT_ALLOCATION(rule->spawnRules);

    rule->capacity = 1;
    rule->count = 0;
    return rule;
}

void ASTEROID_WAVE_SPAWN_RULE_Free(AsteroidWaveSpawnRuleArray *rules) {
    if (rules) {
        free(rules->spawnRules);
    }
    free(rules);
}

void ASTEROID_WAVE_SPAWN_RULE_Add(AsteroidWaveSpawnRuleArray *ruleArray, const AsteroidWaveSpawnRule *rule) {
    if (!rule) {
        printf("cannot add null pointer to AsteroidWaveSpawnRuleArray\n");
        return;
    }

    if (ruleArray->capacity < ruleArray->count + 1) {
        ruleArray->capacity *= 2;
        AsteroidWaveSpawnRule *temp = realloc(ruleArray->spawnRules, sizeof(AsteroidWaveSpawnRule) * (ruleArray->capacity));
        ASSERT_ALLOCATION(temp);
        ruleArray->spawnRules = temp;
    }
    ruleArray->spawnRules[ruleArray->count] = *rule;
    ++ruleArray->count;
}