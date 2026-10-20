#include "game/heap_interface.h"
#include <new>
#include "game/heap_allocators.h"

/* [INFERENCE] Unused caller-owned configuration retained from the sole caller.
 * The inferred class name binds the observed virtual table. */

HeapAllocatorDcb0 *func_0010dcb0(HeapFactoryConfig8 *config, Heap *heap)
{
    void *storage = heap->allocate(sizeof(HeapAllocatorDcb0), 4);
    if (storage)
        return new (storage) HeapAllocatorDcb0(heap);
    return 0;
}
