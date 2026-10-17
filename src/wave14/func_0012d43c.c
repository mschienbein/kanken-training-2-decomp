#include "game/exp_heap.h"
#include "game/service_availability.h"
#include "game/service_result.h"
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

int func_0012d43c(void)
{
    int active;
    struct StreamLockGuard guard;
    acquire_guard(&guard, &bss_0049ee70);
    active = data_00242ffc != 0;
    release_guard(&guard);
    return active;
}
