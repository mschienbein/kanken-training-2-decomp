#include "game/event_route.h"
#include "game/archive.h"


const void *func_001f239c(const struct BankInstrumentInfo *block)
{
    if (block->contents.type != 0x101)
        return 0;
    return (const unsigned char *)block + block->contents.offset;
}
