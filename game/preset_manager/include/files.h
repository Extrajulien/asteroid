#ifndef FILES_H
#define FILES_H
#define PRESET_FILE_NAME "../asteroidsTraits.csv"
#define BUFFER_SIZE 512
#include "asteroid.h"

typedef struct AsteroidPresetArray AsteroidPresetArray;

void FILE_LoadGamePresets(AsteroidPresetArray *presets);
void FILE_LoadAllPresets(AsteroidPresetArray *presets);
char AsteroidSizeToChar(AsteroidSize size);
void savePreset(const AsteroidPreset *preset, char *presetName);


#endif //FILES_H
