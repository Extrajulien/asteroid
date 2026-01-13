#pragma once
#include "raylib.h"

typedef struct Bullet Bullet;



void BULLET_Render(const Bullet *bullet);
void BULLET_Move(Bullet *bullet);

Vector2 BULLET_GetPosition(const Bullet *bullet);
Vector2 BULLET_GetSize(const Bullet *bullet);
Vector2 BULLET_GetSpeed(const Bullet *bullet);
Rectangle BULLET_GetCollisionRectangle(const Bullet *bullet);