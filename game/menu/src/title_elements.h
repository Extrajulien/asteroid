#pragma once
#define BOX_BORDER_SIZE 10.0f
#define FONT_SIZE 20
#define FONT_SPACING 1

static inline Rectangle getBorderBox(const Rectangle box) {
    return (Rectangle) {
        box.x - BOX_BORDER_SIZE / 2, box.y - BOX_BORDER_SIZE / 2, box.width + BOX_BORDER_SIZE,
    box.height + BOX_BORDER_SIZE
    };
}

static inline Rectangle START_BOX() {
    return (Rectangle) {((float) GetScreenWidth()-350) / 2, (float) GetScreenHeight() / 2, 350, 100};
}

static inline Rectangle MAKE_PRESET_BOX() {
    return (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 150, 350, 100};
}
static inline Rectangle EDIT_PRESET_BOX() {
    return (Rectangle){(GetScreenWidth()-350) / 2, GetScreenHeight() / 2 + 300, 350, 100};
}

static inline void DRAW_BOX(const char* text, const Rectangle box) {
    const Color accentColor = CheckCollisionPointRec(GetMousePosition(), box) ? ORANGE : WHITE;
    const Vector2 textSize = MeasureTextEx(GetFontDefault(), text, FONT_SIZE, FONT_SPACING);
    DrawRectangleRec(getBorderBox(box), accentColor);
    DrawRectangleRec(box, BLACK);
    DrawText(text, box.x + (box.width) / 2 - textSize.x / 2, box.y + box.height / 2 - textSize.y / 2, 20, accentColor);
}

static inline void DRAW_START_BOX() {
    DRAW_BOX("Start", START_BOX());
}

static inline void DRAW_MAKE_PRESET_BOX() {
    DRAW_BOX("Edit Mode", MAKE_PRESET_BOX());
}

static inline void DRAW_EDIT_PRESET_BOX() {
    DRAW_BOX("Edit Presets", EDIT_PRESET_BOX());
}

