struct ResetEntry {
    unsigned values[4];
};
struct ResetEntryCollection {
    unsigned char reserved00[8];
    unsigned char selected;
    unsigned char reserved09[3];
    int count;
    unsigned char reserved10[8];
    struct ResetEntry **entries;
    unsigned char *flags;
};

void func_0015f7e4(struct ResetEntryCollection *collection)
{
    int i;
    collection->selected = 255;
    for (i = 0; i < collection->count; ++i) {
        struct ResetEntry *entry = collection->entries[i];
        entry->values[0] = 0;
        entry->values[1] = 0;
        entry->values[2] = 0;
        entry->values[3] = 0;
        collection->flags[i] = 0;
    }
}
