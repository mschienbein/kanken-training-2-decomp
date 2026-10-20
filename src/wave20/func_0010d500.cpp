#include "game/heap_interface.h"
#include <new>
#include "game/heap_allocators.h"

/* [INFERENCE] Class name denotes the observed virtual-table identity. */

HeapAllocatorD500 *func_0010d500(Heap *heap)
{
    void *storage = heap->allocate(sizeof(HeapAllocatorD500), 4);
    if (storage)
        return new (storage) HeapAllocatorD500(heap);
    return 0;
}
