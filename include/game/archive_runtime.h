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

#ifdef __cplusplus
extern "C" {
#endif
extern void func_001d4174(struct SoundArchiveRequestState *state,
    struct ArchiveHandle *archive);
#ifdef __cplusplus
}
#endif

#endif
