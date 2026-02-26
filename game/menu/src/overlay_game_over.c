#include "overlays.h"
#include "raylib.h"

void openGameOverOverlay(const Overlay *currentScreen, const GameContext *gameContext);
void closeGameOverOverlay(const Overlay *currentScreen, const GameContext *gameContext);
void updateGameOverOverlay(Overlay *currentScreen, const GameContext *gameContext);
void drawGameOverOverlay(const Overlay *currentScreen, const GameContext *gameContext);



OverlayVTable OVERLAY_GetGameOverVTable() {
   return (OverlayVTable) {
      .overlay = OVERLAY_GAME_OVER,
      .open   = openGameOverOverlay,
      .close  = closeGameOverOverlay,
      .update = updateGameOverOverlay,
      .draw   = drawGameOverOverlay
  };
}

// calling this function expect that asteroid array and bullets were already created in screen_game
// it will not render bullets and asteroid if they aren't defined
void openGameOverOverlay(const Overlay *currentScreen, const GameContext *gameContext) {

}

void closeGameOverOverlay(const Overlay *currentScreen, const GameContext *gameContext) {

}

void updateGameOverOverlay(Overlay *currentScreen, const GameContext *gameContext) {

}

void drawGameOverOverlay(const Overlay *currentScreen, const GameContext *gameContext) {
   const int fontSize = 256;
   const char* gameover = "Game Over";
   const Vector2 textSize = MeasureTextEx(GetFontDefault(), gameover, fontSize, 25);
   DrawTextEx(GetFontDefault(), gameover,
       (Vector2){GetScreenWidth()/2-textSize.x/2, GetScreenHeight()/2-textSize.y/2}, fontSize, 25, ORANGE);
}


