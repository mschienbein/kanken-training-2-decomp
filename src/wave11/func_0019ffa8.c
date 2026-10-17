#include "game/exp_heap.h"
static __forceinline void acquire_guard(struct StreamLockGuard *guard,
                                       struct StreamMutex *mutex)
{
    guard->mutex = mutex;
    func_00192bd8(guard->mutex);
}

static __forceinline void release_guard(struct StreamLockGuard *guard)
{
    func_00195d74(guard->mutex);
}

static __forceinline void *allocate_locked(struct LockedExpHeap *heap,
                                           unsigned size, int group,
                                           int alignment, int direction, int mode)
{
    struct StreamLockGuard guard;
    void *allocation;
    acquire_guard(&guard, &heap->mutex);
    allocation = func_00195104(&heap->base, size, alignment, group, direction, mode);
    release_guard(&guard);
    return allocation;
}

void *func_0019ffa8(unsigned size)
{
    return allocate_locked(data_00242c54, size, 0, 4, 0, 0);
}
