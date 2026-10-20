#include "game/selection.h"

struct MetadataSelection {
    unsigned reserved00;
    int index;
};

struct MetadataResources {
    struct MetadataHeader base;
    unsigned char reserved08[0x20];
    const unsigned char *counts;
};

int func_001672f8(const struct MetadataSelection *selection, int index)
{
    const struct MetadataResources *header;
    if (selection->index < 0)
        return 0;
    header = (const struct MetadataResources *)data_0023fce4[selection->index].header;
    /* ORIGINAL BUG: a negative index is not rejected before the byte-table lookup. */
    if (index < header->base.height)
        return header->counts[index];
    return 0;
}
