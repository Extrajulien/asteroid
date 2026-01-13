#include "asteroid_wave_spawn_rule.h"

#include <stdio.h>
#include <stdlib.h>

AsteroidWaveSpawnRuleArray* ASTEROID_WAVE_SPAWN_RULE_CreateArray() {
    AsteroidWaveSpawnRuleArray *rule = malloc(sizeof(AsteroidWaveSpawnRuleArray));

    if (!rule) {
        printf("ERROR allocating asteroid Wave spawn rule array pointer\n");
        exit(1);
    }

    rule->spawnRules = malloc(sizeof(AsteroidWaveSpawnRule));
    if (!rule->spawnRules) {
        printf("ERROR allocating asteroid Wave spawn rule array\n");
        exit(1);
    }

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
        if (!temp) {
            printf("ASTEROID_WAVE_SPAWN_RULE_Add Realloc Failed");
            exit(1);
        }
        ruleArray->spawnRules = temp;
    }
    ruleArray->spawnRules[ruleArray->count] = *rule;
    ++ruleArray->count;
}