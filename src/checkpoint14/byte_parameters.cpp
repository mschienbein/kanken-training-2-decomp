#include <preinclude.h>
#include <nw/snd/snd_Util.h>
#include "game/event_route.h"

/* [INFERENCE] Historical helper/TU identity is unrecovered; this operation
 * expresses the actual bounded copy and retains each serialized-offset read. */
static void copy_byte_values(struct ByteParameterValue *output, const struct BinaryByteParameters *entry, unsigned count)
{
    int i;
    count = count > 2 ? 2 : count;
    for (i = 0; i < count; ++i) {
        const nw::snd::internal::Util::Table<unsigned char> *values = static_cast<const nw::snd::internal::Util::Table<unsigned char> *>(nw::ut::AddOffsetToPtr(entry, entry->values_offset));
        output->values[i] = values->item[i];
    }
}

int func_001f29b4(const struct BinaryParameterReader *reader, struct ByteParameterValue *output, int index)
{
    const struct BinaryReferenceTable *table = func_001f1f64(reader->block);
    const struct BinaryByteParameters *entry;
    const nw::snd::internal::Util::Table<unsigned char> *list;
    if (!table)
        return 0;
    if (index >= table->count)
        return 0;
    entry = func_001f1f80(table, index);
    /* ORIGINAL BUG: a failed typed-reference lookup is dereferenced without a null check. */
    output->channels[0] = entry->channels[0];
    output->channels[1] = entry->channels[1];
    output->channels[2] = entry->channels[2];
    output->channels[3] = entry->channels[3];
    list = static_cast<const nw::snd::internal::Util::Table<unsigned char> *>(nw::ut::AddOffsetToPtr(entry, entry->values_offset));
    output->count = list->count;
    copy_byte_values(output, entry, output->count);
    return 1;
}
