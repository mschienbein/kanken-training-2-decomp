#include "game/linear_buffer.h"

static struct LinearBuffer *initialize_buffer_storage(struct LinearBuffer *owner,
    unsigned capacity, void *buffer)
{
    owner->position = 0;
    owner->buffer = buffer;
    owner->capacity = capacity;
    return owner;
}

struct LinearBuffer *func_00112628(struct LinearBuffer *owner,
    struct BufferAllocator *allocator, void *buffer, unsigned capacity)
{
    /* [INFERENCE] Only the first-field store is witnessed, not the identity
     * of a removed base-initializer call. */
    return initialize_buffer_storage(InitializeLinearBufferAllocator(owner, allocator),
        capacity, buffer);
}
