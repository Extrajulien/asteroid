#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define MAX_STRETCH 0.15f
#define THRUST_RAMP_TIME 1.0f
#define SPEED_DAMPING 0.5f
#define MAX_PLAYER_SPEED 1000 // px per sec
#define BULLET_LIFE_DISTANCE 25000
#define MAX_BULLETS 10
#define SQUARE(x) ((x) * (x))

typedef struct Player {
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
} Bullet;


void initPlayer(Player *player);
void resetPlayer(Player *player);
void thrust(Player *player, float thrustTime);
void shoot(const Player *player, Bullet *bullet, float speed);
void drawPlayer(const Player *player);
void updatePlayer(Player *player, Bullet *bulletArr);

#endif //PLAYER_H
