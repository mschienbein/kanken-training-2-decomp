#include "game/heap_interface.h"
#include <new>
#include "game/heap_allocators.h"

/* [INFERENCE] Class name denotes the observed virtual-table identity. */

HeapAllocator6be4 *func_00116be4(Heap *heap)
{
    void *storage = heap->allocate(sizeof(HeapAllocator6be4), 4);
    if (storage)
        return new (storage) HeapAllocator6be4(heap);
    return 0;
}
