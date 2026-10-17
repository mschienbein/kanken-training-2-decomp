#include "game/stream_manager.h"

struct AsyncAllocatorState {
    void *heap;
    unsigned int reserved04[2];
    void *owner;
};
struct AsyncHeap {
    unsigned int reserved00;
    struct StreamMutex mutex;
    struct AsyncAllocatorState allocator;
};
/* INTEGRATOR: move to include/game/async_heap.h */
extern "C" {
extern unsigned func_001d9ca0(void *memory, void *context);
extern void *func_001d995c(struct AsyncAllocatorState *allocator, unsigned int size,
                          unsigned (*complete)(void *, void *), void *context);
}

class ScopedStreamLock {
    StreamMutex *mutex;
public:
    explicit ScopedStreamLock(StreamMutex *value) : mutex(value) { func_00192bd8(mutex); }
    ~ScopedStreamLock() { func_00195d74(mutex); }
};
static __forceinline void *allocate_locked(struct AsyncHeap *heap, unsigned int size,
    unsigned (*complete)(void *, void *), void *context)
{
    ScopedStreamLock guard(&heap->mutex);
    void *memory;
    memory = func_001d995c(&heap->allocator, size, complete, context);
    return memory;
}

extern "C" void *func_001d9ce4(struct AsyncHeap *heap, unsigned int size)
{
    ScopedStreamLock guard(&heap->mutex);
    void *memory;
    memory = allocate_locked(heap, size, func_001d9ca0, 0);
    return memory;
}
