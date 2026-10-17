#include "game/archive_resource.h"
const char *func_001441a8(const struct ArchiveResourceReference *reference)
{
    unsigned type = reference->type;
    if (type == 0)
        return 0;
    switch (type - 0x220c) {
    case 0:
        return 0;
    case 1:
        return (const char *)reference + reference->offset;
    default:
        return 0;
    }
}
