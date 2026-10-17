#include "game/runtime.h"
#include "game/pool_list.h"

struct PoolOwnerVTable;
struct PoolOwnedResource;
struct PoolOwner {
    const PoolOwnerVTable *vtable;
    PoolList children;
    PoolOwnedResource *resource;
    PoolList *extra;
};
struct OwnedBuffer : PoolOwner {
    unsigned char reserved18[0x0c];
    void *allocation;
    unsigned char owns_allocation;
};
/* INTEGRATOR: move to include/game/pool_owned_list.h */
extern "C" {
extern const PoolOwnerVTable data_00224e44;
PoolOwner *func_001cecb8(PoolOwner *owner);
void func_001e87c8(OwnedBuffer *owner);
}

void func_001e87c8(OwnedBuffer *owner)
{
    unsigned char owned = owner->owns_allocation;
    owner->vtable = &data_00224e44;
    if (owned) {
        owner->owns_allocation = 0;
        owner->resource = 0;
        func_0019dc20(owner->allocation);
    }
    func_001852f4(func_001cecb8(owner));
}
