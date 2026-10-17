#include <string.h>
#include "game/sound_format.h"
static const struct SequenceLabelRecord *get_record(const struct SequenceLabelTable *table,
                                                   unsigned index)
{
    return (unsigned)table->count > index ?
        (const struct SequenceLabelRecord *)((const char *)table + table->entries[index].offset) : 0;
}

int func_001f240c(const struct SequenceLabelTable *table, const char *name, unsigned *value)
{
    unsigned length = strlen(name);
    int i;
    for (i = 0; table->count > i; ++i) {
        const struct SequenceLabelRecord *record = get_record(table, i);
        /* ORIGINAL BUG: a matching prefix is accepted without checking the record-name length. */
        if (strncmp(name, record->name, length) == 0) {
            *value = record->value;
            return 1;
        }
    }
    return 0;
}
