#include "game/archive.h"

unsigned func_00140c50(const struct ArchiveSoundInfo *info)
{
    unsigned value;
    if (func_0018279c(info->parameters, &value, 0))
        return value;
    return 0;
}
