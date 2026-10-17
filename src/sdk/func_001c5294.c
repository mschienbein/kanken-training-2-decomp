#include "ctr/svc.h"

struct KernelHandleOwner {
    CtrHandle handle;
};

struct KernelHandleOwner *func_001c5294(struct KernelHandleOwner *object)
{
    if (object->handle != 0) {
        svcCloseHandle(object->handle);
        object->handle = 0;
    }
    return object;
}
