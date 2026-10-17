#include "game/stream_manager.h"

void func_00146ff8(struct StreamManager *arena, void *storage, unsigned int size)
{
    arena->pending_head = 0;
    arena->pending_tail = 0;
    arena->serial = 0;
    arena->write_index = 0;
    arena->read_index = 0;
    arena->full = 0;
    arena->buffer = storage;
    arena->capacity = size >> 2;
    func_00180884(&arena->requests, arena->request_storage, 0x20);
    func_00180884(&arena->completions, arena->completion_storage, 0x21);
    arena->initialized = 1;
}
