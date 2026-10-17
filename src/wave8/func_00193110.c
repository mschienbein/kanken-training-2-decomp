#include "game/runtime.h"

struct W8AllocationTag { unsigned char reserved; };

/* [INFERENCE] The unused stack object serves as a nothrow-style allocation tag. */

void *func_00193110(unsigned size, const char *tag)
{
    struct W8AllocationTag context;
    (void)tag;
    return func_0019ffa4(size, &context);
}
