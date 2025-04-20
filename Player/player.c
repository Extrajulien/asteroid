//
// Created by julie on 2025-04-12.
//

#include "player.h"
#include <math.h>
#include <stdlib.h>


void stretchPlayer(Player *player, float thrustTime) {
    player->angleBackLeft -= SQUARE(thrustTime) / 2;
    player->angleBackRight += SQUARE(thrustTime) / 2;

    if (player->angleBackLeft < ((7.0 * PI) / 6.0) - MAX_STRECH)
        player->angleBackLeft = ((7.0 * PI) / 6.0) - MAX_STRECH;
    if (player->angleBackRight > ((5.0 * PI) / 6.0) + MAX_STRECH)
        player->angleBackRight = ((5.0 * PI) / 6.0) + MAX_STRECH;
}

void resile(Player *player) {
    player->angleBackLeft += 0.01;
    player->angleBackRight -= 0.01;

    if (player->angleBackLeft > ((7.0 * PI) / 6.0)) player->angleBackLeft = ((7.0 * PI) / 6.0);
    if (player->angleBackRight < ((5.0 * PI) / 6.0)) player->angleBackRight = ((5.0 * PI) / 6.0);
}

void wraparoundPlayer(Player *player) {
    if (player->position.x > GetScreenWidth() + player->radius) player->position.x = -player->radius;
    if (player->position.y > GetScreenHeight() + player->radius) player->position.y = -player->radius;
    if (player->position.x < -player->radius) player->position.x = GetScreenWidth() + player->radius;
    if (player->position.y < -player->radius) player->position.y = GetScreenHeight() + player->radius;
}

void resetPlayer(Player *player) {
    player->angle = 90.0;
    player->position = (Vector2) {(GetScreenWidth() / 2), (GetScreenHeight() / 2)};
    player->radius = 40;
    player->borderWidth = 6;
    player->angleBackLeft = (7.0 * PI) / 6.0;
    player->angleBackRight = (5.0 * PI) / 6.0;
    player->speed = (Vector2) {0, 0};
}

void glide(Player *player) {
    player->speed.x = player->speed.x / (GetFrameTime() + 1);
    player->speed.y = player->speed.y / (GetFrameTime() + 1);

    player->position.x += player->speed.x;
    player->position.y -= player->speed.y;

    if (fabsf(player->speed.x) < 0.01) player->speed.x = 0;
    if (fabsf(player->speed.y) < 0.01) player->speed.y = 0;

    resile(player);
}

void thrust(Player *player, float thrustTime) {
    float power = thrustTime * thrustTime * 500;
    float angleRad = player->angle * PI / 180;
    if (power > MAX_PLAYER_SPEED) {
        power = MAX_PLAYER_SPEED;
    }
    player->speed.x += (power * cosf(angleRad)) * GetFrameTime();
    player->speed.y += (power * sinf(angleRad)) * GetFrameTime();
    stretchPlayer(player, thrustTime);
}

//---DRAW FUNCTIONS-----------------------------------------------------

void thrustGraphic(Player *player) {
    const float angleRad = player->angle * DEG2RAD;
    const Vector2 pos = player->position;
    const float radius = player->radius;
    Vector2 speed = player->speed;

    //jitter
    const int jitterAmnt = 4;
    Vector2 jitter;
    jitter.x = (rand() % jitterAmnt) - jitterAmnt / 2;
    jitter.y = (rand() % jitterAmnt) - jitterAmnt / 2;

    //visible tip
    const Vector2 tip = (Vector2) {pos.x + (radius * cos(angleRad)) * (-fabs(speed.x) / 3) + (jitter.x * speed.x),
                                   pos.y - (radius * sin(angleRad)) * (-fabs(speed.y / 3)) + jitter.y * speed.y};

    //hidden corners
    const Vector2 backLeft = (Vector2) {pos.x + ((radius / 2) * cos(angleRad - player->angleBackLeft)),
                                        pos.y - ((radius / 2) * sin(angleRad - player->angleBackLeft))};
    const Vector2 backRight = (Vector2) {pos.x + ((radius / 2) * cos(angleRad - player->angleBackRight)),
                                         pos.y - ((radius / 2) * sin(angleRad - player->angleBackRight))};


    DrawTriangleLines(tip, backLeft, backRight, ORANGE);
}

void drawPlayer(Player *player) {
    thrustGraphic(player);
    DrawTriangle(player->tip, player->backLeft, player->backRight, WHITE);//white border & hitbox
    drawTopPlayer(player);
}

void setPlayerBorders(Player *player) {
    float angleRad = player->angle * DEG2RAD;
    Vector2 pos = player->position;

    player->tip = (Vector2) {pos.x + (player->radius * cos(angleRad)), pos.y - (player->radius * sin(angleRad))};

    player->backLeft = (Vector2) {pos.x + (player->radius * cos(angleRad - player->angleBackLeft)),
                                  pos.y - (player->radius * sin(angleRad - player->angleBackLeft))};

    player->backRight = (Vector2) {pos.x + (player->radius * cos(angleRad - player->angleBackRight)),
                                   pos.y - (player->radius * sin(angleRad - player->angleBackRight))};
}

void drawTopPlayer(Player *player) {//cover noire rendered on top
    float angleRad = player->angle * DEG2RAD;
    Vector2 tip = player->tip;
    Vector2 backLeft = player->backLeft;
    Vector2 backRight = player->backRight;
    tip.x -= player->borderWidth * cos(angleRad);
    tip.y += player->borderWidth * sin(angleRad);

    backLeft.x -= (player->borderWidth) * cos(angleRad - player->angleBackLeft - 0.5);
    backLeft.y += (player->borderWidth) * sin(angleRad - player->angleBackLeft - 0.5);

    backRight.x -= (player->borderWidth) * cos(angleRad - player->angleBackRight + 0.5);
    backRight.y += (player->borderWidth) * sin(angleRad - player->angleBackRight + 0.5);

    DrawTriangle(tip, backLeft, backRight, BLACK);
}

void shoot(Player *player, Bullet *bullet, float speed) {
    float angleRad = player -> angle * DEG2RAD;

    int index = 0;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet[i].distance <= 0) {
            index = i;
            break;
        }
    }
    bullet[index].distance = 2500;//life distance
    bullet[index].size.x = 5;
    bullet[index].size.y = 5;
    bullet[index].position = (Vector2) {player->position.x, player->position.y};
    bullet[index].speed = (Vector2) {
            (speed * cosf(angleRad) * ((fabsf(player->speed.x) / 4) + 1)),
            (speed * -sinf(angleRad) * ((fabsf(player->speed.y) / 4) + 1))};
}
