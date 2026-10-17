#include "game/audio_tasks.h"

struct StreamLockGuard { struct StreamMutex *mutex; };
static __forceinline void release_guard(struct StreamLockGuard *guard)
{
    func_00195d74(guard->mutex);
}

void func_001312a4(struct ObjectManager *manager)
{
    struct StreamLockGuard guard;
    guard.mutex = &manager->lock;
    func_00192bd8(guard.mutex);
    manager->field1b8 = 0;
    manager->field1bc = 0;
    manager->field1c0 = 0;
    release_guard(&guard);
}
