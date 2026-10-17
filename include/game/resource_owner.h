#ifndef GAME_RESOURCE_OWNER_H
#define GAME_RESOURCE_OWNER_H

struct ResourceOwner;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ResourceOwner {
    const void *vtable;
    unsigned resource_bits;
};

#endif
