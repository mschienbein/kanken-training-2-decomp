#include "game/exp_heap.h"
#include "game/runtime.h"
struct W15fVTable;
struct HeapArena {
    const struct W15fVTable *vtable;
    unsigned int state04, state08, state0c, state10, state14, state18;
    unsigned char reserved1c[0x38];
    unsigned allocations;
    unsigned char *lower;
    unsigned size;
};
struct HeapRange { unsigned char *lower; unsigned size; };
struct HeapListNode {
    void *allocation;
    unsigned int size;
    struct HeapListNode *next;
};
struct HeapListState {
    unsigned int counts[2];
    unsigned char reserved08[16];
    struct HeapListNode lists[2][65];
};
/* INTEGRATOR: differs from src/wave6/func_0019dc20.c: initialized is the byte
 * at +0x634, and +4 embeds the two writable allocation lists. Matches the
 * layout refinement in scratch/authors/w15f/src/func_001e3700.cpp. */
struct HeapManager {
    const struct W15fVTable *vtable;
    struct HeapListState lists;
    unsigned char initialized;
    unsigned char reserved635[0x0b];
    struct HeapArena primary[3];
    unsigned reserved760[2];
    struct HeapRange ranges[3];
    struct HeapArena extra[2];
};
/* INTEGRATOR: move to include/game/static_objects.h */
extern "C" {

extern void func_0019f518(struct HeapListState *lists, void *allocation);
extern void func_0011774c(struct HeapListState *lists, void *allocation);
}

/* [INFERENCE] Unused size/flags retain the caller's allocation interface. */
extern "C" void func_001176e0(struct HeapManager *heap, unsigned int type,
                   unsigned int size, unsigned int flags, void *allocation)
{
    void *pointer = allocation;
    if (!heap->initialized)
        func_0019f568();
    switch (type) {
    case 0x10000: func_00193a6c(&heap->primary[1], pointer); break;
    case 0x20000: func_0019f518(&heap->lists, pointer); break;
    case 0x30000: func_0011774c(&heap->lists, pointer); break;
    default: func_0019f568(); break;
    }
}
