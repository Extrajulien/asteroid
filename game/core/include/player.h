#pragma once
#include "bullet_array.h"
#include "components.h"
#include "raylib.h"

static const float PLAYER_BULLET_LIFE_DISTANCE = 2500;
static const unsigned int PLAYER_MAX_BULLETS = 10;

typedef struct Player Player;
typedef struct Bullet Bullet;


Player* PLAYER_Create();
void PLAYER_Free(Player *player);
void PLAYER_Reset(Player *player);
void PLAYER_Respawn(Player *player);
void PLAYER_Draw(const Player *player);
void PLAYER_Update(Player *player, BulletArray *bulletArr);
void PLAYER_Die(Player *player);
bool PLAYER_IsDead(const Player *player);

Vector2 PLAYER_GetSpeed(const Player *player);
Vector2 PLAYER_GetPosition(const Player *player);
void PLAYER_AddScore(Player *player, int score);
int PLAYER_GetScore(const Player *player);
int PLAYER_GetLifeCount(const Player *player);
float PLAYER_GetAngle(const Player *player);
SpawnExclusionCircle PLAYER_GetExclusionCircle(const Player *player);
bool PLAYER_IsLineInBounds(const Player *player, const Vector2* start, const Vector2* end);