#ifndef GAME_KERNEL_OWNERS_H
#define GAME_KERNEL_OWNERS_H
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
#endif
