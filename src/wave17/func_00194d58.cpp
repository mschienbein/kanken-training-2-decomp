#include "game/result.h"
#include "game/file_handle.h"



extern "C" void func_00194d58(FileHandle *handle)
{
    if ((unsigned)handle->object | handle->reserved) {
        func_0018fdd4(*handle);
        handle->object = 0;
        handle->reserved = 0;
    }
}
