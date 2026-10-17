#include "game/functions.h"


struct RegistryOwner {
    const void *vtable;
    Registry registry;
};


extern "C" void func_0018d860(RegistryOwner *owner, RegistryNode *node)
{
    func_0018d868(&owner->registry, node);
}
