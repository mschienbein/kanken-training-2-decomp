#ifndef GAME_FILESYSTEM_H
#define GAME_FILESYSTEM_H

#include "ctr/svc.h"

struct FilesystemFactoryPool;
struct MountedDevice;

/* [INFERENCE] Field roles. Initializer0012e29c publishes factory pool004b1cb0
 * at+4; the file-session and mounted-device pools are separate objects. */
struct FilesystemGlobals {
    unsigned char active, latched;
    unsigned char reserved02[2];
    struct FilesystemFactoryPool *pool;
    struct MountedDevice *current;
    volatile unsigned guard;
    CtrHandle session;
    unsigned reserved14;
    long long interval;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct FilesystemGlobals data_00242c18;
#ifdef __cplusplus
}
#endif

#endif
