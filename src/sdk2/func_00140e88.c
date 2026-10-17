#include "game/archive_resource.h"
int func_00140e88(const struct ArchiveIndex *index, unsigned resource_id, struct ArchiveResourceInfo *out)
{
    const struct ArchiveFileInfo *info = func_00144140(index->table, resource_id);
    if (info == 0)
        return 0;
    out->file_id = info->file_id;
    out->size = func_00144188(info);
    out->external = info->external;
    return 1;
}
