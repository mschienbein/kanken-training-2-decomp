#include "game/file_session.h"

extern "C" CtrResult func_001c4d9c(FileService *service,
                                  FileSessionObject **output, CtrHandle handle)
{
    FileSessionObject *object = func_00192978(&bss_004b1d28);
    if (object != 0) {
        object->vtable = &data_00223af4;
        object->handle = handle;
        *output = object;
        return 0;
    }
    svcCloseHandle(handle);
    return (CtrResult)0xd8604659u;
}
