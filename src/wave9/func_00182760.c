#include "game/sound_format.h"

const struct SoundRangeRecord *func_00182760(const struct SoundArchiveInfo *info,
                                           unsigned item_id)
{
    const struct SoundReferenceTable *table;
    unsigned index;
    if ((item_id >> 24) != 2)
        return 0;
    table = (const struct SoundReferenceTable *)
        ((const unsigned char *)info + info->range_table_offset);
    index = item_id & 0x00ffffff;
    if (index >= table->count)
        return 0;
    return (const struct SoundRangeRecord *)
        ((const unsigned char *)table + table->entries[index].offset);
}
