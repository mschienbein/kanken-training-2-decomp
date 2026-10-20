#include <preinclude.h>
#include "game/object_manager.h"


extern "C" void func_001312d4(ObjectManager *manager, RegistryEntry *entry)
{
    StreamLockGuard guard(&manager->lock);
    nw::ut::LinkList<RegistryEntry, offsetof(RegistryEntry, link)> &list = manager->registry;
    list.erase(entry);
}
