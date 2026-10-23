#ifndef GAME_FILESYSTEM_H
#define GAME_FILESYSTEM_H

#include <stdbool.h>
#include "game/result.h"
#include "game/service_state.h"

/* [INFERENCE] Only the mounted-device disposal slot and actual session/archive
 * prefix are recovered; no complete native filesystem class identity is claimed. */
struct MountedDevice;
struct MountedDeviceVTable {
    const void *reserved00[12];
    void (*dispose)(struct MountedDevice *device);
};
struct MountedDevice {
    const struct MountedDeviceVTable *vtable;
    unsigned session;
    unsigned long long archive;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct Result func_0012e468(struct MountedDevice **device, unsigned archive_id);
extern struct Result func_0019281c(const char *path, struct MountedDevice *device,
                                  unsigned char flags, unsigned char external);
extern struct Result func_00120dc4(const char *path);
extern struct Result func_00120f44(const char *name, unsigned archive_flags,
    unsigned extra_flags, unsigned mode, int max_files, int max_directories,
    void *buffer, unsigned size, bool use_cache);
extern struct Result func_00113254(const char *name, unsigned archive_flags,
    unsigned mode, int max_files, int max_directories,
    void *buffer, unsigned size, bool use_cache);
extern struct Result func_0010bcdc(const char *name, int max_files,
    int max_directories, void *buffer, unsigned size, bool use_cache);
extern struct Result func_00106ec4(int max_files, int max_directories,
    void *buffer, unsigned size, bool use_cache);
#ifdef __cplusplus
}
#endif

#endif
