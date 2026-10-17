#include "game/region_heap.h"

inline HeapStorage::~HeapStorage()
{
    if (heap) {
        func_001834b8(this);
        func_00146758(heap, 3);
        func_001834a4(heap);
        heap = 0;
    }
}

extern "C" HeapStorage *func_001d99cc(HeapStorage *storage)
{
    storage->~HeapStorage();
    return storage;
}
