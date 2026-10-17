#include "game/resource_tables.h"
#include "game/exp_heap.h"

void func_001033cc(int count)
{
    struct Entry *entry;
    int i;
    data_0024225c.entries = func_0019d798(2, 4, count * sizeof(struct Entry));
    data_0024225c.count = count;
    entry = data_0024225c.entries;
    for (i = 0; i < count; ++i, ++entry) {
        entry->id = -1;
        entry->resource = 0;
    }
}
