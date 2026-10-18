#include "game/linear_buffer.h"

/* [INFERENCE] Field-initializer identity is not recovered: an ordinary out-of-line
 * base initializer could have been inlined by armlink and then removed. */
struct LinearBuffer *InitializeLinearBufferAllocator(
    struct LinearBuffer *owner, struct BufferAllocator *allocator)
{
    owner->allocator = allocator;
    return owner;
}
