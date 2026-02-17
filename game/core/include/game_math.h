#pragma once
#include <math.h>

#include "components.h"

static float Square(const float x) {
    return x * x;
}

static void WrapXAroundScreen(Position* pos, const float radius) {
    const float screenWidth = (float) GetScreenWidth();
    if (pos->x > screenWidth + radius) pos->x = -radius;
    if (pos->x < -radius) pos->x = screenWidth + radius;
}

static void WrapYAroundScreen(Position* pos, const float radius) {
    const float screenHeight = (float) GetScreenHeight();
    if (pos->y > screenHeight + radius) pos->y = -radius;
    if (pos->y < -radius) pos->y = screenHeight + radius;
}

static void WrapAroundScreen(Position* pos, const float radius) {
    WrapXAroundScreen(pos, radius);
    WrapYAroundScreen(pos, radius);
}

static void WrapAroundRadAngle(Rotation *rotation) {
    if (rotation->rad >= 2 * PI) rotation->rad = 0;
    if (rotation->rad < 0) rotation->rad = 2 * PI;
}

//reverse y
static float AngleFromComponent(const Vector2 component) {
    return atan2f(-component.y, component.x);
}

static float flipRadAngle(const float rad) {
    float angle = rad + (float) M_PI;
    if (angle < 0) angle += 2 * (float) M_PI;
    if (angle > 2 * M_PI) angle -= 2 * (float) M_PI;
    return angle;
}