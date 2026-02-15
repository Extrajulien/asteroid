#include "asteroid_preset.h"

#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

AsteroidPresetArray *ASTEROID_PRESETS_CreateArray() {
    AsteroidPresetArray *presets = malloc(sizeof(AsteroidPresetArray));
    ASSERT_ALLOCATION(presets);

    presets->presets = malloc(sizeof(AsteroidPreset));
    ASSERT_ALLOCATION(presets->presets);

    presets->capacity = 1;
    presets->presetCount = 0;
    return presets;
}

void ASTEROID_PRESETS_Free(AsteroidPresetArray *presets) {
    if (presets) {
        for (int i = 0; i < presets->capacity; i++) {}


        free(presets->presets);
    }
    free(presets);
}

void ASTEROID_PRESETS_OrderArray(const AsteroidPresetArray *presets) {
    for (size_t i = 0; i < SIZE_COUNT; i++) {
        for (size_t j = i; j < presets->presetCount; j++) {
            if (presets->presets[j].size == i) {
                const AsteroidPreset temp = presets->presets[i];
                presets->presets[i] = presets->presets[j];
                presets->presets[j] = temp;
            }
        }
    }
}

void ASTEROID_PRESETS_Add(AsteroidPresetArray *presetArray, const AsteroidPreset *preset) {
    if (!presetArray) {
        printf("cannot add null pointer to AsteroidPresetArray\n");
        return;
    }

    if (presetArray->capacity < presetArray->presetCount + 1) {
        presetArray->capacity *= 2;
        AsteroidPreset *temp = realloc(presetArray->presets, sizeof(AsteroidPreset) * (presetArray->capacity));
        ASSERT_ALLOCATION(temp);
        presetArray->presets = temp;
    }
    presetArray->presets[presetArray->presetCount] = *preset;
    ++presetArray->presetCount;
}

void ASTEROID_PRESET_Purge(AsteroidPresetArray *presetArray) {
    presetArray->presetCount = 0;
}

