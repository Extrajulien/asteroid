#pragma once
typedef enum Overlay_T {
   OVERLAY_GAME_OVER,
   OVERLAY_PLAYER_HUD,
   OVERLAY_NONE
 } Overlay_T;

typedef struct Overlay Overlay;
typedef struct GameContext GameContext;

typedef struct OverlayVTable {
   Overlay_T overlay;
   void (* open) (const Overlay *current, const GameContext *context);
   void (* close) (const Overlay *current, const GameContext *context);
   void (* update) (Overlay *current, const GameContext *context);
   void (* draw) (const Overlay *current, const GameContext *context);
} OverlayVTable;

// the overlay appends to a screen and is owned by the screen


typedef struct GameOverCtx {

} GameOverCtx;

typedef struct PlayerHudCtx {

} PlayerHudCtx;

typedef struct Overlay {
   Overlay_T overlay;
   OverlayVTable vTable;
   union {
      GameOverCtx gameOverCtx;
      PlayerHudCtx playerHudCtx;
   };
} Overlay;

void OVERLAY_Reset(Overlay* overlay);

Overlay OVERLAY_Get(Overlay_T type);

void OVERLAY_Open(const Overlay* overlay, const GameContext* context);