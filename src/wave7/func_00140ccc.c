#include "game/archive.h"

unsigned char func_00140ccc(const struct ArchiveStreamInfo *info)
{
    unsigned value;
    if (func_0018279c(info->parameters, &value, 1))
        return (unsigned char)value;
    return 0;
}
