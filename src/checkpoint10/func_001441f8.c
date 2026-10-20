#include "game/archive_resource.h"

const struct ArchiveInternalFile *func_001441f8(const struct ArchiveResourceReference *reference)
{
    unsigned type = reference->type;
    if (type == 0)
        return 0;
    type -= 0x220c;
    if (type == 0)
        return (const struct ArchiveInternalFile *)((const unsigned char *)reference + reference->offset);
    return 0;
}
