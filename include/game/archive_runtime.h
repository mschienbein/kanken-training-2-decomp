#ifndef GAME_ARCHIVE_RUNTIME_H
#define GAME_ARCHIVE_RUNTIME_H
#include "game/archive.h"

struct FileInputStream {
    unsigned char reserved00[0x3c];
    int position;
    int start;
    int size;
};
struct MemoryInputStream {
    unsigned char reserved00[0x14];
    const void *buffer;
    int size;
    int position;
};
struct SoundArchiveRequestState {
    unsigned pending;
    struct ArchiveHandle *archive;
};

/* [INFERENCE] External stream and directory-bearing archive roles. The
 * existing ArchiveHandle prefix stays unchanged; this is not an alias for the
 * layout-incompatible older native nw::snd::SoundArchive. Uncalled vtable
 * slots retain their unrecovered roles rather than invented parameter lists. */
struct ArchiveStream;
struct ArchiveHandleVTable {
    void (*reserved[5])(void);
    struct ArchiveStream *(*openExternal)(struct ArchiveHandle *handle,
        void *buffer, unsigned buffer_size, const char *path,
        void *cache, unsigned cache_size, bool option);
};
struct ArchiveDirectoryHandle
#ifdef __cplusplus
    : ArchiveHandle
#endif
{
#ifndef __cplusplus
    struct ArchiveHandle base;
#endif
    void *override_source;
    char directory[256];
};
#ifdef __cplusplus
extern "C" {
#endif
struct ArchiveStream *func_00143df4(struct ArchiveHandle *handle, void *buffer,
    unsigned buffer_size, const char *path, void *cache, unsigned cache_size,
    bool option);
#ifdef __cplusplus
}
#endif
#endif
