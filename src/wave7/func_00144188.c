#include "game/archive.h"

unsigned func_00144188(const struct ArchiveFileInfo *info)
{
    unsigned value;
    if (func_0018279c(info->parameters, &value, 1))
        return value;
    return 0;
}
