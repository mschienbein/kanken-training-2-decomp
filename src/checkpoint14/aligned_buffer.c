#include <stdint.h>
#include "game/linear_buffer.h"

static uintptr_t align_up(uintptr_t value, unsigned alignment)
{
    return (value + alignment - 1) & ~(uintptr_t)(alignment - 1);
}

/* [INFERENCE] State produced by trimming an input span. */
struct LinearBufferState {
    unsigned used;
    unsigned capacity;
    unsigned char enabled;
};

static void apply_buffer_state(struct AlignedBuffer *buffer, const struct LinearBufferState *state)
{
    buffer->size = state->capacity;
    buffer->enabled = state->enabled;
    buffer->used = state->used;
}

static void initialize_linear_buffer(struct AlignedBuffer *buffer,
    unsigned char *memory, unsigned size, unsigned alignment,
    unsigned granularity, unsigned char enabled)
{
    unsigned char *begin = (unsigned char *)align_up((uintptr_t)memory, alignment);
    unsigned remaining;
    struct LinearBufferState state;
    buffer->begin = begin;
    remaining = (unsigned)(memory + size - begin);
    state.used = 0;
    state.capacity = remaining & ~(granularity - 1);
    state.enabled = enabled;
    apply_buffer_state(buffer, &state);
}

void func_00122360(struct AlignedBuffer *buffer, unsigned char *memory,
                   unsigned size, unsigned char enabled)
{
    initialize_linear_buffer(buffer, memory, size, 4, 8, enabled);
}

