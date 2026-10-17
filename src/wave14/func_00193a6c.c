#include "game/exp_heap.h"
void func_00193a6c(struct ManagedExpHeap *heap, void *allocation)
{
    func_00121a88(&heap->heap, allocation);
    --heap->allocations;
}
