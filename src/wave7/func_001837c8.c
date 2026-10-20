#include "game/stream_manager.h"


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

int func_001837c8(struct StreamQueue *queue, struct StreamEvent **output)
{
    struct StreamLockGuard guard;
    acquire_guard(&guard, &queue->mutex);
    if (queue->count > 0) {
        *output = queue->slots[queue->read_index];
        queue->read_index = (queue->read_index + 1) % queue->capacity;
        --queue->count;
        if (queue->waiting_writers > 0)
            func_00181280(&queue->available, 1);
        release_guard(&guard);
        return 1;
    }
    release_guard(&guard);
    return 0;
}
