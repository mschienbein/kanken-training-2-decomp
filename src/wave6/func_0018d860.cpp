#include "game/functions.h"


struct RegistryOwner {
    const void *vtable;
    PoolList registry;
};


extern "C" void func_0018d860(RegistryOwner *owner, PoolLink *node)
{
    func_0018d868(&owner->registry, node);
}
