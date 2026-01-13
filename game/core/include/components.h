#pragma once
#include "raylib.h"

typedef enum COMP {
    COMP_POSITION    = 1 << 0,
    COMP_VELOCITY    = 1 << 1,
    COMP_ROTATION    = 1 << 2,
    COMP_LINE_INFO  = 1 << 3,
    COMP_SCORE       = 1 << 4,
    COMP_SPAWN_EXCLUSION = 1 << 5,
} COMP;

typedef unsigned int ComponentsMask;

typedef struct Vector2 Position;

typedef struct Vector2 Velocity;

typedef struct {
    float rad;
    float increment;
} Rotation;

typedef int Score;

typedef struct {
    Color color;
    float thickness;
} ComponentLineInfo;

typedef struct {
    Vector2 center;
    float radius;
} SpawnExclusionCircle;