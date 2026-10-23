#include <string.h>

struct AttributeMapping { const char *name; const char *value; };
struct AttributeRecord {
    unsigned char reserved00[0x18];
    const struct AttributeMapping *entries;
    unsigned entry_count;
    unsigned reserved20;
    const unsigned short *children;
    unsigned child_count;
    unsigned reserved2c;
};
struct AttributeCursor { const struct AttributeRecord *record; };

static const struct AttributeRecord *attribute_record(const struct AttributeCursor *cursor)
{
    return cursor->record;
}

static unsigned attribute_count(const struct AttributeRecord *record)
{
    return record->entry_count;
}

static const struct AttributeMapping *attribute_entry(const struct AttributeRecord *record,
                                                      unsigned index)
{
    return record->entries + index;
}

struct AttributeIterator {
    const struct AttributeRecord *record;
    unsigned index;
    unsigned count;
};

static struct AttributeIterator attribute_begin(const struct AttributeCursor *cursor)
{
    struct AttributeIterator result;
    result.record = attribute_record(cursor);
    result.index = 0;
    result.count = attribute_count(result.record);
    return result;
}

const char *func_0018fdfc(const struct AttributeCursor *cursor, const char *name)
{
    struct AttributeIterator iterator = attribute_begin(cursor);
    for (; iterator.count > iterator.index; ++iterator.index)
        if (strncmp(attribute_entry(iterator.record, iterator.index)->name, name, 64) == 0)
            return attribute_entry(iterator.record, iterator.index)->value;
    return 0;
}

const char *func_0018fed0(const struct AttributeCursor *cursor, const char *name)
{
    struct AttributeIterator iterator = attribute_begin(cursor);
    for (; iterator.count > iterator.index; ++iterator.index)
        if (strncmp(attribute_entry(iterator.record, iterator.index)->name, name, 64) == 0)
            return attribute_entry(iterator.record, iterator.index)->value;
    return 0;
}

