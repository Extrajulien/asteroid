#pragma once

typedef enum GuiStyle {
    GUI_STYLE_AMBER,
    GUI_STYLE_COUNT
} GuiStyle;

void GUI_LoadStyle(GuiStyle style);