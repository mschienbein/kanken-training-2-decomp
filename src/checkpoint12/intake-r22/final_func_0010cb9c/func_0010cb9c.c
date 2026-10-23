#include "game/serialized_packet_resource.h"

static unsigned packet_size(unsigned count)
{
    unsigned words = (count >> 7) * 132;
    if ((count & 127u) != 0)
        words += ((count & 127u) + 4u) & ~1u;
    return words;
}


struct PacketExtent { unsigned words; };
static struct PacketExtent add_packet(struct PacketExtent extent, unsigned count)
{
    extent.words += packet_size(count);
    return extent;
}
/* The loader supplies an input allocation size which this query ignores. */
unsigned func_0010cb9c(const struct SerializedPacketResource *resource, unsigned input_size)
{
    const unsigned *table = &resource->words[resource->section_word_offset];
    struct PacketExtent extent = {0};
    extent = add_packet(extent, table[3]);
    extent = add_packet(extent, table[5]);
    return 3512u + extent.words * sizeof(unsigned);
}

