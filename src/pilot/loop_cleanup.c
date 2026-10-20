#include "game/runtime.h"
#include "game/resource_tables.h"


void func_00103358(void)
{
    struct Entry *entry = data_0024225c.entries;
    int i;
    if (entry) {
        for (i = 0; i < data_0024225c.count; ++i, ++entry) {
            if (entry->id != -1) {
                func_0019cf20(entry->resource);
                entry->resource = 0;
                entry->id = -1;
            }
        }
        func_0019dc20(data_0024225c.entries);
        data_0024225c.entries = 0;
        data_0024225c.count = 0;
    }
}

void func_00103258(void)
{
    struct DetailEntry *entry = data_00242264.entries;
    int i;
    if (entry) {
        for (i = 0; i < data_00242264.count; ++i, ++entry) {
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
        func_0019dc20(data_00242264.entries);
        data_00242264.entries = 0;
        data_00242264.count = 0;
    }
}
