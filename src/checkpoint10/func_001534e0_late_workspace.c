#include "game/service_runtime.h"
#include "game/functions.h"
#include "game/input_runtime.h"
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


static __forceinline void release_workspace(struct ServiceOutput *owner, unsigned index)
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

int func_001534e0(struct ServiceOutput *owner, unsigned index, struct ExtendedWorkspace *workspace)
{
    struct StreamLockGuard guard;
    struct ExtendedWorkspace *new_workspace;
    if (!workspace)
        return 0;
    new_workspace = workspace;
    release_workspace(owner, index);
    acquire_guard(&guard, &owner->mutex);
    owner->workspaces[index].extended = new_workspace;
    func_001544e0(new_workspace);
    func_0017e858(&bss_004b97ac, index, 1);
    release_guard(&guard);
    return 1;
}
