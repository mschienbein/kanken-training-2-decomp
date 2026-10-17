#include "game/event_route.h"
#include "game/archive.h"
#include "game/item_text.h"


const void *func_001f22c4(const struct W12cSoundDetail *block)
{
    if (block->contents.type != 0x101)
        return 0;
    return (const unsigned char *)block + block->contents.offset;
}
