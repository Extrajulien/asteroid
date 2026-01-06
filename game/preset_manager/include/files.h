#ifndef FILES_H
#define FILES_H
#define RAYGUI_IMPLEMENTATION
#define PRESET_FILE_NAME "../asteroidsTraits.csv"
#define BUFFER_SIZE 512
#include "asteroid.h"

void readPresetFile(AsteroidPresetArray *presets);
void savePreset(const AsteroidPreset *preset, char *presetName);


#endif //FILES_H
