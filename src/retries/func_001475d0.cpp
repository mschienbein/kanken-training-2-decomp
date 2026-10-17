#include "game/region_heap.h"
#include <stdint.h>



inline void HeapStorage::clear()
{
    if (heap) {
        func_001834b8(this);
        func_00146758(heap, 3);
        func_001834a4(heap);
        heap = 0;
    }
}

int HeapStorage::initialize(void *memory, unsigned size)
{
    uintptr_t begin;
    uintptr_t end;
    struct MemoryRegion { void *memory; unsigned size; } region = { memory, size };
    clear();
    begin = ((uintptr_t)region.memory + 3) & ~3u;
    end = (uintptr_t)region.memory + region.size;
    if (begin > end)
        return 0;
    heap = func_00148b08((void *)begin, end - begin, 0);
    if (!heap)
        return 0;
    return func_001856d8(this) != 0;
}
