#include "game/archive_resource.h"

const struct ArchiveResourceReference *func_00144114(const struct SoundArchiveInfo *info,
                                                    unsigned resource_id)
{
    const struct ArchiveReferenceTable *table = (const struct ArchiveReferenceTable *)
        ((const unsigned char *)info + info->location_table_offset);
    unsigned index = resource_id & 0x00ffffff;
    if (index >= table->count)
        return 0;
    return (const struct ArchiveResourceReference *)
        ((const unsigned char *)table + table->entries[index].offset);
}
