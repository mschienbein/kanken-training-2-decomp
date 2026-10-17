#include "game/stream_manager.h"

struct StreamLockGuard {
    struct StreamMutex *mutex;
};

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
struct WorkspaceCoordinator;

struct WorkspacePair {
    struct ChannelWorkspace *channels;
    struct ExtendedWorkspace *extended;
};

struct WorkspaceOwner {
    unsigned char reserved00[0x34];
    struct WorkspacePair slots[2];
    struct StreamMutex mutex;
};

extern struct WorkspaceCoordinator bss_004b97ac;
extern void func_0017e8fc(struct ChannelWorkspace *workspace);
extern void func_0017e8bc(struct ExtendedWorkspace *workspace);
extern void func_0017e858(struct WorkspaceCoordinator *coordinator, unsigned index, unsigned char enabled);
extern int func_001544e0(struct ExtendedWorkspace *workspace);

static __forceinline void release_workspace(struct WorkspaceOwner *owner, unsigned index)
{
    struct StreamLockGuard guard;
    acquire_guard(&guard, &owner->mutex);
    if (owner->slots[index].channels)
        func_0017e8fc(owner->slots[index].channels);
    if (owner->slots[index].extended)
        func_0017e8bc(owner->slots[index].extended);
    owner->slots[index].channels = 0;
    owner->slots[index].extended = 0;
    func_0017e858(&bss_004b97ac, index, 0);
    release_guard(&guard);
}

int func_001534e0(struct WorkspaceOwner *owner, unsigned index, struct ExtendedWorkspace *workspace)
{
    struct StreamLockGuard guard;
    struct ExtendedWorkspace *new_workspace;
    if (!workspace)
        return 0;
    new_workspace = workspace;
    release_workspace(owner, index);
    acquire_guard(&guard, &owner->mutex);
    owner->slots[index].extended = new_workspace;
    func_001544e0(new_workspace);
    func_0017e858(&bss_004b97ac, index, 1);
    release_guard(&guard);
    return 1;
}
