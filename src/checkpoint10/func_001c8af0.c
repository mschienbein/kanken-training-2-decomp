#include "game/functions.h"
#include "game/binary_resource.h"



static struct BinaryResourceHeader *ResolveRelative(int *relative)
{
    void *object = 0;
    if (*relative != 0)
        object = (unsigned char *)relative + *relative;
    return object;
}

void func_001c8af0(struct RelativeBinaryResource *owner)
{
    struct BinaryResourceReference reference;
    owner->state = 0;
    reference.resource = ResolveRelative(&owner->offset);
    if (reference.resource != 0)
        func_001840dc(&reference);
}
