#include "overlay_stack.h"

void OVERLAY_STACK_Push(OverlayStack *stack, const Overlay overlay) {
   if (stack->count < MAX_OVERLAY_COUNT+1) {
      stack->overlays[stack->count] = overlay;
      stack->count++;
   }
}

void OVERLAY_STACK_Reset(OverlayStack *stack) {
   stack->count = 0;
}

void OVERLAY_STACK_Draw(const OverlayStack *stack, const GameContext* context) {
   for (int i = 0; i < stack->count; ++i) {
      stack->overlays[i].vTable.draw(&stack->overlays[i], context);
   }
}

void OVERLAY_STACK_Update(OverlayStack *stack, const GameContext* context) {
   for (int i = 0; i < stack->count; ++i) {
      stack->overlays[i].vTable.update(&stack->overlays[i], context);
   }
}

void OVERLAY_STACK_Close(const OverlayStack *stack, const GameContext* context) {
   for (int i = 0; i < stack->count; ++i) {
      stack->overlays[i].vTable.close(&stack->overlays[i], context);
   }
}

void OVERLAY_STACK_Open(const OverlayStack *stack, const GameContext* context) {
   for (int i = 0; i < stack->count; ++i) {
      stack->overlays[i].vTable.open(&stack->overlays[i], context);
   }
}