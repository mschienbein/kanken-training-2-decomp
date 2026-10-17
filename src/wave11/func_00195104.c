#include "game/exp_heap.h"
static __forceinline void *allocate_configured(struct ManagedExpHeap *heap,
                                              int alignment, unsigned size,
                                              int group, int direction, int mode)
{
    func_0012ee34(&heap->heap, group);
    func_0012ee44(&heap->heap, direction);
    func_0012ef20(&heap->heap, mode);
    return func_0012ec8c(&heap->heap, size, alignment);
}

void *func_00195104(struct ManagedExpHeap *heap, unsigned size, int alignment,
                    int group, int direction, int mode)
{
    void *allocation =
        allocate_configured(heap, alignment, size, group, direction, mode);
    if (allocation)
        ++heap->allocations;
    return allocation;
}
