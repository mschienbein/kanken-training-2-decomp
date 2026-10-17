#include <string.h>

struct RelativeNameEntry {
    unsigned reserved00;
    unsigned offset;
};

struct RelativeNameTable {
    int count;
    struct RelativeNameEntry entries[];
};

struct RelativeNameRecord {
    unsigned reserved00;
    unsigned value;
    unsigned reserved08;
    char name[];
};

static const struct RelativeNameRecord *get_record(const struct RelativeNameTable *table,
                                                   unsigned index)
{
    return (unsigned)table->count > index ?
        (const struct RelativeNameRecord *)((const char *)table + table->entries[index].offset) : 0;
}

int func_001f240c(const struct RelativeNameTable *table, const char *name, unsigned *value)
{
    unsigned length = strlen(name);
    int i;
    for (i = 0; table->count > i; ++i) {
        const struct RelativeNameRecord *record = get_record(table, i);
        /* ORIGINAL BUG: a matching prefix is accepted without checking the record-name length. */
        if (strncmp(name, record->name, length) == 0) {
            *value = record->value;
            return 1;
        }
    }
    return 0;
}
