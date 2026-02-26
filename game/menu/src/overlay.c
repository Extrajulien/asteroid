#include "overlay.h"

#include <overlays.h>

void OVERLAY_Reset(Overlay* overlay) {
   overlay->overlay = OVERLAY_NONE;
}

Overlay OVERLAY_Get(const Overlay_T type) {
   switch (type) {
      case OVERLAY_GAME_OVER:
         return (Overlay) {
            .overlay = OVERLAY_GAME_OVER,
            .vTable = OVERLAY_GetGameOverVTable()
         };
      case OVERLAY_PLAYER_HUD:
         return (Overlay) {
            .overlay = OVERLAY_PLAYER_HUD,
            .vTable = OVERLAY_GetPlayerHudVTable()
         };

      default: return (Overlay) {.overlay = OVERLAY_NONE};

   }
}

void OVERLAY_Open(const Overlay* overlay, const GameContext* context) {
   overlay->vTable.open(overlay, context);
}