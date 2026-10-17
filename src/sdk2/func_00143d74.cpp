#include "game/region_heap.h"
extern "C" int func_00143d74(struct RegionHeapOwner *owner, void *memory, unsigned size)
{
    int result;
    StreamMutex *lock = &owner->lock;
    func_00192bd8(lock);
    result = owner->storage.initialize(memory, size);
    func_00195d74(lock);
    return result;
}
