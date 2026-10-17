#include "game/region_heap.h"

void func_00138d30(struct RegionHeapOwner *owner, int count)
{
    struct StreamManager *arena;
    unsigned serial;
    struct StreamMutex *lock = &owner->lock;
    func_00192bd8(lock);
    func_001855fc(&owner->storage, count);
    arena = func_001916e8();
    serial = func_001912b0(arena, 1);
    func_0019124c(arena, serial);
    func_00195d74(lock);
}
