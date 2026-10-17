#include "game/archive.h"
#include "game/archive_resource.h"
bool ArchiveAccess::read_resource_info(unsigned resource_id, ArchiveResourceInfo *out)
{
    bool result = func_00140e88(base.index, resource_id, out);
    if (provider != 0) {
        result |= provider->read_resource_info(resource_id, out);
    }
    return result;
}
