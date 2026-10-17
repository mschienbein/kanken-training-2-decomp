#include <stddef.h>
#include "game/file_session.h"

extern "C" FileSessionObject *func_001c4d1c(FileSessionObject *object)
{
    CtrHandle *handle = &object->handle;
    if (*handle != 0) {
        svcCloseHandle(*handle);
        *handle = 0;
    }
    return (FileSessionObject *)((unsigned char *)handle -
                                  offsetof(FileSessionObject, handle));
}
