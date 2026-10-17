#include "game/event_route.h"
#include "game/archive.h"



const struct BinaryReferenceTable *func_001f1f64(const struct BinaryTableBlock *block)
{
    if (block->table.type != 0x101)
        return 0;
    return (const struct BinaryReferenceTable *)((const unsigned char *)block + block->table.offset);
}
