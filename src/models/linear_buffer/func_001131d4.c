#include "game/linear_buffer.h"

struct LinearBuffer *func_001131d4(struct LinearBuffer *owner,
    struct BufferAllocator *allocator, void *buffer, unsigned capacity)
{
    /* [INFERENCE] callee identity: inlined by armlink and removed. */
    owner = InitializeLinearBufferAllocator(owner, allocator);
    owner->position = 0;
    owner->capacity = capacity;
    owner->buffer = buffer;
    return owner;
}
