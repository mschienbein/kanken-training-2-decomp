#include <preinclude.h>
#include "game/object_manager.h"


extern "C" void func_0014bc70(ObjectManager *manager, RegistryEntry *entry)
{
    StreamLockGuard guard(&manager->lock);
    manager->registry.push_back(entry);
}
