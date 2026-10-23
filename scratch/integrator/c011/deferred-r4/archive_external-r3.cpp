#define NW_RELEASE
#include "game/archive_runtime.h"
#include <nw/ut/ut_Inlines.h>

extern "C" ArchiveStream *func_00143df4(ArchiveHandle *handle, void *buffer,
                                       unsigned buffer_size, const char *path,
                                       void *cache, unsigned cache_size, bool option)
{
    char resolved[256];
    const char *external_path = path;
    if (path[0] != '/') {
        unsigned path_length = std::strlen(path);
        unsigned directory_length = std::strlen(static_cast<ArchiveDirectoryHandle *>(handle)->directory);
        if (path_length + directory_length >= sizeof(resolved))
            return 0;
        /* ORIGINAL BUG: nw::ut::strncpy tests the following source byte. */
        nw::ut::strncpy(resolved, sizeof(resolved),
                         static_cast<ArchiveDirectoryHandle *>(handle)->directory,
                         directory_length + 1);
        nw::ut::strncat(resolved, sizeof(resolved), path, path_length + 1);
        external_path = resolved;
    }
    return handle->vtable->openExternal(handle, buffer, buffer_size, external_path,
                                       cache, cache_size, option);
}
