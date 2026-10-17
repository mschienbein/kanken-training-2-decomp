#include "game/archive.h"

int func_00140d1c(const struct ArchiveStreamInfo *info)
{
    unsigned value;
    if (func_0018279c(info->parameters, &value, 17))
        return value & 1;
    return 0;
}
