#ifndef FILES_H
#define FILES_H
#define RAYGUI_IMPLEMENTATION
#define PRESET_FILE_NAME "../asteroidsTraits.csv"
#define BUFFER_SIZE 256
#include "asteroids.h"

void readPresetFile();
void savePreset(char *presetName);
bool validatePresetName(char *presetName);

#endif //FILES_H
