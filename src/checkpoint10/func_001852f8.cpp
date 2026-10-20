#include "game/exp_heap.h"
void func_001852f8(void *allocation)
{
    if (!allocation) return;
    void *block = allocation;
    {
    LockedExpHeap *heap = data_00242c54;
    StreamLockGuard guard(&heap->mutex);
    func_00193a6c(&heap->base, block);
    }
}
