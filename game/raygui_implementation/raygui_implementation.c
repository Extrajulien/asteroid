#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <string.h>
#include "style_amber.h"
#include "raygui_styles.h"

void GUI_LoadStyle(const GuiStyle style) {
    switch (style) {
        case GUI_STYLE_AMBER: GuiLoadStyleAmber(); break;
        default: printf("Unknown GUI style\n"); exit(1);
    }
}