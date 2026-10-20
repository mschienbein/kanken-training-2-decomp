#include "game/resource_tables.h"
#include "game/exp_heap.h"

void func_001032e0(int count)
{
    struct DetailEntry *entry;
    int i;
    data_00242264.entries = func_0019d798(2, 4, count * sizeof(struct DetailEntry));
    data_00242264.count = count;
    entry = data_00242264.entries;
    for (i = 0; i < count; ++i, ++entry) {
        entry->id = -1;
        entry->value = 0;
        entry->flags = 0;
        entry->length = 0;
        entry->resource = 0;
    }
}
