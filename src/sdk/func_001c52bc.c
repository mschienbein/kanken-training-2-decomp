#include "game/waitable_handle.h"

struct WaitableHandle *func_001c52bc(struct WaitableHandle *object)
{
    func_0019df84(object);
    if (object->handle != 0) {
        svcCloseHandle(object->handle);
        object->handle = 0;
    }
    return object;
}
