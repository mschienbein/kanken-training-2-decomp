#include "game/resource_objects.h"
#include "game/resource_tables.h"

extern "C" void func_0011f080(ResourceTableEntry *entry)
{
    if (entry->active) {
        entry->active = 0;
        entry->flag = 0;
        if (entry->allocation) {
            func_001852f4(func_0013714c(entry->allocation));
            entry->allocation = 0;
        }
        if (entry->object) {
            entry->object->release();
            delete entry->object;
            entry->object = 0;
        }
        --bss_002d3d60.activeCount;
    }
}
