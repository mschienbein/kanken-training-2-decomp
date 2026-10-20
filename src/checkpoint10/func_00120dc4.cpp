#include "game/filesystem.h"

static inline int error_level(const Result &result)
{
    return result.value & 0x80000000;
}

Result func_00120dc4(const char *path)
{
    FilesystemGlobals *globals = &data_00242c18;
    if (globals->current != 0)
        return Result(0xc92044e7);
    MountedDevice *device;
    Result result = func_0012e468(&device, 4);
    if (error_level(result) >= 0) {
        result = func_0019281c(path, device, 0, 0);
        if (error_level(result) >= 0)
            globals->current = device;
        else
            device->vtable->dispose(device);
    }
    return result.value;
}
