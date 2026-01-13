#include "asteroid_preset.h"

#include <stdio.h>
#include <stdlib.h>

AsteroidPresetArray *ASTEROID_PRESETS_CreateArray() {
    AsteroidPresetArray *presets = malloc(sizeof(AsteroidPresetArray));

    if (!presets) {
        printf("ERROR allocating asteroid preset array pointer\n");
        exit(1);
    }

    presets->presets = malloc(sizeof(AsteroidPreset));
    if (!presets->presets) {
        printf("ERROR allocating asteroid preset array\n");
        exit(1);
    }

    presets->capacity = 1;
    presets->presetCount = 0;
    return presets;
}

void ASTEROID_PRESETS_Free(AsteroidPresetArray *presets) {
    if (presets) {
        free(presets->presets);
    }
    free(presets);
}

void ASTEROID_PRESETS_Add(AsteroidPresetArray *presetArray, const AsteroidPreset *preset) {
    if (!presetArray) {
        printf("cannot add null pointer to AsteroidPresetArray\n");
        return;
    }

    if (presetArray->capacity < presetArray->presetCount + 1) {
        presetArray->capacity *= 2;
        AsteroidPreset *temp = realloc(presetArray->presets, sizeof(AsteroidPreset) * (presetArray->capacity));
        if (!temp) {
            printf("ASTEROID_BULLET_HIT_QUEUE Realloc Failed");
            exit(1);
        }
        presetArray->presets = temp;
    }
    presetArray->presets[presetArray->presetCount] = *preset;
    ++presetArray->presetCount;
}

void ASTEROID_PRESET_Purge(AsteroidPresetArray *presetArray) {
    presetArray->presetCount = 0;
}

