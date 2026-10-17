#include "game/resource_tables.h"

extern struct DetailEntry *func_001851f0(int id, int index);

void func_0019d95c(uintptr_t resource, int index, int force)
{
    struct DetailEntry *entry;
    if (resource < 100)
        entry = func_001851f0(resource, index);
    else
        entry = (struct DetailEntry *)resource;
    if (entry) {
        if (!force && entry->length) {
            if (--entry->length)
                return;
        }
        if (entry->id != -1) {
            if (entry->resource) {
                func_0019cf20(entry->resource);
                entry->resource = 0;
            }
            entry->value = 0;
            entry->id = -1;
            entry->flags = 0;
            entry->length = 0;
        }
    }
}
