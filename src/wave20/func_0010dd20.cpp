#include "game/heap_interface.h"
#include <new>
#include "game/heap_allocators.h"

/* [INFERENCE] Unused caller-owned configuration retained from the sole caller.
 * Class name denotes the observed virtual-table identity. */

HeapAllocatorDd20 *func_0010dd20(HeapFactoryConfig4 *config, Heap *heap)
{
    void *storage = heap->allocate(sizeof(HeapAllocatorDd20), 4);
    if (storage)
        return new (storage) HeapAllocatorDd20(heap);
    return 0;
}
