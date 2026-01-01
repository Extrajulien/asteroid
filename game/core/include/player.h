#pragma once
#include "raylib.h"

static const float PLAYER_MAX_STRETCH = 0.15f;
static const float PLAYER_THRUST_RAMP_TIME = 1.0f;
static const float PLAYER_SPEED_DAMPING = 0.5f;
static const float PLAYER_MAX_PLAYER_SPEED = 1000; // px per sec
static const float PLAYER_BULLET_LIFE_DISTANCE = 25000;
static const unsigned int PLAYER_MAX_BULLETS = 10;

typedef struct {
    Vector2 tip;
    Vector2 backLeft;
    Vector2 backRight;
} Triangle;

typedef struct Player {
    Triangle bounds;
    Vector2 position;
    float angle;
    Vector2 speed;
    float radius;
    float borderWidth;
    float angleBackLeft;
    float angleBackRight;
    int lives;
    void (*die)(struct Player *self);
} Player;

typedef struct {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    float distance;
} Bullet;


void PLAYER_Init(Player *player);
void PLAYER_Reset(Player *player);
void PLAYER_Draw(const Player *player);
void PLAYER_Update(Player *player, Bullet *bulletArr);
bool PLAYER_IsLineInBounds(const Player *player, const Vector2* start, const Vector2* end);