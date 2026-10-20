#include "game/functions.h"
#include <stdint.h>

#include "game/binary_resource.h"


void func_001c8b1c(struct RelativeBinaryResource *object)
{
    struct BinaryResourceReference reference;
    int *relative = &object->offset;
    reference.resource = *relative != 0
        ? (struct BinaryResourceHeader *)((uintptr_t)relative + *relative) : 0;
    if (reference.resource != 0)
        func_001840dc(&reference);
}
