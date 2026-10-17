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

struct AttributeTable {
    const struct AttributeRecord *records;
    unsigned record_count;
};

struct AttributeOwner {
    const void *storage;
    const struct AttributeTable *table;
};

void func_001267c0(struct AttributeCursor *cursor,
                   const struct AttributeOwner *owner, unsigned index)
{
    const struct AttributeTable *table = owner->table;
    /* ORIGINAL BUG: the inclusive bound accepts the one-past record index. */
    cursor->record = table && index <= table->record_count ? &table->records[index] : 0;
}
