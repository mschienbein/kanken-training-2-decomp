#include <stdint.h>

struct PointerTable {
    unsigned count;
    const void *entries[];
};

struct PointerTableOwner {
    unsigned char reserved00[0x218];
    const struct PointerTable *table;
};

const void *func_001f1864(const struct PointerTableOwner *owner, unsigned index)
{
    const struct PointerTable *table = owner->table;
    uintptr_t offset;
    uintptr_t address;
    if (table == 0)
        return 0;
    if (index >= table->count)
        return 0;
    offset = sizeof(table->count) + index * sizeof(table->entries[0]);
    address = (uintptr_t)table + offset;
    return *(const void *const *)address;
}
