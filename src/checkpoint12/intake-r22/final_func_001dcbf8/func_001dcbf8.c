#include "game/dictionary_variants.h"

/* [INFERENCE] Shared non-inline source helpers, not original symbol identities. */
static void append_index(short index, short *output, int capacity)
{
    int i;
    for (i = 0; i < capacity; ++i) {
        if (output[i] == index)
            break;
        if (output[i] < 0) {
            output[i] = index;
            break;
        }
    }
}

static int index_count(const short *output, int capacity)
{
    int i;
    for (i = 0; i < capacity; ++i) {
        if (output[i] < 0)
            return i;
    }
    return capacity;
}

int func_001dcbf8(const struct W20Dictionary *dictionary, int selected,
                   short *output, int capacity)
{
    const struct W20DictionaryRecord *selected_record;
    const struct W20DictionaryRecord *record;
    int index;
    append_index((short)selected, output, capacity);
    selected_record = &dictionary->records[selected];
    if (!selected_record->group)
        return index_count(output, capacity);
    record = dictionary->records;
    index = 0;
    while (record->textOffset) {
        if (selected_record->group == record->group &&
            selected_record->subgroup == record->subgroup &&
            selected_record->variant != record->variant)
            append_index((short)index, output, capacity);
        ++record;
        ++index;
    }
    return index_count(output, capacity);
}
