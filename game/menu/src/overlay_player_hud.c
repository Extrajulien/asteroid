#include <player.h>
#include <stdio.h>

#include "game_api.h"
#include "overlays.h"
#include "raylib.h"

void openPlayerHudOverlay(const Overlay *currentScreen, const GameContext *gameContext);
void closePlayerHudOverlay(const Overlay *currentScreen, const GameContext *gameContext);
void updatePlayerHudOverlay(Overlay *currentScreen, const GameContext *gameContext);
void drawPlayerHudOverlay(const Overlay *currentScreen, const GameContext *gameContext);



OverlayVTable OVERLAY_GetPlayerHudVTable() {
   return (OverlayVTable) {
      .overlay = OVERLAY_GAME_OVER,
      .open   = openPlayerHudOverlay,
      .close  = closePlayerHudOverlay,
      .update = updatePlayerHudOverlay,
      .draw   = drawPlayerHudOverlay
  };
}

void openPlayerHudOverlay(const Overlay *currentScreen, const GameContext *gameContext) {

}

void closePlayerHudOverlay(const Overlay *currentScreen, const GameContext *gameContext) {

}

void updatePlayerHudOverlay(Overlay *currentScreen, const GameContext *gameContext) {

}

void drawPlayerHudOverlay(const Overlay *currentScreen, const GameContext *gameContext) {
   char scoreText[100] = "";
   sprintf(scoreText,"%d", PLAYER_GetScore(gameContext->player));
   DrawText(scoreText, 10, 50, 32, WHITE);
   char livesText[100] = "";
   sprintf(livesText,"pv:%d", PLAYER_GetLifeCount(gameContext->player));
   DrawText(livesText, 10, 10, 40, ORANGE);

   char asteroidCountText[100] = "";
   sprintf(asteroidCountText,"%llu", gameContext->asteroidArray->nbAsteroid);
   DrawText(asteroidCountText, 10, 100, 32, RED);
}