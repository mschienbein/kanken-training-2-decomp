#include <stddef.h>
#include "ctr/svc.h"

struct ThreadState {
    unsigned char reserved00[0x14];
    CtrHandle handle;
    unsigned char active;
};

extern void func_0018f6c8(struct ThreadState *thread);

struct ThreadState *func_001c51a4(struct ThreadState *object)
{
    CtrHandle *handle;
    func_0018f6c8(object);
    handle = &object->handle;
    if (*handle != 0) {
        svcCloseHandle(*handle);
        *handle = 0;
    }
    return (struct ThreadState *)((unsigned char *)handle -
                                  offsetof(struct ThreadState, handle));
}
