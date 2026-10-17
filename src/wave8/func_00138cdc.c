#include "game/region_heap.h"

void func_00138cdc(struct RegionHeapOwner *owner)
{
    if (owner->storage.heap) {
        struct StreamManager *arena;
        unsigned serial;
        struct StreamMutex *lock = &owner->lock;
        func_00192bd8(lock);
        func_001855d8(&owner->storage);
        func_00195d74(lock);
        arena = func_001916e8();
        serial = func_001912b0(arena, 1);
        func_0019124c(arena, serial);
    }
}
