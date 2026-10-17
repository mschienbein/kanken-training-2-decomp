#ifndef GAME_KERNEL_OWNERS_H
#define GAME_KERNEL_OWNERS_H

struct EventOwner;
struct EventPart28;
struct EventPart40;
struct LargeEventOwner;
struct WaitableHandle;

#include "game/waitable_handle.h"
#include "ctr/svc.h"

struct ScopedKernelHandle {
    CtrHandle handle;

    __forceinline ~ScopedKernelHandle()
    {
        if (handle != 0) {
            svcCloseHandle(handle);
            handle = 0;
        }
    }
};

struct DualHandleState {
    ScopedKernelHandle first;
    ScopedKernelHandle second;
    unsigned int reserved08[2];
    int state;
};


extern "C" DualHandleState *func_001c58d0(DualHandleState *object);

#ifdef __cplusplus
struct EventOwner {
    struct WaitableHandle state;

    __forceinline ~EventOwner()
    {
        func_0019df84(&state);
        if (state.handle != 0) {
            svcCloseHandle(state.handle);
            state.handle = 0;
        }
    }
};
#endif

#ifdef __cplusplus
struct EventPart28 {
    unsigned char reserved[0x18];
    /* [INFERENCE] callee identity: inlined by armlink. */
    ~EventPart28();
};
#endif

#ifdef __cplusplus
struct EventPart40 {
    unsigned char reserved[0x4dc - 0x40];
    /* [INFERENCE] callee identity: inlined by armlink. */
    ~EventPart40();
};
#endif

#ifdef __cplusplus
struct LargeEventOwner {
    ~LargeEventOwner();
    unsigned char reserved0000[0x28];
    EventPart28 part28;
    EventPart40 part40;
    ScopedKernelHandle handle04dc;
    unsigned char reserved04e0[0x5308 - 0x4e0];
    EventOwner event;
};
#endif

#endif
