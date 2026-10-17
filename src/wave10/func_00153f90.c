struct PrefetchChannelEntry {
    unsigned start;
    unsigned size;
    unsigned reserved08[2];
    int data_offset;
};

const void *func_00153f90(const struct PrefetchChannelEntry *entry)
{
    return (const char *)entry + entry->data_offset;
}
