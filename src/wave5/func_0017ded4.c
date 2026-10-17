#include "game/archive.h"

const void *func_0017ded4(const struct RelativeReference *reference)
{
    if (reference->type != 0x4100)
        return 0;
    return (const char *)reference + reference->offset;
}
