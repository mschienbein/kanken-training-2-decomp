#include "game/bss_0049ecf0.h"
#include "game/material.h"

#include "game/command_buffer.h"

static struct CommandQueue *writable_queue(struct CommandQueue *queue)
{
    if (!queue)
        return 0;
    if (queue->base)
        return queue;
    if (queue->commands)
        return queue;
    return 0;
}

void func_0019c920(void)
{
    struct CommandQueue *queue = writable_queue(bss_0049ecf0.writing);
    if (queue) {
        func_0019cecc(queue->commands, queue->count * sizeof(*queue->commands));
        queue->offset = 0;
        queue->committed_bytes = 0;
        queue->count = 0;
        queue->cursor = 0;
        queue->committed_offset = 0;
        queue->completed_count = 0;
        queue->patch = 0;
        queue->captured_size = 0;
        data_00242fc8 = queue->base;
        data_00242fcc = data_00242fc8 + queue->capacity;
    }
}
