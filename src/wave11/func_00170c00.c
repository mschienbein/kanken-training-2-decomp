#include "game/messages.h"
const unsigned short *func_00170c00(const struct W11TextResource *resource, int index)
{
    const struct W11TextEntry *entries = resource->entries;
    if (entries != 0) {
        int offset = entries[index].text_offset;
        if (offset == 0)
            return 0;
        return (const unsigned short *)((const unsigned char *)entries + offset);
    }
    return 0;
}
