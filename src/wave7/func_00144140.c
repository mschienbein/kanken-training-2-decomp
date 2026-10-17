#include "game/archive.h"

const struct ArchiveFileInfo *func_00144140(const struct ArchiveResourceTable *table, unsigned resource_id)
{
    const struct ArchiveFileDirectory *directory;
    if (resource_id >> 24 != 5)
        return 0;
    resource_id &= 0xffffff;
    directory = (const struct ArchiveFileDirectory *)((const char *)table + table->file_table_offset);
    if (directory->count <= resource_id)
        return 0;
    return (const struct ArchiveFileInfo *)((const char *)directory + directory->entries[resource_id].offset);
}
