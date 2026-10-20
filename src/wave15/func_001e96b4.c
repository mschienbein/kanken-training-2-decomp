struct FlaggedEntry {
    unsigned int flags;
};
struct FlaggedEntryArray {
    unsigned char reserved00[0x20];
    struct FlaggedEntry *entries[];
};

void func_001e96b4(struct FlaggedEntryArray *array, int index)
{
    struct FlaggedEntry *entry = array->entries[index];
    if (entry)
        entry->flags |= 0x40;
}
