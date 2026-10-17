#include "game/event_route.h"
#include "game/archive.h"



const struct BinaryByteParameters *func_001f1f80(const struct BinaryReferenceTable *table, unsigned index)
{
    unsigned short type = 0x4101;
    if (index < (unsigned)table->count && table->entries[index].type == type)
        return (const struct BinaryByteParameters *)((const unsigned char *)table + table->entries[index].offset);
    return 0;
}
