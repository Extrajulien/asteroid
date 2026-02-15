#pragma once


static inline Rectangle PRESET_NAME_BOX() {
    return (Rectangle) {
        (float)GetScreenWidth()/2 + 10,
        (float)GetScreenHeight()/8*7,
        510,
        100
    };
}

static inline Rectangle VERTICES_NB_BOX() {
    return (Rectangle) {
        (float)GetScreenWidth()/8*7,
        (float)GetScreenHeight()/8+280,
        200,
        50
    };
}

static inline Rectangle SIZE_SELECTION_BOX() {
    return (Rectangle) {
        (float)GetScreenWidth()/2 + 10,
        (float)GetScreenHeight()/8-70,
        250,
        75
    };
}

static inline Rectangle EXIT_BOX() {
    return (Rectangle) {
        (float)GetScreenWidth()-270,
        (float)GetScreenHeight()/8*7,
        250,
        100
    };
}

static inline Rectangle RESET_BOX() {
    return (Rectangle) {
        (float)GetScreenWidth()-270,
        (float)GetScreenHeight()/8*7-120,
        250,
        100
    };
}

static inline Rectangle SAVE_BOX() {
    return (Rectangle) {
        (float)GetScreenWidth()/2+10,
        (float)GetScreenHeight()/8*7-120,
        250,
        100
    };
}

static inline Rectangle SAVE_ALL_BOX() {
    return (Rectangle) {
        (float)GetScreenWidth()/2+10 + 260,
        (float)GetScreenHeight()/8*7-120,
        250,
        100
    };
}

static inline Rectangle GENERATE_BOX() {
    return (Rectangle) {
        (float)GetScreenWidth()-270,
        (float)GetScreenHeight()/8*7-240,
        250,
        100
    };
}

static inline Rectangle RADIUS_BOX() {
    return (Rectangle) {
        ((float)GetScreenWidth() / 8 * 7),
        ((float)GetScreenHeight() / 8 - 70),
        200.0f,
        50.0f
    };
}

static inline Rectangle SPREAD_BOX() {
    return (Rectangle){
        ((float)GetScreenWidth() / 8 * 7),
        ((float)GetScreenHeight() / 8),
        200.0f,
        50.0f
    };
}

static inline Rectangle MIN_ROTATION_BOX() {
    return (Rectangle){
        ((float)GetScreenWidth() / 8 * 7),
        ((float)GetScreenHeight() / 8 + 70),
        200.0f,
        50.0f
    };
}

static inline Rectangle MAX_ROTATION_BOX() {
    return (Rectangle){
        ((float)GetScreenWidth() / 8 * 7),
        ((float)GetScreenHeight() / 8 + 140),
        200.0f,
        50.0f
    };
}

static inline Rectangle MAX_SPEED_BOX() {
    return (Rectangle){
        ((float)GetScreenWidth() / 8 * 7),
        ((float)GetScreenHeight() / 8 + 210),
        200.0f,
        50.0f
    };
}

static inline Rectangle NB_VERTICES_BOX() {
    return (Rectangle){
        ((float)GetScreenWidth() / 8 * 7),
        ((float)GetScreenHeight() / 8 + 280),
        200.0f,
        50.0f
    };
}

static inline Rectangle GENERATION_STYLE_BOX() {
    return (Rectangle){
        ((float)GetScreenWidth() / 8 * 7),
        ((float)GetScreenHeight() / 8 + 350),
        200.0f,
        50.0f
    };
}