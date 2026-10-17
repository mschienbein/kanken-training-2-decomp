#include "game/input_runtime.h"
#include "game/exp_heap.h"
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

struct ChannelWorkspace;
struct ExtendedWorkspace;
struct InputSessionContext;

extern struct InputSessionContext bss_004b97ac;
extern void func_0017e8fc(struct ChannelWorkspace *workspace);
extern void func_0017e8bc(struct ExtendedWorkspace *workspace);
extern void func_0017e858(struct InputSessionContext *coordinator, unsigned index, unsigned char enabled);

void func_0014c930(struct ServiceOutput *owner, signed char index)
{
    struct StreamLockGuard guard;
    acquire_guard(&guard, &owner->mutex);
    if (owner->workspaces[index].channels)
        func_0017e8fc(owner->workspaces[index].channels);
    if (owner->workspaces[index].extended)
        func_0017e8bc(owner->workspaces[index].extended);
    owner->workspaces[index].channels = 0;
    owner->workspaces[index].extended = 0;
    func_0017e858(&bss_004b97ac, index, 0);
    release_guard(&guard);
}

