#include "files.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    PRESET_IS_ACTIVE = 0,
    PRESET_NAME = 1,
    PRESET_SIZE = 2,
    PRESET_RADIUS = 3,
    PRESET_SPREAD = 4,
    PRESET_MIN_ROTATION_SPEED = 5,
    PRESET_MAX_ROTATION_SPEED = 6,
    PRESET_SCORE = 7,
    PRESET_VERTICE_COUNT = 8,
    PRESET_IS_RANDOM_2_SIDE = 9,
    PRESET_MAX_SPEED = 10,
    PRESET_SEMICOLON_AMOUNT = 10
} PresetFileToken;

bool validatePresetName(char *presetName);
bool* createSizeCheckArray(bool *isSizeLoaded, int size);
AsteroidSize letterToAsteroidSize(char letter);
char AsteroidSizeToChar(AsteroidSize size);
AsteroidPreset readPreset(char *line);
FILE *openPresetFile(const char *filename);
void logPreset(const AsteroidPreset *preset);

void createPresetFile();

void readPresetFile(AsteroidPresetArray *presets) {
    FILE *presetFile = openPresetFile(PRESET_FILE_NAME);

    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, presetFile); //skip header

    const int checkArraySize = ASTEROID_SIZE_COUNT;
    bool isSizeLoaded[checkArraySize];
    bool *isSizePresent = createSizeCheckArray(isSizeLoaded, checkArraySize);


    while (fgets(buffer, BUFFER_SIZE, presetFile)) {
        if (buffer[0] != '1') continue; // ne prend en consideration que ceux qui sont actifs


        AsteroidPreset preset = readPreset(buffer);

        preset.lineInfo.color = BLUE;

        preset.lineInfo.thickness = 2;

        preset.particlePreset = (ParticlePreset) {
            12,
            70.0f,
            0.4f,
            YELLOW,
            1200.0f
        };

        if (isSizePresent[preset.size] == false) {
            ASTEROID_PRESETS_Add(presets, &preset);
            isSizePresent[preset.size] = true;
        }



        logPreset(&preset);
    }
    fclose(presetFile);
}

void savePreset(const AsteroidPreset *preset, char *presetName) {
    if (validatePresetName(presetName)){
        FILE *presetFile = fopen(PRESET_FILE_NAME, "a");
        fprintf(presetFile, "0;%s;%c;%.2f;%.2f;%.2f;%.2f;%d;%d;%d;%.2f\n",
            presetName, AsteroidSizeToChar(preset->size),preset->radius,preset->spread,preset->minRotationSpeed,
            preset->maxRotationSpeed, preset->score, preset->nbVertices,
            preset->isRandomBothSides, preset->maxSpeed);
        fclose(presetFile);
    }
}

bool validatePresetName(char *presetName) {
    char spacelessName[strlen(presetName)];
    int pos = 0;
    for (int i = 0;i < strlen(presetName);i++) {
        if (isalpha(presetName[i])) {
            spacelessName[pos] = presetName[i];
            pos++;
        }
    }
    printf("TEXT--'%s' -> '%s'\n", presetName, spacelessName);
    spacelessName[pos] = '\0';
    strcpy(presetName, spacelessName);
    return true;
}

void createPresetFile() {
    FILE *presetFile = fopen(PRESET_FILE_NAME, "w");
    fprintf(presetFile, "isUsed;Nom;AsteroidType;Radius;Spread;MinRotationSpeed;MaxRotationSpeed;score;nbVertices;generationStyle;MaxSpeed\n");
    fprintf(presetFile, "1;DEFAULT;B;100.0;20;-30;30;20;25;1;10\n");
    fprintf(presetFile, "1;DEFAULT;M;65.0;25;-40;40;50;18;1;12\n");
    fprintf(presetFile, "1;DEFAULT;S;35.0;15;-50;50;100;12;1;15\n");
    fclose(presetFile);
}

void logPreset(const AsteroidPreset *preset) {
    printf("Config:\t\t %s-%c\n", preset->presetName, AsteroidSizeToChar(preset->size));
    printf("radius:\t\t %.2f\n", preset->radius);
    printf("Spread:\t\t %.2f\n", preset->spread);
    printf("Min Spin Speed:\t %.2f\n", preset->minRotationSpeed);
    printf("Max Spin Speed:\t %.2f\n", preset->maxRotationSpeed);
    printf("Score:\t\t %d\n\n", preset->score);
    printf("Vertices:\t\t %d\n", preset->nbVertices);
    printf("Random both sides:\t\t %d\n", preset->isRandomBothSides);
    printf("Max Speed:\t %.2f\n", preset->maxSpeed);
}
AsteroidSize letterToAsteroidSize(const char letter) {
    switch (letter) {
        case 'B': return SIZE_BIG;
        case 'M': return SIZE_MEDIUM;
        case 'S': return SIZE_SMALL;
        default:
            printf("le type d'astéroide 'S/M/B' est invalide : '%d'", letter);
            exit(1);
    }
}

char AsteroidSizeToChar(const AsteroidSize size) {
    switch (size) {
        case SIZE_BIG: return 'B';
        case SIZE_MEDIUM: return 'M';
        case SIZE_SMALL: return 'S';
    }
    printf("AsteroidSizeToChar: size invalid '%d'\n", size);
    exit(1);
}

AsteroidPreset readPreset(char *line) {
    int indexEntry = 0;
    char *entry[PRESET_SEMICOLON_AMOUNT + 1];
    char *token = strtok(line, ";");

    while (token && indexEntry < PRESET_SEMICOLON_AMOUNT + 1) {
        entry[indexEntry++] = token;
        token = strtok(NULL, ";");
    }

    AsteroidPreset preset = {};
    preset.presetName = entry[PRESET_NAME];
    preset.size = letterToAsteroidSize(entry[PRESET_SIZE][0]);
    preset.radius = atof(entry[PRESET_RADIUS]);
    preset.spread = atof(entry[PRESET_SPREAD]);
    preset.minRotationSpeed = atof(entry[PRESET_MIN_ROTATION_SPEED]);
    preset.maxRotationSpeed = atof(entry[PRESET_MAX_ROTATION_SPEED]);
    preset.score = atoi(entry[PRESET_SCORE]);
    preset.nbVertices = atoi(entry[PRESET_VERTICE_COUNT]);
    preset.isRandomBothSides = atoi(entry[PRESET_IS_RANDOM_2_SIDE]);
    preset.maxSpeed = atof(entry[PRESET_MAX_SPEED]);
    return preset;
}

FILE* openPresetFile(const char *filename) {
    FILE *presetFile = fopen(filename, "r");
    if (presetFile == NULL) {
        printf("Created: '%s'\n", filename);
        createPresetFile();
        presetFile = fopen(filename, "r");
    }
    return presetFile;
}

bool* createSizeCheckArray(bool *isSizeLoaded, const int size) {
    for (int i = 0; i < size; i++) {
        isSizeLoaded[i] = false;
    }
    return isSizeLoaded;
}