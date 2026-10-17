#include "game/result.h"

/* INTEGRATOR: move to include/game/filesystem_mount.h */
extern "C" {
Result func_0010bcdc(const char *name, int max_files, int max_directories,
                     void *buffer, unsigned size, bool use_cache);
}

extern "C" Result func_00106ec4(int max_files, int max_directories, void *buffer,
                                unsigned size, bool use_cache)
{
    return func_0010bcdc("rom:", max_files, max_directories, buffer, size, use_cache);
}
