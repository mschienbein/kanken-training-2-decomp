struct PrefetchChannelReference {
    unsigned short type;
    unsigned short reserved02;
    int offset;
};
/* INTEGRATOR: differs from include/game/prefetch_stream.h: the reference table
 * has an indexed trailing array of eight-byte channel references after count. */
struct PrefetchReferenceTable {
    unsigned count;
    struct PrefetchChannelReference entries[];
};

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
