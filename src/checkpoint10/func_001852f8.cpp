#include "game/exp_heap.h"
/* INTEGRATOR: differs from src/wave6/func_0019dc20.c: this 32-byte callee
 * uses only the ManagedExpHeap prefix (heap at 0x18 and allocations at 0x54). */
class HeapLockGuard {
    StreamMutex *mutex;
public:
    explicit HeapLockGuard(StreamMutex *value) : mutex(value) { func_00192bd8(mutex); }
    ~HeapLockGuard() { func_00195d74(mutex); }
};
void func_001852f8(void *allocation)
{
    if (!allocation) return;
    void *block = allocation;
    {
    LockedExpHeap *heap = data_00242c54;
    HeapLockGuard guard(&heap->mutex);
    func_00193a6c(&heap->base, block);
    }
}
