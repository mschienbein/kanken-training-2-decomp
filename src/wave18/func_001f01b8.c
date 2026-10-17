struct ObjectLookupEntry {
    unsigned int key;
    void *value;
};

struct ObjectLookup {
    unsigned char reserved00[0x160];
    const struct ObjectLookupEntry *entries;
    void *default_value;
    unsigned char count;
};

void *func_001f01b8(const struct ObjectLookup *object, unsigned char index)
{
    if (index == 0)
        return object->default_value;
    --index;
    if (index >= object->count)
        return 0;
    return object->entries[index].value;
}
