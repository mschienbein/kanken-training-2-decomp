#include "game/prefetch_stream.h"

const struct PrefetchChannelReference *func_00154804(const struct PrefetchReferenceTable *table,
                                                    unsigned index)
{
    unsigned channel_type = 0x4102;
    if (index >= table->count)
        return 0;
    if (table->entries[index].type != channel_type)
        return 0;
    return (const struct PrefetchChannelReference *)
        ((const unsigned char *)table + table->entries[index].offset);
}
