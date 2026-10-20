#include "game/heap_interface.h"
#include <new>
#include "game/heap_allocators.h"

/* [INFERENCE] Class name denotes the observed virtual-table identity. */

HeapAllocatorDce8 *func_0010dce8(Heap *heap)
{
    void *storage = heap->allocate(sizeof(HeapAllocatorDce8), 4);
    if (storage)
        return new (storage) HeapAllocatorDce8(heap);
    return 0;
}
