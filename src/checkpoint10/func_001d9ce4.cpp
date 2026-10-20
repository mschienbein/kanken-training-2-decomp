#include "game/cleanup_registry.h"
#include "game/stream_manager.h"

static __forceinline void *allocate_locked(struct AsyncHeap *heap, unsigned int size,
    CleanupCallback complete, void *context)
{
    StreamLockGuard guard(&heap->mutex);
    void *memory;
    memory = func_001d995c(&heap->allocator, size, complete, context);
    return memory;
}

extern "C" void *func_001d9ce4(struct AsyncHeap *heap, unsigned int size)
{
    StreamLockGuard guard(&heap->mutex);
    void *memory;
    memory = allocate_locked(heap, size, func_001d9ca0, 0);
    return memory;
}
