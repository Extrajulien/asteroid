#include "bullet_array.h"
#include "bullet.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_math.h"

Bullet BULLET_Create(Vector2 position, Vector2 size, Vector2 speed, float distance);
void BULLETS_Add(BulletArray *array, Bullet bullet);
bool InvalidateDeadBullet(const BulletArray *array, Bullet *bullet, size_t *count);
void BULLET_WrapAround(Bullet *bullet);

typedef struct Bullet {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    float distance;
} Bullet;


Bullet BULLET_Create(const Vector2 position, const Vector2 size, const Vector2 speed, const float distance) {
    return (Bullet) {
        .position = position,
        .size = size,
        .speed = speed,
        .distance = distance
        };
}

Rectangle BULLET_GetCollisionRectangle(const Bullet *bullet) {
    return (Rectangle) {
        .x = bullet->position.x,
        .y = bullet->position.y,
        .width = bullet->size.x,
        .height = bullet->size.y
    };
}

void BULLET_MarkDead(Bullet *bullet) {
    bullet->distance = 0;
}

void BULLET_Render(const Bullet *bullet) {
    DrawRectangle((int) bullet->position.x, (int) bullet->position.y, (int) bullet->size.x,
                          (int) bullet->size.y, WHITE);
}

void BULLET_Move(Bullet *bullet) {
    if (bullet->distance == 0) return;
    const float speedX = bullet->speed.x * GetFrameTime();
    const float speedY = bullet->speed.y * GetFrameTime();
    bullet->position.x += speedX;
    bullet->position.y += speedY;
    bullet->distance -= sqrtf(Square(speedX) + Square(speedY));

    if (bullet->distance < 0) {
        BULLET_MarkDead(bullet);
    }
}

void BULLET_WrapAround(Bullet *bullet) {
    WrapXAroundScreen(&bullet->position, bullet->size.x);
    WrapYAroundScreen(&bullet->position, bullet->size.y);
}

Vector2 BULLET_GetPosition(const Bullet *bullet) {
    return bullet->position;
}
Vector2 BULLET_GetSize(const Bullet *bullet) {
    return bullet->size;
}
Vector2 BULLET_GetSpeed(const Bullet *bullet) {
    return bullet->speed;
}

//--------------------------------------------------------------------------------------------------

BulletArray* BULLETS_CreateArray() {
    const size_t arraySize = 1;
    BulletArray *bulletArray = malloc(sizeof(BulletArray));

    if (!bulletArray) {
        printf("ERROR allocating bulletArray pointer\n");
        exit(1);
    }

    bulletArray->bulletArr = malloc(sizeof(Bullet) * arraySize);
    if (!bulletArray->bulletArr) {
        printf("ERROR allocating bullet array\n");
        exit(1);
    }

    bulletArray->capacity = arraySize;
    bulletArray->count = 0;
    return bulletArray;
}

void BULLETS_Shoot(BulletArray *array, const Vector2 position, const Vector2 size, const Vector2 speed , const float distance) {
    BULLETS_Add(array, BULLET_Create(position, size, speed, distance));
}

void BULLETS_Add(BulletArray *array, const Bullet bullet) {
    if (array->capacity < array->count + 1) {
        array->capacity *= 2;
        Bullet *temp = realloc(array->bulletArr, sizeof(Bullet) * array->capacity);
        if (temp == NULL) {
            printf("bullet array Realloc Failed");
            exit(1);
        }
        array->bulletArr = temp;
    }
    array->bulletArr[array->count] = bullet;
    array->count++;
}
void BULLETS_Purge(BulletArray *array) {
    array->count = 0;
}
void BULLETS_Render(const BulletArray *array) {
    for (size_t i = 0; i < array->count; i++) {
        BULLET_Render(&array->bulletArr[i]);
    }
}

void BULLETS_Update(BulletArray *array) {
    size_t i = 0;
    size_t *count = &array->count;
    while (i < *count) {
        Bullet *bullet = &array->bulletArr[i];
        if (!InvalidateDeadBullet(array, bullet, count)) {
            i++;
        }

        BULLET_Move(bullet);
        BULLET_WrapAround(bullet);
    }
}

void BULLETS_Destroy(BulletArray *array, const size_t index) {
    BULLET_MarkDead(&array->bulletArr[index]);
    array->count--;
}

void BULLETS_Spawn(BulletArray *array, const Vector2 position, const Vector2 size, const Vector2 speed, const float distance) {
    BULLETS_Add(array,BULLET_Create(position, size, speed, distance));
}

const Bullet* BULLETS_GetBullet(const BulletArray* array, const size_t index) {
    if (array->count <= index) return NULL;
    return &array->bulletArr[index];
}

void BULLETS_FreeArray(BulletArray *array) {
    if (array == NULL) return;
    free(array->bulletArr);
    free(array);
}

bool InvalidateDeadBullet(const BulletArray *array, Bullet *bullet, size_t *count) {
    if (bullet->distance <= 0) {
        printf("dead bullet invalidated, count: %lu\n", *count);
        (*count)--;
        *bullet = array->bulletArr[*count];
        return true;
    }
    return false;
}