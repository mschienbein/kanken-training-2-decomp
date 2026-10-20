struct ObjectLookupEntryW19G {
    int key;
    void *value;
};

struct ObjectLookupW19G {
    unsigned char reserved00[0x218];
    struct ObjectLookupEntryW19G entries[9];
};

void *func_001f27c0(const struct ObjectLookupW19G *lookup, int key)
{
    int index;
    for (index = 0; index < 9; ++index) {
        if (lookup->entries[index].key == key)
            return lookup->entries[index].value;
    }
    return 0;
}
