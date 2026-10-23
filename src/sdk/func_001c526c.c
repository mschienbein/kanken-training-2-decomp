#include "game/functions.h"
#include "ctr/svc.h"

struct KernelHandleOwner {
    CtrHandle handle;
};

void *func_001c526c(void *storage)
{
    struct KernelHandleOwner *object = storage;
    if (object->handle != 0) {
        svcCloseHandle(object->handle);
        object->handle = 0;
    }
    return object;
}
