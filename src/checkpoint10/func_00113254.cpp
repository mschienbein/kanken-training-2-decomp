#include "game/result.h"

/* INTEGRATOR: move to include/game/filesystem_mount.h */
extern "C" {
Result func_00120f44(const char *name, unsigned archive_flags, unsigned extra_flags,
                     unsigned mode, int max_files, int max_directories,
                     void *buffer, unsigned size, bool use_cache);
}

extern "C" Result func_00113254(const char *name, unsigned archive_flags,
                                unsigned mode, int max_files, int max_directories,
                                void *buffer, unsigned size, bool use_cache)
{
    return func_00120f44(name, archive_flags, 0, mode, max_files,
                         max_directories, buffer, size, use_cache);
}
