#define RAYGUI_IMPLEMENTATION
#include "logger.h"
#include "raygui.h"
#include "style_amber.h"
#include "raygui_styles.h"

void GUI_LoadStyle(const GuiStyle style) {
    switch (style) {
        case GUI_STYLE_AMBER: GuiLoadStyleAmber(); break;
        default: LOG("$Y[Style]$y Unknown GUI style\n"); exit(1);
    }
}
