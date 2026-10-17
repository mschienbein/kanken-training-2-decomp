#include "game/archive.h"

const struct SoundFileInfo *func_0014c25c(const struct SoundArchiveInfo *info, unsigned id)
{
    const struct SoundInfoTable *table;
    if ((id >> 24) != 4)
        return 0;
    id &= 0x00ffffff;
    table = (const struct SoundInfoTable *)((const unsigned char *)info + info->file_table_offset);
    if (id >= table->count)
        return 0;
    return (const struct SoundFileInfo *)((const unsigned char *)table + table->entries[id].offset);
}
