//
// Created by julie on 2025-04-12.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define MAX_STRECH 0.15
#define MAX_PLAYER_SPEED 10
#define MAX_BULLETS 10
#define SQUARE(x) ((x) * (x))

typedef struct {
    Vector2 tip;
    Vector2 backLeft;
    Vector2 backRight;
    Vector2 position;
    float angle;
    Vector2 speed;
    float radius;
    float borderWidth;
    float angleBackLeft;
    float angleBackRight;
} Player;

typedef struct {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    float distance;
    int possibleTarget;
}Bullet;

void resetPlayer(Player *player);
void thrust(Player *player, float thrustTime);
void glide(Player *player);
void stretchPlayer(Player *player, float thrustTime);
void resile(Player *player);
void wrapAroundPlayer(Player *player);
void shoot(Player *player, Bullet *bullet, float speed);
void drawPlayer(Player *player);
void thrustGraphic(Player *player);
void setPlayerBorders(Player *player);
void drawTopPlayer(Player *player);

#endif //PLAYER_H
