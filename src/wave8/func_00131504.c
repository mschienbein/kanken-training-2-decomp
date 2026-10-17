#include "game/region_heap.h"

void func_00131504(struct HeapStorage *storage)
{
    if (storage->heap) {
        func_001834b8(storage);
        func_00146758(storage->heap, 3);
        func_001834a4(storage->heap);
        storage->heap = 0;
    }
}
