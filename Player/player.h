//
// Created by julie on 2025-04-12.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define MAX_STRETCH 0.15f
#define MAX_PLAYER_SPEED 10
#define MAX_BULLETS 10
#define SQUARE(x) ((x) * (x))

typedef struct Player{
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
    int lives;
    void (*die)(struct Player *self);
} Player;

typedef struct {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    float distance;
}Bullet;

void playerDie(Player *player);
void initPlayer(Player *player);
void resetPlayer(Player *player);
void thrust(Player *player, float thrustTime);
void glide(Player *player);
void stretchPlayer(Player *player, float thrustTime);
void resile(Player *player);
void wrapAroundPlayer(Player *player);
void shoot(const Player *player, Bullet *bullet, float speed);
void drawPlayer(const Player *player);
void setPlayerBorders(Player *player);

#endif //PLAYER_H
