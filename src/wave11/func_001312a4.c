#include "game/audio_tasks.h"
#include "game/exp_heap.h"

static __forceinline void release_guard(struct StreamLockGuard *guard)
{
    func_00195d74(guard->mutex);
}

void func_001312a4(struct ObjectManager *manager)
{
    struct StreamLockGuard guard;
    guard.mutex = &manager->lock;
    func_00192bd8(guard.mutex);
    manager->samples = 0;
    manager->capacity = 0;
    manager->count = 0;
    release_guard(&guard);
}
