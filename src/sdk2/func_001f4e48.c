#include <stddef.h>
#include "game/object_manager.h"



void func_001f4e48(struct RegistryEntry *entry, unsigned first, unsigned last)
{
    struct Operation *operation = (struct Operation *)((char *)entry - offsetof(struct Operation, registry_entry));
    if (!operation->active)
        return;
    if (first > operation->key_first)
        return;
    if (operation->key_first > last)
        return;
    operation->stopping = 1;
    if (operation->linked) {
        func_0017e634(func_00185d1c(), &operation->manager_entry);
        operation->linked = 0;
    }
    if (operation->running)
        operation->running = 0;
}
