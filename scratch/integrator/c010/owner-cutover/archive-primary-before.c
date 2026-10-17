/* [INFERENCE] Tagged records follow the fixed block header contiguously. */
struct TaggedEntry {
    unsigned short tag;
    unsigned short flags;
    unsigned value;
    unsigned reserved08;
};
struct TaggedBlock {
    unsigned char reserved00[0x10];
    unsigned short count;
    unsigned short reserved12;
    struct TaggedEntry entries[];
};

const struct TaggedEntry *func_001650f8(const struct TaggedBlock *block, unsigned tag)
{
    int index;
    for (index = 0; index < (int)block->count; ++index) {
        if (block->entries[index].tag == tag)
            return &block->entries[index];
    }
    return 0;
}
