#include "game/archive.h"

bool func_00140c98(const struct ArchiveSoundInfo *info)
{
    unsigned value;
    if (!func_0018279c(info->parameters, &value, 1))
        return true;
    return (unsigned char)(value >> 8) != 0;
}
