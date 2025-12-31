#include "files.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AsteroidTraits bigTraits = {BIG,100,20,-30,30, 100,20, 25, 0};
AsteroidTraits midTraits = {MEDIUM,65,25,-40,40, 100,50, 18, 0};
AsteroidTraits smlTraits = {SMALL,35,15,-50,50, 100, 100, 12, 0};

void createPresetFile();

void readPresetFile() {
    const int maxSemicolon = 10;
    const int indexName = 1;
    const int indexType = 2;
    const int indexRadius = 3;
    const int indexSpread = 4;
    const int indexMinRotationSpeed = 5;
    const int indexMaxRotationSpeed = 6;
    const int indexScore = 7;
    const int indexVertices = 8;
    const int indexGenerationStyle = 9;
    const int indexMaxSpeed = 10;
    char buffer[BUFFER_SIZE];
    FILE *asteroidsTraits = fopen(PRESET_FILE_NAME, "r");
    if (asteroidsTraits == NULL) {
        printf("Created: '%s'\n", PRESET_FILE_NAME);
        createPresetFile();
        asteroidsTraits = fopen(PRESET_FILE_NAME, "r");
    }

    fgets(buffer, BUFFER_SIZE, asteroidsTraits); //skip titres
    while (fgets(buffer, BUFFER_SIZE, asteroidsTraits)) {
        if (buffer[0] != '1') continue; // ne prend en consideration que ceux qui sont actifs
        int indexEntry = 0;
        char *entry[maxSemicolon + 1];
        char *token = strtok(buffer, ";");

        while (token && indexEntry < maxSemicolon + 1) {
            entry[indexEntry++] = token;
            token = strtok(NULL, ";");
        }

        AsteroidTraits *pTraits = NULL;
        switch (entry[indexType][0]) {
            case 'B': pTraits = &bigTraits;
                break;
            case 'M': pTraits = &midTraits;
                break;
            case 'S': pTraits = &smlTraits;
                break;
            default:
                printf("le type d'astéroide 'S/M/B' est invalide");
                exit(1);
        }

        pTraits->radius = atof(entry[indexRadius]);
        pTraits->spread = atof(entry[indexSpread]);
        pTraits->minRotationSpeed = atof(entry[indexMinRotationSpeed]);
        pTraits->maxRotationSpeed = atof(entry[indexMaxRotationSpeed]);
        pTraits->score = atoi(entry[indexScore]);
        pTraits->nbVertices = atoi(entry[indexVertices]);
        pTraits->generationStyle = atoi(entry[indexGenerationStyle]);
        pTraits->maxSpeed = atof(entry[indexMaxSpeed]);

        printf("Config:\t\t %s-%s\n", entry[indexName], entry[indexType]);
        printf("radius:\t\t %.2f\n", pTraits->radius);
        printf("Spread:\t\t %.2f\n", pTraits->spread);
        printf("Min Spin Speed:\t %.2f\n", pTraits->minRotationSpeed);
        printf("Max Spin Speed:\t %.2f\n", pTraits->maxRotationSpeed);
        printf("Score:\t\t %d\n\n", pTraits->score);
        printf("Vertices:\t\t %d\n", pTraits->nbVertices);
        printf("Generation style:\t\t %d\n", pTraits->generationStyle);
        printf("Max Speed:\t %.2f\n", pTraits->maxSpeed);
    }
    fclose(asteroidsTraits);
}

void savePreset(char *presetName) {
    if (validatePresetName(presetName)){
        FILE *asteroidsTraits = fopen(PRESET_FILE_NAME, "a");
        fprintf(asteroidsTraits, "0;%s;B;%.2f;%.2f;%.2f;%.2f;%d;%d;%d;%.2f\n",
            presetName,bigTraits.radius,bigTraits.spread,bigTraits.minRotationSpeed,
            bigTraits.maxRotationSpeed, bigTraits.score, bigTraits.nbVertices,
            bigTraits.generationStyle, bigTraits.maxSpeed);
        fprintf(asteroidsTraits, "0;%s;M;%.2f;%.2f;%.2f;%.2f;%d;%d;%d;%.2f\n",
            presetName,midTraits.radius,midTraits.spread,midTraits.minRotationSpeed,
            midTraits.maxRotationSpeed, midTraits.score, midTraits.nbVertices,
            midTraits.generationStyle, midTraits.maxSpeed);
        fprintf(asteroidsTraits, "0;%s;S;%.2f;%.2f;%.2f;%.2f;%d;%d;%d;%.2f\n",
            presetName,smlTraits.radius,smlTraits.spread,smlTraits.minRotationSpeed,
            smlTraits.maxRotationSpeed, smlTraits.score, smlTraits.nbVertices,
            smlTraits.generationStyle, smlTraits.maxSpeed);
        fclose(asteroidsTraits);
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
    FILE *asteroidsTraits = fopen(PRESET_FILE_NAME, "w");
    fprintf(asteroidsTraits, "isUsed;Nom;AsteroidType;Radius;Spread;MinRotationSpeed;MaxRotationSpeed;score;nbVertices;generationStyle;MaxSpeed\n");
    fprintf(asteroidsTraits, "1;DEFAULT;B;100.0;20;-30;30;20;25;1;10\n");
    fprintf(asteroidsTraits, "1;DEFAULT;M;65.0;25;-40;40;50;18;1;12\n");
    fprintf(asteroidsTraits, "1;DEFAULT;S;35.0;15;-50;50;100;12;1;15\n");
    fclose(asteroidsTraits);
}