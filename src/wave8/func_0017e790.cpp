#include "game/functions.h"

extern "C" struct RegistryNode *func_0017e790(struct Registry *registry, struct RegistryEntry *entry)
{
    return func_00191154(registry, RegistryIterator(&registry->sentinel), &entry->link);
}
