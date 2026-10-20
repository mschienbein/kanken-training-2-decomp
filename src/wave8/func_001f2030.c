#include "game/archive_resource.h"
struct ArchiveStreamInfo {
    unsigned file_id;
};

const struct ArchiveStreamInfo *func_001f2030(const struct SoundArchiveInfo *info,
                                            unsigned resource_id)
{
    const struct ArchiveReferenceTable *table;
    unsigned index;
    if (resource_id >> 24 != 6)
        return 0;
    table = (const struct ArchiveReferenceTable *)
        ((const unsigned char *)info + info->stream_table_offset);
    index = resource_id & 0x00ffffff;
    if (index >= table->count)
        return 0;
    return (const struct ArchiveStreamInfo *)
        ((const unsigned char *)table + table->entries[index].offset);
}
