#pragma once
#include <stddef.h>

#include "bullet.h"

typedef struct BulletArray {
    size_t count;
    size_t capacity;
    Bullet *bulletArr;
} BulletArray;

BulletArray* BULLETS_CreateArray();
const Bullet* BULLETS_GetBullet(const BulletArray* array, size_t index);
void BULLETS_Purge(BulletArray *array);
void BULLETS_Render(const BulletArray *array);
void BULLETS_Update(BulletArray *array);
void BULLETS_FreeArray(BulletArray *array);
void BULLETS_Destroy(BulletArray *array, size_t index);
void BULLETS_Spawn(BulletArray *array, Vector2 position, Vector2 size, Vector2 speed, float distance);