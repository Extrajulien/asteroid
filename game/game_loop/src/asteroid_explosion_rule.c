#include "asteroid_explosion_rule.h"

#include <stdio.h>
#include <stdlib.h>

AsteroidExplosionRuleArray* ASTEROID_EXPLOSION_RULE_CreateArray() {
    AsteroidExplosionRuleArray *rule = malloc(sizeof(AsteroidExplosionRuleArray));

    if (!rule) {
        printf("ERROR allocating asteroid explosion rule array pointer\n");
        exit(1);
    }

    rule->explosionRules = malloc(sizeof(AsteroidExplosionRule));
    if (!rule->explosionRules) {
        printf("ERROR allocating asteroid explosion rule array\n");
        exit(1);
    }

    rule->capacity = 1;
    rule->count = 0;
    return rule;
}

void ASTEROID_EXPLOSION_RULE_Free(AsteroidExplosionRuleArray *rules) {
    if (rules) {
        free(rules->explosionRules);
    }
    free(rules);
}

void ASTEROID_EXPLOSION_RULE_Add(AsteroidExplosionRuleArray *ruleArray, const AsteroidExplosionRule *rule) {
    if (!rule) {
        printf("cannot add null pointer to AsteroidExplosionRuleArray\n");
        return;
    }

    if (ruleArray->capacity < ruleArray->count + 1) {
        ruleArray->capacity *= 2;
        AsteroidExplosionRule *temp = realloc(ruleArray->explosionRules, sizeof(AsteroidExplosionRule) * (ruleArray->capacity));
        if (!temp) {
            printf("ASTEROID_WAVE_SPAWN_RULE_Add Realloc Failed");
            exit(1);
        }
        ruleArray->explosionRules = temp;
    }
    ruleArray->explosionRules[ruleArray->count] = *rule;
    ++ruleArray->count;
}