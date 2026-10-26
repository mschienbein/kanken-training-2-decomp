#include "game/framebuffer.h"

static unsigned clear_and_advance(struct FramebufferState *collection, unsigned index)
{
    collection->entries[index].object = 0;
    collection->entries[index].first = 0;
    collection->entries[index].second = 0;
    collection->entries[index].third = 0;
    return index + 1;
}
void func_0010b598(struct FramebufferState *collection)
{
    unsigned index = 0;
    collection->active = 0;
    collection->mode = 0;
    while (index < 4)
        index = clear_and_advance(collection, index);
}
