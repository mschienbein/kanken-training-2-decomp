#include "game/result.h"

struct MountedDevice;
struct MountedDeviceVTable {
    const void *reserved00[12];
    void (*dispose)(MountedDevice *device);
};
struct MountedDevice {
    const MountedDeviceVTable *vtable;
    unsigned session;
    unsigned long long archive;
};
struct FilesystemGlobals {
    void *allocator;
    void *reserved04;
    MountedDevice *current;
    volatile unsigned guard;
    unsigned session;
};

/* INTEGRATOR: move to include/game/filesystem.h */
extern "C" {
extern FilesystemGlobals data_00242c18;
extern Result func_0012e468(MountedDevice **device, unsigned archive_id);
extern Result func_0019281c(const char *path, MountedDevice *device,
                          unsigned char flags, unsigned char external);
Result func_00120dc4(const char *path);
}

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
