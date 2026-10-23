#include "game/region_heap.h"

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
