#include "game/archive.h"

extern "C" bool func_001f2894(const ResourceBlock *block, const ArchiveFile *file)
{
    (void)block;
    if (file->signature != 0x4d545343u)
        return false;
    unsigned version = file->version;
    bool valid = false;
    if ((unsigned)(version - 0x02000000u) <= 0x00030100u)
        valid = true;
    return valid;
}
