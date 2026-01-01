#include "player.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "game_api.h"
#include "raymath.h"

void updatePlayerMovement(Player *player, Bullet *bulletArr);
void drawTopPlayer(const Player *player);
void drawThrust(const Player *player);
void stretchPlayer(Player *player, float thrustTime);
void glide(Player *player);
void updateCollisionBorders(Player *player);
void wrapAroundPlayer(Player *player);
void playerDie(Player *player);
void resile(Player *player);

void drawPlayer(const Player *player) {
    drawThrust(player);
    DrawTriangle(player->tip, player->backLeft, player->backRight, WHITE);//white border & hitbox
    drawTopPlayer(player);
}

void updatePlayer(Player *player, Bullet *bulletArr) {
    updatePlayerMovement(player, bulletArr);

    glide(player);
    wrapAroundPlayer(player);
    updateCollisionBorders(player);
}

void updatePlayerMovement(Player *player, Bullet *bulletArr) {
    const float rotationSpeed = 230.0f;
    static float howLongPressed = 0; //in seconds

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        howLongPressed += GetFrameTime();
        thrust(player, howLongPressed);
    } else {
        howLongPressed = 0;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player->angle -= rotationSpeed * GetFrameTime(); //rotate
        if (player->angle <= 0) player->angle = 360;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player->angle += rotationSpeed * GetFrameTime();
        if (player->angle >= 360) player->angle = 0;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        shoot(player, bulletArr, 10);
    }
    if (IsKeyDown(KEY_R)) resetPlayer(player);
}

void wrapAroundPlayer(Player *player) {
    if (player->position.x > (float) GetScreenWidth() + player->radius) player->position.x = -player->radius;
    if (player->position.y > (float) GetScreenHeight() + player->radius) player->position.y = -player->radius;
    if (player->position.x < -player->radius) player->position.x = (float) GetScreenWidth() + player->radius;
    if (player->position.y < -player->radius) player->position.y = (float) GetScreenHeight() + player->radius;
}

void resetPlayer(Player *player) {
    player->angle = 90.0f;
    player->position = (Vector2) {((float) GetScreenWidth() / 2), ((float) GetScreenHeight() / 2)};
    player->radius = 40;
    player->borderWidth = 6;
    player->angleBackLeft = (7.0f * PI) / 6.0f;
    player->angleBackRight = (5.0f * PI) / 6.0f;
    player->speed = (Vector2) {0, 0};
}

void initPlayer(Player *player) {
    player->angle = 90.0f;
    player->position = (Vector2) {((float) GetScreenWidth() / 2),  ((float) GetScreenHeight() / 2)};
    player->radius = 40;
    player->borderWidth = 6;
    player->angleBackLeft = (7.0f * PI) / 6.0f;
    player->angleBackRight = (5.0f * PI) / 6.0f;
    player->speed = (Vector2) {0, 0};
    player->lives = 2;
    player->die = playerDie;
}

void glide(Player *player) {
    player->position.x += player->speed.x * GetFrameTime();
    player->position.y -= player->speed.y * GetFrameTime();


    player->speed.x *= expf(- SPEED_DAMPING * GetFrameTime());
    player->speed.y *= expf(- SPEED_DAMPING * GetFrameTime());



    if (fabsf(player->speed.x) < 0.0001) player->speed.x = 0;
    if (fabsf(player->speed.y) < 0.0001) player->speed.y = 0;

    resile(player);
}

void thrust(Player *player, const float thrustTime) {
    const static float THRUST_POWER = MAX_PLAYER_SPEED / SQUARE( THRUST_RAMP_TIME );

    const float currentPower = SQUARE(thrustTime) * THRUST_POWER;

    const float previousPower = SQUARE(thrustTime - GetFrameTime()) * THRUST_POWER;


    const float powerIncrement = currentPower - previousPower;

    const float angleRad = player->angle * DEG2RAD;
    player->speed.x += (powerIncrement * cosf(angleRad));
    player->speed.y += (powerIncrement * sinf(angleRad));

    if (Vector2Length(player->speed) > MAX_PLAYER_SPEED) {
        player->speed = Vector2Normalize(player->speed);
        player->speed.x *= MAX_PLAYER_SPEED;
        player->speed.y *= MAX_PLAYER_SPEED;
    }

    stretchPlayer(player, thrustTime);
}

//---DRAW FUNCTIONS-----------------------------------------------------
void updateCollisionBorders(Player *player) {
    const float angleRad = player->angle * DEG2RAD;
    player->tip = (Vector2) {
        player->position.x + (player->radius * cosf(angleRad)),
        player->position.y - (player->radius * sinf(angleRad))
    };
    player->backLeft = (Vector2) {
        player->position.x + (player->radius * cosf(angleRad - player->angleBackLeft)),
        player->position.y - (player->radius * sinf(angleRad - player->angleBackLeft))
    };
    player->backRight = (Vector2) {
        player->position.x + (player->radius * cosf(angleRad - player->angleBackRight)),
        player->position.y - (player->radius * sinf(angleRad - player->angleBackRight))
    };
}


void shoot(const Player *player, Bullet *bullet, const float speed) {
    const float angleRad = player->angle * DEG2RAD;
    const float playerSpeedNudgeX = fabsf(player->speed.x * GetFrameTime()) / 4;
    const float playerSpeedNudgeY = fabsf(player->speed.y * GetFrameTime()) / 4;

    int index = 0;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet[i].distance <= 0) {
            index = i;
            break;
        }
    }
    bullet[index].distance = BULLET_LIFE_DISTANCE;
    bullet[index].position = (Vector2) {player->position.x, player->position.y};
    bullet[index].speed = (Vector2) {
            (speed *  cosf(angleRad) * (1 + playerSpeedNudgeX)),
            (speed * -sinf(angleRad) * (1 + playerSpeedNudgeY))
    };
}

void playerDie(Player *player) {
    if (player->lives > 0) {
        player->lives--;
        resetPlayer(player);
        return;
    }
    gameoverPlayer(player);
}

void stretchPlayer(Player *player, const float thrustTime) {
    player->angleBackLeft -= SQUARE(thrustTime) / 2;
    player->angleBackRight += SQUARE(thrustTime) / 2;

    if (player->angleBackLeft < ((7.0f * PI) / 6.0f) - MAX_STRETCH)
        player->angleBackLeft = ((7.0f * PI) / 6.0f) - MAX_STRETCH;
    if (player->angleBackRight > ((5.0f * PI) / 6.0f) + MAX_STRETCH)
        player->angleBackRight = ((5.0f * PI) / 6.0f) + MAX_STRETCH;
}

void resile(Player *player) {
    player->angleBackLeft += 0.01f;
    player->angleBackRight -= 0.01f;

    if (player->angleBackLeft > ((7.0f * PI) / 6.0f)) player->angleBackLeft = ((7.0f * PI) / 6.0f);
    if (player->angleBackRight < ((5.0f * PI) / 6.0f)) player->angleBackRight = ((5.0f * PI) / 6.0f);
}

void drawThrust(const Player *player) {
    const float angleRad = player->angle * DEG2RAD;
    const Vector2 pos = player->position;
    const float radius = player->radius;
    Vector2 speed = player->speed;
    speed.x *= GetFrameTime();
    speed.y *= GetFrameTime();

    //jitter
    const float jitterAmount = 4.0f;
    Vector2 jitter;
    jitter.x = (float) rand() * jitterAmount / (float) RAND_MAX - jitterAmount / 2;
    jitter.y = (float) rand() * jitterAmount / (float) RAND_MAX - jitterAmount / 2;

    //visible tip
    const Vector2 tip = (Vector2) {pos.x + (radius * cosf(angleRad)) * -fabsf(speed.x * 0.5f) + (jitter.x * speed.x),
                                   pos.y - (radius * sinf(angleRad)) * -fabsf(speed.y * 0.5f) + jitter.y * speed.y};

    //hidden corners
    const Vector2 backLeft = (Vector2) {pos.x + ((radius / 2) * cosf(angleRad - player->angleBackLeft)),
                                        pos.y - ((radius / 2) * sinf(angleRad - player->angleBackLeft))};
    const Vector2 backRight = (Vector2) {pos.x + ((radius / 2) * cosf(angleRad - player->angleBackRight)),
                                         pos.y - ((radius / 2) * sinf(angleRad - player->angleBackRight))};


    DrawTriangleLines(tip, backLeft, backRight, ORANGE);
}

void drawTopPlayer(const Player *player) {
    const float angleRad = player->angle * DEG2RAD;
    Vector2 tip = player->tip;
    Vector2 backLeft = player->backLeft;
    Vector2 backRight = player->backRight;
    tip.x -= player->borderWidth * cosf(angleRad);
    tip.y += player->borderWidth * sinf(angleRad);

    backLeft.x -= (player->borderWidth) * cosf(angleRad - player->angleBackLeft - 0.5f);
    backLeft.y += (player->borderWidth) * sinf(angleRad - player->angleBackLeft - 0.5f);

    backRight.x -= (player->borderWidth) * cosf(angleRad - player->angleBackRight + 0.5f);
    backRight.y += (player->borderWidth) * sinf(angleRad - player->angleBackRight + 0.5f);

    DrawTriangle(tip, backLeft, backRight, BLACK);
}
