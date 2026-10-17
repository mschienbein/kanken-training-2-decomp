#include "game/pool_list.h"

struct RegionBlock { unsigned tag; unsigned char *low; unsigned char *high; struct RegionBlock *previous; };
/* Layout copied exactly from include/game/region_heap.h; its Free return
 * declaration is deliberately not imported because the native FrameHeap API
 * and the final call establish a void operation, not a returned heap pointer. */
struct RegionHeap {
    const void *vtable;
    struct PoolLink registration;
    unsigned char *start;
    unsigned char *end;
    unsigned signature;
    struct RegionHeap *parent;
    struct PoolLink children;
    unsigned options;
    unsigned char *low;
    unsigned char *high;
    struct RegionBlock *blocks;
};
/* INTEGRATOR: differs from src/wave6/func_001467e4.c: the call sites prepare the heap pointer for the return-only hook. */
extern void func_001467e4(struct RegionHeap *heap);
extern void func_0018216c(struct RegionHeap *heap, void *memory, unsigned size);
/* INTEGRATOR: differs from include/game/region_heap.h: canonical nw::ut::FrameHeap::Free(int) is void; observed callers ignore the incidental r0 value. */
void func_00146758(struct RegionHeap *heap, unsigned mode)
{
    /* [INFERENCE] callee identity: inlined by armlink */
    func_001467e4(heap);
    if (mode & 1) {
        func_0018216c(heap, heap->start, heap->low - heap->start);
        heap->low = heap->start;
        heap->blocks = 0;
    }
    if (mode & 2) {
        struct RegionBlock *block;
        func_0018216c(heap, heap->high, heap->end - heap->high);
        for (block = heap->blocks; block != 0; block = block->previous)
            block->high = heap->end;
        { unsigned char **tail = &heap->high; *tail = heap->end; }
    }
    /* [INFERENCE] callee identity: inlined by armlink */
    func_001467e4(heap);
}
