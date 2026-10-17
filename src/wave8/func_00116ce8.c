#include "game/region_heap.h"

int func_00116ce8(struct RegionHeapOwner *owner)
{
    int tag;
    struct StreamMutex *lock = &owner->lock;
    func_00192bd8(lock);
    tag = func_00125a5c(&owner->storage);
    func_00195d74(lock);
    return tag;
}
