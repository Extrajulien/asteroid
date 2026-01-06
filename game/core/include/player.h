#pragma once
#include "asteroid_event.h"
#include "components.h"
#include "raylib.h"

static const float PLAYER_BULLET_LIFE_DISTANCE = 25000;
static const unsigned int PLAYER_MAX_BULLETS = 10;

typedef struct Player Player;

typedef struct {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    float distance;
} Bullet;


Player* PLAYER_Create();
void PLAYER_Free(Player *player);
void PLAYER_Reset(Player *player);
void PLAYER_Respawn(Player *player);
void PLAYER_Draw(const Player *player);
void PLAYER_Update(Player *player, Bullet *bulletArr);
void PLAYER_Die(Player *player);
bool PLAYER_IsDead(const Player *player);
void PLAYER_UpdateBulletHits(const AsteroidBulletHitEventQueue *queue, Player *player, Bullet *bulletArr);

Vector2 PLAYER_GetSpeed(const Player *player);
Vector2 PLAYER_GetPosition(const Player *player);
void PLAYER_AddScore(Player *player, int score);
int PLAYER_GetScore(const Player *player);
int PLAYER_GetLifeCount(const Player *player);
float PLAYER_GetAngle(const Player *player);
SpawnExclusionCircle PLAYER_GetExclusionCircle(const Player *player);
bool PLAYER_IsLineInBounds(const Player *player, const Vector2* start, const Vector2* end);