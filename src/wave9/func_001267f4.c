struct AttributeMapping {
    const char *name;
    const char *value;
};

struct AttributeRecord {
    unsigned char reserved00[0x18];
    const struct AttributeMapping *entries;
    unsigned entry_count;
    unsigned reserved20;
    const unsigned short *children;
    unsigned child_count;
    unsigned reserved2c;
};

struct AttributeCursor {
    const struct AttributeRecord *record;
};

unsigned func_001267f4(const struct AttributeCursor *cursor)
{
    return cursor->record->child_count;
}
