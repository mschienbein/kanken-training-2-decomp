#ifndef GAME_RESOURCE_INSTANCE_H
#define GAME_RESOURCE_INSTANCE_H

#include "game/heap_interface.h"

/* Both allocation and release consumers select this same pair of heap owners. */
struct AllocatorPair {
    struct Heap *primary;
    struct Heap *secondary;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct AllocatorPair data_00242f84;
void *func_00116cb4(unsigned size, unsigned alignment);
#ifdef __cplusplus
}
#endif

#endif
