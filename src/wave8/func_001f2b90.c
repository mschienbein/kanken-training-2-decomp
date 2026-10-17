#include "game/archive.h"

extern unsigned func_001f2b98(struct ArchiveIndex *index);

unsigned func_001f2b90(struct ArchiveHandle *handle)
{
    return func_001f2b98(handle->index);
}
