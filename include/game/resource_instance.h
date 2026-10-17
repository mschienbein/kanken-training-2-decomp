#ifndef GAME_RESOURCE_INSTANCE_H
#define GAME_RESOURCE_INSTANCE_H

#include "game/heap_interface.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct AllocatorPair;

struct AllocatorPair {
    Heap *primary;
    Heap *secondary;
};

extern "C" {
extern struct AllocatorPair data_00242f84;
void *func_00116cb4(unsigned size, unsigned alignment);
}
#endif

#endif
