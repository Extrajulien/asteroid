#pragma once
#include "components.h"

typedef enum {
    STATE_ALIVE,
    STATE_DEAD,
} EntityState;

typedef struct {
    ComponentsMask mask;
    EntityState state;

} EntityInfo;