#pragma once
#define MAX_OVERLAY_COUNT 16
#include <overlay.h>
#include <stddef.h>

typedef struct OverlayStack {
   Overlay overlays[MAX_OVERLAY_COUNT];
   size_t count;
} OverlayStack;

void OVERLAY_STACK_Push(OverlayStack *stack, Overlay overlay);
void OVERLAY_STACK_Reset(OverlayStack *stack);

void OVERLAY_STACK_Draw(const OverlayStack *stack, const GameContext* context);
void OVERLAY_STACK_Update(OverlayStack *stack, const GameContext* context);
void OVERLAY_STACK_Close(const OverlayStack *stack, const GameContext* context);
void OVERLAY_STACK_Open(const OverlayStack *stack, const GameContext* context);