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

static __forceinline int has_references(void)
{
    struct StreamLockGuard guard;
    int active;
    acquire_guard(&guard, &bss_0049ee70);
    active = data_00242a1c.references != 0;
    release_guard(&guard);
    return active;
}

int func_00186538(void)
{
    int active = has_references();
    signed char available;
    if (active || func_0012d43c()) {
        available = 0;
        func_0013622c(0, &available);
        return available;
    }
    return 0;
}
