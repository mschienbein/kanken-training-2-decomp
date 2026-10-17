#include "game/event_route.h"
#include "game/archive.h"


const void *func_001f22c4(const struct BinaryReferenceBlock *block)
{
    if (block->contents.type != 0x101)
        return 0;
    return (const unsigned char *)block + block->contents.offset;
}
