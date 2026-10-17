#include "game/resource_tables.h"

/* Keep the observed out-of-line calls while compiling these leaf queries together. */
__declspec(noinline) unsigned func_0018269c(int id, int index)
{
    struct Entry *entry = data_0024225c.entries;
    int i;
    for (i = 0; i < data_0024225c.count; ++i, ++entry) {
        if (entry->id == id) {
            if (entry)
                return ((const unsigned *)entry->resource->allocation)[index];
            break;
        }
    }
    return 0;
}

__declspec(noinline) struct DetailEntry *func_001851f0(int id, int index)
{
    struct DetailEntry *entry = data_00242264.entries;
    int i;
    if (id == -1) {
        for (i = 0; i < data_00242264.count; ++i, ++entry) {
            if (entry->id == -1)
                return entry;
        }
    } else {
        for (i = 0; i < data_00242264.count; ++i, ++entry) {
            if (entry->id == id && entry->value <= index && entry->flags >= index)
                return entry;
        }
    }
    return 0;
}

unsigned char *func_001971a0(uintptr_t resource, int index)
{
    struct DetailEntry *entry;
    unsigned start, offset;
    if (resource >= 100)
        return (unsigned char *)resource + ((const unsigned *)resource)[index];
    if (resource) {
        entry = func_001851f0(resource, index);
        if (!entry)
            return 0;
        start = func_0018269c(resource, entry->value);
        offset = func_0018269c(resource, index);
        return (unsigned char *)entry->resource->allocation +
               (offset - start);
    }
    return (unsigned char *)data_0023fd80 + data_0023fd80[index];
}
