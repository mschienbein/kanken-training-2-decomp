#ifndef GAME_HEAP_MANAGER_H
#define GAME_HEAP_MANAGER_H

#include "game/exp_heap.h"
#include "game/heap_interface.h"

struct HeapRange {
    unsigned char *lower;
    unsigned size;
};

/* The arena/range layout is shared with the original address-dispatching
 * deallocator. The allocation-list prefix remains opaque here. */
struct HeapManager {
    unsigned char reserved00[0x640];
    struct HeapArena primary[3];
    unsigned reserved760[2];
    struct HeapRange ranges[3];
    struct HeapArena extra[2];
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct HeapManager bss_004952f0;
/* [INFERENCE] Descriptive selector names. The provider distinguishes region
 * words 0x10000/0x20000/0x30000 and mode words 0x100..0x105. Its fourth
 * incoming word is unused; retain the caller's full word rather than narrow it. */
void *func_001e35e4(struct HeapManager *manager, unsigned region, unsigned mode,
                   unsigned flags, unsigned size);
void *func_001e37b8(struct Heap *allocator, unsigned size, unsigned alignment);
void func_001e3794(struct Heap *allocator, void *allocation);
#ifdef __cplusplus
}
#endif

#endif
