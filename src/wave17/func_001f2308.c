#include "game/archive.h"

struct ArchiveBlockReferenceTable {
    unsigned count;
    struct ArchiveBlockReference entries[];
};

const void *func_001f2308(const struct RelativeReference *reference, int index)
{
    const struct ArchiveBlockReferenceTable *table;
    if (index == -1)
        return 0;
    table = (const struct ArchiveBlockReferenceTable *)
        ((const unsigned char *)reference + reference->offset);
    if (table == 0)
        return 0;
    return (const unsigned char *)table + table->entries[index].offset;
}
