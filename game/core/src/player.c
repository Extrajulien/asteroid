#include "player.h"
#include "game_math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "game_api.h"
#include "raymath.h"

static const float PLAYER_MAX_STRETCH = 0.15f;
static const float PLAYER_THRUST_RAMP_TIME = 1.0f;
static const float PLAYER_SPEED_DAMPING = 0.5f;
static const float PLAYER_MAX_PLAYER_SPEED = 1000; // px per sec
static const float PLAYER_SPAWN_SAFE_ZONE = 100;

typedef struct {
    Vector2 tip;
    Vector2 backLeft;
    Vector2 backRight;
} Triangle;

typedef struct Player {
    Triangle bounds;
    Vector2 position;
    Score score;
    float angle;
    Vector2 speed;
    float radius;
    float borderWidth;
    float angleBackLeft;
    float angleBackRight;
    int lives;
} Player;

void drawPlayerOutline(const Player *player);
void thrust(Player *player, float thrustTime);
void updatePlayerMovement(Player *player, BulletArray *bulletArr);
void shoot(const Player *player, BulletArray *bullet, float speed);
void drawTopPlayer(const Player *player);
void drawThrust(const Player *player);
void stretchPlayer(Player *player, float thrustTime);
void glide(Player *player);
void updateCollisionBorders(Player *player);
void wrapAroundPlayer(Player *player);
void playerDie(Player *player);
void resile(Player *player);

Player* PLAYER_Create() {
    Player *player = malloc(sizeof(Player));
    if (!player) {
        printf("PLAYER_Create - malloc failed\n");
        exit(1);
    }
    PLAYER_Reset(player);
    return player;
}

Vector2 PLAYER_GetSpeed(const Player *player) {
    return player->speed;
}

void PLAYER_Free(Player *player) {
    free(player);
}

void PLAYER_Reset(Player* player) {
    player->angle = 90.0f;
    player->position = (Vector2) {((float) GetScreenWidth() / 2), ((float) GetScreenHeight() / 2)};
    player->radius = 40;
    player->borderWidth = 6;
    player->angleBackLeft = (7.0f * PI) / 6.0f;
    player->angleBackRight = (5.0f * PI) / 6.0f;
    player->speed = (Vector2) {0, 0};
    player->score = 0;
    player->lives = 2;
}

void PLAYER_Respawn(Player *player) {
    player->angle = 90.0f;
    player->position = (Vector2) {((float) GetScreenWidth() / 2), ((float) GetScreenHeight() / 2)};
    player->speed = (Vector2) {0, 0};
}

void PLAYER_Draw(const Player *player) {
    drawThrust(player);
    drawPlayerOutline(player);
    drawTopPlayer(player);
}

void PLAYER_Update(Player *player, BulletArray *bulletArr) {
    updatePlayerMovement(player, bulletArr);
    glide(player);
    wrapAroundPlayer(player);
    updateCollisionBorders(player);
}

bool PLAYER_IsLineInBounds(const Player *player, const Vector2* start, const Vector2* end) {
    return CheckCollisionLines(player->bounds.tip, player->bounds.backLeft,*start, *end, NULL)
                || CheckCollisionLines(player->bounds.backLeft, player->bounds.backRight, *start, *end, NULL)
                || CheckCollisionLines(player->bounds.backRight, player->bounds.tip, *start, *end, NULL);
}

void PLAYER_Die(Player *player) {
    if (player->lives > 0) {
        player->lives--;
        PLAYER_Respawn(player);
    }
}

bool PLAYER_IsDead(const Player *player) {
    return player->lives <= 0;
}

Vector2 PLAYER_GetPosition(const Player *player) {
    return player->position;
}

int PLAYER_GetLifeCount(const Player *player) {
    return player->lives;
}

float PLAYER_GetAngle(const Player *player) {
    return player->angle;
}

void PLAYER_AddScore(Player *player, const int score) {
    player->score += score;
}
int PLAYER_GetScore(const Player *player) {
    return player->score;
}

SpawnExclusionCircle PLAYER_GetExclusionCircle(const Player *player) {
    return (SpawnExclusionCircle) {player->position, PLAYER_SPAWN_SAFE_ZONE};
}






void updatePlayerMovement(Player *player, BulletArray *bulletArr) {
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
        shoot(player, bulletArr, 1200);
    }
    if (IsKeyDown(KEY_R)) PLAYER_Reset(player);
}

void wrapAroundPlayer(Player *player) {
    WrapAroundScreen(&player->position, player->radius);
}

void glide(Player *player) {
    player->position.x += player->speed.x * GetFrameTime();
    player->position.y -= player->speed.y * GetFrameTime();


    player->speed.x *= expf(- PLAYER_SPEED_DAMPING * GetFrameTime());
    player->speed.y *= expf(- PLAYER_SPEED_DAMPING * GetFrameTime());



    if (fabsf(player->speed.x) < 0.0001) player->speed.x = 0;
    if (fabsf(player->speed.y) < 0.0001) player->speed.y = 0;

    resile(player);
}

void thrust(Player *player, const float thrustTime) {
    const float THRUST_POWER = PLAYER_MAX_PLAYER_SPEED / Square( PLAYER_THRUST_RAMP_TIME );

    const float currentPower = Square(thrustTime) * THRUST_POWER;
    const float previousPower = Square(thrustTime - GetFrameTime()) * THRUST_POWER;
    const float powerIncrement = currentPower - previousPower;

    const float angleRad = player->angle * DEG2RAD;
    player->speed.x += (powerIncrement * cosf(angleRad));
    player->speed.y += (powerIncrement * sinf(angleRad));

    if (Vector2Length(player->speed) > PLAYER_MAX_PLAYER_SPEED) {
        player->speed = Vector2Normalize(player->speed);
        player->speed.x *= PLAYER_MAX_PLAYER_SPEED;
        player->speed.y *= PLAYER_MAX_PLAYER_SPEED;
    }

    stretchPlayer(player, thrustTime);
}

//---DRAW FUNCTIONS-----------------------------------------------------
void updateCollisionBorders(Player *player) {
    const float angleRad = player->angle * DEG2RAD;
    player->bounds.tip = (Vector2) {
        player->position.x + (player->radius * cosf(angleRad)),
        player->position.y - (player->radius * sinf(angleRad))
    };
    player->bounds.backLeft = (Vector2) {
        player->position.x + (player->radius * cosf(angleRad - player->angleBackLeft)),
        player->position.y - (player->radius * sinf(angleRad - player->angleBackLeft))
    };
    player->bounds.backRight = (Vector2) {
        player->position.x + (player->radius * cosf(angleRad - player->angleBackRight)),
        player->position.y - (player->radius * sinf(angleRad - player->angleBackRight))
    };
}


void shoot(const Player *player, BulletArray *bulletArr, const float speed) {
    const float angleRad = player->angle * DEG2RAD;
    const float playerSpeedNudgeX = fabsf(player->speed.x * GetFrameTime()) / 4;
    const float playerSpeedNudgeY = fabsf(player->speed.y * GetFrameTime()) / 4;
    const Vector2 bulletSpeed = {
        (speed *  cosf(angleRad) * (1 + playerSpeedNudgeX)),
        (speed * -sinf(angleRad) * (1 + playerSpeedNudgeY))
    };
    BULLETS_Spawn(bulletArr, player->position, (Vector2){5,5}, bulletSpeed, PLAYER_BULLET_LIFE_DISTANCE);

}

void stretchPlayer(Player *player, const float thrustTime) {
    player->angleBackLeft -= Square(thrustTime) / 2;
    player->angleBackRight += Square(thrustTime) / 2;

    if (player->angleBackLeft < ((7.0f * PI) / 6.0f) - PLAYER_MAX_STRETCH)
        player->angleBackLeft = ((7.0f * PI) / 6.0f) - PLAYER_MAX_STRETCH;
    if (player->angleBackRight > ((5.0f * PI) / 6.0f) + PLAYER_MAX_STRETCH)
        player->angleBackRight = ((5.0f * PI) / 6.0f) + PLAYER_MAX_STRETCH;
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
    Vector2 tip = player->bounds.tip;
    Vector2 backLeft = player->bounds.backLeft;
    Vector2 backRight = player->bounds.backRight;
    tip.x -= player->borderWidth * cosf(angleRad);
    tip.y += player->borderWidth * sinf(angleRad);

    backLeft.x -= (player->borderWidth) * cosf(angleRad - player->angleBackLeft - 0.5f);
    backLeft.y += (player->borderWidth) * sinf(angleRad - player->angleBackLeft - 0.5f);

    backRight.x -= (player->borderWidth) * cosf(angleRad - player->angleBackRight + 0.5f);
    backRight.y += (player->borderWidth) * sinf(angleRad - player->angleBackRight + 0.5f);

    DrawTriangle(tip, backLeft, backRight, BLACK);
}

void drawPlayerOutline(const Player *player) {
    DrawTriangle(player->bounds.tip, player->bounds.backLeft, player->bounds.backRight, WHITE);
}
