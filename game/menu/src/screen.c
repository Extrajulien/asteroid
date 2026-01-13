#include "screen.h"
#include "screens.h"

ScreenVTable SCREEN_GetVTable(const Screen screen) {
    switch (screen) {
        case SCREEN_TITLE: return SCREENS_GetTitleVTable();
        case SCREEN_PRESET_CREATE: return SCREENS_GetPresetCreateVTable();
        case SCREEN_GAME: return SCREENS_GetGameVTable();
    }
}
